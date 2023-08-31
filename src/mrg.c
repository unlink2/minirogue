#include "mrg.h"
#include "draw.h"
#include "platform.h"
#include <stdio.h>
#include <string.h>

int mrg_main_loop(struct mrg_state *state) {
  mrg_platform *platform = state->platform;

  while (mrg_pl_video_open(platform)) {
    // input

    // update

    // draw
    mrg_pl_video_begin(platform);

    mrg_pl_camera_begin(platform, &state->main_camera);

    mrg_pl_video_draw_pixel(platform, 0, 0, MRG_WHITE);

    mrg_pl_camera_end(platform, &state->main_camera);

    mrg_pl_video_end(platform);
  }
  return 0;
}

int mrg_main(struct mrg_config *cfg) {
  mrg_platform platform = mrg_platform_init(cfg);

  struct mrg_state state;
  memset(&state, 0, sizeof(state));
  state.main_camera = mrg_pl_camera_init(&platform);
  state.platform = &platform;
  state.cfg = cfg;

  mrg_main_loop(&state);
  mrg_platform_free(&platform);

  return 0;
}
