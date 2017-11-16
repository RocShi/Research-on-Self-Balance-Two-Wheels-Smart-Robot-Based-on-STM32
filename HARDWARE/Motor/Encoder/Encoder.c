#include "Encoder.h"

//�����������ӿڳ�ʼ����TIM2��TIM4
void Encoder_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO��ʼ���ṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  //��ʱ����ʼ���ṹ��
	TIM_ICInitTypeDef TIM_ICInitStructure;  //��ʱ�����벶���ʼ���ṹ��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3,ENABLE);  //������ʱ��2�붨ʱ��3��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //����GPIOAʱ��
	
	//GPIOA��ʼ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7;  //GPIOA.0��GPIOA.1��GPIOA.6��GPIOA.7
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;  //��������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//TIM2��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period=0xFFFF;  //�Զ���װ��ֵ�����ֵ
	TIM_TimeBaseStructure.TIM_Prescaler=0;  //Ԥ��Ƶϵ��������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //ʱ�ӷָ�
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);  //TIM2ʱ����ʼ��
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);  //TIM3ʱ����ʼ��
	
	//ʹ�ñ�����ģʽ3
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	//TIM2��TIM3���벶��IC����ʼ��
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;  //�˴�ѡ�����벶��ͨ��Ϊͨ��1
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;  //�����ز���
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;  //ӳ�䵽TI1��
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;  //�������벶��Ԥ��Ƶ���ӣ�����Ƶ
	TIM_ICInitStructure.TIM_ICFilter=10;  //�˲�������N=4
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���TIM2�ĸ��±�־λ
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);//���TIM2�ĸ��±�־λ
	
	//ʧ�ܸ����ж��벶��Ƚ��ж�
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,DISABLE);  
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC1,DISABLE);
	
	//���ü�����
	TIM_SetCounter(TIM2,0);
	TIM_SetCounter(TIM3,0);
	
	//ʹ��TIM2��TIM3
	TIM_Cmd(TIM2,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

//��ȡ������������
int Encoder_Read(u8 TIMx)
{
	int16_t Encoder_Val;  //һ��Ҫע�����з��Ŷ����ͣ������ֽڣ���������������u16�ͺ�int�ͣ���ɶ�ʱ���޷���ȡ���ٶ�
	switch(TIMx)
	{
		case 2: Encoder_Val=TIM2->CNT; TIM2->CNT=0; break;  //��ȡ������ֵ��������
    case 3: Encoder_Val=TIM3->CNT; TIM3->CNT=0; break;  //��ȡ������ֵ��������
		default:Encoder_Val=0;
	}
	return Encoder_Val;
}
