#include "font.h"
#include <ctype.h>

struct font* _engine_default_font;

void get_character_index(char c, int* letter_x_index, int* letter_y_index)
{
		// This is hell, we index each character 
		switch (c)
		{
			case ' ':
				*letter_x_index = 15;
				*letter_y_index = 4;
				break;
			case 'j':
				*letter_x_index = 0;
				*letter_y_index = 0;
				break;
			case 'Q':
				*letter_x_index = 1;
				*letter_y_index = 0;
				break;
			case '0':
				*letter_x_index = 2;
				*letter_y_index = 0;
				break;
			case '1':
				*letter_x_index = 3;
				*letter_y_index = 0;
				break;
			case '2':
				*letter_x_index = 4;
				*letter_y_index = 0;
				break;
			case '3':
				*letter_x_index = 5;
				*letter_y_index = 0;
				break;
			case '4':
				*letter_x_index = 6;
				*letter_y_index = 0;
				break;
			case '5':
				*letter_x_index = 7;
				*letter_y_index = 0;
				break;
			case '6':
				*letter_x_index = 8;
				*letter_y_index = 0;
				break;
			case '7':
				*letter_x_index = 9;
				*letter_y_index = 0;
				break;
			case '8':
				*letter_x_index = 10;
				*letter_y_index = 0;
				break;
			case '9':
				*letter_x_index = 11;
				*letter_y_index = 0;
				break;
			case 'b':
				*letter_x_index = 12;
				*letter_y_index = 0;
				break;
			case 'd':
				*letter_x_index = 13;
				*letter_y_index = 0;
				break;
			case 'f':
				*letter_x_index = 14;
				*letter_y_index = 0;
				break;
			case 'g':
				*letter_x_index = 15;
				*letter_y_index = 0;
				break;


			case 'h':
				*letter_x_index = 0;
				*letter_y_index = 1;
				break;
			case 'i':
				*letter_x_index = 1;
				*letter_y_index = 1;
				break;
			case 'k':
				*letter_x_index = 2;
				*letter_y_index = 1;
				break;
			case 'l':
				*letter_x_index = 3;
				*letter_y_index = 1;
				break;
			case 'p':
				*letter_x_index = 4;
				*letter_y_index = 1;
				break;
			case 'q':
				*letter_x_index = 5;
				*letter_y_index = 1;
				break;
			case 't':
				*letter_x_index = 6;
				*letter_y_index = 1;
				break;
			case 'y':
				*letter_x_index = 7;
				*letter_y_index = 1;
				break;
			case 'A':
				*letter_x_index = 8;
				*letter_y_index = 1;
				break;
			case 'B':
				*letter_x_index = 9;
				*letter_y_index = 1;
				break;
			case 'C':
				*letter_x_index = 10;
				*letter_y_index = 1;
				break;
			case 'D':
				*letter_x_index = 11;
				*letter_y_index = 1;
				break;
			case 'E':
				*letter_x_index = 12;
				*letter_y_index = 1;
				break;
			case 'F':
				*letter_x_index = 13;
				*letter_y_index = 1;
				break;
			case 'G':
				*letter_x_index = 14;
				*letter_y_index = 1;
				break;
			case 'H':
				*letter_x_index = 15;
				*letter_y_index = 1;
				break;


			case 'I':
				*letter_x_index = 0;
				*letter_y_index = 2;
				break;
			case 'J':
				*letter_x_index = 1;
				*letter_y_index = 2;
				break;
			case 'L':
				*letter_x_index = 2;
				*letter_y_index = 2;
				break;
			case 'M':
				*letter_x_index = 3;
				*letter_y_index = 2;
				break;
			case 'N':
				*letter_x_index = 4;
				*letter_y_index = 2;
				break;
			case 'O':
				*letter_x_index = 5;
				*letter_y_index = 2;
				break;
			case 'P':
				*letter_x_index = 6;
				*letter_y_index = 2;
				break;
			case 'R':
				*letter_x_index = 7;
				*letter_y_index = 2;
				break;
			case 'S':
				*letter_x_index = 8;
				*letter_y_index = 2;
				break;
			case 'T':
				*letter_x_index = 9;
				*letter_y_index = 2;
				break;
			case 'U':
				*letter_x_index = 10;
				*letter_y_index = 2;
				break;
			case 'V':
				*letter_x_index = 11;
				*letter_y_index = 2;
				break;
			case 'W':
				*letter_x_index = 12;
				*letter_y_index = 2;
				break;
			case 'X':
				*letter_x_index = 13;
				*letter_y_index = 2;
				break;
			case 'Y':
				*letter_x_index = 14;
				*letter_y_index = 2;
				break;
			case 'Z':
				*letter_x_index = 15;
				*letter_y_index = 2;
				break;


			case '!':
				*letter_x_index = 0;
				*letter_y_index = 3;
				break;
			case 'a':
				*letter_x_index = 1;
				*letter_y_index = 3;
				break;
			case 'c':
				*letter_x_index = 2;
				*letter_y_index = 3;
				break;
			case 'e':
				*letter_x_index = 3;
				*letter_y_index = 3;
				break;
			case 'm':
				*letter_x_index = 4;
				*letter_y_index = 3;
				break;
			case 'n':
				*letter_x_index = 5;
				*letter_y_index = 3;
				break;
			case 'o':
				*letter_x_index = 6;
				*letter_y_index = 3;
				break;
			case 'r':
				*letter_x_index = 7;
				*letter_y_index = 3;
				break;
			case 's':
				*letter_x_index = 8;
				*letter_y_index = 3;
				break;
			case 'u':
				*letter_x_index = 9;
				*letter_y_index = 3;
				break;
			case 'v':
				*letter_x_index = 10;
				*letter_y_index = 3;
				break;
			case 'w':
				*letter_x_index = 11;
				*letter_y_index = 3;
				break;
			case 'x':
				*letter_x_index = 12;
				*letter_y_index = 3;
				break;
			case 'z':
				*letter_x_index = 13;
				*letter_y_index = 3;
				break;
			case '(':
				*letter_x_index = 14;
				*letter_y_index = 3;
				break;
			case ')':
				*letter_x_index = 15;
				*letter_y_index = 3;
				break;
			
			case 'K':
				*letter_x_index = 0;
				*letter_y_index = 4;
				break;
			case ':':
				*letter_x_index = 1;
				*letter_y_index = 4;
				break;
			case '.':
				*letter_x_index = 2;
				*letter_y_index = 4;
				break;
			case '_':
				*letter_x_index = 3;
				*letter_y_index = 4;
				break;
			case '*':
				*letter_x_index = 4;
				*letter_y_index = 4;
			case '=':
				*letter_x_index = 5;
				*letter_y_index = 4;
				break;
		};
}

