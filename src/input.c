#include "input.h"
#include "mrg.h"
#include "platform.h"
#include <string.h>

struct mrg_input mrg_input_init(int handle) {
  struct mrg_input input;
  memset(&input, 0, sizeof(input));
  input.handle = handle;

  return input;
}

void mrg_input_poll(struct mrg_state *state, struct mrg_input *input) {
  input->prev = input->state;
  input->state = mrg_pl_input_poll(state->platform, input->handle);
}
