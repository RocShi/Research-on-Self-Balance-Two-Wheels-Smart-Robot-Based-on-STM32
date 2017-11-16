#include "MPU6050.h"
#include "IIC.h"
#include "delay.h"

//MPU6050初始化函数
u8 MPU_Init(void)
{
	u8 ID;  //器件ID
	IIC_Init();
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0x80);  //复位MPU6050
	delay_ms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0x00);  //唤醒MPU6050
	MPU_Set_GyroFSR(3);  //设置陀螺仪满量程范围为±2000°/s
	MPU_Set_AccFSR(0);  //设置加速度计满量程范围为±2g
	MPU_Set_SamRate(50);  //设置采样频率50Hz
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	 //I2C主模式关闭
	MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);	 //关闭FIFO
  MPU_Write_Byte(MPU_INTBP_CFG_REG,0x9c);  //1001 1100  选择中断电平，逻辑电平为0  50us高   查询立即自动清除   
  MPU_Write_Byte(MPU_INT_EN_REG,0x01);     //使能数据就绪中断
	ID=MPU_Read_Byte(MPU_DEVICE_ID_REG);  //读取器件ID寄存器中的值
	if(ID==0x68)
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG, 1);	 //设置系统时钟源：PLL，以X轴陀螺为参考
		MPU_Write_Byte(MPU_PWR_MGMT2_REG, 0);	 //开启陀螺仪和加速度计
	  MPU_Set_SamRate(50);  //设置采样频率50Hz
	} else return 1;
  return 0;	
}

//向MPU6050指定寄存器reg写入数据data
u8 MPU_Write_Byte(u8 reg, u8 data)
{
	IIC_Start();
	IIC_Tran_Byte((0x68<<1)|0);  //发送MPU6050器件地址+写命令
	IIC_Wait_Ack();
	IIC_Tran_Byte(reg);  //发送寄存器地址reg
	IIC_Wait_Ack();
	IIC_Tran_Byte(data);  //写入数据data
	IIC_Wait_Ack();
	IIC_Stop();
	return 0;
}

//从MPU6050指定寄存器reg读取数据
u8 MPU_Read_Byte(u8 reg)
{
	u8 temp;
	IIC_Start();
	IIC_Tran_Byte((0x68<<1)|0);  //发送MPU6050器件地址+写命令
	IIC_Wait_Ack();
	IIC_Tran_Byte(reg);  //发送寄存器地址reg
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Tran_Byte((0x68<<1)|1);  //发送MPU6050器件地址+读命令
	IIC_Wait_Ack();
	temp=IIC_Read_Byte(1);  //读取数据，发送nAck
	IIC_Stop();
	return temp;
}

//从MPU6050指定寄存器reg开始写入长度为len的数据数组buf
u8 MPU_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
	u8 i; 
  IIC_Start(); 
	IIC_Tran_Byte((0x68<<1)|0);  //发送器件地址+写命令	
  IIC_Wait_Ack();	//等待应答
  IIC_Tran_Byte(reg);	//写寄存器地址
	IIC_Wait_Ack();		//等待应答
	for(i=0;i<len;i++)
	{
		IIC_Tran_Byte(buf[i]);	//发送数据
		IIC_Wait_Ack();		//等待ACK	
	}
	IIC_Stop();	 
	return 0;	
}

//从MPU6050指定寄存器reg开始读取长度为len的数据，并存入buf数组中
u8 MPU_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
	u8 i=0;
  IIC_Start();
	IIC_Tran_Byte((0x68<<1)|0);  //发送MPU6050器件地址+写命令
	IIC_Wait_Ack();
	IIC_Tran_Byte(reg);  //发送寄存器地址reg
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Tran_Byte((0x68<<1)|1);  //发送MPU6050器件地址+读命令
	IIC_Wait_Ack();
	for(i=0;i<len;i++)
	{
		if(i==len-1) *buf=IIC_Read_Byte(1);  //读完最后一个字节后，告知MPU停止读取
		else *buf=IIC_Read_Byte(0);  //未读到最后一个字节，继续读取
		buf++;
	}
	IIC_Stop();
	return 0;
}

