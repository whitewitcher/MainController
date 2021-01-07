#include "tim2.h"

void TIM2_Init(void){
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	TIM_InitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_InitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period=9;
	TIM_InitStruct.TIM_Prescaler=8399;
	TIM_TimeBaseInit(TIM2,&TIM_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
}

void TIM2_IRQHandler(void){
	if(TIM_GetFlagStatus(TIM2,TIM_IT_Update)){
		#ifdef USE_VESC
		for(int id=0;id<4;id++){
			if(VESCmotor[id].enable){
				if(VESCmotor[id].start){
					switch(VESCmotor[id].mode){
						case current:	VESC_Set_Current(id+1,VESCmotor[id].ValueSet.current,0); 	break;
						case RPM:       VESC_Set_Speed(id+1,VESCmotor[id].ValueSet.speed,0);     break;
						case position:  VESC_Set_Position(id+1,VESCmotor[id].ValueSet.angle,0);  break;
						case duty:      VESC_Set_Duty_Cycle(id+1,VESCmotor[id].ValueSet.duty,0);          break;
						default: break;
					}				
				}
				else VESC_Set_Handbrake_Current(id+1,VESCmotor[id].Limits.breakCurrent,0);
			}
			
			if(VESCmotor[id].start && OSTimeGet()-VESCmotor[id].Argum.lastRxTim>VESCmotor[id].Argum.timeoutTicks){
				if(++VESCmotor[id].Argum.timeoutCnt>10){
					VESCmotor[id].Statue.stuck=true;
					VESCmotor[id].Argum.timeoutCnt=0;
				}
				else VESCmotor[id].Statue.stuck=false;
			}
			else VESCmotor[id].Argum.timeoutCnt=0;
			VESCmotor[id].Argum.lastRxTim =	OSTimeGet();
		}	
		#endif
		
	}
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
}
