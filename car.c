#include <avr/interrupt.h>
#include <avr/io.h>

#include "display.h"
#include "control.h"

// PID implementation from ATMEL AN221
#include "pid.h"

// Motor PID
#define K_P 15.00
#define K_I 2
#define K_D 2.00

#define PID_INTERVAL 14

// Steering PID
#define K_P2 0.2
#define K_I2 0.05
#define K_D2 0.00


uint8_t pidTimer = FALSE;

struct PID_DATA pidDataMotor;
struct PID_DATA pidDataSteering;

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

uint8_t sensorFunction(){
   static uint8_t last;
   uint8_t s = readSensors();
   if (s==0) return last;
   for(uint8_t i = 0; i < 8; i++){
      if(s&(1<<i)){
         last = i;
         return i+1;
      }
   }
}

int main (void) {
        setupControls();
        pid_Init(K_P * SCALING_FACTOR, K_I * SCALING_FACTOR , K_D * SCALING_FACTOR , &pidDataMotor);
        pid_Init(K_P2 * SCALING_FACTOR, K_I2 * SCALING_FACTOR , K_D2 * SCALING_FACTOR , &pidDataSteering);

        while (!buttonPressed());
	initDisplay();

        // Main code here
        PORTK |= 0x01;
	sei();
        uint16_t speed = 2;
        uint16_t referenceValue, measurementValue, inputValue, steeringMeasurement, steeringInput, steeringReference
           ;
	for(;;){
            if(pidTimer){
               referenceValue = speed;
               measurementValue = readTacho();
               inputValue = pid_Controller(referenceValue,measurementValue, &pidDataMotor);
               setMotorPWM(inputValue);
               clearScreen();
               printInteger(referenceValue,1);
               printInteger(measurementValue,2);
               printInteger(inputValue,3);

              // steeringReference = 127;
              // steeringMeasurement = sensorFunction()*32;
              // steeringInput = pid_Controller(steeringReference, steeringMeasurement, &pidDataSteering);
              // setServo(steeringReference -steeringInput);
              // printInteger(steeringReference,5);
              // printInteger(steeringMeasurement,6);
              // printInteger(steeringReference -steeringInput,7);

               pidTimer = FALSE;
            }
            setServo(sensorFunction()*32 -16);
            printString("Auto",0);
	}

}
