#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include "../../../include/GLFW/include/GLFW/glfw3.h"

#include "../math/vector/vector2.h"

#define KEY_NUMBER 350
#define BUTTON_NUMBER 5

//TODO: Support for mouse button input

static int input_keys_pressed[KEY_NUMBER];
static int input_keys_held[KEY_NUMBER];
static int input_mouse_buttons_pressed[BUTTON_NUMBER];
static int input_mouse_buttons_held[BUTTON_NUMBER];

static struct vector2 mouse_pos;

void input_key_callback(GLFWwindow* window, int key, int scancode, int action , int mods);
int input_get_key_down(int key);
int input_get_key(int key);
void input_reset_key_pressed_array(void);

void input_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void input_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
void input_reset_mouse_pressed_array(void);
void input_hide_cursor(int lever, GLFWwindow* window);
int input_get_mouse_button(int button);
int input_get_mouse_button_down(int button);
struct vector2 input_get_mouse_pos(void);

#endif
