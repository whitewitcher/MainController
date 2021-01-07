#ifndef _BEEP_H
#define _BEEP_H

#include "sys.h"
#include "delay.h"
#include "includes.h"

#define BEEP_ON     	 GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define BEEP_OFF     	 GPIO_ResetBits(GPIOA,GPIO_Pin_8)
#define Use_C

#ifdef Use_C
#define music1 523
#define music2 587
#define music3 659
#define music4 698
#define music5 784
#define music6 880
#define music7 988
#endif

#define setARR(arr) {TIM1->ARR=400000/arr;}
#define setCRR1     {TIM1->CCR1=TIM1->ARR/2;}

void Beep_Configuration(int arr,int psc);
void Beep_Show(u8 num);
void Beep_Music(void);

#endif
