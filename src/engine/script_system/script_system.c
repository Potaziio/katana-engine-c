#include "script_system.h"

void script_system_start(struct script_hashmap* script_map, entity e)
{
	struct script* script = script_hashmap_get(script_map, e);

	if (script == NULL) 
	{
		logger_log_string(ERROR, "Script: bad access\n");
		return;
	}

	if (script->start != NULL)
		script->start(e);
}

void script_system_update(struct script_hashmap* script_map, entity e)
{
	struct script* script = script_hashmap_get(script_map, e);

	if (script == NULL)
	{
		logger_log_string(ERROR, "Script: bad access\n");
		return;
	}

	if (script->update != NULL)
		script->update(e);
}

void script_system_set_funcs(struct script* script, void (*start_func)(entity), void(*update_func)(entity))
{
	script->start = start_func;
	script->update = update_func;
}

void script_system_set_data(struct script* script, void* data)
{ script->data = data; }

