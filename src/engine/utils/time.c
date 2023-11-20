#include "time.h"

// Timer goes by number of frames
// Set target to desired frames 
int frame_timer_add_and_respond(struct frame_timer* timer)
{
	if (timer->frames < timer->target)
	{
		timer->frames++;
		return 0;
	}
	else 
		timer->frames = 0;

	return 1;
}


// This does not reset, as soon as it hits the target it will keep returning 1 every frame
int timer_add_and_respond(struct timer* timer, float delta_time)
{
	if (timer->timer < timer->target)
	{
		timer->timer += (delta_time) * 1000;
		return 0;
	}

	return 1;
}

void timer_reset(struct timer* timer)
{
	timer->timer = 0;
}

// Timer goes by number of milliseconds
// Set target to the desired milliseconds 
int timer_add_respond_and_reset(struct timer* timer, float delta_time)
{
	if (timer->timer < timer->target)
	{
		timer->timer += (delta_time) * 1000;
	}
	else
	{
		timer->timer = 0;
		return 1;
	}

	return 0;
}

