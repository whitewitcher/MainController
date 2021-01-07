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
  *@brief 报文控制表
  */
#define CAN_QUEUESIZE           30      //发送报文队列长度
#define CAN_HAVESENDQUEUESIZE   5       //已发送报文队列长度
#define CAN1_NodeNumber         6u      //不要超过8（故障标志位只有8位）
#define CAN2_NodeNumber         6u      //不要超过8（故障标志位只有8位）
#define Rear1	((Can1_Sendqueue.rear+1)%CAN_QUEUESIZE)
#define Rear2	((Can2_Sendqueue.rear+1)%CAN_QUEUESIZE)

/****CAN报文发送 数据结构体****/
typedef struct
{
	u32 ID;
	u8 	DLC;
	u8 	Data[8];
	u8 	InConGrpFlag;
}CANTxDataTypeDef;

//CAN报文 队列结构体
typedef struct{
	u16 front,rear;
	CANTxDataTypeDef CANSentData[CAN_QUEUESIZE];
}CANQueueTypedef;

//CAN流程号结构体
typedef struct {
	vu8 order_valid; //当前有效报文流程号
	vu8 order_real;  //当前报文流程号
	vu8 status;      //当前流程状态
    /*0x01是否拥有	0x02是否正确	0x04超前或滞后	0x80是否矫正*/
}CANListTypedef;

/****报文控制 总控制表****/
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
