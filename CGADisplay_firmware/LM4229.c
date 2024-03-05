#include "LM4229.h"
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

unsigned char dpy_set_graphic_home(unsigned int address)
{
	unsigned char high = address >> 8;
	unsigned char low  = address & 0x00FF;
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

unsigned char dpy_set_text_home(unsigned int address)
{
	unsigned char high = address >> 8;
	unsigned char low  = address & 0x00FF;
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

unsigned char dpy_set_address_pointer(unsigned int address)
{
	unsigned char high = address >> 8;
	unsigned char low  = address & 0x00FF;
	unsigned char ret = dpy_send_param(low);
	ret |= dpy_send_param(high);
	ret |= dpy_send_cmd(SET_ADDRESS_POINTER);
	return ret;
}

unsigned char dpy_set_cursor_pointer(unsigned int address)
{
	unsigned char high = address >> 8;
	unsigned char low  = address & 0x00FF;
	unsigned char ret = dpy_send_param(low);
	ret |= dpy_send_param(high);
	ret |= dpy_send_cmd(SET_CURSOR_POINTER);
	return ret;
}

unsigned int cached_address = 0;

void dpy_set_cached_address(unsigned int address)
{
	if (address != cached_address)
		dpy_set_address_pointer(address);
}

unsigned char dpy_data_write(unsigned int address, unsigned char data)
{
	dpy_set_cached_address(address);
	dpy_data_write_mode();
	unsigned char ret = dpy_send_param(data);
	ret |= dpy_send_cmd(DATA_WRITE);
	return dpy_status_read();
}

unsigned char dpy_data_read(unsigned int address)
{
	dpy_set_cached_address(address);
	dpy_send_cmd(DATA_READ);
	dpy_data_read_mode();
	PORTCMD &= ~CE;
	_delay_us(HARDWARE_DELAY);
	PORTCMD |= CE;
	unsigned char ret = PINDATA;
	return ret;
}

unsigned char dpy_cell;

void dpy_point(unsigned int x, unsigned int y, unsigned char color, unsigned char send)
{
	
	unsigned int pointnum = (x + y * DISPLAY_WIDTH);
	unsigned int pos = pointnum / 8;
	unsigned char bitnum = pointnum % 8;
	if (bitnum == 0) dpy_cell = 0;
	if (color == DISPLAY_BLACK) dpy_cell |= (0b010000000 >> bitnum);
	else dpy_cell &= ~(0b010000000 >> bitnum);
	if (bitnum == 7 || send)
		dpy_data_write(GRAPHIC_AREA + pos, dpy_cell);
}

void dpy_clear(unsigned char color)
{
	unsigned int pos = 0;
	for (unsigned int y = 0; y < DISPLAY_HEIGHT; y++)
		for (unsigned int x = 0; x < DISPLAY_WIDTH / 8; x++)
		{
			dpy_data_write(GRAPHIC_AREA + pos, color);
			pos++;
		}
}
