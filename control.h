#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define IR_PORT PORTA
#define IR_DDC DDRA
#define IR_SENSORS PINA

#define BUTTON_PORT PORTE
#define BUTTON_DDR DDRE
#define BUTTON_PIN PINE 

#define LEDS_PORT PORTC
#define LEDS_DDR DDRC
#define LEDS PORTC

#define PWM_MAX 300

void setupControls(void);

uint8_t buttonPressed(void);

void setServo(uint8_t v);

uint8_t readSensors(void);


void setMotorPWM(uint16_t);

ISR(TIMER3_COMPA);

#endif
