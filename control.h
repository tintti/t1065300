#include <stdint.h>
#include <avr/io.h>

#define IR_PORT PORTD
#define IR_DDC DDRD
#define IR_SENSORS PIND

#define BUTTON_PORT PORTE
#define BUTTON_DDR DDRE
#define BUTTON_PIN PINE 

#define LEDS_PORT PORTC
#define LEDS_DDR DDRC
#define LEDS PORTC

void setupControls(void);

uint8_t buttonPressed(void);

void setServo(uint8_t v);

uint8_t readSensors(void);


