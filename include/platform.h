#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "mrg.h"
#include "draw.h"
#include <stdbool.h>

#define MRG_BACKEND_RAYLIB 

#ifdef MRG_BACKEND_RAYLIB 

#include <raylib.h>

typedef struct mrg_platform {
  int screen_w;
  int screen_h;
  Camera2D cameras[1];
} mrg_platform;

#else 
typedef struct {
  void *data;
} mrg_platform;
#endif 

// platform specific interfaces goes here...

mrg_platform mrg_platform_init(struct mrg_config *cfg);

bool mrg_video_open(mrg_platform *platform);

int mrg_video_draw_pixel(mrg_platform *platform, int x, int y, struct mrg_color color);
int mrg_video_begin(mrg_platform *platform);
int mrg_video_end(mrg_platform *platform);

// create a camera and return a handle or -1 on error 
int mrg_camera_init(mrg_platform *platform);
int mrg_camera_begin(mrg_platform *platform, int camera);
int marg_camera_target(mrg_platform *platform, int camera, int x, int y);
int marg_camera_offset(mrg_platform *platform, int camera, int w, int h);
int mrg_camera_end(mrg_platform *platform, int camera);

void mrg_platform_free(mrg_platform *platform);


#endif