void generate_text_uvs(struct ui_text* text, char c, int vertex_ptr)
{
	int letter_x_index = 0;
	int letter_y_index = 0;

	get_character_index(c, &letter_x_index, &letter_y_index);

	struct vector2 v1_coords = {0};
	struct vector2 v2_coords = {0};
	struct vector2 v3_coords = {0};
	struct vector2 v4_coords = {0};

	// Texture coordinates with the caracter index
	v1_coords = (struct vector2){(letter_x_index * (((float)(text->font->scale) / text->font->image_width))), 
		1.0f - (letter_y_index * (((float)(text->font->scale) / text->font->image_height)))};
	v2_coords = (struct vector2){v1_coords.x + ((float)(text->font->scale) / text->font->image_width), v1_coords.y};

	v3_coords = (struct vector2){v1_coords.x, v1_coords.y - ((float)(text->font->scale) / text->font->image_height)};
	v4_coords = (struct vector2){v2_coords.x, v3_coords.y};

	text->vertices[vertex_ptr].tex_coords = v1_coords;
	text->vertices[vertex_ptr + 1].tex_coords = v2_coords;
	text->vertices[vertex_ptr + 2].tex_coords = v3_coords;	
	text->vertices[vertex_ptr + 3].tex_coords = v4_coords;
}

