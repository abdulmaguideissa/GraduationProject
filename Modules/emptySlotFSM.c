// ********* "Autonomous driving Robot prototype" ********.
// emtySlotFSM.c
// Moore State Machine Design for Auto finding empty .. 
// slot for car parking.
// Runs on TM4C123, ARM processor.
// October 5, 2017.
// Graduation Project, Communications and Electronics Dept, ALEX UNIVERSITY.
// 	 SPS TEAM.
//    LEFT                              RIGHT
//|-------------                   ----------------|
//| slot (1)        <<    ^    >>      slot (2)    |
//|-------------          ^        ----------------|
//| slot (3)        <<    ^    >>      slot (4)    |
//|-------------          ^        ----------------| 
//| slot (5)        <<    ^    >>      slot (6)    |
//|-------------          |        ----------------|
//               ENTRANCE OF GARAGE 



// 1) Preprocessor Directives.
#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "SysTick.h"
#include "carMotor.h"
#include "Ultrasonic_Confg.h"
#include "ParkingCount.h"
#include "Bluetooth_UART.h"
#include "Steering.h"
#include "FSM_Server.h"

// *********************************
// 2) Prototype Definitions Section. 
void DisableInterrupts( void );
void EnableInterrupts( void );

// *******************************
// 3) Globals and Initializations 
unsigned long Input;
unsigned char data;
volatile unsigned long count = 0;
unsigned long START_Flag = 0;

// *************************************************************
// Structure definition of Slot Detection FSM using lookup table.
// Slot Detection Algorithm is to be executed first.
struct SlotDetection {
	void(*Task1)( void );                   // Function pointer to choose the apropriate operation.             
	unsigned long Delay;               // Time delay between states.
	unsigned long Next[2];   // Pointer to the next states.
};

typedef const struct SlotDetection S_Detection;
#define    STRAIGHT_IR            0             // Straight motion dc motor, state0 index.
#define    STOP_YIELD             1             // Stop and set yield flag to the next FSM, state1 index.

// Lookup table for most likely states to find the robot.
// 1.First field is a function pointer to a task to be executed
//  by the robot.
// 2.Second field is a proper delay according to state.
// 3.Third field is linked list pointer to the next state.
S_Detection FSM_1[2] = {
	{ &IR_DCStraight, 50, {STRAIGHT_IR, STOP_YIELD}},                      // State0 
	{ &Stop_Yield,    10, {STOP_YIELD,  STOP_YIELD}} };                        // State1
                                                                         
	
//onst struct SlotDetection *pSlotState;              // Linked list pointer to the next state.
// Declarations
void Robot_Adjustment( void );
void Robot_Ready( void );
void Robot_Park( void );
void Steering_Disable( void );
// *******************************************
// --------- UART1 interrupt Handler ---------
// INPUT: none
// OUTPUT: none
// NOTE: Deliver the received data from DR in UART in case of RX INT.
//       Set flag to initiate the process.
void UART1_Handler( void ){
	if(UART1_RIS_R&UART_RIS_RXRIS){
		UART1_ICR_R = UART_ICR_RXIC;       // Acknowledge RX FIFO.
    data = UART_InChar();              // Get received data
    START_Flag = 1;		                 // Set start flag 
	}
	
	if(UART1_RIS_R&UART_RIS_RTRIS){
		UART1_ICR_R = UART_ICR_RTIC;      // Acknowledge Receiver timeout.
		data = UART_InChar();             // Get received data
		START_Flag = 1;                   // Set start flag 
	}
}
// System operation indicator. 
#define    HEARTBEAT        (*((volatile unsigned long *)0x40025020))  // PF3 bit specific-address


// **********************************
// ********* MAIN Function **********
//unsigned long cState;
unsigned long s;
unsigned long Flag;
int main( void ) {
	DisableInterrupts();
	PLL_Init();                            // Initialize the system clock.
	SysTick_Init();                        // Initialize the system tick for accurate delays.     
	MOTOR_Init();                          // Initiate PWM of DC motors.
  BLUETOOTH_Init();                      // Initiate UART1 for Bluetooth. 
  STEERING_Init();                       // Initiate Steering mechanism.
	SLOTCOUNTER_Init();	                   // Initiate IR sensor.
	HEARTBEAT_Init();                      // System Operability.
	SENSOR_Init();                         // Initiate Ultrasonic sensors.
	//Ultrasonic_Timer_Init(3);
	EnableInterrupts();
	s = STRAIGHT_IR;
	//pSlotState = STRAIGHT_IR;                     // Initial state 
	while( 1 )
	{  
		if( START_Flag == 1 ){           			// Busy wait, Interrupt triggers the parking process. 
      HEARTBEAT = 1;			
			(FSM_1[s].Task1)();                                      // 1.Initial Start.
			SysTick_Wait10ms(FSM_1[s].Delay);                   // 2.Delay.
			Input = INPUT_Server_1(data);                              // 3.Get input index.
			s = FSM_1[s].Next[Input];                      // 4.Go to the next state.
		}
		if( YIELD_Flag ){
			HEARTBEAT = 0;
			
			DCMOTOR_Straight(4000, 0.18);
			SysTick_Wait10ms(50);
			DCMOTOR_Reverse(4000, 0.1);
			Steering_Right();
			
			if( Distance_Sensor4() <= 4){
				Robot_Adjustment();
			}
			
			if( Flag && ( Distance_Sensor1() <= 40 || Distance_Sensor1() >= 5 )){
				Robot_Ready();
				if (Distance_Sensor1() <= 5)
					Robot_Park();
			}
			
			if ( !Flag ){
				Robot_Adjustment();
			}			
				
		}
	}
}
// --------------------------------
// Definitions
void Steering_Disable( void ){
 GPIO_PORTA_DATA_R &= ~0x04;	
}

void Robot_Adjustment( void ){
	Flag = 0;
	DCMOTOR_Stand();
	SysTick_Wait10ms(100);
	Steering_Left();
	DCMOTOR_Straight(4000, 0.1);
	SysTick_Wait10ms(100);
	DCMOTOR_Stand();
	
	if(Distance_Sensor2() == Distance_Sensor3())
   	Flag = 1;
}

// ------------------------
void Robot_Ready( void ){
	Steering_Straight();
	DCMOTOR_Reverse(4000, 0.1);
}

// -------------------------
void Robot_Park( void ){
	DCMOTOR_Stand();
	Steering_Disable();
}
