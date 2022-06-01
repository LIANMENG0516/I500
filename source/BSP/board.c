#include "board.h"

#include "string.h"

uint8_t TimeStr[8]  = __TIME__;
uint8_t DateStr[11] = __DATE__;

static uint8_t RecvDataCmd[100];
static CmdFrameStruct RecvFrameCmd = {0x68, 0x04, 0x00, 0x00, (uint8_t *)RecvDataCmd, 0x00, 0x16};

static uint8_t SendDataCmd[100];
static CmdFrameStruct SendFrameCmd = {0x68, 0x04, 0x00, 0x00, (uint8_t *)SendDataCmd, 0x00, 0x16};

extern System_MsgStruct SysMsg;

FrameStepEnum FrameStep;

ErrorStatus ReceiveFrame()
{
    static int DataInc = 0;
    uint8_t TempChar = 0, ChkSum = 0;
    
    switch(FrameStep)
    {
        case    MSG_RX_STX:
                if(Cmd_ComReadRxBuf(&TempChar) == SUCCESS)
                {
                    if(TempChar == 0x68)
                    {
                        RecvFrameCmd.Header = TempChar;
                        FrameStep = MSG_RX_MODEL_ID;
                    }
                    else
                    {
                        FrameStep = MSG_RX_STX;
                    }
                }
                break;
                
        case    MSG_RX_MODEL_ID:
                if(Cmd_ComReadRxBuf(&TempChar) == SUCCESS)
                {
                    if(TempChar == 0x04)
                    {
                        RecvFrameCmd.Id = TempChar;
                        FrameStep = MSG_RX_FRAME_TYPE;
                    }
                    else
                    {
                        FrameStep = MSG_RX_STX;
                    }
                }
                break;
                
        case    MSG_RX_FRAME_TYPE:
                if(Cmd_ComReadRxBuf(&TempChar) == SUCCESS)
                {
                    RecvFrameCmd.Cid = TempChar;
                    FrameStep = MSG_RX_DATA_LEN;
                }
                break;
                
        case    MSG_RX_DATA_LEN:
                if(Cmd_ComReadRxBuf(&TempChar) == SUCCESS)
                {
                    RecvFrameCmd.Len = TempChar;
                    FrameStep = MSG_RX_DATA;
                }
                break;
                
         case    MSG_RX_DATA:
                if(Cmd_ComReadRxBuf(&TempChar) == SUCCESS)
                {                    
                    RecvFrameCmd.Data[DataInc++] = TempChar;
                    if(DataInc >= RecvFrameCmd.Len)
                    {
                        FrameStep = MSG_RX_CHK;
                    } 
                }
                break;       
        
        case    MSG_RX_CHK:
                if(Cmd_ComReadRxBuf(&TempChar) == SUCCESS)
                {                    
                    RecvFrameCmd.Chk = TempChar;
                    FrameStep = MSG_RX_ETX;
                }
                break; 
                
        case    MSG_RX_ETX:
                if(Cmd_ComReadRxBuf(&TempChar) == SUCCESS)
                {                    
                    if(TempChar == 0x16)
                    {
                        DataInc = 0;
                        
                        ChkSum += RecvFrameCmd.Id;
                        ChkSum += RecvFrameCmd.Cid;
                        ChkSum += RecvFrameCmd.Len;
                        
                        for(int i=0; i<RecvFrameCmd.Len; i++)
                        {
                            ChkSum += RecvFrameCmd.Data[i];
                        }
                        
                        if(ChkSum == RecvFrameCmd.Chk)
                        {
                            FrameStep = MSG_RX_STX;
                            return SUCCESS;
                        }
                        else
                        {
                            FrameStep = MSG_RX_STX;
                            return ERROR;
                        }
                    }
                    else
                    {
                        DataInc = 0;
                        FrameStep = MSG_RX_STX;
                    }                    
                }
                break; 
    }
    return ERROR;
}

