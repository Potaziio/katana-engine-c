#include "ecs.h"

struct ecs* engine_init_ecs()
{
	struct ecs* ecs = (struct ecs*)malloc(sizeof(struct ecs));

	transform_hashmap_create(&ecs->transform_components);
	sprite2d_hashmap_create(&ecs->sprite2d_components);
	textured_sprite2d_hashmap_create(&ecs->textured_sprite2d_components);
	sprite2d_batch_simple_hashmap_create(&ecs->sprite2d_batch_simple_components);
	debug_line_hashmap_create(&ecs->debug_line_components);
	script_hashmap_create(&ecs->script_components);
	sprite2d_batch_complex_hashmap_create(&ecs->sprite2d_batch_complex_components);

	memset(ecs->entities, 0, sizeof(entity) * ENGINE_MAX_ENTITIES);

	ecs->entity_num = 0;

	return ecs;
}

void engine_init_ecs_components(struct ecs *ecs)
{
	// Initialize all entities with sprites
	for (uint32_t i = 0; i < ecs->sprite2d_components.size - 1; i++)
		render_system_init_sprite2d(&ecs->sprite2d_components, ecs->sprite2d_components.key[i]);

	// Initialize all entities with textured sprites
	for (uint32_t i = 0; i < ecs->textured_sprite2d_components.size - 1; i++)
		render_system_init_textured_sprite2d(&ecs->textured_sprite2d_components, ecs->textured_sprite2d_components.key[i]);

	// Initialize all entities with spritebatch sprites
	for (uint32_t i = 0; i < ecs->sprite2d_batch_simple_components.size; i++)
		render_system_init_sprite2d_batch_simple(&ecs->sprite2d_batch_simple_components, ecs->sprite2d_batch_simple_components.key[i]);

	for (uint32_t i = 0; i < ecs->sprite2d_batch_complex_components.size; i++)
		render_system_init_sprite2d_batch_complex(&ecs->sprite2d_batch_complex_components, ecs->sprite2d_batch_complex_components.key[i]);

	for (uint32_t i = 0; i < ecs->debug_line_components.size; i++)
		render_system_init_debug_line(&ecs->debug_line_components, ecs->debug_line_components.key[i]);

	for (uint32_t i = 0; i < ecs->script_components.size - 1; i++)
		script_system_start(&ecs->script_components, ecs->script_components.key[i]);
}

void engine_update_entities(struct ecs* ecs)
{
	for (uint32_t i = 0; i < ecs->script_components.size - 1; i++)
		script_system_update(&ecs->script_components, ecs->script_components.key[i]);

	shader_use(_render_batch_simple_shader);
	for (uint32_t i = 0; i < ecs->sprite2d_batch_simple_components.size; i++)
		render_system_render_sprite2d_batch_simple(&ecs->sprite2d_batch_simple_components, ecs->sprite2d_batch_simple_components.key[i]);
	shader_detach(_render_batch_simple_shader);

	// Render all entities with components of type textured_sprite
	shader_use(_render_default_tex_shader);
	for (uint32_t i = 0; i < ecs->textured_sprite2d_components.size - 1; i++)
		render_system_render_textured_sprite2d(&ecs->transform_components, &ecs->textured_sprite2d_components, ecs->textured_sprite2d_components.key[i]);
	shader_detach(_render_default_tex_shader);

	// Render all entities with components of type sprite2d_batch_simple
	shader_use(_render_batch_complex_shader);
	for (uint32_t i = 0; i < ecs->sprite2d_batch_complex_components.size; i++)
		render_system_render_sprite2d_batch_complex(&ecs->sprite2d_batch_complex_components, ecs->sprite2d_batch_complex_components.key[i]);
	shader_detach(_render_batch_complex_shader);

	// Render all entities with components of type sprite2d
	shader_use(_render_default_shader);
	for (uint32_t i = 0; i < ecs->sprite2d_components.size - 1; i++)
		render_system_render_sprite2d(&ecs->transform_components, &ecs->sprite2d_components, ecs->sprite2d_components.key[i]);
	shader_detach(_render_default_shader);

	// Render all lines
	shader_use(_render_line_shader);
	for (uint32_t i = 0; i < ecs->debug_line_components.size; i++)
	{
		render_system_update_debug_line(&ecs->debug_line_components, ecs->debug_line_components.key[i]);
		render_system_render_debug_line(&ecs->debug_line_components, ecs->debug_line_components.key[i]);
	}
	shader_detach(_render_line_shader);
}

