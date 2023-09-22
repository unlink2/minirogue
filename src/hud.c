#include "hud.h"
#include "draw.h"
#include "platform.h"
#include "mrg.h"
#include <string.h>

#define MRG_HUD_TILES_H 4
#define MRG_HUD_OUTLINE_SIZE 4

struct mrg_hud mrg_hud_init(struct mrg_state *state) {
  struct mrg_hud hud;
  memset(&hud, 0, sizeof(hud));

  hud.w = state->screen_w;
  hud.h = state->map.tile_h * MRG_HUD_TILES_H;

  hud.x = 0;
  hud.y = state->screen_h - hud.h;

  return hud;
}

void mrg_hud_update(struct mrg_state *state, struct mrg_hud *hud) {}

void mrg_hud_draw(struct mrg_state *state, struct mrg_hud *hud) {
  mrg_pl_draw_filled_rec(state->platform, hud->x + MRG_HUD_OUTLINE_SIZE,
                         hud->y + MRG_HUD_OUTLINE_SIZE,
                         hud->w - MRG_HUD_OUTLINE_SIZE * 2,
                         hud->h - MRG_HUD_OUTLINE_SIZE * 2, MRG_COLOR1);

  for (int i = 0; i < MRG_HUD_OUTLINE_SIZE; i++) {
    // do not substract i * 2 here to create a "shadow" effect
    mrg_pl_draw_outlined_rec(state->platform, hud->x + i, hud->y + i,
                             hud->w - i, hud->h - i, MRG_COLOR3);
  }
}
