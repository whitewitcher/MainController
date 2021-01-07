#include "key.h"

void Key_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
    /* 矩阵键盘 */
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    GPIO_Set(GPIOB,PIN12|PIN13|PIN14|PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_2M,GPIO_PUPD_PU);//行线 推挽输出
    GPIO_Set(GPIOC,PIN6|PIN7|PIN8|PIN9,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_2M,GPIO_PUPD_PU);//列线 上拉输入
	GPIO_WriteBit(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15, Bit_RESET);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource6); 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource7); 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource8); 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource9); 
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line6|EXTI_Line7|EXTI_Line8|EXTI_Line9;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVIC_Init(&NVIC_InitStruct);
}

void EXTI9_5_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line6|EXTI_Line7|EXTI_Line8|EXTI_Line9)!=RESET)
		Key_Ctrl(0);
	EXTI_ClearITPendingBit(EXTI_Line6|EXTI_Line7|EXTI_Line8|EXTI_Line9); 
}

u8 Check_Matrixkey(void)
{
		u8 cord_h=0xFF,cord_l=0xFF;//h为行线 l为列线
		u8 Val = 0xFF;
		
		//行线输出全部设置为0
		GPIO_WriteBit(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15, Bit_RESET);
		Delay_us(1);
	
		//读入列线值
		cord_l&=(u8)((GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)<<0)|(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)<<1)|(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)<<2)|(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)<<3));
		if(cord_l!=0x0F)
		{
			//Delay_ms(5);		//去抖
			//cord_l&=(u8)((GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)<<0)|(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)<<1)|(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)<<2)|(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)<<3));
			//if(cord_l!=0x0F)
			{
                //列线 推挽输出
                GPIO_Set(GPIOC,PIN6|PIN7|PIN8|PIN9,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_2M,GPIO_PUPD_PU);
                //行线 上拉输入
                GPIO_Set(GPIOB,PIN12|PIN13|PIN14|PIN15,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_2M,GPIO_PUPD_PU);
                //列线输出全部设置为0
                GPIO_WriteBit(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9, Bit_RESET);
                Delay_ms(2);
                //读入行线值
                cord_h&=(u8)((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)<<3)|(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)<<2)|(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)<<1)|(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)<<0));

                Val=~(cord_h<<4|cord_l);
					return Val;
			}
			
		}
		return ~Val;
}

u8 Check_Dialkey(void)
{
	return ((GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3)<<0)|(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)<<1)|(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)<<2));
}

void Key_Ctrl(u8 id)
{
	#ifdef USE_ELMO_Key
	#ifdef USE_ELMO
	id +=1;
    switch(Check_Matrixkey())
    {
        case S1:	MO(id,SetData,1,1);                                     //使能
            break;
        case S2:	MO(id,SetData,0,1);                                     //失能
            break;
        case S3:	UM(id,SetData,ElmoMotor[0].mode,1);                     //设置模式
            break;	
        case S4:	BG(id);                                                 //开始运动
            break;
        case S5:	ST(id);                                                 //停止
            break;
        case S6:    JV(id, SetData, ElmoMotor[0].ValueSet.speed,1);         //持续运动速度
            break;
        case S7:    SP(id, SetData, ElmoMotor[0].Limits.PosSpLimit,1);      //PTP模式速度
            break;
        case S8:    TC(id, SetData, ElmoMotor[0].ValueSet.current,1);       //设置转矩（电流）
            break;
        case S9:	PA(id, SetData, ElmoMotor[0].ValueSet.pulse,1);         //设置位置
            break;
        case S10:   elmomove=true;                                          //elmo动作
            break;
        case S11:   elmoback=true;                                          //Elmo复位
            break;
        case S12:   valveCtrl(0,1);										    //12:气缸开
            break;
        case S13:   valveCtrl(0,0);										    //13:气缸关
            break;
        case S14:   elmomove=false; elmoback=false; elmothrow=false;
            break;
        case S15:Led8DisData(15);
            break;
        case S16:Led8DisData(0);
            break;
    }
	#endif
	#endif
	#ifdef DJ_Motor_3508_Key
	switch(Check_Matrixkey())
    {
        case S1:	motor[id].enable=ENABLE;                                //使能
            break;
        case S2:	motor[id].enable=DISABLE;                               //失能
            break;
        case S3:	motor[id].ValueSet.current=100;                		    //设置转矩（电流）
            break;	
        case S4:	motor[id].start=ENABLE;                                 //开始运动
            break;
        case S5:	motor[id].start=DISABLE;                                //停止
            break;
        case S6:    motor[id].mode=position;                     		   //设置位置模式
            break;
        case S7:    motor[id].mode=RPM;									   //设置速度模式
            break;
        case S8:{   motor[id].mode=zero;								   //设置寻零模式
					motor[id].Limits.ZeroSpLimit=1000;}
            break;
        case S9:	motor[id].ValueSet.angle=180;						   //设置位置
            break;
        case S10:	motor[id].ValueSet.speed=1000;						   //设置速度
            break;
        case S11:{  motor[id].mode=zero;								   //设置负向寻零模式
					motor[id].Limits.ZeroSpLimit=-1000;}
            break;
        case S12:Led8DisData(12);
            break;
        case S13:Led8DisData(13);
            break;
        case S14:Led8DisData(14);
            break;
        case S15:Led8DisData(15);
            break;
        case S16:Led8DisData(0);
            break;
    }
	#endif
	GPIO_Set(GPIOB,PIN12|PIN13|PIN14|PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_2M,GPIO_PUPD_PU);//行线 推挽输出
    GPIO_Set(GPIOC,PIN6|PIN7|PIN8|PIN9,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_2M,GPIO_PUPD_PU);//列线 上拉输入
	GPIO_WriteBit(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15, Bit_RESET);
}

 int count=0,record=0;
