#include "fxp.h"
#include <stdio.h>
#include <stdlib.h>

int mrg_isqrt(int n) {
  if (n == 0) {
    return 0;
  }

  int x = n;

  int root = 0;

  // guess i iterations... idk how many iterations are good enough tbh
  for (int i = 0; i < 15; i++) {
    root = (x + (n / x)) / 2;
    if (abs(root - x) < 0) {
      break;
    }
    x = root;
  }
  return root;
}
