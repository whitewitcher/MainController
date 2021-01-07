#include "tim3.h"

void TIM3_Init(void){
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); 
	
	TIM_InitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_InitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period=9;
	TIM_InitStruct.TIM_Prescaler=8399;
	TIM_TimeBaseInit(TIM3,&TIM_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	

}

void TIM3_IRQHandler(void){
	if(TIM_GetFlagStatus(TIM3,TIM_IT_Update)){
		#ifdef USE_DJI
		motor_get_position();   //DJ电机位置计算
		for(int id=0;id<8;id++){
			if(motor[id].enable){
				if(motor[id].start){
					switch(motor[id].mode){
						case current:  						     break;
						case RPM:       motor_speed_PID(id);     break;
						case position:  motor_position_PID(id);  break;
						case zero:      motor_zero(id);          break;
						default: break;
					}				
				}
				else motor_position_PID(id);
			}
			
			if(motor[id].start && OSTimeGet()-motor[id].Argum.lastRxTim>motor[id].Argum.timeoutTicks){
				if(++motor[id].Argum.timeoutCnt>10){
					motor[id].Statue.stuck=true;
					motor[id].Argum.timeoutCnt=0;
				}
				else motor[id].Statue.stuck=false;
			}
			else motor[id].Argum.timeoutCnt=0;
			motor[id].Argum.lastRxTim =	OSTimeGet();
			
			Tx_CurrentInput(id);   //发送DJ电机电流值
		
		}	
		#endif
		
	}
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
}

