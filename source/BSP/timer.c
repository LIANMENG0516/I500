#include "timer.h"

TimeFlagStruct TimeFlag;

void Timer_Init(TIM_TypeDef* TIMx, uint16_t period, uint16_t prescaler)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;   
    
    if(TIMx == TIM1)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
    if(TIMx == TIM2)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  
    if(TIMx == TIM3)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   
    if(TIMx == TIM4)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  
    if(TIMx == TIM5)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);     

    TIM_TimeBaseInitStruct.TIM_Period = period;    				//设置重装载计数值
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;    	        //设置计数模式：向上计数模式 
    TIM_TimeBaseInitStruct.TIM_Prescaler = prescaler;	  			//分频系数
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;		 	//设置时钟分频因子
    
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct);				 	
}

void TIM_PwmConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCMode)
{
    TIM_OCInitTypeDef TIM_OCInitSturc;    						
    
    TIM_OCInitSturc.TIM_OCMode = TIM_OCMode;    					
    TIM_OCInitSturc.TIM_OutputState = TIM_OutputState_Enable;		        //比较输出使能
    TIM_OCInitSturc.TIM_OutputNState = TIM_OutputNState_Disable;                //比较输出使能
    TIM_OCInitSturc.TIM_OCPolarity = TIM_OCPolarity_High;    		        //输出极性高
    TIM_OCInitSturc.TIM_Pulse = 0;
    TIM_OCInitSturc.TIM_OCPolarity = TIM_OCPolarity_High;                       //设置输出属性
    TIM_OCInitSturc.TIM_OCNPolarity = TIM_OCNPolarity_Low;                      //设置互补输出属性
    TIM_OCInitSturc.TIM_OCIdleState = TIM_OCIdleState_Set;                      //空闲状态下的工作模式
    TIM_OCInitSturc.TIM_OCNIdleState = TIM_OCNIdleState_Reset;                  //互补空闲状态下的工作模式
    
    TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Disable);				//使能CCR1上的预装载寄存器
    TIM_ARRPreloadConfig(TIMx, DISABLE);                                        
    TIM_OC1Init(TIMx, &TIM_OCInitSturc);    						
}


void TimerConfig()
{
    Timer_Init(TIM1, 1000-1, 84-1);                                             //1Ms
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);                                  //允许定时器更新中断
    TIM_Cmd(TIM1, ENABLE);                                                      //使能定时器
    
    TIM_PwmConfig(TIM1, TIM_OCMode_PWM1);  
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void TIM1_IRQHandler_CallBack()
{
    static uint16_t TimerCount = 0;
    
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        TimerCount++;
        
        if(TimerCount % 10 == 0)
        {
            TIM_FLAG_10MS = TIM_FLAG_SET;
        }
        if(TimerCount % 50 == 0)
        {
            TIM_FLAG_50MS = TIM_FLAG_SET;
        }
        if(TimerCount % 100 == 0)
        {
            TIM_FLAG_100MS = TIM_FLAG_SET;
        }
        if(TimerCount % 200 == 0)
        {
            TIM_FLAG_200MS = TIM_FLAG_SET;
        }
        if(TimerCount % 300 == 0)
        {
            TIM_FLAG_300MS = TIM_FLAG_SET;
        }
        if(TimerCount % 500 == 0)
        {
            TIM_FLAG_500MS = TIM_FLAG_SET;
        }
        if(TimerCount % 1000 == 0)
        {
            TIM_FLAG_1000MS = TIM_FLAG_SET;
        }
        if(TimerCount % 2000 == 0)
        {
            TIM_FLAG_2000MS = TIM_FLAG_SET;
        }
    }

    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
}





