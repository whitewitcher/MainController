#include "motor.h"

MotorParam MotorInstrin;		 //电机内参
MotorValue Motorvalue;			 //电机设置
MotorLimits Motorlimit;			 //电机限制保护结构体
MotorArgum Motorargum; 		 	 //不需要关心的一些参数
MotorDJ motor[8];  				 //电机总结构体

void DJ_motor_Init(void)
{
	#ifdef DJ_Motor_3508
		MotorInstrin.MaxCurrent=14745;
//		MotorInstrin.MaxCurrent=5000;
		MotorInstrin.Pulse=8192;
		MotorInstrin.RATIO=19;
		MotorInstrin.GearRatio=1;
	#endif
	
	#ifdef DJ_Motor_2006
		MotorInstrin.MaxCurrent=9000;
//		MotorInstrin.MaxCurrent=5000;
		MotorInstrin.Pulse=8192;
		MotorInstrin.RATIO=36;
		MotorInstrin.GearRatio=1;
	#endif
	
	Motorlimit.PosLimit_ON=DISABLE;
	Motorlimit.PosLimit=15*MotorInstrin.GearRatio;
	Motorlimit.PositionSpeedLimit_ON=ENABLE;
	Motorlimit.PosSpLimit=3000;
	Motorlimit.RealseWhenStuck_ON=ENABLE;
	Motorlimit.ZeroCurrentLimit=500;
	Motorlimit.ZeroSpLimit=200;
	
	Motorargum.timeoutTicks=1000;
	
	//配置0号电机
	motor[0].enable=DISABLE;
	motor[0].start=DISABLE;
	motor[0].mode=position;
	motor[0].Argum=Motorargum;
	motor[0].Param=MotorInstrin;
	motor[0].Limits=Motorlimit;
	motor[0].ValueSet.angle=0;
	motor[0].ValueSet.current=500;
	motor[0].ValueSet.speed=500;
	PIDInit(5,0.2,0,0.4,motor[0].ValueSet.speed,&motor[0].PIDsp);
	PIDInit(8,0.3,0,1,motor[0].ValueSet.angle,&motor[0].PIDpos);
	motor[0].Argum.maxPulse=motor[0].Limits.PosLimit * motor[0].Param.RATIO * motor[0].Param.Pulse/360.f;
    motor[0].ValueSet.pulse=motor[0].ValueSet.angle  * motor[0].Param.RATIO * motor[0].Param.Pulse/360.f;
	motor[0].ValueReal.pulse=0;

	//配置1号电机
	motor[1].enable=DISABLE;
	motor[1].start=DISABLE;
	motor[1].mode=position;
	motor[1].Argum=Motorargum;
	motor[1].Param=MotorInstrin;
	motor[1].Limits=Motorlimit;
	motor[1].ValueSet.angle=0;
	motor[1].ValueSet.current=500;
	motor[1].ValueSet.speed=500;
	PIDInit(5,0.2,0,0.4,motor[1].ValueSet.speed,&motor[1].PIDsp);
	PIDInit(8,0.3,0,1,motor[1].ValueSet.angle,&motor[1].PIDpos);
	motor[1].Argum.maxPulse=motor[1].Limits.PosLimit * motor[1].Param.RATIO * motor[1].Param.Pulse/360.f;
    motor[1].ValueSet.pulse=motor[1].ValueSet.angle  * motor[1].Param.RATIO * motor[1].Param.Pulse/360.f;
	
	//配置2号电机
	motor[2].enable=DISABLE;
	motor[2].start=DISABLE;
	motor[2].mode=position;
	motor[2].Argum=Motorargum;
	motor[2].Param=MotorInstrin;
	motor[2].Limits=Motorlimit;
	motor[2].ValueSet.angle=0;
	motor[2].ValueSet.current=500;
	motor[2].ValueSet.speed=500;
	PIDInit(5,0.2,0,0.4,motor[2].ValueSet.speed,&motor[2].PIDsp);
	PIDInit(8,0.3,0,1,motor[2].ValueSet.angle,&motor[2].PIDpos);
	motor[2].Argum.maxPulse=motor[2].Limits.PosLimit * motor[2].Param.RATIO * motor[2].Param.Pulse/360.f;
    motor[2].ValueSet.pulse=motor[2].ValueSet.angle  * motor[2].Param.RATIO * motor[2].Param.Pulse/360.f;
	
	//配置3号电机
	motor[3].enable=DISABLE;
	motor[3].start=DISABLE;
	motor[3].mode=position;
	motor[3].Argum=Motorargum;
	motor[3].Param=MotorInstrin;
	motor[3].Limits=Motorlimit;
	motor[3].ValueSet.angle=0;
	motor[3].ValueSet.current=500;
	motor[3].ValueSet.speed=500;
	PIDInit(5,0.2,0,0.4,motor[3].ValueSet.speed,&motor[3].PIDsp);
	PIDInit(8,0.3,0,1,motor[3].ValueSet.angle,&motor[3].PIDpos);
	motor[3].Argum.maxPulse=motor[3].Limits.PosLimit * motor[3].Param.RATIO * motor[3].Param.Pulse/360.f;
	motor[3].Argum.maxPulse=motor[3].Limits.PosLimit * motor[3].Param.RATIO * motor[3].Param.Pulse/360.f;
    motor[3].ValueSet.pulse=motor[3].ValueSet.angle  * motor[3].Param.RATIO * motor[3].Param.Pulse/360.f;
	
	//配置4号电机
	motor[4].enable=DISABLE;
	motor[4].start=DISABLE;
	motor[4].mode=position;
	motor[4].Argum=Motorargum;
	motor[4].Param=MotorInstrin;
	motor[4].Limits=Motorlimit;
	motor[4].ValueSet.angle=0;
	motor[4].ValueSet.current=500;
	motor[4].ValueSet.speed=500;
	PIDInit(5,0.2,0,0.4,motor[4].ValueSet.speed,&motor[4].PIDsp);
	PIDInit(8,0.3,0,1,motor[4].ValueSet.angle,&motor[4].PIDpos);
	motor[4].Argum.maxPulse=motor[4].Limits.PosLimit * motor[4].Param.RATIO * motor[4].Param.Pulse/360.f;
    motor[4].ValueSet.pulse=motor[4].ValueSet.angle  * motor[4].Param.RATIO * motor[4].Param.Pulse/360.f;
	
	//配置5号电机
	motor[5].enable=DISABLE;
	motor[5].start=DISABLE;
	motor[5].mode=position;
	motor[5].Argum=Motorargum;
	motor[5].Param=MotorInstrin;
	motor[5].Limits=Motorlimit;
	motor[5].ValueSet.angle=0;
	motor[5].ValueSet.current=500;
	motor[5].ValueSet.speed=500;
	PIDInit(5,0.2,0,0.4,motor[5].ValueSet.speed,&motor[5].PIDsp);
	PIDInit(8,0.3,0,1,motor[5].ValueSet.angle,&motor[5].PIDpos);
	motor[5].Argum.maxPulse=motor[5].Limits.PosLimit * motor[5].Param.RATIO * motor[5].Param.Pulse/360.f;
    motor[5].ValueSet.pulse=motor[5].ValueSet.angle  * motor[5].Param.RATIO * motor[5].Param.Pulse/360.f;
	
	//配置6号电机
	motor[6].enable=DISABLE;
	motor[6].start=DISABLE;
	motor[6].mode=position;
	motor[6].Argum=Motorargum;
	motor[6].Param=MotorInstrin;
	motor[6].Limits=Motorlimit;
	motor[6].ValueSet.angle=0;
	motor[6].ValueSet.current=500;
	motor[6].ValueSet.speed=500;
	PIDInit(5,0.2,0,0.4,motor[6].ValueSet.speed,&motor[6].PIDsp);
	PIDInit(8,0.3,0,1,motor[6].ValueSet.angle,&motor[6].PIDpos);
	motor[6].Argum.maxPulse=motor[6].Limits.PosLimit * motor[6].Param.RATIO * motor[6].Param.Pulse/360.f;
    motor[6].ValueSet.pulse=motor[6].ValueSet.angle  * motor[6].Param.RATIO * motor[6].Param.Pulse/360.f;
	
	//配置7号电机
	motor[7].enable=DISABLE;
	motor[7].start=DISABLE;
	motor[7].mode=position;
	motor[7].Argum=Motorargum;
	motor[7].Param=MotorInstrin;
	motor[7].Limits=Motorlimit;
	motor[7].ValueSet.angle=0;
	motor[7].ValueSet.current=500;
	motor[7].ValueSet.speed=500;
	PIDInit(5,0.2,0,0.4,motor[7].ValueSet.speed,&motor[7].PIDsp);
	PIDInit(8,0.3,0,1,motor[7].ValueSet.angle,&motor[7].PIDpos);
	motor[7].Argum.maxPulse=motor[7].Limits.PosLimit * motor[7].Param.RATIO * motor[7].Param.Pulse/360.f;
    motor[7].ValueSet.pulse=motor[7].ValueSet.angle  * motor[7].Param.RATIO * motor[7].Param.Pulse/360.f;
	
}

