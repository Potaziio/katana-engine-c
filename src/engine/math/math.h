#ifndef MATH_H
#define MATH_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

#include "../../../include/cglm/include/cglm/mat4.h"


float fisqrt(float num);
void init_random(uint32_t seed);
int32_t rand_int(int32_t min, int32_t max);
float fclamp(float min, float max, float val);

#endif
