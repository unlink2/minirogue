#ifndef MAPED_H_
#define MAPED_H_

#include "defs.h"

int mrg_maped_init(struct mrg_state *state);

int mrg_maped_load_room(struct mrg_state *state, int handle);

void mrg_maped_tile_picker_draw(struct mrg_state *state);

#endif