void engine_end_ecs(struct ecs* ecs)
{
	transform_hashmap_free(&ecs->transform_components);
	sprite2d_hashmap_free(&ecs->sprite2d_components);
	textured_sprite2d_hashmap_free(&ecs->textured_sprite2d_components);
	sprite2d_batch_simple_hashmap_free(&ecs->sprite2d_batch_simple_components);
	debug_line_hashmap_free(&ecs->debug_line_components);
	script_hashmap_free(&ecs->script_components);
	sprite2d_batch_complex_hashmap_free(&ecs->sprite2d_batch_complex_components);
}

entity engine_create_entity(struct ecs* ecs, uint32_t components)
{
	if (ecs->entity_num >= ENGINE_MAX_ENTITIES)
	{	
		logger_log_string(ERROR, "Entity max number reached!\n");
		return 0;
	}

	int next_available_index = 0;

	// Gets next available index
	for (int i = 0; i < ENGINE_MAX_ENTITIES; i++)
	{
		if (ecs->entities[i] == 0)
		{
			next_available_index = i;
			break;
		}
	}

	entity ent = next_available_index + 1;
	ecs->entities[next_available_index] = ent;

	if (!(ecs->config & ENGINE_SILENCE_ENTITY_LOG))
	{
		logger_log_string(LOG, "Creating entity with id: ");
		printf("%d\n", ent);
	}

	if (components & TRANSFORM)
	{
		if (!(ecs->config & ENGINE_SILENCE_ENTITY_LOG)) printf("	Attaching transform\n");
		transform_hashmap_add(&ecs->transform_components, ent);
	}
	if (components & SPRITE2D)
	{
		if (!(ecs->config & ENGINE_SILENCE_ENTITY_LOG)) printf("	Attaching sprite2d\n");
		sprite2d_hashmap_add(&ecs->sprite2d_components, ent);
		struct sprite2d* sprite = (struct sprite2d*)engine_get_entity_component(ecs, ent, SPRITE2D);
		sprite->config = SPRITE2D_CENTERED | SPRITE2D_ENABLED;
	}
	if (components & TEXTURED_SPRITE2D)
	{
		if (!(ecs->config & ENGINE_SILENCE_ENTITY_LOG)) printf("	Attaching textured_sprite2d\n");
		textured_sprite2d_hashmap_add(&ecs->textured_sprite2d_components, ent);
		struct textured_sprite2d* sprite = (struct textured_sprite2d*)engine_get_entity_component(ecs, ent, TEXTURED_SPRITE2D);
		sprite->config = SPRITE2D_CENTERED;
	}
	if (components & SPRITE2D_BATCH_SIMPLE)
	{
		if (!(ecs->config & ENGINE_SILENCE_ENTITY_LOG)) printf(" Attaching sprite2d_batch_simple\n");
		sprite2d_batch_simple_hashmap_add(&ecs->sprite2d_batch_simple_components, ent);
	}
	if (components & DEBUG_LINE)
	{
		if (!(ecs->config & ENGINE_SILENCE_ENTITY_LOG)) printf(" Attaching debug_line\n");
		debug_line_hashmap_add(&ecs->debug_line_components, ent);
	}
	if (components & SCRIPT)
	{
		if (!(ecs->config & ENGINE_SILENCE_ENTITY_LOG)) printf("	Attaching script\n");
		script_hashmap_add(&ecs->script_components, ent);
	}
	if (components & SPRITE2D_BATCH_COMPLEX)
	{
		if (!(ecs->config & ENGINE_SILENCE_ENTITY_LOG)) printf("	Attaching sprite2d_batch_complex\n");
		sprite2d_batch_complex_hashmap_add(&ecs->sprite2d_batch_complex_components, ent);
	}

	ecs->entity_num++;

	return ent;
}

