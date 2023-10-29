#ifndef SHAPES_H
#define SHAPES_H

#include "../../vector/vector2.h"
#include "../../vector/vector3.h"

struct segment
{
	struct vector2 position;
};

struct aabb
{
	struct vector2 position;
	struct vector2 scale;
	int tile;
};

#endif
