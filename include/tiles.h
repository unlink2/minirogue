#ifndef TILES_H_
#define TILES_H_

#include <stdint.h>

#define MRG_TILE_TABLE_LEN 256
#define MRG_TILE_STATIC_LEN 128

enum mrg_tilesets {
  MRG_TILESET_STATIC,
  MRG_TILESET_DEBUG
};

struct mrg_tileset {
  void *static_tiles;
  void *dynamic_tiles;
};

// load static tiles
struct mrg_tileset mrg_tile_setinit(void);

int mrg_tile_set_load(enum mrg_tilesets set); 

void mrg_tile_setfree(struct mrg_tileset *ts);

#endif
