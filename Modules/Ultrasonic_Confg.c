#include <stdint.h>
#include "Ultrasonic_Confg.h"
#include "tm4c123gh6pm.h"
#include "SysTick.h"

//-------------------Important defination for Timer Configuration-----------------

typedef volatile uint32_t* const TIMER_RegAddType;

#define PORTA_BASE_ADDRESS 0x40004000
#define PORTB_BASE_ADDRESS 0x40005000
#define PORTC_BASE_ADDRESS 0x40006000
#define PORTD_BASE_ADDRESS 0x40007000
#define PORTE_BASE_ADDRESS 0x40024000
#define PORTF_BASE_ADDRESS 0x40025000

#define TIMER0_BASE_ADDRESS 0x40030000
#define TIMER1_BASE_ADDRESS 0x40031000
#define TIMER2_BASE_ADDRESS 0x40032000
#define TIMER3_BASE_ADDRESS 0x40033000
#define TIMER4_BASE_ADDRESS 0x40034000

static const uint32_t TimerBaseAddress[5] =
{   TIMER0_BASE_ADDRESS,
    TIMER1_BASE_ADDRESS,
    TIMER2_BASE_ADDRESS,
    TIMER3_BASE_ADDRESS,
    TIMER4_BASE_ADDRESS
};
#define TIMER_REG_ADDRESS(CHANNEL_ID,REG_OFFSET) (TimerBaseAddress[CHANNEL_ID] + REG_OFFSET)
//----------------------------------------------------------------------------------------------//

/*Timer control*/
#define TIMER_CTL_REG(TIMER_ID)           *((TIMER_RegAddType)TIMER_REG_ADDRESS(TIMER_ID,0x00C))
#define TIMER_CFG_REG(TIMER_ID)           *((TIMER_RegAddType)TIMER_REG_ADDRESS(TIMER_ID,0x000))
#define TIMER_TAMR_REG(TIMER_ID)          *((TIMER_RegAddType)TIMER_REG_ADDRESS(TIMER_ID,0x004))
#define TIMER_IMR_REG(TIMER_ID)           *((TIMER_RegAddType)TIMER_REG_ADDRESS(TIMER_ID,0x018))
#define TIMER_ICR_REG(TIMER_ID)           *((TIMER_RegAddType)TIMER_REG_ADDRESS(TIMER_ID,0x024))
#define TIMER_TAR_REG(TIMER_ID)           *((TIMER_RegAddType)TIMER_REG_ADDRESS(TIMER_ID,0x048))
#define TIMER_RIS_REG(TIMER_ID)           *((TIMER_RegAddType)TIMER_REG_ADDRESS(TIMER_ID,0x01C))
/*****************************************************************************************************/
void Sensor1_INIT (void)
{
    GPIO_PORTB_AFSEL_R &= ~0x08;
		GPIO_PORTB_AFSEL_R |= 0x04;           // enable alt funct on PB2
		GPIO_PORTB_PCTL_R &= ~0x0000FF00;     
		GPIO_PORTB_PCTL_R |= 0x00000700;			//timer pB2
		GPIO_PORTB_AMSEL_R &= ~0x0C;          // disable analog functionality on PB2-3
		GPIO_PORTB_DIR_R |= 0x08;							//PB3 trigger
	  GPIO_PORTB_DIR_R &= ~0x04;						//PB2 echo
		GPIO_PORTB_DEN_R |= 0x0C;             // enable digital I/O on PB2-3
}

void Sensor2_INIT (void)
{
    GPIO_PORTB_AFSEL_R &= ~0x80;
		GPIO_PORTB_AFSEL_R |= 0x40;           // enable alt funct on PB6
		GPIO_PORTB_PCTL_R &= ~0xFF000000;     
		GPIO_PORTB_PCTL_R |= 0x07000000;			//timer PB6
		GPIO_PORTB_AMSEL_R &= ~0xC0;          //disable analog functionality on PB6-7
		GPIO_PORTB_DIR_R |= 0x80;							//PB7 trigger
	  GPIO_PORTB_DIR_R &= ~0x40;						//PB6 echo
		GPIO_PORTB_DEN_R |= 0xC0;             //enable digital I/O on PB6-7
}


