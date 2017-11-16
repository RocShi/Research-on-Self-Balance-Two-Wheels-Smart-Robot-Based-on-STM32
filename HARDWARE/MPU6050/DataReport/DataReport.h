#ifndef __DataReport_H  //条件编译
#define __DataReport_H	 
#include "sys.h"

void TranChar(u8 ch);  //单字符发送函数
void ReToANO(u8 fun, u8 *data, u8 len);  //上报数据给匿名四轴上位机（V2.6）
void ReSoDa(short ax, short ay, short az, short gx, short gy, short gz);  //上报陀螺仪和加速度计的原始数据，用于姿态数据波形显示
void ReAllDa(short ax, short ay, short az, short gx, short gy, short gz, short pitch, short roll, short yaw);  //上报全部数据，用于3D姿态显示

#endif
