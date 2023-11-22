#include "math.h"

float fisqrt(float num)
{
	int64_t i;
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

void init_random(uint32_t seed)
{ srand(seed); }

int32_t rand_int(int32_t min, int32_t max)
{ return (rand() % (max - min)) + min; }

float fclamp(float min, float max, float val)
{ return val < min ? min : val > max ? max : val; }

double deg_2_rad(double deg)
{ return deg * (KAT_PI / 180); }
