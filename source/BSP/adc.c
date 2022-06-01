#include "adc.h"

#include "board.h"

extern System_MsgStruct SysMsg;

void Adc_Init(ADC_TypeDef* ADCx, uint8_t NumChannel)
{    
    ADC_CommonInitTypeDef ADC_CommonInitStruct;
    ADC_InitTypeDef ADC_InitStruct;
    
    if(ADCx == ADC1)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    if(ADCx == ADC2) 
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
    if(ADCx == ADC3) 
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
        
    ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;                            //Ԥ��Ƶ4��Ƶ                            
    ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;          //���������׶�֮����ӳ�5��ʱ��
    ADC_CommonInit(&ADC_CommonInitStruct);
    
    ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStruct.ADC_ScanConvMode = ENABLE;                                           //ɨ��ģʽ
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;                                     //����ת��ģʽ
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;            //ת��������������ⲿ��������
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_Ext_IT11;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_NbrOfConversion = NumChannel;                                    //˳����й���ת����ADCͨ������Ŀ 
    ADC_Init(ADCx, &ADC_InitStruct);
    
    ADC_Cmd(ADCx, ENABLE);														        //ʹ��ָ����ADCx
}

uint16_t  Adc1_ConValue[ADC1_SAMPLE_NUM][ADC1_CHANNEL_NUM];

uint16_t Adc1_ReadConvertValue(uint8_t Rank)
{
    uint8_t  i, j;
    uint32_t sum = 0;
    uint16_t temp, dataBuf[ADC1_SAMPLE_NUM];
    
    for(i=0; i<ADC1_SAMPLE_NUM; i++)
    {
        dataBuf[i] = Adc1_ConValue[i][Rank-1];
    }
    for(i=0; i<ADC1_SAMPLE_NUM; i++)
    {
        for(j=0; j<ADC1_SAMPLE_NUM-1-i; j++)
        {
            if(dataBuf[j] > dataBuf[j+1])
            {
                temp = dataBuf[j];
                dataBuf[j] = dataBuf[j+1];
                dataBuf[j+1] = temp;
            }
        }
    }
    for(i=2; i<(ADC1_SAMPLE_NUM-2); i++)						//ȥ��������Сֵ���������ֵ
    {
        sum = sum + dataBuf[i];
    }

    sum = sum >> 4;
    
    return sum;
}

void Adc1_Config()
{
    Adc_Init(ADC1, ADC1_CHANNEL_NUM);

    ADC_RegularChannelConfig(ADC1, CHANNEL_VTMP, RANK_VTMP,  ADC_SampleTime_480Cycles);
    ADC_RegularChannelConfig(ADC1, CHANNEL_VBAT, RANK_VBAT,  ADC_SampleTime_480Cycles);
    ADC_RegularChannelConfig(ADC1, CHANNEL_V5V5, RANK_V5V5,  ADC_SampleTime_480Cycles);
    ADC_RegularChannelConfig(ADC1, CHANNEL_V12V, RANK_V12V,  ADC_SampleTime_480Cycles);
}

void V_AdcDma_Config()
{
    Adc1_Config();
    Dma_Config(ADC1_DMAY_STREAMX_V, ADC1_DMA_CHANNEL_V, (uint32_t)&ADC1->DR, (uint32_t)Adc1_ConValue, DMA_DIR_PeripheralToMemory, ADC1_SAMPLE_NUM * ADC1_CHANNEL_NUM, DMA_PeripheralDataSize_HalfWord, DMA_MemoryDataSize_HalfWord, DMA_Mode_Circular, DMA_Priority_VeryHigh);                                                                                                                                                                                                                  
    DMA_Cmd(ADC1_DMAY_STREAMX_V, ENABLE); 
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);                           //�����������ֻ����һ��DMA�ж�
    ADC_DMACmd(ADC1, ENABLE);
    ADC_SoftwareStartConv(ADC1);
}














