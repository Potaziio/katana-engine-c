#include "engine/core/engine.h"

#include "engine/input/input.h"
#include "engine/math/vector/vector2.h"
#include "engine/render/font.h"
#include "engine/utils/color.h"
#include "engine/utils/logger.h"
#include "engine/render/line.h"
#include "flappy_bird/flappy.h"

/*
#include "midas_curse/map/chunk.h"
#include "midas_curse/map/map.h"
#include "midas_curse/curse.h"
#include "midas_curse/player/player.h"
*/

int main(void)
{
	struct engine app_engine;

	engine_app_start_callback_func(&app_engine, flappy_start);
	engine_app_update_callback_func(&app_engine, flappy_update);
	engine_app_tick_update_callback_func(&app_engine, NULL);
	engine_app_end_callback_func(&app_engine, flappy_end);
	engine_app_preload_callback_func(&app_engine, NULL);

	app_engine.engine_window = (struct window)
	{
		.width = 1440, 
		.height = 768,
		.title = "Katana Engine (dev 0.0)",
		.color = {RGBA_BLACK},
	};

	if (engine_init(&app_engine))
		logger_log_string(SUCCESS, "Engine init was sucessful.\n");

	engine_update(&app_engine);

	if (engine_end(&app_engine))
		logger_log_string(SUCCESS, "Engine exit was successful.\n");

	return 0;
}

