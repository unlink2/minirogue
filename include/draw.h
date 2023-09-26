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

#define MRG_COLOR0 MRG_BLACK
#define MRG_COLOR1                                                             \
  (struct mrg_color) { 0xFE, 0xF6, 0xFE, 0xFF }
#define MRG_COLOR2                                                             \
  (struct mrg_color) { 0x49, 0x6A, 0xB4, 0xFF }
#define MRG_COLOR3                                                             \
  (struct mrg_color) { 0xC9, 0x00, 0x00, 0xFF }
#define MRG_COLOR4                                                             \
  (struct mrg_color) { 0xA7, 0x62, 0x28, 0xFF }
#define MRG_COLOR5                                                             \
  (struct mrg_color) { 0x5D, 0xA9, 0x3F, 0xFF }
#define MRG_COLOR6                                                             \
  (struct mrg_color) { 0x9F, 0x9D, 0x88, 0xFF }
#define MRG_COLOR7                                                             \
  (struct mrg_color) { 0xF5, 0xBE, 0x4B, 0xFF }

#endif
