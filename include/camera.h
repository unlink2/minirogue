#ifndef CAMERA_H_
#define CAMERA_H_

#include "defs.h"

struct mrg_camera {
  int handle;
  int target_entity;
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
