#include "entity.h"
#include "fxp.h"
#include "input.h"
#include "mrg.h"
#include "platform.h"
#include "idc.h"
#include <stdio.h>
#include <string.h>

const mrg_entity_tick mrg_behavior_tbl[] = {mrg_beh_nop, mrg_beh_player_update,
                                            mrg_beh_entity_draw};

int mrg_beh_nop(struct mrg_state *state, struct mrg_entity *entity) {
  return 0;
}

int mrg_beh_player_update(struct mrg_state *state, struct mrg_entity *entity) {

  mrg_fixed px = entity->x;
  mrg_fixed py = entity->y;

  if (MRG_HELD(&state->main_input, MRG_ACTION_UP)) {
    entity->y -= MRG_FIXED(1, 0);
  }

  if (MRG_HELD(&state->main_input, MRG_ACTION_DOWN)) {
    entity->y += MRG_FIXED(1, 0);
  }

  if (MRG_HELD(&state->main_input, MRG_ACTION_LEFT)) {
    entity->x -= MRG_FIXED(1, 0);
  }

  if (MRG_HELD(&state->main_input, MRG_ACTION_RIGHT)) {
    entity->x += MRG_FIXED(1, 0);
  }

  // check tile collision
  enum mrg_map_flags tile_flags = mrg_map_collision(
      &state->map, entity->col_offset_x + MRG_FIXED_WHOLE(entity->x),
      entity->col_offset_y + MRG_FIXED_WHOLE(entity->y), entity->col_w,
      entity->col_h);

  return 0;
}

int mrg_beh_entity_draw(struct mrg_state *state, struct mrg_entity *entity) {
  mrg_tile_draw(&state->tile_tbl, state->platform, entity->tileset_id,
                entity->tile_id, MRG_FIXED_WHOLE(entity->x),
                MRG_FIXED_WHOLE(entity->y));
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
  int32_t flags = entity->flags;
  memset(entity, 0, sizeof(struct mrg_entity));
  entity->flags = flags;

  entity->col_w = 12;
  entity->col_h = 12;
  entity->col_offset_x = 2;
  entity->col_offset_y = 2;

  return 0;
}

int mrg_entities_from_idc(struct mrg_state *state, struct mrg_idc_file *f) {
  struct mrg_entity_tbl *tbl = &state->entity_tbl;

  for (size_t i = 0; i < f->header.n_entries; i++) {
    struct mrg_idc_dir *dir = &f->dirs[i];
    if (!(dir->type & MRG_IDC_DIR_ENTITY) &&
        dir->entry->entity.room_id == state->map.room_id) {
      continue;
    }

    int handle = mrg_entity_alloc(tbl);
    if (handle == -1) {
      return -1;
    }

    // TODO: handle room id
    struct mrg_entity *e = &tbl->slots[handle];
    switch (dir->entry->entity.type) {
    case MRG_ENTITY_PLAYER:
      mrg_entity_init_player(e);
      break;
    }
    e->type = dir->entry->entity.type;
    e->flags = dir->entry->entity.flags | e->flags;
    e->x = dir->entry->entity.x;
    e->y = dir->entry->entity.y;

    char tile_path[MRG_IDC_FILE_NAME_LEN + 1];
    memset(tile_path, 0, MRG_IDC_FILE_NAME_LEN + 1);
    memcpy(tile_path, dir->entry->room.tile_set, MRG_IDC_FILE_NAME_LEN);
    e->tileset_id = mrg_tile_set_load(&state->tile_tbl, state->platform,
                                      tile_path, state->tile_w, state->tile_h);
  }

  return 0;
}

int mrg_entity_init_player(struct mrg_entity *entity) {
  mrg_entity_init(entity);

  entity->type = MRG_ENTITY_PLAYER;
  entity->next_behavior = MRG_BEH_PLAYER_UPDATE;
  entity->next_draw = MRG_BEH_ENTITY_DRAW;

  entity->tile_id = 1;

  fprintf(stdout, "Player created with behavior %d\n", entity->next_behavior);
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

int mrg_entity_tbl_draw(struct mrg_state *state, struct mrg_entity_tbl *tbl) {
  for (size_t i = 0; i < tbl->slots_len; i++) {
    struct mrg_entity *entity = &tbl->slots[i];

    if ((entity->flags & MRG_ENTITY_FLAG_ALLOCED)) {
      if (mrg_entity_draw(state, tbl, entity) == -1) {
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

int mrg_entity_draw(struct mrg_state *state, struct mrg_entity_tbl *tbl,
                    struct mrg_entity *entity) {
  int res = tbl->behavior_tbl[entity->next_draw](state, entity);
#ifdef MRG_DEBUG
  mrg_pl_draw_debug_rec(
      state->platform, entity->col_offset_x + MRG_FIXED_WHOLE(entity->x),
      entity->col_offset_x + MRG_FIXED_WHOLE(entity->y), entity->col_w,
      entity->col_h, (struct mrg_color){255, 0, 0, 255});
#endif

  return res;
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
