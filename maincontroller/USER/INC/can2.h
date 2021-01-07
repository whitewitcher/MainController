#ifndef __CAN2_H
#define __CAN2_H

#include "sys.h"
#include "param.h"
#include "maroc.h"
#include "motor.h"
#include "elmo.h"
#include "key.h"

void CAN2_Init(void);
void CAN2_RX0_IRQHandler(void);
void CAN2_RX1_IRQHandler(void);
void Tx_CurrentInput(u8 id);
void valveCtrl(u8 ID,bool statue);

extern bool value[7];

#endif
