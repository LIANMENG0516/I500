#ifndef __GPIO_H_
#define __GPIO_H_

#include "stm32f4xx.h"

//PW_SW
#define PW_SW_PORT		        GPIOA
#define PW_SW_PIN		        GPIO_Pin_0
#define PW_SW_CHK()	                GPIO_ReadInputDataBit(PW_SW_PORT, PW_SW_PIN)

//V_PORE
#define BAT_OK_PORT		        GPIOA
#define BAT_OK_PIN		        GPIO_Pin_1
#define BAT_INSERT_CHK()	        GPIO_ReadInputDataBit(BAT_OK_PORT, BAT_OK_PIN)

//USART2_DEBUG_COM
#define DEBUG_COM_TX_PORT               GPIOA
#define DEBUG_COM_TX_PIN                GPIO_Pin_2

#define DEBUG_COM_RX_PORT               GPIOA
#define DEBUG_COM_RX_PIN                GPIO_Pin_3

//AC_OK
#define AC_OK_PORT		        GPIOA
#define AC_OK_PIN		        GPIO_Pin_4
#define AC_INSERT_CHK()	                GPIO_ReadInputDataBit(AC_OK_PORT, AC_OK_PIN)

//WDI
#define WDI_PORT		        GPIOA
#define WDI_PIN		                GPIO_Pin_6

#define SET_WDI	                        GPIO_SetBits(WDI_PORT, WDI_PIN)
#define CLR_WDI	                        GPIO_ResetBits(WDI_PORT, WDI_PIN)

#define WDI_FEED()                      GPIO_ReadOutputDataBit(WDI_PORT, WDI_PIN) ? CLR_WDI : SET_WDI

//PWM
#define PWM_PORT		        GPIOA
#define PWM_PIN		                GPIO_Pin_8

//USART1_CMD_COM
#define CMD_COM_TX_PORT                 GPIOA
#define CMD_COM_TX_PIN                  GPIO_Pin_9

#define CMD_COM_RX_PORT                 GPIOA
#define CMD_COM_RX_PIN                  GPIO_Pin_10

//FAN_SENSOR--暂时没用
#define FAN_SENSOR_PORT                 GPIOA
#define FAN_SENSOR_PIN                  GPIO_Pin_12

//MAIN_WKUP
#define MAIN_WKUP_PORT                  GPIOB
#define MAIN_WKUP_PIN                   GPIO_Pin_1

#define SET_MAIN_WKUP			GPIO_SetBits(MAIN_WKUP_PORT, MAIN_WKUP_PIN)
#define CLR_MAIN_WKUP			GPIO_ResetBits(MAIN_WKUP_PORT, MAIN_WKUP_PIN)
#define EN_MAIN_WKUP(x)			x ? SET_MAIN_WKUP : CLR_MAIN_WKUP

//EN_12V
#define EN_12V_PORT                     GPIOB
#define EN_12V_PIN                      GPIO_Pin_4

#define SET_EN_12V			GPIO_SetBits(EN_12V_PORT, EN_12V_PIN)
#define CLR_EN_12V			GPIO_ResetBits(EN_12V_PORT, EN_12V_PIN)
#define EN_12V(x)			x ? CLR_EN_12V : SET_EN_12V

//MCU_LED
#define MCU_LED_PORT		        GPIOB
#define MCU_LED_PIN		        GPIO_Pin_7
#define MCU_LED_OPEN()                  GPIO_SetBits(MCU_LED_PORT, MCU_LED_PIN)
#define MCU_LED_CLOSE()                 GPIO_ResetBits(MCU_LED_PORT, MCU_LED_PIN)
#define MCU_LED_BLINK() 	        GPIO_ReadOutputDataBit(MCU_LED_PORT, MCU_LED_PIN) ? GPIO_ResetBits(MCU_LED_PORT, MCU_LED_PIN) : GPIO_SetBits(MCU_LED_PORT, MCU_LED_PIN)

//开关机状态检测引脚
#define POWER_HOLD_PORT		        GPIOB
#define POWER_HOLD_PIN		        GPIO_Pin_8
#define POWER_HOLD_CHK() 	        GPIO_ReadInputDataBit(POWER_HOLD_PORT, POWER_HOLD_PIN)

//FAN使能控制引脚
#define FAN_EN_PORT 		        GPIOB
#define FAN_EN_PIN                      GPIO_Pin_9

#define SET_FAN_EN		        GPIO_SetBits(FAN_EN_PORT, FAN_EN_PIN)
#define CLR_FAN_EN                      GPIO_ResetBits(FAN_EN_PORT, FAN_EN_PIN)
#define FAN_EN(x)                       x ? SET_FAN_EN : CLR_FAN_EN

