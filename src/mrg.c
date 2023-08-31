#include "mrg.h"
#include "platform.h"
#include <stdio.h>

int mrg_main_loop(struct mrg_config *cfg, mrg_platform *platform) {
  while (mrg_video_open(platform)) {
    // input 

    // update 

    // draw
    mrg_video_begin(platform);
    mrg_video_end(platform);
  }
  return 0;
}

int mrg_main(struct mrg_config *cfg) {
  mrg_platform platform = mrg_video_init(cfg);
  mrg_main_loop(cfg, &platform);
  mrg_video_free(&platform);

  return 0;
}
