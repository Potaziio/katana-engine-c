#ifndef COLOR_H
#define COLOR_H

#include <stdio.h>

#define RGBA_WHITE 255, 255, 255, 255
#define RGBA_BLACK 0, 0, 0, 255
#define RGBA_RED 255, 0, 0, 255
#define RGBA_GREEN 0, 255, 0, 255
#define RGBA_BLUE 0, 0, 255, 255
#define RGBA_YELLOW 255, 255, 0, 255
#define RGBA_MAGENTA 255, 0, 255, 255
#define RGBA_CYAN 0, 255, 255, 255
#define RGBA_GRAY 127, 127, 127, 255
#define RGBA_SKY_BLUE 135, 206, 235, 255
#define RGBA_NORMALIZED_FLOAT(x) ((float)x.r / 255.0f), ((float)x.g / 255.0f), ((float)x.b / 255.0f), ((float)x.a / 255.0f)
#define RGBA_NORMALIZED_INT(x) (int)(x.r / 255), (int)(x.g / 255), (int)(x.b / 255), (int)(x.a / 255)
#define RGBA_NORMALIZED(x) (x.r / 255), (x.g / 255), (x.b / 255), (x.a / 255)
#define RGBA_UNPACK(x) x.r, x.g, x.b, x.a

struct rgba_color 
{
	unsigned char r, g, b, a;
};

struct rgb_color 
{
	unsigned char r, g, b;
};

#endif
