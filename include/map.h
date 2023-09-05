#ifndef MAP_H_
#define MAP_H_

#include <stdbool.h>
#include <stdint.h>
#define MRG_LAYER_W 64
#define MRG_LAYER_H 64
#define MRG_LAYER_LEN (MRG_LAYER_W * MRG_LAYER_H)

struct mrg_map {
  int8_t tiles[MRG_LAYER_LEN];
  int8_t flags[MRG_LAYER_LEN];
};

#endif
