#include "hud.h"
#include "draw.h"
#include "platform.h"
#include "mrg.h"
#include "window.h"
#include <string.h>

#define MRG_HUD_TILES_H 4

struct mrg_hud mrg_hud_init(struct mrg_state *state) {
  struct mrg_hud hud;
  memset(&hud, 0, sizeof(hud));

  hud.w = state->screen_w;
  hud.h = state->map.tile_w * MRG_HUD_TILES_H;

  hud.x = 0;
  hud.y = state->screen_h - hud.h;

  return hud;
}

void mrg_hud_update(struct mrg_state *state, struct mrg_hud *hud) {}

void mrg_hud_draw(struct mrg_state *state, struct mrg_hud *hud) {
  mrg_window_frame_draw(state->platform, hud->x, hud->y, hud->w, hud->h);

  mrg_pl_draw_outlined_rec(state->platform, hud->x + hud->w - 40,
                           hud->y + hud->h - 50, 32, 32, MRG_COLOR0);

  mrg_hud_draw_key_frame(state, hud, hud->x + 10, hud->y + 10, "A");
}

void mrg_hud_draw_key_frame(struct mrg_state *state, struct mrg_hud *hud, int x,
                            int y, const char *txt) {
  mrg_pl_draw_outlined_rec(state->platform, x, y, 32, 32, MRG_COLOR0);

  int font_size = 10;
  int txt_size = mrg_pl_text_pxl(state->platform, txt, font_size);

  mrg_pl_draw_filled_rec(state->platform, x, y, txt_size + 4, txt_size + 4,
                         MRG_COLOR0);
  mrg_pl_print(state->platform, txt, x + 1, y + 1, font_size, MRG_COLOR1);
}
