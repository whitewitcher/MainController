#ifndef _MACRO_H_
#define _MACRO_H_

#include "stdbool.h"
#include "stm32f4xx.h"

#elif defined TryRobot
	#ifdef SteeringMotor
		//nothing
	#elif defined ActionMotor
		#define USE_ELMO
	#endif
#endif

#ifdef SteeringMotor
		#define BROADCAST_ID 0x00010000
	#ifdef PassRobot
		#define MOTOR_0_3 0x305
		#define MOTOR_1_2 0x306
		#define ID_SELF MOTOR_1_2
		#define GearRatio (104.0f/17.0f)
	#elif defined TryRobot

	#endif
#elif defined ActionMotor
		#define ID_SELF 0x00010300
		#define ID_BACK 0x00030101
		#define BROADCAST_ID 0x00010000
	#ifdef PassRobot
		#define Motor_Rotate_PR 0//ת�������
		#define Motor_Pitch 1//����������
		#define Motor_Capture 2//��צ������
	#elif defined TryRobot

	#endif
#endif

#define Can1ControlList 1
#define Can2ControlList 2

/* BIT��ţ��������̺��ж� */
#define BIT0 (0x01<<0)
#define BIT1 (0x01<<1)
#define BIT2 (0x01<<2)
#define BIT3 (0x01<<3)
#define BIT4 (0x01<<4)
#define BIT5 (0x01<<5)
#define BIT6 (0x01<<6)
#define BIT7 (0x01<<7)

/* math function */
#define ABS(x)  ((x)>0? (x):(-(x)))
#define SIG(x)  ((x)>=0? 1:-1)
#define PEAK(A,B)   if(ABS(A)>B) A=SIG(A)*B; //�˴�Ĭ��BΪ��ֵ

#define EncodeS32Data(f,buff)	{*(s32*)buff = *f;}
#define DecodeS32Data(f,buff)	{*f = *(s32 *)buff;}
#define EncodeS16Data(f,buff)	{*(s16*)buff = *f;}
#define DecodeS16Data(f,buff)	{*f = *(s16*)buff;}
#define EncodeU16Data(f,buff)	{*(u16*)buff = *f;}
#define DecodeU16Data(f,buff)	{*f = *(u16*)buff;}
#define EncodeS32Data(f,buff)	{*(s32*)buff = *f;}
#define DecodeS32Data(f,buff)	{*f = *(s32 *)buff;}


