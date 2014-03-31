#include "display.h"
#include <avr/delay.h>
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
}

void setHighSpeed(){
   setLED1();
   sendCommand(0x51);
   sendCommand(0x0a);
   receiveResponse();
   uint16_t ubrr = 25;
   UBRR1H = ubrr>>8;
   UBRR1L = ubrr;
   UCSR1B = (1<<RXEN1) | (1<<TXEN1);
   UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);
   //_delay_ms(100);
   unSetLED1();
}

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

uint8_t receiveResponse(void){
   while ( !(UCSR1A & (1<<RXC1)));
   return UDR1;
}

void printString(char* buf, uint8_t row) { 

   uint8_t data[] = {0x73, 0x00, row, 0x03, 0xff, 0xff};
   sendMultipleCommands(data, 6);

   while(*buf){
      sendCommand(*buf);
      buf++;
   }

   sendCommand(0x00);
   receiveResponse();

}

void printInteger(uint16_t d,uint8_t row){
   char buf[10];
   itoa(d,buf,10);
   printString(buf,row);
}

void clearScreen(void){
   sendCommand(0x45);
   receiveResponse();
}

void addController(void) {
//draw text button
}

void touchScreen(void) {
   uint8_t data[] = {0x6f, 0x05};
   sendMultipleCommands(data, 2); 

   receiveResponse();
   uint8_t x_coordinate = receiveResponse();
   receiveResponse();
   receiveResponse();
   //printString("x coordinate for touch: ", 0); 
   printInteger(x_coordinate, 0); 
}
