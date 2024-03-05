/*
 * CGADisolay_firmware.c
 *
 * Created: 24.07.2023 3:29:55
 * Author : pvsam
 */ 

#include <avr/io.h>
#include "twod.h"
#include "LM4229.h"
#include <avr/builtins.h>

void text(void)
{
	unsigned int dpy_text = 0;
	for (unsigned char i = 0; i < 255; i++)
	{
		dpy_data_write(TEXT_AREA + dpy_text, i);
		dpy_text++;
	}
}

unsigned char twod_to_display_color(color_t color)
{
	return (color == TWOD_BLACK)? DISPLAY_BLACK : DISPLAY_WHITE;
}

void draw(shape** shapes, unsigned char shapes_count, upoint_t x1, upoint_t y1, upoint_t x2, upoint_t y2)
{
	dpy_cell = 0;
	//Order of cycles important for lines and display addressing optimizations
	for (upoint_t y = y1; y < y2; y++)
		for (upoint_t x = x1; x < x2; x++)
			dpy_point(x, y, twod_to_display_color(draw_shapes(x, y, shapes, shapes_count, TWOD_WHITE)), 0);
}

shape* shapes[10];
squar squar1;
circle circle1;
dot dot1;
splinex spline1;
spliney spline2;
triangle triangle1;


int main(void)
{
	dpy_init();
	dpy_set_mode(DISPLAY_MODE|TEXT_ON_GRAPHICS_ON|CURSOR_ON_BLINK_OFF);
	dpy_set_mode(TEXT_MODE|OR_MODE);
	dpy_set_graphic_home(GRAPHIC_AREA);
	dpy_set_text_home(TEXT_AREA);
	
	shapes[0] = (shape*)set_square(20, 10, 60, 10, 70, 100, 10, 100, TWOD_BLACK, TWOD_BLACK, &squar1);//	shapes[1] = (shape*)set_circle(50, 70, 30, WHITE, WHITE, &circle1);
	shapes[1] = (shape*)set_triangle(40, 10, 70, 100, 10, 100, TWOD_WHITE, TWOD_WHITE, &triangle1);
	shapes[2] = (shape*)set_dot(40, 50, TWOD_BLACK, &dot1);
	shapes[3] = (shape*)set_splinex(70, 85, -20, 20, -0.1, -0.05, TWOD_BLACK, &spline1);
	shapes[4] = (shape*)set_spliney(70, 85, -20, 20, -0.1, -0.05, TWOD_BLACK, &spline2);
	shapes[5] = (shape*)set_circle(70, 85, 20, TWOD_BLACK, TWOD_TRANSPARENT, &circle1);

	while (1)
    {
		dpy_clear(DISPLAY_WHITE);
		text();	
		draw(shapes, 6, 8, 8, 128, 128);
    } 
}

