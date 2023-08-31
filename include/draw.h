#ifndef DRAW_H_
#define DRAW_H_

#include <stdint.h>

struct mrg_color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

#define MRG_BLACK                                                              \
  (struct mrg_color) { 0, 0, 0, 0xFF }
#define MRG_WHITE                                                              \
  (struct mrg_color) { 0xFF, 0xFF, 0xFF, 0xFF }

#endif
