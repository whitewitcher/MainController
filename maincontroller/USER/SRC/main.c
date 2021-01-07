#include "main.h"

int main(void)
{
	SystemInit();
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_3);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_DMA1 | RCC_AHB1Periph_DMA2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG | RCC_APB2Periph_USART1, ENABLE);

	LED_Configuration();
	Led8_Configuration();
	Beep_Configuration(music1, 210);
	Key_Configuration();
	CAN1_Init();
	CAN2_Init();
	param_Init();
	Can_SendqueueInit();
	InitCANControlList(Can2_MesgSentList, CAN_2);
	Usart1_Init(); //虚拟示波器
	Usart2_Init(); //串口屏
	//Usart3_Init();   //绝对值编码器
	TIM2_Init();
	TIM3_Init();
#ifdef USE_VESC
	TIM_Cmd(TIM2, ENABLE);
#endif
	TIM_Cmd(TIM3, ENABLE);
	OSInit();
	OSTaskCreate(TaskStart, (void *)0, &start_task_stk[START_TASK_STK_SIZE - 1], START_TASK_PRIO);
	OSStart();

	return 0;
}
/**
  * @brief  起始任务
  */
static void TaskStart(void *pdata)
{

	OS_CPU_SR cpu_sr = 0;
	pdata = pdata;
	OS_CPU_SysTickInit();
	OS_ENTER_CRITICAL(); //进入临界区（无法被中断打断）
	RUN = OSSemCreate(0);

	OSTaskCreate(Task_CAN, (void *)0, (OS_STK *)&CAN_TASK_STK[CAN_STK_SIZE - 1], CAN_TASK_PRIO);
#ifdef USE_DJI
	OSTaskCreate(Task_Motor, (void *)0, (OS_STK *)&MOTOR_TASK_STK[MOTOR_STK_SIZE - 1], MOTOR_TASK_PRIO);
#endif
#ifdef USE_ELMO
	OSTaskCreate(Task_Elmo, (void *)0, (OS_STK *)&ELMO_TASK_STK[ELMO_STK_SIZE - 1], ELMO_TASK_PRIO);
	OSTaskCreate(Task_ElmoMove, (void *)0, (OS_STK *)&ELMOMove_TASK_STK[ELMOMove_STK_SIZE - 1], ELMOMove_TASK_PRIO);
#endif
	OSTaskCreate(Task_Lcd_Tx, (void *)0, (OS_STK *)&LCD_Tx_TASK_STK[LCD_Tx_STK_SIZE - 1], LCD_Tx_TASK_PRIO);
	OSTaskCreate(Task_Lcd_Rx, (void *)0, (OS_STK *)&LCD_Rx_TASK_STK[LCD_Rx_STK_SIZE - 1], LCD_Rx_TASK_PRIO);
	OSTaskCreate(Task_Flag, (void *)0, (OS_STK *)&FLAG_TASK_STK[FLAG_STK_SIZE - 1], FLAG_TASK_PRIO);

#ifdef USE_SCOPE
	OSTaskCreate(Task_Scope, (void *)0, (OS_STK *)&SCOPE_TASK_STK[SCOPE_STK_SIZE - 1], SCOPE_TASK_PRIO);
#endif
#ifdef USE_ELMO
	ElmoInit(CAN2);
	OSTimeDly(5);
	ElmoInit(CAN2);
	OSTimeDly(5);
	ElmoInit(CAN2);
	OSTimeDly(5);
	ElmoInit(CAN2);
	OSTimeDly(5);
	ElmoMotorInit();
#endif
	Beep_Show(2);

	OSTimeDly(1000);
	OSTaskSuspend(START_TASK_PRIO); //挂起起始任务
	OS_EXIT_CRITICAL();				//退出临界区
}
#ifdef USE_DJI
//电机动作
static void Task_Motor(void *pdata)
{
	pdata = pdata;
	static int detcount = 0;

	while (1)
	{

		if (detcount++ < 10)
		{
			for (int i = 0; i < 8; i++)
			{
				motor[i].ValueReal.angle = 0;
				motor[i].ValueReal.pulse = 0;
				motor[i].Argum.distance = 0;
			}
		}
		OSTimeDly(1000);
	}
}
#endif

