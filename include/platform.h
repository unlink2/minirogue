#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "mrg.h"
#include "draw.h"
#include "defs.h"
#include "arena.h"
#include <stdbool.h>
#include <stdint.h>

#define MRG_DIR_PATH_SEP "/"

/**
 * All platform specific APIs go here
 * DO NOT rely on mrg_platform outside of platfomr specific APIs!
 */

#define MRG_BACKEND_RAYLIB

#ifdef MRG_BACKEND_RAYLIB

#include <raylib.h>

typedef struct mrg_platform {
  int good;
  int screen_w;
  int screen_h;
  Camera2D cameras[1];
  RenderTexture2D target;

  bool draw_debug;

  const char *base_assets_path;

  struct mrg_arena arena;
} mrg_platform;

// join paths
char *mrg_join(struct mrg_arena *a, const char *dst, const char *path_sep,
               const char *suffix);

#else
typedef struct {
  void *data;
} mrg_platform;
#endif

mrg_platform mrg_platform_init(struct mrg_config *cfg);

// return >= 0 on success and -1 if the platform did not initialize correctly
int mrg_platform_good(mrg_platform *platform);
void mrg_platform_free(mrg_platform *platform);

/**
 * Platform specific video api
 */

bool mrg_pl_video_open(mrg_platform *platform);

int mrg_pl_video_draw_pixel(mrg_platform *platform, int x, int y,
                            struct mrg_color color);
int mrg_pl_video_begin(mrg_platform *platform);
int mrg_pl_video_end(mrg_platform *platform);

void mrg_pl_draw_debug(mrg_platform *platform);

void mrg_pl_draw_debug_rec(mrg_platform *platform, int x, int y, int w, int h,
                           struct mrg_color color);


void mrg_pl_print(mrg_platform *platform, const char *text, int x, int y,
                  int font_size, struct mrg_color color);

/**
 * Platform specific camera details
 */

// create a camera and return a handle or -1 on error
struct mrg_camera mrg_pl_camera_init(mrg_platform *platform);
int mrg_pl_camera_begin(mrg_platform *platform, struct mrg_camera *camera);
int mrg_pl_camera_target(mrg_platform *platform, struct mrg_camera *camera,
                         int x, int y);
int mrg_pl_camera_offset(mrg_platform *platform, struct mrg_camera *camera,
                         int w, int h);
int mrg_pl_camera_end(mrg_platform *platform, struct mrg_camera *camera);

void mrg_pl_camera_world_to_screen(struct mrg_platform *platform,
                                   struct mrg_camera *camera, int ix, int iy,
                                   int *ox, int *oy);

void mrg_pl_camera_screen_to_world(struct mrg_platform *platform,
                                   struct mrg_camera *camera, int ix, int iy,
                                   int *ox, int *oy);

void mrg_pl_camera_bounds(mrg_platform *platform, struct mrg_camera *camera,
                          int *x, int *y, int *w, int *h);

/**
 * Input devices
 */

struct mrg_input mrg_pl_input_init(void);
uint16_t mrg_pl_input_poll(mrg_platform *platform, int handle);

int mrg_pl_char_pressed(mrg_platform *platform);

/**
 * File system
 */

// read an entire file into a buffer
char *mrg_pl_fread(struct mrg_arena *a, const char *path, size_t *len);

// write file to asset path
size_t mrg_pl_fwrite(const char *path, const char *data, size_t len);

/**
 * Tiles
 */

int mrg_pl_tile_set_load(struct mrg_tile_set *set,
                         struct mrg_platform *platform, const char *path);

void mrg_pl_tile_set_free(struct mrg_tile_set *set,
                          struct mrg_platform *platform, int handle);

void mrg_pl_tile_draw(struct mrg_tile_set *set, struct mrg_platform *platform,
                      int tile, int x, int y);

/**
 * Platform specific collision code
 */

bool mrg_pl_col_recs(int x1, int y1, int w1, int h1, int x2, int y2, int w2,
                     int h2);

#endif
