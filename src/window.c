#include "window.h"
#include "platform.h"

#define MRG_WIN_OUTLINE_SIZE 4

void mrg_window_frame_draw(mrg_platform *p, int x, int y, int w, int h) {
  mrg_pl_draw_filled_rec(p, x + MRG_WIN_OUTLINE_SIZE, y + MRG_WIN_OUTLINE_SIZE,
                         w - MRG_WIN_OUTLINE_SIZE * 2,
                         h - MRG_WIN_OUTLINE_SIZE * 2, MRG_COLOR1);

  for (int i = 0; i < MRG_WIN_OUTLINE_SIZE; i++) {
    // do not substract i * 2 here to create a "shadow" effect
    mrg_pl_draw_outlined_rec(p, x + i, y + i, w - i, h - i, MRG_COLOR3);
  }
}
