#include "app.h"
#include "player/player.h"
#include "map/map_editor/map_editor.h"

int editing = 0;

struct game_map game_map;
struct player player;
struct timer shoot_timer;

enum WEAPON_FIRE_MODE
{
	WEAPON_FIRE_MODE_AUTO, 
	WEAPON_FIRE_MODE_SEMI,
};

struct weapon_data
{
	int cap;
	float reload_time;
	float fire_rate;
	float bullet_speed; 
	enum WEAPON_FIRE_MODE fire_mode;
};

struct bullet_data
{
	struct vector2 dir;
	struct aabb collider;
};

struct data
{
	struct weapon_data weapon;
	struct bullet_data bullet;
	struct map_colliders* colliders;
};

struct text* ammo_text;
struct text* entities_text;

struct weapon_data small_rifle = {30, 300.0f, 125.0f, 1000.0f, WEAPON_FIRE_MODE_AUTO};
struct weapon_data small_pistol = {8, 150.0f, 300.0f, 650.0f, WEAPON_FIRE_MODE_SEMI};
struct weapon_data current_weapon;

int mag;

void test_u(entity e)
{
	struct transform* t = ENTITY_GET_TRANSFORM(e);
	struct script* sc = ENTITY_GET_SCRIPT(e);
	struct data* data = (struct data*)sc->data;

	vector2_normalize(&data->bullet.dir);

	t->position.x += data->bullet.dir.x * data->weapon.bullet_speed * global_engine->delta_time;
	t->position.y += data->bullet.dir.y * data->weapon.bullet_speed * global_engine->delta_time;

	t->rotation_angle += data->bullet.dir.x * global_engine->delta_time * 1000.0f;
	t->rotation.z = 1.0f;

	data->bullet.collider.position.x = t->position.x - t->scale.x * 0.5f;
	data->bullet.collider.position.y = t->position.y - t->scale.y * 0.5f;
	data->bullet.collider.scale = (struct vector2){t->scale.x, t->scale.y};

	for (int i = 0; i < data->colliders->size; i++)
	{
		if (physics_aabb_in_aabb(data->bullet.collider, data->colliders->colliders[i]))
			engine_pop_entity(global_engine, e);
	}

	if (t->position.x > engine_camera.bounds.x || t->position.x < 0.0f || t->position.y < 0.0f)
		engine_pop_entity(global_engine, e);
}

struct frame_timer color_timer;
struct animation player_animation;

void app_start(void)
{
	global_engine->config |= ENGINE_SILENCE_ENTITY_LOG;
	engine_camera.type = ORTHOGRAPHIC;

	if (editing) { 
		map_editor_set_layout();
		return;
	}

	current_weapon = small_rifle;

	mag = current_weapon.cap; 

	game_map_create(&game_map);

	texture_load(&player.texture, "../src/app/assets/textures/player_spritesheet.png");
	player_create(&player);
	player.speed = 425.0f;

	script_system_set_funcs(player.script, NULL, NULL);
	player.transform->position = (struct vector3){rand_int(0, engine_camera.bounds.x), 300.0f, 0.0f};

	ammo_text = text_init(_engine_default_font, (struct rgba_color){RGBA_BLACK}, (struct vector2){engine_camera.bounds.x - 140, 25.0f}, 50.0f, "");
	entities_text = text_init(_engine_default_font, (struct rgba_color){RGBA_BLACK}, (struct vector2){-3.0f, 25.0f}, 50.0f, "");

	shoot_timer.target = current_weapon.fire_rate;

	player_animation.sprite_num = 3;
	player_animation.sprite_padding = 1.0f;
	player_animation.sprite_width = 64.0f;
	player_animation.clock.target = 2000;
	player_animation.start = 0;
	player_animation.end = 1;
	player_animation.spritesheet_width = player.texture.width;

	render_textured_sprite_set_sprite(player_animation, 0, player.sprite);

	color_timer.target = 1000;

	engine_app_preload_callback_func(global_engine, app_preload);
}