void Sensor3_INIT (void)   //Sensor3 uses (echo->PF2 , trigger->PF0) Timer1
{ 
    GPIO_PORTF_LOCK_R = 0x4C4F434B;       // 2) unlock PortF PF0	
		GPIO_PORTF_AFSEL_R &= ~0x01;         
		GPIO_PORTF_AFSEL_R |= 0x04;           // enable alt funct on PF2
		GPIO_PORTF_PCTL_R &= ~0x00000F0F;     
		GPIO_PORTF_PCTL_R |= 0x00000700;			//timer 
		GPIO_PORTF_AMSEL_R &= ~0x05;          // disable analog functionality on PF0 PF2
		GPIO_PORTF_DIR_R |= 0x01;							//PF0 trigger
	  GPIO_PORTF_DIR_R &= ~0x04;						//PF2 echo
		GPIO_PORTF_DEN_R |= 0x05;             // enable digital I/O on PF0, PF2
}


void Sensor4_INIT (void)   //Sensor4 uses (echo->PF4 , trigger->PF1) Timer2
{ 
		GPIO_PORTF_AFSEL_R &= ~0x02;					//Disable alt funct on PF1
		GPIO_PORTF_AFSEL_R |= 0x10;           // enable alt funct on PF4
		GPIO_PORTF_PCTL_R &= ~0x000F00F0;     
		GPIO_PORTF_PCTL_R |= 0x00070000;			//timer PF4
		GPIO_PORTF_AMSEL_R &= ~0x12;          //disable analog functionality on PF1, PF4
		GPIO_PORTF_DIR_R |= 0x02;							//PF1 trigger
	  GPIO_PORTF_DIR_R &= ~0x10;						//PF4 echo
		GPIO_PORTF_DEN_R |= 0x12;             // enable digital I/O on PF1,PF4
}

void SENSOR_Init (void)
{
	 Sensor1_INIT ();  //Sensor1
	 Sensor2_INIT ();  //Sensor2 
	 Sensor3_INIT ();  //Sensor3 
	 Sensor4_INIT ();  //Sensor4 
}

void Ultrasonic_Timer_Init(uint8_t TimerA_no)
{
    SYSCTL_RCGCTIMER_R |=(1<<TimerA_no);
    //GPIOPCTL_REG(Port_no)  |= 7<<(Pin_Echo_no*4);
    TIMER_CTL_REG(TimerA_no)  = 0x00000000;     //Disable TimerA
    TIMER_CFG_REG(TimerA_no)  = 0x04;     //Configure TIMER   For a 16/32-bit timer, this value selects the 16-bit timer configuration.
    TIMER_TAMR_REG(TimerA_no) = 0x17;     //Capture_MODE + COUNT_UP + Edge Time Mode
    TIMER_CTL_REG(TimerA_no)  = 0x0C;     //choose Both Edges
    TIMER_CTL_REG(TimerA_no)  |= (1<<0);  //Enable TimerA
}

