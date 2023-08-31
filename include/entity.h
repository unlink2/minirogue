#ifndef ENTITY_H_
#define ENTITY_H_

enum mrg_entities { MRG_ENTITY_PLAYER, MRG_ENTITY_BAT };

struct mrg_entity {
  enum mrg_entities type;

  // entity stats
  int lvl;
};

#endif
