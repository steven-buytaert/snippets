// Copyright 2023 Steven Buytaert

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <getopt.h>
#include <pthread.h>
#include <inttypes.h>

static uint32_t NT = 20;          // Number of modifier threads.
static uint32_t prunepoint = 200; // Number of nodes in the list to start pruning.

// Enable when the garbage collection cycle should be done by the main
// thread when all modifier threads are idle.

static uint32_t idle_cleanup = 0;

typedef struct Node_t * node_t;

typedef struct Node_t {           // Prototype of a node.
  node_t       next;
  union {
    uint32_t   header;
    struct {
      uint32_t        :  2;
      uint32_t locked :  1;       // One bit locking flag.
      uint32_t        : 29;
    };
  };
} Node_t;

typedef struct Thr_t * thr_t;

typedef struct Thr_t {            // Modifier thread.
  pthread_t         pthread;
  uint64_t          ops;          // Operations done (adding/deleting).
  uint64_t          missed;       // Operations aborted due to lock issues.
  uint64_t          cleansed;     // Number of GC cycles/windows performed.
  uint32_t          tid;
  pthread_mutex_t   Mut;
  pthread_cond_t    Cond;
  volatile uint32_t holding;
} Thr_t;

static uint32_t mrand(uint32_t mod) {
  return (uint32_t) rand() % (mod ? mod : 1);
}

static void atomic_inc64(uint64_t * where) {
  __atomic_add_fetch(where, 1, __ATOMIC_SEQ_CST);
}

static void atomic_inc(uint32_t * where) {
  __atomic_add_fetch(where, 1, __ATOMIC_SEQ_CST);
}

static void atomic_dec(uint32_t * where) {
  __atomic_sub_fetch(where, 1, __ATOMIC_SEQ_CST);
}

#define CAX(P, X, D) __atomic_compare_exchange(P, X, D, 0, __ATOMIC_SEQ_CST, __ATOMIC_ACQUIRE)

static uint32_t trylock(node_t node) {                      // Try locking a node; returns non zero when locked.

  Node_t Exp = { .header = node->header };
  Node_t Des = { .header = node->header };

  Exp.locked = 0;
  Des.locked = 1;

  return CAX(& node->header, & Exp.header, & Des.header);

}

static void unlock(node_t node) {                           // Unlock a locked node.

  Node_t Exp = { .header = node->header };
  Node_t Des = { .header = node->header };

  assert(node->locked);                                     // Once locked, it must remain locked.

  do {
    Exp.locked = 1;
    Des.locked = 0;
  } while (! CAX(& node->header, & Exp.header, & Des.header));

}

static volatile uint32_t canyield = 0;                      // Flipped by controller thread every now and then.

static void couldyield(thr_t thr) {                         // Called at various points to enforce thread switching beyond normal.

  if (canyield && 0 == mrand(400)) {                        // Every so many calls and if allowed, give up cpu.
    sched_yield();
  }

}

#define _C_ couldyield(t);

static uint32_t numnodes = 0;                               // Total number of nodes in the list.

static Node_t Head = {                                      // List header pseudo node.
  .locked = 0,
  .next = NULL,
};

static node_t cooling = NULL;                               // List with deleted nodes but still potentially used for navigation.

static node_t zombies = NULL;                               // List with nodes to be physically free'd next GC window.

static volatile uint64_t freed = 0;                         // Total freed nodes; is incremented without locks/atomicity.

static uint64_t added = 0;                                  // Total number of nodes ever added to the list; only grows.

static volatile uint32_t windowmissed = 0;                  // Number of times we could not properly get the cooling list in the window.

static volatile uint32_t hold = 0;                          // When non zero, all modifier threads should hold their horses.

static uint32_t otc = 0;                                    // Ongoing transaction count.

static void add2cooling(node_t node) {                      // Insert node atomically at start of the cooling list.

  assert(node->locked);                                     // Node must remain locked during cooling.

  assert(node != & Head);
  
  do {
    node->next = cooling;                                   // Node is added at the start!
  } while (! CAX(& cooling, & node->next, & node));

}

static void txstarts(thr_t t) {                             // Called at the start of any list transaction: navigation/manipulation.
  atomic_inc(& otc);
}

static void cleanup(node_t nodes2free) {                    // Release the given list of nodes.

  node_t n;

  while (nodes2free) {
    n = nodes2free;
    nodes2free = nodes2free->next;
    n->next = (node_t) 0xcafebabe;                          // Ensure navigating the node segfaults.
    assert(n->locked);
    n->locked = 0;                                          // Node is no longer locked.
    free(n);
    freed++;                                                // Here we can update total number freed race free.
  }

}

