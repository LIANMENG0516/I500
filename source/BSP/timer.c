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

    TIM_TimeBaseInitStruct.TIM_Period = period;    				//������װ�ؼ���ֵ
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;    	        //���ü���ģʽ�����ϼ���ģʽ 
    TIM_TimeBaseInitStruct.TIM_Prescaler = prescaler;	  			//��Ƶϵ��
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;		 	//����ʱ�ӷ�Ƶ����
    
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct);				 	
}

void TIM_PwmConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCMode)
{
    TIM_OCInitTypeDef TIM_OCInitSturc;    						
    
    TIM_OCInitSturc.TIM_OCMode = TIM_OCMode;    					
    TIM_OCInitSturc.TIM_OutputState = TIM_OutputState_Enable;		        //�Ƚ����ʹ��
    TIM_OCInitSturc.TIM_OutputNState = TIM_OutputNState_Disable;                //�Ƚ����ʹ��
    TIM_OCInitSturc.TIM_OCPolarity = TIM_OCPolarity_High;    		        //������Ը�
    TIM_OCInitSturc.TIM_Pulse = 0;
    TIM_OCInitSturc.TIM_OCPolarity = TIM_OCPolarity_High;                       //�����������
    TIM_OCInitSturc.TIM_OCNPolarity = TIM_OCNPolarity_Low;                      //���û����������
    TIM_OCInitSturc.TIM_OCIdleState = TIM_OCIdleState_Set;                      //����״̬�µĹ���ģʽ
    TIM_OCInitSturc.TIM_OCNIdleState = TIM_OCNIdleState_Reset;                  //��������״̬�µĹ���ģʽ
    
    TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Disable);				//ʹ��CCR1�ϵ�Ԥװ�ؼĴ���
    TIM_ARRPreloadConfig(TIMx, DISABLE);                                        
    TIM_OC1Init(TIMx, &TIM_OCInitSturc);    						
}


void TimerConfig()
{
    Timer_Init(TIM1, 1000-1, 84-1);                                             //1Ms
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);                                  //����ʱ�������ж�
    TIM_Cmd(TIM1, ENABLE);                                                      //ʹ�ܶ�ʱ��
    
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





