#include "hud.h"
#include "defs.h"
#include "draw.h"
#include "platform.h"
#include "mrg.h"
#include "tiles.h"
#include "window.h"
#include <stdio.h>
#include <string.h>
#include "input.h"

#define MRG_HUD_TILES_H 4
#define MRG_HUD_KEYFRAME_SIZE 32

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
  struct mrg_entity *player = &state->entity_tbl.slots[hud->player_handle];
  if (!(player->flags & MRG_ENTITY_FLAG_ALLOCED)) {
    fprintf(stderr, "Hud: Player %d is not allocated!", hud->player_handle);
    return;
  }

  const int alpha = 0xC0;
  const int key_frame_y = hud->y + 10;

  const int act_a_x = hud->x + 10;
  const int act_b_x = act_a_x + 10 + MRG_HUD_KEYFRAME_SIZE;
  const int act_pause_x = act_b_x + 10 + MRG_HUD_KEYFRAME_SIZE;
  const int act_bag_x = act_pause_x + 10 + MRG_HUD_KEYFRAME_SIZE;

  const int hp_bar_x = act_a_x;
  const int hp_bar_y = hud->y - 10;
  // mrg_window_frame_draw(state->platform, hud->x, hud->y, hud->w, hud->h);

  // action a
  mrg_hud_draw_key_frame(state, hud, act_a_x, key_frame_y,
                         mrg_pl_input_key(state->platform, MRG_ACTION_A));

  // action b
  mrg_hud_draw_key_frame(state, hud, act_b_x, key_frame_y,
                         mrg_pl_input_key(state->platform, MRG_ACTION_B));

  // pause icon
  mrg_hud_draw_key_frame(state, hud, act_pause_x, key_frame_y,
                         mrg_pl_input_key(state->platform, MRG_ACTION_PAUSE));
  mrg_tile_draw(&state->tile_tbl, state->platform, state->map.tileset_id, 10,
                act_pause_x + 14, key_frame_y + 14);

  // menu icon
  mrg_hud_draw_key_frame(state, hud, act_bag_x, key_frame_y,
                         mrg_pl_input_key(state->platform, MRG_ACTION_MENU));
  mrg_tile_draw(&state->tile_tbl, state->platform, state->map.tileset_id, 12,
                act_bag_x + 14, key_frame_y + 14);

  switch (state->mode) {
  case MRG_MODE_MAPED:
    // action a icon maped
    mrg_tile_draw(&state->tile_tbl, state->platform, state->map.tileset_id,
                  state->entity_tbl.slots[state->maped.cursor_handle]
                      .stats[MRG_STAT_USTAT1],
                  act_a_x + 14, key_frame_y + 14);

    // action b flags
    {
      char tileflags[16];
      sprintf(tileflags, "%03d\n",
              (uint8_t)state->entity_tbl.slots[state->maped.cursor_handle]
                  .stats[MRG_STAT_USTAT2]);
      mrg_pl_print(state->platform, tileflags, act_b_x + 10, key_frame_y + 16,
                   12, MRG_COLOR1);
    }
    break;
  default:
    break;
  }

  mrg_hud_draw_bar(state, hp_bar_x, hp_bar_y, 100, 10,
                   player->stats[MRG_STAT_HP], player->stats[MRG_STAT_HP_MAX],
                   MRG_COLOR_ALPHA(MRG_COLOR2, alpha),
                   MRG_COLOR_ALPHA(MRG_COLOR3, alpha));
}

void mrg_hud_draw_key_frame(struct mrg_state *state, struct mrg_hud *hud, int x,
                            int y, const char txt) {
  char txt_str[2] = {txt, '\0'};

  mrg_pl_draw_filled_rec(state->platform, x, y, MRG_HUD_KEYFRAME_SIZE,
                         MRG_HUD_KEYFRAME_SIZE,
                         MRG_COLOR_ALPHA(MRG_COLOR0, 0x60));

  int font_size = 10;
  int txt_size = mrg_pl_text_pxl(state->platform, txt_str, font_size);

  mrg_pl_draw_outlined_rec(state->platform, x, y, txt_size + 8, txt_size + 8,
                           MRG_COLOR1);
  mrg_pl_print(state->platform, txt_str, x + 2, y + 2, font_size, MRG_COLOR1);
}

void mrg_hud_draw_bar(struct mrg_state *state, int x, int y, int w, int h,
                      int current, int max, struct mrg_color c1,
                      struct mrg_color c2) {

  int prop = 0;
  if (max > 0) {
    prop = current % max;
  }

  mrg_pl_draw_filled_rec(state->platform, x, y, w, h, c1);
  mrg_pl_draw_filled_rec(state->platform, x, y, prop, h, c2);
}
