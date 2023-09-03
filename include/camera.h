#ifndef CAMERA_H_
#define CAMERA_H_

#include "defs.h"

struct mrg_camera {
  int handle;
  int target_entity;
};

struct mrg_camera mrg_camera_init(struct mrg_state *state);

#endif 
