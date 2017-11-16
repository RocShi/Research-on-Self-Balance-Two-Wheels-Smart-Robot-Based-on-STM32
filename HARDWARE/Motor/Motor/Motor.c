#include "Motor.h"

void Motor_Init(u16 arr, u16 prer)  //arr:�Զ���װ��ֵ��prer:Ԥ��Ƶֵ
{
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO��ʼ���ṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  //��ʱ����ʼ���ṹ��
	TIM_OCInitTypeDef TIM_OCInitStructure;  //��ʱ������Ƚϳ�ʼ���ṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  //������ʱ��1��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //����GPIOAʱ��
	
	//���PWM�������ų�ʼ�����߼���ʱ��1ͨ��1��ͨ��4��ʼ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_11;  //GPIOA.8��GPIOA.11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//�������������ų�ʼ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;  //GPIOA.2��GPIOA.3��GPIOA.4��GPIOA.5
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//�߼���ʱ��1�ĳ�ʼ��
	TIM_TimeBaseStructure.TIM_Period=arr;  //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler=prer;  //Ԥ��Ƶϵ��
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_ClockDivision=0;  //ʱ�ӷָ�
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);  //TIM1ʱ����ʼ��
	
	//�߼���ʱ��1ͨ��1��ͨ��4������Ƚ�(OC)��ʼ��
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;  //�˴�ѡ������Ƚ�ģʽΪPWM1
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;  //��ʼռ�ձ�Ϊ0
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;  //����Ƚϼ���Ϊ�ߣ����ߵ�ƽ��Ч
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);  //TIM1 MOEʹ��
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);  //����Ƚ�1Ԥװ��ʹ��
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);  //����Ƚ�4Ԥװ��ʹ��
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);  //�߼���ʱ��1�Զ���װ�ؼĴ���ARRԤװ��ʹ��
	
	TIM_Cmd(TIM1,ENABLE);  //ʹ�ܸ߼���ʱ��1
	
	Motor(0,0);
}

//������ƺ�����PWM1�����1��PWMռ�ձȣ�PWM2�����2��PWMռ�ձ�
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
