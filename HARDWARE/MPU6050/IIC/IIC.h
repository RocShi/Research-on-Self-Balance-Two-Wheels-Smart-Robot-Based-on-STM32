#ifndef __IIC_H  //��������
#define __IIC_H	 
#include "sys.h"

#define IIC_SCL PCout(12)  //IICʱ��
#define IIC_SDA PCout(11)  //IIC����
#define Read_SDA PCin(11)  //��ȡ������״̬

//���������߷���
#define SDA_IN()  {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}  //�����仹��û����̫����
#define SDA_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}

void IIC_Init(void);  //IIC��ʼ��
void IIC_Start(void);  //IICͨ�ſ�ʼ
void IIC_Stop(void);  //IICͨ��ֹͣ
u8 IIC_Wait_Ack(void);  //�����ȴ�Ӧ���ź�Ack���������������������ݹ�����
void IIC_Tran_Ack(u8 ack);  //��������Ӧ���ź�Ack�����������������ݹ����� 
void IIC_Tran_Byte(u8 Sdata);  //����һ���ֽڵ����ݣ�Sdata��������
u8 IIC_Read_Byte(u8 ack);  //��ȡ�ӻ�һ���ֽڵ����ݣ�����ӻ�������Ӧ��Ӧ���ź�

#endif