void simulate_key(u8 id){

	if(count>=0 && count<=16 && count!=record){
		record=count;
	#ifdef USE_ELMO_Key
		#ifdef USE_ELMO
	id +=1;
    switch(count)
    {
        case 1:	MO(id,SetData,1,1);                                     //使能
            break;
        case 2:	MO(id,SetData,0,1);                                     //失能
            break;
        case 3:	UM(id,SetData,ElmoMotor[0].mode,1);                     //设置模式
            break;	
        case 4:	BG(id);                                                 //开始运动
            break;
        case 5:	ST(id);                                                 //停止
            break;
        case 6: JV(id, SetData, ElmoMotor[0].ValueSet.speed,1);         //持续运动速度
            break;
        case 7: SP(id, SetData, ElmoMotor[0].Limits.PosSpLimit,1);      //PTP模式速度
            break;
        case 8: TC(id, SetData, ElmoMotor[0].ValueSet.current,1);       //设置转矩（电流）
            break;
        case 9:	PA(id, SetData, ElmoMotor[0].ValueSet.pulse,1);         //设置位置
            break;
        case 10:elmomove=true;                                          //elmo动作
            break;
        case 11:elmoback=true;                                          //Elmo复位
            break;
        case 12:valveCtrl(0,1);										    //12:气缸开
            break;
        case 13:valveCtrl(0,0);										    //13:气缸关
            break;
        case 14:Led8DisData(14);
            break;
        case 15:Led8DisData(15);
            break;
        case 16:Led8DisData(0);
            break;
    }
	#endif
	#endif
	#ifdef DJ_Motor_3508_Key
	switch(count)
    {
        case 1:	motor[id].enable=ENABLE;                                //1:使能
            break;
        case 2:	motor[id].enable=DISABLE;                               //2:失能
            break;
        case 3:	motor[id].ValueSet.current=100;                		    //3:转矩（电流）
            break;	
        case 4:	motor[id].start=ENABLE;                                 //4:开始运动
            break;
        case 5:	motor[id].start=DISABLE;                                //5:停止
            break;
        case 6:    motor[id].mode=position;                     		//6:位置模式
            break;
        case 7:    motor[id].mode=RPM;									//7:速度模式
            break;
        case 8: {   motor[id].mode=zero;								    //8:寻零模式
					motor[id].Limits.ZeroSpLimit=1000;}
            break;
        case 9:		motor[id].ValueSet.angle=180;						    //9:设置位置
            break;
        case 10:	motor[id].ValueSet.speed=1000;						//10:设置速度
            break;
        case 11: {  motor[id].mode=zero;								    //11:负向寻零模式
				   motor[id].Limits.ZeroSpLimit=-1000;}
            break;
        case 12:   valveCtrl(0,1);										//12:气缸开
            break;
        case 13:   valveCtrl(0,0);										//13:气缸关
            break;
        case 14:Led8DisData(14);
            break;
        case 15:Led8DisData(15);
            break;
        case 16:Led8DisData(0);
            break;
    }
	#endif
}
}
