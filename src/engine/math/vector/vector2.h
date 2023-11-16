#ifndef VECTOR2_H
#define VECTOR2_H

#include "../math.h"

struct vector2
{
	float x, y;
};

float vector2_mag(const struct vector2 vec);
void vector2_normalize(struct vector2* vec);
float vector2_dot(struct vector2 v1, struct vector2 v2);
float vector2_dist(struct vector2 v1, struct vector2 v2);
struct vector2 vector2_lerp(struct vector2 target, struct vector2 source, float speed);
struct vector2 vector2(float x, float y);
struct vector2 vector2_zero();

#endif 
