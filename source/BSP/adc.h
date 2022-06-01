#ifndef __ADC_H_
#define __ADC_H_ 	

#include "stm32f4xx.h"
#include "board.h"

#define ADC1_DMAY_STREAMX_V DMA2_Stream0
#define ADC1_DMA_CHANNEL_V  DMA_Channel_0

#define CHANNEL_VTMP    ADC_Channel_10                                          
#define CHANNEL_VBAT    ADC_Channel_11
#define CHANNEL_V5V5    ADC_Channel_14
#define CHANNEL_V12V    ADC_Channel_15

#define RANK_VTMP       1
#define RANK_VBAT       2
#define RANK_V5V5       3
#define RANK_V12V       4

#define ADC1_SAMPLE_NUM  20
#define ADC1_CHANNEL_NUM 4

uint16_t Adc1_ReadConvertValue(uint8_t Rank);
void V_AdcDma_Config(void);

#endif
