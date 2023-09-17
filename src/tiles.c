#include "tiles.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "platform.h"

struct mrg_tile_set_tbl mrg_tile_set_tbl_init(void) {
  struct mrg_tile_set_tbl tbl;
  memset(&tbl, 0, sizeof(tbl));

  tbl.len = MRG_TILE_SET_TBL_LEN;

  return tbl;
}

int mrg_tile_set_load(struct mrg_tile_set_tbl *tbl,
                      struct mrg_platform *platform, const char *path,
                      int tile_w, int tile_h) {

  // attempt finding an exisiting set
  for (size_t i = 0; i < tbl->len; i++) {
    struct mrg_tile_set *set = &tbl->sets[i];
    if (set->flags & MRG_TILE_SET_FLAG_FREE &&
        strncmp(set->path, path, strlen(set->path)) == 0) {
      return (int)i;
    }
  }

  // if there is none, return a new one
  for (size_t i = 0; i < tbl->len; i++) {
    struct mrg_tile_set *set = &tbl->sets[i];
    if (!(set->flags & MRG_TILE_SET_FLAG_FREE) &&
        mrg_pl_tile_set_load(set, platform, path) != -1) {
      set->tile_h = tile_h;
      set->tile_w = tile_w;
      set->flags |= MRG_TILE_SET_FLAG_FREE;
      set->path = strdup(path);
      return (int)i;
    }
  }

  fprintf(stderr, "Unable to load tile set '%s'\n", path);
  return -1;
}

void mrg_tile_set_free(struct mrg_tile_set_tbl *tbl,
                       struct mrg_platform *platform, int handle) {
  struct mrg_tile_set *set = &tbl->sets[handle];

  if (!set->data) {
    return;
  }

  if (!(set->flags & MRG_TILE_SET_FLAG_FREE)) {
    fprintf(stderr, "Tile set %d (%s): double free detected!\n", handle,
            set->path);
    return;
  }

  mrg_pl_tile_set_free(set, platform, handle);
  free((void *)set->path);
  set->data = NULL;

  set->flags = 0;
}

int mrg_tile_img_x(int tile, int img_w, int tile_w) {
  return (tile % (img_w / tile_w)) * tile_w;
}

int mrg_tile_img_y(int tile, int img_w, int tile_w, int tile_h) {
  return (tile / (img_w / tile_w)) * tile_h;
}

void mrg_tile_draw(struct mrg_tile_set_tbl *tbl, struct mrg_platform *platform,
                   int handle, int tile, int x, int y) {
  struct mrg_tile_set *set = &tbl->sets[handle];
  if (!(set->flags & MRG_TILE_SET_FLAG_FREE)) {
    fprintf(stderr, "Attempted to use unallocated tile set handle %d!\n",
            handle);
    return;
  }

  mrg_pl_tile_draw(set, platform, tile, x, y);
}
