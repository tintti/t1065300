#include <avr/interrupt.h>
#include <avr/io.h>

#include "display.h"
#include "control.h"

int main (void) {
        setupControls();

        while (!buttonPressed());
	initDisplay();

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
