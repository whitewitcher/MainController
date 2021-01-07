#include "can1.h"

void CAN1_Init(void){
	CAN_InitTypeDef CAN_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);	
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	CAN_InitStruct.CAN_Mode=CAN_Mode_Normal;
	CAN_InitStruct.CAN_Prescaler=3;
	CAN_InitStruct.CAN_BS1=CAN_BS1_9tq;
	CAN_InitStruct.CAN_BS2=CAN_BS2_4tq;
	CAN_InitStruct.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStruct.CAN_ABOM=DISABLE;
	CAN_InitStruct.CAN_AWUM=DISABLE;
	CAN_InitStruct.CAN_NART=DISABLE;
	CAN_InitStruct.CAN_RFLM=DISABLE;
	CAN_InitStruct.CAN_TXFP=DISABLE;
	CAN_InitStruct.CAN_TTCM=DISABLE;
	CAN_Init(CAN1,&CAN_InitStruct);
	
	CAN_FilterInitStruct.CAN_FilterNumber=0;
	CAN_FilterInitStruct.CAN_FilterMode=CAN_FilterMode_IdList;
	CAN_FilterInitStruct.CAN_FilterIdHigh=0x305<<5;
	CAN_FilterInitStruct.CAN_FilterIdLow=0x306<<5;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh=0x307<<5;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow=0x308<<5;
	CAN_FilterInitStruct.CAN_FilterScale=CAN_FilterScale_16bit;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
	CAN_FilterInitStruct.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStruct);
	
	CAN_FilterInitStruct.CAN_FilterNumber=1;
	CAN_FilterInitStruct.CAN_FilterMode=CAN_FilterMode_IdList;
	CAN_FilterInitStruct.CAN_FilterIdHigh=0x320<<5;
	CAN_FilterInitStruct.CAN_FilterIdLow=0x106<<5;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh=0x107<<5;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow=0x108<<5;
	CAN_FilterInitStruct.CAN_FilterScale=CAN_FilterScale_16bit;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
	CAN_FilterInitStruct.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStruct);
		
	NVIC_InitStruct.NVIC_IRQChannel=CAN1_RX0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
		
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
	//CAN_ITConfig(CAN1,CAN_IT_FMP1,ENABLE);
}

static void answer_master(CanRxMsg *RxMessage){
	if(QueueFull(&Can1_Sendqueue))
	{
		flag.Can1SendqueueFULL++;
		return;
	}
	else
	{

		Can1_Sendqueue.CANSentData[Can1_Sendqueue.rear].ID=RxMessage->StdId-0x80;
		Can1_Sendqueue.CANSentData[Can1_Sendqueue.rear].DLC=RxMessage->DLC;
		memcpy(&Can1_Sendqueue.CANSentData[Can1_Sendqueue.rear].Data[0],&RxMessage->Data[0],sizeof(RxMessage->Data[0])*RxMessage->DLC);
		Can1_Sendqueue.CANSentData[Can1_Sendqueue.rear].InConGrpFlag=1;
	}
	Can1_Sendqueue.rear=Rear1;
}

static void CAN1_Feedback_Deel(MesgControlGrpTypeDef* Can_MesgSentList)
{
	Can_MesgSentList->ReceiveNumber+=1;
	Can_MesgSentList->TimeOut=0;
	Can_MesgSentList->SendSem--;
	Can_MesgSentList->SentQueue.front=(Can_MesgSentList->SentQueue.front+1)%CAN_QUEUESIZE;
}

void CAN1_RX0_IRQHandler(void){ 
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)){
		CanRxMsg RxMessage;
		CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
		if(RxMessage.StdId >=0x305 && RxMessage.StdId<=0x308 && RxMessage.RTR==0){
			u32 id =RxMessage.StdId-0x305;
			u16 u16temp;s16 s16temp;
			
			if(RxMessage.Data[0]=='C' && RxMessage.Data[1]=='W' && RxMessage.Data[0]=='H' && RxMessage.Data[1]=='U'){
				CAN1_Feedback_Deel(&Can1_MesgSentList[id]);
				answer_master(&RxMessage);				
			}
			else if(RxMessage.Data[0]=='M' && RxMessage.Data[1]=='O'){
				if(RxMessage.Data[2]==0){
					motor[id].start=0;
					motor[id].enable=0;
					CAN1_Feedback_Deel(&Can1_MesgSentList[id]);
					answer_master(&RxMessage);
				}
				else if(RxMessage.Data[2]==1){
					motor[id].mode=position;
					motor[id].enable=1;
					motor[id].start=1;
					CAN1_Feedback_Deel(&Can1_MesgSentList[id]);
					answer_master(&RxMessage);
				}
			}
			else if(RxMessage.Data[0]=='B' && RxMessage.Data[1]=='G'){
				motor[id].start=1;
				CAN1_Feedback_Deel(&Can1_MesgSentList[id]);
				answer_master(&RxMessage);
			}
			else if(RxMessage.Data[0]=='S' && RxMessage.Data[1]=='T'){
				motor[id].start=0;
				CAN1_Feedback_Deel(&Can1_MesgSentList[id]);
				answer_master(&RxMessage);
			}
			else if(RxMessage.Data[0]=='S' && RxMessage.Data[1]=='P'){
				DecodeS16Data(&u16temp,&RxMessage.Data[2]);
				motor[id].Limits.PosSpLimit=u16temp;
				CAN1_Feedback_Deel(&Can1_MesgSentList[id]);
				answer_master(&RxMessage);
			}
			else if(RxMessage.Data[0]=='C' && RxMessage.Data[1]=='H'){
				DecodeS16Data(&s16temp,&RxMessage.Data[2]);
				motor[id].ValueSet.angle=s16temp*motor[id].Param.GearRatio/30.f;
				motor[id].start=1;
				CAN1_Feedback_Deel(&Can1_MesgSentList[id]);
				answer_master(&RxMessage);
			}
		}
		else if(RxMessage.StdId ==0x320 && RxMessage.RTR==0){
			u32 id =0x00;
			s16 s16temp[4];
			DecodeS16Data(&s16temp[0],&RxMessage.Data[0]);
			DecodeS16Data(&s16temp[1],&RxMessage.Data[2]);
			DecodeS16Data(&s16temp[2],&RxMessage.Data[4]);
			DecodeS16Data(&s16temp[3],&RxMessage.Data[6]);
			
			for(id=0;id<4;id++){
				motor[id].ValueSet.angle=s16temp[id]*motor[id].Param.GearRatio/30.f;
				motor[id].mode=position;
				motor[id].start=1;
			}
		}
	}
	CAN_ClearFlag(CAN1,CAN_IT_FMP0);
}


