#include <stdint.h>
#include "GPIO_Config.h"

typedef volatile uint32_t* const GPIO_RegAddType;

#define PORTA_BASE_ADDRESS 0x40004000
#define PORTB_BASE_ADDRESS 0x40005000
#define PORTC_BASE_ADDRESS 0x40006000
#define PORTD_BASE_ADDRESS 0x40007000
#define PORTE_BASE_ADDRESS 0x40024000
#define PORTF_BASE_ADDRESS 0x40025000

#define GPIO_PORT_UNLOCK_VALUE 0x4C4F434B

static const uint32_t PortsBaseAddress[6] =
{   PORTA_BASE_ADDRESS,
    PORTB_BASE_ADDRESS,
    PORTC_BASE_ADDRESS,
    PORTD_BASE_ADDRESS,
    PORTE_BASE_ADDRESS,
    PORTF_BASE_ADDRESS
};

#define  GPIO_REG_ADDRESS(CHANNEL_ID,REG_OFFSET) (PortsBaseAddress[CHANNEL_ID] + REG_OFFSET)
#define  RCGCGPIO_REG                      (*((volatile uint32_t *)0x400FE608))
#define  RCGCTIMER_REG                     (*((volatile uint32_t *)0x400FE604))

/*Commit control*/
#define GPIOLOCK_REG(PORT_ID)              *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x520))
#define GPIOCR_REG(PORT_ID)                *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x524))

/*Mode Control*/
#define GPIOAFSEL_REG(PORT_ID)             *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x420))
#define GPIOPCTL_REG(PORT_ID)              *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x52C))

/*Port Control*/
#define GPIODATA_WRITE(DATA,MASK,PORT_ID)  *((GPIO_RegAddType)(GPIO_REG_ADDRESS(PORT_ID,0x000) + (MASK << 2))) = DATA
#define GPIODIR_REG(PORT_ID)               *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x400))

/*Pad control*/
#define GPIODR8R_REG(PORT_ID)           *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x508))
#define GPIODEN_REG(PORT_ID)            *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x51C))
#define GPIOAMSEL_REG(PORT_ID)          *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x528))

/********************************/
void GPIO_init (uint8_t Port_no,uint8_t Pin,uint8_t direction)
{
    volatile uint32_t delay;
    RCGCGPIO_REG |= 1 << Port_no;
    delay = RCGCGPIO_REG;
    GPIOLOCK_REG(Port_no) = GPIO_PORT_UNLOCK_VALUE;
    GPIOCR_REG(Port_no) = 0xff;
    if (direction == 1)
        GPIODIR_REG(Port_no)   |= 1<<Pin;
    else
        GPIODIR_REG(Port_no)   &= ~(1<<Pin);
		
    GPIODR8R_REG(Port_no)  |= 1<<Pin;
	GPIOAFSEL_REG(Port_no) = 0x00;
    GPIOAMSEL_REG(Port_no) = 0x00;
    GPIODEN_REG(Port_no)   = 1<<Pin;
}

void GPIO_write (uint8_t Port_no,uint8_t Pin,uint8_t Data)  //Data in hex as there is masking
{
    GPIODATA_WRITE(Data,1<<Pin,Port_no);
}
