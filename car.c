#include <avr/interrupt.h>
#include <avr/io.h>


#include "control.h"

int main (void) {

        setupControls();

        // Wait untill black button is pressed.
        while (!buttonPressed());

        // Main code here
	//setMotorPWM(200);
        PORTK |= 0x01;
	sei();
	for(;;){
		uint8_t s = readSensors();
		for(uint8_t i = 0; i < 8; i++){
			if(s&(1<<i))
				setServo(i*32);
		}
	}

}
