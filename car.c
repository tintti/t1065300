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
#define K_I 1.0
#define K_D 1.0

#define PID_INTERVAL 200 
// Steering PID
#define K_P2 0.3
#define K_I2 0.0
#define K_D2 0.2


volatile uint8_t pidTimer = FALSE;

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
    uint8_t largest = 5;
    uint8_t index = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        if (sensorArray[i] > largest)
        {
            largest = sensorArray[i];
            index = i+1;
	}
    }
    return index;
}


void clearArray(void){
   for(uint8_t i = 0; i<8;i++){
      sensorArray[i] = 0;
   }
}

int main (void)
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
  uint16_t speed = 4;
  int16_t referenceValue, measurementValue, inputValue, steeringMeasurement,
    steeringInput, steeringReference;
 int16_t target;
 uint8_t laps = 0;
 uint8_t onFinishLine = FALSE;
  for (;;)
    {
      if (pidTimer)
	{
	  referenceValue = speed;
	  measurementValue = readTacho ();
	  inputValue = pid_Controller (referenceValue, measurementValue, &pidDataMotor);
          if(inputValue < 0) inputValue=0;
	  setMotorPWM (inputValue+70);
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
	  pidTimer = FALSE;
	}
      static uint8_t avg_counter;
      if(avg_counter != 10){
          avg_counter++;
      }
      else{
          clearArray();
          avg_counter = 0;
      }
      
      if((sensorArray[0] > 10
         && sensorArray[1]>10
         && sensorArray[6]>10
         && sensorArray[7] > 10)){
         setServo(127);
         onFinishLine = TRUE;
      }
      else {
          if(onFinishLine){
              printInteger(laps++,10);
              onFinishLine = FALSE;
          }
      }


      uint8_t s = sensorFunction();
      if(s == 1 || s == 8) speed = 1;
      else if(s == 4 || s == 5) speed = 6;
      else if((s == 3 || s == 6) && speed  >3) speed = 2;
      else if(s != 0) speed = 3;

      if(s != 0)
          setServo(s*32 -16);

    }
}
