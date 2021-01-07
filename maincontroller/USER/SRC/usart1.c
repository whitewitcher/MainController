#include "usart1.h"            //虚拟示波器
u8 usart1_tx[32];              //发送数组

void Usart1_Init(void){
	
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	DMA_InitTypeDef DMA_InitStruct;
	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);   //必须先开IO口时钟，再开串口、DMA
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;            //???
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_InitStruct.USART_BaudRate=115200;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_Init(USART1,&USART_InitStruct);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);		
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;	
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;	
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStruct);
	
	DMA_DeInit(DMA2_Stream7);
	DMA_InitStruct.DMA_Channel=DMA_Channel_4;
	DMA_InitStruct.DMA_PeripheralBaseAddr=(uint32_t)&USART1->DR;
	DMA_InitStruct.DMA_Memory0BaseAddr=(uint32_t)(&usart1_tx[0]);
	DMA_InitStruct.DMA_Mode=DMA_Mode_Normal;
	DMA_InitStruct.DMA_DIR=DMA_DIR_MemoryToPeripheral;
	DMA_InitStruct.DMA_BufferSize=0;
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_Priority=DMA_Priority_VeryHigh;
	DMA_InitStruct.DMA_FIFOThreshold=DMA_FIFOThreshold_HalfFull;
	DMA_InitStruct.DMA_FIFOMode=DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_MemoryBurst=DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream7,&DMA_InitStruct);
	
	DMA_ClearFlag (DMA2_Stream7,DMA_IT_TCIF7);					//清除中断标志
	DMA_ClearITPendingBit(DMA2_Stream7,DMA_IT_TCIF7);
	DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	NVIC_InitStruct.NVIC_IRQChannel=DMA2_Stream7_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_Cmd(USART1,ENABLE);
}

void USART1_Send(INT8U *data,INT8U count){
	if(!flag.Usart1DMASendFinish)//若上一次传输未完成，则舍弃本次传输
    {
		flag.Usart1DMASendFinish=1;                   //标志位置1 一定要在DMA使能之前
		memcpy(usart1_tx,data,count);
		DMA_SetCurrDataCounter(DMA2_Stream7,count);
		DMA_Cmd(DMA2_Stream7, ENABLE);

    }
}

void DMA2_Stream7_IRQHandler(void){
	if(DMA_GetITStatus(DMA2_Stream7,DMA_IT_TCIF7)){
		DMA_ClearITPendingBit(DMA2_Stream7,DMA_IT_TCIF7);
		DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);
		flag.Usart1DMASendFinish = 0;
	}
}


