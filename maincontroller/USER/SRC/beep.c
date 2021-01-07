#include "beep.h"

void Beep_Configuration(int arr, int psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	//TIM_OCInitTypeDef TIM_OCInitStruct;
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

	//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_ResetBits(GPIOA, GPIO_Pin_8);

	/*	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_Period=400000/arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM2;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCIdleState=TIM_OCNIdleState_Set;
	TIM_OCInitStruct.TIM_Pulse=0;
	TIM_OC1Init(TIM1,&TIM_OCInitStruct);
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM1,ENABLE);*/
}

void Beep_Show(u8 num)
{
	for (int i = 0; i < num; i++)
	{
		BEEP_ON;
		Delay_ms(100);
		BEEP_OFF;
		Delay_ms(100);
	}
}

/*
void Beep_Music(void){
	TIM_SetCompare1(TIM1,TIM1->ARR/2);
	setARR(music1); setCRR1; Delay_ms(1000);
	TIM1->CCR1=0; Delay_ms(2000);
	setARR(music2); setCRR1; Delay_ms(1000);
	TIM1->CCR1=0; Delay_ms(2000);
	setARR(music3); setCRR1; Delay_ms(1000);
	TIM1->CCR1=0; Delay_ms(2000);
	setARR(music4); setCRR1; Delay_ms(1000);
	TIM1->CCR1=0; Delay_ms(2000);
	setARR(music5); setCRR1; Delay_ms(1000);
	TIM1->CCR1=0; Delay_ms(2000);
	setARR(music6); setCRR1; Delay_ms(1000);
	TIM1->CCR1=0; Delay_ms(2000);
	setARR(music7); setCRR1; Delay_ms(1000);
	TIM1->CCR1=0; Delay_ms(2000);
}*/
