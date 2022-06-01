#include "dma.h"
#include "usart.h"

void Dma_Config(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_Channelx, uint32_t peripher_address, uint32_t ram_address, uint32_t dma_dir, uint16_t rebuf_seize, uint32_t pdatalen, uint32_t mdatalen, uint32_t dma_mode, uint32_t dma_priority)
{
    DMA_InitTypeDef DMA_InitStruct;
    
    if((uint32_t)DMAy_Streamx < (uint32_t)DMA2)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    else
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

    DMA_InitStruct.DMA_Channel = DMA_Channelx;          
    DMA_InitStruct.DMA_PeripheralBaseAddr = peripher_address; 
    DMA_InitStruct.DMA_Memory0BaseAddr = ram_address;    
    DMA_InitStruct.DMA_DIR = dma_dir;   
    DMA_InitStruct.DMA_BufferSize = rebuf_seize;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = pdatalen;
    DMA_InitStruct.DMA_MemoryDataSize = mdatalen;
    DMA_InitStruct.DMA_Mode = dma_mode;							
    DMA_InitStruct.DMA_Priority = dma_priority;									
    DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;	
    DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;	
    DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;	
    DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMAy_Streamx, &DMA_InitStruct);

    DMA_Cmd(DMAy_Streamx, DISABLE);
}

extern uint8_t	RcvData;
extern uint8_t	SenData;

extern Com_Buffer DebugComRX;
extern Com_Buffer DebugComTX;



void Dmay_Streamx_IRQHandler_CallBack()
{
    if(DMA_GetITStatus(DEBUG_COM_DMAY_STREAMX_TX, DEBUG_COM_DMA_IT_TCX) != RESET)
    {
        DMA_Cmd(DEBUG_COM_DMAY_STREAMX_TX, DISABLE); 
        DMA_SetCurrDataCounter(DEBUG_COM_DMAY_STREAMX_TX, sizeof(SenData));
        DMA_ClearITPendingBit(DEBUG_COM_DMAY_STREAMX_TX, DEBUG_COM_DMA_IT_TCX);					//清除DMA发送完成中断标志位
    }
}




















