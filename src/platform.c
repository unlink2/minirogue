#include "platform.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

mrg_platform mrg_platform_init(struct mrg_config *cfg) {
  mrg_platform platform;
  memset(&platform, 0, sizeof(platform));

  InitWindow(800, 450, "mrg");
  SetTargetFPS(60);
  return platform;
}

bool mrg_video_open(mrg_platform *platform) { return !WindowShouldClose(); }

int mrg_video_begin(mrg_platform *platform) {
  BeginDrawing();

  ClearBackground(BLACK);
  return 0;
}

int mrg_video_draw_pixel(mrg_platform *platform, int x, int y, struct mrg_color color) {
  DrawPixel(x, y, (Color){ color.r, color.g, color.b, color.a});

  return 0;
}

int mrg_video_end(mrg_platform *platform) {
  EndDrawing();
  return 0;
}

void mrg_platform_free(mrg_platform *platform) { CloseWindow(); }


