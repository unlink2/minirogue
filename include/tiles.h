#ifndef TILES_H_
#define TILES_H_

#include <stddef.h>
#include <stdint.h>
#include "defs.h"

#define MRG_TILE_SET_TBL_LEN 4

enum mrg_tile_set_flags { MRG_TILE_SET_FLAG_FREE = 1 };

struct mrg_tile_set {
  const char *path;
  void *data;
  enum mrg_tile_set_flags flags;
  int tile_w;
  int tile_h;
};

struct mrg_tile_set_tbl {
  struct mrg_tile_set sets[MRG_TILE_SET_TBL_LEN];
  size_t len;
};

struct mrg_tile_set_tbl mrg_tile_set_tbl_init(void);

// loads a tile set and returns the apropriate handle or -1 on error
int mrg_tile_set_load(struct mrg_tile_set_tbl *tbl,
                      struct mrg_platform *platform, const char *path,
                      int tile_w, int tile_h);

int mrg_tile_img_x(int tile, int img_w, int tile_w);
int mrg_tile_img_y(int tile, int img_w, int tile_w, int tile_h);

// frees a tile set with handle n
void mrg_tile_set_free(struct mrg_tile_set_tbl *tbl,
                       struct mrg_platform *platform, int handle);

void mrg_tile_draw(struct mrg_tile_set_tbl *tbl, struct mrg_platform *platform,
                   int handle, int tile, int x, int y);

void mrg_tile_draw_adv(struct mrg_tile_set_tbl *tbl, struct mrg_platform *platform,
                   int handle, int tile, int x, int y, int hflip, int vflip);

#endif
