/*
 * CGADisolay_firmware.c
 *
 * Created: 24.07.2023 3:29:55
 * Author : pvsam
 */ 

#include <avr/io.h>
#include "twod.h"
#include "LM4229.h"

void draw(shape** shapes, unsigned char shapes_count)
{
	dpy_set_address_pointer(GRAPHIC_AREA & 0x00FF, GRAPHIC_AREA >> 8);
	for (unsigned char y = 0; y < DISPLAY_HEIGHT; y++)
		for (unsigned int x = 0; x < DISPLAY_WIDTH; x++)
				dpy_point(point_color(x, y, shapes, shapes_count));
}



shape* shapes[10];
rect rect1;
circle circle1;
line line1;
line line2;
line line3;
dot dot1;


int main(void)
{
	dpy_init();
    /* Replace with your application code */
	shapes[0] = (shape*)set_rect(20, 20, 60, 60, BLACK, BLACK, &rect1);
	shapes[1] = (shape*)set_circle(50, 50, 30, WHITE, WHITE, &circle1);
	shapes[2] = (shape*)set_line(20, 50, 30, 30, BLACK, &line1);
	shapes[3] = (shape*)set_line(50, 80, 30, -30, BLACK, &line2);
	shapes[4] = (shape*)set_line(20, 50, 60, 0, BLACK, &line3);
	shapes[5] = (shape*)set_dot(50, 40, BLACK, &dot1);
	
    while (1)
    {
		dpy_clear();
		draw(shapes, 6);
    }
}

