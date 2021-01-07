#include "can2.h"

void CAN2_Init(void){
	CAN_InitTypeDef CAN_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_CAN2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_CAN2);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
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
	CAN_Init(CAN2,&CAN_InitStruct);
	    //大疆过滤器
	CAN_FilterInitStruct.CAN_FilterNumber=14;
	CAN_FilterInitStruct.CAN_FilterMode=CAN_FilterMode_IdList;
	CAN_FilterInitStruct.CAN_FilterIdHigh=0x201<<5;
	CAN_FilterInitStruct.CAN_FilterIdLow=0x202<<5;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh=0x203<<5;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow=0x204<<5;
	CAN_FilterInitStruct.CAN_FilterScale=CAN_FilterScale_16bit;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
	CAN_FilterInitStruct.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStruct);
	
	CAN_FilterInitStruct.CAN_FilterNumber=15;
	CAN_FilterInitStruct.CAN_FilterMode=CAN_FilterMode_IdList;
	CAN_FilterInitStruct.CAN_FilterIdHigh=0x205<<5;
	CAN_FilterInitStruct.CAN_FilterIdLow=0x206<<5;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh=0x207<<5;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow=0x208<<5;
	CAN_FilterInitStruct.CAN_FilterScale=CAN_FilterScale_16bit;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
	CAN_FilterInitStruct.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStruct);
	
	//Elmo
	CAN_FilterInitStruct.CAN_FilterNumber=16;
	CAN_FilterInitStruct.CAN_FilterMode=CAN_FilterMode_IdList;
	CAN_FilterInitStruct.CAN_FilterIdHigh=Elmo_Motor1_RX<<5;
	CAN_FilterInitStruct.CAN_FilterIdLow=Elmo_Motor2_RX<<5;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh=Elmo_Motor3_RX<<5;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow=Elmo_Motor4_RX<<5;
	CAN_FilterInitStruct.CAN_FilterScale=CAN_FilterScale_16bit;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment=CAN_Filter_FIFO1;
	CAN_FilterInitStruct.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStruct);
	
	CAN_FilterInitStruct.CAN_FilterNumber=17;
	CAN_FilterInitStruct.CAN_FilterMode=CAN_FilterMode_IdList;
	CAN_FilterInitStruct.CAN_FilterIdHigh=Elmo_Motor5_RX<<5;
	CAN_FilterInitStruct.CAN_FilterIdLow=Elmo_Motor6_RX<<5;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh=Elmo_Motor7_RX<<5;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow=Elmo_Motor8_RX<<5;
	CAN_FilterInitStruct.CAN_FilterScale=CAN_FilterScale_16bit;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment=CAN_Filter_FIFO1;
	CAN_FilterInitStruct.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStruct);
	
	CAN_FilterInitStruct.CAN_FilterNumber=18;
	CAN_FilterInitStruct.CAN_FilterMode=CAN_FilterMode_IdList;
	CAN_FilterInitStruct.CAN_FilterIdHigh=Elmo_Motor1_error<<5;
	CAN_FilterInitStruct.CAN_FilterIdLow=Elmo_Motor2_error<<5;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh=Elmo_Motor3_error<<5;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow=Elmo_Motor4_error<<5;
	CAN_FilterInitStruct.CAN_FilterScale=CAN_FilterScale_16bit;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment=CAN_Filter_FIFO1;
	CAN_FilterInitStruct.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStruct);
	
	CAN_FilterInitStruct.CAN_FilterNumber=19;
	CAN_FilterInitStruct.CAN_FilterMode=CAN_FilterMode_IdList;
	CAN_FilterInitStruct.CAN_FilterIdHigh=Elmo_Motor5_error<<5;
	CAN_FilterInitStruct.CAN_FilterIdLow=Elmo_Motor6_error<<5;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh=Elmo_Motor7_error<<5;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow=Elmo_Motor8_error<<5;
	CAN_FilterInitStruct.CAN_FilterScale=CAN_FilterScale_16bit;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment=CAN_Filter_FIFO1;
	CAN_FilterInitStruct.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=CAN2_RX0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=CAN2_RX1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);
		
	CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);
	CAN_ITConfig(CAN2,CAN_IT_FMP1,ENABLE);
}

