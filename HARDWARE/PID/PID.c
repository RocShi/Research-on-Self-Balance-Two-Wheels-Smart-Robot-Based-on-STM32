#include "PID.h"
#include "inv_mpu.h"
#include "MPU6050.h"
#include "Motor.h"
#include "Encoder.h"

//MPU6050 数据中断（5ms定时）服务函数，具体的姿态调整由此函数完成
int EXTI15_10_IRQHandler(void)
{
	if (EXTI_Come)  //MPU6050中断到来
	{
		EXTI_ClearITPendingBit(EXTI_Line12);  //清除中断标志位
		SpeedControl=!SpeedControl;  //速度控制标志取反
		
		ShowCNT+=1;
		if (ShowCNT==10)  //以MPU6050的5ms数据中断周期为基准，为LCD提供了50ms的刷新周期
		{
			StopShow=0;  //可以刷新LCD了
			ShowCNT=0;
		}
		
		if (SpeedControl==1)  //此处判别的存在决定了速度调整周期为5ms*20=100ms
		{
			Update();  //更新姿态数据
//			GesturePWM=Gesture(BalanAngle,BalanG);  //姿态环PWM
//			APWM=GesturePWM-0;  //电机A PWM
//			BPWM=GesturePWM-0;  //电机B PWM
//			Motor(APWM, BPWM);
			return 0;
		}
		
		EcA=-Encoder_Read(2);
		EcB=Encoder_Read(3);
		Update();  //更新姿态数据
		GesturePWM=Gesture(BalanAngle,BalanG);  //姿态环PWM
		SpeedPWM=Speed(EcA,EcB);  //速度环PWM
		APWM=GesturePWM-SpeedPWM;  //电机A PWM
		BPWM=GesturePWM-SpeedPWM;  //电机B PWM
		Motor(APWM, BPWM);
	}
	return 0;
}

//姿态环：PD控制器
int Gesture(float Angle, float Gyro)
{
	float AngleBias;  //姿态角偏差
	int GPWM;  //姿态环PWM输出
	AngleBias=Angle-BalanceAngle;  //计算姿态角偏差
	GPWM=Kp_Gesture*AngleBias+Kd_Gesture*Gyro;  //计算姿态环PWM输出
	return GPWM;
}

//速度环：PI控制器
int Speed(int EcA, int EcB)
{
	static float EncoderBias=0,	EB_Integral=0;  //速度偏差与偏差积分
	int SPWM;  //速度环PWM输出
	EncoderBias=EncoderBias*0.8+(EcA+EcB-0)*0.2;  //速度偏差，低通滤波
	EB_Integral+=EncoderBias;  //偏差积分
	if (EB_Integral>10000) EB_Integral=10000;  //积分限幅
	if (EB_Integral<-10000) EB_Integral=-10000;  //积分限幅
	SPWM=Kp_Speed*EncoderBias+Ki_Speed*EB_Integral;  //计算速度环PWM输出
	return SPWM;
}

//转向环
int Turn()
{
	
}

//姿态更新
void Update(void)
{
	MPU_DMP_GetData(&pitch, &roll, &yaw);  //获取全部欧拉角数据
	MPU_Get_Gyro(&gx, &gy, &gz);  //获取全部陀螺仪数据
	BalanAngle=roll;  //更新平衡倾角
	BalanG=gx;  //更新平衡角速度
}
