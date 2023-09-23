#include "maped.h"
#include "entity.h"
#include "mrg.h"
#include "room.h"
#include <stdio.h>
#include <string.h>

struct mrg_maped mrg_maped_init(struct mrg_state *state) {
  struct mrg_maped maped;
  memset(&maped, 0, sizeof(maped));
  fprintf(stderr, "Launching maped...\n");
  state->mode = MRG_MODE_MAPED;

  state->room_tbl =
      mrg_room_tbl_from_idc(state, &state->room_arena, &state->idc);
  mrg_entity_tbl_clear(&state->entity_tbl);

  int handle = mrg_entity_alloc(&state->entity_tbl);
  if (handle == -1) {
    maped.ok = -1;
    return maped;
  }

  // set up cursor
  struct mrg_entity *e = &state->entity_tbl.slots[handle];
  mrg_entity_init_type(e, MRG_ENTITY_CURSOR);

  e->tileset_id = mrg_tile_set_load(&state->tile_tbl, state->platform,
                                    "dbg.png", state->tile_w, state->tile_h);

  mrg_maped_load_room(state, 0);
  return maped;
}

int mrg_maped_load_room(struct mrg_state *state, int handle) {
  struct mrg_room_tbl *tbl = &state->room_tbl;
  for (size_t i = 0; i < tbl->len; i++) {
    if (tbl->rooms[i]->iflags & MRG_ROOM_ALLOC) {
      mrg_map_free(&state->map);
      state->map = mrg_map_init(state, 0);
    }
    return 0;
  }

  fprintf(stderr, "Unabel to load room %d\n", handle);
  return -1;
}

void mrg_maped_tile_picker_draw(struct mrg_state *state) {}
