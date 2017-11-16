#ifndef __IIC_H  //条件编译
#define __IIC_H	 
#include "sys.h"

#define IIC_SCL PCout(12)  //IIC时钟
#define IIC_SDA PCout(11)  //IIC数据
#define Read_SDA PCin(11)  //读取数据线状态

//设置数据线方向
#define SDA_IN()  {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}  //这两句还是没有整太明白
#define SDA_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}

void IIC_Init(void);  //IIC初始化
void IIC_Start(void);  //IIC通信开始
void IIC_Stop(void);  //IIC通信停止
u8 IIC_Wait_Ack(void);  //主机等待应答信号Ack到来，用在主机发送数据过程中
void IIC_Tran_Ack(u8 ack);  //主机发送应答信号Ack，用在主机接收数据过程中 
void IIC_Tran_Byte(u8 Sdata);  //发送一个字节的数据，Sdata：短数据
u8 IIC_Read_Byte(u8 ack);  //读取从机一个字节的数据，并向从机发送相应的应答信号

#endif
