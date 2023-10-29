#include "camera.h"

vec3 _camera_up;
vec3 _camera_front;

void camera_update(struct camera* camera)
{
	_camera_front[0] = 0.0f;
	_camera_front[1] = 0.0f;
	_camera_front[2] = -1.0f;

	_camera_up[0] = 0.0f;
	_camera_up[1] = 1.0f;
	_camera_up[2] = 0.0f;

	vec3 pos = {camera->position.x, camera->position.y, camera->position.z};
	vec3 dest;

	glm_mat4_identity(camera->projection_mat);

	switch(camera->type)
	{
		case ORTHOGRAPHIC:
			glm_ortho(0.0f, camera->bounds.x, camera->bounds.y, 
					0.0f, 0.0f, 1000.0f, camera->projection_mat);
			break;
		case PERSPECTIVE:
			glm_perspective(glm_rad(75.0f), camera->bounds.x / camera->bounds.y, 
					0.1f, 10000.0f, camera->projection_mat);
			break;
	}

	glm_mat4_identity(camera->view_mat);
	glm_vec3_add(pos, _camera_front, dest);
	glm_lookat(pos, dest, _camera_up, camera->view_mat);
}

void camera_send_matrices_to_shader(struct camera* camera, struct shader* shader, char* proj_var, char* view_var)
{
	shader_send_mat4(shader, camera->projection_mat, proj_var);
	shader_send_mat4(shader, camera->view_mat, view_var);
}

