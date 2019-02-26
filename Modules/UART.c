#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "UART.h"

//----------------------------------------------------------------------------------
void UART_Init(void)   //initializing the UART with 9600 Baudrate
{
    //for port A for UART (PA0,PA1)
        SYSCTL_RCGCGPIO_R |= 1<<0;
        GPIO_PORTA_LOCK_R = 0x4C4F434B;   //Unlock the lock register for PA0 & PA1
        GPIO_PORTA_CR_R = 0xff;
        GPIO_PORTA_AMSEL_R = 0x00;
        GPIO_PORTA_AFSEL_R = 0x03;
        GPIO_PORTA_PCTL_R = 0x11;         //choose UART for PA0,PA1
        GPIO_PORTA_DR8R_R = 0x11;
        GPIO_PORTA_DIR_R = 0x00;
        GPIO_PORTA_DEN_R = 0x03;

        SYSCTL_RCGCUART_R |= (1<<0);        //Enable the UART module
        GPIO_PORTA_PCTL_R = 0x11;
        UART0_CTL_R= 0x00;                  //Disable the UART by clearing the UARTEN bit in the UARTCTL register
        UART0_IBRD_R=0x208;                 //Adding the integar part of the Baudrate divisor
                                            //IBRD = int(80,000,000 / (16 * 9600)) = int(520.8333333)=0x208
        UART0_FBRD_R=0x35;                  //Adding the fraction part of the Baudrate divisor
                                            // FBRD = round(0.833333333 * 64) =53=0x35
        UART0_LCRH_R= 0x60;                 //Determining the length of the frame
        UART0_CC_R= 0x00;                   //Make UART based on system clock and divisor factor
        UART0_CTL_R= 0x301;                  //Enable the UART again
}

//----------------------------------------------------------------------------------
void printChar(char c)
{
    while((UART0_FR_R & (1<<5)) != 0);
    UART0_DR_R = c;
}

void printString(char * string)
{
  while(*string)
  {
    printChar(*(string++));
  }
}