//LED_GREEN_CTL控制引脚
#define LED_GREEN_CTL_PORT              GPIOB
#define LED_GREEN_CTL_PIN               GPIO_Pin_10

#define SET_LED_GREEN_CTL	        GPIO_SetBits(LED_GREEN_CTL_PORT, LED_GREEN_CTL_PIN)
#define CLR_LED_GREEN_CTL               GPIO_ResetBits(LED_GREEN_CTL_PORT, LED_GREEN_CTL_PIN)
#define LED_GREEN_CTL(x)	        x ? CLR_LED_GREEN_CTL : SET_LED_GREEN_CTL

#define STATE_LED_GREEN                 GPIO_ReadOutputDataBit(LED_GREEN_CTL_PORT, LED_GREEN_CTL_PIN)
#define LED_GREEN_BLINK()	        STATE_LED_GREEN ? CLR_LED_GREEN_CTL : SET_LED_GREEN_CTL

//OFF_CMD
#define OFF_CMD_PORT	                GPIOB
#define OFF_CMD_PIN	                GPIO_Pin_12

#define SET_OFF_CMD		        GPIO_SetBits(OFF_CMD_PORT, OFF_CMD_PIN)
#define CLR_OFF_CMD		        GPIO_ResetBits(OFF_CMD_PORT, OFF_CMD_PIN)
#define OFF_CMD_CTL(x)		        x ? CLR_OFF_CMD : SET_OFF_CMD

//开关机检测引脚
#define PW_INT_PORT		        GPIOB
#define PW_INT_PIN		        GPIO_Pin_13

#define PW_INT_CHK()		        GPIO_ReadInputDataBit(PW_INT_PORT, PW_INT_PIN)

//BAT充电检测引脚2
#define BAT_STATE1_PORT		        GPIOB
#define BAT_STATE1_PIN		        GPIO_Pin_14
#define BAT_STATE1_CHK()		GPIO_ReadInputDataBit(BAT_STATE1_PORT, BAT_STATE1_PIN)

//BAT充电检测引脚1
#define BAT_STATE2_PORT		        GPIOB
#define BAT_STATE2_PIN		        GPIO_Pin_15
#define BAT_STATE2_CHK()		GPIO_ReadInputDataBit(BAT_STATE2_PORT, BAT_STATE2_PIN)

//TEMP
#define TEMP_PORT		        GPIOC
#define TEMP_PIN		        GPIO_Pin_0

//VBAT
#define VBAT_PORT		        GPIOC
#define VBAT_PIN		        GPIO_Pin_1

//AD5V5
#define AD5V5_PORT		        GPIOC
#define AD5V5_PIN		        GPIO_Pin_4

//AD12V
#define AD12V_PORT		        GPIOC
#define AD12V_PIN		        GPIO_Pin_5

//EN_5V5
#define EN_5V5_PORT		        GPIOC
#define EN_5V5_PIN		        GPIO_Pin_7

#define SET_EN_5V5			GPIO_SetBits(EN_5V5_PORT, EN_5V5_PIN)
#define CLR_EN_5V5			GPIO_ResetBits(EN_5V5_PORT, EN_5V5_PIN)
#define EN_5V5(x)			x ? SET_EN_5V5 : CLR_EN_5V5

//BAT快充使能控制引脚
#define FAST_CHG_EN_PORT		GPIOC
#define FAST_CHG_EN_PIN			GPIO_Pin_9

#define SET_FSATCHANGE_EN		GPIO_SetBits(FAST_CHG_EN_PORT, FAST_CHG_EN_PIN)
#define CLR_FSATCHANGE_EN		GPIO_ResetBits(FAST_CHG_EN_PORT, FAST_CHG_EN_PIN)
#define BAT_FASTCHARGE_EN(x)		x ? SET_FSATCHANGE_EN : CLR_FSATCHANGE_EN

//BAT充电使能控制引脚
#define CHG_EN_PORT			GPIOC
#define CHG_EN_PIN			GPIO_Pin_12
#define CHG_EN_MODE			GPIO_Mode_Out_PP

#define SET_CHARGE_EN			GPIO_SetBits(CHG_EN_PORT, CHG_EN_PIN)
#define CLR_CHARGE_EN			GPIO_ResetBits(CHG_EN_PORT, CHG_EN_PIN)
#define BAT_CHARGE_EN(x)		x ? SET_CHARGE_EN : CLR_CHARGE_EN

void Gpio_Config(void);

#endif
