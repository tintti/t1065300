#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <avr/io.h>
#include <stdlib.h>

void sendCommand(uint8_t c);

uint8_t receiveResponse(void);

void printString(char* buf,uint8_t row);

void printInteger(uint16_t d,uint8_t row);

void clearScreen(void);

void initDisplay(void);

#endif
