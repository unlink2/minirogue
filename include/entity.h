#ifndef ENTITY_H_
#define ENTITY_H_

#include "fxp.h"
#include <stddef.h>

enum mrg_entities { MRG_ENTITY_PLAYER, MRG_ENTITY_BAT };

enum mrg_entity_flags { MRG_ENTITY_FLAG_FREE = 1 };

struct mrg_entity {
  enum mrg_entities type;

  // position
  mrg_fixed x;
  mrg_fixed y;

  int tile_id;

  // entity stats
  int lvl;

  int32_t flags;
};

#define MRG_ENTITY_SLOTS_MAX 128

struct mrg_entity_slots {
  struct mrg_entity slots[MRG_ENTITY_SLOTS_MAX];
  size_t nslots;
};

#endif
