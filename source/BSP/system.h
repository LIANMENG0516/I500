#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#include "stm32f4xx.h"

#define SYSTEM_OFF   0	                                                        //系统关机状态标志
#define SYSTEM_ON    1 	                                                        //系统开机状态标志
#define SYSTEM_SLEEP 2 

#define ALARM_TEMP 		0x01	//010
#define ALARM_BAT_VOL		0X02	//100

typedef enum {FALSE = 0, TRUE = !FALSE} bool;

/*****************************************************************************/
typedef __packed struct
{
    bool 	        Ac_Insert;				                //AC是否在位的标志
    bool	        Bat_PowerLow;	                                        //电池电量低
    bool 	        Bat_Insert;			                        //电池是否在位的标志位

    bool	        Bat_ErrorFlag;				                //电池故障标志位
    uint8_t 	        Bat_State;				                //电池状态
    uint8_t	        Bat_Power;		                                //电池电量
    uint16_t	        Bat_Voltage;				                //电池电压
}Power_MsgStruct;

typedef __packed struct
{
    uint16_t	        Voltage_5V5;
    uint16_t	        Voltage_12V;
    uint16_t            Voltage_Tmp;
}Voltage_MsgStruct;

typedef __packed struct
{
    Power_MsgStruct 	Pwr_MsgStr;		
    Voltage_MsgStruct	Vol_MsgStr;
    uint8_t		BoradTemp;
    uint8_t		SysAlarmState;			                        //系统告警状态--温度高告警、电量低告警
    bool		Borad_TempHigh;
    uint8_t		SystemState;
}System_MsgStruct;

typedef __packed struct
{
    vu8                 b0:1;
    vu8                 b1:1;
    vu8                 b2:1;
    vu8                 b3:1;
    vu8                 b4:1;
    vu8                 b5:1;
    vu8                 b6:1;
    vu8                 b7:1;
}PowerInfoSturct;

void System_BspInit(void);
void System_SateInit(void);
void System_KeyProcess(void);
uint8_t System_TemperatureRead(void);
void System_VoltageMinitor(void);
bool AC_InserStateChnageMinistor(void);
void System_StateMinitor(void);
void System_InfoRead(void);
uint8_t System_TemperatureRead(void);
bool System_TempHigh_Minitor(void);

#endif
