#include "IIC.h"
#include "delay.h"

//IIC初始化
void IIC_Init(void)  
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //开启GPIOC时钟，IIC占用了两个IO口：时钟线与数据线
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	IIC_SCL=0;
	IIC_SDA=0;
}

//IIC通信开始
void IIC_Start(void)  
{
	SDA_OUT();  //数据线置为输出状态
  IIC_SCL=1;
	IIC_SDA=1;
	delay_us(5);
	IIC_SDA=0;  //IIC的时钟线处于高电平时，数据线的电平若出现下降沿，则会开启IIC通信
	delay_us(5);
	IIC_SCL=0;  //钳住IIC时钟线电平，可以开始传输数据了
}

//IIC通信停止
void IIC_Stop(void)
{
	SDA_OUT();  //数据线置为输出状态
  IIC_SCL=0;
	IIC_SDA=0;
	delay_us(5);
	IIC_SCL=1;
	IIC_SDA=1;  //IIC的时钟线处于高电平时，数据线的电平若出现上升沿，则会关闭
	delay_us(5);
}

/*
 *主机等待应答信号Ack到来
 *返回1，接收应答信号失败
 *返回0，接收应答信号成功
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
			return 1;  //接收应答失败
		}
	}
	IIC_SCL=0;
	return 0;  //接收应答成功
}

/*
 *主机发送应答信号Ack，用在主机接收数据过程中
 *ack=0，发送应答信号
 *ack=1，发送非应答信号
 */
void IIC_Tran_Ack(u8 ack)  //ack=0 or 1
{
	IIC_SCL=0;
	SDA_OUT();  //数据线置为输出状态
	IIC_SDA=ack;
	delay_us(5);
	IIC_SCL=1;
	delay_us(5);
	IIC_SCL=0;  //一个时钟周期完成一次收发操作
}

//发送一个字节的数据，Sdata：短数据
void IIC_Tran_Byte(u8 Sdata)
{
	u8 i;
	IIC_SCL=0;
	SDA_OUT();  //数据线置为输出状态
	for(i=0;i<8;i++)
	{
		IIC_SDA=((Sdata&0x80)>>7);  //从高位开始发送
		delay_us(5);
		IIC_SCL=1;
		delay_us(5);
		IIC_SCL=0;  //一个时钟周期完成一次收发操作
		delay_us(5);
		Sdata<<=1;
	}
}

/*
 *读取从机一个字节的数据，并向从机发送相应的应答或非应答信号信号
 *ack=0，发送应答信号
 *ack=1，发送非应答信号
 */
u8 IIC_Read_Byte(u8 ack)
{
	u8 i, temp=0;
	SDA_IN();
	for(i=0;i<8;i++)
	{
		temp<<=1;  //从高位读取
		IIC_SCL=0;
		delay_us(5);
		IIC_SCL=1;
		if(Read_SDA) temp++;
		delay_us(5);
	}
	IIC_Tran_Ack(ack);
	return temp;
}
