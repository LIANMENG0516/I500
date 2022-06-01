#include "system.h"

#include "board.h"

System_MsgStruct SysMsg;

void System_VoltageMinitor()
{     
    SysMsg.Vol_MsgStr.Voltage_Tmp = (uint32_t)((Adc1_ReadConvertValue(RANK_VTMP) * 3.3 /4095) * 100);
    SysMsg.Vol_MsgStr.Voltage_12V = (uint32_t)(((Adc1_ReadConvertValue(RANK_V12V) * 3.3 /4095) / 10 * (10 + 27)) * 100);
    SysMsg.Vol_MsgStr.Voltage_5V5 = (uint32_t)(((Adc1_ReadConvertValue(RANK_V5V5) * 3.3 /4095) / 10 * (10 + 6.8)) * 100);
    SysMsg.Pwr_MsgStr.Bat_Voltage = (uint32_t)(((Adc1_ReadConvertValue(RANK_VBAT) * 3.3 /4095) / 100 * (100 + 422)) * 100);
}

bool AC_InserStateChnageMinistor()
{
    bool stateChange = FALSE;
    static uint8_t startCnt = 0;
    bool stateNow = FALSE, stateOld = FALSE;
    
    stateNow = AC_INSERT_CHK() ? TRUE : FALSE;
    
    if(stateNow != stateOld && ++startCnt >= 10)
    {
        startCnt = 0;
        SysMsg.Pwr_MsgStr.Ac_Insert = stateOld = stateNow;
        stateChange = TRUE;
    }
    if(stateNow == stateOld)
    {
        startCnt = 0;
    }
    
    return stateChange;
}

void System_StateMinitor()
{
    AC_InserStateChnageMinistor();
    Bat_InsertStateChangeMinitor();
    Bat_StateChangeMinitor();
}

PowerInfoSturct PowerInfo = {0,0,0,0,0,0,0,0};

void System_InfoRead()
{
    if(SysMsg.Pwr_MsgStr.Bat_PowerLow == TRUE)		                        PowerInfo.b0 = 1;
    else							                PowerInfo.b0 = 0;									
    if(SysMsg.Pwr_MsgStr.Bat_Insert == TRUE)			                PowerInfo.b1 = 1;
    else									PowerInfo.b1 = 0;							
    if(SysMsg.Pwr_MsgStr.Bat_State == BAT_STATE_CHARGE)		                PowerInfo.b2 = 1;
    else									PowerInfo.b2 = 0;							
    if(SysMsg.Pwr_MsgStr.Bat_ErrorFlag == TRUE)			                PowerInfo.b3 = 1;
    else									PowerInfo.b3 = 0;							
    if(SysMsg.Pwr_MsgStr.Ac_Insert == TRUE)			                PowerInfo.b4 = 1;
    else									PowerInfo.b4 = 0;						
    
    PowerInfo.b5 = 0; PowerInfo.b6 = 0; PowerInfo.b7 = 0;
}

/*
**************************************************************************************************************
*					System_TemperatureRead()
* Description : 获取当前系统温度
* Input	      : None
* Output      : None
* Return      : 当前系统温度
**************************************************************************************************************
*/
uint8_t System_TemperatureRead()
{
    float valueR = 0.0;
    uint8_t hotTemp;
    
    valueR = (SysMsg.Vol_MsgStr.Voltage_Tmp / 100.0) / ((3.3 - (SysMsg.Vol_MsgStr.Voltage_Tmp / 100.0)) / 47);			//热敏电阻的电阻值
    
    if(valueR >= 10.00)		 					        hotTemp = SYSTEMPLEVEL1;
    else if(valueR >= 9.287 && valueR < 10)			                hotTemp = SYSTEMPLEVEL2;
    else if(valueR >= 8.632 && valueR < 9.287)		                        hotTemp = SYSTEMPLEVEL3;
    else if(valueR >= 8.028 && valueR < 8.632)		                        hotTemp = SYSTEMPLEVEL4;
    else if(valueR >= 7.472 && valueR < 8.028)		                        hotTemp = SYSTEMPLEVEL5;
    else									hotTemp = SYSTEMPLEVEL6;
        
    return hotTemp;
}

bool System_TempHigh_Minitor()
{
    bool   stateChange = FALSE;
    static uint16_t startCnt = 0;
    static bool stateNow = FALSE, stateOld = FALSE;
    
    SysMsg.BoradTemp = System_TemperatureRead();				//获取当前系统温度
    
    stateNow = SysMsg.BoradTemp > SYSTEMTEMPALARM ? TRUE : FALSE;	        //判断高温预警状态	
    
    if(stateNow != stateOld && ++startCnt >= 10)
    {
        startCnt = 0;
        stateChange = TRUE;							//高温预警状态改变
        
        SysMsg.Borad_TempHigh = stateOld = stateNow;				//获取高温预警状态
        
        if(stateNow == TRUE)							//高温预警发生
        {
            SysMsg.SysAlarmState |= ALARM_TEMP;
        }
        else									//高温预警消失
        {
            SysMsg.SysAlarmState &= (~ALARM_TEMP);
        }
    }
    if(stateNow == stateOld)
    {
        startCnt = 0;
    }
    
    return stateChange;
}

void System_BspInit()
{
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);

    Nvic_Config();
    Gpio_Config();
    SysTick_Iint();
    TimerConfig();
    Cmd_Com_Config();
    Debug_Com_Config();
    V_AdcDma_Config();
    Fan_Init();
}

void System_SateInit()
{
    SysMsg.Vol_MsgStr.Voltage_12V = 0;
    SysMsg.Vol_MsgStr.Voltage_5V5 = 0;
    SysMsg.Pwr_MsgStr.Ac_Insert = FALSE;

    SysMsg.Pwr_MsgStr.Bat_Insert = FALSE;
    SysMsg.Pwr_MsgStr.Bat_ErrorFlag = FALSE;
    
    SysMsg.Pwr_MsgStr.Bat_Power = 0;
    SysMsg.Pwr_MsgStr.Bat_PowerLow = FALSE;
    SysMsg.Pwr_MsgStr.Bat_State = 0;
    SysMsg.Pwr_MsgStr.Bat_Voltage = 0;
    SysMsg.BoradTemp = 0;
    SysMsg.SysAlarmState = 0;
    SysMsg.Borad_TempHigh = FALSE;
    SysMsg.SystemState = SYSTEM_OFF;
}

void System_KeyProcess()
{
    bool stateNow = FALSE;
    static uint16_t startCnt = 0;

    stateNow = PW_SW_CHK() ? FALSE : TRUE;

    if(stateNow == TRUE && ++startCnt)
    {
        if(SysMsg.SystemState == SYSTEM_OFF && startCnt >= 20)
        {
            SysMsg.SystemState = SYSTEM_ON;
            OFF_CMD_CTL(1);            
            EN_5V5(1);
            EN_12V(1);
        }
        if(SysMsg.SystemState == SYSTEM_ON && startCnt >= 500)
        {
            SysMsg.SystemState = SYSTEM_OFF;
            EN_5V5(0);
            EN_12V(0);
            OFF_CMD_CTL(0);
        }      
        if(SysMsg.SystemState == SYSTEM_SLEEP && startCnt >= 20)
        {
            SysMsg.SystemState = SYSTEM_ON;
            EN_5V5(1);
            EN_12V(1);
            OFF_CMD_CTL(1);
        } 
    }
    if(stateNow == FALSE)
    {
        startCnt = 0;
    }
}




