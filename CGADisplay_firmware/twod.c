#include "twod.h"
#include <math.h>

#define CONTUR	1
#define UPPER	2
#define LOWER	3
#define LEFTER	4
#define RIGHTER	5
#define NOTDEFINED	6

dot* set_dot(upoint_t x, upoint_t y, color_t color, dot* shape)
{
	shape->flags = DOT;
	shape->x = x;
	shape->y = y;
	shape->color = color;

	return shape;
}

color_t calc_dot(upoint_t x, upoint_t y, shape* shap, color_t transparent)
{
	if (x == ((dot*)shap)->x && y == ((dot*)shap)->y)
	return ((dot*)shap)->color;
	return transparent;
}

color_t gorizontal_line(point_t idx, point_t idy, shape* shap)
{
	
	line* line1 = (line*) shap;
	
	if (!(idy == 0 && ((line1->dx >= 0 && idx <= line1->dx && idx >= 0) ||
	(line1->dx <= 0 && idx >= line1->dx && idx <= 0 )))) return NOTDEFINED;
	
	return CONTUR;
	
	
}

color_t vertical_line(point_t idx, point_t idy, shape* shap)
{
	line* line1 = (line*) shap;
		
	if (!((line1->dy >= 0 && idy <= line1->dy && idy >= 0) ||
	(line1->dy <= 0 && idy >= line1->dy && idy <= 0)))
		return NOTDEFINED;

	if (line1->cache_dy != idy)
	{
		line1->cache_dx = line1->k * idy;
		line1->cache_dy = idy;
	}
		
	if (line1->cache_dx == idx) return CONTUR;
	if (line1->cache_dx > idx) return LEFTER;
	if (line1->cache_dx < idx) return RIGHTER;
		
	return NOTDEFINED;
}


line* set_line(upoint_t x1, upoint_t y1, upoint_t x2, upoint_t y2, char color, line* shap)
{
	shap->flags = LINE;
	shap->x = x1;
	shap->y = y1;
	shap->dx = x2 - x1;
	shap->dy = y2 - y1;
	shap->cache_dx = 0;
	shap->cache_dy = 0;
	shap->color = color;
	if (shap->dy == 0)
		shap->k = 0;
	else
		shap->k = (double)shap->dx/(double)shap->dy;
	
	return shap;
}

short calc_line_raw(upoint_t x, upoint_t y, shape* shap)
{
	point_t dx = x - ((line*)shap)->x;
	point_t dy = y - ((line*)shap)->y;
	
	if (((line*)shap)->dy == 0)
		return gorizontal_line(dx, dy, shap);
	else
		return vertical_line(dx, dy, shap);
}

color_t calc_line(upoint_t x, upoint_t y, shape* shap, color_t transparent)
{
	if (calc_line_raw(x, y, shap) == CONTUR)
		return ((line*)shap)->color;

	return transparent;
}


triangle* set_triangle(upoint_t x1, upoint_t y1, upoint_t x2, upoint_t y2, upoint_t x3, upoint_t y3, color_t color, color_t fillcolor, triangle* shap)
{
	shap->flags = TRIANGLE;
	set_line(x1, y1, x2, y2, color, &shap->line1);
	set_line(x1, y1, x3, y3, color, &shap->line2);
	set_line(x2, y2, x3, y3, color, &shap->line3);
	shap->fillcolor = fillcolor;
	shap->color = color;
	
	return shap;
}

color_t calc_triangle(upoint_t x, upoint_t y, shape* shap, color_t transparent)
{
	short ret1, ret2, ret3;
	line* line1 = &((triangle*)shap)->line1;
	line* line2 = &((triangle*)shap)->line2;
	line* line3 = &((triangle*)shap)->line3;
	
	ret1 = calc_line_raw(x, y, (shape*)line1);
	ret2 = calc_line_raw(x, y, (shape*)line2);
	ret3 = calc_line_raw(x, y, (shape*)line3);

	if (ret1 == CONTUR || ret2 == CONTUR || ret3 == CONTUR)
	return ((triangle*)shap)->color;
	
	
	
	if (ret1 == LEFTER && (ret2 == RIGHTER || ret3 == RIGHTER)) return ((triangle*)shap)->fillcolor == (TWOD_TRANSPARENT) ? transparent : ((triangle*)shap)->fillcolor;
	if (ret2 == LEFTER && (ret1 == RIGHTER || ret3 == RIGHTER)) return ((triangle*)shap)->fillcolor == (TWOD_TRANSPARENT) ? transparent : ((triangle*)shap)->fillcolor;
	if (ret3 == LEFTER && (ret1 == RIGHTER || ret2 == RIGHTER)) return ((triangle*)shap)->fillcolor == (TWOD_TRANSPARENT) ? transparent : ((triangle*)shap)->fillcolor;
	
	return transparent;
}

squar* set_square(upoint_t x1, upoint_t y1, upoint_t x2, upoint_t y2, upoint_t x3, upoint_t y3, upoint_t x4, upoint_t y4, color_t color, color_t fillcolor, squar* shap)
{
	shap->flags = SQUARE;
	set_line(x1, y1, x2, y2, color, &shap->line1);
	set_line(x2, y2, x3, y3, color, &shap->line2);
	set_line(x3, y3, x4, y4, color, &shap->line3);
	set_line(x1, y1, x4, y4, color, &shap->line4);
	shap->color = color;
	shap->fillcolor = fillcolor;
	return shap;
}

