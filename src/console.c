#include "console.h"
#include "mrg.h"
#include <stdlib.h>
#include <string.h>

struct mrg_console mrg_console_init(void) {
  struct mrg_console console;
  console.bottom = 0;

  size_t lineslen = sizeof(struct mrg_console_line) * MRG_CONSOLE_LINES_MAX;
  console.lines = malloc(lineslen);

  memset(console.lines, 0, lineslen);

  return console;
}

int mrg_console_draw(struct mrg_state *state, struct mrg_console *console) {
  if (state->mode != MRG_MODE_CONSOLE) {
    return 0;
  }

  return 0;
}

int mrg_console_update(struct mrg_state *state, struct mrg_console *console) {
  if (state->mode != MRG_MODE_CONSOLE) {
    return 0;
  }

  return 0;
}

void mrg_console_free(struct mrg_console *console) { free(console->lines); }
