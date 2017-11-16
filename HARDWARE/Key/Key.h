#ifndef __Key_H  //条件编译
#define __Key_H	 
#include "sys.h"

#define Key0_On GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)==0
#define Key1_On GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)==0
#define WK_UP_On GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)==1

#define Key0_Off GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)==1
#define Key1_Off GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)==1
#define WK_UP_Off GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)==0

void Key_Init(void);//初始化

#endif
