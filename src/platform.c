#include "platform.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

mrg_platform mrg_video_init(struct mrg_config *cfg) {
  mrg_platform platform;
  memset(&platform, 0, sizeof(platform));

  InitWindow(800, 450, "raylib [core] example - basic window");
  SetTargetFPS(60);
  return platform;
}

bool mrg_video_open(mrg_platform *platform) { return !WindowShouldClose(); }

int mrg_video_begin(mrg_platform *platform) {
  BeginDrawing();

  ClearBackground(RAYWHITE);
  return 0;
}

int mrg_video_end(mrg_platform *platform) {
  EndDrawing();
  return 0;
}

void mrg_video_free(mrg_platform *platform) { CloseWindow(); }
