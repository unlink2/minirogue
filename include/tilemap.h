#ifndef TILEMAP_H_
#define TILEMAP_H_

#include <stdint.h>

#define MRG_TILE_MAP_W (64)
#define MRG_TILE_MAP_H (32)
#define MRG_TILE_MAP_SIZE ((MRG_TILE_MAP_W) * (MRG_TILE_MAP_H))

extern uint8_t mrg_tiles[MRG_TILE_MAP_SIZE];
extern uint8_t mrg_attrs[MRG_TILE_MAP_SIZE];

#endif
