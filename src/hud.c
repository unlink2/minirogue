#include "hud.h"
#include "draw.h"
#include "platform.h"
#include "mrg.h"
#include <string.h>

struct mrg_hud mrg_hud_init(struct mrg_state *state) {
  struct mrg_hud hud;
  memset(&hud, 0, sizeof(hud));

  hud.x = 0;
  hud.y = state->screen_h - state->map.tile_h * 4;

  return hud;
}

void mrg_hud_update(struct mrg_state *state, struct mrg_hud *hud) {}

void mrg_hud_draw(struct mrg_state *state, struct mrg_hud *hud) {
  mrg_pl_draw_filled_rec(state->platform, hud->x, hud->y, state->screen_w,
                         state->map.tile_h * 4, MRG_COLOR1);
}
