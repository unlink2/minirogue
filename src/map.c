#include "map.h"
#include "tiles.h"
#include "mrg.h"
#include <stdio.h>
#include <string.h>

struct mrg_map mrg_map_init(void) {
  struct mrg_map map;
  memset(&map, 0, sizeof(map));

  map.w = MRG_LAYER_W;
  map.h = MRG_LAYER_H;

  map.tile_w = 16;
  map.tile_h = 16;

  return map;
}

int mrg_map_update(struct mrg_state *state, struct mrg_map *map) { return 0; }

int mrg_map_draw(struct mrg_state *state, struct mrg_map *map) {
  // FIXME: implement culling, for now literally just draw everything
  size_t tile = 0;

  map->tile_w = state->tile_tbl.sets[map->tileset_id].tile_w;
  map->tile_h = state->tile_tbl.sets[map->tileset_id].tile_h;

  int camera_x = 0;
  int camera_y = 0;
  int camera_w = 0;
  int camera_h = 0;

  mrg_camera_bounds(state, &state->main_camera, &camera_x, &camera_y, &camera_w,
                    &camera_h);

  int start_tile_x = 0;
  int start_tile_y = 0;
  mrg_map_to_tile(map, camera_x, camera_y, &start_tile_x, &start_tile_y);
  start_tile_x = MAX(0, start_tile_x);
  start_tile_y = MAX(0, start_tile_y);
  printf("start tile: %d/%d\n", start_tile_x, start_tile_y);

  int tx = 0;
  int ty = 0;

  size_t end_h = map->h;
  size_t end_w = map->w;

  for (size_t y = start_tile_y; y < end_h; y++) {
    for (size_t x = start_tile_x; x < end_w; x++) {
      mrg_tile_draw(&state->tile_tbl, state->platform, map->tileset_id,
                    map->tiles[tile], tx, ty);
      tile++;
      tx += map->tile_w;
    }
    tx = 0;
    ty += map->tile_h;
  }

  return 0;
}

void mrg_map_to_tile(struct mrg_map *map, int xi, int yi, int *xo, int *yo) {
  *xo = xi / map->tile_w;
  *yo = yi / map->tile_h;
}

void mrg_map_free(struct mrg_map *map) {}
