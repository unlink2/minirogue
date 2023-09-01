#include "platform.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "camera.h"
#include "input.h"

#ifdef MRG_BACKEND_RAYLIB
mrg_platform mrg_platform_init(struct mrg_config *cfg) {
  mrg_platform platform;
  memset(&platform, 0, sizeof(platform));

  platform.screen_w = 800;
  platform.screen_h = 450;

  // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(800, 450, "mrg");

  SetTargetFPS(60);
  return platform;
}

bool mrg_pl_video_open(mrg_platform *platform) { return !WindowShouldClose(); }

int mrg_pl_video_begin(mrg_platform *platform) {
  BeginDrawing();

  ClearBackground(BLACK);
  return 0;
}

struct mrg_camera mrg_pl_camera_init(mrg_platform *platform) {
  platform->cameras[0] = (Camera2D){0};
  platform->cameras[0].offset = (Vector2){(float)platform->screen_w / 2.0F,
                                          (float)platform->screen_h / 2.0F};
  platform->cameras[0].rotation = 0.0F;
  platform->cameras[0].zoom = 1.0F;

  struct mrg_camera camera;
  memset(&camera, 0, sizeof(camera));

  // currently theres only one camera...
  camera.handle = 0;

  return camera;
}

int mrg_pl_video_draw_pixel(mrg_platform *platform, int x, int y,
                            struct mrg_color color) {
  DrawPixel(x, y, (Color){color.r, color.g, color.b, color.a});

  return 0;
}

int mrg_pl_video_end(mrg_platform *platform) {
  EndDrawing();
  return 0;
}

int mrg_pl_camera_target(mrg_platform *platform, struct mrg_camera *camera,
                         int x, int y) {
  platform->cameras[camera->handle].target = (Vector2){(float)x, (float)y};
  return 0;
}

int marg_pl_camera_offset(mrg_platform *platform, struct mrg_camera *camera,
                          int w, int h) {
  platform->cameras[camera->handle].offset = (Vector2){(float)w, (float)h};
  return 0;
}

int mrg_pl_camera_begin(mrg_platform *platform, struct mrg_camera *camera) {
  BeginMode2D(platform->cameras[camera->handle]);
  return 0;
}

int mrg_pl_camera_end(mrg_platform *platform, struct mrg_camera *camera) {
  EndMode2D();
  return 0;
}

void mrg_platform_free(mrg_platform *platform) { CloseWindow(); }

struct mrg_input mrg_pl_input_init(void) { return mrg_input_init(0); }

uint16_t mrg_pl_input_poll(mrg_platform *platform, int handle) {
  uint16_t input_state = 0;

  if (IsKeyDown(KEY_W)) {
    input_state |= MRG_ACTION_UP;
  }
  if (IsKeyDown(KEY_S)) {
    input_state |= MRG_ACTION_DOWN;
  }
  if (IsKeyDown(KEY_A)) {
    input_state |= MRG_ACTION_LEFT;
  }
  if (IsKeyDown(KEY_D)) {
    input_state |= MRG_ACTION_RIGHT;
  }
  return input_state;
}

int mrg_pl_tile_set_load(struct mrg_tile_set *set,
                         struct mrg_platform *platform, const char *path) {
  return -1;
}

void mrg_pl_tile_set_free(struct mrg_tile_set *set, struct mrg_platform *platform,
                       int handle) {}

#endif
