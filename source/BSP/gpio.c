#include "gpio.h"

static void Gpio_Init(GPIO_TypeDef* GPIOx, uint16_t Gpio_Pin, GPIOMode_TypeDef GPIO_Mode, GPIOOType_TypeDef GPIO_OType, GPIOPuPd_TypeDef GPIO_PuPd)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.GPIO_Pin = Gpio_Pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode;				
    GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;		
    GPIO_InitStruct.GPIO_OType = GPIO_OType;				
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd;			
    GPIO_Init(GPIOx, &GPIO_InitStruct);	
}

void Gpio_Config()
{	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);                                   
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);         //使能PWR时钟
    PWR_BackupAccessCmd(ENABLE);                                //使能后备寄存器访问
    RCC_LSEConfig(RCC_LSE_OFF);                                 //关闭外部低速外部时钟, 关闭后PC14 PC15才可以当普通IO用。
    PWR_BackupAccessCmd(DISABLE);                               //禁止修改后备寄存器
    
    //PW_SW
    Gpio_Init(PW_SW_PORT, PW_SW_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);
    
    //V_PORE
    Gpio_Init(BAT_OK_PORT, BAT_OK_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL);

    //USART2_DEBUG_COM
    GPIO_PinAFConfig(DEBUG_COM_TX_PORT, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(DEBUG_COM_RX_PORT, GPIO_PinSource3, GPIO_AF_USART2);
    Gpio_Init(DEBUG_COM_TX_PORT, DEBUG_COM_TX_PIN, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL);
    Gpio_Init(DEBUG_COM_RX_PORT, DEBUG_COM_RX_PIN, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL);
    
    //AC_OK
    Gpio_Init(AC_OK_PORT, AC_OK_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL);
    
    //WDI
    Gpio_Init(WDI_PORT, WDI_PIN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);
    
    //PWM
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
    Gpio_Init(PWM_PORT, PWM_PIN, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL);
    
    //USART1_CMD_COM
    GPIO_PinAFConfig(CMD_COM_TX_PORT, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(CMD_COM_RX_PORT, GPIO_PinSource10, GPIO_AF_USART1);
    Gpio_Init(CMD_COM_TX_PORT, CMD_COM_TX_PIN, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL);
    Gpio_Init(CMD_COM_RX_PORT, CMD_COM_RX_PIN, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL);
    
    //MAIN_WKUP
    Gpio_Init(MAIN_WKUP_PORT, MAIN_WKUP_PIN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);
    EN_MAIN_WKUP(1);                                                                                    //默认高电平为唤醒状态

    //EN_12V
    Gpio_Init(EN_12V_PORT, EN_12V_PIN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);
    EN_12V(0);
    
    //MCU_LED
    Gpio_Init(MCU_LED_PORT, MCU_LED_PIN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);

    //POWER_HOLD
    Gpio_Init(POWER_HOLD_PORT, POWER_HOLD_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL);

    //FAN_EN
    Gpio_Init(FAN_EN_PORT, FAN_EN_PIN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);
    
    //LED_GREEN_CTL
    Gpio_Init(LED_GREEN_CTL_PORT, LED_GREEN_CTL_PIN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);
    
    //OFF_CMD
    Gpio_Init(OFF_CMD_PORT, OFF_CMD_PIN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);
    OFF_CMD_CTL(0);
    
    //BAT_STATE2
    Gpio_Init(BAT_STATE2_PORT, BAT_STATE2_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL);
    
    //BAT_STATE1
    Gpio_Init(BAT_STATE1_PORT, BAT_STATE1_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL);
    
    //TEMP
    Gpio_Init(TEMP_PORT, TEMP_PIN, GPIO_Mode_AN, GPIO_OType_PP, GPIO_PuPd_NOPULL);
    
    //VBAT
    Gpio_Init(VBAT_PORT, VBAT_PIN, GPIO_Mode_AN, GPIO_OType_PP, GPIO_PuPd_NOPULL);
    
    //AD5V5
    Gpio_Init(AD5V5_PORT, AD5V5_PIN, GPIO_Mode_AN, GPIO_OType_PP, GPIO_PuPd_NOPULL);
    
    //AD12V
    Gpio_Init(AD12V_PORT, AD12V_PIN, GPIO_Mode_AN, GPIO_OType_PP, GPIO_PuPd_NOPULL);
    
    //EN_5V5
    Gpio_Init(EN_5V5_PORT, EN_5V5_PIN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL); 
    EN_5V5(0);
    

    //FAST_CHG
    Gpio_Init(FAST_CHG_EN_PORT, FAST_CHG_EN_PIN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);
    
    //CHG_EN
    Gpio_Init(CHG_EN_PORT, CHG_EN_PIN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);  
}
