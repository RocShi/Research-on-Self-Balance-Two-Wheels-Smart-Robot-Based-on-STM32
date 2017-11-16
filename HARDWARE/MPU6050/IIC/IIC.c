#include "IIC.h"
#include "delay.h"

//IIC��ʼ��
void IIC_Init(void)  
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //����GPIOCʱ�ӣ�IICռ��������IO�ڣ�ʱ������������
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	IIC_SCL=0;
	IIC_SDA=0;
}

//IICͨ�ſ�ʼ
void IIC_Start(void)  
{
	SDA_OUT();  //��������Ϊ���״̬
  IIC_SCL=1;
	IIC_SDA=1;
	delay_us(5);
	IIC_SDA=0;  //IIC��ʱ���ߴ��ڸߵ�ƽʱ�������ߵĵ�ƽ�������½��أ���Ὺ��IICͨ��
	delay_us(5);
	IIC_SCL=0;  //ǯסIICʱ���ߵ�ƽ�����Կ�ʼ����������
}

//IICͨ��ֹͣ
void IIC_Stop(void)
{
	SDA_OUT();  //��������Ϊ���״̬
  IIC_SCL=0;
	IIC_SDA=0;
	delay_us(5);
	IIC_SCL=1;
	IIC_SDA=1;  //IIC��ʱ���ߴ��ڸߵ�ƽʱ�������ߵĵ�ƽ�����������أ����ر�
	delay_us(5);
}

/*
 *�����ȴ�Ӧ���ź�Ack����
 *����1������Ӧ���ź�ʧ��
 *����0������Ӧ���źųɹ�
 */
u8 IIC_Wait_Ack(void)  
{
	u8 timeout=0;
	SDA_IN();
	IIC_SCL=1;
	delay_us(5);
	IIC_SDA=1;
	delay_us(5);
	while(Read_SDA)
	{
		timeout++;
		if(timeout>250)
		{
			IIC_Stop();
			return 1;  //����Ӧ��ʧ��
		}
	}
	IIC_SCL=0;
	return 0;  //����Ӧ��ɹ�
}

/*
 *��������Ӧ���ź�Ack�����������������ݹ�����
 *ack=0������Ӧ���ź�
 *ack=1�����ͷ�Ӧ���ź�
 */
void IIC_Tran_Ack(u8 ack)  //ack=0 or 1
{
	IIC_SCL=0;
	SDA_OUT();  //��������Ϊ���״̬
	IIC_SDA=ack;
	delay_us(5);
	IIC_SCL=1;
	delay_us(5);
	IIC_SCL=0;  //һ��ʱ���������һ���շ�����
}

//����һ���ֽڵ����ݣ�Sdata��������
void IIC_Tran_Byte(u8 Sdata)
{
	u8 i;
	IIC_SCL=0;
	SDA_OUT();  //��������Ϊ���״̬
	for(i=0;i<8;i++)
	{
		IIC_SDA=((Sdata&0x80)>>7);  //�Ӹ�λ��ʼ����
		delay_us(5);
		IIC_SCL=1;
		delay_us(5);
		IIC_SCL=0;  //һ��ʱ���������һ���շ�����
		delay_us(5);
		Sdata<<=1;
	}
}

/*
 *��ȡ�ӻ�һ���ֽڵ����ݣ�����ӻ�������Ӧ��Ӧ����Ӧ���ź��ź�
 *ack=0������Ӧ���ź�
 *ack=1�����ͷ�Ӧ���ź�
 */
u8 IIC_Read_Byte(u8 ack)
{
	u8 i, temp=0;
	SDA_IN();
	for(i=0;i<8;i++)
	{
		temp<<=1;  //�Ӹ�λ��ȡ
		IIC_SCL=0;
		delay_us(5);
		IIC_SCL=1;
		if(Read_SDA) temp++;
		delay_us(5);
	}
	IIC_Tran_Ack(ack);
	return temp;
}
