#include "canctrlist.h"

CANListTypedef 			CANList;
CANTxDataTypeDef 	    Can_Sendtruct;
CANQueueTypedef 	 	Can_Sentqueue,Can1_Sendqueue,Can2_Sendqueue;
MesgControlGrpTypeDef 	Can1_MesgSentList[CAN1_NodeNumber],Can2_MesgSentList[CAN2_NodeNumber];

/****基本数据****/
u8 CanSendCount=0;
int Can1_Tx_NoMailBox=0;
int Can2_Tx_NoMailBox=0;

void Can_SendqueueInit(void){
	Can1_Sendqueue.front=Can1_Sendqueue.rear=0;
	Can2_Sendqueue.front=Can2_Sendqueue.rear=0;
}

bool QueueEmpty(CANQueueTypedef* can_queue){
	return (can_queue->front==can_queue->rear);
}

bool QueueFull(CANQueueTypedef* can_queue){
	return (((can_queue->rear+1) % CAN_QUEUESIZE)==can_queue->front);
}

void Can_DeQueue(CAN_TypeDef* CANx , CANQueueTypedef *can_queue){
	CanTxMsg TxMessage;
		CanSendCount=0;
	if(QueueEmpty(can_queue))
		flag.CanSendqueueEMPTY=1;
	else{
		if(can_queue->CANSentData[can_queue->front].ID<0x400)   //Elmo
		{
			TxMessage.IDE=CAN_ID_STD;
			TxMessage.StdId=can_queue->CANSentData[can_queue->front].ID;
		}
		else{
			if((can_queue->CANSentData[can_queue->front].ID & 0xf0000000)!=0)//VESC报文的标志
			{
				can_queue->CANSentData[can_queue->front].ID=can_queue->CANSentData[can_queue->front].ID & 0x0fffffff;
				ChangeData(&can_queue->CANSentData[can_queue->front].Data[0],&can_queue->CANSentData[can_queue->front].Data[3]);
				ChangeData(&can_queue->CANSentData[can_queue->front].Data[1],&can_queue->CANSentData[can_queue->front].Data[2]);

			}
			TxMessage.IDE=CAN_ID_EXT;
			TxMessage.ExtId=can_queue->CANSentData[can_queue->front].ID;
		}
		if(CAN1==CANx)	
			Can_MesgCtrlList(Can1_MesgSentList,&Can1_Sendqueue,CAN_1);
		else
			Can_MesgCtrlList(Can2_MesgSentList,&Can2_Sendqueue,CAN_2);
		
		TxMessage.RTR=CAN_RTR_Data;
		TxMessage.DLC= can_queue->CANSentData[can_queue->front].DLC;
		memcpy(&TxMessage.Data,&can_queue->CANSentData[can_queue->front].Data,sizeof(u8) * TxMessage.DLC);
		can_queue->front = (can_queue->front+1) % CAN_QUEUESIZE;
		
		while(CAN_Transmit(CANx,&TxMessage)==CAN_TxStatus_NoMailBox){
			CanSendCount++;
			if(CanSendCount>10){
				if (CANx==CAN1)
					Can1_Tx_NoMailBox++;
				else
					Can2_Tx_NoMailBox++;
				break;
			}
			
			if(Can1_Tx_NoMailBox>=5){
				Can1_Tx_NoMailBox=0;
				flag.led=error;
			}
			else if(Can2_Tx_NoMailBox>=5){
				Can2_Tx_NoMailBox=0;
				flag.led=error;
			}
		}
		
	}

}

void InitCANControlList(MesgControlGrpTypeDef* CAN_MesgSentList , u8 CAN_x){
	u8 CANNodeNumber=0;
	if(CAN_x==CAN_1) {
		CANNodeNumber=CAN1_NodeNumber;
		flag.Can1CtrlList_Enable=1;
	}
	else if(CAN_x==CAN_2) {
		CANNodeNumber=CAN2_NodeNumber;
		flag.Can2CtrlList_Enable=1;
	}
	
	for(int i=0;i<CANNodeNumber;i++){
		CAN_MesgSentList->QUEUEFullTimeout=0;
		CAN_MesgSentList->ReceiveNumber=0;
		CAN_MesgSentList->SendNumber=0;
		CAN_MesgSentList->SendSem=0;
		CAN_MesgSentList->SentQueue.CANSentData[0].InConGrpFlag=0;
		CAN_MesgSentList->TimeOut=0;
	}
}

