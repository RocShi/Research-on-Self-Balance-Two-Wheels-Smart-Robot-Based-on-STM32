#ifndef _Encoder_H  //条件编译
#define _Encoder_H
#include "sys.h"

void Encoder_Init(void);  //霍尔编码器接口初始化
int Encoder_Read(u8 TIMx);  //读取编码器脉冲数

#endif
