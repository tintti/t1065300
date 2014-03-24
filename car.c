#include <avr/interrupt.h>
#include <avr/io.h>

void setup(void) {

        // setup timer 1 mode to 14
	TCNT1 = 0;
        TCCR1A |= (1<<WGM11);
        TCCR1B |= (1<<WGM13) | (1<<WGM12);
        ICR1 = 40000;
        // prescaler...
        TCCR1B |= (1<<CS11);

        // setup timer 1 output compare unit A
	//      DDRB |= (1<<PB5);
        TCCR1A |= (1<<COM1A1) | (1<<COM1A0); // set oc1 on compare match
        OCR1A = 40000 - 1600;

	sei();
}


int main (void) {

	setup();

        int servo_values[8] = {2000,2250, 2500,2750, 3000,3250,3500,3750};

	for (;;) {

		// If the car can see the track, drive
		// Sensors are on PA0-PA7
		// Polling for code
                for(int i = 0; i < 8; i++) {

			if (!(PINA & (1 << i)))  {
				DDRB |= (1<<PB5);
				PORTC = _BV(i);
				OCR1A = 40000-servo_values[i];

				// If the sensor input changes, change direction
				// Servo steering is at PB5
				DDRB &= ~(1<<PB5);

				//Motor
				//INA = PK0, INB= PK1, ENA/DIAGA=PK2
				//ENB/DIAGB = PK3, CS=PK4, PWM=PH3
			}
	        }
	}
	return 0;
}
