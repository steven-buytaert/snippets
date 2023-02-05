// Copyright 2023 Steven Buytaert

#ifndef TXT_TR_UTILS_H
#define TXT_TR_UTILS_H

#include <stdint.h>

// Extremely simple in place text transformations.

typedef struct Reflow_t {
  char *     text;                // Text to reflow; assign before calling reflowText().
  uint32_t   width;               // Width of lines to produce. Lines are only cut at a space.
  uint32_t   cur;                 // Current snip (internal use).
  char       restore;             // The character to restore (internal use only).
  uint32_t   numlines;            // Number of lines created; when == cap4snips, capacity exhausted.
  uint32_t   cap4snips;           // Total number of slots in snips array.
  uint32_t   snips[0];            // Snip a line at text[snips[]].
} Reflow_t;

// Reflow a text block into a number of lines of given width. Pass a Reflow_t structure of enough
// capacity for the passed text. Check that cap4snips is less than numlines, after the first call.
// The structure must be cleared, except the Reflow.text field properly assigned, a non zero Reflow.width set
// and the cap4snips set to the properly allocated tail space.
// 
// Each call, a line of the proper width is returned or NULL, when no there are more lines. The final argument,
// when not NULL, is set to 1, only when the final line is returned; otherwise it is set to 0.

char * reflowText(Reflow_t * ref, uint32_t * final);

// None of the following functions, if given a buffer, will check if the buffer is large enough!

uint32_t turnCRIntoSpace(char * buf);      // Replace all \n by a space; return # replaced.
uint32_t trimLeadingSpaces(char * buf);    // Remove all leading spaces; return number removed; return # removed.
uint32_t trimInterSpaces(char * buf);      // Trim consecutive spaces into a single space; return # removed.
uint32_t turnCont2SingleWord(char * buf);  // Change 'xx- yy' into 'xxyy'; return # characters removed.
uint32_t trimTrailSpaces(char * buf);      // Remove all trailing spaces; return # removed.

// Replace a string with another string; return number of replacements done.

uint32_t replaceStrWithStr(char * buf, const char * replace, const char * with);

#endif // TXT_TR_UTILS_H