void CAN2_RX0_IRQHandler(void){
	if(CAN_GetITStatus(CAN2,CAN_IT_FMP0)){
		CanRxMsg RxMessage;
		CAN_Receive(CAN2,CAN_FIFO0,&RxMessage);
		CAN_ClearITPendingBit(CAN2,CAN_IT_FMP0);
		if(RxMessage.StdId >=0x201 && RxMessage.StdId<=0x208 && RxMessage.RTR==0){
			u32 id =RxMessage.StdId-0x201;
			ChangeData(&RxMessage.Data[0],&RxMessage.Data[1]);
			ChangeData(&RxMessage.Data[2],&RxMessage.Data[3]);
			ChangeData(&RxMessage.Data[4],&RxMessage.Data[5]);
			DecodeS16Data(&motor[id].ValueReal.pulseRead,&RxMessage.Data[0]);
			DecodeS16Data(&motor[id].ValueReal.speed,&RxMessage.Data[2]);
			DecodeS16Data(&motor[id].ValueReal.current,&RxMessage.Data[4]);
			
	//		motor[id].ValueReal.pulseRead=RxMessage.Data[0]<<8|RxMessage.Data[1];
	//		motor[id].ValueReal.speed=RxMessage.Data[2]<<8|RxMessage.Data[3];
	//		motor[id].ValueReal.current=RxMessage.Data[4]<<8|RxMessage.Data[5];	
			motor[id].ValueReal.angle =motor[id].ValueReal.pulse *360.f /(motor[id].Param.RATIO * motor[id].Param.Pulse);
			
		}
		
	}
	
	CAN_ClearFlag(CAN2,CAN_IT_FMP0);

}

/****接收到报文反馈后控制表处理****/
static void Elmo_Feedback_Deel(MesgControlGrpTypeDef* Can_MesgSentList)
{
	Can_MesgSentList->ReceiveNumber+=1;
	Can_MesgSentList->TimeOut=0;
	Can_MesgSentList->SendSem--;
	Can_MesgSentList->SentQueue.front=(Can_MesgSentList->SentQueue.front+1)%CAN_QUEUESIZE;
}


