#ifndef _TIME_H
#define _TIME_H
#include "sys.h"
#include "GLOBAL.h"
#include "led.h"
#include "delay.h"
#include "M8266_init.h"
#include "M8266WIFIDrv.h"
void TIM4_Int_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);
#endif
