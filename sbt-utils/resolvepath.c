// Copyright 2023 Steven Buytaert

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <sbt-utils.h>

/*

  char * resolvepath(const char * restrict path, char * restrict respath, size_t rsz);

  Essentially a clone of the Posix realpath function, but with following changes:
  
  - the resolved buffer size is passed as third argument.

  - the resolved buffer pointer respath may NOT be NULL. In the realpath case, it would allocate a
    suitable buffer that the caller needed to free() afterwards, but the size allocated would be PATH_MAX.
    The buffer was never larger than PATH_MAX and we want to get rid of the PATH_MAX constant; especially
    since it is not always correct (see https://eklitzke.org/path-max-is-tricky: good article though the
    author is wrong about the error returned fom getcwd when the buffer is overflown; it's ERANGE not
    ENAMETOOLONG).

  - in case an error condition occurs, the resolved path buffer contains the path up the component that
    caused the issue. In the case of realpath, the manual page states that the contents are undefined.

For the rest, the semantics of this function are (should be) identical to the Posix realpath function.

This code is only supported on systems that provide lstat/readlink/getcwd.

Compile with -DNDEBUG to disable the assert statement. The implementation uses the strlcat and strlcpy variants with
a passed reference to an overflow counter.

It uses a while loop and an internal resolve function to solve the issue where the resolution of a symbolic link might
introduce other relative paths. Also uses the lstat function so that we can make the proper distinction between
ENOTDIR en ENOENT situations that readlink alone doesn't handle.

It is not the smallest possible implementation but good enough and (hopefully) correct.

NOTE: the working buffer sizes are sized according to the passed path or buffer size, whichever is larger. The working
      buffers are allocated on the stack; in the case of very large passed buffer sizes, you might want to prefer to
      allocate these working buffers from the heap and release them afterwards.

*/

size_t rpmaxnsl = 40;                                       // Maximum number of symlinks; we don't use MAXSYMLINKS.

static size_t backup(char path[], size_t x) {               // Backup path until the initial / in the path.

  while (x > 1) {
    x--;
    if ('/' == path[x]) {
      path[x] = 0;
      break;
    }
    path[x] = 0;                                            // Clear; suppose we drop out of the while without finding a '/'.
  }

  return x;

}

typedef struct Ctx_t {            // Internal context.
  char *     pathbuf;             // Modifiable path buffer
  char *     symbuf;              // Symbolic link resolution buffer.
  uint32_t   wbsz;                // Working buffer size of both above.
  uint32_t   overflow;            // Should remain at 0.
  uint32_t * o;                   // (shorthand) reference to overflow.
  int32_t    error;               // Error of not 0.
  uint32_t   nsl;                 // Number of resolved symbolic links.
} Ctx_t;

