#include "engine.h"
#include <stddef.h>

// TODO: Separate this by files (Render, ECS)
// TODO: Fix scripts n shit

struct engine* global_engine;
struct camera engine_camera;
int32_t engine_fps_counter;

void engine_app_start_callback_func(struct engine* engine, void (*func)(void))
{ engine->app_start_func = func; }

void engine_app_update_callback_func(struct engine* engine, void (*func)(void))
{ engine->app_update_func = func; }

void engine_app_end_callback_func(struct engine* engine, void (*func)(void))
{ engine->app_end_func = func; }

void engine_app_tick_update_callback_func(struct engine *engine, void (*func)(void))
{ engine->app_on_tick_func = func; } 

void engine_app_preload_callback_func(struct engine* engine, void (*func)(void))
{ engine->app_engine_preload_func = func; }

int8_t engine_init(struct engine* engine_ptr)
{	
	logger_log_string(LOG, "Creating window...\n");
	int status = window_create(&engine_ptr->engine_window);

	if (!status)
		return 0;

	engine_ptr->delta_time = 0.0f;

	engine_camera.position = vector2_zero();
	engine_camera.type = CAMERA_ORTHOGRAPHIC;

	engine_camera.bounds = (struct vector2){engine_ptr->engine_window.width, engine_ptr->engine_window.height};

	_engine_default_font = (struct font*) malloc(sizeof(struct font));

	// Allocate memory for global shaders
	_render_default_shader = (struct shader*) malloc(sizeof(struct shader));
	_render_default_tex_shader = (struct shader*) malloc(sizeof(struct shader));
	_render_line_shader = (struct shader*) malloc(sizeof(struct shader));
	_render_batch_simple_shader = (struct shader*) malloc(sizeof(struct shader));
	_render_font_shader = (struct shader*) malloc(sizeof(struct shader));

	_render_font_default_texture = (struct texture*) malloc(sizeof(struct texture));

	_render_batch_complex_shader = (struct shader*) malloc(sizeof(struct shader));

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

	shader_load_and_compile(_render_batch_complex_shader, 
			"../src/engine/assets/shaders/default_batch_complex_vertex.glsl",
			"../src/engine/assets/shaders/default_batch_complex_fragment.glsl");

	texture_load(_render_font_default_texture, "../src/engine/assets/fonts/Nes/Nes.png");

	shader_load_and_compile(_render_font_shader, "../src/engine/assets/shaders/default_font_vertex.glsl", "../src/engine/assets/shaders/default_font_fragment.glsl");

	_engine_default_font->bitmap = _render_font_default_texture;
	_engine_default_font->char_padding = 0;
	_engine_default_font->image_width = texture_get_width(*_render_font_default_texture);
	_engine_default_font->image_height = texture_get_height(*_render_font_default_texture);
	ui_font_get_fnt_data(_engine_default_font, "../src/engine/assets/fonts/Nes/Nes.fnt");

	global_engine = engine_ptr;

	init_random(time(NULL));

	// We create allocate the map for each type

	logger_log_string(LOG, "Creating audio manager\n");
	_engine_default_audio_manager = audio_manager_create();	

	if (engine_ptr->app_start_func == NULL)
	{
		logger_log_string(ERROR, "No app start function set, quitting engine\n");
		return -1; 
	}
	
	// Allocates memory for our ecs components and our ecs
	global_engine->ecs = engine_init_ecs();

	// Run this before returning to make sure everything is allocated and initialized
	engine_ptr->app_start_func();

	// Initializes our ecs components
	engine_init_ecs_components(global_engine->ecs);

	if (engine_ptr->app_engine_preload_func != NULL)
		engine_ptr->app_engine_preload_func();

	return status;
}

void engine_update(struct engine* engine_ptr)
{
	// TODO: Separate into files
	
	double start_time = engine_get_mills();

	// for fps calculations
	double prev_time = engine_get_mills();

	while (!window_should_close(&engine_ptr->engine_window))
	{
		if (input_get_key(GLFW_KEY_L))
			engine_ptr->delta_time *= 0.1f;

		window_begin_frame(&engine_ptr->engine_window);

		// We leave this at the beggining of the loop
		float current_time = engine_get_mills();

		engine_fps_counter++;

		engine_init_ecs_components(global_engine->ecs);	

		// We update the camera bounds to the screen dimensions here

		if (engine_ptr->config & ENGINE_UPDATE_CAMERA_BOUNDS)
			engine_camera.bounds = (struct vector2){engine_ptr->engine_window.width, engine_ptr->engine_window.height};

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

		shader_use(_render_font_shader);
		camera_send_matrices_to_shader(&engine_camera, _render_font_shader, "projection", "view");
		shader_detach(_render_font_shader);

		shader_use(_render_batch_complex_shader);
		camera_send_matrices_to_shader(&engine_camera, _render_batch_complex_shader, "projection", "view");
		shader_detach(_render_batch_complex_shader);

		shader_detach(_render_line_shader);

		// Render here


		if (engine_ptr->app_update_func != NULL)
			engine_ptr->app_update_func();
		else 
			logger_log_string(ERROR, "No app update function set\n");

		engine_update_entities(global_engine->ecs);

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
				logger_log_float(LOG, engine_fps_counter);

			engine_ptr->fps = engine_fps_counter;

			engine_fps_counter = 0;
			prev_time = current_time;
		}

		engine_ptr->ticks++; 

		// Runs every 20 frames 
		if (engine_ptr->ticks >= TICK_TIME)
		{
			if (engine_ptr->app_on_tick_func != NULL)
				engine_ptr->app_on_tick_func();

			engine_ptr->ticks = 0;
		}
	}
	
}


// TODO: Deltatime: this works but its not the best (or flexible) way in terms of compatibility with other devices

void engine_time_end_frame(struct engine* engine_ptr, double* start_time)
{
	/* engine_ptr->delta_time = 1 / 75.0f; */

	double delta = engine_get_mills() - *start_time;
	
	if (delta > ENGINE_MIN_DELTA_MS / 1000.0)
	{
		logger_log_string(LOG, "Halt\n");
		delta = 0;
	}

	engine_ptr->delta_time = delta;

	*start_time = engine_get_mills();
}

int8_t engine_end(struct engine* engine_ptr)
{
	// Free all memory allocated by engine
	
	logger_log_string(WARNING, "Closing engine...\n");

	window_free_memory(&engine_ptr->engine_window);

	shader_free_memory(_render_default_shader);
	shader_free_memory(_render_line_shader);
	shader_free_memory(_render_default_tex_shader);
	shader_free_memory(_render_batch_simple_shader);
	shader_free_memory(_render_font_shader);

	free(_render_default_shader);
	free(_render_default_tex_shader);
	free(_render_line_shader);
	free(_render_batch_simple_shader);
	free(_render_font_shader);

	texture_free(_render_font_default_texture);
	free(_engine_default_font);

	free(_engine_default_audio_manager->engine);
	free(_engine_default_audio_manager);

	engine_end_ecs(global_engine->ecs);

	if (engine_ptr->app_end_func != NULL)
		engine_ptr->app_end_func();
		
	return 1;
}

double engine_get_mills(void)
{ return glfwGetTime(); }

