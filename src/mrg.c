#include "mrg.h"
#include "command.h"
#include "defaults.h"
#include "defs.h"
#include "draw.h"
#include "entity.h"
#include "input.h"
#include "platform.h"
#include "tiles.h"
#include <stdio.h>
#include <string.h>

int mrg_mode_maped(struct mrg_state *state) {
  mrg_maped_update(state, &state->maped);
  return 0;
}

int mrg_mode_game(struct mrg_state *state) {
  mrg_pl_video_draw_pixel(state->platform, 0, 0, MRG_WHITE);
  return 0;
}

int mrg_main_loop(struct mrg_state *state) {
  mrg_platform *platform = state->platform;

  while (mrg_pl_video_open(platform) && state->idc.ok != -1) {
    // input
    mrg_input_poll(state, &state->main_input);

    if (MRG_PRESSED(&state->main_input, MRG_ACTION_DBG_TOGGLE_CONSOLE)) {
      if (state->mode == MRG_MODE_CONSOLE) {
        mrg_transition(state, state->console.prev);
      } else {
        mrg_transition(state, MRG_MODE_CONSOLE);
      }
    }

    // update
    if (mrg_map_update(state, &state->map) == -1) {
      fprintf(stderr, "Map update failed!\n");
    }

    if (mrg_entity_tbl_update(state, &state->entity_tbl) == -1) {
      fprintf(stderr, "Entity update failed!\n");
    }

    mrg_camera_update(state, &state->main_camera);

    switch (state->mode) {
    case MRG_MODE_GAME:
      mrg_mode_game(state);
      break;
    case MRG_MODE_CONSOLE:
      mrg_console_update(state, &state->console);
      break;
    case MRG_MODE_MAPED:
      mrg_mode_maped(state);
      break;
    }

    mrg_hud_update(state, &state->hud);

    // draw
    mrg_pl_video_begin(platform);

    mrg_pl_camera_begin(platform, &state->main_camera);

    mrg_map_draw(state, &state->map);
    mrg_entity_tbl_draw(state, &state->entity_tbl);
    mrg_pl_camera_end(platform, &state->main_camera);

    mrg_hud_draw(state, &state->hud);

    switch (state->mode) {
    case MRG_MODE_CONSOLE:
      mrg_console_draw(state, &state->console);
      break;
    case MRG_MODE_MAPED:
      mrg_maped_draw(state, &state->maped);
      break;
    default:
      break;
    }

    mrg_pl_draw_debug(state->platform);

    mrg_pl_video_end(platform);

    if (state->frame % MRG_MAP_GLOBAL_ANIMATION_TIMER == 0) {
      state->alt_anim = !state->alt_anim;
    }

    state->frame++;
  }
  return 0;
}

int mrg_std_fputs(const char *s, void *fp) { return fputs(s, fp); }

struct mrg_state mrg_state_init(struct mrg_config *cfg,
                                mrg_platform *platform) {
  struct mrg_state state;
  memset(&state, 0, sizeof(state));

  state.screen_w = MRG_PL_SCREEN_W;
  state.screen_h = MRG_PL_SCREEN_H;

  state.platform = platform;
  state.room_arena = mrg_arena_init(4096);
  state.idc_arena = mrg_arena_init(4096);
  state.tmp_arena = mrg_arena_init(4096);

  state.main_camera = mrg_camera_init(&state);
  if (state.main_camera.handle == -1) {
    fprintf(stderr, "Unabel to init main camera!\n");
    state.good = -1;
    return state;
  }

  state.entity_tbl = mrg_entity_tbl_init();

  state.cfg = cfg;
  state.main_input = mrg_pl_input_init();
  if (state.main_input.handle == -1) {
    fprintf(stderr, "Unabel to init input handler!\n");
    state.good = -1;
    return state;
  }

  state.tile_tbl = mrg_tile_set_tbl_init();
  state.tile_w = MRG_TILE_W;
  state.tile_h = MRG_TILE_H;

  state.idc = mrg_default_idc();
  state.room_tbl = mrg_room_tbl_from_idc(&state, &state.room_arena, &state.idc);
  // TODO: dynamically load rooms!
  state.map = mrg_map_init(&state, 0);
  // TODO: dynamically load entities!
  mrg_entities_from_idc(&state, &state.idc);

  state.console = mrg_console_init();
  state.hud = mrg_hud_init(&state);

  mrg_transition(&state, MRG_MODE_GAME);

  // exec startup commands
  for (size_t i = 0; i < cfg->exec_len; i++) {
    mrg_cmd_exec(stdout, mrg_std_fputs, cfg->exec[i], mrg_cmd_tbl, &state);
  }

  return state;
}

void mrg_state_free(struct mrg_state *state) {
  mrg_arena_free(&state->room_arena);
  mrg_arena_free(&state->idc_arena);
  mrg_arena_free(&state->tmp_arena);
  mrg_map_free(&state->map);

  for (size_t i = 0; i < state->tile_tbl.len; i++) {
    mrg_tile_set_free(&state->tile_tbl, state->platform, (int)i);
  }

  mrg_console_free(&state->console);

  mrg_idc_free(&state->idc);
}

int mrg_transition(struct mrg_state *state, enum mrg_mode mode) {
  switch (state->mode) {
  default:
    break;
  }

  // do not transition when we were just in console mode...
  if (state->mode != MRG_MODE_CONSOLE || state->console.prev != mode) {
    switch (mode) {
    case MRG_MODE_MAPED:
      state->maped = mrg_maped_init(state);
      break;
    case MRG_MODE_CONSOLE:
      state->console.prev = state->mode;
      // clear keyboard buffer before entering console
      while (mrg_pl_char_pressed(state->platform)) {
      }
      break;
    default:
      break;
    }
  }

  state->mode = mode;

  return 0;
}

void mrg_state_invalidate(struct mrg_state *state) {}

void mrg_toggle_dbg(struct mrg_state *state) {
  mrg_pl_toggle_dbg(state->platform);
}

int mrg_main(struct mrg_config *cfg) {
  if (!cfg->verbose) {
    fclose(stdout);
  }

  mrg_platform platform = mrg_platform_init(cfg);
  if (mrg_platform_good(&platform) == -1) {
    fprintf(stderr, "Unable to init window!\n");
    return -1;
  }

  struct mrg_state state = mrg_state_init(cfg, &platform);
  if (state.good == -1) {
    fprintf(stderr, "Setup failed!\n");
    return -1;
  }

  mrg_main_loop(&state);

  mrg_state_free(&state);
  mrg_platform_free(&platform);

  return 0;
}
