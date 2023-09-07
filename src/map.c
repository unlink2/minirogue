#include "map.h"
#include "platform.h"
#include "tiles.h"
#include "mrg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mrg_map mrg_map_init(void) {
  struct mrg_map map;
  memset(&map, 0, sizeof(map));

  map.w = MRG_LAYER_W;
  map.h = MRG_LAYER_H;

  size_t tiles = map.w * map.h;

  map.flags = malloc(tiles * sizeof(int8_t));
  map.tiles = malloc(tiles * sizeof(int8_t));
  map.light = malloc(tiles * sizeof(int8_t));

  map.tile_w = 16;
  map.tile_h = 16;

  return map;
}

int mrg_map_update(struct mrg_state *state, struct mrg_map *map) { return 0; }

enum mrg_map_flags mrg_map_collision(struct mrg_map *map, int x, int y, int w,
                                     int h) {
  return 0;
}

int mrg_map_draw(struct mrg_state *state, struct mrg_map *map) {
  map->tile_w = state->tile_tbl.sets[map->tileset_id].tile_w;
  map->tile_h = state->tile_tbl.sets[map->tileset_id].tile_h;

  int camera_x = 0;
  int camera_y = 0;
  int camera_w = 0;
  int camera_h = 0;

  mrg_camera_bounds(state, &state->main_camera, &camera_x, &camera_y, &camera_w,
                    &camera_h);

  // start tile that is on screen
  int start_tile_x = 0;
  int start_tile_y = 0;
  mrg_map_to_tile(map, camera_x, camera_y, &start_tile_x, &start_tile_y);
  start_tile_x = MAX(0, start_tile_x);
  start_tile_y = MAX(0, start_tile_y);

  int start_tx = start_tile_x * map->tile_w;
  int tx = start_tx;
  int ty = start_tile_y * map->tile_h;

  // end tile that is on screen
  int end_h = (int)map->h;
  int end_w = (int)map->w;
  mrg_map_to_tile(map, camera_x + camera_w, camera_y + camera_h, &end_w,
                  &end_h);
  end_w = MIN(map->w, end_w + 1);
  end_h = MIN(map->h, end_h + 1);

  for (size_t y = start_tile_y; y < end_h; y++) {
    for (size_t x = start_tile_x; x < end_w; x++) {
      size_t tile = y * map->w + x;
      mrg_tile_draw(&state->tile_tbl, state->platform, map->tileset_id,
                    map->tiles[tile], tx, ty);

#ifdef MRG_DEBUG
      if (map->flags[tile] & MRG_MAP_FLAG_COLLISION) {
        mrg_pl_draw_debug_rec(state->platform, tx, ty, map->tile_w, map->tile_h,
                              (struct mrg_color){255, 0, 0, 255});
      }

      if (map->flags[tile] & MRG_MAP_FLAG_DAMAGE) {
        mrg_pl_draw_debug_rec(state->platform, tx, ty, map->tile_w, map->tile_h,
                              (struct mrg_color){255, 255, 0, 255});
      }
#endif

      tx += map->tile_w;
    }
    tx = start_tx;
    ty += map->tile_h;
  }

  return 0;
}

void mrg_map_to_tile(struct mrg_map *map, int xi, int yi, int *xo, int *yo) {
  *xo = xi / map->tile_w;
  *yo = yi / map->tile_h;
}

void mrg_map_free(struct mrg_map *map) {
  free(map->tiles);
  free(map->flags);
  free(map->light);
}