static void FrameCmdPackage(CmdFrameStruct *SFrameStr)
{
    SFrameStr->Chk = SFrameStr->Id + SFrameStr->Cid + SFrameStr->Len;
    for(int i=0; i<SFrameStr->Len; i++)
    {
        SFrameStr->Chk += SFrameStr->Data[i];
    }
}

static void Send_CmdFramePackage(CmdFrameStruct *SFrameStr)
{
    Usart_Transmit_Char(CMD_COM, SFrameStr->Header);
    Usart_Transmit_Char(CMD_COM, SFrameStr->Id);
    Usart_Transmit_Char(CMD_COM, SFrameStr->Cid);
    Usart_Transmit_Char(CMD_COM, SFrameStr->Len);
    
    for(int i=0; i<SFrameStr->Len; i++)
    {
        Usart_Transmit_Char(CMD_COM, SFrameStr->Data[i]);
    }
    
    Usart_Transmit_Char(CMD_COM, SFrameStr->Chk);
    Usart_Transmit_Char(CMD_COM, SFrameStr->Tail);
}

void Get_FirmWare_Info(CmdFrameStruct *SFrameStr)
{
    SFrameStr->Cid = CMD_FIRMWARE_VERSION;
    SFrameStr->Len = 1;
    SFrameStr->Data[0] = FIRMWARE_VERSION;
}

void Get_COMPILE_Info(CmdFrameStruct *SFrameStr)
{
    SFrameStr->Cid = CMD_COMPILE_INFO;
    SFrameStr->Len = sizeof(TimeStr) + sizeof(DateStr);
    memcpy(&SFrameStr->Data[0], DateStr, 11);
    memcpy(&SFrameStr->Data[11], DateStr, 8);
}

void Get_EnterSleepInfo(CmdFrameStruct *SFrameStr)
{
    SFrameStr->Cid = CMD_FIRMWARE_VERSION;
    SFrameStr->Len = 1;
    SFrameStr->Data[0] = ACK_OK;

    SysMsg.SystemState = SYSTEM_SLEEP;
    EN_MAIN_WKUP(0);                                                            //进入唤醒模式时拉低，唤醒以后拉高，通知主板，电源板已唤醒
    MCU_LED_OPEN();                                                             //指示灯常量表示进入停止模式  
    EN_5V5(0);
    EN_12V(0);
}

void Get_PowerOff_Info(CmdFrameStruct *SFrameStr)
{
    SFrameStr->Cid = CMD_POWER_OFF;
    SFrameStr->Len = 1;
    SFrameStr->Data[0] = ACK_OK;
    
    EN_5V5(0);
    EN_12V(0);
    SysMsg.SystemState = SYSTEM_OFF;
}

void Get_Alarm_Info(CmdFrameStruct *SFrameStr)
{
    SFrameStr->Cid = CMD_ALARM_STATE;
    SFrameStr->Len = 1;
    SFrameStr->Data[0] = SysMsg.SysAlarmState;
}

extern PowerInfoSturct PowerInfo;
void Get_PowerState_Info(CmdFrameStruct *SFrameStr)
{
    SFrameStr->Cid = CMD_POWER_STATE;
    SFrameStr->Len = 1;
    SFrameStr->Data[0] = *(uint8_t *)&PowerInfo;
}

void Get_BoardTemp_Info(CmdFrameStruct *SFrameStr)
{
    SFrameStr->Cid = CMD_BOARD_TEMP;
    SFrameStr->Len = 1;
    SFrameStr->Data[0] = SysMsg.BoradTemp;
}

void Get_Voltage_Info(CmdFrameStruct *SFrameStr)
{
    SFrameStr->Cid = CMD_VOLTAGE_INFO;
    SFrameStr->Len      =  6;
    SFrameStr->Data[0]  =  SysMsg.Pwr_MsgStr.Bat_Voltage >> 8;
    SFrameStr->Data[1]  =  SysMsg.Pwr_MsgStr.Bat_Voltage;
    SFrameStr->Data[2]  =  SysMsg.Vol_MsgStr.Voltage_12V >> 8;
    SFrameStr->Data[3]  =  SysMsg.Vol_MsgStr.Voltage_12V;
    SFrameStr->Data[4]  =  SysMsg.Vol_MsgStr.Voltage_5V5 >> 8;
    SFrameStr->Data[5]  =  SysMsg.Vol_MsgStr.Voltage_5V5;
}

