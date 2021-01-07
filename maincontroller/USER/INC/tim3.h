#ifndef __TIM3_H
#define __TIM3_H

#include "sys.h"
#include "param.h"
#include "can1.h"
#include "motor.h"
#include "can2.h"

void TIM3_Init(void);
void TIM3_IRQHandler(void);

#endif