static void resolve(Ctx_t * ctx, char * restrict respath, size_t rsz) {

  ssize_t     rrs;                                          // readlink() return status.
  char *      t = ctx->pathbuf;                             // Start of token/path component.
  char *      e;                                            // End of token.
  int32_t     s;                                            // strlen(t).
  size_t      x = 0;                                        // Running strlen(respath).
  size_t      rem;                                          // Remaining characters.
  struct stat Stat;

  ctx->error = ENAMETOOLONG;                                // Preset most occuring issue.

  rem = strlen(ctx->pathbuf);
  
  if ('/' == ctx->pathbuf[0]) {
    strcpy4o(respath, "/", rsz, ctx->o);                    // Add starting slash to resolved path.
    t++; rem--; x++;                                        // One character already consumed from path and added to respath.
  }
  else {
    if (! getcwd(respath, rsz)) {                           // Not an absolute path; start from current folder.
      if (ERANGE != errno) { ctx->error = errno; }          // ERANGE becomes ENAMETOOLONG; rest of errors unmodified.
      return;
    }
    x = strlen(respath);
  }

  while (rem && ! ctx->overflow) {                          // Process the rest of the path components.
    for (e = t; *e && rem; e++, rem--) {                    // Find a '/' if any characters left.
      if ('/' == *e) {
        rem--;                                              // To account for this slash, being removed.
        break;                                              // Process this new path component.
      }
    }
    *e = 0;                                                 // NUL-Terminate the token.

    s = e - t;                                              // s = strlen(t);

         if (! s)                   { }                     // // skip.
    else if ('.' == t[0] && 1 == s) { }                     // ./ skip.
    else if ('.' == t[0] && 2 == s) {                       // ../ backup 1 element.
      x = backup(respath, x);
    }
    else {                                                  // A normal token.
      assert(x && x == strlen(respath));                    // Since we use x - 1 and check that x upholds its premise.

      if ('/' != respath[x - 1]) {                          // Ensure current resolved path ends with a / ...
        strcat4o(respath, "/", rsz, ctx->o);
        x++;
      }

      strcat4o(respath, t, rsz, ctx->o);                    // ... before we add the component token.

      if (ctx->overflow) { break; }                         // Check before we lstat.

      x += s;                                               // Update running length.

      if (lstat(respath, & Stat)) {
        if (ENOTDIR == errno) { backup(respath, x); }       // Ensure the contents of respath are consistent for caller.
        if (rem && ENOENT == errno) { errno = ENOTDIR; }    // To comply with realpath man page.
        ctx->error = errno;
        return;
      }

      if (S_ISLNK(Stat.st_mode)) {                          // A symbolic link, resolve it.
        if (++ctx->nsl == rpmaxnsl) {
          ctx->error = ELOOP;
          return;
        }
        rrs = readlink(respath, ctx->symbuf, ctx->wbsz - 1);
        if (-1 == rrs) {
          ctx->error = errno;
          return;
        }
        ctx->symbuf[rrs] = 0;                               // Since readlink doesn't NUL-terminate the buffer.
        if ('/' == ctx->symbuf[0]) {
          strcpy4o(respath, ctx->symbuf, rsz, ctx->o);      // Resolved link is absolute, replace whole resolved path.
          x = rrs;                                          // Becomes the new strlen(respath).
        }
        else {
          x = backup(respath, x);                           // Relative link, remove link name and replace with resolved.
          strcat4o(respath, "/", rsz, ctx->o);
          strcat4o(respath, ctx->symbuf, rsz, ctx->o);
          x += 1 + rrs;                                     // Account for added / and resolved link length.
        }
      }
    }

    t = e + 1;                                              // Next token, if any characters left.
  }

  if (! ctx->overflow) { ctx->error = 0; }                  // Succesful end if nothing overflowed.

}

char * resolvepath(const char * restrict path, char * restrict respath, size_t rsz) {

  size_t       pathsize = path ? strlen(path) + 1 : 0;      // In case of NULL path, we don't use pathsize anyway.
  size_t       wbsz = rsz > pathsize ? rsz + 1 : pathsize;  // Ensure reasonable working buffer sizes.
  char         pathbuf[wbsz];                               // Buffer to copy path into for modification.
  char         symbuf[wbsz];                                // readlink() buffer.
  Ctx_t        Ctx;
  const char * src = path;                                  // Source to copy into the pathbuf; starts with given path.

  if (! path || ! respath) {                                // Note that we do not support a NULL respath pointer like realpath does.
    errno = EINVAL;
    return NULL;
  }
  
  Ctx.pathbuf = pathbuf;
  Ctx.symbuf  = symbuf;
  Ctx.wbsz = wbsz;
  Ctx.overflow = 0;
  Ctx.o = & Ctx.overflow;
  Ctx.nsl = 0;
  Ctx.error = 0;

  do {
    strlcpy(pathbuf, src, wbsz);                            // Can not fail since wbsz > strlen(path) or rsz.
    resolve(& Ctx, respath, rsz);
    src = respath;
  } while (! Ctx.error && strstr(src, "./"));

  errno = Ctx.error;

  return Ctx.error ? NULL : respath;
    
}