color_t calc_square(upoint_t x, upoint_t y, shape* shap, color_t transparent)
{
	short ret1, ret2, ret3, ret4;
	line* line1 = &((squar*)shap)->line1;
	line* line2 = &((squar*)shap)->line2;
	line* line3 = &((squar*)shap)->line3;
	line* line4 = &((squar*)shap)->line4;
	
	ret1 = calc_line_raw(x, y, (shape*)line1);
	ret2 = calc_line_raw(x, y, (shape*)line2);
	ret3 = calc_line_raw(x, y, (shape*)line3);
	ret4 = calc_line_raw(x, y, (shape*)line4);

	if (ret1 == CONTUR || ret2 == CONTUR || ret3 == CONTUR || ret4 == CONTUR)
	return ((squar*)shap)->color;
	
	if (ret1 == LEFTER && (ret2 == RIGHTER || ret3 == RIGHTER || ret4 == RIGHTER)) return ((squar*)shap)->fillcolor == (TWOD_TRANSPARENT) ? transparent : ((squar*)shap)->fillcolor;
	if (ret2 == LEFTER && (ret1 == RIGHTER || ret3 == RIGHTER || ret4 == RIGHTER)) return ((squar*)shap)->fillcolor == (TWOD_TRANSPARENT) ? transparent : ((squar*)shap)->fillcolor;
	if (ret3 == LEFTER && (ret2 == RIGHTER || ret1 == RIGHTER || ret4 == RIGHTER)) return ((squar*)shap)->fillcolor == (TWOD_TRANSPARENT) ? transparent : ((squar*)shap)->fillcolor;
	if (ret4 == LEFTER && (ret2 == RIGHTER || ret3 == RIGHTER || ret1 == RIGHTER)) return ((squar*)shap)->fillcolor == (TWOD_TRANSPARENT) ? transparent : ((squar*)shap)->fillcolor;
	
	return transparent;

}

circle* set_circle(upoint_t x, upoint_t y, upoint_t r, color_t color, color_t fillcolor, circle* shape)
{
	shape->flags = CIRCLE;
	shape->x = x;
	shape->y = y;
	shape->r = r;
	shape->color = color;
	shape->fillcolor = fillcolor;
	return shape;
}


color_t calc_circle(upoint_t x, upoint_t y, shape* shap, color_t transparent)
{
	color_t color = transparent;
	
	if (x >= ((circle*)shap)->x - ((circle*)shap)->r &&
	x <= ((circle*)shap)->x + ((circle*)shap)->r &&
	y >= ((circle*)shap)->y - ((circle*)shap)->r &&
	y <= ((circle*)shap)->y + ((circle*)shap)->r)
	{
		point_t dx = ((circle*)shap)->x - x;
		point_t dy = ((circle*)shap)->y - y;
		upoint_t r = sqrt(dx * dx +  dy * dy);
		if (r < ((circle*)shap)->r)
		color = ((circle*)shap)->fillcolor == (TWOD_TRANSPARENT) ? transparent : ((circle*)shap)->fillcolor;
		if (r == ((circle*)shap)->r)
		color = (((circle*)shap)->color);
	}
	
	return color;
}

splinex* set_splinex(point_t x, point_t y, point_t dx1,  point_t dx2, float k1, float k2, color_t color, splinex* shape)
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

spliney* set_spliney(point_t x, point_t y, point_t dy1,  point_t dy2, float k1, float k2, color_t color, spliney* shape)
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

color_t calc_splinex(upoint_t x, upoint_t y, shape* shap, color_t transparent)
{
	color_t color = transparent;
	point_t dx = x - ((splinex*)shap)->x;
	point_t dy = y - ((splinex*)shap)->y;
	
	if ((dx < ((splinex*)shap)->dx1 || dx > ((splinex*)shap)->dx2))
	return transparent;
	
	point_t dytemp = (point_t)(((splinex*)shap)->k1 * dx * dx + ((splinex*)shap)->k2 * dx);
	
	if (dy == dytemp || dy + 1 == dytemp)
	color = ((splinex*)shap)->color;

	return color;
}


color_t calc_spliney(upoint_t x, upoint_t y, shape* shap, color_t transparent)
{
	color_t color = transparent;
	point_t dx = x - ((spliney*)shap)->x;
	point_t dy = y - ((spliney*)shap)->y;
	
	if ((dy < ((spliney*)shap)->dy1 || dy > ((spliney*)shap)->dy2))
	return transparent;
	
	point_t dxtemp = (point_t)(((spliney*)shap)->k1 * dy * dy + ((spliney*)shap)->k2 * dy);
	
	if (dx == dxtemp || dx + 1 == dxtemp)
	color = ((spliney*)shap)->color;

	return color;
}


color_t draw_shapes(upoint_t x, upoint_t y, shape** shapes, unsigned int shapes_count, color_t transparent)
{
	color_t color = transparent;
	
	for (unsigned int j = 0; j < shapes_count; j++)
	{
		if (shapes[j] == 0) continue;
		switch(shapes[j]->flags & SHAPE_MASK)
			{
			case DOT:
				color = calc_dot(x, y, shapes[j], color);
				break;
			case LINE:
				color = calc_line(x, y, shapes[j], color);
				break;
			case TRIANGLE:
				color = calc_triangle(x, y, shapes[j], color);
				break;
			case SQUARE:
				color = calc_square(x, y, shapes[j], color);
				break;
			case CIRCLE:
				color = calc_circle(x, y, shapes[j], color);
				break;
			case SPLINEX:
				color = calc_splinex(x, y, shapes[j], color);
				break;
			case SPLINEY:
				color = calc_spliney(x, y, shapes[j], color);
				break;
			default:
				color = transparent;
				break;
		}
	}
	
	return color;
}
