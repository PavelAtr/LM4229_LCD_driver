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
	
	if (dx <= 0 && dx >= ((line*)shape)->dx)
		return TRANSPARENT_COLOR;
		
	if (dy < 0 && dy > ((line*)shape)->dy && ((line*)shape)->dy >= 0)
		return TRANSPARENT_COLOR;

	if (dy > 0 && dy < ((line*)shape)->dy && ((line*)shape)->dy < 0)
		return TRANSPARENT_COLOR;

	double k = (double)(((line*)shape)->dy/((line*)shape)->dx);

	if (dx >= 0 && dx <= ((line*)shape)->dx && dy == k * dx)
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

		if (dx >= 0 && dx <= ((rect*)shape)->dx && (dy == 0 || dy == ((rect*)shape)->dy))
			color = ((rect*)shape)->color;

		if (dy >= 0 && dy <= ((rect*)shape)->dy && (dx == 0 || dx == ((rect*)shape)->dx))
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

#define NUM_ROWS 16

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
			default:
				color = TRANSPARENT_COLOR;
				break;
		}
		pcolor = (color != TRANSPARENT_COLOR)? color : pcolor;
	}
	
	return pcolor;
}
