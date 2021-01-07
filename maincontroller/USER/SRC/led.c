#include "led.h"

void LED_Configuration()
{
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_Set(GPIOA,PIN4|PIN5|PIN6|PIN7,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_2M,GPIO_PUPD_NONE);
	LED_ALL_OFF;
}

void Led_Show(void)
{
	
//	static int ledcount=0;
//	if(ledcount++==0){
//		LED_ALL_ON;OSTimeDly(5000);
//		LED_ALL_OFF;OSTimeDly(5000);
//		LED_ALL_ON;OSTimeDly(5000);
//		LED_ALL_OFF;OSTimeDly(5000);
//	}ledcount%=5;
	LED_RED_ON;OSTimeDly(2000);
	LED_YELLOW_ON;OSTimeDly(2000);
	LED_BLUE_ON;OSTimeDly(2000);
	//LED_GREEN_ON;OSTimeDly(2000);
	LED_RED_OFF;OSTimeDly(2000);
	LED_YELLOW_OFF;OSTimeDly(2000);
	LED_BLUE_OFF;OSTimeDly(2000);
	//LED_GREEN_OFF;OSTimeDly(2000);
}

void LedDisplay(u8 led, u8 num)
{
  for(int i=0; i<num; i++)
  {
    GPIOA->BSRRH = led;
    OSTimeDly(2000);
    GPIOA->BSRRL = led;
    OSTimeDly(2000);
  }
}
