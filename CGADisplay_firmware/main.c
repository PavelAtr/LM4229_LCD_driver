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
	unsigned int scene_x1 = 20;
	unsigned int scene_x2 = 80;
	unsigned int scene_y1 = 40;
	unsigned int scene_y2 = 100;
	dpy_set_address_pointer(GRAPHIC_AREA >> 8, GRAPHIC_AREA & 0x00FF);
	for (unsigned int y = 0; y < DISPLAY_HEIGHT; y++)
		for (unsigned int x = 0; x < DISPLAY_WIDTH; x++)
		{
			if (x >= scene_x1 && x <= scene_x2 && y >= scene_y1 && y <= scene_y2)
				PORTD = dpy_point(point_color(x, y, shapes, shapes_count));
			else
				PORTD = dpy_point(DISPLAY_WHITE);	
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
	dpy_set_mode(DISPLAY_MODE|TEXT_ON_GRAPHICS_ON|CURSOR_ON_BLINK_OFF);
	dpy_set_mode(TEXT_MODE|OR_MODE);
	dpy_set_graphic_home(GRAPHIC_AREA >> 8, GRAPHIC_AREA & 0x00FF);
	dpy_set_text_home(TEXT_AREA >> 8, TEXT_AREA & 0x00FF);
	
	shapes[0] = (shape*)set_rect(20, 40, 60, 60, BLACK, BLACK, &rect1);
	shapes[1] = (shape*)set_circle(50, 70, 30, WHITE, WHITE, &circle1);
	shapes[2] = (shape*)set_line(20, 70, 30, 30, BLACK, &line1);
	shapes[3] = (shape*)set_line(50, 100, 30, -30, BLACK, &line2);
	shapes[4] = (shape*)set_line(20, 70, 60, 0, BLACK, &line3);
	shapes[5] = (shape*)set_dot(50, 60, BLACK, &dot1);
	
    while (1)
    {
		dpy_clear();
		text();		
		draw(shapes, 6);
    }
}

