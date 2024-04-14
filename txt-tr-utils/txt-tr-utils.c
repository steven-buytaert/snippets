// Copyright 2023-2024 Steven Buytaert

#include <string.h>

#include <txt-tr-utils.h>

uint32_t turnCRIntoSpace(char * buf) {                      // Replace all \n by a space; return # replaced.

  uint32_t count;
  char *   cur;

  for (count = 0, cur = buf; *cur; cur++) {
    if (*cur == '\n') { *cur = ' '; count++; }
  }

  return count;

}

uint32_t trimLeadingSpaces(char * buf) {                    // Remove all leading spaces; return number removed; return # removed.

  char * end = buf + strlen(buf);
  char * orig = end;

  while (*buf == ' ') {
    memmove(buf, buf + 1, (size_t)(end - buf));
    end -= 1;
  }

  return (uint32_t)(orig - end);

}

uint32_t trimInterSpaces(char * buf) {                      // Trim consecutive spaces into a single space; return # removed.

  char *   cur = buf;
  char *   end = buf + strlen(buf);
  uint32_t spaces = 0;
  uint32_t count = 0;

  while (*cur) {
    if (*cur == ' ') { spaces++; }
    else {                                                  // Non space character, evaluate spaces.
      if (spaces > 1) {
        spaces--;                                           // We want to keep 1 space.
        memmove(cur - spaces, cur, (size_t)(end - cur + 1));
        end   -= spaces;
        count += spaces;
        cur   -= spaces;                                    // Start rechecking here; note that cur is always incremented below.
      }
      spaces = 0;
    }
    cur++;
  }

  if (spaces > 1) {                                         // Reduce any final spaces.
    spaces--;                                               // We want to keep 1 space.
    memmove(cur - spaces, cur, (size_t)(end - cur + 1));
    count += spaces;
  }

  return count;

}

uint32_t turnCont2SingleWord(char * buf) {                  // Change e.g. 'xx- yy' into 'xxyy'; return # characters removed.

  char *   cur = buf;
  char *   end = buf + strlen(buf);
  uint32_t count = 0;

  while (*cur) {
    if (cur[-1] != ' ' && cur[0] == '-' && cur[1] == ' ') {
      memmove(cur, cur + 2, (size_t)(end - cur));
      end -= 2;                                             // End has contracted.
      count += 2;
    }
    cur++;
  }

  return count;

}

uint32_t trimTrailSpaces(char * buf) {                     // Remove all trailing spaces; return # removed.

  char * end = buf + strlen(buf);
  char * orig = end;

  while (end > buf && end[-1] == ' ') {
    *--end = 0;
  }

  return (uint32_t)(orig - end);                            // Return number of characters removed.

}

uint32_t replaceStrWithStr(char * buf, const char * replace, const char * with) {

  char *   c;
  char *   end = buf + strlen(buf);
  char *   src;
  char *   dst;
  uint32_t count = 0;
  uint32_t withsize = (uint32_t) strlen(with);
  uint32_t replsize = (uint32_t) strlen(replace);
  int32_t  diff = (int32_t)(withsize - replsize);

  for (c = & buf[0]; c < end; c++) {
    if (0 == strncmp(c, replace, replsize)) {               // OK replace here.
      count++;
      src = c + replsize;
      dst = c + withsize;
      if (diff > 0) {
        memmove(dst, src, (size_t)(end - src));             // Need to increase.
      }
      else if (diff < 0) {
        memmove(dst, src, (size_t)(end - dst));             // Need to shrink.
      }
      memcpy(c, with, withsize);                            // Insert replacement.
      c += withsize;
      end += diff;
    }
  }

  return count;

}

static void chopit(uint32_t rwidth, Reflow_t * reflow) {    // Chop the block to be reflown into appropriate pieces.

  uint32_t   line = 0;
  char *     cur = reflow->text;
  uint32_t   width = 0;
  uint32_t   snip = 0;

  memset(reflow->snips, 0x00, sizeof(reflow->snips[0]) * reflow->cap4snips);

  reflow->cur = 0;

  while (*cur) {
    cur++; width++;
    if (width + 1 == rwidth) {
      while (*cur != ' ' && width) {                        // Never break a word in 2; backup to a space.
        cur--; width--;
      }
      if (*cur) { cur++; }                                  // Make the space part of this line, not the start of the next.
      reflow->snips[snip] = (uint32_t)(cur - reflow->text);
      snip++; line++;
      if (line == reflow->cap4snips) { width = 0; break; }
      width = 0;
    }
  }

  if (width) {                                              // Do the final snip, if any.
    reflow->snips[snip] = (uint32_t)(cur - reflow->text);
    line++;
    snip++;
  }

  reflow->numlines = line;

}

char * reflowText(Reflow_t * reflow, uint32_t * final) {

  char *   line = NULL;
  uint32_t cur = reflow->cur;
  uint32_t prevpos;
  uint32_t curpos;

  if (0 == reflow->numlines) {                              // Initialize first time here.
    chopit(reflow->width, reflow);
  }
  
  if (cur < reflow->numlines) {
    if (cur == 0) {                                         // First time called.
      line = & reflow->text[0];
    }
    else {
      prevpos = reflow->snips[cur - 1];
      reflow->text[prevpos] = reflow->restore;              // Previous snip contains restore position.
      line = & reflow->text[prevpos];                       // Line starts where the previous was chopped.
    }
    curpos = reflow->snips[cur];
    reflow->restore = reflow->text[curpos];
    reflow->text[curpos] = 0;                               // Chop this line.
    reflow->cur++;
  }

  *final = (reflow->cur == reflow->numlines) ? 1 : 0;

  return line;

}
