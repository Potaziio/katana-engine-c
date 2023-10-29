#include "engine.h"

struct engine* global_engine;
struct camera engine_camera;
int engine_fps;

void engine_app_start_callback_func(struct engine* engine, void (*func)(void))
{ engine->app_start_func = func; }

void engine_app_update_callback_func(struct engine* engine, void (*func)(void))
{ engine->app_update_func = func; }

void engine_app_end_callback_func(struct engine* engine, void (*func)(void))
{ engine->app_end_func = func; }

void engine_app_tick_update_callback_func(struct engine *engine, void (*func)(void))
{ engine->app_on_tick_func = func; } 

int engine_init(struct engine* engine_ptr)
{	
	logger_log_string(LOG, "Creating window...\n");
	int status = window_create(&engine_ptr->engine_window);

	if (!status)
		return 0;

	engine_ptr->delta_time = 0.0f;

	engine_camera.position = (struct vector3){0.0f, 0.0f, 0.0f};

	engine_camera.bounds = (struct vector2){engine_ptr->engine_window.width, engine_ptr->engine_window.height};

	// Allocate memory for global shaders
	_render_default_shader = (struct shader*) malloc(sizeof(struct shader));
	_render_default_tex_shader = (struct shader*) malloc(sizeof(struct shader));
	_render_line_shader = (struct shader*) malloc(sizeof(struct shader));
	_render_batch_simple_shader = (struct shader*) malloc(sizeof(struct shader));

	engine_ptr->config = 0;

	shader_load_and_compile(_render_default_shader,
			"../src/engine/assets/shaders/default_vertex.glsl", 
			"../src/engine/assets/shaders/default_fragment.glsl");

	shader_load_and_compile(_render_line_shader,
			"../src/engine/assets/shaders/line_vertex.glsl",
			"../src/engine/assets/shaders/line_fragment.glsl");

	shader_load_and_compile(_render_default_tex_shader, 
			"../src/engine/assets/shaders/default_tex_vertex.glsl", 
			"../src/engine/assets/shaders/default_tex_fragment.glsl");

	shader_load_and_compile(_render_batch_simple_shader, 
			"../src/engine/assets/shaders/default_batch_simple_vertex.glsl",
			"../src/engine/assets/shaders/default_batch_simple_fragment.glsl");

	global_engine = engine_ptr;

	init_random(time(NULL));

	// We create allocate the map for each type
	transform_hashmap_create(&engine_ptr->transform_components);
	sprite2d_hashmap_create(&engine_ptr->sprite2d_components);
	textured_sprite2d_hashmap_create(&engine_ptr->textured_sprite2d_components);
	sprite2d_batch_simple_hashmap_create(&engine_ptr->sprite2d_batch_simple_components);
	debug_line_hashmap_create(&engine_ptr->debug_line_components);

	engine_ptr->entity_num = 0;

	for (int i = 0; i < ENGINE_MAX_ENTITIES; i++)
		engine_ptr->entities[i] = 0;

	// Run this before returning to make sure everything is allocated and initialized
	engine_ptr->app_start_func();

	// Initialize all entities with sprites
	for (int i = 0; i < engine_ptr->sprite2d_components.size; i++)
		render_system_init_sprite2d(&engine_ptr->transform_components, &engine_ptr->sprite2d_components, engine_ptr->sprite2d_components.key[i]);

	// Initialize all entities with textured sprites
	for (int i = 0; i < engine_ptr->textured_sprite2d_components.size; i++)
		render_system_init_textured_sprite2d(&engine_ptr->transform_components, &engine_ptr->textured_sprite2d_components, engine_ptr->textured_sprite2d_components.key[i]);

	// Initialize all entities with spritebatch sprites
	for (int i = 0; i < engine_ptr->sprite2d_batch_simple_components.size; i++)
		render_system_init_sprite2d_batch_simple(&engine_ptr->sprite2d_batch_simple_components, engine_ptr->sprite2d_batch_simple_components.key[i]);

	for (int i = 0; i < engine_ptr->debug_line_components.size; i++)
		render_system_init_debug_line(&engine_ptr->debug_line_components, engine_ptr->debug_line_components.key[i]);

	return status;
}

