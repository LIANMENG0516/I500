#include "battery.h"

#include "board.h"

extern System_MsgStruct SysMsg;

/*
**************************************************************************************************************
*					Bat_ChargeManager()
* Description : ������
* Input	      : None
* Output      : None
* Return      : None
**************************************************************************************************************
*/
void Bat_ChargeManager()
{
    static uint8_t batChargeRecoverCnt = 0;		                        
    
    if(SysMsg.Pwr_MsgStr.Ac_Insert == TRUE && SysMsg.Pwr_MsgStr.Bat_Insert == TRUE)
    {
        if(SysMsg.Pwr_MsgStr.Bat_ErrorFlag == FALSE)
        {
            if(SysMsg.Pwr_MsgStr.Bat_State == BAT_STATE_FULL ||SysMsg.Pwr_MsgStr.Bat_State == BAT_STATE_ERROR)	
            {
                if(SysMsg.Pwr_MsgStr.Bat_Voltage < BAT_CHARGEENERGY_100)
                {
                    if(++batChargeRecoverCnt > 100)
                    {
                        BAT_CHARGE_EN(0);	                                
                        SysMsg.Pwr_MsgStr.Bat_ErrorFlag = TRUE;
                    }
                    else
                    {
                        BAT_CHARGE_EN(1);	                                
                        SysMsg.Pwr_MsgStr.Bat_ErrorFlag = FALSE;
                    }
                }
                else
                {       
                    BAT_CHARGE_EN(0);	                                
                    SysMsg.Pwr_MsgStr.Bat_ErrorFlag = FALSE;
                }       
            }
            if(SysMsg.Pwr_MsgStr.Bat_State == BAT_STATE_CHARGE)
            {
                batChargeRecoverCnt = 0;
                BAT_CHARGE_EN(1);	                                        
                SysMsg.Pwr_MsgStr.Bat_ErrorFlag = FALSE;
            }
                
            if(SysMsg.SystemState == SYSTEM_ON)
            {
                BAT_FASTCHARGE_EN(0);	                                        
            }
            else
            {
                if(SysMsg.Pwr_MsgStr.Bat_ErrorFlag == FALSE)	
                {
                    BAT_FASTCHARGE_EN(1);
                }
                else
                {
                    BAT_FASTCHARGE_EN(0);
                }    
            }
        }
    }
    else
    {
        BAT_CHARGE_EN(0);
        BAT_FASTCHARGE_EN(0);
        SysMsg.Pwr_MsgStr.Bat_ErrorFlag = FALSE;
    }
}

