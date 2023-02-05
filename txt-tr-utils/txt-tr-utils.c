// Copyright 2023 Steven Buytaert

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
    memmove(buf, buf + 1, end - buf);
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
    else {
      if (spaces > 1) {
        spaces--;
        memmove(cur - spaces, cur, end - cur + 1);
        end -= spaces;
        count += spaces;
      }
      spaces = 0;
    }
    cur++;
  }

  return count;

}

uint32_t turnCont2SingleWord(char * buf) {                  // Change 'xx- yy' into 'xxyy'; return # characters removed.

  char *   cur = buf;
  char *   end = buf + strlen(buf);
  uint32_t count = 0;

  while (*cur) {
    if (cur[-1] != ' ' && cur[0] == '-' && cur[1] == ' ') {
      memmove(cur, cur + 2, end - cur);
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

typedef struct Rep_t {     // Replacement.
  struct {
    const char * str;
    uint32_t     size;
  } Replace;
  struct {
    const char * str;
    uint32_t     size;
  } With;
} Rep_t;

uint32_t replaceStrWithStr(char * buf, const char * macro, const char * with) {

  char *   c;
  char *   end = buf + strlen(buf);
  char *   src;
  char *   dst;
  Rep_t    Rep = { { .str = macro }, { .str = with } };
  Rep_t *  m = & Rep;
  uint32_t count = 0;
  int32_t  diff = strlen(with) - strlen(macro);

  Rep.Replace.size = strlen(macro);
  Rep.With.size = strlen(with);

  for (c = & buf[0]; c < end; c++) {
    if (0 == strncmp(c, m->Replace.str, m->Replace.size)) { // OK replace here.
      count++;
      src = c + m->Replace.size;
      dst = c + m->With.size;
      if (diff > 0) {
        memmove(dst, src, end - src);                       // Need to increase.
      }
      else if (diff < 0) {
        memmove(dst, src, end - dst);                       // Need to shrink.
      }
      memcpy(c, m->With.str, m->With.size);                 // Insert replacement.
      c += m->With.size;
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
    if (width == rwidth) {
      while (*cur != ' ' && *cur) {                         // Never break a word in 2.
        cur++; width++;
      }
      if (*cur) { cur++; }                                  // Make the space part of this line, not the start of the next.
      reflow->snips[snip] = cur - reflow->text;
      snip++; line++;
      if (line == reflow->cap4snips) { width = 0; break; }
      width = 0;
    }
  }

  if (width) {                                              // Do the final snip, if any.
    reflow->snips[snip] = cur - reflow->text;
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
