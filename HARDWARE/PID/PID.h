#ifndef __PID_H  //条件编译
#define __PID_H	 
#include "sys.h"

#define BalanceAngle -7.5
#define EXTI_Come PAin(12)==0

extern float pitch, roll, yaw, BalanAngle; 		//欧拉角，平衡倾角
extern short gx, gy, gz, BalanG;  //陀螺仪原始数据，平衡角速度
extern short ax, ay, az;  //加速度计原始数据
extern int GesturePWM;  //姿态环PWM
extern int SpeedPWM;  //速度环PWM
extern int EcA, EcB;  //编码器测量值
extern int APWM;  //电机A PWM
extern int BPWM;  //电机B PWM
extern float Kp_Gesture, Kd_Gesture;  //姿态环PID参数
extern float Kp_Speed, Ki_Speed;  //速度环PID参数
extern u8 StopShow;  //LCD刷新控制标志
extern u8 ShowCNT;  //LCD刷新计数
extern u8 SpeedControl;  //速度控制标志

int EXTI15_10_IRQHandler(void);  //MPU6050 5ms定时中断服务函数，具体的姿态调整由此函数完成
int Gesture(float Angle, float Gyro);  //姿态环
int Speed(int EcA, int EcB);  //速度环
int Turn(void);  //转向环
void Update(void);  //姿态更新

#endif