static void txstops(thr_t t) {                              // Called at the end of any list transaction operation.

  uint32_t locked = 0xffffff;                               // To lock the cleanup window to this thread.
  uint32_t expected = 1;

  if (CAX(& otc, & expected, & locked)) {
    static uint32_t homealone = 0;

    assert(0 == homealone++);                               // Only one thread allowed in the window code.
    
    if (! idle_cleanup) {
      t->cleansed++;
_C_   node_t n2free = cooling;
_C_   node_t empty = NULL;
_C_   if (CAX(& cooling, & n2free, & empty)) {
        cleanup(zombies);                                   // Cleanup the cooling list from previous window.
        zombies = n2free;                                   // Current cooling list will be deleted next time.
      }
      else {
        windowmissed++;
      }
    }

    assert(0 == --homealone);

_C_ __atomic_sub_fetch(& otc, locked, __ATOMIC_SEQ_CST);    // Release cleanup window lock.

  }
  else {
    atomic_dec(& otc);
  }

}

volatile uint32_t also_remove = 100;                        // Can be changed by controller thread to prune the list.

static void * tester(void * arg) {

  thr_t    t = arg;
  uint32_t i;
  node_t   node;
  uint32_t where;
  node_t   left;
  node_t   right = NULL;

  while (1) {
    pthread_mutex_lock(& t->Mut);
    while (hold) {
      t->holding = 1;                                       // ACK we're holding.
      pthread_cond_wait(& t->Cond, & t->Mut);
    }
    t->holding = 0;
    pthread_mutex_unlock(& t->Mut);

    i = mrand(15);
    if (0 == i) {                                           // --- Try to insert a new node.
      node = malloc(sizeof(Node_t)); assert(node);
      memset(node, 0x00, sizeof(Node_t));
_C_   txstarts(t);                                          // Transaction start.
      where = mrand(numnodes);                              // Where to insert the node.
      left = & Head;

      for (i = 0; i < where; i++) {
_C_     if (! left) break;                                  // We absolutely need a left node.
_C_     left = left->next;
      }
      if (left) {                                           // Only continue if we found a proper left node.
_C_     if (trylock(left)) {
          right = left->next;                               // Assign when left has been locked.
_C_       if (right && ! trylock(right)) {                  // There's a right node, but we can't lock it.
_C_         unlock(left);
            free(node);
            t->missed++;
          }
          else {                                            // We have locked left and either no right or a locked right node.
            node->locked = 1;                               // Simple lock claim here on the new node.
            node->next = right;                             // Node not yet visible; set right first.
_C_         left->next = node;                              // Link node in; node is now visible to all threads.
            atomic_inc(& numnodes);
            atomic_inc64(& added);
_C_         unlock(left);
_C_         unlock(node);
_C_         if (right) { unlock(right); }
            t->ops++;
          }
        }
        else {                                              // Could not lock left.
          t->missed++;
          free(node);
        }
      }
      else {
        free(node);                                         // Node not added, release.
      }
_C_   txstops(t);                                           // Transaction stops.
    }
    else if (1 == i || also_remove == i) {                  // --- Try to remove a node.
      where = mrand(numnodes);                              // Where to remove a node.
_C_   txstarts(t);                                          // Transaction start.
      left = & Head;

      for (i = 0; i < where; i++) {                         // Try to find it.
_C_     if (! left) break;
_C_     left = left->next;
      }
_C_   if (left) {
_C_     if (trylock(left)) {
_C_       if (left->next) {
            node = left->next;                              // Node we'll remove; assign when left locked only.
_C_         if (! trylock(node)) {
_C_           unlock(left);
              t->missed++;
            }
            else {
              right = node->next;                           // Assign right only when node itself has been locked.
_C_           if (right && ! trylock(right)) {
_C_             unlock(left);
_C_             unlock(node);
                t->missed++;
              }
              else {                                        // Ok we have all required locks.
_C_             left->next = right;                         // Unlink the node.
                atomic_dec(& numnodes);
                if (left == & Head) {                       // We unlinked the first node.
                  assert(right || 0 == numnodes);           // If there's no right node left, numnodes must be 0.
                }
_C_             unlock(left);
_C_             if (right) { unlock(right); }
                t->ops++;
_C_             add2cooling(node);
              }
            }
          }
          else {                                            // Left has no next; empty list.
            unlock(left);
          }
        }
      }
_C_   txstops(t);
    }
  }

  return NULL;
  
}

