#include "twod.h"
#include <math.h>
#include <stdlib.h>

dot* set_dot(unsigned int x, unsigned char y, unsigned char color, dot* shape)
{
	shape->flags = DOT;
	shape->x = x;
	shape->y = y;
	shape->color = color;

	return shape;
}

line* set_line(unsigned int x, unsigned char y, short dx, short dy, char color, line* shape)
{
	shape->flags = LINE;
	shape->x = x;
	shape->y = y;
	shape->dx = dx;
	shape->dy = dy;
	shape->color = color;
	return shape;
}


rect* set_rect(unsigned int x, unsigned char y, unsigned char dx, unsigned char dy, unsigned char color, unsigned char fillcolor, rect* shape)
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

circle* set_circle(unsigned int x, unsigned char y, unsigned char r, unsigned char color, unsigned char fillcolor, circle* shape)
{
	shape->flags = CIRCLE;
	shape->x = x;
	shape->y = y;
	shape->r = r;
	shape->color = color;
	shape->fillcolor = fillcolor;
	return shape;
}


unsigned char calc_dot(unsigned int x, unsigned char y, shape* shape)
{
	if (x == ((dot*)shape)->x && y == ((dot*)shape)->y)
		return ((dot*)shape)->color;
	return TRANSPARENT_COLOR;
}

unsigned char calc_line(unsigned int x, unsigned char y, shape* shape)
{
	int xx = x - ((line*)shape)->x;
	short yy = y - ((line*)shape)->y;
	
	if (xx <= 0 && xx >= ((line*)shape)->dx)
		return TRANSPARENT_COLOR;
		
	if (yy < 0 && yy > ((line*)shape)->dy && ((line*)shape)->dy >= 0)
		return TRANSPARENT_COLOR;

	if (yy > 0 && yy < ((line*)shape)->dy && ((line*)shape)->dy <= 0)
		return TRANSPARENT_COLOR;

	float k = (float )(((line*)shape)->dy/((line*)shape)->dx);
	if (xx >= 0 && xx <= ((line*)shape)->dx && yy == k * xx)
		return ((line*)shape)->color;
	
	return TRANSPARENT_COLOR;
}

unsigned char calc_rect(unsigned int x, unsigned char y, shape* shape)
{
	unsigned char color = TRANSPARENT_COLOR;

	int xx = x - ((line*)shape)->x;
	short yy = y - ((line*)shape)->y;
	
	if (xx >= 0 &&
		xx <= ((rect*)shape)->dx &&
		yy >= 0 &&
		yy <= ((rect*)shape)->dy)
	{
		color = ((rect*)shape)->fillcolor;

		if (xx >= 0 && xx <= ((rect*)shape)->dx && (yy == 0 || yy == ((rect*)shape)->dy))
			color = ((rect*)shape)->color;

		if (yy >= 0 && yy <= ((rect*)shape)->dy && (xx == 0 || xx == ((rect*)shape)->dx))
			color = ((rect*)shape)->color;
	}
		
	return color;
}

unsigned char calc_circle(unsigned int x, unsigned char y, shape* shape)
{
	unsigned char color = TRANSPARENT_COLOR;
	
	if (x >= ((circle*)shape)->x - ((circle*)shape)->r &&
	    x <= ((circle*)shape)->x + ((circle*)shape)->r &&
		y >= ((circle*)shape)->y - ((circle*)shape)->r &&
		y <= ((circle*)shape)->y + ((circle*)shape)->r)
	{
		int dx = ((circle*)shape)->x - x;
		short dy = ((circle*)shape)->y - y;
		unsigned int r = sqrt(dx * dx +  dy * dy);
		if (r <= ((circle*)shape)->r)
			color = (((circle*)shape)->fillcolor);
		if (r == ((circle*)shape)->r)
			color = (((circle*)shape)->color);
	}
		
	return color;
}

#define NUM_ROWS 16

unsigned char point_color(unsigned int x, unsigned char y, shape** shapes, unsigned char shapes_count)
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
