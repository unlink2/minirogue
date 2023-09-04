#ifndef MAP_H_
#define MAP_H_

#include <stdbool.h>
#include <stdint.h>
#define MRG_LAYER_W 64
#define MRG_LAYER_H 64
#define MRG_LAYER_LEN (MRG_LAYER_W * MRG_LAYER_H)

struct mrg_layer {
  int8_t tile[MRG_LAYER_LEN];
  int8_t flags[MRG_LAYER_LEN];
};

struct mrg_map {
  struct mrg_layer bg;
  struct mrg_layer fg;
};

#endif
