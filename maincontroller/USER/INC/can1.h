#ifndef __CAN1_H
#define __CAN1_H

#include "sys.h"
#include "param.h"
#include "maroc.h"
#include "motor.h"

void CAN1_Init(void);
void CAN1_RX0_IRQHandler(void);


#endif
