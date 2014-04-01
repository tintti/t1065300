#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <avr/io.h>
#include <stdlib.h>


// for lightweight printf
struct coord {
   short x, y; 	
};

void sendCommand(uint8_t c);

void sendMultipleCommands(uint8_t data[], size_t size);

uint8_t receiveResponse(void);

void printString(char* buf,uint8_t row);

void printInteger(uint16_t d,uint8_t row);

void clearScreen(void);

void initDisplay(void);

void setHighSpeed(void);

void * lcd_putat(void * ap, const char *s, size_t n);

int lcd_printf(struct coord loc, const char *fmt, ...);

#endif
