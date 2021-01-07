#ifndef _USART3_
#define _USART3_

#include "sys.h"
#include "stm32f4xx_usart.h"
#include "stdio.h"
#include "string.h"
#include "maroc.h"
#include "param.h"

void Usart3_Init(void);
void USART3_IRQHandler(void); 

extern vs32 Pulse_Abs;

#endif