void engine_update(struct engine* engine_ptr)
{
	// TODO: Separate into files
	
	float start_time = engine_get_mills();

	// for fps calculations
	double prev_time = engine_get_mills();

	while (!window_should_close(&engine_ptr->engine_window))
	{
		// We leave this at the beggining of the loop
		double current_time = engine_get_mills();

		engine_fps++;

		for (int i = 0; i < engine_ptr->sprite2d_components.size; i++)
			render_system_init_sprite2d(&engine_ptr->transform_components, &engine_ptr->sprite2d_components, engine_ptr->sprite2d_components.key[i]);

		for (int i = 0; i < engine_ptr->textured_sprite2d_components.size; i++)
			render_system_init_textured_sprite2d(&engine_ptr->transform_components, &engine_ptr->textured_sprite2d_components, engine_ptr->textured_sprite2d_components.key[i]);

		for (int i = 0; i < engine_ptr->sprite2d_batch_simple_components.size; i++)
			render_system_init_sprite2d_batch_simple(&engine_ptr->sprite2d_batch_simple_components, engine_ptr->sprite2d_batch_simple_components.key[i]);

		for (int i = 0; i < engine_ptr->debug_line_components.size; i++)
			render_system_init_debug_line(&engine_ptr->debug_line_components, engine_ptr->debug_line_components.key[i]);

		window_begin_frame(&engine_ptr->engine_window);

		// We update the camera bounds to the screen dimensions here
		engine_camera.bounds = (struct vector2){engine_ptr->engine_window.width,
												engine_ptr->engine_window.height};
		// Then we update the camera matrices
		camera_update(&engine_camera);

		// We send matrices to the default and line shaders
		// Default 
		shader_use(_render_default_shader);
		camera_send_matrices_to_shader(&engine_camera, 
				_render_default_shader, "projection", "view");
		shader_detach(_render_default_shader);

		// Texture
		shader_use(_render_default_tex_shader);
		camera_send_matrices_to_shader(&engine_camera,
				_render_default_tex_shader, "projection", "view");
		shader_detach(_render_default_tex_shader);

		// Line 
		shader_use(_render_line_shader);
		camera_send_matrices_to_shader(&engine_camera, 
				_render_line_shader, "projection", "view");
		shader_detach(_render_line_shader);

		// Batch simple
		shader_use(_render_batch_simple_shader);
		camera_send_matrices_to_shader(&engine_camera,
				_render_batch_simple_shader, "projection", "view");
		shader_detach(_render_batch_simple_shader);

		// Render here
		engine_ptr->app_update_func();

		// Render all entities with components of type sprite2d_batch_simple
		shader_use(_render_batch_simple_shader);
		for (int i = 0; i < engine_ptr->sprite2d_batch_simple_components.size; i++)
			render_system_render_sprite2d_batch_simple(&engine_ptr->sprite2d_batch_simple_components, engine_ptr->sprite2d_batch_simple_components.key[i]);
		shader_detach(_render_batch_simple_shader);

		// Render all lines
		shader_use(_render_line_shader);
		for (int i = 0; i < engine_ptr->debug_line_components.size; i++)
		{
			render_system_update_debug_line(&engine_ptr->debug_line_components, engine_ptr->debug_line_components.key[i]);
			render_system_render_debug_line(&engine_ptr->debug_line_components, engine_ptr->debug_line_components.key[i]);
		}
		shader_detach(_render_line_shader);

		// Render all entities with components of type textured_sprite
		shader_use(_render_default_tex_shader);
		for (int i = 0; i < engine_ptr->textured_sprite2d_components.size; i++)
			render_system_render_textured_sprite2d(&engine_ptr->transform_components, &engine_ptr->textured_sprite2d_components, engine_ptr->textured_sprite2d_components.key[i]);
		shader_detach(_render_default_tex_shader);

		// Render all entities with components of type sprite2d
		shader_use(_render_default_shader);
		for (int i = 0; i < engine_ptr->sprite2d_components.size; i++)
			render_system_render_sprite2d(&engine_ptr->transform_components, &engine_ptr->sprite2d_components, engine_ptr->sprite2d_components.key[i]);
		shader_detach(_render_default_shader);

		// Exit on escape
		if (input_get_key_down(GLFW_KEY_ESCAPE))
			break;
		
		// Some end of frame stuff (input, swapping buffers, resetting delta_time
		input_reset_key_pressed_array();
		input_reset_mouse_pressed_array();
		window_end_frame(&engine_ptr->engine_window);

		// Calculating delta_time
		engine_time_end_frame(engine_ptr, &start_time);

		// Calculating fps 
		if (current_time - prev_time >= 1.0f)
		{
			if (engine_ptr->config & ENGINE_PRINT_FPS)
				logger_log_float(LOG, engine_get_fps());

			engine_fps = 0;
			prev_time = current_time;
		}

		engine_ptr->ticks++; 

		// Runs every 20 frames 
		if (engine_ptr->ticks >= TICK_TIME)
		{
			engine_ptr->app_on_tick_func();
			engine_ptr->ticks = 0;
		}
	}
}

