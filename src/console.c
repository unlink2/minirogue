#include "console.h"
#include "command.h"
#include "input.h"
#include "mrg.h"
#include "platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mrg_console mrg_console_init(void) {
  struct mrg_console console;
  memset(&console, 0, sizeof(console));
  console.bs_delay = 0;
  console.lines_to_draw = 25;
  console.line_scroll = 0;

  console.cmd_tbl = mrg_cmd_tbl;

  return console;
}

int mrg_console_draw(struct mrg_state *state, struct mrg_console *console) {
  int y_per_line = 16;
  int lines_to_draw = console->lines_to_draw;

  int x = 10;
  int y = 32 + y_per_line * lines_to_draw;
  int input_y = y + y_per_line;

  int font_size = 16;

  mrg_pl_draw_filled_rec(state->platform, 0, 0, state->screen_w,
                         state->screen_h, (struct mrg_color){0, 0, 0, 128});
  mrg_pl_draw_outlined_rec(state->platform, 0, 0, state->screen_w,
                           state->screen_h, (struct mrg_color){255, 0, 0, 255});

  for (size_t i = console->line_scroll;
       i < MIN(console->lines_len, lines_to_draw) + console->line_scroll; i++) {
    size_t index = console->lines_len - i - 1;
    if (index > console->lines_len) {
      continue;
    }

    mrg_pl_print(state->platform, console->lines[index], x, y, font_size,
                 MRG_WHITE);
    y -= y_per_line;
  }

  mrg_pl_print(state->platform, ">", x, input_y, font_size, MRG_WHITE);
  mrg_pl_print(state->platform, console->input.buffer, x + 20, input_y,
               font_size, MRG_WHITE);

  return 0;
}

int mrg_console_exec(struct mrg_state *state, struct mrg_console *console,
                     const char *cmd) {
  return mrg_cmd_exec(console, mrg_console_puts, cmd, console->cmd_tbl, state);
}

int mrg_console_puts(const char *s, void *fp) {
  struct mrg_console *console = fp;
  // FIXME: this will malloc way too often...
  console->lines_len++;
  size_t lines_len = sizeof(char **) * console->lines_len;
  char **new_lines = NULL;

  if (console->lines) {
    new_lines = realloc(console->lines, lines_len);
  } else {
    new_lines = malloc(lines_len);
  }

  if (!new_lines) {
    fprintf(stderr, "Realloc failed for console lines...\n");
    return -1;
  }
  console->lines = new_lines;

  size_t input_len = strlen(s);

  char *new_str = malloc(input_len + 1);
  if (!new_str) {
    fprintf(stderr, "Malloc failed for console lines...\n");
    return -1;
  }
  memset(new_str, 0, input_len + 1);
  strncpy(new_str, s, input_len);

  console->lines[console->lines_len - 1] = new_str;

  return (int)input_len;
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
    mrg_console_puts(console->input.buffer, console);
    mrg_console_exec(state, console, console->input.buffer);

    console->input.index = 0;
    memset(console->input.buffer, 0, MRG_CONSOLE_LINE_LEN);
    console->line_scroll = 0;
  }

  if (MRG_HELD(&state->main_input, MRG_ACTION_SCRLDOWN) && !console->bs_delay) {
    console->line_scroll = MAX(0, console->line_scroll - 1);
  }

  if (MRG_HELD(&state->main_input, MRG_ACTION_SCRLUP) && !console->bs_delay) {
    console->line_scroll = console->line_scroll + 1;
  }

  if (console->bs_delay) {
    console->bs_delay--;
  }

  return 0;
}

void mrg_console_free(struct mrg_console *console) {
  for (size_t i = 0; i < console->lines_len; i++) {
    free(console->lines[i]);
  }
  if (console->lines) {
    free(console->lines);
  }
}
