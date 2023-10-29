#ifndef SHADER_H
#define SHADER_H

#define GLFW_INCLUDE_NONE
#include "../../../include/GLFW/include/GLFW/glfw3.h"
#include "../../../include/GL/include/GL/glew.h"

#include "../utils/io_utils.h"
#include "../utils/logger.h"

#include "../../../include/cglm/include/cglm/mat4.h"

struct shader
{
	char* vertex_string;
	char* fragment_string;

	int being_used;

	unsigned int id;

	int was_initialized;
	int was_freed;
};

int shader_load_and_compile(struct shader* shader, char* vertex_path, char* fragment_path);

void shader_use(struct shader* shader);
void shader_detach(struct shader* shader);

int shader_send_float(struct shader* shader, float val, char* name);
int shader_send_mat4(struct shader* shader, mat4 mat, char* name);

void shader_free_memory(struct shader* shader);

/* void shader_send_mat4(struct shader* shader, const char* var_name, mat4 mat); */

#endif
