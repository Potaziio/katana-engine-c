#include "engine/core/engine.h"
#include "engine/entity/entity.h"
#include "engine/input/input.h"
#include "engine/render/font.h"
#include "engine/utils/color.h"
#include "engine/utils/logger.h"
#include "engine/window/window.h"
#include <stdlib.h>

void app_start(void);
void app_update(void);
void app_on_tick(void);
void app_end(void);
void app_preload(void);

char* accepted_commands[] = 
{ "move", "add", "delete", "window"};

struct game_ui_command
{
	char* str;
	int len;
	struct ui_text* ui_text;
	int is_typing;
	entity command_bar;
	entity text_cursor;
	int text_scale;
};

struct game_ui_command command;

void move_entity(entity e, float x, float y)
{
	struct transform* t = ENTITY_GET_TRANSFORM(e);
	if (t != NULL)
	{
		logger_log_string(LOG, "Moving entity\n");
		t->position = (struct vector2){x, y};
	}
}

void pop_entity(entity e)
{
	engine_pop_entity(global_engine, e);
}

void create_entity_at(float x, float y, float s_x, float s_y, int color)
{
	logger_log_string(LOG, "Creating entity\n");
	entity e = engine_create_entity(global_engine, TRANSFORM | SPRITE2D);
	struct transform* t = ENTITY_GET_TRANSFORM(e);
	struct sprite2d* spr = ENTITY_GET_SPRITE2D(e);
	t->position = (struct vector2){x, y};
	t->scale = (struct vector2){s_x, s_y};

	struct rgba_color clr = {RGBA_BLACK};

	switch (color) {
		case 0:
			clr = (struct rgba_color){RGBA_BLACK};
			break;
		case 1:
			clr = (struct rgba_color){RGBA_RED};
			break;
		case 2:
			clr = (struct rgba_color){RGBA_GREEN};
			break;
		case 3:
			clr = (struct rgba_color){RGBA_BLUE};
			break;
	}

	spr->color = clr;
	spr->config = SPRITE2D_TOP_LEFT;
}

void parse_command(char* string)
{
	if (string[0] == '\0') return;

	char* cpy = strdup(string);
	char* tok = strtok(cpy, " ");
	int command = -1;
		
	char** parsed_command = (char**)malloc(sizeof(char*) * 10);

	int i = 0;

	do 
	{
		if (command < 0)
		{
			for (int i = 0; i < (int)(sizeof(accepted_commands) / sizeof(char*)); i++)
			{
				if (!strcmp(tok, accepted_commands[i]))
				{
					command = i;
					break;
				}
			}
		}

		if (command < 0) break;

		parsed_command[i - 1] = (char*)malloc(strlen(tok) + 1);
		parsed_command[i - 1] = (char*)memcpy(parsed_command[i - 1], tok, strlen(tok));

		i++;
	}
	while ((tok = strtok(NULL, " ")) != NULL);

	switch (command)
	{
		case -1:
			break;
		case 0:
			move_entity(atoi(parsed_command[0]), atoi(parsed_command[1]), atoi(parsed_command[2]));
			break;
		case 1:
			create_entity_at(atoi(parsed_command[0]), atoi(parsed_command[1]), atoi(parsed_command[2]), atoi(parsed_command[3]), atoi(parsed_command[4]));
			break;
		case 2:
			pop_entity(atoi(parsed_command[0]));
			break;
		case 3:
			if (strcmp(parsed_command[0], "fullscreen"))
				window_toggle_fullscreen(&global_engine->engine_window);
			break;
	};

	for (; i > 0; i--)
		free(parsed_command[i]);

	free(parsed_command);
	free(cpy);
}

void game_command_append_key(struct game_ui_command* command, char key)
{
	command->str = realloc(command->str, command->len + 2);
	if (command->str == NULL)
	{
		logger_log_string(ERROR, "Error reallocating string\n");
	}	
	int len = strlen(command->str);
	command->str[len] = key;
	command->str[len + 1] = '\0';
	command->len++;
}

void game_command_remove_last(struct game_ui_command* command)
{
	command->str[command->len - 1] = '\0';
	memset(command->str + (command->len), 0, 1); 
	command->str = realloc(command->str, command->len);
	if (command->str == NULL)
	{
		logger_log_string(ERROR, "Error reallocating string\n");
	}	
	command->len--;
}

