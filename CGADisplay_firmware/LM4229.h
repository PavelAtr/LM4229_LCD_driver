#ifndef _LM4229_H
#define _LM4229_H

#include <avr/io.h>

#define BIT0 0
#define BIT1 1
#define BIT2 2
#define BIT3 3
#define BIT4 4
#define BIT5 5
#define BIT6 6
#define BIT7 7

#define PORTDATA PORTB
#define PINDATA PINB
#define DDRDATA DDRB
#define PORTCMD PORTC
#define DDRCMD DDRC

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 128
#define DISPLAY_COLS 30
#define DISPLAY_ROWS 24

#define DISPLAY_BLACK 0xFF
#define DISPLAY_WHITE 0x00
#define DISPLAY_BACKGROUND DISPLAY_WHITE

#define HARDWARE_DELAY 0

#define CD (1<<BIT0) 
#define RD (1<<BIT1) 
#define WR (1<<BIT2) 
#define CE (1<<BIT3)

//Data adresses
#define TEXT_AREA 0x0000
#define GRAPHIC_AREA 0x0C00
#define CG_RAM_AREA 0x1800

// dpy status bits
#define COMMAND_EXEC_CAP (1<<BIT0)
#define DATA_RW_CAP (1<<BIT1)
#define AUTO_READ_CAP (1<<BIT2)
#define AUTO_WRITE_CAP (1<<BIT3)
#define CNTRLR_OPERATION_CAP (1<<BIT5)
#define ERROR_FLAG (1<<BIT6)
#define BLINK_COND (1<<BIT7)

//dpy display mode bits
#define DISPLAY_OFF 0b0000000
#define DISPLAY_MODE 0b10010000
#define TEXT_ON_GRAPHICS_ON 0b1100
#define GRAPHICS_ONLY_ON 0b1000
#define TEXT_ONLY_ON 0b0100
#define CURSOR_ON_BLINK_OFF 0b10
#define CURSOR_ON_BLINK_ON 0b11

//dpy text mode
#define TEXT_MODE 0b10000000 
#define OR_MODE 0b000
#define EXOR_MODE 0b001
#define AND_MODE 0b011
#define ATTR_MODE 0b100

// commands
#define SET_GRAPHIC_HOME 0b01000010
#define SET_TEXT_HOME 0b01000000
#define SET_GRAPHIC_AREA 0b01000011
#define SET_TEXT_AREA 0b01000001
#define DATA_WRITE 0b11000000
#define SET_CURSOR_POINTER 0b00100001
#define SET_OFFSET_REGISTER 0b00100010
#define SET_ADDRESS_POINTER 0b00100100

void dpy_init(void);
unsigned char dpy_status_read(void);
unsigned char dpy_send_cmd(unsigned char cmd);
unsigned char dpy_send_param(unsigned char cmd);
unsigned char dpy_set_mode(unsigned char mode);
unsigned char dpy_set_graphic_home(unsigned char high, unsigned char low);
unsigned char dpy_set_graphic_area(unsigned char columns);
unsigned char dpy_set_text_home(unsigned char high, unsigned char low);
unsigned char dpy_set_text_area(unsigned char columns);
unsigned char dpy_set_address_pointer(unsigned char high, unsigned char low);
unsigned char dpy_set_cursor_pointer(unsigned char high, unsigned char low);
unsigned char dpy_data_write(unsigned char data);
unsigned char dpy_point(unsigned char color);
void dpy_clear();

#endif
