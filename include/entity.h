#ifndef ENTITY_H_
#define ENTITY_H_

#include "fxp.h"
#include "defs.h"
#include "room.h"
#include <stdbool.h>
#include <stddef.h>

struct mrg_entity_tbl {
  // currently used entities
  struct mrg_entity slots[MRG_ENTITY_SLOTS_MAX];
  size_t slots_len;

  // all entity callbacks are in this table
  const mrg_entity_tick *behavior_tbl;
};

struct mrg_entity_tbl mrg_entity_tbl_init(void);

// spawn the entity to the current map
// from an entity template
int mrg_entity_spawn(struct mrg_state *state, struct mrg_entity *entity);

int mrg_beh_nop(struct mrg_state *state, struct mrg_entity *entity);
int mrg_beh_player_update(struct mrg_state *state, struct mrg_entity *entity);
int mrg_beh_entity_draw(struct mrg_state *state, struct mrg_entity *entity);
int mrg_entity_draw_alt(struct mrg_state *state, struct mrg_entity *entity);

// alloc a new entity
// return a handle to the allocated entity
// or -1 on error
int mrg_entity_alloc(struct mrg_entity_tbl *tbl);

int mrg_entity_init_type(struct mrg_entity *entity, enum mrg_entities type);
int mrg_entity_init(struct mrg_entity *entity);

int mrg_entities_from_idc(struct mrg_state *state, struct mrg_idc_file *f);

// init functions for entities
int mrg_entity_init_player(struct mrg_entity *entity);
int mrg_entity_init_cursor(struct mrg_entity *entity);

int mrg_entity_tbl_update(struct mrg_state *state, struct mrg_entity_tbl *tbl);
int mrg_entity_update(struct mrg_state *state, struct mrg_entity_tbl *tbl,
                      struct mrg_entity *entity);

int mrg_entity_tbl_draw(struct mrg_state *state, struct mrg_entity_tbl *tbl);
int mrg_entity_draw(struct mrg_state *state, struct mrg_entity_tbl *tbl,
                    struct mrg_entity *entity);
void mrg_entity_free(struct mrg_entity_tbl *tbl, int handle);

void mrg_entity_tbl_clear(struct mrg_entity_tbl *tbl);
void mrg_entity_tbl_free(struct mrg_entity_tbl *tbl);

#endif
