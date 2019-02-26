// Bluetooth_UART.c
// Runs on TM4C123 or LM4F120
// Running bluetooth device based on UART serial Communication.
// As part of AUTO PARKING ROBOT using IoT.
// Data is being sent from mobile app to the uController.
// uController counts the slots number based on the sent data, then starts to park.
// ALEXANDRIA UNIVERSITY, ELECREICAL ENGINEERING DEPARTMENT.

// U1Rx (PC4) connected to serial port on PC
// U1Tx (PC5) connected to serial port on PC
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Bluetooth_UART.h"
#define  NVIC_EN0_INT6        0x00000040 
#define  NVIC_PRI1_INTC       0x00200000 

//------------UART_Init------------
// Initialize the UART for appropriate baud rate (assuming 80 MHz UART clock),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// Input: none
// Output: none
// NOTE: Use interrupts in case of receiving data for effeciency.
//     PC5,PC4  UART Module 1.

void BLUETOOTH_Init(void){
	volatile unsigned long delay;
  SYSCTL_RCGCUART_R |= 0x02;              // activate UART1 clock
	delay = SYSCTL_RCGCUART_R;
  SYSCTL_RCGCGPIO_R |= 0x04;               // activate port C clock
	delay = SYSCTL_RCGCGPIO_R;
	GPIO_PORTC_CR_R |= 0x30;
	GPIO_PORTC_AMSEL_R &= ~0x30;          // disable analog functionality on PC4-5
	GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF);
	GPIO_PORTC_PCTL_R |= 0x00220000;      // Enable UART Module 1 on PC4-5.     
	GPIO_PORTC_AFSEL_R |= 0x30;           // Enable alt funct on PC4-5.
	//GPIO_PORTC_DIR_R |= 0x20;
	//GPIO_PORTC_DIR_R &= ~0x40;            // PC4 Input and PC5 output.
  GPIO_PORTC_DEN_R |= 0x30;             // enable digital I/O on PC4-5.
	
  UART1_CTL_R &= ~UART_CTL_UARTEN;      // disable UART.
  UART1_IBRD_R = 520;                    // i.e. IBRD = int(80,000,000 / (16 * 115200)) = int(43.402778)
  UART1_FBRD_R = 53;                    //       FBRD = round(0.402778 * 64) = 26, here we used 9600 baud rate.
                                  	// 8 bit word length (no parity bits, one stop bit, FIFOs)
  UART1_LCRH_R |= (UART_LCRH_WLEN_8|UART_LCRH_FEN);
	UART1_CC_R &= ~0x0F;                  // Configure for main system clock source.
	UART1_IFLS_R &= ~0x3F;
	UART1_IFLS_R |= UART_IFLS_RX1_8;      // Use interrupt FIFO level select in receiving >= 1/8 full.
  UART1_ICR_R |= (UART_ICR_RXIC|UART_ICR_RTIC);             // 1. Clear RX interrupt bit int IM & RIS registers 
  UART1_IM_R |= (UART_IM_RXIM|UART_IM_RTIM);                // 2. Send the interrupt to interrupt controller if RXRIS is set.
	UART1_CTL_R |= UART_CTL_UARTEN;                           // Enable UART
  NVIC_PRI1_R |=(NVIC_PRI1_R&0xF00FFFFF)|NVIC_PRI1_INTC;    // 3. Set the corresponding priority to PRI register, priority (21:23)       
  NVIC_EN0_R |= NVIC_EN0_INT6;                              // 4. Enable interrupt 6 in NVIC	
}

//------------UART_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
unsigned char UART_InChar(void){
  while((UART1_FR_R&UART_FR_RXFE) != 0);
  return((unsigned char)(UART1_DR_R&0xFF));
}

//------------UART_InCharNonBlocking------------
// Get oldest serial port input and return immediately
// if there is no data.
// Input: none
// Output: ASCII code for key typed or 0 if no character
unsigned char UART_InCharNonBlocking(void){
  if((UART1_FR_R&UART_FR_RXFE) == 0){
    return((unsigned char)(UART1_DR_R&0xFF));
  } else{
    return 0;
  }
}

//------------UART_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART_OutChar(unsigned char data){
  while((UART1_FR_R&UART_FR_TXFF) != 0);
  UART1_DR_R = data;
}


//------------UART_InString------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until <enter> is typed
//    or until max length of the string is reached.
// It echoes each character as it is inputted.
// If a backspace is inputted, the string is modified
//    and the backspace is echoed
// terminates the string with a null character
// uses busy-waiting synchronization on RDRF
// Input: pointer to empty buffer, size of buffer
// Output: Null terminated string
// -- Modified by Agustinus Darmawan + Mingjie Qiu --
void UART_InString( char *bufPt, uint16_t max ) {
int length = 0;
char character;
  character = UART_InChar();
  while(character != CR){
    if(character == BS){
      if(length){
        bufPt--;
        length--;
        UART_OutChar(BS);
      }
    }
    else if(length < max){
      *bufPt = character;
      bufPt++;
      length++;
      UART_OutChar(character);
    }
    character = UART_InChar();
  }
  *bufPt = 0;
}

//------------UART_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART_OutString(unsigned char *buffer){
	while(*buffer){
		UART_OutChar(*buffer++);
	}
}




