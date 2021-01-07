#ifndef _LED_H
#define _LED_H

#include "sys.h"
#include "includes.h"

#define LED_RED           PIN4
#define LED_YELLOW        PIN5
#define LED_BLUE          PIN6
#define LED_GREEN         PIN7

#define LED_RED_OFF  			GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define LED_YELLOW_OFF  		GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define LED_BLUE_OFF    	    GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define LED_GREEN_OFF  			GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define LED_ALL_OFF             GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7)

#define LED_RED_ON   			GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define LED_YELLOW_ON   		GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define LED_BLUE_ON   			GPIO_ResetBits(GPIOA,GPIO_Pin_6)
#define LED_GREEN_ON   			GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define LED_ALL_ON              GPIO_ResetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7)

#define LED_RED_TOGGLE			GPIOA->ODR ^= PIN4
#define LED_YELLOW_TOGGLE 	    GPIOA->ODR ^= PIN5
#define LED_BLUE_TOGGLE			GPIOA->ODR ^= PIN6
#define LED_GREEN_TOGGLE		GPIOA->ODR ^= PIN7

/****LED??****/
enum LED_MODE { normal, error};

void LED_Configuration(void);
void Led_Show(void);
void LedDisplay(u8 led, u8 num);

#endif
