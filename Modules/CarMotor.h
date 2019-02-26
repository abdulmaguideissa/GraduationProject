// carMotor.h
// DC Motor Driver header file.
// This driver is tested on duel channel Cytron motor driver MDD10A according to its datasheet so it is not supposed to work with any other motor drivers
#ifndef _carMotor_H_
#define _carMotor_H_ 

// Initialize the motor with the entire configurations (period==Load count , Duty cycle)
void Motor_Init(unsigned long period,float duty_cycle);

// Initialize Pulse Width Modulation module 0 with the required period and duty cycle
void PWM0_Init(void);

// Initialize the direction pin of the motor
void DIR_Init(void);

// Go Straight forward (period==Load count , Duty cycle) --> recommended Dc in range (0.2-0.05)
void Straight(unsigned long Period,float Duty_Cycle);
// Still Stand
void Stand(void);
// forward move.
void Forward(void);
// backward move
void Backward(void );

// Initializations of steering mechanism and motor in this function
void Steering_Init( void );

// Moving steering in Right direction
void Steering_Right( void );

// Moving steering in Left direction.
void Steering_Left( void );

// Steering in straight motion.
void Steering_Straight( void );

#endif
