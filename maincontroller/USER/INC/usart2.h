#ifndef __USART2_H
#define __USART2_H

#include "sys.h"
#include "stdio.h"
#include "string.h"
#include "param.h"
#include "stm32f4xx_usart.h"
#include "key.h"
#include "stm32f4xx_dma.h"
#include "param.h"

void Usart2_Init(void);
void UsartLCDshow(void);
void UsartLCDReceive(void);
void USART2_IRQHandler(void); 
void USART2_Send(u8 count);
void DMA1_Stream6_IRQHandler(void);

extern u8 usart2_tx[256];


#endif
