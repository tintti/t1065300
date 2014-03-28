#include "display.h"

void USART_Transmit(uint8_t data){
   while (!(UCSR1A & (1<<UDRE1)));

   UDR1 = data;
}

uint8_t USART_Receive(void){
   while ( !(UCSR1A & (1<<RXC1)));
   return UDR1;
}

void printString(char* buf,uint8_t row){
   USART_Transmit(0x73);
   USART_Transmit(0x00);
   USART_Transmit(row);
   USART_Transmit(0x02);
   USART_Transmit(0xff);
   USART_Transmit(0xff);
   while(*buf){
      USART_Transmit(*buf);
      buf++;
   }
   USART_Transmit(0x00);

   USART_Receive();

}

void printInteger(uint16_t d,uint8_t row){
	char buf[10];
        itoa(d,buf,10);
	printString(buf,row);
}

void clearScreen(void){
	USART_Transmit(0x45);
	USART_Receive();
}
