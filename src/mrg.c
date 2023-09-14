#include "mrg.h"
#include "defaults.h"
#include "draw.h"
#include "entity.h"
#include "platform.h"
#include "tiles.h"
#include <stdio.h>
#include <string.h>

int mrg_mode_maped(struct mrg_state *state) { return 0; }

int mrg_mode_game(struct mrg_state *state) {
  mrg_pl_video_draw_pixel(state->platform, 0, 0, MRG_WHITE);
  return 0;
}

int mrg_main_loop(struct mrg_state *state) {
  mrg_platform *platform = state->platform;

  // TEST CODE for entity
  int player = mrg_entity_alloc(&state->entity_tbl);
  mrg_entity_init_player(&state->entity_tbl.slots[player]);

  while (mrg_pl_video_open(platform)) {
    // input
    mrg_input_poll(state, &state->main_input);

    // update
    if (mrg_map_update(state, &state->map) == -1) {
      fprintf(stderr, "Map update failed!\n");
    }

    if (mrg_entity_tbl_update(state, &state->entity_tbl) == -1) {
      fprintf(stderr, "Entity update failed!\n");
    }

    mrg_camera_update(state, &state->main_camera);

    state->mode_update(state);

    // draw
    mrg_pl_video_begin(platform);

    mrg_pl_camera_begin(platform, &state->main_camera);

    mrg_map_draw(state, &state->map);
    mrg_entity_tbl_draw(state, &state->entity_tbl);
    mrg_pl_camera_end(platform, &state->main_camera);

    mrg_pl_draw_debug(state->platform);

    mrg_pl_video_end(platform);
  }
  return 0;
}

struct mrg_state mrg_state_init(struct mrg_config *cfg,
                                mrg_platform *platform) {
  struct mrg_state state;
  memset(&state, 0, sizeof(state));
  state.platform = platform;
  state.room_arena = mrg_arena_init(4096);

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

  struct mrg_idc_file idc = mrg_default_idc();
  state.room_tbl = mrg_room_tbl_from_idc(&state, &state.room_arena, &idc);
  // TODO: dynamically load rooms!
  state.map = mrg_map_init(&state, state.room_tbl.graph.rooms[0]);

  mrg_transition(&state, MRG_MODE_GAME);

  return state;
}

void mrg_state_free(struct mrg_state *state) {
  mrg_arena_free(&state->room_arena);
}

int mrg_transition(struct mrg_state *state, enum mrg_mode mode) {
  state->mode = mode;
  switch (state->mode) {
  case MRG_MODE_GAME:
    state->mode_update = mrg_mode_game;
    break;
  case MRG_MODE_MAPED:
    state->mode_update = mrg_mode_maped;
    break;
  }

  return 0;
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
