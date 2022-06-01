#include "fan.h"

#include "board.h"

extern System_MsgStruct SysMsg;

/*
**************************************************************************************************************
*					Fan_Init()
* Description : 风扇初始化
* Input	      : None
* Output      : None
* Return      : None
**************************************************************************************************************
*/
void Fan_Init()
{
    FAN_EN(0);								        //关闭风扇电源
    TIM_SetCompare1(TIM1, CCR1_VAL_FANSPEED0);				        //设置PWM占空比为0%
}

/*
**************************************************************************************************************
*					Fan_Control()
* Description : 风扇控制
* Input	      : None
* Output      : None
* Return      : None
**************************************************************************************************************
*/
void Fan_Control()
{
    if(SysMsg.SystemState == SYSTEM_ON)
    {
        if(SysMsg.BoradTemp == SYSTEMPLEVEL1)
        {
            FAN_EN(0);
            TIM_SetCompare1(TIM1, CCR1_VAL_FANSPEED0);
        }
        else if(SysMsg.BoradTemp == SYSTEMPLEVEL2)
        {
            FAN_EN(1);
            TIM_SetCompare1(TIM1, CCR1_VAL_FANSPEED1);
        }
        else if(SysMsg.BoradTemp == SYSTEMPLEVEL3)
        {
            FAN_EN(1);
            TIM_SetCompare1(TIM1, CCR1_VAL_FANSPEED2);
        }
        else if(SysMsg.BoradTemp == SYSTEMPLEVEL4)
        {
            FAN_EN(1);
            TIM_SetCompare1(TIM1, CCR1_VAL_FANSPEED3);
        }
        else if(SysMsg.BoradTemp == SYSTEMPLEVEL5)
        {
            FAN_EN(1);
            TIM_SetCompare1(TIM1, CCR1_VAL_FANSPEED4);
        }
        else if(SysMsg.BoradTemp == SYSTEMPLEVEL6)
        {
            FAN_EN(1);
            TIM_SetCompare1(TIM1, CCR1_VAL_FANSPEED5);
        }
        else
        {
            FAN_EN(1);
            TIM_SetCompare1(TIM1, CCR1_VAL_FANSPEED5);				
        }
    }
    else
    {
        FAN_EN(0);							//关闭风扇电源
        TIM_SetCompare1(TIM1, CCR1_VAL_FANSPEED0);			//设置PWM占空比为0%
    }
}








