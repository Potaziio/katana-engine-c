#include "vector2.h"

float vector2_mag(const struct vector2 vec)
{
	float mag_squared = (vec.x * vec.x) + (vec.y * vec.y);
	float invsqrt = sqrt(mag_squared);
	/* float invsqrt = fisqrt(mag_squared); */

	return invsqrt;
}

void vector2_normalize(struct vector2* vec)
{
	float m = vector2_mag(*vec);

	if (m <= 0) return;

	vec->x /= m;
	vec->y /= m;
}

struct vector2 vector2_lerp(struct vector2 target, struct vector2 source, float speed)
{
	struct vector2 res;
	res.x = source.x + ((target.x - source.x) * speed);
	res.y = source.y + ((target.y - source.y) * speed);

	return res;
}

float vector2_dot(struct vector2 v1, struct vector2 v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y);
}

float vector2_dist(struct vector2 v1, struct vector2 v2)
{
	float d = sqrt(((v1.x - v2.x) * (v1.x - v2.x)) + ((v1.y - v2.y) * (v1.y - v2.y)));
	return d;
}

struct vector2 vector2(float x, float y)
{	return (struct vector2){x, y}; }

struct vector2 vector2_zero()
{ return (struct vector2){0.0f, 0.0f}; }

// Allocates memory for the string 
void vector2_to_string(struct vector2 vec, char* dest)
{
 	sprintf(dest, "(%.2f, %.2f)\n", vec.x, vec.y); 
}

