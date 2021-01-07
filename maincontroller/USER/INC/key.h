#ifndef _key_
#define _key_

#include "sys.h"
#include "led8.h"
#include "delay.h"
#include "stm32f4xx_syscfg.h"
#include "elmo.h"


/********* Matrixkey **********/
#define S1    0x11
#define S2    0x12
#define S3    0x14
#define S4    0x18
#define S5    0x21
#define S6    0x22
#define S7    0x24
#define S8    0x28
#define S9    0x41
#define S10   0x42
#define S11   0x44
#define S12   0x48
#define S13   0x81
#define S14   0x82
#define S15   0x84
#define S16   0x88
/*********************************/


void Key_Configuration(void);
u8 Check_Matrixkey(void);
void Key_Ctrl(u8 id);
void EXTI9_5_IRQHandler(void);
void simulate_key(u8 id);

#endif