/*
**************************************************************************************************************
*				           Bat_Energy_Manager()
* Description : ��ص�������
* Input	      : None
* Output      : None
* Return      : ��ص�ǰ����
**************************************************************************************************************
*/
uint8_t Bat_Energy_Manager()
{
    static uint8_t batEnergy;
    static uint8_t startCnt = 0;
    static uint16_t valEnergyNew = 0, valEnergyOld = 0;
    
    valEnergyNew = SysMsg.Pwr_MsgStr.Bat_Voltage;		                                        //��ȡ��ص�����ʱֵ
    
    if(SysMsg.Pwr_MsgStr.Ac_Insert == TRUE)			                                        //AC��λʱ�ĵ�������--���ݳ�����߽��й���
    {
        if(valEnergyNew != valEnergyOld && ++startCnt >= 1)
        {
            valEnergyOld = valEnergyNew;						
            
            if(valEnergyNew < BAT_CHARGEENERGY_1)	                                                //��ص���0%����
            {
                batEnergy = 0;
            }
            else if(valEnergyNew >= BAT_CHARGEENERGY_1 && valEnergyNew < BAT_CHARGEENERGY_21)           //����1%~20%����
            {
                for(int i=1; i<=20; i++)
                {
                    if(valEnergyNew < (BAT_CHARGEENERGY_1 + i * 7.5))
                    {
                            batEnergy = i;
                            break;
                    }
                }
            }
            else if(valEnergyNew >= BAT_CHARGEENERGY_21 && valEnergyNew < BAT_CHARGEENERGY_81)          //����21%~80%����
            {
                for(int i=1; i<=60; i++)
                {
                    if(valEnergyNew < (BAT_CHARGEENERGY_21 + i * 1.66))
                    {
                            batEnergy = i + 20;
                            break;
                    }
                }
            }
            else if(valEnergyNew>=BAT_CHARGEENERGY_81 && valEnergyNew<BAT_CHARGEENERGY_100)             //����81%~99%����
            {
                for(int i=1; i<=19; i++)
                {
                    if(valEnergyNew < (BAT_CHARGEENERGY_81 + i * 3.68))
                    {
                            batEnergy = i + 80;
                            break;
                    }
                }
            }
            else
            {
                batEnergy = 100;
            }
        }
        
        if(valEnergyNew == valEnergyOld)
        {
            startCnt = 0;
        }
    }
    else	                                                                                        //��طŵ�ʱ�ĵ�������--���ݷŵ����߽��й���
    {
        if(valEnergyNew != valEnergyOld && ++startCnt >= 50)
        {
            valEnergyOld = valEnergyNew;

            if(valEnergyNew >= BAT_DISCHARGEENERGY_100)	                                                //��ص���100%����
            {
                batEnergy = BAT_ENERGY_100;
            }
            else if(valEnergyNew < BAT_DISCHARGEENERGY_100 && valEnergyNew >= BAT_DISCHARGEENERGY_1)	//����99%~1%����
            {
                for(int i=1; i<=99; i++)
                {
                    if(valEnergyNew > BAT_DISCHARGEENERGY_99 - i * 3.03)
                    {
                        batEnergy = BAT_ENERGY_100 - i;
                        break;
                    }
                }
            }
            else	                                                                                //����0%����
            {
                batEnergy = BAT_ENERGY_0;
            }
        }
    }
    return batEnergy;
}

void Bat_InformationRead()
{
    SysMsg.Pwr_MsgStr.Bat_Power = Bat_Energy_Manager();
    
    if(SysMsg.Pwr_MsgStr.Ac_Insert == TRUE)
    {
        SysMsg.Pwr_MsgStr.Bat_PowerLow = FALSE;
    }
    else
    {
        if(SysMsg.Pwr_MsgStr.Bat_Power <= 20)
        {
            SysMsg.Pwr_MsgStr.Bat_PowerLow = TRUE;
            SysMsg.SysAlarmState |= ALARM_BAT_VOL;
        }
        else
        {
            SysMsg.Pwr_MsgStr.Bat_PowerLow = FALSE;
            SysMsg.SysAlarmState &= (~ALARM_BAT_VOL);
        }
    }
}

bool Bat_StateChangeMinitor()
{
    bool stateChange = FALSE;
    static uint8_t startCnt = 0;
    uint8_t stateNow = 0, stateOld = 0;
    
    stateNow = Get_BatteryChargeState();
    
    if(stateNow != stateOld && ++startCnt >= 10)
    {
        startCnt = 0;
        SysMsg.Pwr_MsgStr.Bat_State = stateOld = stateNow;
        stateChange = TRUE;
    }
    if(stateNow == stateOld)
    {
        startCnt = 0;
    }
    
    return stateChange;
}

bool Bat_InsertStateChangeMinitor()
{
    bool stateChange = FALSE;
    static uint8_t startCnt = 0;
    bool stateNow = FALSE, stateOld = FALSE;
    
    stateNow = BAT_INSERT_CHK() ? TRUE : FALSE;
    
    if(stateNow != stateOld && ++startCnt >= 10)
    {
        startCnt = 0;
        SysMsg.Pwr_MsgStr.Bat_Insert = stateOld = stateNow;
        stateChange = TRUE;
    }
    if(stateNow == stateOld)
    {
        startCnt = 0;
    }
    
    return stateChange;
}

uint8_t Get_BatteryChargeState()
{
    uint8_t state = 0, state1 = 0, state2 = 0;
    
    state1 = BAT_STATE1_CHK() ? TRUE : FALSE;
    state2 = BAT_STATE2_CHK() ? TRUE : FALSE;
    
    state = (state1 << 1) | state2;
    
    return state;
}









