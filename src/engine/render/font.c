#include "font.h"
#include "render.h"
#include <ctype.h>

struct font* _engine_default_font;

void ui_font_get_fnt_data(struct font* font, char* file)
{
	logger_log_string(LOG, "Reading font info from: ");
	printf("%s\n", file);

	char* font_string= malloc(1024 * 1024 * 2);

	io_utils_read_file(file,font_string);

	char * curLine = font_string;

	int32_t i = 0;

	while(curLine)
	{
		char * nextLine = strchr(curLine, '\n');
		if (nextLine) *nextLine = '\0';  // temporarily terminate the current line

		char* id = strtok(curLine, "=");

		while ((id = strtok(NULL, "=")) != NULL)
		{
			int32_t num = atoi(id);
			font->data[i] = num;
			i++;
		}


		if (nextLine) *nextLine = '\n';  // then restore newline-char, just to be tidy    
		curLine = nextLine ? (nextLine+1) : NULL;
	}

	free(font_string);
}

void generate_uvs_from_fnt(struct ui_text* text, int32_t x, int32_t y, int32_t w, int32_t h, int32_t vertex_ptr)
{
	struct vector2 v1_coords = {0};
	struct vector2 v2_coords = {0};
	struct vector2 v3_coords = {0};
	struct vector2 v4_coords = {0};

	// Texture coordinates with the caracter index
	v1_coords = (struct vector2){(((float)(x) / text->font->image_width)), 1.0f - (((float)(y) / text->font->image_height))};
	v2_coords = (struct vector2){((float)(x + w) / text->font->image_width), v1_coords.y};

	v3_coords = (struct vector2){v1_coords.x, 1.0f - ((float)(y + h) / text->font->image_height)};
	v4_coords = (struct vector2){v2_coords.x, v3_coords.y};

	text->vertices[vertex_ptr].tex_coords = v1_coords;
	text->vertices[vertex_ptr + 1].tex_coords = v2_coords;
	text->vertices[vertex_ptr + 2].tex_coords = v3_coords;	
	text->vertices[vertex_ptr + 3].tex_coords = v4_coords;
}

void text_alloc_data(struct ui_text* text, const char* str, int32_t len)
{
	size_t str_cpy_size = sizeof(char) * len + 1;

	void* str_cpy = (char*)malloc(str_cpy_size);

	if (str_cpy == NULL)
		logger_log_string(ERROR, "Error allocating memory for text string, returning null\n");

	if (str_cpy == NULL)
		logger_log_string(ERROR, "Error copying memory for ui_text string\n");
	else
		text->str = str_cpy;

	memcpy(text->str, str, len);

	text->str[len] = '\0';

	size_t vert_size = sizeof(struct font_sprite_vertex) * 4 * len;

	void* vert_alloc = malloc(vert_size);
	
	if (vert_alloc == NULL)
		logger_log_string(ERROR, "Error allocating memory for ui_text vertices\n");
	else 
		text->vertices = vert_alloc;

	size_t ind_size = sizeof(uint32_t) * 6 * len;

	void* ind_alloc = malloc(ind_size);

	if (ind_alloc == NULL)
		logger_log_string(ERROR, "Error allocating memory for ui_text_indices\n");
	else 
		text->indices = ind_alloc;
}

void text_generate_data(struct ui_text* text)
{
	int32_t v_index = 0;
	int32_t i_index = 0;
	int32_t i_num_ptr = 0;

	// Position of our cursor

	struct vector2 v1_pos;

	for (int32_t i = 0; i < text->str_len; i++)
	{
		if (text->str[i] == '\0') continue;

		int32_t fnt_index = (text->str[i] - 32) * 10;
		struct vector2 scale = vector2(text->font->data[fnt_index + 3], text->font->data[fnt_index + 4]);
		struct vector2 offset = vector2(text->font->data[fnt_index + 5], text->font->data[fnt_index + 6]);
		struct vector2 pos = vector2(text->font->data[fnt_index + 1], text->font->data[fnt_index + 2]);
		int32_t advance = text->font->data[fnt_index + 7];

		// Generate vertices, this just generates rects that hold our characters

		v1_pos.x += text->font->char_padding + (i > 0 && text->str[i - 1] != '\n' ? advance : 0.0f);
		
		if (text->str[i] == '\n')
		{
			v1_pos.x = 0.0f;
			v1_pos.y += 100.0f;
		}

		text->vertices[v_index].position = (struct vector2){v1_pos.x + offset.x, v1_pos.y + offset.y};
		text->vertices[v_index + 1].position = (struct vector2){v1_pos.x + scale.x + offset.x, v1_pos.y + offset.y};
		text->vertices[v_index + 2].position = (struct vector2){v1_pos.x + offset.x, v1_pos.y + scale.y + offset.y};
		text->vertices[v_index + 3].position = (struct vector2){v1_pos.x + scale.x + offset.x, v1_pos.y + scale.y + offset.y};

		text->vertices[v_index].color = text->color;
		text->vertices[v_index + 1].color = text->color;
		text->vertices[v_index + 2].color = text->color;
		text->vertices[v_index + 3].color = text->color;

		generate_uvs_from_fnt(text, pos.x, pos.y, scale.x, scale.y, v_index);

		text->indices[i_index + 0] = i_num_ptr + 0;
		text->indices[i_index + 1] = i_num_ptr + 1;
		text->indices[i_index + 2] = i_num_ptr + 2;
		text->indices[i_index + 3] = i_num_ptr + 2;
		text->indices[i_index + 4] = i_num_ptr + 3;
		text->indices[i_index + 5] = i_num_ptr + 1;

		v_index += 4;
		i_num_ptr += 4;
		i_index += 6;
	}
}

