#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <stddef.h>
#include "defs.h"
#include "arena.h"

#define MRG_CONSOLE_LINE_LEN 1024
#define MRG_CONSOLE_LINES_MAX 20

struct mrg_console_line {
  char buffer[MRG_CONSOLE_LINE_LEN];
  int index;

  int next;
};

struct mrg_console {
  struct mrg_console_line *lines;
  int bottom;
  enum mrg_mode prev;
  int bs_delay;
};

struct mrg_console mrg_console_init(void);

int mrg_console_draw(struct mrg_state *state, struct mrg_console *console);

int mrg_console_update(struct mrg_state *state, struct mrg_console *console);

void mrg_console_free(struct mrg_console *console);

#endif
