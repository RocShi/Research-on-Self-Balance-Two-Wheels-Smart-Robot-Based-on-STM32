#include "Encoder.h"

//霍尔编码器接口初始化：TIM2与TIM4
void Encoder_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO初始化结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  //定时器初始化结构体
	TIM_ICInitTypeDef TIM_ICInitStructure;  //定时器输入捕获初始化结构体
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3,ENABLE);  //开启定时器2与定时器3的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //开启GPIOA时钟
	
	//GPIOA初始化
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7;  //GPIOA.0、GPIOA.1、GPIOA.6、GPIOA.7
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;  //浮空输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//TIM2和TIM3初始化
	TIM_TimeBaseStructure.TIM_Period=0xFFFF;  //自动重装载值，最大值
	TIM_TimeBaseStructure.TIM_Prescaler=0;  //预分频系数，不分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //时钟分割
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);  //TIM2时基初始化
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);  //TIM3时基初始化
	
	//使用编码器模式3
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	//TIM2和TIM3输入捕获（IC）初始化
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;  //此处选择输入捕获通道为通道1
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;  //上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;  //映射到TI1上
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;  //配置输入捕获预分频因子，不分频
	TIM_ICInitStructure.TIM_ICFilter=10;  //滤波器长度N=4
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清除TIM2的更新标志位
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);//清除TIM2的更新标志位
	
	//失能更新中断与捕获比较中断
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,DISABLE);  
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC1,DISABLE);
	
	//重置计数器
	TIM_SetCounter(TIM2,0);
	TIM_SetCounter(TIM3,0);
	
	//使能TIM2和TIM3
	TIM_Cmd(TIM2,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

//读取编码器脉冲数
int Encoder_Read(u8 TIMx)
{
	int16_t Encoder_Val;  //一定要注意是有符号短整型（两个字节）变量！起初定义成u16型和int型，造成定时器无法读取负速度
	switch(TIMx)
	{
		case 2: Encoder_Val=TIM2->CNT; TIM2->CNT=0; break;  //读取计数器值，并重置
    case 3: Encoder_Val=TIM3->CNT; TIM3->CNT=0; break;  //读取计数器值，并重置
		default:Encoder_Val=0;
	}
	return Encoder_Val;
}
