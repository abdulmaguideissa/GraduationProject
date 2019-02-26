// carMotor.h
// DC Motor Driver header file.
#ifndef _carMotor_H_
#define _carMotor_H_ 

// Initialize the motor with the entire configurations (period==Load count , Duty cycle) --> recommended DC in range (0.2-0.05)
// INPUT: Total period and Duty Cycle
// OUTPUT: none
void MOTOR_Init( void );

// Initialize Pulse Width Modulation module 0 with the required period and duty cycle
// INPUT: none
// OUTPUT: none
void PWM0_Init(void);

// Initialize the direction pin of the motor
// INPUT: none
// OUTPUT: none
void DIR_Init(void);

// Prototype to run on cases,
// each case is captured from state machine from main
// Prototypes for state machine.

// Go Straight forward (period==Load count , Duty cycle) --> recommended Dc in range (0.2-0.05)
// INPUT: Total period and Duty Cycle
// OUTPUT: none
void DCMOTOR_Straight( unsigned long Period, float Duty_Cycle );

// Go Reverse (period==Load count , Duty cycle) --> recommended Dc in range (0.2-0.05)
// INPUT: Total period and Duty Cycle
// OUTPUT: none
void DCMOTOR_Reverse( unsigned long Period, float Duty_Cycle );

// Turn Right
// INPUT: none
// OUTPUT: none
void TurnRight(void);

// Turn Left
// INPUT: none
// OUTPUT: none
void TurnLeft(void);

// Still Stand
// INPUT: none
// OUTPUT: none
void DCMOTOR_Stand( void );

// forward move.
// INPUT: none
// OUTPUT: none
void DCMOTOR_Forward( void );

// backward move
// INPUT: none
// OUTPUT: none
void DCMOTOR_Backward( void );

#endif
