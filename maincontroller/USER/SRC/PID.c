#include "PID.h"

void PIDInit(float setKp,float setKi,float setKd,float setKs,s32 setvalu,PIDvalue* PIDx){
	PIDx->Kd=setKd;
	PIDx->Ki=setKi;
	PIDx->Kp=setKp;
	PIDx->Ks=setKs;
	PIDx->setvalue=setvalu;
	PIDx->realvalue=0;
	PIDx->delta=0;
	PIDx->savedeviates[0]=0;
	PIDx->savedeviates[1]=0;
	PIDx->savedeviates[2]=0;
	
}

void PIDcalculate(PIDvalue* PIDx){
	PIDx->savedeviates[0]=PIDx->setvalue-PIDx->realvalue;
	PIDx->delta=PIDx->Kp *(PIDx->savedeviates[0] - PIDx->savedeviates[1] )+ PIDx->Ki * PIDx->savedeviates[0] + 
							PIDx->Kd *(PIDx->savedeviates[0] - 2* PIDx->savedeviates[1] +PIDx->savedeviates[2]);
	PIDx->savedeviates[2]=PIDx->savedeviates[1];
	PIDx->savedeviates[1]=PIDx->savedeviates[0];
}


