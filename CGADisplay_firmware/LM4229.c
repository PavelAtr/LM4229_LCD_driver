#include "LM4229.h"
#include "main.h"
#include <util/delay.h>

enum dpy_mode{command_write, data_write, status_read, data_read};

enum dpy_mode mode;

void dpy_dataport_write_mode(void)
{
	PORTDATA = (0<<BIT7)|(0<<BIT6)|(0<<BIT5)|(0<<BIT4)|(0<<BIT3)|(0<<BIT2)|(0<<BIT1)|(0<<BIT0);
	DDRDATA = (1<<BIT7)|(1<<BIT6)|(1<<BIT5)|(1<<BIT4)|(1<<BIT3)|(1<<BIT2)|(1<<BIT1)|(1<<BIT0);
}

void dpy_dataport_read_mode(void)
{
	PORTDATA = (0<<BIT7)|(0<<BIT6)|(0<<BIT5)|(0<<BIT4)|(0<<BIT3)|(0<<BIT2)|(0<<BIT1)|(0<<BIT0);
	DDRDATA = (0<<BIT7)|(0<<BIT6)|(0<<BIT5)|(0<<BIT4)|(0<<BIT3)|(0<<BIT2)|(0<<BIT1)|(0<<BIT0);
}

void dpy_data_read_mode(void)
{
	if (mode != data_read)
	{
		dpy_dataport_read_mode();
		PORTCMD &= ~RD;
		PORTCMD &= ~CD;
		PORTCMD = WR;
		mode = data_read;
	}
}

void dpy_status_read_mode(void)
{
	if (mode != status_read)
	{
		dpy_dataport_read_mode();
		PORTCMD &= ~RD;
		PORTCMD = WR | CD;
		mode = status_read;
	}
}

void dpy_init(void)
{
	PORTCMD = (0<<BIT3)|(0<<BIT2)|(0<<BIT1)|(0<<BIT0);
	DDRCMD = (1<<BIT3)|(1<<BIT2)|(1<<BIT1)|(1<<BIT0);
	dpy_status_read_mode();
	_delay_ms(200);
	PORTCMD |= CE;
}


void dpy_command_write_mode(void)
{
	if (mode != command_write)
	{
		dpy_dataport_write_mode();
		PORTCMD &= ~WR;
		PORTCMD = RD | CD;
		mode = command_write;
	}
}

void dpy_data_write_mode(void)
{
	if (mode != command_write)
	{
		dpy_dataport_write_mode();
		PORTCMD &= ~WR;
		PORTCMD &= ~CD;
		PORTCMD = RD;
		mode = command_write;
	}
}

unsigned char dpy_send_cmd(unsigned char cmd)
{
	dpy_command_write_mode();
	PORTDATA = cmd;
	PORTCMD &= ~CE;
	_delay_us(HARDWARE_DELAY);
	PORTCMD |= CE;
	return  dpy_status_read();
}

unsigned char dpy_send_param(unsigned char data)
{
	dpy_data_write_mode();
	PORTDATA = data;
	PORTCMD &= ~CE;
	_delay_us(HARDWARE_DELAY);
	PORTCMD |= CE;
	return dpy_status_read();
}

unsigned char dpy_status_read(void)
{
	dpy_status_read_mode();
	PORTCMD &= ~CE;
	_delay_us(HARDWARE_DELAY);
	unsigned char ret = PINDATA;
	PORTCMD |= CE;
	return ret;
}

unsigned char dpy_set_mode(unsigned char mode)
{
	return dpy_send_cmd(mode);
}

unsigned char dpy_set_graphic_home(unsigned char high, unsigned char low)
{
	unsigned char ret = dpy_send_param(low);
	ret |= dpy_send_param(high);
	ret |= dpy_send_cmd(SET_GRAPHIC_HOME);
	return ret;
}

unsigned char dpy_set_graphic_area(unsigned char columns)
{
	unsigned char ret = dpy_send_param(columns);
	ret |= dpy_send_cmd(SET_GRAPHIC_AREA);
	return ret;
}

unsigned char dpy_set_text_home(unsigned char high, unsigned char low)
{
	unsigned char ret = dpy_send_param(low);
	ret |= dpy_send_param(high);
	ret |= dpy_send_cmd(SET_TEXT_HOME);
	return ret;
}

unsigned char dpy_set_text_area(unsigned char columns)
{
	unsigned char ret = dpy_send_param(columns);
	ret |= dpy_send_cmd(SET_TEXT_AREA);
	return ret;
}

unsigned char dpy_set_address_pointer(unsigned char high, unsigned char low)
{
	unsigned char ret = dpy_send_param(low);
	ret |= dpy_send_param(high);
	ret |= dpy_send_cmd(SET_ADDRESS_POINTER);
	return ret;
}

unsigned char dpy_set_cursor_pointer(unsigned char high, unsigned char low)
{
	unsigned char ret = dpy_send_param(low);
	ret |= dpy_send_param(high);
	ret |= dpy_send_cmd(SET_CURSOR_POINTER);
	return ret;
}

unsigned char dpy_data_write(unsigned char data)
{
	unsigned char ret = dpy_send_param(data);
	ret |= dpy_send_cmd(DATA_WRITE);
	return ret;
}

unsigned char row = 0;
unsigned char cell = 0;

unsigned char dpy_point(unsigned char color)
{
	unsigned char ret = 0;
	row |= color & (0b10000000>>cell);
	if (cell == 7) ret = dpy_data_write(row);
	cell ++;
	if (cell == 8)
	{
		row = 0;
		cell = 0;
	}
	
	return ret;
}

void dpy_clear()
{
	dpy_set_address_pointer(GRAPHIC_AREA >> 8, GRAPHIC_AREA & 0x00FF);
	for (unsigned char y = 0; y < DISPLAY_HEIGHT; y++)
		for (unsigned int x = 0; x < DISPLAY_WIDTH; x++)
			dpy_point(DISPLAY_BACKGROUND);
}
