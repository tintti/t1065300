#include "display.h"
#include <avr/delay.h>

#include "format.h"

void initDisplay()
{
  //autobaud
  sendCommand(0x55);
  receiveResponse();

  //set text to opaque
  sendCommand(0x4F);
  sendCommand(0x01);
  receiveResponse(); 

  //enable touch screen
  sendCommand(0x59);
  sendCommand(0x05);
  sendCommand(0x00);
  receiveResponse();

  //set font
  sendCommand(0x46);
  sendCommand(0x03);
  receiveResponse();
}

//makes screen faster
void setHighSpeed(){
   sendCommand(0x51);
   sendCommand(0x0a);
   receiveResponse();
   uint16_t ubrr = 25;
   UBRR1H = ubrr>>8;
   UBRR1L = ubrr;
   UCSR1B = (1<<RXEN1) | (1<<TXEN1);
   UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);
}


// UART transmitter to send command to display
void sendCommand(uint8_t data){
   while (!(UCSR1A & (1<<UDRE1)));
   UDR1 = data;
}

void sendMultipleCommands(uint8_t data[], size_t size){
  for (size_t i=0; i<size; i++){
	while (!(UCSR1A & (1<<UDRE1)));
	UDR1 = data[i];
  }
}

// UART response receiver from display
uint8_t receiveResponse(void){
   while ( !(UCSR1A & (1<<RXC1)));
   return UDR1;
}

void printString(char* buf, uint8_t row) { 

   struct coord loc;
   loc.x=0;
   loc.y=row;

   lcd_printf(loc, buf);

}

void printInteger(uint16_t d,uint8_t row){
   struct coord loc;
   loc.x=0; 
   loc.y=row;
   lcd_printf(loc, "%3d", d);
}

/*
void printIntegerGraphic(uint16_t d)
{
   char buf[10];
   itoa(d, buf, 10);

   uint8_t data[]={0x74, buf, 0x00, 0xbf, 0x00, 0xbf, 0xff, 0xff, 0xff, 0xff};
   sendMultipleCommands(data, 10);
   receiveResponse(); 
}
*/

void clearScreen(void){
   sendCommand(0x45);
   receiveResponse();
}

// consumer for custom display printing implemented with format library

void * lcd_putat(void * ap, const char *s, size_t n)
{
   struct coord *pc = (struct coord *)ap;

   while (n--)
   {
      uint8_t data[]={0x54, *s, pc->x, pc->y, 0xff, 0xff};
      sendMultipleCommands(data, 6);
      receiveResponse();

      pc->x++;
      s++;

      if (pc->x >= 20)
      {
	 pc->x = 0; 
	 pc->y++;
      }
   }

   return (void*)pc; 
}


// custom printf to print out stuff nicely 
int lcd_printf(struct coord loc, const char *fmt, ...)
{
   va_list arg;
   int done;

   va_start(arg, fmt); 
   done = format(lcd_putat, &loc, fmt, arg);
   va_end(arg);

   return done;
}
