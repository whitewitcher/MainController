#ifndef _motor_
#define _motor_

#include "sys.h"
#include "stm32f4xx.h"
#include "includes.h"
#include "stdbool.h"
#include "param.h"
#include "PID.h"
#include "maroc.h"
#include "can1.h"
#include "can2.h"
#include "tim3.h"


enum MotorMode {duty, current, RPM, brake, zero, position, handbrake};

typedef struct{
	bool PosLimit_ON;
	float PosLimit;
	
	bool PositionSpeedLimit_ON;
	vs16 PosSpLimit;
	
	bool RealseWhenStuck_ON;
	vs16 ZeroSpLimit;
	u16  ZeroCurrentLimit;
}MotorLimits;

typedef struct{
	u16   Pulse;
	u16   RATIO;
	u16   MaxCurrent;
	float GearRatio;
}MotorParam;

typedef struct{
	vs32  angle;
	vs16  speed;
	vs16  current;
	vs32   pulse;
	vs16  pulseRead;
}MotorValue;

typedef struct{
	bool arrived;
	bool zero;
	bool stuck;
	bool timeout;
	bool overspeed;
	bool isSetZero;
}MotorStatus;

typedef struct{
	u32 maxPulse;//�����ת����,[����������ת�Ƕ�]
    vs32 lockPulse;//��λ���õ�ǰλ�ü�¼��[�����begin]
    s16 distance;//��ǰ����������ǰ�η��������ֵ��[������ۼ�����pulse]
    s32 difPulseSet;//��ǰ�ۼ��������趨�����ֵ
    u16 zeroCnt;//Ѱ���ü���ֵ��[�����Ѱ��״̬zero]
    vu16 stuckCnt;//��ת�ü���ֵ��[����ڶ�ת״̬stuck]
    u8  timeoutCnt;//��ʱ�ü���ֵ��[����ڳ�ʱ״̬timeout]
    u32 lastRxTim;//�ϴν��յ����ĵ�ϵͳʱ�䣬[����ڳ�ʱ״̬timeout]
    u16 timeoutTicks;//�жϳ�ʱ��ϵͳ��������[����ڳ�ʱ״̬timeout]
}MotorArgum;

typedef struct{
	bool enable;
	bool start;
	u8 mode;
	MotorLimits Limits;
	MotorParam  Param;
	MotorValue  ValueSet,ValueReal,ValuePrevious;
	MotorStatus Statue;
	MotorArgum  Argum;
	PIDvalue    PIDsp,PIDpos;
}MotorDJ;


void DJ_motor_Init(void);
void motor_speed_PID(s16 id);
void motor_position_PID(s16 id);
void motor_get_position(void);
void motor_set_zero(s16 id);
void motor_zero(s16 id);

extern MotorDJ motor[8];

#endif
