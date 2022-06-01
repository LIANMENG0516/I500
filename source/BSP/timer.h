#ifndef __TIMER_H_
#define __TIMER_H_

#include "stm32f4xx.h"

typedef __packed struct
{
    vu8 b0:1;
    vu8 b1:1;
    vu8 b2:1;
    vu8 b3:1;
    vu8 b4:1;
    vu8 b5:1;
    vu8 b6:1;
    vu8 b7:1;
}TimeFlagStruct;

#define TIM_FLAG_SET 1
#define TIM_FLAG_CLR 0

#define TIM_FLAG_10MS   TimeFlag.b0
#define TIM_FLAG_50MS   TimeFlag.b1
#define TIM_FLAG_100MS  TimeFlag.b2
#define TIM_FLAG_200MS  TimeFlag.b3
#define TIM_FLAG_300MS  TimeFlag.b4
#define TIM_FLAG_500MS  TimeFlag.b5
#define TIM_FLAG_1000MS TimeFlag.b6
#define TIM_FLAG_2000MS TimeFlag.b7

void Timer_Init(TIM_TypeDef* TIMx, uint16_t period, uint16_t prescaler);
void TimerConfig(void);
void TIM1_IRQHandler_CallBack(void);
    
#endif


