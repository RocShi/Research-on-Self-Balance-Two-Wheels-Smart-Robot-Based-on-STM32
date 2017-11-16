#ifndef _Motor_H  //条件编译
#define _Motor_H
#include "sys.h"

#define AIN1 PAout(2)
#define AIN2 PAout(3)
#define BIN1 PAout(4)
#define BIN2 PAout(5)
#define Motor1 TIM_SetCompare1
#define Motor2 TIM_SetCompare4

void Motor_Init(u16 arr, u16 prer);  //电机定时器初始化函数，arr：自动重装载值，prer:预分频值
void Motor(int PWM1, int PWM2);  //电机控制函数，PWM1：电机1的PWM占空比，PWM2：电机2的PWM占空比

#endif
