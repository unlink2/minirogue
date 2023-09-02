#ifndef mrgH__
#define mrgH__

#include "camera.h"
#include "entity.h"
#include "input.h"
#include "tiles.h"


struct mrg_platform;

struct mrg_config {
  _Bool verbose;
};

struct mrg_state {
  struct mrg_camera main_camera;
  struct mrg_input main_input;

  struct mrg_tile_set_tbl tile_tbl;

  struct mrg_platform *platform;
  const struct mrg_config *cfg;

  // all entity callbacks are in this table 
  const mrg_entity_tick *behavior_tbl;
};

int mrg_main(struct mrg_config *cfg);

#endif
