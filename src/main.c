#include "engine/core/engine.h"
#include "atari_combat/combat.h"

int main(void)
{
	struct engine app_engine;

	engine_app_start_callback_func(&app_engine, combat_start);
	engine_app_update_callback_func(&app_engine, combat_update);
	engine_app_end_callback_func(&app_engine, combat_end);
	engine_app_tick_update_callback_func(&app_engine, NULL);
	engine_app_preload_callback_func(&app_engine, NULL);

	app_engine.engine_window = (struct window)
	{
		.width = 800, 
		.height = 600,
		.title = "Katana Engine (dev 0.0)",
		.color = {170, 232, 53, 255},
	};

	if (engine_init(&app_engine))
		logger_log_string(SUCCESS, "Engine init was sucessful.\n");

	engine_update(&app_engine);

	if (engine_end(&app_engine))
		logger_log_string(SUCCESS, "Engine exit was successful.\n");

	return 0;
}

