#include "console.h"
#include "input.h"
#include "mrg.h"
#include "platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mrg_console mrg_console_init(void) {
  struct mrg_console console;
  console.bottom = 0;
  console.bs_delay = 0;

  size_t lineslen = sizeof(struct mrg_console_line) * MRG_CONSOLE_LINES_MAX;
  console.lines = malloc(lineslen);

  memset(console.lines, 0, lineslen);

  for (size_t i = 0; i < MRG_CONSOLE_LINES_MAX; i++) {
    console.lines[i].next = -1;
  }

  return console;
}

int mrg_console_draw(struct mrg_state *state, struct mrg_console *console) {

  int handle = console->bottom;
  int x = 10;
  int y = 32;
  while (handle != -1) {
    mrg_pl_print(state->platform, console->lines[handle].buffer, x, y, 20,
                 MRG_WHITE);
    y += 20;
    handle = console->lines[handle].next;
  }

  return 0;
}

int mrg_console_update(struct mrg_state *state, struct mrg_console *console) {
  int c = 0;
  struct mrg_console_line *l = &console->lines[console->bottom];
  do {
    c = mrg_pl_char_pressed(state->platform);
    if (c && l->index < MRG_CONSOLE_LINE_LEN - 1) {
      l->buffer[l->index++] = (char)c;
    }
  } while (c);

  if (MRG_HELD(&state->main_input, MRG_ACTION_BACKSPACE) &&
      !console->bs_delay) {
    console->bs_delay = 10;
    if (l->index<= 0) {
      l->index = 1;
    }
    l->buffer[--l->index] = '\0';

  }

  if (console->bs_delay) {
    console->bs_delay--;
  }

  return 0;
}

void mrg_console_free(struct mrg_console *console) { free(console->lines); }
