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
		{
			PORTD = dpy_point(point_color(x, y, shapes, shapes_count));
			
		}
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
	PORTD = (0<<BIT7)|(0<<BIT6)|(0<<BIT5)|(0<<BIT4)|(0<<BIT3)|(0<<BIT2)|(0<<BIT1)|(0<<BIT0);
	DDRD = (1<<BIT7)|(1<<BIT6)|(1<<BIT5)|(1<<BIT4)|(1<<BIT3)|(1<<BIT2)|(1<<BIT1)|(1<<BIT0);
	//Display init
	dpy_init();
	dpy_set_display_mode(DISPLAY_MODE|TEXT_ON_GRAPHICS_ON|CURSOR_ON_BLINK_OFF);
	dpy_set_graphic_home(GRAPHIC_AREA & 0x00FF, GRAPHIC_AREA >> 8);
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
		dpy_set_address_pointer(TEXT_AREA & 0x00FF, TEXT_AREA >> 8);
		dpy_data_write(65);
		dpy_data_write(66);
		dpy_data_write(67);
		dpy_data_write(68);
		
		draw(shapes, 6);
    }
}

