#include "VESC.h"

VESCParam instrin;
VESCLimits VESClimit;
VESCArgum VESCargum;
VESC_Motor VESCmotor[4];

void VESCInit(void){
	{//电机内参
		instrin.POLE_PAIRS=7;
	}
	{//电机限制保护
		VESClimit.breakCurrent=10;
	}
	{//间值参数设置
		VESCargum.timeoutTicks=2000;//2000ms
	}
	for(int i=0;i<4;i++){
		VESCmotor[i].Argum=VESCargum;
		VESCmotor[i].Limits=VESClimit;
		VESCmotor[i].enable=DISABLE;
		VESCmotor[i].start=0;
		VESCmotor[i].mode=RPM;
		VESCmotor[i].ValueSet.speed=500;
		VESCmotor[i].ValueSet.current=1000;
		VESCmotor[i].ValueSet.angle=0;
		VESCmotor[i].ValueSet.duty=50;
		VESCmotor[i].Param=instrin;
	}
}

void VESC_Set_Duty_Cycle(u8 controller_ID,float duty_cycle,u8 InConGrpFlag){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		s32 temp=(s32)(duty_cycle*1000);
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0xf0000000|((u32)CAN_PACKET_SET_DUTY<<8)|controller_ID;
		EncodeS32Data(&temp,&Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]);
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.rear=Rear2;
}

void VESC_Set_Speed(u8 controller_ID,s32 speed,u8 InConGrpFlag){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		s32 temp=(s32)(speed*VESCmotor[controller_ID].Param.POLE_PAIRS);
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0xf0000000|((u32)CAN_PACKET_SET_RPM<<8)|controller_ID;
		EncodeS32Data(&temp,&Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]);
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.rear=Rear2;
}

void VESC_Set_Current(u8 controller_ID,float current,u8 InConGrpFlag){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		s32 temp=(s32)(current*1000);
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0xf0000000|((u32)CAN_PACKET_SET_CURRENT<<8)|controller_ID;
		EncodeS32Data(&temp,&Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]);
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.rear=Rear2;
}

void VESC_Set_Brake_Current(u8 controller_ID,float brake_current,u8 InConGrpFlag){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		s32 temp=(s32)(brake_current*1000);
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0xf0000000|((u32)CAN_PACKET_SET_CURRENT_BRAKE<<8)|controller_ID;
		EncodeS32Data(&temp,&Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]);
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.rear=Rear2;
}

void VESC_Set_Handbrake_Current(u8 controller_ID,float handbrake_current,u8 InConGrpFlag){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		s32 temp=(s32)(handbrake_current*1000);
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0xf0000000|((u32)CAN_PACKET_SET_CURRENT_HANDBRAKE<<8)|controller_ID;
		EncodeS32Data(&temp,&Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]);
//		ChangeData(&Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0],&Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]);
//		ChangeData(&Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1],&Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]);
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.rear=Rear2;
}

void VESC_Set_Position(u8 controller_ID,float angle,u8 InConGrpFlag){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		s32 temp=(s32)(angle*1000000);
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0xf0000000|((u32)CAN_PACKET_SET_POS<<8)|controller_ID;
		EncodeS32Data(&temp,&Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]);
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.rear=Rear2;
}


