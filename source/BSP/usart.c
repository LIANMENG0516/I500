#include "usart.h"

void Usart_Init(USART_TypeDef* USARTx, uint32_t Baud_Rate)
{
    USART_InitTypeDef USART_InitStructure;
    
    if(USARTx == USART1)
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    else if(USARTx == USART2)
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    else if(USARTx == USART3)
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    else if(USARTx == UART4)
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    else if(USARTx == UART5)
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    
    USART_InitStructure.USART_BaudRate = Baud_Rate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    
    USART_Init(USARTx, &USART_InitStructure); 	
}

void Usart_Transmit_Char(USART_TypeDef* USARTx, uint8_t Data)
{
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) != SET);	                //wait to tx register is empty
    USART_SendData(USARTx, Data);
}

void Usart_Transmit_String(USART_TypeDef* USARTx, uint8_t *Data)
{
    while(*Data)
    {
        Usart_Transmit_Char(USARTx, *Data);
        Data++;
    }
}

void Usart_Transmit_Nchar(USART_TypeDef* USARTx, uint8_t *Data, uint8_t Len)
{
    while(Len--)
    {
        Usart_Transmit_Char(USARTx, *Data);
        Data++;
    }
}

struct __FILE
{
    int handle;
};
FILE __stdout; 

void _sys_exit(int x)
{
    x = x;
}

int fputc(int ch, FILE *f)
{
    while(USART_GetFlagStatus(DEBUG_COM, USART_FLAG_TC) == RESET);
    USART_SendData(DEBUG_COM, (uint8_t)ch);
    return (ch);
}

/*     
********************************************************************************
*
*              以串口接收中断 + 环形缓冲区的方式配置串口
*
********************************************************************************
*/
uint8_t Cmd_ComRxBuf[COM_RXBUFLEN];
uint8_t Cmd_ComTxBuf[COM_TXBUFLEN];

Usart_BufStruct Cmd_ComRxBufStruct = {Cmd_ComRxBuf, 0, 0, Empty};
Usart_BufStruct Cmd_ComTxBufStruct = {Cmd_ComTxBuf, 0, 0, Empty};

unsigned char Cmd_ComWriteRxBuf(unsigned char Data)
{
    ErrorStatus TempState = ERROR;

    if(Cmd_ComRxBufStruct.State != Full)
    {
        Cmd_ComRxBufStruct.Buf[Cmd_ComRxBufStruct.pBufIn++] = Data;
        if(Cmd_ComRxBufStruct.pBufIn >= COM_RXBUFLEN)
        {
            Cmd_ComRxBufStruct.pBufIn = 0;
        }
        if(Cmd_ComRxBufStruct.pBufIn == Cmd_ComRxBufStruct.pBufOut)
        {
            Cmd_ComRxBufStruct.State = Full;
        }
        else
        {
            Cmd_ComRxBufStruct.State = Nchar;
        }
        TempState = SUCCESS;
    }
    
    return TempState;
}

unsigned char Cmd_ComReadRxBuf(unsigned char *Data)
{
    ErrorStatus TempState = ERROR;
  
    if(Cmd_ComRxBufStruct.State != Empty)
    {
        *Data = Cmd_ComRxBufStruct.Buf[Cmd_ComRxBufStruct.pBufOut++];
        
        if(Cmd_ComRxBufStruct.pBufOut >= COM_RXBUFLEN)
        {
            Cmd_ComRxBufStruct.pBufOut = 0;
        }
        if(Cmd_ComRxBufStruct.pBufOut == Cmd_ComRxBufStruct.pBufIn)
        {
            Cmd_ComRxBufStruct.State = Empty;
        }
        else
        {
            Cmd_ComRxBufStruct.State = Nchar;
        }
        TempState = SUCCESS;
    }
    
    return TempState;
}

void Cmd_Com_Config()
{
    Usart_Init(USART1, 115200);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}

