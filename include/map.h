#ifndef MAP_H_
#define MAP_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "defs.h"
#include "room.h"

#define MRG_LAYER_W 16
#define MRG_LAYER_H 16
#define MRG_LAYER_LEN (MRG_LAYER_W * MRG_LAYER_H)

#define MRG_MAP_COORDS_TO_TILE(map, x, y) (y) * (map)->w + (x)

enum mrg_map_flags {
  MRG_MAP_FLAG_COLLISION = 1,
  MRG_MAP_FLAG_DAMAGE = 2,
  MRG_MAP_FLAG_VFLIP = 4,
  MRG_MAP_FLAG_HFLIP = 8
};

enum mrg_map_dbg_flags { MRG_MAP_DBG_FLAG_DID_COLLIDE = 1 };

struct mrg_map {
  int ok;
  int tileset_id;

  int room_handle;
  struct mrg_room *room;

  int8_t *light;
  int8_t *dbg_flags;

  int x;
  int y;
  size_t w;
  size_t h;
  int tile_w;
  int tile_h;
};

struct mrg_map mrg_map_init(struct mrg_state *state, int room_handle);

int mrg_map_update(struct mrg_state *state, struct mrg_map *map);
int mrg_map_draw(struct mrg_state *state, struct mrg_map *map);

void mrg_map_to_tile(struct mrg_map *map, int xi, int yi, int *xo, int *yo);
enum mrg_map_flags mrg_map_collision(struct mrg_map *map, int x, int y, int w,
                                     int h);

void mrg_map_tile_set(struct mrg_map *map, int x, int y, int8_t tile);
void mrg_map_flag_set(struct mrg_map *map, int x, int y, int8_t flag);

void mrg_map_free(struct mrg_map *map);

#endif
