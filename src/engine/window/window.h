#ifndef WINDOW_H
#define WINDOW_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/GL/include/GL/glew.h"
#include "../../../include/GLFW/include/GLFW/glfw3.h"
#include "../utils/color.h"
#include "../utils/logger.h"

#include "../input/input.h"

enum WINDOW_EVENTS 
{
	WINDOW_RESIZE_EVENT = 0,
	WINDOW_MAXIMIZE_EVENT,
	WINDOW_EVENTS_SIZE,
};

struct window
{	
	GLFWmonitor* glfw_monitor;
	GLFWwindow* glfw_window;
	int width, height;
	int events[WINDOW_EVENTS_SIZE];
	char* title;
	struct rgba_color color;
	int should_close;
	int fullscreen;
};

int window_create(struct window* window);
void window_begin_frame(struct window* window);
int window_should_close(struct window* window);
void window_end_frame(struct window* window);
void window_free_memory(struct window* window);
int window_get_event(enum WINDOW_EVENTS event, struct window window);
void window_close(struct window* window);
void window_toggle_fullscreen(struct window* window);

#endif
