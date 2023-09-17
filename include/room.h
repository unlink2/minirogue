#ifndef ROOM_H_
#define ROOM_H_

#include <stddef.h>
#include <stdint.h>
#include "defs.h"
#include "idc.h"

#define MRG_ROOMS_MAX 64

/**
 * A room is an abstract representation of
 * a walkable area. It connects to other rooms in a graph
 * and it can be drawn onto the map
 */

// collection of all room templates
struct mrg_room_tbl {
  int ok;
  struct mrg_room *rooms[MRG_ROOMS_MAX];
  size_t len;

  // dimension of the room layout
  size_t g_w;
  size_t g_h;

};

struct mrg_room_tbl mrg_room_tbl_from_idc(struct mrg_state *state,
                                          struct mrg_arena *a,
                                          struct mrg_idc_file *f);
// crates a room without cloning. use for map editor!
struct mrg_room mrg_room_instance_from(struct mrg_state *state,
                                     struct mrg_room *room);

#endif
