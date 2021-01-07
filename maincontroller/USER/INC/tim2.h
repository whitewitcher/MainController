#ifndef _TIM2_
#define _TIM2_

#include "sys.h"
#include "param.h"
#include "can1.h"
#include "can2.h"
#include "VESC.h"

void TIM2_Init(void);
void TIM2_IRQHandler(void);


#endif
