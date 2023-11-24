#ifndef ECS_H
#define ECS_H

#include "../entity.h"
#include "../entity_hashmap/transform_hashmap.h"
#include "../entity_hashmap/sprite2d_hashmap.h"
#include "../entity_hashmap/textured_sprite2d_hashmap.h"
#include "../entity_hashmap/sprite2d_batch_simple.h"
#include "../entity_hashmap/sprite2d_batch_complex.h"
#include "../entity_hashmap/script_hashmap.h"
#include "../entity_hashmap/debug_line_hashmap.h"

#include "../../render/render.h"
#include "../../script_system/script_system.h"

#define ENGINE_MAX_ENTITIES 10000

// Some helper macros to make fetching components easier
#define ENTITY_GET_TRANSFORM(x) (struct transform*)(engine_get_entity_component(global_engine->ecs, x, TRANSFORM))
#define ENTITY_GET_SPRITE2D(x) (struct sprite2d*)(engine_get_entity_component(global_engine->ecs, x, SPRITE2D))
#define ENTITY_GET_TEXTURED_SPRITE2D(x) (struct textured_sprite2d*)(engine_get_entity_component(global_engine->ecs, x, TEXTURED_SPRITE2D))
#define ENTITY_GET_SPRITE2D_BATCH_SIMPLE(x) (struct sprite2d_batch_simple*)(engine_get_entity_component(global_engine->ecs, x, SPRITE2D_BATCH_SIMPLE))
#define ENTITY_GET_DEBUG_LINE(x) (struct debug_line*)(engine_get_entity_component(global_engine->ecs, x, DEBUG_LINE));
#define ENTITY_GET_SCRIPT(x) (struct script*)(engine_get_entity_component(global_engine->ecs, x, SCRIPT));
#define ENTITY_GET_SPRITE2D_BATCH_COMPLEX(x) (struct sprite2d_batch_complex*)(engine_get_entity_component(global_engine->ecs, x, SPRITE2D_BATCH_COMPLEX))

#define ENTITY_FOR_LOOP(x) for (int x = 0; x < ENGINE_MAX_ENTITIES; x++)
#define ENTITY_ARRAY global_engine->ecs->entities

#define ENGINE_SILENCE_ENTITY_LOG 2

struct ecs 
{
	entity entities[ENGINE_MAX_ENTITIES];

    struct transform_hashmap transform_components;
    struct sprite2d_hashmap sprite2d_components;
    struct textured_sprite2d_hashmap textured_sprite2d_components;
    struct sprite2d_batch_simple_hashmap sprite2d_batch_simple_components;
    struct debug_line_hashmap debug_line_components;
    struct sprite2d_batch_complex_hashmap sprite2d_batch_complex_components;
    struct script_hashmap script_components;

    uint8_t config;

    uint16_t entity_num;
};

// Initialization
struct ecs* engine_init_ecs();
void engine_init_ecs_components(struct ecs* ecs);

// Update 
void engine_update_entities(struct ecs* ecs);

// Utility functions
entity engine_create_entity(struct ecs* ecs, uint32_t components);
void engine_pop_entity(struct ecs* ecs, entity e);
void* engine_get_entity_component(struct ecs* ecs, entity ent, uint32_t component);
int8_t engine_has_entity(struct ecs* ecs, entity e);

// Finalize
void engine_end_ecs(struct ecs* ecs);

#endif

