#include "usart2.h"

void Usart2_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	DMA_InitTypeDef DMA_InitStruct;
	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;            //???
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_InitStruct.USART_BaudRate=115200;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_Init(USART2,&USART_InitStruct);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	DMA_DeInit(DMA1_Stream6);
	DMA_InitStruct.DMA_Channel=DMA_Channel_4;
	DMA_InitStruct.DMA_PeripheralBaseAddr=(uint32_t)(&USART2->DR);
	DMA_InitStruct.DMA_Memory0BaseAddr=(uint32_t)(&usart2_tx[0]);
	DMA_InitStruct.DMA_Mode=DMA_Mode_Normal;
	DMA_InitStruct.DMA_DIR=DMA_DIR_MemoryToPeripheral;
	DMA_InitStruct.DMA_BufferSize=256;
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_Priority=DMA_Priority_Medium;
	DMA_InitStruct.DMA_FIFOThreshold=DMA_FIFOThreshold_Full;
	DMA_InitStruct.DMA_FIFOMode=DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_MemoryBurst=DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream6,&DMA_InitStruct);
	
	
	DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);
	NVIC_InitStruct.NVIC_IRQChannel=DMA1_Stream6_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
	USART_Cmd(USART2,ENABLE);
}

vu8 interface=0;               //记录画面id
u8 USART2_RX_STA = 0;          //接收存储计数
u8 usart2_rx[40];              //接收数组
bool receive=false;
void USART2_IRQHandler(void) 
{
	u8 temp=0;                 //临时接收
	if(USART_GetITStatus(USART2,USART_IT_ORE_RX)!=RESET){                 //接收溢出
		USART_ReceiveData(USART2);
		USART2_RX_STA = 0;
		memset(usart2_rx, 0, sizeof(usart2_rx));                          //归零
	}
	else if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET){              //接收中断
		temp=USART_ReceiveData(USART2);                                   //接收数据
		USART_ClearFlag(USART2,USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);                    //清除标志位
		if(temp==0xEE)                                                    //判断开头
			USART2_RX_STA|=0x40;
		else if((USART2_RX_STA & 0x40)!=0 && (USART2_RX_STA & 0x80)==0){       //接收数据中
			if(temp==0xFF)                                                //判断结尾
				USART2_RX_STA|=0x80;
			else{                                                         //存储数据
				usart2_rx[USART2_RX_STA &0x3F]=temp;
				USART2_RX_STA++;
				if((USART2_RX_STA &0x3F)>40)
					USART2_RX_STA=0;
			}
		}
		if((USART2_RX_STA &0x80)!=0){                                     //接收完成 处理数据
			receive=true;
		}
		
	}
	
}

