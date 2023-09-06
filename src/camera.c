#include "camera.h"
#include "entity.h"
#include "platform.h"
#include <stdio.h>

struct mrg_camera mrg_camera_init(struct mrg_state *state) {
  return mrg_pl_camera_init(state->platform);
}

int mrg_camera_update(struct mrg_state *state, struct mrg_camera *camera) {
  struct mrg_entity *entity = &state->entity_tbl.slots[camera->target_entity];
  if (!(entity->flags & MRG_ENTITY_FLAG_ALLOCED)) {
    fprintf(stderr, "Camera target entity is not allocated!\n");
    return -1;
  }

  mrg_pl_camera_target(state->platform, camera, MRG_FIXED_WHOLE(entity->x),
                       MRG_FIXED_WHOLE(entity->y));

  return 0;
}

void mrg_camera_world_to_screen(struct mrg_state *state,
                                struct mrg_camera *camera, int ix, int iy,
                                int *ox, int *oy) {
  mrg_pl_camera_world_to_screen(state->platform, camera, ix, iy, ox, oy);
}

void mrg_camera_screen_to_world(struct mrg_state *state,
                                struct mrg_camera *camera, int ix, int iy,
                                int *ox, int *oy) {
  mrg_pl_camera_screen_to_world(state->platform, camera, ix, iy, ox, oy);
}

void mrg_camera_bounds(struct mrg_state *state, struct mrg_camera *camera,
                       int *x, int *y, int *w, int *h) {
  mrg_pl_camera_bounds(state->platform, camera, x, y, w, h); 
}
