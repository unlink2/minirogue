#include "room.h"
#include "idc.h"
#include <assert.h>
#include <stdio.h>

struct mrg_room_tbl mrg_room_tbl_from_idc(struct mrg_state *state,
                                          struct mrg_idc_file *f) {
  struct mrg_room_tbl tbl;
  tbl.len = 0;
  tbl.ok = 0;

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

  return tbl;
}
