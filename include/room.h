#ifndef ROOM_H_
#define ROOM_H_

#include <stddef.h>

/**
 * A room is an abstract representation of
 * a walkable area. It connects to other rooms in a graph
 * and it can be drawn onto the map
 */
struct mrg_room {
  // room handles for all directions
  int room_west;
  int room_east;
  int room_north;
  int room_south;
};

struct mrg_room_graph {
  struct mrg_room *rooms;
  size_t room_len;
};

// collection of all room templates
struct mrg_room_tbl {
  // TODO: how do we store rooms?
  size_t len;
};

#endif
