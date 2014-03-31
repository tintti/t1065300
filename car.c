#include <avr/interrupt.h>
#include <avr/io.h>
#define F_CPU 16000000UL
#include <avr/delay.h>

#include "display.h"
#include "control.h"

// PID implementation from ATMEL AN221
#include "pid.h"

// Motor PID
#define K_P 15.00
#define K_I 6
#define K_D 4.00

#define PID_INTERVAL 100

// Steering PID
#define K_P2 0.3
#define K_I2 0.0
#define K_D2 0.2


volatile uint8_t pidTimer = FALSE;
volatile uint8_t servoTimer = FALSE;

pidData_t pidDataMotor;
pidData_t pidDataSteering;
volatile uint8_t sensorArray[8];
ISR (TIMER3_COMPA_vect)
{
  static uint16_t i = 0;
  if (i < PID_INTERVAL)
    {
      i++;
    }
  else
    {
      pidTimer = TRUE;
      i = 0;
    }

  static uint8_t k =0;
  if (k < 5) k++;
  else {
     servoTimer = TRUE;
     k = 0;
  }

  uint8_t s = readSensors ();
  for (uint8_t j = 0; j < 8; j++){
     if (s & (1<<j)){
        sensorArray[j]++;
     }
  }
}

uint8_t
sensorFunction (void)
{
  for (uint8_t i = 0; i < 8; i++)
    {
      if (sensorArray[i] > 10)
	{
	  return i + 1;
	}
    }
}


void clearArray(void){
   for(uint8_t i = 0; i<8;i++){
      sensorArray[i] = 0;
   }
}

int
main (void)
{
  setupControls ();
  pid_Init (K_P * SCALING_FACTOR, K_I * SCALING_FACTOR, K_D * SCALING_FACTOR,
	    &pidDataMotor);
  pid_Init (K_P2 * SCALING_FACTOR, K_I2 * SCALING_FACTOR,
	    K_D2 * SCALING_FACTOR, &pidDataSteering);
  _delay_ms(500);
  initDisplay ();
  printString("      Welcome!",0);
  printString("Press the black button to continue.",2);
  while (!buttonPressed ());
  clearScreen();
  setHighSpeed();

  // Main code here
  PORTK |= 0x01; // Set motor direction to forward.
  sei ();
  uint16_t speed = 1;
  int16_t referenceValue, measurementValue, inputValue, steeringMeasurement,
    steeringInput, steeringReference;
 int16_t target;
  for (;;)
    {
      if (pidTimer)
	{
	  referenceValue = speed;
	  measurementValue = readTacho ();
	  inputValue =
	    pid_Controller (referenceValue, measurementValue, &pidDataMotor);
          if(inputValue < 0) inputValue=0;
	  setMotorPWM (inputValue);
	  printInteger (referenceValue, 1);
	  printInteger (measurementValue, 2);
	  printInteger (inputValue, 3);

	 // steeringReference = 127;
	 // steeringMeasurement = sensorFunction () * 32 - 16;
	 // steeringInput =
	 //   pid_Controller (steeringReference, steeringMeasurement,
	 //       	    &pidDataSteering);
         // if(steeringInput > 128){
         //    steeringInput = 128;
         //    pid_Reset_Integrator(&pidDataSteering);
         // }
         // else if(steeringInput < -128){
         //    steeringInput = -128;
         //    pid_Reset_Integrator(&pidDataSteering);
         // }

	 // setServo (steeringReference - steeringInput);
	 // printInteger (steeringReference, 5);
	 // printInteger (steeringMeasurement, 6);
	 // printInteger (steeringReference - steeringInput, 7);
         


          printInteger(sensorArray[0],9);
          clearArray();
	  pidTimer = FALSE;
	}
      
      if((sensorArray[0] > 10
         && sensorArray[1]>10
         && sensorArray[6]>10
         && sensorArray[7] > 10)){
         setServo(127);
      }
      else setServo(sensorFunction()*32 -16);
      touchScreen();
    }
}
