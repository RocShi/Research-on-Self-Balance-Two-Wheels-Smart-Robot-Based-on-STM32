#include "MPU6050.h"
#include "IIC.h"
#include "delay.h"

//MPU6050��ʼ������
u8 MPU_Init(void)
{
	u8 ID;  //����ID
	IIC_Init();
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0x80);  //��λMPU6050
	delay_ms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0x00);  //����MPU6050
	MPU_Set_GyroFSR(3);  //���������������̷�ΧΪ��2000��/s
	MPU_Set_AccFSR(0);  //���ü��ٶȼ������̷�ΧΪ��2g
	MPU_Set_SamRate(50);  //���ò���Ƶ��50Hz
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	 //I2C��ģʽ�ر�
	MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);	 //�ر�FIFO
  MPU_Write_Byte(MPU_INTBP_CFG_REG,0x9c);  //1001 1100  ѡ���жϵ�ƽ���߼���ƽΪ0  50us��   ��ѯ�����Զ����   
  MPU_Write_Byte(MPU_INT_EN_REG,0x01);     //ʹ�����ݾ����ж�
	ID=MPU_Read_Byte(MPU_DEVICE_ID_REG);  //��ȡ����ID�Ĵ����е�ֵ
	if(ID==0x68)
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG, 1);	 //����ϵͳʱ��Դ��PLL����X������Ϊ�ο�
		MPU_Write_Byte(MPU_PWR_MGMT2_REG, 0);	 //���������Ǻͼ��ٶȼ�
	  MPU_Set_SamRate(50);  //���ò���Ƶ��50Hz
	} else return 1;
  return 0;	
}

//��MPU6050ָ���Ĵ���regд������data
u8 MPU_Write_Byte(u8 reg, u8 data)
{
	IIC_Start();
	IIC_Tran_Byte((0x68<<1)|0);  //����MPU6050������ַ+д����
	IIC_Wait_Ack();
	IIC_Tran_Byte(reg);  //���ͼĴ�����ַreg
	IIC_Wait_Ack();
	IIC_Tran_Byte(data);  //д������data
	IIC_Wait_Ack();
	IIC_Stop();
	return 0;
}

//��MPU6050ָ���Ĵ���reg��ȡ����
u8 MPU_Read_Byte(u8 reg)
{
	u8 temp;
	IIC_Start();
	IIC_Tran_Byte((0x68<<1)|0);  //����MPU6050������ַ+д����
	IIC_Wait_Ack();
	IIC_Tran_Byte(reg);  //���ͼĴ�����ַreg
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Tran_Byte((0x68<<1)|1);  //����MPU6050������ַ+������
	IIC_Wait_Ack();
	temp=IIC_Read_Byte(1);  //��ȡ���ݣ�����nAck
	IIC_Stop();
	return temp;
}

//��MPU6050ָ���Ĵ���reg��ʼд�볤��Ϊlen����������buf
u8 MPU_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
	u8 i; 
  IIC_Start(); 
	IIC_Tran_Byte((0x68<<1)|0);  //����������ַ+д����	
  IIC_Wait_Ack();	//�ȴ�Ӧ��
  IIC_Tran_Byte(reg);	//д�Ĵ�����ַ
	IIC_Wait_Ack();		//�ȴ�Ӧ��
	for(i=0;i<len;i++)
	{
		IIC_Tran_Byte(buf[i]);	//��������
		IIC_Wait_Ack();		//�ȴ�ACK	
	}
	IIC_Stop();	 
	return 0;	
}

//��MPU6050ָ���Ĵ���reg��ʼ��ȡ����Ϊlen�����ݣ�������buf������
u8 MPU_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
	u8 i=0;
  IIC_Start();
	IIC_Tran_Byte((0x68<<1)|0);  //����MPU6050������ַ+д����
	IIC_Wait_Ack();
	IIC_Tran_Byte(reg);  //���ͼĴ�����ַreg
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Tran_Byte((0x68<<1)|1);  //����MPU6050������ַ+������
	IIC_Wait_Ack();
	for(i=0;i<len;i++)
	{
		if(i==len-1) *buf=IIC_Read_Byte(1);  //�������һ���ֽں󣬸�֪MPUֹͣ��ȡ
		else *buf=IIC_Read_Byte(0);  //δ�������һ���ֽڣ�������ȡ
		buf++;
	}
	IIC_Stop();
	return 0;
}

/*
 *���������������̷�Χ��fsrȡֵ0-3
 *fsr=0:���̡�250��/s
 *fsr=1:���̡�500��/s
 *fsr=2:���̡�1000��/s
 *fsr=3:���̡�2000��/s
 */
