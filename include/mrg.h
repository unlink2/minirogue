#ifndef mrgH__
#define mrgH__

#include "camera.h"
#include "console.h"
#include "entity.h"
#include "input.h"
#include "map.h"
#include "tiles.h"
#include "room.h"
#include "arena.h"

struct mrg_platform;

struct mrg_config {
  _Bool verbose;
};

enum mrg_mode {
  MRG_MODE_GAME,
  MRG_MODE_CONSOLE,
  MRG_MODE_MAPED,
};

typedef int (*mrg_mode_tick)(struct mrg_state *state);

int mrg_mode_game(struct mrg_state *state);
int mrg_mode_maped(struct mrg_state *state);

struct mrg_state {
  int good;
  enum mrg_mode mode;

  mrg_mode_tick mode_update;

  struct mrg_camera main_camera;
  struct mrg_input main_input;

  struct mrg_tile_set_tbl tile_tbl;
  struct mrg_entity_tbl entity_tbl;
  struct mrg_room_tbl room_tbl;

  struct mrg_map map;

  struct mrg_platform *platform;
  const struct mrg_config *cfg;

  int tile_w;
  int tile_h;

  struct mrg_arena room_arena;
  struct mrg_console console;
};

int mrg_main(struct mrg_config *cfg);

int mrg_transition(struct mrg_state *state, enum mrg_mode mode);

#endif
