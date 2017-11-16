#include "Key.h"

void Key_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PA,PC�˿�ʱ��
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //�ر�JTAG
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;  //��������
	GPIO_Init(GPIOC,&GPIO_InitStructure);  //��ʼ��PC5
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;  //��������
	GPIO_Init(GPIOA,&GPIO_InitStructure);  //��ʼ��PA15
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;  //��������
	GPIO_Init(GPIOA,&GPIO_InitStructure);  //��ʼ��PA0
}
