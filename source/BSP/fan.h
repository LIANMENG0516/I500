#ifndef __FAN_H_
#define __FAN_H_

#include "stm32f4xx.h"

#define SYSTEMPLEVEL1 		25
#define SYSTEMPLEVEL2 		27
#define SYSTEMPLEVEL3 		29
#define SYSTEMPLEVEL4 		31
#define SYSTEMPLEVEL5 		33
#define SYSTEMPLEVEL6 		65

#define SYSTEMTEMPALARM 	33

#define CCR1_VAL_FANSPEED0 	0			//PWMÕ¼¿Õ±ÈÎª0%
#define CCR1_VAL_FANSPEED1 	250
#define CCR1_VAL_FANSPEED2 	450
#define CCR1_VAL_FANSPEED3 	650
#define CCR1_VAL_FANSPEED4 	800
#define CCR1_VAL_FANSPEED5 	999

void Fan_Init(void);
void Fan_Control(void);

#endif

