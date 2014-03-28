#include <avr/interrupt.h>
#include <avr/io.h>

#include "display.h"
#include "control.h"
#include "pid.h"


#define K_P 1.00
#define K_I 0.00
#define K_D 0.00

#define PID_INTERVAL 10

uint8_t pidTimer = 0;

struct PID_DATA pidData;

ISR(TIMER3_COMPA_vect){
   static uint16_t i = 0;
   if (i < PID_INTERVAL){
      i++;
   }
   else {
      pidTimer = TRUE;
      i = 0;
   }
}


int main (void) {
        setupControls();
        pid_Init(K_P * SCALING_FACTOR, K_I * SCALING_FACTOR , K_D * SCALING_FACTOR , &pidData);

        while (!buttonPressed());
	initDisplay();

        // Main code here
        PORTK |= 0x01;
	sei();
        uint16_t speed = 6;
        uint16_t referenceValue, measurementValue, inputValue;
	for(;;){
            if(pidTimer){
               referenceValue = speed;
               measurementValue = readTacho();
               inputValue = pid_Controller(referenceValue,measurementValue, &pidData);
               setMotorPWM(inputValue);
               clearScreen();
               printInteger(measurementValue,0);
               printInteger(inputValue,1);
               pidTimer = FALSE;
            }

		uint8_t s = readSensors();
		for(uint8_t i = 0; i < 8; i++){
			if(s&(1<<i)){
				setServo(i*32);
                                if(i <=1 || i>=7) speed = 4;
                                else speed = 6;
                        }
		}
	}

}