void engine_time_end_frame(struct engine* engine_ptr, float* start_time)
{
	engine_ptr->delta_time = engine_get_mills() - *start_time;
	*start_time = engine_get_mills();
}

int engine_end(struct engine* engine_ptr)
{
	// Free all memory allocated by engine
	logger_log_string(WARNING, "Closing engine...\n");
	window_free_memory(&engine_ptr->engine_window);

	shader_free_memory(_render_default_shader);
	shader_free_memory(_render_line_shader);
	shader_free_memory(_render_default_tex_shader);
	shader_free_memory(_render_batch_simple_shader);

	free(_render_default_shader);
	free(_render_default_tex_shader);
	free(_render_line_shader);
	free(_render_batch_simple_shader);

	transform_hashmap_free(&engine_ptr->transform_components);
	sprite2d_hashmap_free(&engine_ptr->sprite2d_components);
	textured_sprite2d_hashmap_free(&engine_ptr->textured_sprite2d_components);
	sprite2d_batch_simple_hashmap_free(&engine_ptr->sprite2d_batch_simple_components);
	debug_line_hashmap_free(&engine_ptr->debug_line_components);

	engine_ptr->app_end_func();
		
	return 1;
}

entity engine_create_entity(struct engine* engine, unsigned int components)
{
	if (engine->entity_num >= ENGINE_MAX_ENTITIES)
	{	
		logger_log_string(ERROR, "Entity max number reached!\n");
		return 0;
	}

	int next_available_index = 0;

	// Gets next available index
	for (int i = 0; i < ENGINE_MAX_ENTITIES; i++)
	{
		if (engine->entities[i] == 0)
		{
			next_available_index = i;
			break;
		}
	}

	entity ent = next_available_index + 1;
	engine->entities[next_available_index] = ent;

	if (!(engine->config & ENGINE_SILENCE_ENTITY_LOG))
	{
		logger_log_string(LOG, "Creating entity with id: ");
		printf("%d\n", ent);
	}

	if (components & TRANSFORM)
	{
		if (!(engine->config & ENGINE_SILENCE_ENTITY_LOG)) printf("	Attaching transform\n");
		transform_hashmap_add(&engine->transform_components, ent);
		struct transform* t = ENTITY_GET_TRANSFORM(ent);
		*t = (struct transform){0};
	}
	if (components & SPRITE2D)
	{
		if (!(engine->config & ENGINE_SILENCE_ENTITY_LOG)) printf("	Attaching sprite2d\n");
		sprite2d_hashmap_add(&engine->sprite2d_components, ent);
		struct sprite2d* sprite = ENTITY_GET_SPRITE2D(ent);
		*sprite = (struct sprite2d){0};
		sprite->config = SPRITE2D_CENTERED;
	}
	if (components & TEXTURED_SPRITE2D)
	{
		if (!(engine->config & ENGINE_SILENCE_ENTITY_LOG)) printf("	Attaching textured_sprite2d\n");
		textured_sprite2d_hashmap_add(&engine->textured_sprite2d_components, ent);
		struct textured_sprite2d* sprite = ENTITY_GET_TEXTURED_SPRITE2D(ent);
		*sprite = (struct textured_sprite2d){0};
		sprite->config = SPRITE2D_CENTERED;
	}
	if (components & SPRITE2D_BATCH_SIMPLE)
	{
		if (!(engine->config & ENGINE_SILENCE_ENTITY_LOG)) printf(" Attaching sprite2d_batch_simple\n");
		sprite2d_batch_simple_hashmap_add(&engine->sprite2d_batch_simple_components, ent);
		struct sprite2d_batch_simple* sprite = ENTITY_GET_SPRITE2D_BATCH_SIMPLE(ent);
		*sprite = (struct sprite2d_batch_simple){0};
	}
	if (components & DEBUG_LINE)
	{
		if (!(engine->config & ENGINE_SILENCE_ENTITY_LOG)) printf(" Attaching debug_line\n");
		debug_line_hashmap_add(&engine->debug_line_components, ent);
		struct debug_line* line = ENTITY_GET_DEBUG_LINE(ent);
		*line = (struct debug_line){0};
	}

	engine->entity_num++;

	return next_available_index + 1;
}

