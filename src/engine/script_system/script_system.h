#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include "../entity/entity.h"
#include "../entity/entity_hashmap/script_hashmap.h"

void script_system_start(struct script_hashmap* script_map, entity e);
void script_system_update(struct script_hashmap* script_map, entity e);
void script_system_set_data(struct script* script, void* data);
void script_system_set_funcs(struct script* script, void (*start_func)(entity), void(*update_func)(entity));

#endif