int main(void)
{
	struct engine app_engine;

	engine_app_start_callback_func(&app_engine, app_start);
	engine_app_update_callback_func(&app_engine, app_update);
	engine_app_tick_update_callback_func(&app_engine, app_on_tick);
	engine_app_end_callback_func(&app_engine, app_end);
	engine_app_preload_callback_func(&app_engine, NULL);

	app_engine.engine_window = (struct window)
	{
		.width = 1440, 
		.height = 768,
		.title = "Katana Engine (dev 0.0)",
		.color = {RGBA_GRAY}
	};

	if (engine_init(&app_engine))
		logger_log_string(SUCCESS, "Engine init was sucessful.\n");

	engine_update(&app_engine);

	if (engine_end(&app_engine))
		logger_log_string(SUCCESS, "Engine exit was successful.\n");

	return 0;
}

void app_start(void)
{
	engine_camera.type = CAMERA_ORTHOGRAPHIC;
	global_engine->config = ENGINE_SILENCE_ENTITY_LOG | ENGINE_UPDATE_CAMERA_BOUNDS;

	command.str = malloc(1);
	command.len = 0;
	*command.str = '\0';
	command.text_scale = 20.0f;
}

void app_update(void)
{
	unsigned int key = input_get_text_stream();

	if (input_get_key(GLFW_KEY_LEFT_SHIFT) && input_get_key_down(GLFW_KEY_TAB))
	{
		command.is_typing = 1;

		command.command_bar = engine_create_entity(global_engine, TRANSFORM | SPRITE2D);
		struct transform* bar_t = ENTITY_GET_TRANSFORM(command.command_bar);
		struct sprite2d* bar_sp = ENTITY_GET_SPRITE2D(command.command_bar);

		bar_t->position.y = engine_camera.bounds.y - command.text_scale;
		bar_t->scale = (struct vector2){engine_camera.bounds.x, command.text_scale};
		bar_sp->color = (struct rgba_color){RGBA_BLACK};
		bar_sp->config = SPRITE2D_TOP_LEFT;

		command.text_cursor = engine_create_entity(global_engine, TRANSFORM | SPRITE2D);
		struct transform* cursor_t = ENTITY_GET_TRANSFORM(command.text_cursor);
		struct sprite2d* cursor_sp = ENTITY_GET_SPRITE2D(command.text_cursor);

		cursor_t->position.y = (engine_camera.bounds.y - command.text_scale) + 5;
		cursor_t->scale = (struct vector2){3.0f, command.text_scale - 10};
		cursor_sp->color = (struct rgba_color){RGBA_WHITE};
		cursor_sp->config = SPRITE2D_TOP_LEFT;

		command.ui_text = ui_text_init(_engine_default_font, (struct rgba_color){RGBA_WHITE}, 
				(struct vector2){0.0f, engine_camera.bounds.y - command.text_scale}, command.text_scale, "command: ", 9);
	}

	if (command.is_typing)
	{
		struct transform* cursor_t = ENTITY_GET_TRANSFORM(command.text_cursor);

		if (key != 0)
		{
			game_command_append_key(&command, key);
			ui_text_mod(command.ui_text, command.str, command.len );
			cursor_t->position.x += command.text_scale;
		}
		
		if (input_get_key_down(GLFW_KEY_BACKSPACE))
		{
			game_command_remove_last(&command);
			ui_text_mod(command.ui_text, command.str, command.len);
			cursor_t->position.x -= command.text_scale;
		}

		ui_text_render(command.ui_text);

		if (input_get_key(GLFW_KEY_ENTER))
		{
			command.is_typing = 0;

			engine_pop_entity(global_engine, command.command_bar);
			engine_pop_entity(global_engine, command.text_cursor);

			parse_command(command.str);
			ui_text_free(command.ui_text);

			memset(command.str, 0, command.len + 1);
			command.str = realloc(command.str, 1);
			command.len = 0;
			*command.str = '\0';
		}
	}
}

void app_on_tick(void)
{
}

void app_end(void)
{
	if (command.is_typing)
	{
		ui_text_free(command.ui_text);
		free(command.str);
	}
}

void app_preload(void)
{

}

