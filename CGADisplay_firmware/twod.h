#ifndef _MINIGL_H
#define _MINIGL_H

#define DOT 0
#define LINE 1
#define RECTANGLE 3
#define CIRCLE 4
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
	unsigned char y;
	unsigned char color;
	} dot;

typedef struct {
	unsigned char flags;
	unsigned int x;
	unsigned char y;
	short dx;
	short dy;
	unsigned char color;
	} line;

typedef struct {
	unsigned char flags;
	unsigned int x;
	unsigned char y;
	unsigned char dx;
	unsigned char dy;
	unsigned char color;
	unsigned char fillcolor;
	} rect;	
	
typedef struct {
	unsigned char flags;
	unsigned int x;
	unsigned char y;
	unsigned char r;
	unsigned char color;
	unsigned char fillcolor;
	} circle;	

dot* set_dot(unsigned int x, unsigned char y, unsigned char color, dot* shape);
line* set_line(unsigned int x, unsigned char y, short dx, short dy, char color, line* shape);
rect* set_rect(unsigned int x, unsigned char y, unsigned char dx, unsigned char dy, unsigned char color, unsigned char fillcolor, rect* shape);
circle* set_circle(unsigned int x, unsigned char y, unsigned char r, unsigned char color, unsigned char fillcolor, circle* shape);

unsigned char calc_dot(unsigned int x, unsigned char y, shape* shape);
unsigned char calc_line(unsigned int x, unsigned char y, shape* shape);
unsigned char calc_rect(unsigned int x, unsigned char y, shape* shape);
unsigned char calc_circle(unsigned int x, unsigned char y, shape* shape);

typedef void (*draw_point_function)(char color);
unsigned char point_color(unsigned int x, unsigned char y, shape** shapes, unsigned char shapes_count);

#endif