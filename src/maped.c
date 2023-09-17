#include "maped.h"
#include "entity.h"
#include "mrg.h"
#include "room.h"
#include <stdio.h>
#include <string.h>

int mrg_maped_init(struct mrg_state *state) {
  fprintf(stderr, "Launching maped...\n");
  state->mode = MRG_MODE_MAPED;

  mrg_entity_tbl_clear(&state->entity_tbl);

  int handle = mrg_entity_alloc(&state->entity_tbl);
  if (handle == -1) {
    return -1;
  }

  // set up cursor
  struct mrg_entity *e = &state->entity_tbl.slots[handle];
  mrg_entity_init_type(e, MRG_ENTITY_CURSOR);

  e->tileset_id = mrg_tile_set_load(&state->tile_tbl, state->platform,
                                    "dbg.png", state->tile_w, state->tile_h);

  // load map directly from table instead of creating a dedicated cloned
  // instance
  struct mrg_arena *a = &state->room_arena;
  mrg_arena_clear(a);

  struct mrg_room_tbl *tbl = &state->room_tbl;
  if (tbl->len > 0) {
    tbl->graph.rooms[0] = mrg_arena_malloc(a, sizeof(struct mrg_room_instance));
    tbl->graph.g_w = 1;
    tbl->graph.g_h = 1;

    *tbl->graph.rooms[0] =
        mrg_room_instance_ed(state, state->room_tbl.rooms[0]);

    mrg_map_free(&state->map);
    state->map = mrg_map_init(state, 0);
  }

  return 0;
}
