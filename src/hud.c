#include "hud.h"
#include "draw.h"
#include "platform.h"
#include "mrg.h"
#include "window.h"
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
  mrg_window_frame_draw(state->platform, hud->x, hud->y, hud->w, hud->h); 
}
