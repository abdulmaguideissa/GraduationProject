#include <stdint.h>
#include "TM4C123GH6PM.h"
#include "SysTick.h"
#include "CarMotor.h"

// Functions declarations
// PB5(M0PWM3) for driving the back wheels while PE5(M0PWM5) is for steering the front wheels
// LOAD VALUE = period of PWM Output / period of PWM clock
// PWM Clock = 80 MHz / 64 = 1.25 MHz

//PB5 Initialization
void PWM0_Init(void)  
{                   
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

// Declaration for initialization both motors.
void Motor_Init(unsigned long period,float duty_cycle){
    PWM0_Init();
    Straight(period,duty_cycle);
    DIR_Init();
}

// Motor Direction function Initialization.
void DIR_Init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGCGPIO_R |= 0x04;          // activate port C
	delay = SYSCTL_RCGCGPIO_R;
	GPIO_PORTC_AFSEL_R &= ~0x80;        // disable alternate function on PC7
	GPIO_PORTC_PCTL_R &= ~0xF0000000;
	GPIO_PORTC_AMSEL_R &= ~0x80;
	GPIO_PORTC_DIR_R |= 0x80;
	GPIO_PORTC_DEN_R |= 0x80;
}

// Forward move
void Forward(void)
{
	GPIO_PORTC_DATA_R &= ~0x80; //set PC7 to digital "1"
}
// Back move
void Backward(void)
{
	GPIO_PORTC_DATA_R |= 0x80; //set PC7 to digital "0"
}

// Declaration of the car operations
// Straight Forward
// Note: Turn both wheels on.
void Straight(unsigned long Period,float Duty_Cycle)
{
	PWM0_1_CTL_R = 0;
	//PWM0, Generator A  make pwmA "high" in reloading and "low" when matching compA during counting down
    PWM0_1_GENB_R = 0x08C;     //for PB5 (PWM3)
	PWM0_1_LOAD_R = (Period-1)/2;
	PWM0_1_CMPA_R = ((Period-1)/2)*(1-Duty_Cycle);
	PWM0_1_CTL_R |= PWM_0_CTL_ENABLE;
	PWM0_ENABLE_R |= PWM_ENABLE_PWM3EN;
}

// Stand
// Note: Turn Both wheels off.
void Stand(void){
  PWM0_ENABLE_R &= ~(PWM_ENABLE_PWM3EN);  // disable the PWM
}
//=====================================================================

// Initializations of Steering.
//PA2 is output a high signal to the PWM in the motor driver and PE5 (M0PWM5) is used to send PWM signal to the direction pin
void Steering_Init( void ){ 
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
	GPIO_PORTA_DATA_R |= 0x04;             // PA2 high

    GPIO_PORTE_AFSEL_R |= 0x20;           //enable alternate function on PE5
    GPIO_PORTE_PCTL_R &= ~0x00F00000;     //configure PE5 as PWM0
    GPIO_PORTE_PCTL_R |= 0x00400000;
    GPIO_PORTE_AMSEL_R &= ~0x20;          //disable analog functionality on PE5
    GPIO_PORTE_DIR_R |= 0x20;             //Making PE5 output
    GPIO_PORTE_DR8R_R |=0x20;             //Make PE5 drive 8mA
    GPIO_PORTE_DEN_R |= 0x20;             //enable digital I/O on PE5
}

// Moving steering in Right direction by setting the Duty Cycle less than 50%
void Steering_Right(void){        //--> purple wire is connected with the green one
    PWM0_2_CTL_R = 0;
    //PWM0, Generator A  make pwmA "high" in reloading and "low" when matching compA during counting down
    PWM0_2_GENB_R = 0x08C;     //for PE5 (M0PWM5)
    PWM0_2_LOAD_R = 2000-1;
    PWM0_2_CMPA_R = 300-1;       //to make the axis turn clock wise according to the datasheet
    PWM0_2_CTL_R |= PWM_0_CTL_ENABLE;
    PWM0_ENABLE_R |= PWM_ENABLE_PWM5EN;
}

// Moving steering in Left direction by setting the Duty Cycle more than 50%
void Steering_Left(void){     //--> yellow wire is connected with the red one
    PWM0_2_CTL_R = 0;
    //PWM0, Generator A  make pwmA "high" in reloading and "low" when matching compA during counting down
    PWM0_2_GENB_R = 0x08C;     //for PE5 (M0PWM5)
    PWM0_2_LOAD_R = 2000-1;
    PWM0_2_CMPA_R = 1700-1;      //to make the axis turn anti clock wise according to the datasheet
    PWM0_2_CTL_R |= PWM_0_CTL_ENABLE;
    PWM0_ENABLE_R |= PWM_ENABLE_PWM5EN;
}

// Steering in straight motion by setting the Duty Cycle on 50%
void Steering_Straight(void){
    PWM0_2_CTL_R = 0;
    //PWM0, Generator A  make pwmA "high" in reloading and "low" when matching compA during counting down
    PWM0_2_GENB_R = 0x08C;     //for PE5 (M0PWM5)
    PWM0_2_LOAD_R = 2000-1;
    PWM0_2_CMPA_R = 1000-1;      //to make the steering wheels in the steady straight direction according to the datasheet
    PWM0_2_CTL_R |= PWM_0_CTL_ENABLE;
    PWM0_ENABLE_R |= PWM_ENABLE_PWM5EN;
}

