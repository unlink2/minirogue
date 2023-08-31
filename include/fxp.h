#ifndef FXP_H_
#define FXP_H_

#include <stdint.h>


// 24.8 fixed point number 
typedef int32_t fxp;

#define FPX_WHOLE(n) ((n) >> 8) 
#define FXP_FRACT(n) ((n) & 0xFF)

int mrg_isqrt(int n);

#endif 