static void initthr(thr_t t) {

  pthread_mutexattr_t Attr;

  pthread_mutexattr_init(& Attr);
  pthread_mutexattr_settype(& Attr, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(& t->Mut, & Attr);
  pthread_mutexattr_destroy(& Attr);
  pthread_cond_init(& t->Cond, NULL);

}

static const struct option Options[] = {
  { "numthreads", 1, NULL, 'n' },
  { "prune",      1, NULL, 'p' },
  { "idleclean",  0, NULL, 'i' },
  { "help",       0, NULL, 'h' },
  { NULL,         0, NULL,  0  },
};

int main(int argc, char * argv[]) {

  Thr_t    Thr[100];
  uint32_t i;
  thr_t    t;
  uint32_t pruned = 0;
  node_t   n;
  int32_t  ai;                                              // Argument index.
  char     o;

  assert(sizeof(Thr) / sizeof(Thr[0]) >= NT);

  memset(Thr, 0x00, sizeof(Thr));

  do {
    o = getopt_long(argc, argv, "hin:p:", Options, & ai);
    switch (o) {
      case 'i':
        idle_cleanup = 1;
        break;

      case 'n': 
        NT = (uint32_t) atoi(optarg);
        if (NT > 100) { NT = 100; }
        break;

      case 'p':
        prunepoint = (uint32_t) atoi(optarg);
        break;

      case 'h': case '?':
        printf("%s [options]\n", argv[0]);
        printf("--numthreads -n : number of mutator threads; default %u threads.\n", NT);
        printf("--prune      -p : prune list when more than this # elements; default %u elements.\n", prunepoint);
        printf("--idleclean  -i : GC done in idle thread; default is hot i.e. mutator threads do GC.\n");
        printf("Runs until ctrl-c.\n");
        return 1;
        break;
    }
  } while (o != -1);                                        // Process as long as there are options.
    
  for (i = 0; i < NT; i++) {
    Thr[i].tid = i + 1;
    initthr(& Thr[i]);
    pthread_create(& Thr[i].pthread, NULL, tester, & Thr[i]);
  }

  while (1) {
    sleep(1);                                               // Free run all threads.
    hold = 1;                                               // Do a check and a release.
    uint32_t wait4ack = 0;
    while (! wait4ack) {
      for (t = Thr, i = 0; i < NT; i++, t++) {
        if (! t->holding) break;
      }
      wait4ack = (i == NT) ? 1 : 0;
    }

    assert(0 == otc);

    for (t = Thr, i = 0; i < NT; i++, t++) {
      double pms = ((double) t->missed / (double) t->ops) * 100.0;
      printf("%3u ops %"PRIu64" misses %"PRIu64" %6.3f%% gcw %"PRIu64"\n",
        t->tid, t->ops, t->missed, pms, t->cleansed);
      (void) rand();
    }

    for (i = 0, n = Head.next; n; n = n->next, i++) { }     // Just count number of nodes during idle time.
    const char * gcm = "hot";                               // Garbage collection moment; hot means during modification.
    uint32_t delayed = 0;                                   // Number of nodes on cooling and zombie list.

    if (idle_cleanup) {
      cleanup(cooling);                                     // Garbage collect the nodes here during idle moment.
      cooling = NULL;
      gcm = "idle";
      assert(NULL == zombies);                              // Should never be used in idle collect.
    }
    else {                                                  // Hot GC, we need to count the cooling and zombie nodes.
      for (n = cooling; n; n = n->next) {                   // Count the cooling nodes.
        delayed++;
      }
      for (n = zombies; n; n = n->next) {                   // Count the zombie nodes.
        delayed++;
      }
    }

    printf("-- %u/%u nodes, prune > %u %u, freed %"PRIu64", added %"PRIu64" (%d) cooling %u%s %s GC.\n",
      i, numnodes, prunepoint, pruned, 
      freed, added, (int32_t) (added - freed),
      delayed, canyield ? " (chopped)" : "", gcm);

    assert(i == numnodes);                                  // The real eating of the pudding.
    assert(i == added - freed - delayed);

    if (numnodes > prunepoint && also_remove > 2) {         // Keep contention high enough by favoring remove when prune limit reached.
      also_remove = 2;
      pruned++;
    }
    else {
      also_remove = prunepoint;                             // This is outside the range of commands.
    }

    canyield = canyield ? 0 : 1;                            // Flip canyield.

    hold = 0;
    for (t = Thr, i = 0; i < NT; i++, t++) {                // Resume all idled mutator threads.
      pthread_mutex_lock(& t->Mut);
      pthread_cond_broadcast(& t->Cond);
      pthread_mutex_unlock(& t->Mut);
    }
    
  }

  return 0;

}
