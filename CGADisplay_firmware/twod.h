#ifndef _MINIGL_H
#define _MINIGL_H

#define DOT 0
#define LINE 1
#define TRIANGLE 2
#define SQUARE 3
#define CIRCLE 4
#define SPLINEX 5
#define SPLINEY 6
#define SHAPE_MASK 0x07

#define COLOR_BLACK 1
#define COLOR_WHITE 2
#define COLOR_TRANSPARENT 0

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
	point_t cache_dy;
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
	upoint_t r;
	color_t color;
	color_t fillcolor;
	} circle;	

typedef struct {
	unsigned char flags;
	upoint_t x;
	upoint_t y;
	point_t dx1;
	point_t dx2;
	float k1;
	float k2;
	color_t color;
} splinex;

typedef struct {
	unsigned char flags;
	upoint_t x;
	upoint_t y;
	point_t dy1;
	point_t dy2;
	float k1;
	float k2;
	color_t color;
} spliney;

dot* set_dot(upoint_t x, upoint_t y, color_t color, dot* shap);
line* set_line(upoint_t x1, upoint_t y1, upoint_t x2, upoint_t y2, char color, line* shap);
triangle* set_triangle(upoint_t x1, upoint_t y1, upoint_t x2, upoint_t y2, upoint_t x3, upoint_t y3, color_t color, color_t fillcolor, triangle* shap);
squar* set_square(upoint_t x1, upoint_t y1, upoint_t x2, upoint_t y2, upoint_t x3, upoint_t y3, upoint_t x4, upoint_t y4, color_t color, color_t fillcolor, squar* shap);
circle* set_circle(upoint_t x, upoint_t y, upoint_t r, color_t color, color_t fillcolor, circle* shap);
//y = k1*x^2 + k2*x
splinex* set_splinex(point_t x, point_t y, point_t dx1,  point_t dx2, float k1, float k2, color_t color, splinex* shap);
//x = k1*y^2 + k2*y
spliney* set_spliney(point_t x, point_t y, point_t dy1,  point_t dy2, float k1, float k2, color_t color, spliney* shap);

color_t calc_dot(upoint_t x, upoint_t y, shape* shap, color_t transparent);
color_t calc_line(upoint_t x, upoint_t y, shape* shap, color_t transparent);
color_t calc_triangle(upoint_t x, upoint_t y, shape* shap, color_t transparent);
color_t calc_square(upoint_t x, upoint_t y, shape* shap, color_t transparent);
color_t calc_circle(upoint_t x, upoint_t y, shape* shap, color_t transparent);
color_t calc_splinex(upoint_t x, upoint_t y, shape* shap, color_t transparent);
color_t calc_spliney(upoint_t x, upoint_t y, shape* shap, color_t transparent);

color_t draw_shapes(upoint_t x, upoint_t y, shape** shapes, unsigned int shapes_count, color_t transparent);

#endif