void* engine_get_entity_component(struct ecs* ecs, entity ent, uint32_t component)
{
	// Check entity array at index, to check if entity exists
	// Since our arrays are 0 indexed and our entity ids start at 1 we substract one if our entity id is more than
	if (ecs->entities[ent - 1] == 0) 
	{
		logger_log_string(ERROR, "Trying to get component of non-existent entity\n");
		return NULL;
	}

	if (component & TRANSFORM)
		return (void*)transform_hashmap_get(&ecs->transform_components, ent);
	else if (component & SPRITE2D)
		return (void*)sprite2d_hashmap_get(&ecs->sprite2d_components, ent);
	else if (component & SPRITE2D_BATCH_SIMPLE)
		return (void*)sprite2d_batch_simple_hashmap_get(&ecs->sprite2d_batch_simple_components, ent);
	else if (component & TEXTURED_SPRITE2D)
		return (void*)textured_sprite2d_hashmap_get(&ecs->textured_sprite2d_components, ent);
	else if (component & DEBUG_LINE)
		return (void*)debug_line_hashmap_get(&ecs->debug_line_components, ent);
	else if (component & SCRIPT)
		return (void*)script_hashmap_get(&ecs->script_components, ent);
	else if (component & SPRITE2D_BATCH_COMPLEX)
		return (void*)sprite2d_batch_complex_hashmap_get(&ecs->sprite2d_batch_complex_components, ent);

	logger_log_string(ERROR, "Entity has no component of that type");
	printf("- Entity: %d\n", ent);

	return NULL;
}

void engine_pop_entity(struct ecs* ecs, entity e)
{
	unsigned char components = 0;

	// Our entities start with id 1 so the index is 0 if its the first entity
	int entity_index = e > 1 ? e - 1 : 0;

	if (ecs->entities[entity_index] == 0 || ecs->entities[entity_index] != e) return;

	if (!(ecs->config & ENGINE_SILENCE_ENTITY_LOG))
	{
		logger_log_string(WARNING, "Popping entity with id: ");
		printf("%d\n", e);
	}

	// We search for the components that our entity had
	for (uint32_t i = 0; i < ecs->transform_components.size - 1; i++)
	{
		if (ecs->transform_components.key[i] == e)
		{
			components |= TRANSFORM;
			break;
		}
	}
	for (uint32_t i = 0; i < ecs->sprite2d_components.size - 1; i++)
	{
		if (ecs->sprite2d_components.key[i] == e)
		{
			components |= SPRITE2D;
			break;
		}
	}
	for (uint32_t i = 0; i < ecs->textured_sprite2d_components.size - 1; i++)
	{
		if (ecs->textured_sprite2d_components.key[i] == e)
		{
			components |= TEXTURED_SPRITE2D;
			break;
		}
	}
	for (uint32_t i = 0; i < ecs->sprite2d_batch_simple_components.size; i++)
	{
		if (ecs->sprite2d_batch_simple_components.key[i] == e)
		{
			components |= SPRITE2D_BATCH_SIMPLE;
			break;
		}
	}
	for (uint32_t i = 0; i < ecs->debug_line_components.size; i++)
	{
		if (ecs->debug_line_components.key[i] == e)
		{
			components |= DEBUG_LINE;
			break;
		}	
	}
	for (uint32_t i = 0; i < ecs->script_components.size - 1; i++)
	{
		if (ecs->script_components.key[i] == e)
		{
			components |= SCRIPT;
			break;
		}
	}
	for (uint32_t i = 0; i < ecs->sprite2d_batch_complex_components.size; i++)
	{
		if (ecs->sprite2d_batch_complex_components.key[i] == e)
		{
			components |= SPRITE2D_BATCH_COMPLEX;
			break;
		}
	}

	// We remove all the components found
	if (components & TRANSFORM)
		transform_hashmap_pop(&ecs->transform_components, e);
	if (components & SPRITE2D)
		sprite2d_hashmap_pop(&ecs->sprite2d_components, e);
	if (components & TEXTURED_SPRITE2D)
		textured_sprite2d_hashmap_pop(&ecs->textured_sprite2d_components, e);
	if (components & SPRITE2D_BATCH_SIMPLE)
		sprite2d_batch_simple_hashmap_pop(&ecs->sprite2d_batch_simple_components, e);
	if (components & DEBUG_LINE)
		debug_line_hashmap_pop(&ecs->debug_line_components, e);
	if (components & SCRIPT)
		script_hashmap_pop(&ecs->script_components, e);
	if (components & SPRITE2D_BATCH_COMPLEX)
		sprite2d_batch_complex_hashmap_pop(&ecs->sprite2d_batch_complex_components, e);

	ecs->entities[entity_index] = 0;
	ecs->entity_num--;
}

int8_t engine_has_entity(struct ecs* ecs, entity e)
{
	for (int i = 0; i < ENGINE_MAX_ENTITIES; i++)
		if (ecs->entities[i] == e) 
			return 1; 

	return 0;
}

