#ifndef CAMERA_H
#define CAMERA_H

#include "../../../include/cglm/include/cglm/cglm.h"
#include "../../../include/cglm/include/cglm/mat4.h"
#include "../../../include/cglm/include/cglm/vec3.h"
#include "../../../include/cglm/include/cglm/vec2.h"

#include "../math/vector/vector3.h"
#include "../math/vector/vector2.h"
#include "../shader/shader.h"

extern vec3 _camera_up;
extern vec3 _camera_front;

enum CAMERA_TYPE
{
	ORTHOGRAPHIC,
	PERSPECTIVE,
};

struct camera
{
	mat4 projection_mat, view_mat;
	struct vector3 position;
	struct vector2 bounds;

	enum CAMERA_TYPE type;
};

void camera_update(struct camera* camera);
void camera_send_matrices_to_shader(struct camera* camera, struct shader* shader, char* proj_var, char* view_var);

#endif 

