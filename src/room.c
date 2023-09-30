#include "room.h"
#include "arena.h"
#include "idc.h"
#include "tiles.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mrg.h"

struct mrg_room_tbl mrg_room_tbl_from_idc(struct mrg_state *state,
                                          struct mrg_arena *a,
                                          struct mrg_idc_file *f) {
  mrg_arena_clear(a);
  struct mrg_room_tbl tbl;
  memset(&tbl, 0, sizeof(struct mrg_room_tbl));

  size_t room_handle = 0;
  for (size_t i = 0; i < f->header.n_entries; i++) {
    struct mrg_idc_dir *dir = &f->dirs[i];

    if (dir->type != MRG_IDC_DIR_ROOM) {
      continue;
    }

    tbl.len++;
    if (tbl.len > MRG_ROOMS_MAX) {
      fprintf(stderr,
              "idc contained more than the max number of rooms allowed!\n");
      tbl.ok = -1;
      return tbl;
    }

    tbl.rooms[room_handle] = mrg_arena_malloc(a, sizeof(struct mrg_room));
    *tbl.rooms[room_handle] = mrg_room_instance_from(state, &dir->entry.room);
    printf("room: %d\n", tbl.rooms[room_handle]->iflags);

    char tile_path[MRG_IDC_FILE_NAME_LEN + 1];
    memset(tile_path, 0, MRG_IDC_FILE_NAME_LEN + 1);
    memcpy(tile_path, dir->entry.room.tile_set, MRG_IDC_FILE_NAME_LEN);
    mrg_tile_set_load(&state->tile_tbl, state->platform, tile_path,
                      state->tile_w, state->tile_h);

    room_handle++;
  }

  assert(tbl.len <= MRG_ROOMS_MAX);

  if (tbl.len > 0) {
    tbl.g_w = 1;
    tbl.g_h = 1;
  }

  return tbl;
}

struct mrg_room mrg_room_instance_from(struct mrg_state *state,
                                     struct mrg_room *room) {
  struct mrg_room i;
  memset(&i, 0, sizeof(i));

  memcpy(&i, room, sizeof(struct mrg_room));
  i.ok = 0;
  i.iflags = MRG_ROOM_ALLOC;

  char tile_path[MRG_IDC_FILE_NAME_LEN];
  strncpy(tile_path, room->tile_set, sizeof(tile_path) - 1);
  i.tileset_id = mrg_tile_set_load(&state->tile_tbl, state->platform, tile_path,
                                   MRG_TILE_W, MRG_TILE_H);

  return i;
}
