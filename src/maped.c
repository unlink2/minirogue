#include "maped.h"
#include "mrg.h"
#include <stdio.h>

int mrg_maped_init(struct mrg_state *state) {
  fprintf(stderr, "Launching maped...\n");
  state->mode = MRG_MODE_MAPED;
  return 0;
}