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