// CAN报文任务
static void Task_CAN(void *pdata)
{

	while (1)
	{
		Can_DeQueue(CAN2, &Can2_Sendqueue); //队列处理
		if (flag.Can2CtrlList_Enable)
			TraversalControlList(Can2_MesgSentList, &Can2_Sendqueue, CAN_2);

		OSTimeDly(3);
	}
}

#ifdef USE_ELMO
// Elmo查询任务
static void Task_Elmo(void *pdata)
{

	while (1)
	{
		static int i = 0; //初始电机状态
		if (i++ == 0)
		{
			MO(1, SetData, 0, 1);
			PX(1, SetData, 0, 1);
			JV(1, SetData, ElmoMotor[0].ValueSet.speed, 1);
			SP(1, SetData, ElmoMotor[0].ValueSet.speed, 1);
		}
		VX(1);
		PX(1, GetData, 0, 0);
		IQ(1);
		MO(1, GetData, 0, 0);
		UM(1, GetData, 0, 0);
		ElmoMotor[0].ValueSet.pulse = ElmoMotor[0].Param.Pulse / 360.f * ElmoMotor[0].Param.RATIO * ElmoMotor[0].ValueSet.angle;

		for (int id = 0; id < 8; id++)
		{
			if (ElmoMotor[id].enable && OSTimeGet() - ElmoMotor[id].Argum.lastRxTim > ElmoMotor[id].Argum.timeoutTicks)
				ElmoMotor[id].Argum.timeoutCnt++;
			else
				ElmoMotor[id].Argum.timeoutCnt = 0;
			if (ElmoMotor[id].Argum.timeoutCnt > 10)
			{
				insertError(ErrorList, ELMOERROR | TIMEOUT | id);
				ElmoMotor[id].Statue.timeout = true;
			}
			else
			{
				ElmoMotor[id].Statue.timeout = false;
				deleteError(ErrorList, ErrorFind(ErrorList, ELMOERROR | TIMEOUT | id));
			}
		}
		//if(elmomove)
		//	OSTimeDly(10);
		//else
		OSTimeDly(50);
	}
}

// Elmo动作任务
bool elmomove = false;
u8 elmoback = 0;
bool elmothrow = false;

static void Task_ElmoMove(void *pdata)
{

	while (1)
	{
		static int i = 0;
		if (elmomove)
		{

			if (!i++)
			{
				PR_(1, SetData, elmo_throw, 1);
				//SP(1,SetData,ElmoMotor[0].Limits.PosSpLimit,1);
				valveCtrl(0, 0);
			}
			if (i == 2)
			{
				BG(1);
				elmomove = false;
				i = 0;
			}
		}
		if (elmoback == 1)
		{
			valveCtrl(0, 1);
			elmoback = 0;
		}
		else if (elmoback == 2)
		{
			if (!i++)
				PR_(1, SetData, -elmo_stop, 1);
			if (i == 2)
			{
				BG(1);
				elmoback = 0;
				i = 0;
			}
		}
		OSTimeDly(10);
	}
}

/*
static void Task_ElmoMove(void *pdata) 
{
	
  while (1) 
  {
	if(elmomove){
		static int i=0;
		if(!i++){			
			JV(1, SetData, ElmoMotor[0].ValueSet.speed,1);			
			BG(1);
			ElmoMotor[0].ValueSet.pulse=elmo_stop;
			ElmoMotor[0].ValueSet.angle=360.f/ElmoMotor[0].Param.Pulse/ElmoMotor[0].Param.RATIO*elmo_stop;}
			
		if(ElmoMotor[0].ValueReal.pulse<elmo_throw && !elmothrow){
			valveCtrl(0,1);
			elmothrow=true;
			//PA(1, SetData, ElmoMotor[0].ValueSet.pulse,1);
			//BG(1);
			ST(1);
		//}
		//if(ElmoMotor[0].ValueReal.pulse>elmo_stop-50 && ElmoMotor[0].ValueReal.pulse<elmo_stop+50){
			
			elmomove=false;
			i=0;
			elmothrow=false;   //temp
		}
	}
	if(elmoback){
		static int j=0;
		if(!j++){
			ElmoMotor[0].ValueSet.pulse=elmo_startps;
			ElmoMotor[0].ValueSet.angle=360.f/ElmoMotor[0].Param.Pulse/ElmoMotor[0].Param.RATIO*elmo_startps;
			PA(1, SetData, ElmoMotor[0].ValueSet.pulse,1);}
		if(j++==3)
			BG(1);
		if(ElmoMotor[0].ValueReal.pulse>elmo_startps-50 && ElmoMotor[0].ValueReal.pulse<elmo_startps+50){
			j=0;
			elmoback=false;
		}
	}
	//if(elmomove)
	//	OSTimeDly(10);
	//else
		OSTimeDly(10);
  }
}
*/
#endif

