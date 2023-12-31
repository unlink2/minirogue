#ifndef mrgH__
#define mrgH__

#include "camera.h"
#include "console.h"
#include "entity.h"
#include "hud.h"
#include "input.h"
#include "map.h"
#include "maped.h"
#include "tiles.h"
#include "room.h"
#include "arena.h"

struct mrg_platform;

struct mrg_config {
  _Bool verbose;
  const char **exec;
  size_t exec_len;
};

int mrg_mode_game(struct mrg_state *state);
int mrg_mode_maped(struct mrg_state *state);

struct mrg_state {
  int good;
  enum mrg_mode mode;
  int frame;

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
  struct mrg_arena idc_arena;
  struct mrg_arena tmp_arena;

  struct mrg_console console;
  struct mrg_hud hud;

  struct mrg_idc_file idc;

  int screen_w;
  int screen_h;

  bool alt_anim;

  // mode specific data
  union {
    struct mrg_maped maped;
  };
};

int mrg_main(struct mrg_config *cfg);

void mrg_toggle_dbg(struct mrg_state *state);

void mrg_state_invalidate(struct mrg_state *state);

int mrg_transition(struct mrg_state *state, enum mrg_mode mode);

#endif
