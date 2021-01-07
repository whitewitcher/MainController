#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f4xx.h"
#include "includes.h"
#include "motor.h"
#include "elmo.h"
#include "key.h"
#include "usart2.h"
#include "usart1.h"
#include "usart3.h"
#include "visual_scope.h"
#include "DataScope_DP.h"
#include "beep.h"
#include "led.h"
#include "param.h"
#include "tim2.h"

void testfunction(void);

/****UCOSII��������****/
//START����
#define START_TASK_PRIO          5
#define START_TASK_STK_SIZE         256
__align(8) OS_STK start_task_stk[START_TASK_STK_SIZE];
static void TaskStart(void *arg);

//CAN��������
#define CAN_TASK_PRIO			15
#define CAN_STK_SIZE			128
__align(8) OS_STK CAN_TASK_STK[CAN_STK_SIZE];
static void Task_CAN(void *pdata);

//LCD����
#define LCD_Tx_TASK_PRIO       	25
#define LCD_Tx_STK_SIZE     		512
__align(8) OS_STK LCD_Tx_TASK_STK[LCD_Tx_STK_SIZE];
static void Task_Lcd_Tx(void *pdata);

#define LCD_Rx_TASK_PRIO       	30
#define LCD_Rx_STK_SIZE     		512
__align(8) OS_STK LCD_Rx_TASK_STK[LCD_Rx_STK_SIZE];
static void Task_Lcd_Rx(void *pdata);

//Flag����
#define FLAG_TASK_PRIO			35
#define FLAG_STK_SIZE			128
__align(8) OS_STK FLAG_TASK_STK[FLAG_STK_SIZE];
static void Task_Flag(void *pdata);

//�������ִ������
#define	MOTOR_TASK_PRIO		    40
#define MOTOR_STK_SIZE		    1024
__align(8) OS_STK MOTOR_TASK_STK[MOTOR_STK_SIZE];
static void Task_Motor(void *pdata);

//����ʾ����
#define SCOPE_TASK_PRIO			45
#define SCOPE_STK_SIZE			128
__align(8) OS_STK SCOPE_TASK_STK[SCOPE_STK_SIZE];
static void Task_Scope(void *pdata);

//ELMO��ѯ����
#define ELMO_TASK_PRIO			50
#define ELMO_STK_SIZE			128
__align(8) OS_STK ELMO_TASK_STK[ELMO_STK_SIZE];
static void Task_Elmo(void *pdata);

//ELMO��������
#define ELMOMove_TASK_PRIO			55
#define ELMOMove_STK_SIZE			128
__align(8) OS_STK ELMOMove_TASK_STK[ELMOMove_STK_SIZE];
static void Task_ElmoMove(void *pdata);


OS_EVENT *RUN;

#endif
