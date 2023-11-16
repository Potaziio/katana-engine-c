#ifndef MATH_H
#define MATH_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

float fisqrt(float num);
void init_random(unsigned int seed);
int rand_int(int min, int max);
float fclamp(float min, float max, float val);

#endif
