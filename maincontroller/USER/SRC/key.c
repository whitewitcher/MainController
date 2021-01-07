#include "key.h"

void Key_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
    /* ������� */
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    GPIO_Set(GPIOB,PIN12|PIN13|PIN14|PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_2M,GPIO_PUPD_PU);//���� �������
    GPIO_Set(GPIOC,PIN6|PIN7|PIN8|PIN9,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_2M,GPIO_PUPD_PU);//���� ��������
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
		u8 cord_h=0xFF,cord_l=0xFF;//hΪ���� lΪ����
		u8 Val = 0xFF;
		
		//�������ȫ������Ϊ0
		GPIO_WriteBit(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15, Bit_RESET);
		Delay_us(1);
	
		//��������ֵ
		cord_l&=(u8)((GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)<<0)|(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)<<1)|(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)<<2)|(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)<<3));
		if(cord_l!=0x0F)
		{
			//Delay_ms(5);		//ȥ��
			//cord_l&=(u8)((GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)<<0)|(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)<<1)|(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)<<2)|(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)<<3));
			//if(cord_l!=0x0F)
			{
                //���� �������
                GPIO_Set(GPIOC,PIN6|PIN7|PIN8|PIN9,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_2M,GPIO_PUPD_PU);
                //���� ��������
                GPIO_Set(GPIOB,PIN12|PIN13|PIN14|PIN15,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_2M,GPIO_PUPD_PU);
                //�������ȫ������Ϊ0
                GPIO_WriteBit(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9, Bit_RESET);
                Delay_ms(2);
                //��������ֵ
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
        case S1:	MO(id,SetData,1,1);                                     //ʹ��
            break;
        case S2:	MO(id,SetData,0,1);                                     //ʧ��
            break;
        case S3:	UM(id,SetData,ElmoMotor[0].mode,1);                     //����ģʽ
            break;	
        case S4:	BG(id);                                                 //��ʼ�˶�
            break;
        case S5:	ST(id);                                                 //ֹͣ
            break;
        case S6:    JV(id, SetData, ElmoMotor[0].ValueSet.speed,1);         //�����˶��ٶ�
            break;
        case S7:    SP(id, SetData, ElmoMotor[0].Limits.PosSpLimit,1);      //PTPģʽ�ٶ�
            break;
        case S8:    TC(id, SetData, ElmoMotor[0].ValueSet.current,1);       //����ת�أ�������
            break;
        case S9:	PA(id, SetData, ElmoMotor[0].ValueSet.pulse,1);         //����λ��
            break;
        case S10:   elmomove=true;                                          //elmo����
            break;
        case S11:   elmoback=true;                                          //Elmo��λ
            break;
        case S12:   valveCtrl(0,1);										    //12:���׿�
            break;
        case S13:   valveCtrl(0,0);										    //13:���׹�
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
        case S1:	motor[id].enable=ENABLE;                                //ʹ��
            break;
        case S2:	motor[id].enable=DISABLE;                               //ʧ��
            break;
        case S3:	motor[id].ValueSet.current=100;                		    //����ת�أ�������
            break;	
        case S4:	motor[id].start=ENABLE;                                 //��ʼ�˶�
            break;
        case S5:	motor[id].start=DISABLE;                                //ֹͣ
            break;
        case S6:    motor[id].mode=position;                     		   //����λ��ģʽ
            break;
        case S7:    motor[id].mode=RPM;									   //�����ٶ�ģʽ
            break;
        case S8:{   motor[id].mode=zero;								   //����Ѱ��ģʽ
					motor[id].Limits.ZeroSpLimit=1000;}
            break;
        case S9:	motor[id].ValueSet.angle=180;						   //����λ��
            break;
        case S10:	motor[id].ValueSet.speed=1000;						   //�����ٶ�
            break;
        case S11:{  motor[id].mode=zero;								   //���ø���Ѱ��ģʽ
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
	GPIO_Set(GPIOB,PIN12|PIN13|PIN14|PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_2M,GPIO_PUPD_PU);//���� �������
    GPIO_Set(GPIOC,PIN6|PIN7|PIN8|PIN9,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_2M,GPIO_PUPD_PU);//���� ��������
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
        case 1:	MO(id,SetData,1,1);                                     //ʹ��
            break;
        case 2:	MO(id,SetData,0,1);                                     //ʧ��
            break;
        case 3:	UM(id,SetData,ElmoMotor[0].mode,1);                     //����ģʽ
            break;	
        case 4:	BG(id);                                                 //��ʼ�˶�
            break;
        case 5:	ST(id);                                                 //ֹͣ
            break;
        case 6: JV(id, SetData, ElmoMotor[0].ValueSet.speed,1);         //�����˶��ٶ�
            break;
        case 7: SP(id, SetData, ElmoMotor[0].Limits.PosSpLimit,1);      //PTPģʽ�ٶ�
            break;
        case 8: TC(id, SetData, ElmoMotor[0].ValueSet.current,1);       //����ת�أ�������
            break;
        case 9:	PA(id, SetData, ElmoMotor[0].ValueSet.pulse,1);         //����λ��
            break;
        case 10:elmomove=true;                                          //elmo����
            break;
        case 11:elmoback=true;                                          //Elmo��λ
            break;
        case 12:valveCtrl(0,1);										    //12:���׿�
            break;
        case 13:valveCtrl(0,0);										    //13:���׹�
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
        case 1:	motor[id].enable=ENABLE;                                //1:ʹ��
            break;
        case 2:	motor[id].enable=DISABLE;                               //2:ʧ��
            break;
        case 3:	motor[id].ValueSet.current=100;                		    //3:ת�أ�������
            break;	
        case 4:	motor[id].start=ENABLE;                                 //4:��ʼ�˶�
            break;
        case 5:	motor[id].start=DISABLE;                                //5:ֹͣ
            break;
        case 6:    motor[id].mode=position;                     		//6:λ��ģʽ
            break;
        case 7:    motor[id].mode=RPM;									//7:�ٶ�ģʽ
            break;
        case 8: {   motor[id].mode=zero;								    //8:Ѱ��ģʽ
					motor[id].Limits.ZeroSpLimit=1000;}
            break;
        case 9:		motor[id].ValueSet.angle=180;						    //9:����λ��
            break;
        case 10:	motor[id].ValueSet.speed=1000;						//10:�����ٶ�
            break;
        case 11: {  motor[id].mode=zero;								    //11:����Ѱ��ģʽ
				   motor[id].Limits.ZeroSpLimit=-1000;}
            break;
        case 12:   valveCtrl(0,1);										//12:���׿�
            break;
        case 13:   valveCtrl(0,0);										//13:���׹�
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