u8 MPU_Set_GyroFSR(u8 fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG, fsr<<3);  //���������üĴ�����bit4:bit3Ϊ���̿���λ
}

/*
 *���ü��ٶȼ������̷�Χ��fsrȡֵ0-3
 *fsr=0:���̡�2g
 *fsr=1:���̡�4g
 *fsr=2:���̡�8g
 *fsr=3:���̡�16g
 */
u8 MPU_Set_AccFSR(u8 fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG, fsr<<3);  //���ٶȼ����üĴ�����bit4:bit3Ϊ���̿���λ
}

/*
 *�������ֵ�ͨ�˲�������
 *�˴�����޷��ﵽ��ߴ���
 */
u8 MPU_Set_DLPF(u16 bandwidth)
{
	u8 DLPF_CFG;
	if(bandwidth>=188) DLPF_CFG=1;
	if((bandwidth>=98)&&(bandwidth<188)) DLPF_CFG=2;
	if((bandwidth>=42)&&(bandwidth<98)) DLPF_CFG=3;
	if((bandwidth>=20)&&(bandwidth<42)) DLPF_CFG=4;
	if((bandwidth>=10)&&(bandwidth<20)) DLPF_CFG=5;
	if(bandwidth<10) DLPF_CFG=6;
	return MPU_Write_Byte(MPU_CFG_REG, DLPF_CFG);
}

/*
 *���������ǲ���Ƶ�ʣ��˴�Ĭ�����������Ƶ��1KHz
 *�����ǲ���Ƶ�ʷ�Ƶ�Ĵ���Ϊ8λ��ȡֵ��Χ0-255
 *�ʲ���Ƶ����СΪ1000/256=4Hz�����Ϊ1000/1=1KHz
 */
u8 MPU_Set_SamRate(u16 rate)
{
	u8 SMPLRT_DIV;  //�����ǲ���Ƶ�ʷ�Ƶ�Ĵ�����Ƶֵ
	if(rate>1000) rate=1000;
	if(rate<4) rate=4;
	SMPLRT_DIV=1000/rate-1;
	MPU_Write_Byte(MPU_SAMPLE_RATE_REG, SMPLRT_DIV);
	return MPU_Set_DLPF(rate/2);  //�Զ������ֵ�ͨ�˲�����������Ϊ����Ƶ��һ��
}

//��ȡ�¶�ֵ
short MPU_Get_Temp(void)
{
	u8 buf[2];
	short raw;
	float temp;
	MPU_Read_Len(0x68, MPU_TEMP_OUTH_REG, 2, buf);
	raw=(((u16)buf[0])<<8)|buf[1];
	temp=36.53+((double)raw)/340;  //����õ����¶�ֵ������λС��
	return temp*100;  //�����¶�ֵ��100��
}

//��ȡ������ֵ
u8 MPU_Get_Gyro(short *gx, short *gy, short *gz)
{
	u8 buf[6], ret;
	ret=MPU_Read_Len(0x68, MPU_GYRO_XOUTH_REG, 6, buf);  //��x�������Ǹ�8λ��ʼ����
	if(ret==0)  //��ȡ�ɹ�
	{
		*gx=(((u16)buf[0])<<8)|buf[1];  //x������
		*gy=(((u16)buf[2])<<8)|buf[3];  //y������
		*gz=(((u16)buf[4])<<8)|buf[5];  //z������
	}
	return ret;
}

//��ȡ���ٶ�ֵ
u8 MPU_Get_Acc(short *ax, short *ay, short *az)
{
	u8 buf[6], ret;
	ret=MPU_Read_Len(0x68, MPU_ACCEL_XOUTH_REG, 6, buf);  //��x����ٶȼƸ�8λ��ʼ����
	if(ret==0)  //��ȡ�ɹ�
	{
		*ax=(((u16)buf[0])<<8)|buf[1];  //x������
		*ay=(((u16)buf[2])<<8)|buf[3];  //y������
		*az=(((u16)buf[4])<<8)|buf[5];  //z������
	}
	return ret;
}

//MPU6050 5ms��ʱ�жϳ�ʼ������
void MPU_EXTI_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //ʹ�ܸ���ʱ��
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //�ر�JTAG
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;  //��������
	GPIO_Init(GPIOA,&GPIO_InitStructure);  //��ʼ��PA12
		
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12); //����PA12
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //��ռ���ȼ���λ����Ӧ���ȼ���λ
	
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	//PA12 NVIC����	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           
	NVIC_Init(&NVIC_InitStructure);  	
}
