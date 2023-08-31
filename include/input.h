#ifndef INPUT_H_
#define INPUT_H_

#include <stdint.h>

struct mrg_state;

enum mrg_actions {
  MRG_ACTION_UP = 1,
  MRG_ACTION_DOWN = 2,
  MRG_ACTION_LEFT = 4,
  MRG_ACTION_RIGHT = 8
};

struct mrg_input {
  int handle;
  uint16_t state;
  uint16_t prev;
};

struct mrg_input mrg_input_init(int handle);

void mrg_input_poll(struct mrg_state *state, struct mrg_input *input);

#define MRG_PRESSED(input, action)                                             \
  ((input)->state & (action) && !((input)->prev & (action)))

#define MRG_HELD(input, action)                                                \
  ((input)->state & (action) && ((input)->prev & (action)))

#endif