void motor_set_zero(s16 id){
	motor[id].Statue.isSetZero=false;
	motor[id].ValueReal.angle=0;
	motor[id].ValueReal.pulse=0;
}

void motor_get_position(void){
	for(int id=0;id<8;id++){
		motor[id].Argum.distance=motor[id].ValueReal.pulseRead - motor[id].ValuePrevious.pulseRead;
		motor[id].ValuePrevious=motor[id].ValueReal;
		if(ABS(motor[id].Argum.distance)>4000)
			motor[id].Argum.distance -= SIG(motor[id].Argum.distance) * motor[id].Param.Pulse;   //电调返回位置跳变时处理  8192->0或0->8192
		motor[id].ValueReal.pulse +=motor[id].Argum.distance;
		motor[id].Argum.difPulseSet=(motor[id].ValueSet.pulse - motor[id].ValueReal.pulse);   //更新误差
		
		if (motor[id].enable && (ABS(motor[id].ValueSet.current)>=motor[id].Param.MaxCurrent)){
			motor[id].Argum.stuckCnt++;
			if(motor[id].Argum.stuckCnt>1000){
				motor[id].Statue.stuck=true;
				insertError(ErrorList,DJERROR|STUCK|id);
				if(motor[id].Limits.RealseWhenStuck_ON)
					motor[id].start=0;
			}
			else {				
				motor[id].Statue.stuck=false;
			}
		}
		else {
			deleteError(ErrorList,ErrorFind(ErrorList,DJERROR|STUCK|id));
			motor[id].Argum.stuckCnt=0;
		}
		
		 /* 锁点位置记录 */
        if(motor[id].start) {
			motor[id].Argum.lockPulse=motor[id].ValueReal.pulse;
		}
        /* 判断是否需要重置零点 */
        if(motor[id].Statue.isSetZero) motor_set_zero(id);
     /* 反馈超时判断 */
        if((motor[id].enable)&&(OSTimeGet()-motor[id].Argum.lastRxTim)>motor[id].Argum.timeoutTicks)
        {
          if(++motor[id].Argum.timeoutCnt>10) 
          {
            motor[id].Statue.timeout=true; 
            insertError(ErrorList, DJERROR|TIMEOUT|id);
          }
          else 
          {
            motor[id].Statue.timeout=false;
          }
        }
        else 
        {
          motor[id].Argum.timeoutCnt=0;
          deleteError(ErrorList, ErrorFind(ErrorList, DJERROR|TIMEOUT|id));
        }
	}
}

