// Steering.h
// Runs on TM4C123
// Graduation Project, ALEX UNIVERSITY. 
// Junuary, 2018.


#ifndef _STEERING_H_
#define _STEERING_H_

#include <stdint.h>

// Initializations of steering mechanism and motor in this function
// INPUT: none
// OUTPUT: none
void STEERING_Init( void );

// Moving steering in Right direction
// INPUT: none
// OUTPUT: none
void Steering_Right( void );

// Moving steering in Left direction.
// INPUT: none
// OUTPUT: none
void Steering_Left( void );

// Steering in straight motion.
// INPUT: none
// OUTPUT: none
void Steering_Straight( void );

#endif /*_STEERING_H_*/
