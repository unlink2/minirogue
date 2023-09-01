#include "mrg.h"
#include "draw.h"
#include "platform.h"
#include "tiles.h"
#include <stdio.h>
#include <string.h>

int mrg_main_loop(struct mrg_state *state) {
  mrg_platform *platform = state->platform;

  // test texture 
  mrg_tile_set_load(&state->tile_tbl, state->platform, "./assets/debugset.png", 16, 16);

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

    // draw
    mrg_pl_video_begin(platform);

    mrg_pl_camera_begin(platform, &state->main_camera);

    mrg_pl_video_draw_pixel(platform, 0, 0, MRG_WHITE);
    mrg_tile_draw(&state->tile_tbl, state->platform, 0, 0, 10, 10);

    mrg_pl_camera_end(platform, &state->main_camera);

    mrg_pl_video_end(platform);
  }
  return 0;
}

int mrg_main(struct mrg_config *cfg) {
  mrg_platform platform = mrg_platform_init(cfg);

  // TODO: directd stderr to /dev/NULL if 
  // verbose is turned off

  struct mrg_state state;
  memset(&state, 0, sizeof(state));
  state.main_camera = mrg_pl_camera_init(&platform);
  state.platform = &platform;
  state.cfg = cfg;
  state.main_input = mrg_pl_input_init();
  state.tile_tbl = mrg_tile_set_tbl_init();

  mrg_main_loop(&state);
  mrg_platform_free(&platform);

  return 0;
}
