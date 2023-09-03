#include "mrg.h"
#include "draw.h"
#include "entity.h"
#include "platform.h"
#include "tiles.h"
#include <stdio.h>
#include <string.h>

int mrg_main_loop(struct mrg_state *state) {
  mrg_platform *platform = state->platform;

  // test texture
  mrg_tile_set_load(&state->tile_tbl, state->platform, "debugset.png", 16, 16);

  // TEST CODE for entity
  int player = mrg_entity_alloc(&state->entity_tbl);
  mrg_entity_init_player(&state->entity_tbl.slots[player]);

  while (mrg_pl_video_open(platform)) {
    // input
    mrg_input_poll(state, &state->main_input);

    // update

    if (MRG_PRESSED(&state->main_input, MRG_ACTION_UP)) {
      printf("pressed!\n");
    }

    if (MRG_HELD(&state->main_input, MRG_ACTION_UP)) {
      printf("held!\n");
    }
    if (mrg_entity_tbl_update(state, &state->entity_tbl) == -1) {
      fprintf(stderr, "Entity update failed!\n");
    }

    // draw
    mrg_pl_video_begin(platform);

    mrg_pl_camera_begin(platform, &state->main_camera);

    mrg_pl_video_draw_pixel(platform, 0, 0, MRG_WHITE);
    mrg_tile_draw(&state->tile_tbl, state->platform, 0, 34, 10, 10);

    mrg_pl_camera_end(platform, &state->main_camera);

    mrg_pl_video_end(platform);
  }
  return 0;
}

struct mrg_state mrg_state_init(struct mrg_config *cfg,
                                mrg_platform *platform) {

  // TODO: directd stderr to /dev/NULL if
  // verbose is turned off

  struct mrg_state state;
  memset(&state, 0, sizeof(state));
  state.platform = platform;

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

  return state;
}

void mrg_state_free(struct mrg_state *state) {}

int mrg_main(struct mrg_config *cfg) {
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
