#ifndef ENTITY_H_
#define ENTITY_H_

#include "fxp.h"

enum mrg_entities { MRG_ENTITY_PLAYER, MRG_ENTITY_BAT };

struct mrg_entity {
  enum mrg_entities type;

  // position 
  mrg_fixed x;
  mrg_fixed y;

  // entity stats
  int lvl;
};

#endif
