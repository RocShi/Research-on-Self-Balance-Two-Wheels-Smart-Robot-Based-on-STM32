#include "DataReport.h"

//单字符发送函数
void TranChar(u8 ch)
{
	while((USART1->SR&0x40)==0);  //等待上一次发送完成
	USART1->DR=ch;  //发送字符ch
}

/* 
 * 上报数据给匿名四轴上位机（V2.6）
 * fun：控制功能字，取值0xA0-0xAF。估计与匿名上位机的通信协议有关
 * data：数据缓冲区，最大28Byte
 * len：data区长度
 */
void ReToANO(u8 fun, u8 *data, u8 len)
{
	u8 buf[32], i;
	if(len>28) return;
	buf[0]=0x88;  //帧头。不知道什么情况
	buf[1]=fun;  //功能字
	buf[2]=len;  //data区长度
	buf[len+3]=0;  //校验字清零
	for(i=0;i<len;i++) buf[3+i]=data[i];  //复制数据
	for(i=0;i<len+3;i++) buf[len+3]+=buf[i];  //计算校验和。这个不太懂
	for(i=0;i<len+4;i++) TranChar(buf[i]);  //发送数据。每次要发全，估计还是和通信协议有关
} 

//上报陀螺仪和加速度计的原始数据，用于姿态数据波形显示
void ReSoDa(short ax, short ay, short az, short gx, short gy, short gz)
{
	u8 buf[12];
	buf[0]=(ax>>8)&0xFF;   //X轴加速度高8位
	buf[1]=ax&0xFF;        //X轴加速度低8位
	buf[2]=(ay>>8)&0xFF;   //Y轴加速度高8位
	buf[3]=ay&0xFF;        //Y轴加速度低8位
	buf[4]=(az>>8)&0xFF;   //Z轴加速度高8位
	buf[5]=az&0xFF;        //Z轴加速度低8位
	buf[6]=(gx>>8)&0xFF;   //X轴陀螺仪高8位
	buf[7]=gx&0xFF;        //X轴陀螺仪低8位
	buf[8]=(gy>>8)&0xFF;   //Y轴陀螺仪高8位
	buf[9]=gy&0xFF;        //Y轴陀螺仪低8位
	buf[10]=(gz>>8)&0xFF;  //Z轴陀螺仪高8位
	buf[11]=gz&0xFF;       //Z轴陀螺仪低8位
	
	ReToANO(0xA1, buf, 12);  //自定义帧0xA1，不太理解
} 

/*
 * 上报全部数据，用于3D姿态显示
 * 不理解数据为何这么存储
 */
void ReAllDa(short ax, short ay, short az, short gx, short gy, short gz, short pitch, short roll, short yaw)
{
	u8 buf[28];
	buf[0]=(ax>>8)&0xFF;   //X轴加速度高8位
	buf[1]=ax&0xFF;        //X轴加速度低8位
	buf[2]=(ay>>8)&0xFF;   //Y轴加速度高8位
	buf[3]=ay&0xFF;        //Y轴加速度低8位
	buf[4]=(az>>8)&0xFF;   //Z轴加速度高8位
	buf[5]=az&0xFF;        //Z轴加速度低8位
	buf[6]=(gx>>8)&0xFF;   //X轴陀螺仪高8位
	buf[7]=gx&0xFF;        //X轴陀螺仪低8位
	buf[8]=(gy>>8)&0xFF;   //Y轴陀螺仪高8位
	buf[9]=gy&0xFF;        //Y轴陀螺仪低8位
	buf[10]=(gz>>8)&0xFF;  //Z轴陀螺仪高8位
	buf[11]=gz&0xFF;       //Z轴陀螺仪低8位
	
	buf[18]=(roll>>8)&0xFF;    //横滚角高8位
	buf[19]=roll&0xFF;         //横滚角低8位
	buf[20]=(pitch>>8)&0xFF;   //俯仰角高8位
	buf[21]=pitch&0xFF;        //俯仰角低8位
	buf[22]=(yaw>>8)&0xFF;     //航向角高8位
	buf[23]=yaw&0xFF;          //航向角低8位
	
	ReToANO(0xAF, buf, 12);  //飞控显示帧0xAF，不太理解
}
