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
	u32 maxPulse;//最大旋转脉冲,[相关于最大旋转角度]
    vs32 lockPulse;//锁位置用当前位置记录，[相关于begin]
    s16 distance;//当前反馈脉冲与前次反馈脉冲差值，[相关于累计脉冲pulse]
    s32 difPulseSet;//当前累计脉冲与设定脉冲差值
    u16 zeroCnt;//寻零用计数值，[相关于寻零状态zero]
    vu16 stuckCnt;//堵转用计数值，[相关于堵转状态stuck]
    u8  timeoutCnt;//超时用计数值，[相关于超时状态timeout]
    u32 lastRxTim;//上次接收到报文的系统时间，[相关于超时状态timeout]
    u16 timeoutTicks;//判断超时用系统节拍数，[相关于超时状态timeout]
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
