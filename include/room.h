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
#define mrg_room mrg_idc_room

enum mrg_room_flags { MRG_ROOM_ALLOC };

struct mrg_room_instance {
  struct mrg_room instanced;

  enum mrg_room_flags iflags;
};

struct mrg_room_graph {
  struct mrg_room_instance *rooms[MRG_ROOMS_MAX];
  // dimension of the graph
  size_t g_w;
  size_t g_h;
};

// collection of all room templates
struct mrg_room_tbl {
  int ok;
  struct mrg_room *rooms[MRG_ROOMS_MAX];

  struct mrg_room_graph graph;

  size_t len;
};

struct mrg_room_tbl mrg_room_tbl_from_idc(struct mrg_state *state,
                                          struct mrg_arena *a,
                                          struct mrg_idc_file *f);

// create an instance of a room from a room template
struct mrg_room_instance mrg_room_instance_from(struct mrg_arena *a,
                                                struct mrg_room *room, int w,
                                                int e, int n, int s);

#endif
