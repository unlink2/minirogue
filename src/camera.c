#include "camera.h"
#include "platform.h"


struct mrg_camera mrg_camera_init(struct mrg_state *state) {
  return mrg_pl_camera_init(state->platform);
}
