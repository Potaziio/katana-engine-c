#ifndef RENDER_H
#define RENDER_H

#include <stdlib.h>

#include "../shader/shader.h"
#include "../math/vector/vector3.h"
#include "../texture/texture.h"

#include "../entity/entity.h"
#include "../entity/entity_hashmap/transform_hashmap.h"
#include "../entity/entity_hashmap/textured_sprite2d_hashmap.h"
#include "../entity/entity_hashmap/sprite2d_hashmap.h"
#include "../entity/entity_hashmap/sprite2d_batch_simple.h"
#include "../entity/entity_hashmap/debug_line_hashmap.h"

#include "../math/physics/shape/shapes.h"

#include "../../../include/cglm/include/cglm/mat4.h"
#include "../../../include/cglm/include/cglm/cglm.h"
#include "../../../include/cglm/include/cglm/vec3.h"
#include "../../../include/cglm/include/cglm/vec2.h"

#define GLFW_INCLUDE_NONE
#include "../../../include/GLFW/include/GLFW/glfw3.h"
#include "../../../include/GL/include/GL/glew.h"

#include "../utils/time.h"

#define SPRITE2D_VERTEX_NUM 4
#define TEXTURED_SPRITE2D_VERTEX_NUM 4


// Only works with spritesheets that are horizontal 
struct animation 
{
    struct frame_timer clock; // how long each sprite is going to be on the screen for 
    int sprite_num; // number of sprites in spritesheet
    float sprite_padding; // padding between sprites in spritesheet
    float sprite_width;
    float spritesheet_width;
    int current_index;
    int start;
    int end;
};

extern struct shader* _render_default_shader;
extern struct shader* _render_default_tex_shader;
extern struct shader* _render_line_shader;
extern struct shader* _render_batch_simple_shader;
extern struct shader* _render_font_shader;
extern struct texture* _render_font_default_texture;

// Sprite2d
void render_system_init_sprite2d(struct transform_hashmap* transform_map, struct sprite2d_hashmap* sprite2d_map,  entity entity);
void render_system_render_sprite2d(struct transform_hashmap* transfom_map, struct sprite2d_hashmap* sprite2d_map, entity entity);
void render_system_update_sprite2d_verts(struct sprite2d_hashmap* sprite2d_map, entity entity);

// Textured sprite2d
void render_system_init_textured_sprite2d(struct transform_hashmap* transform_map, struct textured_sprite2d_hashmap* sprite2d_map,  entity entity);
void render_system_render_textured_sprite2d(struct transform_hashmap* transfom_map, struct textured_sprite2d_hashmap* sprite2d_map, entity entity);

// Sprite2d batch simple
void render_system_init_sprite2d_batch_simple(struct sprite2d_batch_simple_hashmap* batch_map, entity entity);
void render_system_render_sprite2d_batch_simple(struct sprite2d_batch_simple_hashmap* batch_map, entity entity);

// Debug Line 
void render_system_init_debug_line(struct debug_line_hashmap* line_map, entity entity);
void render_system_update_debug_line(struct debug_line_hashmap* line_map, entity entity);
void render_system_render_debug_line(struct debug_line_hashmap* line_map, entity entity);

void render_draw_debug_line(struct debug_line* line);
void render_init_debug_line(struct debug_line* line);
void render_free_debug_line(struct debug_line* line);

// Debug collider

// four lines have to be passed into the function
void render_init_debug_aabb_collider(struct aabb aabb, struct debug_line* lines);
void render_draw_debug_aabb_collider(struct aabb aabb, struct debug_line* lines);
void render_free_debug_aabb_collider(struct aabb aabb, struct debug_line* lines);

// Sprite animations, this is to be used by textured sprites

void render_textured_sprite_set_sprite(struct animation animation, int index, struct textured_sprite2d* sprite); 
void animation_cycle_through(struct animation* animation, struct textured_sprite2d* sprite, struct frame_timer* timer);

#endif 

