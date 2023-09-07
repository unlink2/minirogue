#ifndef MAP_H_
#define MAP_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "defs.h"

#define MRG_LAYER_W 256
#define MRG_LAYER_H 256 
#define MRG_LAYER_LEN (MRG_LAYER_W * MRG_LAYER_H)

struct mrg_map {
  int tileset_id;
  int8_t *tiles;
  int8_t *flags;
  int8_t *light;
  size_t w;
  size_t h;
  int tile_w;
  int tile_h;
};

struct mrg_map mrg_map_init(void);

int mrg_map_update(struct mrg_state *state, struct mrg_map *map);
int mrg_map_draw(struct mrg_state *state, struct mrg_map *map);

void mrg_map_to_tile(struct mrg_map *map, int xi, int yi, int *xo, int *yo);

void mrg_map_free(struct mrg_map *map);

#endif
