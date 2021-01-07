#ifndef _PID_
#define _PID_

#include "sys.h"
#include "stm32f4xx.h"

typedef struct{
	s32 setvalue;
	s32 realvalue;
	s32 delta;
	s32 savedeviates[3];
	float Kp;
	float Ki;
	float Kd;
	float Ks;
}PIDvalue;

void PIDInit(float setKp,float setKi,float setKd,float setKs,s32 setvalu,PIDvalue* PIDx);
void PIDcalculate(PIDvalue* PIDx);



#endif
