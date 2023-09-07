#include "rand.h"

static int mrg_tl_seed;

void mrg_srand(int seed) {
  if (seed != 0) {
    mrg_tl_seed = seed;
  }
}

int mrg_randa(int *tl_seed) {
  int seed = ((*tl_seed) << 13) ^ (*tl_seed); // NOLINT
  *tl_seed += 1;
  return ((seed * (seed * seed * 15731 + 789221) + 1376312589) & // NOLINT
          0x7fffffff);
}

int mrg_rand(void) {
  return mrg_randa(&mrg_tl_seed);
}