void app_preload(void)
{
	render_textured_sprite_set_sprite(player_animation, 0, player.sprite);	
}

void app_update(void)
{
	if (editing) 
	{ 
		map_editor_on_update();
		return;
	}

	struct vector2 shot_dir;

	switch (current_weapon.fire_mode)
	{
		case WEAPON_FIRE_MODE_AUTO: 
			shot_dir = (struct vector2){input_get_key(GLFW_KEY_RIGHT) ? 1 : input_get_key(GLFW_KEY_LEFT) ? -1 : 0,
				input_get_key(GLFW_KEY_DOWN) ? 1 : input_get_key(GLFW_KEY_UP) ? -1 : 0};
			break;
		case WEAPON_FIRE_MODE_SEMI:
			shot_dir = (struct vector2){input_get_key_down(GLFW_KEY_RIGHT) ? 1 : input_get_key_down(GLFW_KEY_LEFT) ? -1 : 0,
				input_get_key_down(GLFW_KEY_DOWN) ? 1 : input_get_key_down(GLFW_KEY_UP) ? -1 : 0};
			break;
	}

	if (timer_add_and_respond(&shoot_timer, global_engine->delta_time))
	{
		if (vector2_mag(shot_dir) > 0 && mag > 0)
		{
			audio_manager_play_and_forget_sound(global_engine->audio_manager, "../src/app/assets/sounds/shoot.wav");
			entity e = engine_create_entity(global_engine, TRANSFORM | SPRITE2D | SCRIPT);

			struct transform* t = ENTITY_GET_TRANSFORM(e);
			struct sprite2d* sp = ENTITY_GET_SPRITE2D(e);
			struct script* sc = ENTITY_GET_SCRIPT(e);

			t->position = (struct vector3){player.transform->position.x + player.transform->scale.x * 0.5f, player.transform->position.y + player.transform->scale.y * 0.5f, 0.0f};
			t->scale = (struct vector3){10.0f, 10.0f, 0.0f};

			sp->color = (struct rgba_color){RGBA_BLACK};

			struct data* data = (struct data*)malloc(sizeof(struct data));
			data->weapon = current_weapon;
			data->bullet.dir = shot_dir;
			data->colliders = &game_map.colliders; 

			script_system_set_data(sc, data);
			script_system_set_funcs(sc, NULL, test_u);

			timer_reset(&shoot_timer);

			mag--;
		}
	}

	if (input_get_key_down(GLFW_KEY_R))
	{
		mag = current_weapon.cap;
	}

	struct vector2 input = {input_get_key(GLFW_KEY_A) ? -1 : input_get_key(GLFW_KEY_D) ? 1 : 0, 0.0f};
    player.delta_move = (struct vector2){input.x * player.speed * global_engine->delta_time, 0.0};

	player_move_and_collide(&player, &game_map.colliders, GLFW_KEY_SPACE);

	struct rgba_color colors[] =
	{
		{RGBA_BLACK}, {RGBA_BLUE}, {RGBA_RED}, {RGBA_YELLOW}, {RGBA_GREEN}, {RGBA_MAGENTA}
	};

	if (frame_timer_add_and_respond(&color_timer))
		ammo_text->color = colors[rand_int(0, 6)];

	char buf[100];
	sprintf(buf, "%d", mag);
	text_mod(ammo_text, buf); 
	memset(buf, 0, 100 * sizeof(char));
	sprintf(buf, "%d", global_engine->fps);

	text_mod(entities_text, buf);

	text_render(ammo_text);
	text_render(entities_text);

	animation_cycle_through(&player_animation, player.sprite, &player_animation.clock);
}

void app_on_tick(void)
{
}

void app_end(void)
{
	if (editing) 
	{ 
		map_editor_serialize();
		return;
	}

	texture_free(&grass_texture);
	texture_free(&cobble_texture);
	texture_free(&player.texture);

	free(game_map.colliders.colliders);

	text_free(entities_text);
	free(entities_text);

	text_free(ammo_text);
	free(ammo_text);
}

