#include "math.h"

float fisqrt(float num)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = num * 0.5F;
	y  = num;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
												// y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
	return y;
}

void init_random(unsigned int seed)
{ srand(seed); }

int rand_int(int min, int max)
{ return (rand() % (max - min)) + min; }