void text_alloc_data(struct ui_text* text, const char* str, int len)
{
	void* str_cpy = (char*)malloc(sizeof(char) * (len + 1));

	if (str_cpy == NULL)
		logger_log_string(ERROR, "Error allocating memory for text string, returning null\n");

	if (str_cpy == NULL)
		logger_log_string(ERROR, "Error copying memory for ui_text string\n");
	else
		text->str = str_cpy;

	memcpy(text->str, str, len + 1);

	text->str[len] = '\0';

	void* vert_alloc = malloc(sizeof(struct font_sprite_vertex) * 4 * len);
	
	if (vert_alloc == NULL)
		logger_log_string(ERROR, "Error allocating memory for ui_text vertices\n");
	else 
		text->vertices = vert_alloc;

	void* ind_alloc = malloc(sizeof(unsigned int) * 6 * len);

	if (ind_alloc == NULL)
		logger_log_string(ERROR, "Error allocating memory for ui_text_indices\n");
	else 
		text->indices = ind_alloc;
}

void text_generate_data(struct ui_text* text)
{
	int v_index = 0;
	int i_index = 0;
	int i_num_ptr = 0;

	for (int i = 0; i < text->str_len; i++)
	{
		if (text->str[i] == '\0') continue;

		// Generate vertices, this just generates rects that hold our characters
		struct vector3 v1_pos = {(text->position.x + (text->font->char_padding * i)) + text->scale * i, 0.0f, 0.0f};

		text->vertices[v_index].position = (struct vector3){v1_pos.x, text->position.y, 0.0f};
		text->vertices[v_index + 1].position = (struct vector3){v1_pos.x + text->scale, text->position.y, 0.0f};
		text->vertices[v_index + 2].position = (struct vector3){v1_pos.x, text->position.y + text->scale, 0.0f};
		text->vertices[v_index + 3].position = (struct vector3){v1_pos.x + text->scale, text->position.y + text->scale, 0.0f};

		text->vertices[v_index].color = (struct rgba_color){RGBA_NORMALIZED_INT(text->color)};
		text->vertices[v_index + 1].color = (struct rgba_color){RGBA_NORMALIZED_INT(text->color)};
		text->vertices[v_index + 2].color = (struct rgba_color){RGBA_NORMALIZED_INT(text->color)};
		text->vertices[v_index + 3].color = (struct rgba_color){RGBA_NORMALIZED_INT(text->color)};

		generate_text_uvs(text, text->str[i], v_index);

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

void ui_text_mod(struct ui_text* text, const char* str, int length)
{
	text->str = (char*)memset(text->str, 0, sizeof(char) * (text->str_len + 1));

	if (text->str == NULL)
		logger_log_string(ERROR, "Failed re-setting ui_text string memory\n");

	text->vertices = (struct font_sprite_vertex*)memset(text->vertices, 0, sizeof(struct font_sprite_vertex) * 4 * text->str_len);

	if (text->vertices == NULL)
		logger_log_string(ERROR, "Failed re-setting ui_text vertices memory\n");

	text->indices = (unsigned int*)memset(text->indices, 0, sizeof(unsigned int) * 6 * text->str_len);

	if (text->indices == NULL)
		logger_log_string(ERROR, "Failed re-setting ui_text indices memory\n");

	text->str_len = length;

	if (text->str_len > FONT_MAX_STRING_SIZE)
		logger_log_string(WARNING, "Maximum font string size reached!!! string may behave weird\n");

	void* str_realloc = realloc(text->str, sizeof(char) * (length + 1));

	if (str_realloc == NULL)
		logger_log_string(ERROR, "Reallocation of string for ui_text_mod failed\n");
	else
		text->str = str_realloc;

	void* vert_realloc = realloc(text->vertices, sizeof(struct font_sprite_vertex) * 4 * length);
	
	if (vert_realloc == NULL)
		logger_log_string(ERROR, "Reallocation of vertices for ui_text_mod failed\n");
	else 
		text->vertices = vert_realloc;

	void* ind_realloc = realloc(text->indices, sizeof(unsigned int) * 6 * length);

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

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(struct font_sprite_vertex) * 4 * length, text->vertices);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * 6 * length, text->indices);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

struct ui_text* ui_text_init(struct font* font, struct rgba_color color, struct vector2 position, float scale, const char* str, int len)
{
	int str_len = len;

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

	glBufferData(GL_ARRAY_BUFFER, sizeof(struct font_sprite_vertex) * 4 * FONT_MAX_STRING_SIZE, t->vertices, GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6 * FONT_MAX_STRING_SIZE, t->indices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct font_sprite_vertex), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct font_sprite_vertex), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 4, GL_BYTE, GL_FALSE, sizeof(struct font_sprite_vertex), (void*)(5 * sizeof(float)));
 
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

