#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>

#include "../window/window.h"
#include "../input/input.h"
#include "../shader/shader.h"
#include "../render/render.h"
#include "../render/font.h"
#include "../script_system/script_system.h"
#include "../camera/camera.h"
#include "../math/physics/physics.h"
#include "../entity/entity.h"
#include "../audio/audio_manager.h"
#include "../entity/ecs/ecs.h"
#include "../utils/time.h"

#include "../../../include/c-utils/c_utils.h"

#define GL_SILENCE_DEPRECATION

// Engine general macros
#define TICK_TIME 20
#define ENGINE_WINDOW_CENTER engine_camera.bounds.x * 0.5f, engine_camera.bounds.y * 0.5f
#define ENGINE_MIN_DELTA_MS 50.0

// Engine config 
#define ENGINE_PRINT_FPS 1
#define ENGINE_UPDATE_CAMERA_BOUNDS 4

extern struct engine* global_engine;
extern struct camera engine_camera;
extern int engine_fps_counter;

struct engine
{
	// Engine is only going to support one window so we make it static 

	struct window engine_window;
	double delta_time;

	// ECS -------------------------

	struct ecs* ecs;

	int32_t ticks;
	int32_t fps;

	uint8_t config;

	void (*app_start_func)(void); // Called after initializing all engine systems, before initializing ecs systems
	void (*app_update_func)(void); // Called on each frame
	void (*app_end_func)(void); // Called before engine quits
	void (*app_on_tick_func)(void); // Called every 'TICK_TIME' frames
	void (*app_engine_preload_func)(void); // Called after loading everything including ecs systems, pre update loop
};

// ENGINE LOGIC FUNCTIONS 
int8_t engine_init(struct engine* engine_ptr);
void engine_update(struct engine* engine_ptr);
int8_t engine_end(struct engine* engine_ptr);

// CALLBACKS
// Add functions that you want to run on the start and update of the engine
void engine_app_start_callback_func(struct engine* engine, void (*func)(void));
void engine_app_update_callback_func(struct engine* engine, void (*func)(void));
void engine_app_end_callback_func(struct engine* engine, void (*func)(void));
void engine_app_tick_update_callback_func(struct engine* engine, void (*func)(void));
void engine_app_preload_callback_func(struct engine* engine, void (*func)(void));

// TIME FUNCTIONS
void engine_time_end_frame(struct engine* engine_ptr, double* start_time);
double engine_get_mills(void);

#endif

