#ifndef ENTITY_H_
#define ENTITY_H_

#include "fxp.h"
#include "defs.h"
#include <stddef.h>

enum mrg_entities { MRG_ENTITY_PLAYER, MRG_ENTITY_BAT };

enum mrg_entity_flags { MRG_ENTITY_FLAG_ALLOCED = 1 };

enum mrg_entity_stats {
  MRG_STAT_LEVEL,
  MRG_STAT_HP,
  MRG_STAT_HP_MAX,
  MRG_STATS_LEN
};

enum mrg_entity_behavior { MRG_BEH_NOP, MRG_BEH_PLAYER_INPUT };

typedef void (*mrg_entity_tick)(struct mrg_state *state,
                                struct mrg_entity *entity);

struct mrg_entity {
  enum mrg_entities type;
  enum mrg_entity_behavior next_behavior;

  // position
  mrg_fixed x;
  mrg_fixed y;

  int tileset_id;
  int tile_id;

  // entity stats
  int stats[MRG_STATS_LEN];

  int32_t flags;
  // entity specific flags
  int16_t uflags;
};

void mrg_beh_nop(struct mrg_state *state, struct mrg_entity *entity);

#define MRG_ENTITY_SLOTS_MAX 128

struct mrg_entity_tbl {
  struct mrg_entity slots[MRG_ENTITY_SLOTS_MAX];
  size_t slots_len;
};

struct mrg_entity_tbl mrg_entity_tbl_init(void);

// alloc a new entity
// return a handle to the allocated entity
// or -1 on error
int mrg_entity_alloc(struct mrg_entity_tbl *tbl);

void mrg_entity_free(struct mrg_entity_tbl *tbl, int handle);

void mrg_entity_tbl_free(struct mrg_entity_tbl *tbl);

#endif
