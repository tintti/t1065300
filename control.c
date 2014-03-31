#include "control.h"

void setupControls(void){
   // IR sensors
   IR_DDC = 0x00; //Set as input.
   IR_PORT = 0x00;

   // Black button
   BUTTON_DDR = 0x00;
   BUTTON_PORT = 0x00;

   // LEDs
   LEDS_DDR = 0xff; // Set as output.

   // Steering servo (Timer 1)
   TCCR1A |= (1<<COM1A1) | (1<<WGM11); // Set at bottom, clear at match. 
   TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS11); // Set mode to Fast PWM. Set prescaler to 8.
   
   ICR1 = 40000; // Set timer 1 top. FCPU / 8 / 40000 = 50 Hz
   OCR1A = 3000; // Set steering to center.
   DDRB |= (1<<PB5); // Enable Servo output.

   // Motor PWM (Timer 4)
   TCCR4A |= (1<<WGM41) | (1<< COM4A1);
   TCCR4B |= (1<<WGM43) | (1<< CS40);
   ICR4 = 800;
   DDRH |= (1<<PH3);

   // Motor half-bridge control
   DDRK = 0x0F;
   PORTK |= 0x0C;

   // Tachometer (Timer/Counter 5)
   DDRL = 0x00;
   PORTL = 0x00;
   TCCR5B |= (1<<CS52) | (1<<CS51) | (1<<CS50); // Set external clock input.

   // Interrupt (Timer 3)
   TCCR3B |= (1<<CS31) | (1<<CS30) | (1<<WGM32); // 1/64 prescaler.
   TIMSK3 |= (1<<OCIE3A);
   OCR3A = 250;

   // USART config
   uint16_t ubrr = 103;
   UBRR1H = ubrr>>8;
   UBRR1L = ubrr;
   UCSR1B = (1<<RXEN1) | (1<<TXEN1);
   UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);

}

void setServo(uint8_t v){
   // OCR1A is adjusted between 1980 and (1980+255*8) = 4020.
   // This equals 0.99 msec and 2.01 msec.
   OCR1A = 1980 + (v * 8);
}

inline uint8_t readSensors(void){
   return ~IR_SENSORS;
}


uint8_t buttonPressed(void){
   uint8_t pressed =  ~BUTTON_PIN;
   return (pressed & (1<<PE5));
}

void setMotorPWM(uint16_t pwm){
   if( pwm > PWM_MAX) pwm = PWM_MAX;
   OCR4A = pwm;
   }

uint16_t readTacho(){
   uint16_t t = TCNT5;
   TCNT5 = 0;
   return t;
}

void setLED1(void){
   LEDS_PORT |= (1<<PC1);
}

void unSetLED1(void){
   LEDS_PORT &= ~(1<<PC1);
}
