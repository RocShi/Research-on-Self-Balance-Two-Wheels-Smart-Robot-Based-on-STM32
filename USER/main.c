#include "sys.h"  //This file has beem modified by the Lubuntu at 2017.8.4
#include "led.h"
#include "Key.h"
#include "lcd.h"
#include "usart.h"
#include "delay.h"
#include "MPU6050.h"
#include "Motor.h"
#include "Encoder.h"
#include "PID.h"

u16 t=0;
float pitch, roll, yaw, BalanAngle; 		//欧拉角，平衡倾角
short gx, gy, gz, BalanG;  //陀螺仪原始数据，平衡角速度
short ax, ay, az;  //加速度计原始数据
int GesturePWM;  //姿态环PWM
int SpeedPWM;  //速度环PWM
int EcA, EcB;  //编码器测量值
int APWM;  //电机A PWM
int BPWM;  //电机B PWM
float Kp_Gesture=360, Kd_Gesture=1.76;  //姿态环PID参数：360，1.8；1.5会晃，2会抖
float Kp_Speed=30, Ki_Speed=0.35;  //速度环PID参数：30,0.3。起初参数取得负值，错误地起到了“负反馈”的作用
u8 StopShow;  //LCD刷新控制标志
u8 ShowCNT;  //LCD刷新计数
u8 SpeedControl=0;  //速度控制标志
short temp;  //中间变量

int main(void)
{
	delay_init();  //延时函数初始化
	LED_Init();  //进行LED相关GPIO的初始化
	Key_Init();  //按键初始化
	Motor_Init(7199, 0);  //电机PWM波频率f=72000/(7199+1)=10KHz
	Encoder_Init();  //霍尔编码器初始化
	MPU_Init();
	uart_init(9600);  //串口初始化
	LCD_Init();  //LCD初始化。LCD初始化应在其它外设初始化完成后进行，否则会出现板子掉电或复位后LCD白屏、程序无法正常运行的问题
	
	LCD_Fill(0,0,538,538,WHITE);
	POINT_COLOR=RED;
	while(MPU_DMP_Init())
	{
		LCD_ShowString(10, 150, 200, 16, 16, "CAN NOT INITIAL MPU6050!");
		delay_ms(500);
		LCD_Fill(0,0,538,538,WHITE);
	}
	LCD_Fill(0,0,538,538,WHITE);
	POINT_COLOR=GREEN;
	LCD_ShowString(10, 150, 200, 16, 16, "MPU6050 IS READY!");
	LCD_Fill(0,0,538,538,WHITE);
	POINT_COLOR=BLUE;
	LCD_ShowString(10, 80, 200, 16, 16, "The current data is:");
	LCD_ShowString(10, 120, 200, 16, 16, " Temp:    . C");  //温度。每个数字、字符占一个8*8的像素块，这点对于计算屏幕显示的像素坐标很重要
	LCD_ShowString(10, 140, 200, 16, 16, "Pitch:    . ");  //Pitch
	LCD_ShowString(10, 160, 200, 16, 16, " Roll:    . ");  //Roll
	LCD_ShowString(10, 180, 200, 16, 16, "  Yaw:    . ");  //Yaw
	
	MPU_EXTI_Init();  //MPU6050数据中断初始化
	
	while(1)
	{
		temp=MPU_Get_Temp();  //获取温度的100倍
		MPU_DMP_GetData(&pitch, &roll, &yaw);  //获取欧拉角
		if(temp<0)
		{
			LCD_ShowChar(10+48, 120, '-', 16, 0);		//显示负号
			temp=-temp;  //将温度值转换为正值
		} else LCD_ShowChar(10+48, 120, ' ', 16, 0);	//不显示负号
		LCD_ShowNum(10+48+8, 120, temp/100, 3, 16);		//显示整数部分	    
		LCD_ShowNum(10+48+40, 120, ((temp-temp%10)/10)%10, 1, 16);	//显示小数部分。漂地太严重
		
		temp=pitch*10;  //计算得到的欧拉角保留1位小数
		if(temp<0)
		{
			LCD_ShowChar(10+48, 140, '-', 16, 0);		//显示负号
			temp=-temp;		//将pitch转化为正数
		}else LCD_ShowChar(10+48, 140, ' ', 16, 0);		//不显示负号 
		LCD_ShowNum(10+48+8,140, temp/10, 3, 16);		  //显示整数部分	    
		LCD_ShowNum(10+48+40,140, temp%10, 1, 16);		//显示小数部分 
		
		temp=roll*10;
		if(temp<0)
		{
			LCD_ShowChar(10+48,160, '-', 16, 0);		//显示负号
			temp=-temp;		//将roll转为正数
		}else LCD_ShowChar(10+48,240,' ', 16, 0);		//不显示负号 
		LCD_ShowNum(10+48+8,160, temp/10, 3, 16);		//显示整数部分	    
		LCD_ShowNum(10+48+40,160, temp%10, 1, 16);	//显示小数部分 
		
		temp=yaw*10;
		if(temp<0)
		{
			LCD_ShowChar(10+48, 180, '-', 16, 0);		//显示负号
			temp=-temp;		//将yaw转为正数
		}else LCD_ShowChar(10+48,180, ' ', 16, 0);		//不显示负号 
		LCD_ShowNum(10+48+8,180, temp/10, 3, 16);		  //显示整数部分	    
		LCD_ShowNum(10+48+40,180, temp%10, 1, 16);		//显示小数部分  
		t++;
		
		if(t==2)  //闪烁周期T=50ms*20=1s
		{
			LED1=!LED1;  //LED闪烁用于指示程序运行状态
			printf("电机A的当前脉冲数为 %d \r\n\n",Encoder_Read(2));  //向串口打印电机A脉冲数
			printf("电机B的当前脉冲数为 %d \r\n\n",Encoder_Read(3));  //向串口打印电机B脉冲数
			t=0;
		}
		StopShow=1;
		ShowCNT=0;
		while(StopShow);		
	}
}