void Get_BAT_ENERGY_Info(CmdFrameStruct *SFrameStr)
{
    SFrameStr->Cid = CMD_BAT_ENERGY;
    SFrameStr->Len =  1;
    SFrameStr->Data[0]  = SysMsg.Pwr_MsgStr.Bat_Power;
}

void Illagal_CidData(CmdFrameStruct *SFrameStr)
{
    SFrameStr->Cid = CMD_CID_FAIL;
    SFrameStr->Len = 1;
    SFrameStr->Data[0] = ACK_FAIL;
}

void Cmd_Process(CmdFrameStruct *RFrameStr, CmdFrameStruct *SFrameStr)
{
    switch(RFrameStr->Cid)
    {
        case    CMD_FIRMWARE_VERSION:
                Get_FirmWare_Info(SFrameStr);
                break;
                
        case    CMD_COMPILE_INFO:
                Get_COMPILE_Info(SFrameStr);
                break;
                
        case    CMD_SLEEP_EN:                                                   //休眠命令
                Get_EnterSleepInfo(SFrameStr);
                break;
                
        case	CMD_POWER_OFF:							//关机命令
                Get_PowerOff_Info(SFrameStr);
                break;
                
        case	CMD_ALARM_STATE:						//获取报警状态
                Get_Alarm_Info(SFrameStr);
                break;
                
        case	CMD_POWER_STATE:						//获取电源板状态
                Get_PowerState_Info(SFrameStr);
                break;
                
        case	CMD_BOARD_TEMP:							//获取板子温度
                Get_BoardTemp_Info(SFrameStr);
                break;
                
        case	CMD_VOLTAGE_INFO:						//获取各路电压
                Get_Voltage_Info(SFrameStr);
                break;
                
        case	CMD_BAT_ENERGY:							//获取电池电量
                Get_BAT_ENERGY_Info(SFrameStr);
                break;
                
        default:
                Illagal_CidData(SFrameStr);					//CID无效
                break;	
    }
}


void CmdDealProcess()
{
    Cmd_Process(&RecvFrameCmd, &SendFrameCmd);
    FrameCmdPackage(&SendFrameCmd);
    Send_CmdFramePackage(&SendFrameCmd);
}

extern TimeFlagStruct TimeFlag;

unsigned int test = 0;

void Main_Loop_Task()
{
    if(TIM_FLAG_10MS)
    {
        TIM_FLAG_10MS = TIM_FLAG_CLR;
        WDI_FEED();
        System_KeyProcess();
    }
    
    if(TIM_FLAG_100MS)
    {
        TIM_FLAG_100MS = TIM_FLAG_CLR;
        System_StateMinitor();
        System_VoltageMinitor();
        System_TemperatureRead();
        System_TempHigh_Minitor();      //系统高温监控
    }
    
    if(TIM_FLAG_300MS)
    {
        TIM_FLAG_300MS = TIM_FLAG_CLR;
        if(SysMsg.SystemState == SYSTEM_ON)
        { 
            MCU_LED_BLINK();
        }
        else
        {
            MCU_LED_CLOSE();
        }
    }
    
    if(TIM_FLAG_500MS)
    {
        TIM_FLAG_500MS = TIM_FLAG_CLR;
        System_InfoRead();
    }
    
    if(TIM_FLAG_1000MS)
    {
        TIM_FLAG_1000MS = TIM_FLAG_CLR;
        Bat_ChargeManager();                                                    //电池充放电管理
        Bat_InformationRead();                                                  //电池信息读取
        Fan_Control();
    }
}


