#ifndef CAMERA_H_
#define CAMERA_H_

#include "defs.h"

enum mrg_camera_target {
  MRG_CAMT_POINT,
  MRG_CAMT_ENTITY,
};

struct mrg_camera {
  int handle;

  enum mrg_camera_target target_type;
  int target_entity;
  int target_x;
  int target_y;
};

struct mrg_camera mrg_camera_init(struct mrg_state *state);

int mrg_camera_update(struct mrg_state *state, struct mrg_camera *camera);

void mrg_camera_world_to_screen(struct mrg_state *state,
                                struct mrg_camera *camera, int ix, int iy,
                                int *ox, int *oy);

void mrg_camera_screen_to_world(struct mrg_state *state,
                                struct mrg_camera *camera, int ix, int iy,
                                int *ox, int *oy);

void mrg_camera_bounds(struct mrg_state *state, struct mrg_camera *camera,
                       int *x, int *y, int *w, int *h);

#endif
