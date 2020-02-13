#ifndef _HW_H
#define _HW_H
#include "sys.h"
#define HW GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_2)
#define guscheak GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_1)
void GUS_Init(void);


#endif
