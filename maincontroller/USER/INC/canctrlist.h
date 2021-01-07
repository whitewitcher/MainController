#ifndef _canctrlist_
#define _canctrlist_

#include "sys.h"
#include "includes.h"
#include "param.h"
#include "string.h"
#include "beep.h"

#define CAN_1 (u8)(0x01<<0)
#define CAN_2 (u8)(0x01<<1)

/**
  *@brief ���Ŀ��Ʊ�
  */
#define CAN_QUEUESIZE           30      //���ͱ��Ķ��г���
#define CAN_HAVESENDQUEUESIZE   5       //�ѷ��ͱ��Ķ��г���
#define CAN1_NodeNumber         6u      //��Ҫ����8�����ϱ�־λֻ��8λ��
#define CAN2_NodeNumber         6u      //��Ҫ����8�����ϱ�־λֻ��8λ��
#define Rear1	((Can1_Sendqueue.rear+1)%CAN_QUEUESIZE)
#define Rear2	((Can2_Sendqueue.rear+1)%CAN_QUEUESIZE)

/****CAN���ķ��� ���ݽṹ��****/
typedef struct
{
	u32 ID;
	u8 	DLC;
	u8 	Data[8];
	u8 	InConGrpFlag;
}CANTxDataTypeDef;

//CAN���� ���нṹ��
typedef struct{
	u16 front,rear;
	CANTxDataTypeDef CANSentData[CAN_QUEUESIZE];
}CANQueueTypedef;

//CAN���̺Žṹ��
typedef struct {
	vu8 order_valid; //��ǰ��Ч�������̺�
	vu8 order_real;  //��ǰ�������̺�
	vu8 status;      //��ǰ����״̬
    /*0x01�Ƿ�ӵ��	0x02�Ƿ���ȷ	0x04��ǰ���ͺ�	0x80�Ƿ����*/
}CANListTypedef;

/****���Ŀ��� �ܿ��Ʊ�****/
typedef struct
{
	s32 SendNumber;
	s32	ReceiveNumber;
	u32 QUEUEFullTimeout;	
	u8  TimeOut;	
	s32	SendSem;	
	CANQueueTypedef SentQueue;              
}MesgControlGrpTypeDef;

extern CANListTypedef 			CANList;
extern CANTxDataTypeDef 	    Can_Sendtruct;
extern CANQueueTypedef 	 		Can_Sentqueue,Can1_Sendqueue,Can2_Sendqueue;
extern MesgControlGrpTypeDef 	Can1_MesgSentList[CAN1_NodeNumber],Can2_MesgSentList[CAN2_NodeNumber];

void Can_SendqueueInit(void);
bool QueueEmpty(CANQueueTypedef* can_queue);
bool QueueFull(CANQueueTypedef* can_queue);
void Can_DeQueue(CAN_TypeDef* CANx , CANQueueTypedef *can_queue);
void InitCANControlList(MesgControlGrpTypeDef* CAN_MesgSentList , u8 CAN_x);
void Can_MesgCtrlList(MesgControlGrpTypeDef* CAN_MesgSentList,CANQueueTypedef* can_queue , u8 CAN_x);
void TraversalControlList(MesgControlGrpTypeDef* CAN_MesgSentList,CANQueueTypedef* can_queue , u8 CAN_x);

#endif
