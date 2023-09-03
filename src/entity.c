#include "entity.h"
#include "input.h"
#include "mrg.h"
#include <stdio.h>
#include <string.h>

const mrg_entity_tick mrg_behavior_tbl[] = {mrg_beh_nop};

int mrg_beh_nop(struct mrg_state *state, struct mrg_entity *entity) {
  return 0;
}

int mrg_beh_player_update(struct mrg_state *state, struct mrg_entity *entity) {

  if (MRG_PRESSED(&state->main_input, MRG_ACTION_UP)) {
    printf("pressed!\n");
  }

  if (MRG_HELD(&state->main_input, MRG_ACTION_UP)) {
    printf("held!\n");
  }

  return 0;
}

struct mrg_entity_tbl mrg_entity_tbl_init(void) {
  struct mrg_entity_tbl tbl;
  memset(&tbl, 0, sizeof(tbl));

  tbl.slots_len = MRG_ENTITY_SLOTS_MAX;
  tbl.behavior_tbl = mrg_behavior_tbl;

  return tbl;
}

int mrg_entity_alloc(struct mrg_entity_tbl *tbl) {
  for (size_t i = 0; i < tbl->slots_len; i++) {
    struct mrg_entity *entity = &tbl->slots[i];
    if (!(entity->flags & MRG_ENTITY_FLAG_ALLOCED)) {
      entity->flags |= MRG_ENTITY_FLAG_ALLOCED;
      return (int)i;
    }
  }
  return -1;
}

int mrg_entity_init(struct mrg_entity *entity) {
  memset(entity, 0, sizeof(struct mrg_entity));
  return 0;
}

int mrg_entity_init_player(struct mrg_entity *entity) {
  mrg_entity_init(entity);

  entity->type = MRG_ENTITY_PLAYER;
  entity->next_behavior = MRG_BEH_PLAYER_UPDATE;

  return 0;
}

int mrg_entity_tbl_update(struct mrg_state *state, struct mrg_entity_tbl *tbl) {
  for (size_t i = 0; i < tbl->slots_len; i++) {
    struct mrg_entity *entity = &tbl->slots[i];

    if ((entity->flags & MRG_ENTITY_FLAG_ALLOCED)) {
      if (mrg_entity_update(state, tbl, entity) == -1) {
        return -1;
      }
    }
  }
  return 0;
}

int mrg_entity_update(struct mrg_state *state, struct mrg_entity_tbl *tbl,
                      struct mrg_entity *entity) {
  return tbl->behavior_tbl[entity->next_behavior](state, entity);
}

void mrg_entity_free(struct mrg_entity_tbl *tbl, int handle) {
  struct mrg_entity *entity = &tbl->slots[handle];
  if (!(entity->flags & MRG_ENTITY_FLAG_ALLOCED)) {
    fprintf(stderr, "Double free detected for entity slot %d\n", handle);
    return;
  }
  entity->flags = 0;
}

void mrg_entity_tbl_free(struct mrg_entity_tbl *tbl) {}
