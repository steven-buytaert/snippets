# Micro Locking Linked List Proof of Concept

## Intro

This small snippet of sample code tries to illustrate the following
principles for multithreaded list manipulation:

- fine grained locking: each node has its own lock bit.

- optimistic synchronization: navigating over the nodes is done without
  locking each node; just the next pointer is followed.

- hand-over-hand locking: when the proper node is found, the predecessor
  (left) node and successor (right) node is locked. If the operation is
  to delete the node, the node itself is also locked.

More info on Hand-over-Hand or HoH locking and some other sample code can be found
[here](https://www.usenix.org/system/files/login/articles/login_fall20_14_kelly.pdf).
Note that the referred to example does not use the optimistic synchronization
principle (as shown in this snippet) as it locks 2 nodes while navigating over the list.

## PoC Code

The main function of the PoC code, creates a number of threads that each will
randomly pick a function to perform, i.e. either add a new node or delete a
random node.

After launching the threads, the main thread/function will linger around and
check the list every second or so. To perform this checking, it will make
all testing threads run idle, i.e. not making any additions or doing any
deletions.

During this idle moment, the main thread will perform some assertions to
asses the soundness of the list and the PoC code.

Note that in this proof of concept code, the current number of threads (to
be configured at the top of the file) is rather high versus the size of the
linked list (to be set in the variable **prunepoint** in main). This is to
ensure we have a lot of contention between the threads.

There's also an inline macro **_C_** defined that is inserted in the list
manipulation code of the threads, to ensure that the thread will be
interrupted more than enough to proof the correctness. This is just to proof
the point of the prototype code and is not meant to be copied in a real
world application.

### Selecting an Insertion/Deletion Position

For either adding a node or deleting a node, a tester thread will navigate
over the list by using the next field of each node without any
locking/unlocking of the node to evaluate the next field. This is the
optimistic synchronization. In fact, locking the node during navigation
would slow down the navigation itself and would lead to deadlocks with other
threads trying to add/remove nodes from the list.

Note that in a real life operation, the search would not be random but for a
specific node; in any case, the search can be done without locking. Just the
search for the proper node and the operation, should be surrounded with the
txstarts() and txstops() function calls, to announce the start and the end
of the transaction during which we want the list to be stable.

### Adding/Deleting a Node

Once a random insertion/deletion point has been found, only then the nodes
involved are getting locked, if possible. In case a node can not be locked,
the operation will abort; successful operations and aborted ones are each
counted so that we can evaluate the hit/miss ratio during the 1 second
evalution window of the main thread.

In a real life application, the thread would re-start the whole operation of
searching and re-attempting the locking and operation. 


## The ABA Problem

As a node is removed from the list, we can not just call free() on it, since
it could still be in use by another thread for navigation. If we call free()
on it, it could be even re-allocated by another thread and re-inserted,
creating havoc. This is called the 
[ABA problem](https://en.wikipedia.org/wiki/ABA_problem).

So we must keep deleted nodes as long as there is a possibility that another
thread might still be analyzing it, even when that thread will do this
analysis far in the future. Therefore, this sample code will add the node
via a compare and exchange method, i.e. thread safe, to a linked list of
nodes that are 'cooling down'.

In this sample code txstarts() is called at the start of an
operation and txstops() is called at when the thread has finished the
operation or transaction on the list.

1. At the start, the count of outstanding operations is incremented by 1 by
   an atomic increment operation.

2. At the end of the operation, it is checked if, when the operation count is
   atomically reduced by 1, it would reach 0. If that is the case, 
   a locked out cleanup window is created; i.e. there is only a single thread that
   **enters** the window; i.e. there is only 1 thread that is running the
   code in the window, but it is **still competing with other threads** over
   shared variables, **during that time in the window**. I.e. during the window,
   still nodes are possibly being added and deleted by the other threads.
   That lockout is created by temporarily adding a large number (larger than
   the number of competing threads) to the outstanding operation count.

The single thread in the cleanup window code, moves  the whole list of
cooling down nodes, to a second list, the **zombie** list, which will be
cleaned out the **next** time in the cleanup window. Thus we effectively
create a 2-phase delete of the deleted nodes.

In this sample code, we show that the cleanup of nodes can be done either by
a support thread, when all manipulator threads have been set to idle (by
enabling the IDLE_CLEANUP preprocessor guard at the start of the file) or by
leaving the IDLE_CLEANUP macro inactive (comment it out), the inline cleanup
during txstops() is being used; also called the 'hot' way of collecting
garbage as it is done inline with manipulation of the list.
