#include "color.h"

// Returns 1 if colors are the same 
// Returns 0 if colors are not the same
uint8_t colorcmp(struct rgba_color a, struct rgba_color b)
{ return (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a); }

struct rgba_color rgba_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{ return (struct rgba_color){ r, g, b, a }; }

