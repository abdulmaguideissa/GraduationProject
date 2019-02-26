#ifndef ULTRASONIC_CONFG_H_
#define ULTRASONIC_CONFG_H_

#include <stdint.h>
/*----------------------------------------------------------------------------------*
 Steps to use the driver:
 -------------------------
 1) Include files:
             #include "PLL_Confg.h"
             #include "Ultrasonic_Confg.h"

 2) Initialize the needed function 1st:
        PLL_Init();                       //intialize freq 80Mhz
        SysTick_Init();                   //intialize systick timer for accurate delay
        Ultrasonic_Gpio_Init(1,6,7);      // Enable port(B),PIN_ECHO(6) , PIN_tigger(7) as an example
        Ultrasonic_Timer_Init(1,0);     // Enable using Port(B) and PIN_ECHO(6) TimerA(3)

3) Add the return value of the function --> "variable=uint32_t Distance_Sensor2();"
   using Port(B) , PIN_Tigger(5) and TimerA(0)
*-----------------------------------------------------------------------------------------------------*/
/*   Availabe sensors:  --> Sensor1 uses (echo->PB2 , trigger->PB3) Timer3
                        --> Sensor2 uses (echo->PB6 , trigger->PB7) Timer0
                        --> Sensor3 uses (echo->PF2 , trigger->PF3) Timer1
                        --> Sensor4 uses (echo->PF4 , trigger->PF1) Timer2
*/


// Function to inialize the GPIO for the pin connected to "Trigger" and "Echo"
// Input: Port_name (A-F),Pin connected to Trigger,Pin connected to Echo
void SENSOR_Init (void);

// Function to inialize the GPTM for the pin connected to "Echo"
// Input:(0-1-2-3) which is the Number of TimerAn corresponding to each sensor
void Ultrasonic_Timer_Init(uint8_t TimerA_no);

// Function to get the accurate distance according to each sensor.
uint32_t Distance_Sensor1 (void);
uint32_t Distance_Sensor2 (void);
uint32_t Distance_Sensor3 (void);
uint32_t Distance_Sensor4 (void);

#endif /* ULTRASONIC_CONFG_H_ */