void UsartLCDReceive(){
	if(receive){
	if(usart2_rx[1]==0x01)                                        //活动画面id
				interface=usart2_rx[3];
			
			
			switch(usart2_rx[3]){                                         //判断画面id
				case 0:{
					
				}break;
				
				case 1:{
					switch(usart2_rx[5]){                                 //判断控件id
						#ifdef USE_ELMO
						case 1:{
							#ifdef USE_ELMO
							ElmoInit(CAN2);
							ElmoMotorInit();
							elmoback=0;
							elmomove=0;
							#endif
						}break;
						case 2:{                                          //复位1
							elmoback=1;
						}break;
						case 3:{                                          //发射
							elmomove=1;
						}break;
						case 4:{                                          //复位2
							elmoback=2;
						}break;
						case 5:{                                          //急停 失能
								MO(1,SetData,0,1);
								valveCtrl(0,0);
						}break;
						
						case 9:{                                          //初始位置
								elmo_startps=atof((char*)(&usart2_rx[7]));
						}break;
						case 11:{                                         //发射位置
								elmo_throw=atof((char*)(&usart2_rx[7]));
						}break;
						case 13:{                                         //停止位置
								elmo_stop=atof((char*)(&usart2_rx[7]));
						}break;
						#endif
					}
				}break;
				
				case 2:{
					switch(usart2_rx[5]){                                 //判断控件id
						case 1:{                                          //急停 失能
							for(int id=0;id<8;id++)
								motor[id].enable=DISABLE;
						}break;
						case 2:{                                          
							switch(usart2_rx[7]){
								case 0: motor[0].mode=RPM;break;
								case 1: motor[0].mode=zero;break;
								case 2: motor[0].mode=position;break;
							}
						}break;
						case 3:{                                          
							switch(usart2_rx[7]){
								case 0: motor[1].mode=RPM;break;
								case 1: motor[1].mode=zero;break;
								case 2: motor[1].mode=position;break;
							}
						}break;
						case 4:{                                          
							switch(usart2_rx[7]){
								case 0: motor[2].mode=RPM;break;
								case 1: motor[2].mode=zero;break;
								case 2: motor[2].mode=position;break;
							}
						}break;
						case 5:{                                          
							switch(usart2_rx[7]){
								case 0: motor[3].mode=RPM;break;
								case 1: motor[3].mode=zero;break;
								case 2: motor[3].mode=position;break;
							}
						}break;
						case 6:{
								motor[0].enable=usart2_rx[8];
							}break;
						case 7:{
								motor[1].enable=usart2_rx[8];
							}break;
						case 8:{
								motor[2].enable=usart2_rx[8];
							}break;
						case 9:{
								motor[3].enable=usart2_rx[8];
							}break;
						case 10:{
								motor[0].start=usart2_rx[8];
							}break;
						case 11:{
								motor[1].start=usart2_rx[8];
							}break;
						case 12:{
								motor[2].start=usart2_rx[8];
							}break;
						case 13:{
								motor[3].start=usart2_rx[8];
							}break;
						case 14:{
								motor[0].ValueSet.speed=atof((char*)(&usart2_rx[7]));
							}break;
						case 15:{
								motor[1].ValueSet.speed=atof((char*)(&usart2_rx[7]));
							}break;
						case 16:{
								motor[2].ValueSet.speed=atof((char*)(&usart2_rx[7]));
							}break;
						case 17:{
								motor[3].ValueSet.speed=atof((char*)(&usart2_rx[7]));
							}break;
						case 18:{
								motor[0].ValueSet.angle=atof((char*)(&usart2_rx[7]));
							}break;
						case 19:{
								motor[1].ValueSet.angle=atof((char*)(&usart2_rx[7]));
							}break;
						case 20:{
								motor[2].ValueSet.angle=atof((char*)(&usart2_rx[7]));
							}break;
						case 21:{
								motor[3].ValueSet.angle=atof((char*)(&usart2_rx[7]));
							}break;
						case 22:{
								motor[0].Limits.PosSpLimit=atof((char*)(&usart2_rx[7]));
								motor[0].Limits.ZeroSpLimit=atof((char*)(&usart2_rx[7]));
							}break;
						case 23:{
								motor[1].Limits.PosSpLimit=atof((char*)(&usart2_rx[7]));
								motor[1].Limits.ZeroSpLimit=atof((char*)(&usart2_rx[7]));
							}break;
						case 24:{
								motor[2].Limits.PosSpLimit=atof((char*)(&usart2_rx[7]));
								motor[2].Limits.ZeroSpLimit=atof((char*)(&usart2_rx[7]));
							}break;
						case 25:{
								motor[3].Limits.PosSpLimit=atof((char*)(&usart2_rx[7]));
								motor[3].Limits.ZeroSpLimit=atof((char*)(&usart2_rx[7]));
							}break;						
					}					
				}break;
				
				case 3:{
					switch(usart2_rx[5]){                                 //判断控件id
						case 1:{                                          //急停 失能
							for(int id=0;id<8;id++)
								MO(id,SetData,0,1);
						}break;
						case 2:{                                          
							switch(usart2_rx[7]){
								case 0: UM(0,SetData,RPM,1);
								case 1: UM(0,SetData,zero,1);
								case 2: UM(0,SetData,position,1);
							}
						}break;
						case 3:{                                          
							switch(usart2_rx[7]){
								case 0: UM(1,SetData,RPM,1);
								case 1: UM(1,SetData,zero,1);
								case 2: UM(1,SetData,position,1);
							}
						}break;
						case 4:{                                          
							switch(usart2_rx[7]){
								case 0: UM(2,SetData,RPM,1);
								case 1: UM(2,SetData,zero,1);
								case 2: UM(2,SetData,position,1);
							}
						}break;
						case 5:{                                          
							switch(usart2_rx[7]){
								case 0: UM(3,SetData,RPM,1);
								case 1: UM(3,SetData,zero,1);
								case 2: UM(3,SetData,position,1);
							}
						}break;
						case 6:{
								MO(0,SetData,usart2_rx[8],1);
							}break;
						case 7:{
								MO(1,SetData,usart2_rx[8],1);
							}break;
						case 8:{
								MO(2,SetData,usart2_rx[8],1);
							}break;
						case 9:{
								MO(3,SetData,usart2_rx[8],1);
							}break;
						case 10:{
								if(usart2_rx[8])BG(0);
								else 			ST(0);
							}break;
						case 11:{
								if(usart2_rx[8])BG(1);
								else 			ST(1);
							}break;
						case 12:{
								if(usart2_rx[8])BG(2);
								else 			ST(2);
							}break;
						case 13:{
								if(usart2_rx[8])BG(3);
								else 			ST(3);
							}break;
						case 14:{
								ElmoMotor[0].ValueSet.speed=atof((char*)(&usart2_rx[7]));
								JV(0, SetData, atof((char*)(&usart2_rx[7])),1);
							}break;
						case 15:{
								ElmoMotor[0].ValueSet.speed=atof((char*)(&usart2_rx[7]));
								JV(1, SetData, atof((char*)(&usart2_rx[7])),1);
							}break;
						case 16:{
								ElmoMotor[0].ValueSet.speed=atof((char*)(&usart2_rx[7]));
								JV(2, SetData, atof((char*)(&usart2_rx[7])),1);
							}break;
						case 17:{
								ElmoMotor[0].ValueSet.speed=atof((char*)(&usart2_rx[7]));
								JV(3, SetData, atof((char*)(&usart2_rx[7])),1);
							}break;
						case 18:{
								ElmoMotor[0].ValueSet.pulse=atof((char*)(&usart2_rx[7]));
								PA(0, SetData, ((atof((char*)(&usart2_rx[7]))*ElmoMotor[0].Param.RATIO)/360.f)*ElmoMotor[0].Param.Pulse ,1);
							}break;
						case 19:{
								ElmoMotor[0].ValueSet.pulse=atof((char*)(&usart2_rx[7]));
								PA(1, SetData, ((atof((char*)(&usart2_rx[7]))*ElmoMotor[1].Param.RATIO)/360.f)*ElmoMotor[1].Param.Pulse ,1);
							}break;
						case 20:{
								ElmoMotor[0].ValueSet.pulse=atof((char*)(&usart2_rx[7]));
								PA(2, SetData, ((atof((char*)(&usart2_rx[7]))*ElmoMotor[2].Param.RATIO)/360.f)*ElmoMotor[2].Param.Pulse ,1);
							}break;
						case 21:{
								ElmoMotor[0].ValueSet.pulse=atof((char*)(&usart2_rx[7]));
								PA(3, SetData, ((atof((char*)(&usart2_rx[7]))*ElmoMotor[3].Param.RATIO)/360.f)*ElmoMotor[3].Param.Pulse ,1);
							}break;
						case 22:{
								ElmoMotor[0].Limits.PosSpLimit=atof((char*)(&usart2_rx[7]));
								SP(0, SetData, ElmoMotor[0].Limits.PosSpLimit,1);
							}break;
						case 23:{
								ElmoMotor[0].Limits.PosSpLimit=atof((char*)(&usart2_rx[7]));
								SP(1, SetData, ElmoMotor[0].Limits.PosSpLimit,1);
							}break;
						case 24:{
								ElmoMotor[0].Limits.PosSpLimit=atof((char*)(&usart2_rx[7]));
								SP(2, SetData, ElmoMotor[0].Limits.PosSpLimit,1);
							}break;
						case 25:{
								ElmoMotor[0].Limits.PosSpLimit=atof((char*)(&usart2_rx[7]));
								SP(3, SetData, ElmoMotor[0].Limits.PosSpLimit,1);
							}break;
					}
				}break;
				
				case 4:{
					switch(usart2_rx[5]){
						case 1:{
								valveCtrl(0,usart2_rx[8]);
							}break;
						case 2:{
								valveCtrl(1,usart2_rx[8]);
							}break;
						case 3:{
								valveCtrl(2,usart2_rx[8]);
							}break;
						case 4:{
								valveCtrl(3,usart2_rx[8]);
							}break;
						case 5:{
								valveCtrl(4,usart2_rx[8]);
							}break;
						case 6:{
								valveCtrl(5,usart2_rx[8]);
							}break;
						case 7:{
								valveCtrl(6,usart2_rx[8]);
							}break;
					}
				}break;
			}
			USART2_RX_STA=0;
			memset(usart2_rx, 0, sizeof(usart2_rx));
			receive=false;
			Beep_Show(1);
		}
}


