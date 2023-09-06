#include "map.h"
#include <string.h>

struct mrg_map mrg_map_init(void) {
  struct mrg_map map;
  memset(&map, 0, sizeof(map));

  return map;
}

void mrg_map_free(struct mrg_map *map) {}
