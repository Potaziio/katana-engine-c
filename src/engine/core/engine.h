#ifndef ENGINE_H
#define ENGINE_H

#include "../window/window.h"
#include "../input/input.h"
#include "../shader/shader.h"
#include "../render/render.h"
#include "../camera/camera.h"
#include "../../../include/c-utils/c_utils.h"
#include "../math/physics/physics.h"
#include "../entity/entity.h"

#include "../entity/entity_hashmap/transform_hashmap.h"
#include "../entity/entity_hashmap/sprite2d_hashmap.h"
#include "../entity/entity_hashmap/textured_sprite2d_hashmap.h"
#include "../entity/entity_hashmap/sprite2d_batch_simple.h"
#include "../entity/entity_hashmap/debug_line_hashmap.h"

#define GL_SILENCE_DEPRECATION

// Engine general macros
#define ENGINE_MAX_ENTITIES 5000
#define TICK_TIME 10
#define ENGINE_WINDOW_CENTER engine_camera.bounds.x * 0.5f, engine_camera.bounds.y * 0.5f

// Engine config 
#define ENGINE_PRINT_FPS 1
#define ENGINE_SILENCE_ENTITY_LOG 2

// Some helper macros to make fetching components easier
#define ENTITY_GET_TRANSFORM(x) (struct transform*)(engine_get_entity_component(global_engine, x, TRANSFORM))
#define ENTITY_GET_SPRITE2D(x) (struct sprite2d*)(engine_get_entity_component(global_engine, x, SPRITE2D))
#define ENTITY_GET_TEXTURED_SPRITE2D(x) (struct textured_sprite2d*)(engine_get_entity_component(global_engine, x, TEXTURED_SPRITE2D))
#define ENTITY_GET_SPRITE2D_BATCH_SIMPLE(x) (struct sprite2d_batch_simple*)(engine_get_entity_component(global_engine, x, SPRITE2D_BATCH_SIMPLE))
#define ENTITY_GET_DEBUG_LINE(x) (struct debug_line*)(engine_get_entity_component(global_engine, x, DEBUG_LINE));

#define ENTITY_FOR_LOOP(x) for (int x = 0; x < ENGINE_MAX_ENTITIES; x++)
#define ENTITY_ARRAY global_engine->entities

extern struct engine* global_engine;
extern struct camera engine_camera;
extern int engine_fps;

struct frame_timer 
{
	int frames;
	int target;
};

struct timer 
{
	double target;
	double timer;
};

struct engine
{
	// Engine is only going to support one window so we make it static 

	struct window engine_window;
	float delta_time;

	entity entities[ENGINE_MAX_ENTITIES];

	// We have a map for each component 
	struct transform_hashmap transform_components;
	struct sprite2d_hashmap sprite2d_components;
	struct textured_sprite2d_hashmap textured_sprite2d_components;
	struct sprite2d_batch_simple_hashmap sprite2d_batch_simple_components;
	struct debug_line_hashmap debug_line_components;

	int ticks;

	int entity_num;

	unsigned char config;

	void (*app_start_func)(void);
	void (*app_update_func)(void);
	void (*app_end_func)(void);
	void (*app_on_tick_func)(void);
};

// ENGINE LOGIC FUNCTIONS 
int engine_init(struct engine* engine_ptr);
void engine_update(struct engine* engine_ptr);
int engine_end(struct engine* engine_ptr);

// ECS
entity engine_create_entity(struct engine* engine, unsigned int components);
void engine_pop_entity(struct engine* engine, entity e);
void* engine_get_entity_component(struct engine* engine, entity ent, unsigned int component);
int engine_has_entity(struct engine* engine, entity e);

// CALLBACKS
// Add functions that you want to run on the start and update of the engine
void engine_app_start_callback_func(struct engine* engine, void (*func)(void));
void engine_app_update_callback_func(struct engine* engine, void (*func)(void));
void engine_app_end_callback_func(struct engine* engine, void (*func)(void));
void engine_app_tick_update_callback_func(struct engine* engine, void (*func)(void));

// TIME FUNCTIONS
void engine_time_end_frame(struct engine* engine_ptr, float* start_time);
double engine_get_mills(void);
int engine_get_fps(void);

int frame_timer_add_and_respond(struct frame_timer* timer);
int timer_add_and_respond(struct timer* timer);

#endif

