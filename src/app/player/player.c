#include "player.h"


void player_create(struct player* player)
{
	player->id = engine_create_entity(global_engine, TRANSFORM | TEXTURED_SPRITE2D | SCRIPT);
	player->transform = ENTITY_GET_TRANSFORM(player->id);
	player->sprite = ENTITY_GET_TEXTURED_SPRITE2D(player->id);
	player->script = ENTITY_GET_SCRIPT(player->id);

	player->transform->scale = (struct vector3){64.0f, 64.0f, 0.0f};
	player->sprite->texture = player->texture;
	player->sprite->config = SPRITE2D_TOP_LEFT;

	player->collider.scale = (struct vector2){32.0f, player->transform->scale.y - 1};
	
	player->speed = 600.0f;
	player->grounded = 0;
	player->jump_force = 440.0f;
	player->velocity.y = 1;
}

void player_move_and_collide(struct player* player, struct map_colliders* colliders, int jump_key)
{
	player->transform = ENTITY_GET_TRANSFORM(player->id);
	player->script = ENTITY_GET_SCRIPT(player->id);

	// GRAVITY
	if (!player->grounded) 
		player->velocity.y += GRAVITY * global_engine->delta_time;

	// Jump
	if (player->grounded && input_get_key_down(jump_key))
	{
		player->grounded = 0;
		player->velocity.y = -player->jump_force;
		audio_manager_play_and_forget_sound(global_engine->audio_manager, "../src/app/assets/sounds/jump.wav");
	}

	// Gravity is applied when our player is not on the ground

	player->delta_move.y += player->velocity.y * global_engine->delta_time;

	// Out of bounds
	if ((player->collider.position.x + player->collider.scale.x) + player->delta_move.x > engine_camera.bounds.x || player->collider.position.x + player->delta_move.x < 0.0f)
		player->delta_move.x = 0.0f;

	// Separate movement by axis 
	// Set collider to where we are moving 
	// Check for collisions an stop if one 
	for (int i = 0; i < colliders->size; i++)
	{
		// MOVE ON X AXIS ========================
		player->collider.position = (struct vector2){(player->transform->position.x + 16.0f) + player->delta_move.x, player->transform->position.y};

		if (physics_aabb_in_aabb(colliders->colliders[i], player->collider))
		{
			if (player->delta_move.x > 0)
				player->delta_move.x = 0;
				/* player->transform->position.x = colliders->colliders[i].position.x - player->collider.scale.x; */
			else if (player->delta_move.x < 0)
				player->delta_move.x = 0;
				/* player->transform->position.x = colliders->colliders[i].position.x + colliders->colliders[i].scale.x; */

			player->delta_move.x = 0;
		}

		// MOVE ON Y AXIS ======================
		player->collider.position = (struct vector2){player->transform->position.x + 16.0f, player->transform->position.y + player->delta_move.y};

		player->grounded = 0;

		if (physics_aabb_in_aabb(colliders->colliders[i], player->collider))
		{
			// If we are on the floor (velocity was positive due to gravity
			if (player->delta_move.y > 0)
			{
				player->grounded = 1;
				player->delta_move.y = 0;
				player->transform->position.y = colliders->colliders[i].position.y - player->collider.scale.y;
			}
			else if (player->delta_move.y < 0) // Velocity was negative (jumping)
			{
				player->velocity.y = 0;
				player->grounded = 0;
				player->delta_move.y = 0;
				player->transform->position.y = colliders->colliders[i].position.y + colliders->colliders[i].scale.y;
			}

			player->delta_move.y = 0;
			break;
		}
	}


	// Set position
	player->transform->position.x += player->delta_move.x;
	player->transform->position.y += player->delta_move.y;
}

