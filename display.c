#include "display.h"

void initDisplay()
{
  sendCommand(0x55);
  receiveResponse();
}

void sendCommand(uint8_t data){
   while (!(UCSR1A & (1<<UDRE1)));

   UDR1 = data;
}

void sendMultipleCommands(uint8_t data[]){
  for (uint8_t i=0; i<(sizeof(data)/sizeof(uint8_t)); i++){
	while (!(UCSR1A & (1<<UDRE1)));
	UDR1 = data[i];
  }
}

uint8_t receiveResponse(void){
   while ( !(UCSR1A & (1<<RXC1)));
   return UDR1;
}

void printString(char* buf,uint8_t row){ 

   uint8_t data[] = {0x73, 0x00, row, 0x03, 0xff, 0xff};
   sendMultipleCommands(data);

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
