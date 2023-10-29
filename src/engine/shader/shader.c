#include "shader.h"

int shader_load_and_compile(struct shader* shader, char* vertex_path, char* fragment_path)
{
	unsigned int vertex_id, fragment_id;

	logger_log_string(LOG, "Loading and compiling shaders.\n");
	printf("%s\n%s\n\n", vertex_path, fragment_path);

	if (!io_utils_file_exists(vertex_path) || !io_utils_file_exists(fragment_path)) 
	{
		logger_log_string(ERROR, "File doesn't exist.\n");
		return 0;
	};

	shader->vertex_string = (char*)malloc(sizeof(char) * io_utils_file_length(vertex_path));
	io_utils_read_file(vertex_path, shader->vertex_string);

	shader->fragment_string = (char*)malloc(sizeof(char) * io_utils_file_length(fragment_path));
	io_utils_read_file(fragment_path, shader->fragment_string);

	logger_log_string(SUCCESS, "Shaders loaded successfully.\n");

	vertex_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_id, 1, (const char* const*)&shader->vertex_string, NULL);
	glCompileShader(vertex_id);

	int success;
	char log[512];
	glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertex_id, 512, NULL, log);
		shader_free_memory(shader);
		logger_log_string(ERROR, "Shader vertex compilation failed.\n");
		fputs(log, stderr);
		return 0;
	}

	fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_id, 1,  (const char* const*)&shader->fragment_string, NULL);
	glCompileShader(fragment_id);

	glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragment_id, 512, NULL, log);
		shader_free_memory(shader);
		logger_log_string(ERROR, "Shader fragment compilation failed.\n");
		fputs(log, stderr);
		return 0;
	}

	shader->id = glCreateProgram();

	glAttachShader(shader->id, vertex_id);
	glAttachShader(shader->id, fragment_id);

	glLinkProgram(shader->id);

	glGetProgramiv(shader->id, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shader->id, 512, NULL, log);
		logger_log_string(ERROR, "Shader program link failed.\n");
		fputs(log, stderr);
		return 0;
	}

	shader->was_initialized = 1;
	shader->was_freed = 0;

	glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);
	
	free(shader->vertex_string);
	free(shader->fragment_string);

	shader_detach(shader);

	logger_log_string(SUCCESS, "Shaders compiled successfully.\n\n");

	return 1;
}

int shader_send_float(struct shader* shader, float val, char* name)
{	
	GLint loc = glGetUniformLocation(shader->id, name);
	glUniform1f(loc, val);

	return 1;
}

int shader_send_mat4(struct shader* shader, mat4 mat, char* name)
{
	GLint loc = glGetUniformLocation(shader->id, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)mat);

	return 1;
}


void shader_free_memory(struct shader* shader)
{
	logger_log_string(WARNING, "Freeing shader memory.\n\n");
	printf("%s%s[DELETED]%s Shader ID: %d\n\n", COLOR_START, LOG_YELLOW, COLOR_END, shader->id);

    glDeleteProgram(shader->id);

	shader->was_freed = 1;
}

void shader_use(struct shader* shader)
{
	if (shader->being_used) return;

	glUseProgram(shader->id);
	shader->being_used = 1;
}

void shader_detach(struct shader* shader)
{
	if (!shader->being_used) return;

	glUseProgram(0);
	shader->being_used = 0;
}

