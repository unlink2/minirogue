#include "console.h"
#include "input.h"
#include "mrg.h"
#include "platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mrg_console mrg_console_init(void) {
  struct mrg_console console;
  console.bs_delay = 0;

  size_t lineslen = sizeof(struct mrg_console_line) * MRG_CONSOLE_LINES_MAX;
  console.buffer = malloc(lineslen);

  memset(console.buffer, 0, lineslen);

  return console;
}

int mrg_console_draw(struct mrg_state *state, struct mrg_console *console) {

  int x = 10;
  int y = 32;
  for (size_t i = 0; i < MRG_CONSOLE_LINES_MAX; i++) {
    mrg_pl_print(state->platform, console->buffer[i].buffer, x, y, 20,
                 MRG_WHITE);
    y += 20;
  }

  mrg_pl_print(state->platform, ">", x, y, 20, MRG_WHITE);
  mrg_pl_print(state->platform, console->input.buffer, x + 20, y, 20,
               MRG_WHITE);

  return 0;
}

int mrg_console_update(struct mrg_state *state, struct mrg_console *console) {
  int c = 0;
  struct mrg_console_line *l = &console->input;
  do {
    c = mrg_pl_char_pressed(state->platform);
    if (c && l->index < MRG_CONSOLE_LINE_LEN - 1) {
      l->buffer[l->index++] = (char)c;
    }
  } while (c);

  if (MRG_HELD(&state->main_input, MRG_ACTION_BACKSPACE) &&
      !console->bs_delay) {
    console->bs_delay = 5;
    if (l->index <= 0) {
      l->index = 1;
    }
    l->buffer[--l->index] = '\0';
  }

  if (MRG_PRESSED(&state->main_input, MRG_ACTION_ENTER)) {
    console->input.index = 0;

    // move all buffers up one and drop first one
    for (size_t i = MRG_CONSOLE_LINES_MAX - 2; i > 0;
         i -= 2) {
      memcpy(console->buffer[i - 1].buffer, console->buffer[i].buffer,
             MRG_CONSOLE_LINE_LEN);
    }

    // move input buffer to last buffer
    memcpy(console->buffer[MRG_CONSOLE_LINES_MAX - 1].buffer,
           console->input.buffer, MRG_CONSOLE_LINE_LEN);

    memset(console->input.buffer, 0, MRG_CONSOLE_LINE_LEN);
  }

  if (console->bs_delay) {
    console->bs_delay--;
  }

  return 0;
}

void mrg_console_free(struct mrg_console *console) { free(console->buffer); }
