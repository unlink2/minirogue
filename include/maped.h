#ifndef MAPED_H_
#define MAPED_H_

#include "defs.h"
#include <stdbool.h>

struct mrg_maped {
  int ok;
  bool tile_picker;
};

struct mrg_maped mrg_maped_init(struct mrg_state *state);

int mrg_maped_load_room(struct mrg_state *state, int handle);

void mrg_maped_tile_picker_draw(struct mrg_state *state);

#endif