uint32_t Distance_Sensor1 (void)   //Sensor1 uses (echo->PB2 , trigger->PB3) Timer3
{
    uint32_t highEdge,lowEdge;
    uint32_t timediff;
    uint32_t result;
    const double temp = 1.0/80.0;


    TIMER3_TAV_R = 0x00000000;         //making The current counting reg=0

    GPIO_PORTB_DATA_R &= ~(1<<3);      //off the tirgger pin B3
    SysTick_Wait10us(2);
    GPIO_PORTB_DATA_R |= (1<<3);       //on the trigger pin B3
    SysTick_Wait10us(2);
    GPIO_PORTB_DATA_R &= ~(1<<3);      //off the tirgger pin B3
    TIMER3_ICR_R = 0x04;
    while ((TIMER3_RIS_R & 4) == 0){}; //Wait till capturing the rising edge.
    highEdge = TIMER3_TAR_R;

    TIMER3_ICR_R = 0x04;
    while ((TIMER3_RIS_R & 4) == 0){}; //Wait till capturing the falling edge close the timer
    lowEdge = TIMER3_TAR_R;
    timediff = lowEdge - highEdge;     //subtracting the time of the rising and falling edge to get the time of the whole signal
    result = (timediff * temp)/58;

    return result;
}
uint32_t Distance_Sensor2 (void)  //Sensor2 uses (echo->PB6 , trigger->PB7) Timer0
{
    uint32_t highEdge,lowEdge;
    uint32_t timediff;
    uint32_t result;
    const double temp = 1.0/80.0;

    TIMER0_TAV_R = 0x00000000;         //making The current counting reg=0

    GPIO_PORTB_DATA_R &= ~(1<<7);      //off the tirgger pin B7
    SysTick_Wait10us(2);
    GPIO_PORTB_DATA_R |= (1<<7);       //on the trigger pin B7
    SysTick_Wait10us(2);
    GPIO_PORTB_DATA_R &= ~(1<<7);      //off the tirgger pin B7

    TIMER0_ICR_R = 0x04;
    while ((TIMER0_RIS_R & 4) == 0){}; //Wait till capturing the rising edge.
    highEdge = TIMER0_TAR_R;

    TIMER0_ICR_R = 0x04;
    while ((TIMER0_RIS_R & 4) == 0){}; //Wait till capturing the falling edge.
    lowEdge = TIMER0_TAR_R;

    timediff = lowEdge - highEdge;     //subtracting the time of the rising and falling edge to get the time of the whole signal
    result = (timediff * temp)/58;

    return result;
}

uint32_t Distance_Sensor3 (void)   //Sensor3 uses (echo->PF2 , trigger->PF3) Timer1
{
    uint32_t highEdge,lowEdge;
    uint32_t timediff;
    uint32_t result;
    const double temp = 1.0/80.0;

    TIMER1_TAV_R = 0x00000000;         //making The current counting reg=0

    GPIO_PORTF_DATA_R &= ~(1<<3);      //off the tirgger pin F3
    SysTick_Wait10us(2);
    GPIO_PORTF_DATA_R |= (1<<3);       //on the trigger pin F3
    SysTick_Wait10us(2);
    GPIO_PORTF_DATA_R &= ~(1<<3);      //off the tirgger pin F3

    TIMER1_ICR_R = 0x04;
    while ((TIMER1_RIS_R & 4) == 0){}; //Wait till capturing the rising edge.
    highEdge = TIMER1_TAR_R;

    TIMER1_ICR_R = 0x04;
    while ((TIMER1_RIS_R & 4) == 0){}; //Wait till capturing the falling edge.
    lowEdge = TIMER1_TAR_R;

    timediff = lowEdge - highEdge;     //subtracting the time of the rising and falling edge to get the time of the whole signal
    result = (timediff * temp)/58;

    return result;
}

uint32_t Distance_Sensor4 (void)   //Sensor4 uses (echo->PF4 , trigger->PF1) Timer2
{
    uint32_t highEdge,lowEdge;
    uint32_t timediff;
    uint32_t result;
    const double temp = 1.0/80.0;

    TIMER2_TAV_R = 0x00000000;         //making The current counting reg=0

    GPIO_PORTF_DATA_R &= ~(1<<1);      //off the tirgger pin F1
    SysTick_Wait10us(2);
    GPIO_PORTF_DATA_R |= (1<<1);       //on the trigger pin F1
    SysTick_Wait10us(2);
    GPIO_PORTF_DATA_R &= ~(1<<1);      //off the tirgger pin F1

    TIMER2_ICR_R = 0x04;
    while ((TIMER2_RIS_R & 4) == 0){}; //Wait till capturing the rising edge.
    highEdge = TIMER2_TAR_R;

    TIMER2_ICR_R = 0x04;
    while ((TIMER2_RIS_R & 4) == 0){}; //Wait till capturing the falling edge.
    lowEdge = TIMER2_TAR_R;

    timediff = lowEdge - highEdge;     //subtracting the time of the rising and falling edge to get the time of the whole signal
    result = (timediff * temp)/58;

    return result;
}

