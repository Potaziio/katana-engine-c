#ifndef MATH_H
#define MATH_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

#include "../../../include/cglm/include/cglm/mat4.h"


float32_t fisqrt(float32_t num);
void init_random(uint32_t seed);
int32_t rand_int(int32_t min, int32_t max);
float32_t fclamp(float32_t min, float32_t max, float32_t val);

#endif
