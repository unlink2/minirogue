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
  int8_t tiles[MRG_LAYER_LEN];
  int8_t flags[MRG_LAYER_LEN];
  size_t w;
  size_t h;
};

struct mrg_map mrg_map_init(void);

int mrg_map_update(struct mrg_state *state, struct mrg_map *map);
int mrg_map_draw(struct mrg_state *state, struct mrg_map *map);

void mrg_map_free(struct mrg_map *map);

#endif