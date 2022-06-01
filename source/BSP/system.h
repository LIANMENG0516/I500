#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#include "stm32f4xx.h"

#define SYSTEM_OFF   0	                                                        //ϵͳ�ػ�״̬��־
#define SYSTEM_ON    1 	                                                        //ϵͳ����״̬��־
#define SYSTEM_SLEEP 2 

#define ALARM_TEMP 		0x01	//010
#define ALARM_BAT_VOL		0X02	//100

typedef enum {FALSE = 0, TRUE = !FALSE} bool;

/*****************************************************************************/
typedef __packed struct
{
    bool 	        Ac_Insert;				                //AC�Ƿ���λ�ı�־
    bool	        Bat_PowerLow;	                                        //��ص�����
    bool 	        Bat_Insert;			                        //����Ƿ���λ�ı�־λ

    bool	        Bat_ErrorFlag;				                //��ع��ϱ�־λ
    uint8_t 	        Bat_State;				                //���״̬
    uint8_t	        Bat_Power;		                                //��ص���
    uint16_t	        Bat_Voltage;				                //��ص�ѹ
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
    uint8_t		SysAlarmState;			                        //ϵͳ�澯״̬--�¶ȸ߸澯�������͸澯
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
