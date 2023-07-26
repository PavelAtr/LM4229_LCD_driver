#ifndef _MINIGL_H
#define _MINIGL_H

#define DOT 0
#define LINE 1
#define RECTANGLE 3
#define CIRCLE 4
#define SPLINEX 5
#define SPLINEY 6
#define SHAPE_MASK 0x07

#define BLACK 0xFF
#define WHITE 0x00
#define TRANSPARENT_COLOR 0x0F
#define BACKGROUND_COLOR WHITE


typedef struct {
	unsigned char flags;
} shape;

typedef struct {
	unsigned char flags;
	unsigned int x;
	unsigned int y;
	unsigned char color;
	} dot;

typedef struct {
	unsigned char flags;
	unsigned int x;
	unsigned int y;
	int dx;
	int dy;
	unsigned char color;
	} line;

typedef struct {
	unsigned char flags;
	unsigned int x;
	unsigned int y;
	unsigned char dx;
	unsigned char dy;
	unsigned char color;
	unsigned char fillcolor;
	} rect;	
	
typedef struct {
	unsigned char flags;
	unsigned int x;
	unsigned int y;
	unsigned char r;
	unsigned char color;
	unsigned char fillcolor;
	} circle;	

typedef struct {
	unsigned char flags;
	int x;
	int y;
	int dx1;
	int dx2;
	float k1;
	float k2;
	unsigned char color;
} splinex;

typedef struct {
	unsigned char flags;
	int x;
	int y;
	int dy1;
	int dy2;
	float k1;
	float k2;
	unsigned char color;
} spliney;

//x y начало координат примитива
//dx dx1 dx2 dy dy1 dy2 область определения

dot* set_dot(unsigned int x, unsigned int y, unsigned char color, dot* shape);
line* set_line(unsigned int x, unsigned int y, int dx, int dy, char color, line* shape);
rect* set_rect(unsigned int x, unsigned int y, unsigned int dx, unsigned int dy, unsigned char color, unsigned char fillcolor, rect* shape);
circle* set_circle(unsigned int x, unsigned int y, unsigned char r, unsigned char color, unsigned char fillcolor, circle* shape);
//y = k1*x^2 + k2*x
splinex* set_splinex(int x, int y, int dx1,  int dx2, float k1, float k2, unsigned char color, splinex* shape);
//x = k1*y^2 + k2*y
spliney* set_spliney(int x, int y, int dy1,  int dy2, float k1, float k2, unsigned char color, spliney* shape);

unsigned char calc_dot(unsigned int x, unsigned int y, shape* shape);
unsigned char calc_line(unsigned int x, unsigned int y, shape* shape);
unsigned char calc_rect(unsigned int x, unsigned int y, shape* shape);
unsigned char calc_circle(unsigned int x, unsigned int y, shape* shape);
unsigned char calc_splinex(unsigned int x, unsigned int y, shape* shape);
unsigned char calc_spliney(unsigned int x, unsigned int y, shape* shape);

typedef void (*draw_point_function)(char color);
unsigned char point_color(unsigned int x, unsigned int y, shape** shapes, unsigned char shapes_count);

#endif