void engine_pop_entity(struct engine *engine, entity e)
{
	unsigned char components = 0;

	// Our entities start with id 1 so the index is 0 if its the first entity
	int entity_index = e > 1 ? e - 1 : 0;

	if (engine->entities[entity_index] == 0) return;

	// Goes through all entities looking for the entity we are trying to remove
	if (engine->entities[entity_index] != e) return;

	if (!(engine->config & ENGINE_SILENCE_ENTITY_LOG))
	{
		logger_log_string(WARNING, "Popping entity with id: ");
		printf("%d\n", e);
	}

	// We search for the components that our entity had
	for (int i = 0; i < engine->transform_components.size; i++)
	{
		if (engine->transform_components.key[i] == e)
		{
			components |= TRANSFORM;
			break;
		}
	}
	for (int i = 0; i < engine->sprite2d_components.size; i++)
	{
		if (engine->sprite2d_components.key[i] == e)
		{
			components |= SPRITE2D;
			break;
		}
	}
	for (int i = 0; i < engine->textured_sprite2d_components.size; i++)
	{
		if (engine->textured_sprite2d_components.key[i] == e)
		{
			components |= TEXTURED_SPRITE2D;
			break;
		}
	}
	for (int i = 0; i < engine->sprite2d_batch_simple_components.size; i++)
	{
		if (engine->sprite2d_batch_simple_components.key[i] == e)
		{
			components |= SPRITE2D_BATCH_SIMPLE;
			break;
		}
	}
	for (int i = 0; i < engine->debug_line_components.size; i++)
	{
		if (engine->debug_line_components.key[i] == e)
		{
			components |= DEBUG_LINE;
			break;
		}	
	}

	// We remove all the components found
	if (components & TRANSFORM)
		transform_hashmap_pop(&engine->transform_components, e);
	if (components & SPRITE2D)
		sprite2d_hashmap_pop(&engine->sprite2d_components, e);
	if (components & TEXTURED_SPRITE2D)
		textured_sprite2d_hashmap_pop(&engine->textured_sprite2d_components, e);
	if (components & SPRITE2D_BATCH_SIMPLE)
		sprite2d_batch_simple_hashmap_pop(&engine->sprite2d_batch_simple_components, e);
	if (components & DEBUG_LINE)
		debug_line_hashmap_pop(&engine->debug_line_components, e);

	engine->entities[entity_index] = 0;
	engine->entity_num--;
}

void* engine_get_entity_component(struct engine* engine, entity ent, unsigned int component)
{
	if (ENTITY_ARRAY[ent > 0 ? ent - 1 : 0] == 0) 
	{
		logger_log_string(ERROR, "Trying to get component of non-existent entity\n");
		return NULL;
	}

	// Return a void pointer that is to be casted to the specific type
	if (component & TRANSFORM)
		return (void*)transform_hashmap_get(&engine->transform_components, ent);
	else if (component & SPRITE2D)
		return (void*)sprite2d_hashmap_get(&engine->sprite2d_components, ent);
	else if (component & TEXTURED_SPRITE2D)
		return (void*)textured_sprite2d_hashmap_get(&engine->textured_sprite2d_components, ent);
	else if (component & SPRITE2D_BATCH_SIMPLE)
		return (void*)sprite2d_batch_simple_hashmap_get(&engine->sprite2d_batch_simple_components, ent);
	else if (component & DEBUG_LINE)
		return (void*)debug_line_hashmap_get(&engine->debug_line_components, ent);
	
	return NULL;
}

int engine_has_entity(struct engine* engine, entity e)
{
	for (int i = 0; i < ENGINE_MAX_ENTITIES; i++)
		if (engine->entities[i] == e) 
			return 1; 

	return 0;
}

double engine_get_mills(void)
{ return glfwGetTime(); }

int engine_get_fps(void)
{ return engine_fps; }

int frame_timer_add_and_respond(struct frame_timer* timer)
{
	if (timer->frames < timer->target)
	{
		timer->frames++;
		return 0;
	}
	else 
		timer->frames = 0;

	return 1;
}

int timer_add_and_respond(struct timer* timer)
{
	if (timer->timer < timer->target)
	{
		timer->timer += global_engine->delta_time * 1000;
		return 0;
	}
	else 
		timer->timer = 0;

	return 1;
}
