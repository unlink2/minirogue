#ifndef MAPED_H_
#define MAPED_H_

#include "defs.h"
#include <stdbool.h>

enum mrg_maped_mode {
  MRG_MAPED_MODE_ED,
  MRG_MAPED_MODE_TILESEL,
  MRG_MAPED_MODE_PROPSEL,
};

struct mrg_maped {
  int ok;
  bool tile_picker;

  enum mrg_maped_mode mode;
};

struct mrg_maped mrg_maped_init(struct mrg_state *state);

int mrg_maped_load_room(struct mrg_state *state, int handle);

void mrg_maped_update(struct mrg_state *state, struct mrg_maped *maped);

void mrg_maped_draw(struct mrg_state *state, struct mrg_maped *maped);

int mrg_beh_cursor_update(struct mrg_state *state, struct mrg_entity *entity);
int mrg_beh_cursor_draw(struct mrg_state *state, struct mrg_entity *entity);

#endif
