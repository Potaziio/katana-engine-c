#ifndef PHYSICS_H
#define PHYSICS_H

#include "shape/shapes.h"

int physics_point_in_aabb(struct vector2 point, struct aabb aabb);
int physics_aabb_in_aabb(struct aabb r1, struct aabb r2);
struct vector2 physics_raycast(struct vector2 origin, struct vector2 direction, float scalar);
struct aabb physics_aabb_minkowski_diff(struct aabb a, struct aabb b);
void physics_aabb_pen_vec2(struct vector2 r, struct aabb aabb);
int physics_ray_vs_aabb(struct vector2 origin, struct vector2 direction, struct aabb aabb, struct vector2* contact_point, struct vector2* contact_normal, float* t_hit_near);

#endif