void motor_speed_PID(s16 id){	
	motor[id].PIDsp.setvalue=motor[id].ValueSet.speed;
	motor[id].PIDsp.realvalue=motor[id].ValueReal.speed;
	PIDcalculate(&motor[id].PIDsp);
	motor[id].ValueSet.current+=motor[id].PIDsp.delta;
}

void motor_position_PID(s16 id){
	motor[id].ValueSet.pulse=(motor[id].Param.RATIO * motor[id].Param.Pulse/360.f) * motor[id].ValueSet.angle;
	motor[id].PIDpos.setvalue=motor[id].ValueSet.pulse;
	if (!motor[id].start) {
		motor[id].PIDpos.setvalue=motor[id].Argum.lockPulse;
	}
	if (motor[id].Limits.PosLimit_ON) 
		PEAK(motor[id].PIDpos.setvalue,motor[id].Argum.maxPulse);
	motor[id].PIDpos.realvalue=motor[id].ValueReal.pulse;
	PIDcalculate(&motor[id].PIDpos);
	motor[id].PIDsp.setvalue = motor[id].PIDsp.Ks *(motor[id].PIDsp.setvalue + motor[id].PIDpos.delta);
	if (motor[id].Limits.PositionSpeedLimit_ON) 
		PEAK(motor[id].PIDsp.setvalue,motor[id].Limits.PosSpLimit);
	motor[id].PIDsp.realvalue=motor[id].ValueReal.speed;
	PIDcalculate(&motor[id].PIDsp);
	motor[id].ValueSet.current += motor[id].PIDsp.delta;
	if(ABS(motor[id].Argum.difPulseSet)<60) motor[id].Statue.arrived=true;//到达指定位置
    else motor[id].Statue.arrived=false;               //标志位
	
}

void motor_zero(s16 id){
	motor[id].PIDsp.setvalue=motor[id].Limits.ZeroSpLimit;
	motor[id].PIDsp.realvalue=motor[id].ValueReal.speed;
	PIDcalculate(&motor[id].PIDsp);
	motor[id].ValueSet.current += motor[id].PIDsp.delta;
	PEAK(motor[id].ValueSet.current,motor[id].Limits.ZeroCurrentLimit);
//	if(motor[id].Argum.distance==0 && motor[id].ValueReal.pulse>5000)
	if(motor[id].Argum.distance==0)
		motor[id].Argum.zeroCnt++;
	else motor[id].Argum.zeroCnt=0;
	if (motor[id].Argum.zeroCnt>100){
		motor[id].Argum.zeroCnt=0;
		motor[id].ValueReal.pulse=0;
		motor[id].ValueSet.angle=0;
		motor[id].mode=position;
		motor[id].Statue.zero=1;
	}
		
}

