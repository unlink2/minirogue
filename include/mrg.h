#ifndef mrgH__
#define mrgH__

#include "camera.h"

struct mrg_platform;

struct mrg_config {
  _Bool verbose;
};

struct mrg_state {
  struct mrg_camera main_camera;

  struct mrg_platform *platform;
  const struct mrg_config *cfg;
};

int mrg_main(struct mrg_config *cfg);

#endif
