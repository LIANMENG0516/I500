#ifndef __BOARD_H_
#define __BOARD_H_

#include "stm32f4xx.h"

#include "stdio.h"
#include "string.h"

#include "battery.h"
#include "systick.h"
#include "system.h"
#include "timer.h"
#include "usart.h"
#include "gpio.h"
#include "nvic.h"
#include "dma.h"
#include "adc.h"
#include "fan.h"

#define FIRMWARE_VERSION 0x10

#define CMD_FIRMWARE_VERSION                    0x01
#define CMD_COMPILE_INFO                        0x02
#define CMD_SLEEP_EN                            0x05
#define CMD_POWER_OFF				0x48			        //关机命令
#define CMD_ALARM_STATE				0x47		                //获取报警状态
#define CMD_POWER_STATE				0x41		                //获取电源板状态
#define CMD_BOARD_TEMP                          0x46				//获取板子温度
#define CMD_VOLTAGE_INFO                        0x42        			//获取各路电压              
#define CMD_BAT_ENERGY                          0x4B				//获取电池电量
#define CMD_CID_FAIL                            0x4C

//ACK
#define ACK_OK					0x00
#define ACK_FAIL				0x01

typedef struct
{
    uint8_t Header;
    uint8_t Id;
    uint8_t Cid;
    uint8_t Len;
    uint8_t *Data;
    uint8_t Chk;
    uint8_t Tail;
}CmdFrameStruct;

typedef enum
{
    MSG_RX_STX,
    MSG_RX_MODEL_ID,
    MSG_RX_FRAME_TYPE,
    MSG_RX_DATA_LEN,
    MSG_RX_DATA,
    MSG_RX_CHK,
    MSG_RX_ETX,
}FrameStepEnum;

ErrorStatus ReceiveFrame(void);

void CmdDealProcess(void);
void Main_Loop_Task(void);


#endif
