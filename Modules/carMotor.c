// caMotor.c
// Declaring functions of motors.
// Creating PWM using PWM Device to control motors speed,
// right motor, left motor and both together.
// Graduation Project, ALEX UNIVERSITY.
// 11 Oct, 2017

// Preprocessor Directves.
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "carMotor.h"

// Functions declarations
// Declaring PB4-5 to generate PWM
// LOAD VALUE = period of PWM Output / period of PWM clock
// PWM Clock = 80 MHz / 64 = 1.25 MHz
// INPUT: none
// OUTPUT: none

void PWM0_Init( void ){
  volatile uint32_t delay;		          //volatile unsigned long delay;
  SYSCTL_RCGCPWM_R |= 0x01;             //activate PWM0
  delay=SYSCTL_RCGCPWM_R;
  SYSCTL_RCGCGPIO_R |= 0x02;       	    //activate port B
  delay = SYSCTL_RCGCGPIO_R;            //allow time to finish activating

  GPIO_PORTB_AFSEL_R |= 0x20;           //enable alternate function on PB5
  GPIO_PORTB_PCTL_R &= ~0x00F00000;     //configure PB5 as PWM0
  GPIO_PORTB_PCTL_R |= 0x00400000;
  GPIO_PORTB_AMSEL_R &= ~0x20;          //disable analog functionality on PB5
  GPIO_PORTB_DIR_R |= 0x20;             //Making PB5 output
  GPIO_PORTB_DR8R_R |=0x20;             //Make PB5 drive 8mA
  GPIO_PORTB_DEN_R |= 0x20;             //enable digital I/O on PB5

  SYSCTL_RCC_R |= SYSCTL_RCC_USEPWMDIV; //use PWM divider
  SYSCTL_RCC_R &= ~SYSCTL_RCC_PWMDIV_M; //clear PWM divider field
  SYSCTL_RCC_R |= SYSCTL_RCC_PWMDIV_64; //configure for PWM CLock=(Freq Osc./64) (64 divider)
}


// Motor Direction function Initialization.
// INPUT: none
// OUTPUT: none
void DIR_Init( void ){
	volatile unsigned long delay;
	SYSCTL_RCGCGPIO_R |= 0x04;          // activate port C
	delay = SYSCTL_RCGCGPIO_R;
	GPIO_PORTC_AFSEL_R &= ~0x80;        // disable alt func on PC7
	GPIO_PORTC_PCTL_R &= ~(0xF0000000);
	GPIO_PORTC_AMSEL_R &= ~0x80;
	GPIO_PORTC_DIR_R |= 0x80;
	GPIO_PORTC_DEN_R |= 0x80;
}

// Forward move
// INPUT: none
// OUTPUT: none
void DCMOTOR_Forward( void ){
	GPIO_PORTC_DATA_R &= ~0x80; //set PC7 to digital "1"
}

// Back move
// INPUT: none
// OUTPUT: none
void DCMOTOR_Backward( void ){
	GPIO_PORTC_DATA_R |= 0x80; //set PC7 to digital "0"
}

// Declaration of the car operations
// Straight Forward
// Note: Turn both wheels on.
void DCMOTOR_Straight( unsigned long Period, float Duty_Cycle ){
	PWM0_1_CTL_R = 0;
	//PWM0, Generator A  make pwmA "high" in reloading and "low" when matching compA during counting down
  PWM0_1_GENB_R = 0x08C;     //for PB5 (PWM3)
	PWM0_1_LOAD_R = (Period - 1 ) / 2;
	PWM0_1_CMPA_R = ((Period - 1) / 2) * (1 - Duty_Cycle);
	PWM0_1_CTL_R |= PWM_0_CTL_ENABLE;
	PWM0_ENABLE_R |= PWM_ENABLE_PWM3EN;
	DCMOTOR_Forward();
}

// Go Reverse (period==Load count , Duty cycle) --> recommended Dc in range (0.2-0.05)
// INPUT: Total period and Duty Cycle
// OUTPUT: none
void DCMOTOR_Reverse( unsigned long Period, float Duty_Cycle ){
	PWM0_1_CTL_R = 0;
	//PWM0, Generator A  make pwmA "high" in reloading and "low" when matching compA during counting down
  PWM0_1_GENB_R = 0x08C;     //for PB5 (PWM3)
	PWM0_1_LOAD_R = (Period - 1 ) / 2;
	PWM0_1_CMPA_R = ((Period - 1) / 2) * (1 - Duty_Cycle);
	PWM0_1_CTL_R |= PWM_0_CTL_ENABLE;
	PWM0_ENABLE_R |= PWM_ENABLE_PWM3EN;
	DCMOTOR_Backward();
}
// Stand
// Note: Turn Both wheels off.
// INPUT: none
// OUTPUT: none
void DCMOTOR_Stand( void ){
  PWM0_ENABLE_R &= ~(PWM_ENABLE_PWM3EN);  // disable the PWM
}

// Declaration for initializing both motors.
// INPUT: 
// OUTPUT: none
void MOTOR_Init( void ){
    PWM0_Init();
    DIR_Init();
	  DCMOTOR_Forward();
}
