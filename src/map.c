#include "map.h"
#include "tiles.h"
#include "mrg.h"
#include <string.h>

struct mrg_map mrg_map_init(void) {
  struct mrg_map map;
  memset(&map, 0, sizeof(map));

  map.w = MRG_LAYER_W;
  map.h = MRG_LAYER_H;

  return map;
}

int mrg_map_update(struct mrg_state *state, struct mrg_map *map) { return 0; }

int mrg_map_draw(struct mrg_state *state, struct mrg_map *map) {
  // FIXME: implement culling, for now literally just draw everything
  size_t tile = 0;

  int tx = 0;
  int ty = 0;
  for (size_t y = 0; y < map->h; y++) {
    for (size_t x = 0; x < map->w; x++) {
      mrg_tile_draw(&state->tile_tbl, state->platform, map->tileset_id,
                    map->tiles[tile], tx, ty);
      tile++;
      tx += state->tile_tbl.sets[map->tileset_id].tile_w;
    }
    tx = 0;
    ty += state->tile_tbl.sets[map->tileset_id].tile_h;
  }

  return 0;
}

void mrg_map_free(struct mrg_map *map) {}
