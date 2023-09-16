#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <stddef.h>
#include "defs.h"
#include "arena.h"

#define MRG_CONSOLE_LINE_LEN 1024

struct mrg_console_line {
  char buffer[MRG_CONSOLE_LINE_LEN];
  int index;
};

struct mrg_console {
  char **lines;
  size_t lines_len;
  struct mrg_console_line input;
  enum mrg_mode prev;
  int bs_delay;

  int lines_to_draw;
  int line_scroll;
};

struct mrg_console mrg_console_init(void);

int mrg_console_draw(struct mrg_state *state, struct mrg_console *console);

int mrg_console_exec(struct mrg_state *state, const char *cmd);

int mrg_console_puts(const char *s, void *fp);
int mrg_console_update(struct mrg_state *state, struct mrg_console *console);

void mrg_console_free(struct mrg_console *console);

#endif
