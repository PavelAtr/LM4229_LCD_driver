#include "twod.h"
#include <math.h>

dot* set_dot(unsigned int x, unsigned int y, unsigned char color, dot* shape)
{
	shape->flags = DOT;
	shape->x = x;
	shape->y = y;
	shape->color = color;

	return shape;
}

line* set_line(unsigned int x, unsigned int y, int dx, int dy, char color, line* shape)
{
	shape->flags = LINE;
	shape->x = x;
	shape->y = y;
	shape->dx = dx;
	shape->dy = dy;
	shape->color = color;
	return shape;
}


rect* set_rect(unsigned int x, unsigned int y, unsigned int dx, unsigned int dy, unsigned char color, unsigned char fillcolor, rect* shape)
{
	shape->flags = RECTANGLE;
	shape->x = x;
	shape->y = y;
	shape->dx = dx;
	shape->dy = dy;
	shape->color = color;
	shape->fillcolor = fillcolor;
	return shape;
}

circle* set_circle(unsigned int x, unsigned int y, unsigned char r, unsigned char color, unsigned char fillcolor, circle* shape)
{
	shape->flags = CIRCLE;
	shape->x = x;
	shape->y = y;
	shape->r = r;
	shape->color = color;
	shape->fillcolor = fillcolor;
	return shape;
}

splinex* set_splinex(int x, int y, int dx1,  int dx2, float k1, float k2, unsigned char color, splinex* shape)
{
	shape->flags = SPLINEX;
	shape->x = x;
	shape->y = y;
	shape->dx1 = dx1;
	shape->dx2 = dx2;
	shape->k1 = k1;
	shape->k2 = k2;
	shape->color = color;
	return shape;
}

spliney* set_spliney(int x, int y, int dy1,  int dy2, float k1, float k2, unsigned char color, spliney* shape)
{
	shape->flags = SPLINEY;
	shape->x = x;
	shape->y = y;
	shape->dy1 = dy1;
	shape->dy2 = dy2;
	shape->k1 = k1;
	shape->k2 = k2;
	shape->color = color;
	return shape;
}

unsigned char calc_dot(unsigned int x, unsigned int y, shape* shape)
{
	if (x == ((dot*)shape)->x && y == ((dot*)shape)->y)
		return ((dot*)shape)->color;
	return TRANSPARENT_COLOR;
}

unsigned char calc_line(unsigned int x, unsigned int y, shape* shape)
{
	int dx = x - ((line*)shape)->x;
	int dy = y - ((line*)shape)->y;
	
	if (dx <= 0 || dx >= ((line*)shape)->dx)
		return TRANSPARENT_COLOR;
		
	if ((dy < 0 || dy > ((line*)shape)->dy) && ((line*)shape)->dy >= 0)
		return TRANSPARENT_COLOR;

	if ((dy > 0 || dy < ((line*)shape)->dy) && ((line*)shape)->dy < 0)
		return TRANSPARENT_COLOR;

	double k = (double)(((line*)shape)->dy/((line*)shape)->dx);

	if (dy == k * dx)
		return ((line*)shape)->color;


	return TRANSPARENT_COLOR;
}

unsigned char calc_rect(unsigned int x, unsigned int y, shape* shape)
{
	unsigned char color = TRANSPARENT_COLOR;
    int dx = x - ((line*)shape)->x;
	int dy = y - ((line*)shape)->y;
	
	if (dx >= 0 &&
		dx <= ((rect*)shape)->dx &&
		dy >= 0 &&
		dy <= ((rect*)shape)->dy)
	{
		color = ((rect*)shape)->fillcolor;

		if (dy == 0 || dy == ((rect*)shape)->dy)
			color = ((rect*)shape)->color;

		if (dx == 0 || dx == ((rect*)shape)->dx)
			color = ((rect*)shape)->color;
	}
		
	return color;
}

unsigned char calc_circle(unsigned int x, unsigned int y, shape* shape)
{
	unsigned char color = TRANSPARENT_COLOR;
	
	if (x >= ((circle*)shape)->x - ((circle*)shape)->r &&
	    x <= ((circle*)shape)->x + ((circle*)shape)->r &&
		y >= ((circle*)shape)->y - ((circle*)shape)->r &&
		y <= ((circle*)shape)->y + ((circle*)shape)->r)
	{
		int dx = ((circle*)shape)->x - x;
		int dy = ((circle*)shape)->y - y;
		unsigned int r = sqrt(dx * dx +  dy * dy);
		if (r <= ((circle*)shape)->r)
			color = (((circle*)shape)->fillcolor);
		if (r == ((circle*)shape)->r)
			color = (((circle*)shape)->color);
	}
		
	return color;
}

unsigned char calc_splinex(unsigned int x, unsigned int y, shape* shape)
{
	unsigned char color = TRANSPARENT_COLOR;
	int dx = x - ((splinex*)shape)->x;
	int dy = y - ((splinex*)shape)->y;
	
	if ((dx < ((splinex*)shape)->dx1 || dx > ((splinex*)shape)->dx2))
		return TRANSPARENT_COLOR;
	
	int dytemp = (int)(((splinex*)shape)->k1 * dx * dx + ((splinex*)shape)->k2 * dx);
	
	if (dy == dytemp || dy + 1 == dytemp)
		color = ((splinex*)shape)->color;

	return color;
}

unsigned char calc_spliney(unsigned int x, unsigned int y, shape* shape)
{
	unsigned char color = TRANSPARENT_COLOR;
	int dx = x - ((spliney*)shape)->x;
	int dy = y - ((spliney*)shape)->y;
	
	if ((dy < ((spliney*)shape)->dy1 || dy > ((spliney*)shape)->dy2))
		return TRANSPARENT_COLOR;
	
	int dxtemp = (int)(((spliney*)shape)->k1 * dy * dy + ((spliney*)shape)->k2 * dy);
	
	if (dx == dxtemp || dx + 1 == dxtemp)
	color = ((spliney*)shape)->color;

	return color;
}


unsigned char point_color(unsigned int x, unsigned int y, shape** shapes, unsigned char shapes_count)
{
	unsigned char color = TRANSPARENT_COLOR;
	unsigned char pcolor = BACKGROUND_COLOR;
	
	for (unsigned char j = 0; j < shapes_count; j++)
	{
		color = TRANSPARENT_COLOR;
		if (shapes[j] == 0) continue;
		switch(shapes[j]->flags & SHAPE_MASK)
			{
			case DOT:
				color = calc_dot(x, y, shapes[j]);
				break;
			case LINE:
				color = calc_line(x, y, shapes[j]);
				break;
			case RECTANGLE:
				color = calc_rect(x, y, shapes[j]);
				break;
			case CIRCLE:
				color = calc_circle(x, y, shapes[j]);
				break;
			case SPLINEX:
				color = calc_splinex(x, y, shapes[j]);
				break;
			case SPLINEY:
				color = calc_spliney(x, y, shapes[j]);
				break;
			default:
				color = TRANSPARENT_COLOR;
				break;
		}
		pcolor = (color != TRANSPARENT_COLOR)? color : pcolor;
	}
	
	return pcolor;
}
