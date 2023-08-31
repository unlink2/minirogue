#ifndef FXP_H_
#define FXP_H_

#include <stdint.h>

/**
 * We use fixed point because 
 * the ultimate goal is to port 
 * the game to hardware that may not have 
 * decent floating point support
 */

// 24.8 fixed point number 
typedef int32_t mrg_fixed;

#define MRG_FIXED_WHOLE(n) ((n) >> 8) 
#define MRG_FIXED_FRACT(n) ((n) & 0xFF)

int mrg_isqrt(int n);

#endif 
