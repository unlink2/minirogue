#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "mrg.h"
#include "draw.h"
#include <stdbool.h>

typedef struct {
  void *data;
} mrg_platform;

// platform specific interfaces goes here...

mrg_platform mrg_platform_init(struct mrg_config *cfg);

bool mrg_video_open(mrg_platform *platform);

int mrg_video_draw_pixel(mrg_platform *platform, int x, int y, struct mrg_color color);

int mrg_video_begin(mrg_platform *platform);
int mrg_video_end(mrg_platform *platform);

void mrg_platform_free(mrg_platform *platform);

#endif
