#include "usart3.h"

void Usart3_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);   //�����ȿ�IO��ʱ�ӣ��ٿ����ڡ�DMA
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3);	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART3);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;            //???
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_InitStruct.USART_BaudRate=115200;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_Init(USART3,&USART_InitStruct);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);		
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;	
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStruct);	
	
	USART_Cmd(USART3,ENABLE);
}

vs32 Pulse_Abs;            //����ֵ������
u8 USART3_RX_STA = 0;      //���մ洢����
u8 usart3_rx[10];              //��������
void USART3_IRQHandler(void){
	u8 temp=0;                 //��ʱ����
	if(USART_GetITStatus(USART3,USART_IT_ORE_RX)!=RESET){                 //�������
		USART_ReceiveData(USART3);
		USART3_RX_STA = 0;
		memset(usart3_rx, 0, sizeof(usart3_rx));                          //����
	}
	else if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET){              //�����ж�
		temp=USART_ReceiveData(USART3);                                   //��������
		USART_ClearFlag(USART3,USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);                    //�����־λ
		if(temp==0xAB)                                                    //�жϿ�ͷ
			USART3_RX_STA|=0x20;
		if(temp==0xCD)
			USART3_RX_STA|=0x40;
		else if((USART3_RX_STA & 0x20)!=0 && (USART3_RX_STA & 0x40)!=0 && (USART3_RX_STA & 0x80)==0){       //����������
			if(temp==0x3D)                                                //�жϽ�β
				USART3_RX_STA|=0x80;
			else{                                                         //�洢����
				usart3_rx[USART3_RX_STA &0x1F]=temp;
				USART3_RX_STA++;
				if((USART3_RX_STA &0x1F)>10)
					USART3_RX_STA=0;
			}
		}
		if((USART3_RX_STA &0x80)!=0){                                     //������� ��������
			ChangeData(&usart3_rx[1],&usart3_rx[2]);
			ChangeData(&usart3_rx[3],&usart3_rx[4]);
			DecodeS32Data(&Pulse_Abs,&usart3_rx[1]);
			
			USART3_RX_STA=0;
			memset(usart3_rx, 0, sizeof(usart3_rx));
		}
	}
}
