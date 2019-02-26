#include <stdint.h>
#include "ParkingCount.h"
#include "tm4c123gh6pm.h"
#include "SysTick.h"
unsigned long flag = 0;
unsigned long SW;


void SLOTCOUNTER_Init ( void ){
 volatile unsigned long delay;
 SYSCTL_RCGCGPIO_R |= (1<<4);          //E0
 delay = SYSCTL_RCGCGPIO_R;
 //INPUT at E
 GPIO_PORTE_CR_R |= 0x01;
 GPIO_PORTE_AFSEL_R &= ~0x01;
 GPIO_PORTE_AMSEL_R &= ~0x01;
 GPIO_PORTE_PCTL_R &= ~0x0000000F;
 GPIO_PORTE_DIR_R &= ~0x01;
 GPIO_PORTE_DEN_R |= 0x01;
}

// INPUT: Global counter 
// OUTPUT: couting value
unsigned char SlotCounter ( volatile unsigned char x ){
  SW = GPIO_PORTE_DATA_R&0x01;
  if (SW == 0) {
   if (flag == 1) {
    x++;
    SysTick_Wait10ms(1);
    flag = 0;
   }
  }
  else {	
   SysTick_Wait10ms(1);
   flag = 1;
  }
  return x;
}

