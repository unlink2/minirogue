#include "map.h"
#include "platform.h"
#include "rand.h"
#include "tiles.h"
#include "mrg.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "idc.h"
#include "room.h"

struct mrg_map mrg_map_init(struct mrg_state *state, int room_handle) {
  struct mrg_map map;
  memset(&map, 0, sizeof(map));

  struct mrg_room *room = state->room_tbl.rooms[room_handle];
  assert(room);
  assert(state);

  map.room = room;
  map.tileset_id = room->tileset_id;

  if (room->ok == -1 || map.tileset_id == -1) {
    map.ok = -1;
  }

  map.tile_w = state->tile_tbl.sets[map.tileset_id].tile_w;
  map.tile_h = state->tile_tbl.sets[map.tileset_id].tile_h;

  map.room_handle = room_handle;
  map.x = 0;
  map.y = 0;
  map.w = room->room_w;
  map.h = room->room_h;

  map.global_light = 0x79;

  size_t tiles = map.w * map.h;
  map.light = malloc(tiles * sizeof(int8_t));

#ifdef MRG_DEBUG
  map.dbg_flags = malloc(tiles * sizeof(bool));
#endif

  map.tile_w = 16;
  map.tile_h = 16;

  return map;
}

int mrg_map_update(struct mrg_state *state, struct mrg_map *map) {
  if (map->ok == -1) {
    return -1;
  }

#ifdef MRG_DEBUG
  memset(map->dbg_flags, 0, map->w * map->h);
#endif

  return 0;
}

enum mrg_map_flags mrg_map_collision(struct mrg_map *map, int x, int y, int w,
                                     int h) {
  // start in the top left, step in tile-sized increments
  // until bottom right corner and check every tile along the way
  enum mrg_map_flags result = 0;

  // for tile collision we at least need to check
  // for a w/h of a tile
  // h = MAX(h, map->tile_h);
  // w = MAX(w, map->tile_w);

  int step_x = MIN(map->tile_w, w);
  int step_y = MIN(map->tile_h, h);

  for (int iy = y; iy <= y + h; iy += step_y) {
    for (int ix = x; ix <= x + w; ix += step_x) {
      int tx = 0;
      int ty = 0;
      mrg_map_to_tile(map, (int)ix, (int)iy, &tx, &ty);

      // out of bounds!
      if (tx > map->w || ty > map->h || tx < 0 || ty < 0) {
        continue;
      }

      int tile = MRG_MAP_COORDS_TO_TILE(map, tx, ty);
      result |= map->room->flags[tile];
#ifdef MRG_DEBUG
      map->dbg_flags[tile] |= MRG_MAP_DBG_FLAG_DID_COLLIDE;
#endif
    }
  }

  return result;
}

void mrg_map_tile_set(struct mrg_map *map, int x, int y, int8_t tile) {
  int xi = -1;
  int yi = -1;
  mrg_map_to_tile(map, x, y, &xi, &yi);
  int update_index = MRG_MAP_COORDS_TO_TILE(map, xi, yi);

  if (update_index < 0 ||
      update_index >= map->room->room_w * map->room->room_h) {
    return;
  }

  map->room->tiles[update_index] = tile;
}

void mrg_map_flag_set(struct mrg_map *map, int x, int y, int8_t flag) {
  int xi = -1;
  int yi = -1;
  mrg_map_to_tile(map, x, y, &xi, &yi);
  int update_index = MRG_MAP_COORDS_TO_TILE(map, xi, yi);

  if (update_index < 0 ||
      update_index >= map->room->room_w * map->room->room_h) {
    return;
  }

  map->room->flags[update_index] = flag;
}

int mrg_map_draw(struct mrg_state *state, struct mrg_map *map) {
  if (map->ok == -1) {
    return -1;
  }

  int camera_x = 0;
  int camera_y = 0;
  int camera_w = 0;
  int camera_h = 0;

  map->w = map->room->room_w;
  map->h = map->room->room_h;

  mrg_camera_bounds(state, &state->main_camera, &camera_x, &camera_y, &camera_w,
                    &camera_h);
  // start tile that is on screen
  int start_tile_x = 0;
  int start_tile_y = 0;
  mrg_map_to_tile(map, camera_x, camera_y, &start_tile_x, &start_tile_y);
  start_tile_x = MAX(0, start_tile_x);
  start_tile_y = MAX(0, start_tile_y);

  int start_tx = start_tile_x * map->tile_w + map->x;
  int tx = start_tx;
  int ty = start_tile_y * map->tile_h + map->y;

  // end tile that is on screen
  int end_h = (int)map->h;
  int end_w = (int)map->w;
  mrg_map_to_tile(map, camera_x + camera_w, camera_y + camera_h, &end_w,
                  &end_h);
  end_w = MIN(map->w, end_w + 1);
  end_h = MIN(map->h, end_h + 1);

  for (size_t y = start_tile_y; y < end_h; y++) {
    for (size_t x = start_tile_x; x < end_w; x++) {
      size_t tile = MRG_MAP_COORDS_TO_TILE(map, x, y);

      int hflip = 1;
      int vflip = 1;

      if (map->room->flags[tile] & MRG_MAP_FLAG_HFLIP) {
        hflip = -1;
      }

      if (map->room->flags[tile] & MRG_MAP_FLAG_VFLIP) {
        vflip = -1;
      }

      uint8_t tile_handle = map->room->tiles[tile];

      if (map->room->flags[tile] & MRG_MAP_FLAG_GLOBAL_ANIMATION) {
        tile_handle += state->alt_anim;
      }
      mrg_tile_draw_adv(&state->tile_tbl, state->platform, map->tileset_id,
                        tile_handle, tx, ty, hflip, vflip, 1, 1, MRG_WHITE);

#ifdef MRG_DEBUG
      if (map->room->flags[tile]) {
        uint8_t c = map->room->flags[tile];
        mrg_pl_draw_debug_rec(state->platform, tx, ty, map->tile_w, map->tile_h,
                              (struct mrg_color){~c, c, 0, 255});
      }

      if (map->dbg_flags[tile] & MRG_MAP_DBG_FLAG_DID_COLLIDE) {
        mrg_pl_draw_debug_rec(state->platform, tx, ty, map->tile_w, map->tile_h,
                              (struct mrg_color){255, 20, 150, 0xA0});
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
  *xo = (xi - map->x) / map->tile_w;
  *yo = (yi - map->y) / map->tile_h;
}

void mrg_map_free(struct mrg_map *map) {
  if (map->ok == -1) {
    return;
  }
  map->ok = -1;
  free(map->light);
  map->light = NULL;
#ifdef MRG_DEBUG
  free(map->dbg_flags);
  map->dbg_flags = NULL;
#endif
}