/*     
********************************************************************************
*
*              以普通串口空闲中断 + DMA的方式配置串口
*
********************************************************************************
*/
uint8_t	RcvData[100];
uint8_t	SenData[100];

Com_Buffer DebugComRX = {RcvData, 0x00};
Com_Buffer DebugComTX = {SenData, 0x00};

void Debug_Com_Config()
{
    Usart_Init(DEBUG_COM, 115200);
    USART_ITConfig(DEBUG_COM, USART_IT_RXNE, DISABLE);								//关闭RXNE中断 
    USART_ITConfig(DEBUG_COM, USART_IT_TXE, DISABLE);								//关闭TXE中断
    USART_ITConfig(DEBUG_COM, USART_IT_IDLE, ENABLE);								//开启IDLE中断
    
    USART_DMACmd(DEBUG_COM, USART_DMAReq_Rx, ENABLE);								//开启USART_DMA接收通道
    USART_DMACmd(DEBUG_COM, USART_DMAReq_Tx, ENABLE);								//开启USART_DMA发送通道

    Dma_Config(DEBUG_COM_DMAY_STREAMX_RX, DEBUG_COM_DMA_CHANNEL_RX, (uint32_t)&DEBUG_COM->DR, (uint32_t)&DebugComRX.Data[0], DMA_DIR_PeripheralToMemory, sizeof(RcvData), DMA_PeripheralDataSize_Byte, DMA_MemoryDataSize_Byte, DMA_Mode_Circular, DMA_Priority_VeryHigh);                                                      
    Dma_Config(DEBUG_COM_DMAY_STREAMX_TX, DEBUG_COM_DMA_CHANNEL_TX, (uint32_t)&DEBUG_COM->DR, (uint32_t)&DebugComTX.Data[0], DMA_DIR_MemoryToPeripheral, sizeof(SenData), DMA_PeripheralDataSize_Byte, DMA_MemoryDataSize_Byte, DMA_Mode_Circular, DMA_Priority_VeryHigh);

    DMA_Cmd(DEBUG_COM_DMAY_STREAMX_RX, ENABLE);                                                                 //使能DMA接收
    DMA_Cmd(DEBUG_COM_DMAY_STREAMX_TX, DISABLE);                                                                //关闭DMA发送

    USART_Cmd(DEBUG_COM, ENABLE);
}

/*     
********************************************************************************
*
*              串口中断回调函数
*
********************************************************************************
*/
void Com_IRQHandler_CallBack()
{
    unsigned char TempByte;
    if(USART_GetITStatus(CMD_COM, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(CMD_COM, USART_IT_RXNE);	
        TempByte = USART_ReceiveData(CMD_COM);
        Cmd_ComWriteRxBuf(TempByte);
    }

    if(USART_GetITStatus(DEBUG_COM, USART_IT_IDLE) != RESET)
    {
        USART_ClearITPendingBit(DEBUG_COM, USART_IT_IDLE);                      //清串口空闲状态
        DEBUG_COM->SR;
        DEBUG_COM->DR;
        
        DMA_Cmd(DEBUG_COM_DMAY_STREAMX_RX, DISABLE);                                                        //关闭接收通道
        DMA_ClearFlag(DEBUG_COM_DMAY_STREAMX_RX, DEBUG_COM_DMA_FLAG_TCIFX);   				                //清除DMA标记位
        
        DebugComRX.Len = sizeof(RcvData) - DMA_GetCurrDataCounter(DEBUG_COM_DMAY_STREAMX_RX); 		        //获取接收到的数据长度

        DEBUG_COM_DMAY_STREAMX_RX->M0AR = (uint32_t)&DebugComRX.Data[0];
        DEBUG_COM_DMAY_STREAMX_RX->NDTR = sizeof(RcvData);
        DMA_Cmd(DEBUG_COM_DMAY_STREAMX_RX, ENABLE);                                                         //重新打开接收通道
    }  
}