// LCD任务
static void Task_Lcd_Tx(void *pdata)
{
	while (1)
	{
		UsartLCDshow();
		OSTimeDly(3000);
	}
}

static void Task_Lcd_Rx(void *pdata)
{
	while (1)
	{
		UsartLCDReceive();
		OSTimeDly(500);
	}
}

//示波器任务
#ifdef USE_SCOPE
static void Task_Scope(void *pdata)
{
	while (1)
	{
		VS4Channal_Send(ElmoMotor[0].ValueReal.angle, ElmoMotor[0].ValueReal.speed, 10000, 0);
		OSTimeDly(30);
	}
}
#endif

#ifdef USE_DataScope
u8 Send_Count;
//新示波器任务
//static void Task_DataScope(void *pdata)
static void Task_Scope(void *pdata)
{
	while (1)
	{
		DataScope_Get_Channel_Data(1.0, 1);		   //将数据   写入通道 1
		DataScope_Get_Channel_Data(2.0, 2);		   //将数据   写入通道 2
		DataScope_Get_Channel_Data(3.0, 3);		   //将数据   写入通道 3
		DataScope_Get_Channel_Data(4.0, 4);		   //将数据   写入通道 4
		DataScope_Get_Channel_Data(5.0, 5);		   //将数据   写入通道 5
		DataScope_Get_Channel_Data(6.0, 6);		   //将数据   写入通道 6
		DataScope_Get_Channel_Data(7.0, 7);		   //将数据   写入通道 7
		DataScope_Get_Channel_Data(8.0, 8);		   //将数据   写入通道 8
		DataScope_Get_Channel_Data(9.0, 9);		   //将数据   写入通道 9
		DataScope_Get_Channel_Data(TIM3->CNT, 10); //将数据   写入通道 10
		Send_Count = DataScope_Data_Generate(10);  //数据转换，输入有几个通道
		USART1_Send(DataScope_OutPut_Buffer, Send_Count);
		for (u8 i = 0; i < Send_Count; i++)
		{
			while ((USART1->SR & 0X40) == 0)
				;
			USART1->DR = DataScope_OutPut_Buffer[i];
		}
		OSTimeDly(30);
	}
}
#endif

//标志位判断任务
static void Task_Flag(void *pdata)
{
	ErrorNode *_error;
	while (1)
	{
#ifdef use_debug_key
		simulate_key(0); //debug虚拟矩阵键盘
#endif

		if (ErrorList.size == 0)
			Led_Show();
		else
		{
			LED_RED_ON;
			_error = ErrorList.head->next;
			for (int i = 0; i < ErrorList.size; i++)
			{
				if ((_error->code & 0x00F0) >> 4 == 0)
				{
					LED_GREEN_OFF;
					Led8DisData((_error->code & 0x00F0) >> 4);
					LED_YELLOW_ON;
				}
				else
				{
					//          LedDisplay(LED_GREEN,   _error->code&0x000F);
					//          LedDisplay(LED_BLUE,   (_error->code&0x00F0)>>4);
					//          LedDisplay(LED_YELLOW, (_error->code&0x0F00)>>8);
					//          LedDisplay(LED_RED,    (_error->code&0xF000)>>12);
					LED_BLUE_ON;
					LED_GREEN_ON;
					LED_YELLOW_ON;
					Led8DisData(_error->code & 0x000F);
					Led8DisData((_error->code & 0x00F0) >> 4);
					Led8DisData((_error->code & 0x0F00) >> 8);
					Led8DisData((_error->code & 0xF000) >> 12);
				}
				_error = _error->next;
				OSTimeDly(1000);
				LED_BLUE_OFF;
				LED_GREEN_OFF;
				LED_YELLOW_OFF;
				OSTimeDly(1000);
			}
		}
		OSTimeDly(1000);
	}
}