/*
 *设置陀螺仪满量程范围，fsr取值0-3
 *fsr=0:量程±250°/s
 *fsr=1:量程±500°/s
 *fsr=2:量程±1000°/s
 *fsr=3:量程±2000°/s
 */
u8 MPU_Set_GyroFSR(u8 fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG, fsr<<3);  //陀螺仪配置寄存器的bit4:bit3为量程控制位
}

/*
 *设置加速度计满量程范围，fsr取值0-3
 *fsr=0:量程±2g
 *fsr=1:量程±4g
 *fsr=2:量程±8g
 *fsr=3:量程±16g
 */
u8 MPU_Set_AccFSR(u8 fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG, fsr<<3);  //加速度计配置寄存器的bit4:bit3为量程控制位
}

/*
 *设置数字低通滤波器带宽
 *此处设计无法达到最高带宽
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
 *设置陀螺仪采样频率，此处默认陀螺仪输出频率1KHz
 *陀螺仪采样频率分频寄存器为8位，取值范围0-255
 *故采样频率最小为1000/256=4Hz，最大为1000/1=1KHz
 */
u8 MPU_Set_SamRate(u16 rate)
{
	u8 SMPLRT_DIV;  //陀螺仪采样频率分频寄存器分频值
	if(rate>1000) rate=1000;
	if(rate<4) rate=4;
	SMPLRT_DIV=1000/rate-1;
	MPU_Write_Byte(MPU_SAMPLE_RATE_REG, SMPLRT_DIV);
	return MPU_Set_DLPF(rate/2);  //自动将数字低通滤波器带宽设置为采样频率一半
}

//获取温度值
short MPU_Get_Temp(void)
{
	u8 buf[2];
	short raw;
	float temp;
	MPU_Read_Len(0x68, MPU_TEMP_OUTH_REG, 2, buf);
	raw=(((u16)buf[0])<<8)|buf[1];
	temp=36.53+((double)raw)/340;  //计算得到的温度值保留两位小数
	return temp*100;  //返回温度值的100倍
}

//获取陀螺仪值
u8 MPU_Get_Gyro(short *gx, short *gy, short *gz)
{
	u8 buf[6], ret;
	ret=MPU_Read_Len(0x68, MPU_GYRO_XOUTH_REG, 6, buf);  //从x轴陀螺仪高8位开始读起
	if(ret==0)  //读取成功
	{
		*gx=(((u16)buf[0])<<8)|buf[1];  //x轴陀螺
		*gy=(((u16)buf[2])<<8)|buf[3];  //y轴陀螺
		*gz=(((u16)buf[4])<<8)|buf[5];  //z轴陀螺
	}
	return ret;
}

//获取加速度值
u8 MPU_Get_Acc(short *ax, short *ay, short *az)
{
	u8 buf[6], ret;
	ret=MPU_Read_Len(0x68, MPU_ACCEL_XOUTH_REG, 6, buf);  //从x轴加速度计高8位开始读起
	if(ret==0)  //读取成功
	{
		*ax=(((u16)buf[0])<<8)|buf[1];  //x轴陀螺
		*ay=(((u16)buf[2])<<8)|buf[3];  //y轴陀螺
		*az=(((u16)buf[4])<<8)|buf[5];  //z轴陀螺
	}
	return ret;
}

//MPU6050 5ms定时中断初始化函数
void MPU_EXTI_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //使能复用时钟
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //关闭JTAG
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;  //上拉输入
	GPIO_Init(GPIOA,&GPIO_InitStructure);  //初始化PA12
		
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12); //挂载PA12
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //抢占优先级两位，响应优先级两位
	
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	//PA12 NVIC配置	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           
	NVIC_Init(&NVIC_InitStructure);  	
}
