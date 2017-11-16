#ifndef _Motor_H  //��������
#define _Motor_H
#include "sys.h"

#define AIN1 PAout(2)
#define AIN2 PAout(3)
#define BIN1 PAout(4)
#define BIN2 PAout(5)
#define Motor1 TIM_SetCompare1
#define Motor2 TIM_SetCompare4

void Motor_Init(u16 arr, u16 prer);  //�����ʱ����ʼ��������arr���Զ���װ��ֵ��prer:Ԥ��Ƶֵ
void Motor(int PWM1, int PWM2);  //������ƺ�����PWM1�����1��PWMռ�ձȣ�PWM2�����2��PWMռ�ձ�

#endif
