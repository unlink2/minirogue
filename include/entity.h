#ifndef ENTITY_H_
#define ENTITY_H_

enum mr_entities { MR_ENTITY_PLAYER, MR_ENTITY_BAT };

struct mr_entity {
  enum mr_entities type;

  // entity stats
  int lvl;
};

#endif
