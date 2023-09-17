#include "maped.h"
#include "entity.h"
#include "mrg.h"
#include <stdio.h>

int mrg_maped_init(struct mrg_state *state) {
  fprintf(stderr, "Launching maped...\n");
  state->mode = MRG_MODE_MAPED;

  mrg_entity_tbl_clear(&state->entity_tbl);

  int handle = mrg_entity_alloc(&state->entity_tbl);
  if (handle == -1) {
    return -1;
  }

  struct mrg_entity *e = &state->entity_tbl.slots[handle];
  mrg_entity_init_type(e, MRG_ENTITY_CURSOR);

  e->tileset_id = mrg_tile_set_load(&state->tile_tbl, state->platform,
                                    "dbg.png", state->tile_w, state->tile_h);

  return 0;
}
