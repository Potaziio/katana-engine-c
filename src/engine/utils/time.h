#ifndef TIME_H
#define TIME_H

#include <stdio.h>

struct frame_timer 
{
	int frames;
	int target;
};

struct timer 
{
	double target;
	double timer;
};

int frame_timer_add_and_respond(struct frame_timer* timer);
int timer_add_and_respond(struct timer* timer, float delta_time);
int timer_add_respond_and_reset(struct timer* timer, float delta_time);
void timer_reset(struct timer* timer);

#endif 