void TraversalControlList(MesgControlGrpTypeDef* CAN_MesgSentList,CANQueueTypedef* can_queue , u8 CAN_x){
	u8 CANNodeNumber=0;
	if(CAN_x==CAN_1) CANNodeNumber=CAN1_NodeNumber;
	else if(CAN_x==CAN_2) CANNodeNumber=CAN2_NodeNumber;
	
	for(int i=0;i<CANNodeNumber;i++){
		if(CAN_MesgSentList[i].SendSem >=1){
			if(CAN_MesgSentList[i].TimeOut++>=30){
				if(QueueFull(can_queue)){
					CAN_MesgSentList[i].QUEUEFullTimeout++;
					return;
				}
				else if(QueueEmpty(can_queue)){
					CAN_MesgSentList[i].QUEUEFullTimeout=0;
					return;
				}
				else{
					CAN_MesgSentList[i].SendSem -=1;
					CAN_MesgSentList[i].QUEUEFullTimeout=0;
					CAN_MesgSentList[i].TimeOut=0;
					can_queue->CANSentData[can_queue->rear].ID=CAN_MesgSentList[i].SentQueue.CANSentData[CAN_MesgSentList[i].SentQueue.front].ID;
					can_queue->CANSentData[can_queue->rear].DLC=CAN_MesgSentList[i].SentQueue.CANSentData[CAN_MesgSentList[i].SentQueue.front].DLC;
					can_queue->CANSentData[can_queue->rear].InConGrpFlag=CAN_MesgSentList[i].SentQueue.CANSentData[CAN_MesgSentList[i].SentQueue.front].InConGrpFlag;
					memcpy(can_queue->CANSentData[can_queue->rear].Data,CAN_MesgSentList[i].SentQueue.CANSentData[CAN_MesgSentList[i].SentQueue.front].Data,
								sizeof(u8) *CAN_MesgSentList[i].SentQueue.CANSentData[CAN_MesgSentList[i].SentQueue.front].DLC);
				}
				can_queue->rear = (can_queue->rear+1)%CAN_QUEUESIZE;
				CAN_MesgSentList[i].SentQueue.front = (CAN_MesgSentList[i].SentQueue.front+1)%CAN_QUEUESIZE;
			}
			if(CAN_MesgSentList[i].QUEUEFullTimeout>10||(CAN_MesgSentList[i].SendNumber - CAN_MesgSentList[i].ReceiveNumber)>50)
			{
				Beep_Show(7);
				CAN_MesgSentList[i].SendSem=-10000;
				if(CAN_1 == CAN_x)
				{
					switch(i)
					{
						;
					}
				}
				else
				{
					switch(i)
					{
						case 0:flag.led=error;
							break;
						case 1:flag.led=error;
							break;
						default:
							break;
					}
				}
			 }			
		}		
	}
}

void Can_MesgCtrlList(MesgControlGrpTypeDef* CAN_MesgSentList,CANQueueTypedef* can_queue , u8 CAN_x){
	u8 CANNodeNumber=0;
	u16 ControlListID=0xFF;
	
	if (can_queue->CANSentData[can_queue->front].InConGrpFlag==0) return;
	
	if(CAN_x==CAN_1) {
		CANNodeNumber=CAN1_NodeNumber;
	}
	else if(CAN_x==CAN_2) {
		CANNodeNumber=CAN2_NodeNumber;
		ControlListID=can_queue->CANSentData[can_queue->front].ID-0x301;
	}
	if(ControlListID<CANNodeNumber){
		if(QueueFull(&CAN_MesgSentList[ControlListID].SentQueue )){
			if(CAN_x==CAN_1) flag.Can1SendqueueFULL++;
			else			 flag.Can2SendqueueFULL++;
		}
		else{
			CAN_MesgSentList[ControlListID].SendSem++;
			CAN_MesgSentList[ControlListID].SendNumber++;
			CAN_MesgSentList[ControlListID].SentQueue.CANSentData[CAN_MesgSentList[ControlListID].SentQueue.rear].ID=can_queue->CANSentData[can_queue->front].ID;
			CAN_MesgSentList[ControlListID].SentQueue.CANSentData[CAN_MesgSentList[ControlListID].SentQueue.rear].DLC=can_queue->CANSentData[can_queue->front].DLC;
			CAN_MesgSentList[ControlListID].SentQueue.CANSentData[CAN_MesgSentList[ControlListID].SentQueue.rear].InConGrpFlag=can_queue->CANSentData[can_queue->front].InConGrpFlag;
			memcpy(CAN_MesgSentList[ControlListID].SentQueue.CANSentData[CAN_MesgSentList[ControlListID].SentQueue.rear].Data,can_queue->CANSentData[can_queue->front].Data,
								sizeof(u8) *can_queue->CANSentData[can_queue->front].DLC);
			CAN_MesgSentList[ControlListID].SentQueue.rear = (CAN_MesgSentList[ControlListID].SentQueue.rear+1)%CAN_QUEUESIZE;
		
		}
	}
}


