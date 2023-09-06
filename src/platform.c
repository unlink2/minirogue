#include "platform.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "camera.h"
#include "input.h"
#include "tiles.h"

char *mrg_join(char *dst, const char *path_sep, const char *suffix) {
  if (!dst || !suffix || !path_sep) {
    return NULL;
  }

  size_t len = strlen(dst) + strlen(suffix) + strlen(path_sep) + 1;
  char *new_dst = realloc(dst, len);
  strcat(new_dst, path_sep);
  strcat(new_dst, suffix);
  return new_dst;
}

#ifdef MRG_BACKEND_RAYLIB

mrg_platform mrg_platform_init(struct mrg_config *cfg) {
  mrg_platform platform;
  memset(&platform, 0, sizeof(platform));

  platform.screen_w = 800;
  platform.screen_h = 600;
  platform.draw_debug = cfg->verbose;

  // SetConfigFlags(FLAG_WINDOW_RESIZABLE);

  // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(platform.screen_w, platform.screen_h, "mrg");

  platform.target = LoadRenderTexture(platform.screen_w, platform.screen_h);

  SetTargetFPS(60);

  if (!IsWindowReady()) {
    platform.good = -1;
  }

  return platform;
}

int mrg_platform_good(mrg_platform *platform) { return platform->good; }

bool mrg_pl_video_open(mrg_platform *platform) { return !WindowShouldClose(); }

int mrg_pl_video_begin(mrg_platform *platform) {
  BeginTextureMode(platform->target);

  ClearBackground(BLACK);

  return 0;
}

struct mrg_camera mrg_pl_camera_init(mrg_platform *platform) {
  platform->cameras[0] = (Camera2D){0};
  platform->cameras[0].offset = (Vector2){(float)platform->screen_w / 2.0F,
                                          (float)platform->screen_h / 2.0F};
  platform->cameras[0].rotation = 0.0F;
  platform->cameras[0].zoom = 2.0F;

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

void mrg_pl_draw_debug(mrg_platform *platform) {
  if (platform->draw_debug) {
    char dbg[128];
    sprintf(dbg, "%d fps, %f ms", GetFPS(), GetFrameTime());
    DrawText(dbg, 10, 10, 20, WHITE);
  }
}

int mrg_pl_video_end(mrg_platform *platform) {
  EndTextureMode();
  BeginDrawing();

  // TODO: Keep aspect ratio of original source
  DrawTexturePro(
      platform->target.texture,
      (Rectangle){0, 0, (float)platform->target.texture.width,
                  (float)-platform->target.texture.height},
      (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
      (Vector2){0, 0}, 0, WHITE);
  EndDrawing();
  return 0;
}

int mrg_pl_camera_target(mrg_platform *platform, struct mrg_camera *camera,
                         int x, int y) {
  platform->cameras[camera->handle].target = (Vector2){(float)x, (float)y};
  return 0;
}

int mrg_pl_camera_offset(mrg_platform *platform, struct mrg_camera *camera,
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

void mrg_pl_camera_bounds(mrg_platform *platform, struct mrg_camera *camera,
                          int *x, int *y, int *w, int *h) {
  Camera2D *c2d = &platform->cameras[camera->handle];

  *x = (int)c2d->target.x - (int)c2d->offset.x;
  *y = (int)c2d->target.y - (int)c2d->offset.y;

  *w = (int)((float)platform->screen_w / c2d->zoom);
  *h = (int)((float)platform->screen_h / c2d->zoom);
}

void mrg_platform_free(mrg_platform *platform) {
  UnloadRenderTexture(platform->target);
  CloseWindow();
}
void mrg_pl_camera_world_to_screen(struct mrg_platform *platform,
                                   struct mrg_camera *camera, int ix, int iy,
                                   int *ox, int *oy) {
  Vector2 res = GetWorldToScreen2D((Vector2){(float)ix, (float)iy},
                                   platform->cameras[camera->handle]);
  *ox = (int)res.x;
  *oy = (int)res.y;
}

void mrg_pl_camera_screen_to_world(struct mrg_platform *platform,
                                   struct mrg_camera *camera, int ix, int iy,
                                   int *ox, int *oy) {
  Vector2 res = GetScreenToWorld2D((Vector2){(float)ix, (float)iy},
                                   platform->cameras[camera->handle]);
  *ox = (int)res.x;
  *oy = (int)res.y;
}

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
  char *asset_path = "./assets";
  // TODO: allow env to set paths here

  char *real_path = mrg_join(strdup(asset_path), MRG_DIR_PATH_SEP, path);

  set->data = malloc(sizeof(Texture2D));

  Texture2D texture = LoadTexture(real_path);
  *(Texture2D *)set->data = texture;

  free(real_path);
  if (texture.id <= 0) {
    return -1;
  }

  return 0;
}

void mrg_pl_tile_set_free(struct mrg_tile_set *set,
                          struct mrg_platform *platform, int handle) {
  UnloadTexture(*(Texture2D *)set->data);
  free(set->data);
}

void mrg_pl_tile_draw(struct mrg_tile_set *set, struct mrg_platform *platform,
                      int tile, int x, int y) {
  Texture2D texture = *(Texture2D *)set->data;
  Rectangle source = {
      (float)mrg_tile_img_x(tile, texture.width, set->tile_w),
      (float)mrg_tile_img_y(tile, texture.width, set->tile_w, set->tile_h),
      (float)set->tile_w, (float)set->tile_h};
  Vector2 position = {(float)x, (float)y};

  DrawTextureRec(texture, source, position, WHITE);
}

#endif
