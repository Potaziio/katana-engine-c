#include "physics.h"
#include <complex.h>
#include <iso646.h>

int physics_point_in_aabb(struct vector2 point, struct aabb aabb)
{
	return point.x >= aabb.position.x && point.y >= aabb.position.y &&
		   point.x < aabb.position.x + aabb.scale.x && point.y < aabb.position.y + aabb.scale.y;
}

int physics_aabb_in_aabb(struct aabb r1, struct aabb r2)
{
	return r1.position.x < r2.position.x + r2.scale.x && r1.position.x + r1.scale.x > r2.position.x &&
		r1.position.y < r2.position.y + r2.scale.y && r1.position.y + r1.scale.y > r2.position.y;
}

struct vector2 physics_raycast(struct vector2 origin, struct vector2 direction, float scalar)
{
	struct vector2 res = {origin.x + (direction.x * scalar), origin.y + (direction.y * scalar)};
 	return res;
}

struct aabb physics_aabb_minkowski_diff(struct aabb a, struct aabb b)
{
	struct aabb result;
	result.position.x = a.position.x - b.position.x;
	result.position.y = a.position.y - b.position.y;
	result.scale.x = a.scale.x + b.scale.x;
	result.scale.y = a.scale.y + b.scale.y;

	return result;
}

int physics_ray_vs_aabb(struct vector2 origin, struct vector2 direction, struct aabb aabb, struct vector2* contact_point, struct vector2* contact_normal, float* t_hit_near)
{
	struct vector2 t_near = {(aabb.position.x - origin.x) / direction.x, (aabb.position.y - origin.y) / direction.y};
	struct vector2 t_far = {((aabb.position.x + aabb.scale.x) - origin.x) / direction.x, ((aabb.position.y + aabb.scale.y) - origin.y) / direction.y};

	if (t_near.x > t_far.x) 
	{
		// Swap values 
		float tmp = t_near.x;
		t_near.x = t_far.x;
		t_far.x = tmp;
	}
	
	if (t_near.y > t_far.y)
	{
		float tmp = t_near.y;
		t_near.y = t_far.y;
		t_far.y = tmp;
	}

	if (t_near.x > t_far.y || t_near.y > t_far.x) return 0;

	*t_hit_near = fmaxf(t_near.x, t_near.y);
	float t_hit_far = fminf(t_far.x, t_far.y);

	// Dont want to check collisions behind ray
	if (t_hit_far < 0) return 0;

	contact_point->x = origin.x + *t_hit_near * direction.x;
	contact_point->y = origin.x + *t_hit_near * direction.y;

	if (t_near.x > t_near.y)
	{
		if (direction.x < 0)
		{
			contact_normal->x = 1;
			contact_normal->y = 0;
		}
		else 
		{
			contact_normal->x = -1;
			contact_normal->y = 0;

		}
	} 
	else if (t_near.x < t_near.y)
	{
		if (direction.y < 0)
		{
			contact_normal->x = 0;
			contact_normal->y = 1;	
		}
		else 
		{
			contact_normal->x = 0;
			contact_normal->y = -1;
		}
	}

	return 1;
}

