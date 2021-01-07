#ifndef __USART1_H
#define __USART1_H

#include "sys.h"
#include "stdio.h"
#include "string.h"
#include "param.h"
#include "stm32f4xx_usart.h"
#include "key.h"
#include "stm32f4xx_dma.h"

void Usart1_Init(void);
void USART1_Send(u8 *data,u8 count);
void DMA2_Stream7_IRQHandler(void);

#endif
