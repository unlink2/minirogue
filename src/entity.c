#include "entity.h"
#include <stdio.h>
#include <string.h>

void mrg_beh_nop(struct mrg_state *state, struct mrg_entity *entity) {}

struct mrg_entity_tbl mrg_entity_tbl_init(void) {
  struct mrg_entity_tbl tbl;
  memset(&tbl, 0, sizeof(tbl));

  tbl.slots_len = MRG_ENTITY_SLOTS_MAX;

  return tbl;
}

int mrg_entity_alloc(struct mrg_entity_tbl *tbl) { return -1; }

void mrg_entity_free(struct mrg_entity_tbl *tbl, int handle) {
  struct mrg_entity *entity = &tbl->slots[handle];
  if ((entity->flags & MRG_ENTITY_FLAG_ALLOCED) == 1) {
    fprintf(stderr, "Double free detected for entity slot %d\n", handle);
    return;
  }
  entity->flags = 0;
}

void mrg_entity_tbl_free(void) {}
