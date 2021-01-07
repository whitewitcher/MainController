#ifndef _PARAM_H_
#define _PARAM_H_

#include "sys.h"
#include "includes.h"
#include "stdbool.h"
#include "led.h"
#include "motor.h"
#include "VESC.h"

/**
 * @description: set specific rotor to control for code selecting
 */
#define ThrowRobot
//#define DefineRobot
//#define SteeringMotor

#define USE_ELMO
//#define USE_DJI
//#define USE_VESC

//#define DJ_Motor_3508
#define DJ_Motor_2006

//#define USE_Flat90
#define USE_U10
//#define USE_EC4P30

#define use_debug_key
//#define DJ_Motor_Key
#define USE_ELMO_Key

//#define USE_SCOPE
//#define USE_DataScope//新示波器

/****标志位结构体****/
typedef struct{
    u8 led;
    volatile bool Can1CtrlList_Enable; // CAN1报文控制表启动标志位
    volatile bool Can2CtrlList_Enable; // CAN2报文控制表启动标志位
    volatile bool Can1SendqueueFULL;   // CAN1发送队列满
    volatile bool Can2SendqueueFULL;   // CAN2发送队列满
    volatile bool CanSendqueueEMPTY;   // CAN队列为空
    volatile bool has_timeout[8];      // CAN报文接收超时
    volatile bool poor_contact[8];     //接触不良
    volatile bool Usart1DMASendFinish; // Usart1_DMA传输
    volatile bool Usart2DMASendFinish; // Usart2_DMA传输
    bool valve[7];                     //当前电磁阀状态
} FlagTypeDef;

//第一位
//      电机id
//第二位
#define SYSTEMERROR   0x0000
#define DJERROR       0x0010
#define ELMOERROR     0x0020
#define VESCERROR     0x0030
#define EPOSERROR     0x0040
//第三位
#define TIMEOUT       0x0100
#define STUCK         0x0200
#define EMERGENCY     0x0300
//第四位 EMERGENCY
#define OVERCURRENT   0x1000
#define SPEEDFAULT    0x2000
#define OVERSPEED     0x3000
#define OVERPOSITIION 0x4000
#define CANTSTART     0x5000
#define OVERHEAT      0x6000

/****错误示显链表****/
typedef struct _ErrorList{
  u16 code;
  struct _ErrorList* next;
}ErrorNode;

typedef struct{
  u8 size;
  ErrorNode* head;
}ErrorTypeDef;

extern FlagTypeDef flag;
extern ErrorTypeDef ErrorList;

extern bool elmomove;
extern u8   elmoback;
extern bool elmothrow;

extern vs32 elmo_startps;
extern vs32 elmo_throw;
extern vs32 elmo_stop;

void param_Init(void);
void Flag_Init(void);
void ChangeData(u8* buff2,u8* buff1);
ErrorNode* creatError(void);
void deleteError(ErrorTypeDef errorList, ErrorNode* nodedel);
void insertError(ErrorTypeDef errorList, uc16 errorcode);
ErrorNode* ErrorFind(ErrorTypeDef errorList, uc16 errorcode);

#endif
