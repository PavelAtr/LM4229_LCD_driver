#ifndef _MINIGL_H
#define _MINIGL_H

#define DOT 0
#define LINE 1
#define TRIANGLE 2
#define SQUARE 3
#define CIRCLE 4
#define SHAPE_MASK 0x07

#define TWOD_BLACK 1
#define TWOD_WHITE 2
#define TWOD_TRANSPARENT 0

typedef unsigned int upoint_t;
typedef int point_t;
typedef unsigned char color_t;

typedef struct {
	unsigned char flags;
} shape;

typedef struct {
	unsigned char flags;
	upoint_t x;
	upoint_t y;
	color_t color;
	} dot;

typedef struct {
	unsigned char flags;
	upoint_t x;
	upoint_t y;
	point_t dx;
	point_t dy;
	point_t cache_dx;
	float k;
	color_t color;
	} line;

typedef struct {
	unsigned char flags;
	line line1;
	line line2;
	line line3;
	color_t color;
	color_t fillcolor;
} triangle;

typedef struct {
	unsigned char flags;
	line line1;
	line line2;
	line line3;
	line line4;
	color_t color;
	color_t fillcolor;
	} squar;	
	
typedef struct {
	unsigned char flags;
	upoint_t x;
	upoint_t y;
	point_t x1;
	point_t y1;
	point_t x2;
	point_t y2;
	upoint_t r;
	color_t color;
	color_t fillcolor;
	} circle;	

dot* set_dot(upoint_t x, upoint_t y, color_t color, dot* shap);
line* set_line(upoint_t x1, upoint_t y1, upoint_t x2, upoint_t y2, char color, line* shap);
triangle* set_triangle(upoint_t x1, upoint_t y1, upoint_t x2, upoint_t y2, upoint_t x3, upoint_t y3, color_t color, color_t fillcolor, triangle* shap);
squar* set_square(upoint_t x1, upoint_t y1, upoint_t x2, upoint_t y2, upoint_t x3, upoint_t y3, upoint_t x4, upoint_t y4, color_t color, color_t fillcolor, squar* shap);
//x,y center; x1,y1 - x2,y2 visiblities
circle* set_circle(upoint_t x, upoint_t y, point_t x1, point_t y1, point_t x2, point_t y2, upoint_t r, color_t color, color_t fillcolor, circle* shap);

color_t calc_dot(upoint_t x, upoint_t y, shape* shap, color_t transparent);
color_t calc_line(upoint_t x, upoint_t y, shape* shap, color_t transparent);
color_t calc_triangle(upoint_t x, upoint_t y, shape* shap, color_t transparent);
color_t calc_square(upoint_t x, upoint_t y, shape* shap, color_t transparent);
color_t calc_circle(upoint_t x, upoint_t y, shape* shap, color_t transparent);

color_t draw_shapes(upoint_t x, upoint_t y, shape** shapes, unsigned int shapes_count, color_t transparent);

#endif