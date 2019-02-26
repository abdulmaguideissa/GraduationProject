// Steering.c
// Runs on TM4C123x
// Use PC5 creating control pulses for steering mechanism motion.
// Grad Project, ELECTRICAL ENGINEERING DEPT, ALEXANDRIA UNIVERSITY
// June 20, 2018.

 
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Steering.h"

//               STEERING MOTION
// --------------------------------------------------
//    RED WIRE PULSE |  BLACK WIRE PULSE |  MOVE
// --------------------------------------|-----------
//          1        |        0          |   RIGHT
//          0        |        1          |   LEFT
// otherwise, straight motion.         
// --------------------------------------------------

// Functions Definitions.
// Initializations of Steering.
// INPUT: none
// OUTPUT: none
// PA2-3 work as pulse output generating 0 and 1 to control steering mechanism 
void STEERING_Init( void ){ 
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= (1<<0|1<<4);       // Activate clock for Port A and E
  delay = SYSCTL_RCGCGPIO_R;	           // Wait to finish activating.
  SYSCTL_RCGCPWM_R |= 0x01;              //activate PWM0
  delay=SYSCTL_RCGCPWM_R;
  GPIO_PORTA_PCTL_R &= ~0xFFFFFFFF;      // Clear for control register for GPIO
	GPIO_PORTA_AFSEL_R &= ~0x04;           // Disable alternative functions for PA2
	GPIO_PORTA_AMSEL_R &= ~0x04;           // Disable analog function on PA2
	GPIO_PORTA_DIR_R |= 0x04;              // Set PA2 as outputs
	GPIO_PORTA_DR8R_R |= 0x04;             // PA2 drives 8mA
	GPIO_PORTA_DEN_R |= 0x04;              // Enable digital function on PA2
	

  GPIO_PORTE_AFSEL_R |= 0x20;           //enable alternate function on PE5
  GPIO_PORTE_PCTL_R &= ~0x00F00000;     //configure PE5 as PWM0
  GPIO_PORTE_PCTL_R |= 0x00400000;
  GPIO_PORTE_AMSEL_R &= ~0x20;          //disable analog functionality on PE5
  GPIO_PORTE_DIR_R |= 0x20;             //Making PE5 output
  GPIO_PORTE_DR8R_R |=0x20;             //Make PE5 drive 8mA
  GPIO_PORTE_DEN_R |= 0x20;             //enable digital I/O on PE5
}

// Moving steering in Right direction
// INPUT: none
// OUTPUT: none
// NOTE: set PA2 and clear PA3 for right operation
//   ______
// _|  PA2 |___PA3__________

void Steering_Right( void ){
	GPIO_PORTA_DATA_R |= 0x04;             // PA2 high
  PWM0_2_CTL_R = 0;
    //PWM0, Generator A  make pwmA "high" in reloading and "low" when matching compA during counting down
  PWM0_2_GENB_R = 0x08C;     //for PE5 (M0PWM5)
  PWM0_2_LOAD_R = 2000-1;
  PWM0_2_CMPA_R = 100-1;       //to make the axis turn clock wise according to the datasheet
  PWM0_2_CTL_R |= PWM_0_CTL_ENABLE;
  PWM0_ENABLE_R |= PWM_ENABLE_PWM5EN;
}

// Moving steering in Left direction.
// INPUT: none
// OUTPUT: none
// NOTE: set PA3 and clear PA2 for left operation
//                  ______
// ________PA2_____| PA3  |_

void Steering_Left( void ){
 GPIO_PORTA_DATA_R |= 0x04;             // PA2 high
 PWM0_2_CTL_R = 0;
 //PWM0, Generator A  make pwmA "high" in reloading and "low" when matching compA during counting down
 PWM0_2_GENB_R = 0x08C;     //for PE5 (M0PWM5)
 PWM0_2_LOAD_R = 2000 - 1;
 PWM0_2_CMPA_R = 1800 - 1;      //to make the axis turn anti clock wise according to the datasheet
 PWM0_2_CTL_R |= PWM_0_CTL_ENABLE;
 PWM0_ENABLE_R |= PWM_ENABLE_PWM5EN;
}

// Steering in straight motion.
// INPUT: none
// OUTPUT: none
// NOTE: set/clear both PA2-3
void Steering_Straight( void ){
 GPIO_PORTA_DATA_R |= 0x04;             // PA2 high
 PWM0_2_CTL_R = 0;
 //PWM0, Generator A  make pwmA "high" in reloading and "low" when matching compA during counting down
 PWM0_2_GENB_R = 0x08C;     //for PE5 (M0PWM5)
 PWM0_2_LOAD_R = 2000-1;
 PWM0_2_CMPA_R = 1000-1;      //to make the steering wheels in the steady straight direction according to the datasheet
 PWM0_2_CTL_R |= PWM_0_CTL_ENABLE;
 PWM0_ENABLE_R |= PWM_ENABLE_PWM5EN;
}
