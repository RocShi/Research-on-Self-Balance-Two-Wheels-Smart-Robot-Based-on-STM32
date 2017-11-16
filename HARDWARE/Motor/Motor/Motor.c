#include "Motor.h"

void Motor_Init(u16 arr, u16 prer)  //arr:自动重装载值，prer:预分频值
{
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO初始化结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  //定时器初始化结构体
	TIM_OCInitTypeDef TIM_OCInitStructure;  //定时器输出比较初始化结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  //开启定时器1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //开启GPIOA时钟
	
	//电机PWM输入引脚初始化：高级定时器1通道1和通道4初始化
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_11;  //GPIOA.8和GPIOA.11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//电机方向控制引脚初始化
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;  //GPIOA.2、GPIOA.3、GPIOA.4和GPIOA.5
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//高级定时器1的初始化
	TIM_TimeBaseStructure.TIM_Period=arr;  //自动重装载值
	TIM_TimeBaseStructure.TIM_Prescaler=prer;  //预分频系数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseStructure.TIM_ClockDivision=0;  //时钟分割
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);  //TIM1时基初始化
	
	//高级定时器1通道1、通道4的输出比较(OC)初始化
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;  //此处选择输出比较模式为PWM1
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;  //初始占空比为0
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;  //输出比较极性为高，即高电平有效
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);  //TIM1 MOE使能
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);  //输出比较1预装载使能
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);  //输出比较4预装载使能
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);  //高级定时器1自动重装载寄存器ARR预装载使能
	
	TIM_Cmd(TIM1,ENABLE);  //使能高级定时器1
	
	Motor(0,0);
}

//电机控制函数，PWM1：电机1的PWM占空比，PWM2：电机2的PWM占空比
void Motor(int PWM1, int PWM2)  
{
	if (PWM1==0)
	{
		AIN1=0, AIN2=0;
		Motor1(TIM1, 0);
	}
	else if (PWM1<0)
	{
		AIN1=1, AIN2=0;
		Motor1(TIM1, -PWM1);
	}
	else
	{
		AIN1=0, AIN2=1;
		Motor1(TIM1, PWM1);
	}
	
	if (PWM2==0)
	{
		BIN1=0, BIN2=0;
		Motor2(TIM1, 0);
	}
	else if (PWM2<0)
	{
		BIN1=0, BIN2=1;
		Motor2(TIM1, -PWM2);
	}
	else
	{
		BIN1=1, BIN2=0;
		Motor2(TIM1, PWM2);
	}
}