u8 usart2_tx[256];              //发送数组
char str[32];
void UsartLCDshow(void){
	int i=0;
	switch(interface){
		case 0:{
			usart2_tx[i++]=0xee;
			usart2_tx[i++]=0xb1;
			usart2_tx[i++]=0x10;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x00;
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x05;
			usart2_tx[i++]=0x10;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=0x00;
			
			usart2_tx[i++]=0xff;
			usart2_tx[i++]=0xfc;
			usart2_tx[i++]=0xff;
			usart2_tx[i++]=0xff;
		}break;
		
		case 1:{
			usart2_tx[i++]=0xee;
			usart2_tx[i++]=0xb1;
			usart2_tx[i++]=0x12;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			
			usart2_tx[i++]=0xff;
			usart2_tx[i++]=0xfc;
			usart2_tx[i++]=0xff;
			usart2_tx[i++]=0xff;
		}break;
		
		case 2:{
			usart2_tx[i++]=0xee;
			usart2_tx[i++]=0xb1;
			usart2_tx[i++]=0x12;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x02;
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x06;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=motor[0].enable;
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x07;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=motor[1].enable;
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x08;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=motor[2].enable;
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x09;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=motor[3].enable;
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x0A;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=motor[0].start;
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x0B;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=motor[1].start;
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x0C;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=motor[2].start;
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x0D;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=motor[3].start;
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x0E;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",motor[0].ValueSet.speed);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x0F;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",motor[1].ValueSet.speed);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x10;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",motor[2].ValueSet.speed);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x11;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",motor[3].ValueSet.speed);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x12;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",motor[0].ValueSet.angle);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x13;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",motor[1].ValueSet.angle);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x14;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",motor[2].ValueSet.angle);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x15;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",motor[3].ValueSet.angle);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x16;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",motor[0].Limits.PosSpLimit);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x17;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",motor[1].Limits.PosSpLimit);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x18;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",motor[2].Limits.PosSpLimit);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x19;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",motor[3].Limits.PosSpLimit);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x1A;
			usart2_tx[i++]=0x00;
			switch(motor[0].mode){
				case 2:sprintf(str,"%s","速度");break;
				case 4:sprintf(str,"%s","寻零");break;
				case 5:sprintf(str,"%s","位置");break;				
			}
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x1C;
			usart2_tx[i++]=0x00;
			switch(motor[1].mode){
				case 2:sprintf(str,"%s","速度");break;
				case 4:sprintf(str,"%s","寻零");break;
				case 5:sprintf(str,"%s","位置");break;				
			}
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x1E;
			usart2_tx[i++]=0x00;
			switch(motor[2].mode){
				case 2:sprintf(str,"%s","速度");break;
				case 4:sprintf(str,"%s","寻零");break;
				case 5:sprintf(str,"%s","位置");break;				
			}
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x20;
			usart2_tx[i++]=0x00;
			switch(motor[3].mode){
				case 2:sprintf(str,"%s","速度");break;
				case 4:sprintf(str,"%s","寻零");break;
				case 5:sprintf(str,"%s","位置");break;				
			}
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0xff;
			usart2_tx[i++]=0xfc;
			usart2_tx[i++]=0xff;
			usart2_tx[i++]=0xff;
		}break;
		
		case 3:{
			usart2_tx[i++]=0xee;
			usart2_tx[i++]=0xb1;
			usart2_tx[i++]=0x12;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x03;
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x06;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=ElmoMotor[0].enable;
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x07;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=ElmoMotor[1].enable;
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x08;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=ElmoMotor[2].enable;
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x09;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=ElmoMotor[3].enable;
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x0A;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=ElmoMotor[0].start;
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x0B;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=ElmoMotor[1].start;
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x0C;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=ElmoMotor[2].start;
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x0D;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=ElmoMotor[3].start;
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x0E;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",ElmoMotor[0].ValueSet.speed);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x0F;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",ElmoMotor[1].ValueSet.speed);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x10;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",ElmoMotor[2].ValueSet.speed);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x11;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",ElmoMotor[3].ValueSet.speed);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x12;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",ElmoMotor[0].ValueSet.angle);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x13;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",ElmoMotor[1].ValueSet.angle);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x14;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",ElmoMotor[2].ValueSet.angle);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x15;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",ElmoMotor[3].ValueSet.angle);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x16;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",ElmoMotor[0].Limits.PosSpLimit);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x17;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",ElmoMotor[1].Limits.PosSpLimit);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x18;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",ElmoMotor[2].Limits.PosSpLimit);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x19;
			usart2_tx[i++]=0x00;
			sprintf(str,"%d",ElmoMotor[3].Limits.PosSpLimit);
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x1A;
			usart2_tx[i++]=0x00;
			switch(ElmoMotor[0].mode){
				case 2:sprintf(str,"%s","速度");break;
				case 4:sprintf(str,"%s","寻零");break;
				case 5:sprintf(str,"%s","位置");break;				
			}
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x1C;
			usart2_tx[i++]=0x00;
			switch(ElmoMotor[1].mode){
				case 2:sprintf(str,"%s","速度");break;
				case 4:sprintf(str,"%s","寻零");break;
				case 5:sprintf(str,"%s","位置");break;				
			}
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x1E;
			usart2_tx[i++]=0x00;
			switch(ElmoMotor[2].mode){
				case 2:sprintf(str,"%s","速度");break;
				case 4:sprintf(str,"%s","寻零");break;
				case 5:sprintf(str,"%s","位置");break;				
			}
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x20;
			usart2_tx[i++]=0x00;
			switch(ElmoMotor[3].mode){
				case 2:sprintf(str,"%s","速度");break;
				case 4:sprintf(str,"%s","寻零");break;
				case 5:sprintf(str,"%s","位置");break;				
			}
			usart2_tx[i++]=strlen(str);
			strcpy((char*)&usart2_tx[i],str);
			i+=strlen(str);
			
			usart2_tx[i++]=0xff;
			usart2_tx[i++]=0xfc;
			usart2_tx[i++]=0xff;
			usart2_tx[i++]=0xff;
		}break;
		
		case 4:{
			usart2_tx[i++]=0xee;
			usart2_tx[i++]=0xb1;
			usart2_tx[i++]=0x12;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x04;
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=flag.valve[0];
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x02;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=flag.valve[1];
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x03;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=flag.valve[2];
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x04;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=flag.valve[3];
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x05;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=flag.valve[4];
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x06;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=flag.valve[5];
			
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x07;
			usart2_tx[i++]=0x00;
			usart2_tx[i++]=0x01;
			usart2_tx[i++]=flag.valve[6];
			
			usart2_tx[i++]=0xff;
			usart2_tx[i++]=0xfc;
			usart2_tx[i++]=0xff;
			usart2_tx[i++]=0xff;
		}break;
	}
	USART2_Send(i);
}

void USART2_Send(u8 count){
	if(!flag.Usart2DMASendFinish)//若上一次传输未完成，则舍弃本次传输
    {
	  flag.Usart2DMASendFinish=1;
      DMA_SetCurrDataCounter(DMA1_Stream6,count);
      DMA_Cmd(DMA1_Stream6, ENABLE);
    }
}

void DMA1_Stream6_IRQHandler(void){
	if(DMA_GetITStatus(DMA1_Stream6,DMA_IT_TCIF6)){
		DMA_ClearITPendingBit(DMA1_Stream6,DMA_IT_TCIF6);
		DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);
		flag.Usart2DMASendFinish = 0;
	}
}

