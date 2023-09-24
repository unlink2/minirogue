#include "maped.h"
#include "defs.h"
#include "entity.h"
#include "mrg.h"
#include "platform.h"
#include "room.h"
#include <stdio.h>
#include <string.h>

struct mrg_maped mrg_maped_init(struct mrg_state *state) {
  struct mrg_maped maped;
  memset(&maped, 0, sizeof(maped));
  fprintf(stderr, "Launching maped...\n");
  state->mode = MRG_MODE_MAPED;

  maped.mode = MRG_MAPED_MODE_ED;

  state->room_tbl =
      mrg_room_tbl_from_idc(state, &state->room_arena, &state->idc);
  mrg_entity_tbl_clear(&state->entity_tbl);

  int handle = mrg_entity_alloc(&state->entity_tbl);
  if (handle == -1) {
    maped.ok = -1;
    return maped;
  }

  maped.cursor_handle = handle;

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

void mrg_maped_update(struct mrg_state *state, struct mrg_maped *maped) {}

void mrg_maped_draw(struct mrg_state *state, struct mrg_maped *maped) {}

int mrg_beh_cursor_update(struct mrg_state *state, struct mrg_entity *entity) {
  int step_x = state->map.tile_w;
  int step_y = state->map.tile_h;

  int16_t cursor_delay = 4;

  if (state->mode == MRG_MODE_MAPED && entity->uflags == 0) {
    if (MRG_HELD(&state->main_input, MRG_ACTION_UP)) {
      entity->y -= MRG_FIXED(step_y, 0);
      entity->uflags = cursor_delay;
    }

    if (MRG_HELD(&state->main_input, MRG_ACTION_DOWN)) {
      entity->y += MRG_FIXED(step_y, 0);
      entity->uflags = cursor_delay;
    }

    if (MRG_HELD(&state->main_input, MRG_ACTION_LEFT)) {
      entity->x -= MRG_FIXED(step_x, 0);
      entity->uflags = cursor_delay;
    }

    if (MRG_HELD(&state->main_input, MRG_ACTION_RIGHT)) {
      entity->x += MRG_FIXED(step_x, 0);
      entity->uflags = cursor_delay;
    }

    /**
     * USTAT1 contains tile index
     */
    if (MRG_HELD(&state->main_input, MRG_ACTION_SCRLUP)) {
      entity->stats[MRG_STAT_USTAT1]++;
      entity->uflags = cursor_delay;
    }
    if (MRG_HELD(&state->main_input, MRG_ACTION_SCRLDOWN)) {
      entity->stats[MRG_STAT_USTAT1] = MIN(0, entity->stats[MRG_STAT_USTAT1]);
      entity->uflags = cursor_delay;
    }
  }

  if (state->mode == MRG_MODE_MAPED) {
    if (MRG_HELD(&state->main_input, MRG_ACTION_A)) {
      mrg_map_tile_set(&state->map, MRG_FIXED_WHOLE(entity->x),
                       MRG_FIXED_WHOLE(entity->y),
                       (int8_t)entity->stats[MRG_STAT_USTAT1]);
    }

    /**
     * USTAT2 contains flag value
     */
    if (MRG_HELD(&state->main_input, MRG_ACTION_B)) {
      mrg_map_flag_set(&state->map, MRG_FIXED_WHOLE(entity->x),
                       MRG_FIXED_WHOLE(entity->y),
                       (int8_t)entity->stats[MRG_STAT_USTAT2]);
    }
  }

  if (entity->uflags > 0) {
    entity->uflags--;
  }

  return 0;
}

int mrg_beh_cursor_draw(struct mrg_state *state, struct mrg_entity *entity) {
  int offset = 2;
  mrg_pl_draw_filled_rec(
      state->platform, MRG_FIXED_WHOLE(entity->x) - offset,
      MRG_FIXED_WHOLE(entity->y) - offset, state->map.tile_w + offset * 2,
      state->map.tile_h + offset * 2, (struct mrg_color){255, 159, 0, 255});

  mrg_pl_draw_filled_rec(state->platform, MRG_FIXED_WHOLE(entity->x),
                         MRG_FIXED_WHOLE(entity->y), state->map.tile_w,
                         state->map.tile_h, (struct mrg_color){0, 0, 0, 255});
  mrg_tile_draw(&state->tile_tbl, state->platform, state->map.tileset_id,
                entity->stats[MRG_STAT_USTAT1], MRG_FIXED_WHOLE(entity->x),
                MRG_FIXED_WHOLE(entity->y));
  return 0;
}
