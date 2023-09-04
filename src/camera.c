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
