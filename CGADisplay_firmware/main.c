/*
 * CGADisolay_firmware.c
 *
 * Created: 24.07.2023 3:29:55
 * Author : pvsam
 */ 

#include <avr/io.h>
#include "twod.h"
#include "LM4229.h"

void text(void)
{
	dpy_set_address_pointer(TEXT_AREA >> 8, TEXT_AREA & 0x00FF);
	for (unsigned char i = 0; i < 255; i++)
		dpy_data_write(i);
}

void draw(shape** shapes, unsigned char shapes_count)
{
	dpy_set_address_pointer(GRAPHIC_AREA >> 8, GRAPHIC_AREA & 0x00FF);
	for (upoint_t y = 0; y < DISPLAY_HEIGHT; y++)
		for (upoint_t x = 0; x < DISPLAY_WIDTH; x++)
		{
			color_t color = draw_shapes(x, y, shapes, shapes_count, COLOR_WHITE);
			if (color == COLOR_BLACK)
				dpy_point(DISPLAY_BLACK);
			else
				dpy_point(DISPLAY_WHITE);
		}
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
	PORTD = (0<<BIT7)|(0<<BIT6)|(0<<BIT5)|(0<<BIT4)|(0<<BIT3)|(0<<BIT2)|(0<<BIT1)|(0<<BIT0);
	DDRD = (1<<BIT7)|(1<<BIT6)|(1<<BIT5)|(1<<BIT4)|(1<<BIT3)|(1<<BIT2)|(1<<BIT1)|(1<<BIT0);

	//Display init
	dpy_init();
	dpy_set_mode(DISPLAY_MODE|TEXT_ON_GRAPHICS_ON|CURSOR_ON_BLINK_OFF);
	dpy_set_mode(TEXT_MODE|OR_MODE);
	dpy_set_graphic_home(GRAPHIC_AREA >> 8, GRAPHIC_AREA & 0x00FF);
	dpy_set_text_home(TEXT_AREA >> 8, TEXT_AREA & 0x00FF);
	
	shapes[0] = (shape*)set_square(20, 10, 60, 10, 70, 100, 10, 100, COLOR_BLACK, COLOR_WHITE, &squar1);//	shapes[1] = (shape*)set_circle(50, 70, 30, WHITE, WHITE, &circle1);
	shapes[1] = (shape*)set_triangle(40, 10, 70, 100, 10, 100, COLOR_WHITE, COLOR_BLACK, &triangle1);
	shapes[2] = (shape*)set_dot(40, 50, COLOR_WHITE, &dot1);
	shapes[3] = (shape*)set_splinex(70, 85, -20, 20, -0.1, -0.05, COLOR_WHITE, &spline1);
	shapes[4] = (shape*)set_spliney(70, 85, -20, 20, -0.1, -0.05, COLOR_WHITE, &spline2);
	shapes[5] = (shape*)set_circle(70, 85, 20, COLOR_WHITE, COLOR_TRANSPARENT, &circle1);

	
    while (1)
    {
		dpy_clear();
		text();		
		draw(shapes, 6);
    }
}

