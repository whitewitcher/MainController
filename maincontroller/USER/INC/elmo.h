#ifndef _elmo_
#define _elmo_

#include "sys.h"
#include "stdarg.h"
#include "param.h"
#include "maroc.h"
#include "motor.h"
#include "canctrlist.h"


#define GetData	1
#define	SetData 2

/****ELMO CAN����ID****/
#define Elmo_Motor1_RX	0x281
#define Elmo_Motor2_RX	0x282
#define Elmo_Motor3_RX	0x283
#define Elmo_Motor4_RX	0x284
#define Elmo_Motor5_RX	0x285
#define Elmo_Motor6_RX	0x286
#define Elmo_Motor7_RX	0x287
#define Elmo_Motor8_RX	0x288
#define Elmo_Motor1_TX	0x301
#define Elmo_Motor2_TX	0x302
#define Elmo_Motor3_TX	0x303
#define Elmo_Motor4_TX	0x304
#define Elmo_Motor5_TX	0x305
#define Elmo_Motor6_TX	0x306
#define Elmo_Motor7_TX	0x307
#define Elmo_Motor8_TX	0x308
#define Elmo_Motor1_error 0x81
#define Elmo_Motor2_error 0x82
#define Elmo_Motor3_error 0x83
#define Elmo_Motor4_error 0x84
#define Elmo_Motor5_error 0x85
#define Elmo_Motor6_error 0x86
#define Elmo_Motor7_error 0x87
#define Elmo_Motor8_error 0x88

typedef struct{
	u16  Pulse;
	float  RATIO;
	float  GearRatio;
}ElmoParam;

typedef struct{
	vs16  angle;
	vs16  current;
	vs32  speed;
	vs32  pulse;
}ElmoValue;

typedef struct{
	bool PosLimit_ON;
	float AngleLimit;
	
	bool PositionSpeedLimit_ON;
	vs32  PosSpLimit;
	
	bool RealseWhenStuck_ON;
	vs32  ZeroSpLimit;
	s16  ZeroCurrentLimit;
}ElmoLimits;

typedef struct{
	bool arrived;
	bool zero;
	bool stuck;
	bool timeout;
	bool overspeed;
}ElmoStatus;

typedef struct{
	u32 maxPulse;//�����ת����,[����������ת�Ƕ�]
//    u16 zeroCnt;//Ѱ���ü���ֵ��[�����Ѱ��״̬zero]
//    vu16 stuckCnt;//��ת�ü���ֵ��[����ڶ�ת״̬stuck]
    u8  timeoutCnt;//��ʱ�ü���ֵ��[����ڳ�ʱ״̬timeout]
    u32 lastRxTim;//�ϴν��յ����ĵ�ϵͳʱ�䣬[����ڳ�ʱ״̬timeout]
    u16 timeoutTicks;//�жϳ�ʱ��ϵͳ��������[����ڳ�ʱ״̬timeout]
}ElmoArgum;

typedef struct{
	bool enable;
	bool start;
	u8 mode;
	ElmoLimits Limits;
	ElmoParam  Param;
	ElmoValue  ValueSet,ValueReal;
	ElmoStatus Statue;
	ElmoArgum  Argum;
}Elmo;

extern Elmo ElmoMotor[5];

void ElmoInit(CAN_TypeDef* CANx);
void ElmoMotorInit(void);
void MO(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag);   //����/����
void BG(u32 ID);                   //��ʼ�˶�
void AC(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag);   //���ٶ�
void DC(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag);   //���ٶ�
void JV(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag);   //�����˶��ٶ�
void IQ(u32 ID);                   //�й�����
void PA(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag);   //����λ��
void PE(u32 ID);                   //λ�����
void PR_(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag);                   //���λ��
void PX(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag);   //��λ��
void SD(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag);   //ֹͣ���ٶ�
void SP(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag);   //PTP�ٶ�
void ST(u32 ID);                   //ֹͣ
void TC(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag);   //����ת�أ�������
void UM(u32 ID,u8 cmd,s32 data,u8 InConGrpFlag);   //����ģʽ
void VX(u32 ID);                   //������



#endif
