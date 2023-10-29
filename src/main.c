#include "app/app.h"
#include "engine/core/engine.h"
#include "engine/utils/color.h"

int main(void)
{
	struct engine app_engine;

	engine_app_start_callback_func(&app_engine, app_start);
	engine_app_update_callback_func(&app_engine, app_update);
	engine_app_tick_update_callback_func(&app_engine, app_on_tick);
	engine_app_end_callback_func(&app_engine, app_end);

	app_engine.engine_window = (struct window)
	{
		.width = 1440, 
		.height = 768,
		.title = "Katana Engine (dev 0.0)",
		.color = {RGBA_SKY_BLUE}
	};

	if (engine_init(&app_engine))
		logger_log_string(SUCCESS, "Engine init was sucessful.\n");

	engine_update(&app_engine);

	if (engine_end(&app_engine))
		logger_log_string(SUCCESS, "Engine exit was successful.\n");

	return 0;
}

