#include "mrg.h"
#include "draw.h"
#include "platform.h"
#include <stdio.h>

int mrg_main_loop(struct mrg_config *cfg, mrg_platform *platform) {
  while (mrg_video_open(platform)) {
    // input 

    // update 

    // draw
    mrg_video_begin(platform);
    mrg_video_draw_pixel(platform, 0, 0, MRG_WHITE);
    mrg_video_end(platform);
  }
  return 0;
}

int mrg_main(struct mrg_config *cfg) {
  mrg_platform platform = mrg_platform_init(cfg);
  mrg_main_loop(cfg, &platform);
  mrg_platform_free(&platform);

  return 0;
}