void ui_text_mod(struct ui_text* text, const char* str, int32_t length)
{
	if ((text->str != NULL) && !strcmp(text->str, str) && length == text->str_len) return;

	size_t t_str_size = sizeof(char) * text->str_len + 1;

	text->str = (char*)memset(text->str, 0, t_str_size);

	if (text->str == NULL)
		logger_log_string(ERROR, "Failed re-setting ui_text string memory\n");

	size_t t_vert_size = sizeof(struct font_sprite_vertex) * 4 * text->str_len;

	text->vertices = (struct font_sprite_vertex*)memset(text->vertices, 0, t_vert_size);

	if (text->vertices == NULL)
		logger_log_string(ERROR, "Failed re-setting ui_text vertices memory\n");

	size_t t_ind_size = sizeof(uint32_t) * 6 * text->str_len;

	text->indices = (unsigned int*)memset(text->indices, 0, t_ind_size);

	if (text->indices == NULL)
		logger_log_string(ERROR, "Failed re-setting ui_text indices memory\n");

	text->str_len = length;

	if (text->str_len > FONT_MAX_STRING_SIZE)
		logger_log_string(WARNING, "Maximum font string size reached!!! string may behave weird\n");

	size_t str_size = sizeof(char) * length + 1;

	void* str_realloc = realloc(text->str, str_size);

	if (str_realloc == NULL)
		logger_log_string(ERROR, "Reallocation of string for ui_text_mod failed\n");
	else
		text->str = str_realloc;

	size_t vert_size = sizeof(struct font_sprite_vertex) * 4 * length;

	void* vert_realloc = realloc(text->vertices, vert_size);
	
	if (vert_realloc == NULL)
		logger_log_string(ERROR, "Reallocation of vertices for ui_text_mod failed\n");
	else 
		text->vertices = vert_realloc;

	size_t ind_size = sizeof(uint32_t) * 6 * length;

	void* ind_realloc = realloc(text->indices, ind_size);

	if (ind_realloc == NULL)
		logger_log_string(ERROR, "Reallocation of indices for ui_text_mod failed\n");
	else
		text->indices = ind_realloc;

	text->str = strcpy(text->str, str);

	text->str[text->str_len] = '\0';

	text_generate_data(text);

	glBindVertexArray(text->vao);
	glBindBuffer(GL_ARRAY_BUFFER, text->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, text->ebo);

	glBufferSubData(GL_ARRAY_BUFFER, 0, vert_size, text->vertices);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, ind_size, text->indices);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

struct ui_text* ui_text_init(struct font* font, struct rgba_color color, struct vector2 position, float scale, const char* str, int32_t len)
{
	int32_t str_len = len;

	if (str_len > FONT_MAX_STRING_SIZE)
		logger_log_string(WARNING, "Maximum font string size reached!!! string may behave weird\n");

	// Allocate memory for our text
	struct ui_text* t = (struct ui_text*)malloc(sizeof(struct ui_text));
	t->position = position;
	t->scale = scale;
	t->font = font;
	t->color = color;
	t->str_len = str_len;

	if (str == NULL)
		t->str[0] = '\0';

	text_alloc_data(t, str, str_len);
	text_generate_data(t);

	glGenBuffers(1, &t->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, t->vbo);
	glGenVertexArrays(1, &t->vao);
	glBindVertexArray(t->vao);
	glGenBuffers(1, &t->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, t->ebo);

	size_t vert_size = sizeof(struct font_sprite_vertex) * 4 * FONT_MAX_STRING_SIZE;
	size_t ind_size =  sizeof(uint32_t) * 6 * FONT_MAX_STRING_SIZE;

	glBufferData(GL_ARRAY_BUFFER, vert_size, t->vertices, GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind_size, t->indices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(struct font_sprite_vertex), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct font_sprite_vertex), (void*)(2 * sizeof(float)));
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(struct font_sprite_vertex), (void*)(4 * sizeof(float)));
 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return t;
}

void ui_text_render(struct ui_text* text)
{
	shader_use(_render_font_shader);

	texture_bind(*text->font->bitmap);
	glBindVertexArray(text->vao);
	glBindBuffer(GL_ARRAY_BUFFER, text->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, text->ebo);

	mat4 transform;
	vec3 glm_scale_vec = {fclamp(0, 100, text->scale), fclamp(0, 100, text->scale), 0.0f};
	vec3 glm_transform_vec = {text->position.x, text->position.y, 0.0f};

	glm_mat4_identity(transform);
	glm_translate(transform, glm_transform_vec);
	glm_scale(transform, glm_scale_vec);
	shader_send_mat4(_render_font_shader, transform, "transform");

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glDrawElements(GL_TRIANGLES, 6 * text->str_len, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	texture_unbind();

	shader_detach(_render_font_shader);
}

void ui_text_free(struct ui_text* text)
{
	free(text->vertices);
	free(text->indices);
	free(text->str);

	glDeleteBuffers(1, &text->vbo);
	glDeleteBuffers(1, &text->ebo);
	glDeleteVertexArrays(1, &text->vao);

	free(text);
}

