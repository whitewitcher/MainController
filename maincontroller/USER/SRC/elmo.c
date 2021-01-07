#include "elmo.h"

ElmoLimits Limits;
ElmoParam  Flat90, U10, EC_4P_30;
ElmoArgum  Argum;
Elmo ElmoMotor[5];

void ElmoInit(CAN_TypeDef* CANx){
	CanTxMsg TxMessage;
	TxMessage.StdId=0x00;
	TxMessage.DLC=8;
	TxMessage.RTR=CAN_RTR_Data;
	TxMessage.IDE=CAN_Id_Standard;
	TxMessage.Data[0]=0x01;
	TxMessage.Data[1]=0x00;
	TxMessage.Data[2]=0x00;
	TxMessage.Data[3]=0x00;
	TxMessage.Data[4]=0x00;
	TxMessage.Data[5]=0x00;
	TxMessage.Data[6]=0x00;
	TxMessage.Data[7]=0x00;

	CAN_Transmit(CANx,&TxMessage);
}

void ElmoMotorInit(void){
	//电机内参
	Flat90.Pulse=16384;		
	Flat90.RATIO=1;
	U10.Pulse=16384;			
	U10.RATIO=1;
	EC_4P_30.Pulse=4000;	
	EC_4P_30.RATIO=169.f/9;
	
	Limits.PosLimit_ON=false;
	Limits.AngleLimit=0;
	Limits.PositionSpeedLimit_ON=true;
	Limits.PosSpLimit=800;
	Limits.RealseWhenStuck_ON=true;
	Limits.ZeroCurrentLimit=1000;
	Limits.ZeroSpLimit=500;
	
	Argum.timeoutTicks =1000;
	
	for(int i=0;i<5;i++){
		#ifdef USE_Flat90
		ElmoMotor[i].Param=Flat90;
		#endif
		#ifdef USE_U10
		ElmoMotor[i].Param=U10;
		#endif
		#ifdef USE_EC4P30
		ElmoMotor[i].Param=EC_4P_30;
		#endif
		
		ElmoMotor[i].Argum=Argum;
		ElmoMotor[i].Limits=Limits;
		ElmoMotor[i].enable=DISABLE;
		ElmoMotor[i].start=0;
		ElmoMotor[i].mode=RPM;
		ElmoMotor[i].ValueSet.speed=500;
		ElmoMotor[i].ValueSet.current=1000;
		ElmoMotor[i].ValueSet.angle=0;
		ElmoMotor[i].ValueSet.pulse=0;
	}
	
	
}

void MO(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0x300+ID;
		if(cmd==GetData){
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='M';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='O';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
		}
		else if(cmd==SetData){
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=8;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='M';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='O';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[4]=data;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[5]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[6]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[7]=0x00;
		}
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.rear=Rear2;
}

void BG(u32 ID){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		ElmoMotor[ID].start=true;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0x300+ID;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='B';
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='G';
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
	}
	Can2_Sendqueue.rear=Rear2;
}

void AC(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0x300+ID;
		if(cmd==GetData){
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='A';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='C';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
		}
		else if(cmd==SetData){
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=8;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='A';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='C';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
			EncodeS32Data(&data,&Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[4]);
		}
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.rear=Rear2;
}

void DC(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0x300+ID;
		if(cmd==GetData){
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='D';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='C';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
		}
		else if(cmd==SetData){
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=8;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='D';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='C';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
			EncodeS32Data(&data,&Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[4]);
		}
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.rear=Rear2;
}

void JV(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0x300+ID;
		if(cmd==GetData){
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='J';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='V';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
		}
		else if(cmd==SetData){
			int data_cnt=ElmoMotor[ID].Param.Pulse/60.f *data;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=8;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='J';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='V';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
			EncodeS32Data(&data_cnt,&Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[4]);
		}
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.rear=Rear2;
}

void IQ(u32 ID){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0x300+ID;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='I';
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='Q';
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x80;
	}
	Can2_Sendqueue.rear=Rear2;
}

void PA(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0x300+ID;
		if(cmd==GetData){
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='P';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='A';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
		}
		else if(cmd==SetData){
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=8;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='P';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='A';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
			EncodeS32Data(&data,&Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[4]);
		}
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.rear=Rear2;
}

void PE(u32 ID){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0x300+ID;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='P';
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='E';
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
	}
	Can2_Sendqueue.rear=Rear2;
}

void PR_(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0x300+ID;
		if(cmd==GetData){
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='P';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='R';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
		}
		else if(cmd==SetData){
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=8;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='P';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='R';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
			EncodeS32Data(&data,&Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[4]);
		}
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.rear=Rear2;
}

void PX(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0x300+ID;
		if(cmd==GetData){
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='P';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='X';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
		}
		else if(cmd==SetData){
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=8;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='P';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='X';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
			EncodeS32Data(&data,&Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[4]);
		}
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.rear=Rear2;
}

void SD(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0x300+ID;
		if(cmd==GetData){
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='S';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='D';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
		}
		else if(cmd==SetData){
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=8;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='S';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='D';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
			EncodeS32Data(&data,&Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[4]);
		}
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.rear=Rear2;
}

void SP(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0x300+ID;
		if(cmd==GetData){
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='S';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='P';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
		}
		else if(cmd==SetData){
			int data_cnt=ElmoMotor[ID].Param.Pulse/60.f *data;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=8;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='S';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='P';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
			EncodeS32Data(&data_cnt,&Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[4]);
		}
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.rear=Rear2;
}

void ST(u32 ID){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		ElmoMotor[ID].start=false;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0x300+ID;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='S';
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='T';
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
	}
	Can2_Sendqueue.rear=Rear2;
}

void TC(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0x300+ID;
		if(cmd==GetData){
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='T';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='C';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
		}
		else if(cmd==SetData){
			data=data/100.f;  //???
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=8;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='T';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='C';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
			EncodeS32Data(&data,&Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[4]);
		}
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.rear=Rear2;
}

void UM(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0x300+ID;
		if(cmd==GetData){
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='U';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='M';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
		}
		else if(cmd==SetData){
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=8;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='U';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='M';
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
			Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
			EncodeS32Data(&data,&Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[4]);
		}
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.rear=Rear2;
}

void VX(u32 ID){
	if(QueueFull(&Can2_Sendqueue))
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].ID=0x300+ID;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].DLC=4;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[0]='V';
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[1]='X';
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[2]=0x00;
		Can2_Sendqueue.CANSentData[Can2_Sendqueue.rear].Data[3]=0x00;
	}
	Can2_Sendqueue.rear=Rear2;
}


