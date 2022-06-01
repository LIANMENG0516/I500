#ifndef __USART_H
#define __USART_H 	

#include "stm32f4xx.h"
#include "board.h"

#define CMD_COM   USART1
#define DEBUG_COM USART2

#define COM_RXBUFLEN 200
#define COM_TXBUFLEN 200

#define DEBUG_COM_DMAY_STREAMX_RX       DMA1_Stream5
#define DEBUG_COM_DMAY_STREAMX_TX       DMA1_Stream6
    
#define DEBUG_COM_DMA_CHANNEL_RX        DMA_Channel_4
#define DEBUG_COM_DMA_CHANNEL_TX        DMA_Channel_4

#define DEBUG_COM_DMA_FLAG_TCIFX        DMA_FLAG_TCIF4
#define DEBUG_COM_DMA_IT_TCX            DMA_IT_TCIF4

typedef struct
{
    uint8_t *Data;
    uint8_t Len;
}Com_Buffer;

typedef enum
{
    Empty = 0,
    Nchar,
    Full,
}Usart_BufState;

typedef struct
{
    unsigned char  *Buf;
    unsigned char   pBufIn;
    unsigned char   pBufOut;
    Usart_BufState  State;
}Usart_BufStruct;

void Usart_Transmit_Char(USART_TypeDef* USARTx, uint8_t Data);
void Usart_Transmit_String(USART_TypeDef* USARTx, uint8_t *Data);
void Usart_Transmit_Nchar(USART_TypeDef* USARTx, uint8_t *Data, uint8_t Len);
int fputc(int ch, FILE *f);

unsigned char Cmd_ComWriteRxBuf(unsigned char Data);
unsigned char Cmd_ComReadRxBuf(unsigned char *Data);
void Cmd_Com_Config(void);
void Debug_Com_Config(void);
void Com_IRQHandler_CallBack(void);

#endif
