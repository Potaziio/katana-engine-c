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
	return sqrt(d);
}

