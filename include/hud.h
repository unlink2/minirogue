#ifndef HUD_H_
#define HUD_H_

#include "defs.h"

struct mrg_hud {
  int player_handle;
  int x;
  int y;
  int w;
  int h;
};

struct mrg_hud mrg_hud_init(struct mrg_state *state);

void mrg_hud_update(struct mrg_state *state, struct mrg_hud *hud);
void mrg_hud_draw(struct mrg_state *state, struct mrg_hud *hud);

void mrg_hud_draw_key_frame(struct mrg_state *state, struct mrg_hud *hud, int x,
                            int y, const char *txt);

#endif
