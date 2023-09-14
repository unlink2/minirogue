#include "room.h"
#include "arena.h"
#include "idc.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct mrg_room_tbl mrg_room_tbl_from_idc(struct mrg_state *state,
                                          struct mrg_arena *a,
                                          struct mrg_idc_file *f) {
  struct mrg_room_tbl tbl;
  tbl.len = 0;
  tbl.ok = 0;
  memset(&tbl.graph, 0, sizeof(struct mrg_room_graph));

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

    tbl.rooms[i] = &dir->entry->room;
  }

  assert(tbl.len <= MRG_ROOMS_MAX);

  if (tbl.len > 0) {
    tbl.graph.rooms[0] = mrg_arena_malloc(a, sizeof(struct mrg_room_instance));
    *tbl.graph.rooms[0] =
        mrg_room_instance_from(a, tbl.rooms[0], -1, -1, -1, -1);
    tbl.graph.room_len = 1;
  }

  return tbl;
}

struct mrg_room_instance mrg_room_instance_from(struct mrg_arena *a,
                                                struct mrg_room *room, int w,
                                                int e, int n, int s) {
  struct mrg_room_instance i = {w,e,n,s, NULL, NULL, room->room_w, room->room_h};

  size_t tlen = (size_t)room->room_w * room->room_h;
  i.tiles = mrg_arena_malloc(a, tlen);
  i.flags = mrg_arena_malloc(a, tlen);

  memcpy(i.tiles, room->tiles, tlen); 
  memcpy(i.flags, room->flags, tlen); 

  return i;
}
