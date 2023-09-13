#ifndef ROOM_H_
#define ROOM_H_

#include <stddef.h>
#include <stdint.h>
#include "defs.h"

#define MRG_ROOMS_MAX 64

/**
 * A room is an abstract representation of
 * a walkable area. It connects to other rooms in a graph
 * and it can be drawn onto the map
 */
#define mrg_room mrg_idc_room

struct mrg_room_instacne {
  // room handles for all directions
  int room_west;
  int room_east;
  int room_north;
  int room_south;

  int room_instance;
};

struct mrg_room_graph {
  struct mrg_room *rooms;
  size_t room_len;
};

// collection of all room templates
struct mrg_room_tbl {
  int ok;
  // TODO: how do we store rooms?
  struct mrg_room *rooms[MRG_ROOMS_MAX];
  size_t len;
};

struct mrg_room_tbl mrg_room_tbl_from_idc(struct mrg_state *state,
                                          struct mrg_idc_file *f);

#endif