void CAN2_RX1_IRQHandler(void){
	if(CAN_GetITStatus(CAN2,CAN_IT_FMP1)){
		CanRxMsg RxMessage;                  
		CAN_Receive(CAN2,CAN_FIFO1,&RxMessage);
		CAN_ClearITPendingBit(CAN2,CAN_IT_FMP1);
		if(RxMessage.StdId>0x280 && RxMessage.RTR==CAN_RTR_Data && RxMessage.IDE==CAN_Id_Standard){
			u8 id=RxMessage.StdId-0x281;
			if(	(RxMessage.Data[0]=='A'&&RxMessage.Data[1]=='C'&&(RxMessage.Data[3]&BIT6)!=1)|
			(RxMessage.Data[0]=='B'&&RxMessage.Data[1]=='G'&&(RxMessage.Data[3]&BIT6)!=1)|
			(RxMessage.Data[0]=='J'&&RxMessage.Data[1]=='V'&&(RxMessage.Data[3]&BIT6)!=1)|
			(RxMessage.Data[0]=='M'&&RxMessage.Data[1]=='O'&&(RxMessage.Data[3]&BIT6)!=1)|
			(RxMessage.Data[0]=='P'&&RxMessage.Data[1]=='R'&&(RxMessage.Data[3]&BIT6)!=1)|
			(RxMessage.Data[0]=='S'&&RxMessage.Data[1]=='D'&&(RxMessage.Data[3]&BIT6)!=1)|
			(RxMessage.Data[0]=='S'&&RxMessage.Data[1]=='T'&&(RxMessage.Data[3]&BIT6)!=1)|
			(RxMessage.Data[0]=='T'&&RxMessage.Data[1]=='C'&&(RxMessage.Data[3]&BIT6)!=1)|
			(RxMessage.Data[0]=='U'&&RxMessage.Data[1]=='M'&&(RxMessage.Data[3]&BIT6)!=1) )
			Elmo_Feedback_Deel(&Can2_MesgSentList[id]);
			if(RxMessage.Data[0]=='M' && RxMessage.Data[1]=='O')
				ElmoMotor[id].enable=RxMessage.Data[4];
			if(RxMessage.Data[0]=='U' && RxMessage.Data[1]=='M')
				ElmoMotor[id].mode=RxMessage.Data[4];
			if(RxMessage.Data[0]=='V' && RxMessage.Data[1]=='X'){
				DecodeS32Data(&ElmoMotor[id].ValueReal.speed,&RxMessage.Data[4]);
				ElmoMotor[id].ValueReal.speed /= ElmoMotor[id].Param.Pulse/60;
			}
			if(RxMessage.Data[0]=='P' && RxMessage.Data[1]=='X'){
				DecodeS32Data(&ElmoMotor[id].ValueReal.pulse,&RxMessage.Data[4]);
				ElmoMotor[id].ValueReal.angle=ElmoMotor[id].ValueReal.pulse*(360.f/ElmoMotor[id].Param.RATIO)/ElmoMotor[id].Param.Pulse;
			}
			if(RxMessage.Data[0]=='I' && RxMessage.Data[1]=='Q')
				DecodeS32Data(&ElmoMotor[id].ValueReal.current,&RxMessage.Data[4]);
			
			
			ElmoMotor[id].Argum.lastRxTim =	OSTimeGet();
		}
		if(RxMessage.StdId>0x80 && RxMessage.StdId<0x89 && RxMessage.RTR==CAN_RTR_Data){
			u8 id=RxMessage.StdId-0x81;
			if(RxMessage.Data[0]==0x11&&RxMessage.Data[1]==0x83){
				insertError(ErrorList,ELMOERROR|EMERGENCY|OVERCURRENT|id);
				Beep_Show(3);
			}
			else if(RxMessage.Data[0]==0x80&&RxMessage.Data[1]==0x84){
				insertError(ErrorList,ELMOERROR|EMERGENCY|SPEEDFAULT|id);
				Beep_Show(3);
			}
			else if(RxMessage.Data[0]==0x81&&RxMessage.Data[1]==0x84){
				insertError(ErrorList,ELMOERROR|EMERGENCY|OVERSPEED|id);
				Beep_Show(3);
			}
			else if(RxMessage.Data[0]==0x21&&RxMessage.Data[1]==0x71){
				insertError(ErrorList,ELMOERROR|EMERGENCY|STUCK|id);
				Beep_Show(3);
			}
			else if(RxMessage.Data[0]==0x80&&RxMessage.Data[1]==0x86){
				insertError(ErrorList,ELMOERROR|EMERGENCY|OVERPOSITIION|id);
				Beep_Show(3);
			}
			else if(RxMessage.Data[0]==0x10&&RxMessage.Data[1]==0xFF){
				insertError(ErrorList,ELMOERROR|EMERGENCY|CANTSTART|id);
				Beep_Show(3);
			}
			else if(RxMessage.Data[0]==0x10&&RxMessage.Data[1]==0x43){
				insertError(ErrorList,ELMOERROR|EMERGENCY|OVERHEAT|id);
				Beep_Show(3);
			}
			ElmoMotor[id].Argum.lastRxTim =	OSTimeGet();
		}
	}
	
	CAN_ClearFlag(CAN2,CAN_IT_FMP1);
}


	static CanTxMsg TxMessage;	//静态变量 四个电机一起发送 前一个信息不能被清除
void Tx_CurrentInput(u8 id){
	PEAK(motor[id].ValueSet.current,motor[id].Param.MaxCurrent);
		
	if(!motor[id].start||!motor[id].enable) motor[id].ValueSet.current=0;
	
	if(id<4) TxMessage.StdId=0x200;
	else     TxMessage.StdId=0x1FF;
	TxMessage.RTR=CAN_RTR_Data;    //0x00
	TxMessage.IDE=CAN_Id_Standard; //0x00
	TxMessage.DLC=8;
	u8 tempid =2 *(id %4);
	EncodeS16Data(&motor[id].ValueSet.current,&TxMessage.Data[tempid]);
	ChangeData(&TxMessage.Data[tempid],&TxMessage.Data[tempid+1]);
	if(id==3 || id==7)   //全部电机循环发送时可以只在3、7发
		CAN_Transmit(CAN2,&TxMessage);
}

void valveCtrl(u8 ID,bool statue){
	CanTxMsg tx_message;
	tx_message.ExtId = 0x00030101;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.IDE = CAN_Id_Extended;
	tx_message.DLC = 4;
	tx_message.Data[0] = 'V';
	tx_message.Data[1] = 'L';
	tx_message.Data[2] = statue;
	tx_message.Data[3] = ID;
	CAN_Transmit(CAN2,&tx_message);
	flag.valve[ID] = statue;
}
