#ifndef __GPIO__CONFG_H_
#define __GPIO__CONFG_H_

#define OUTPUT 1
#define INPUT 0
#include <stdint.h>
void GPIO_init (uint8_t Port_no,uint8_t Pin,uint8_t direction);
void GPIO_write (uint8_t Port_no,uint8_t Pin,uint8_t Data);  //Data in hex as there is masking so don't worry

#endif
