#include "DataReport.h"

//���ַ����ͺ���
void TranChar(u8 ch)
{
	while((USART1->SR&0x40)==0);  //�ȴ���һ�η������
	USART1->DR=ch;  //�����ַ�ch
}

/* 
 * �ϱ����ݸ�����������λ����V2.6��
 * fun�����ƹ����֣�ȡֵ0xA0-0xAF��������������λ����ͨ��Э���й�
 * data�����ݻ����������28Byte
 * len��data������
 */
void ReToANO(u8 fun, u8 *data, u8 len)
{
	u8 buf[32], i;
	if(len>28) return;
	buf[0]=0x88;  //֡ͷ����֪��ʲô���
	buf[1]=fun;  //������
	buf[2]=len;  //data������
	buf[len+3]=0;  //У��������
	for(i=0;i<len;i++) buf[3+i]=data[i];  //��������
	for(i=0;i<len+3;i++) buf[len+3]+=buf[i];  //����У��͡������̫��
	for(i=0;i<len+4;i++) TranChar(buf[i]);  //�������ݡ�ÿ��Ҫ��ȫ�����ƻ��Ǻ�ͨ��Э���й�
} 

//�ϱ������Ǻͼ��ٶȼƵ�ԭʼ���ݣ�������̬���ݲ�����ʾ
void ReSoDa(short ax, short ay, short az, short gx, short gy, short gz)
{
	u8 buf[12];
	buf[0]=(ax>>8)&0xFF;   //X����ٶȸ�8λ
	buf[1]=ax&0xFF;        //X����ٶȵ�8λ
	buf[2]=(ay>>8)&0xFF;   //Y����ٶȸ�8λ
	buf[3]=ay&0xFF;        //Y����ٶȵ�8λ
	buf[4]=(az>>8)&0xFF;   //Z����ٶȸ�8λ
	buf[5]=az&0xFF;        //Z����ٶȵ�8λ
	buf[6]=(gx>>8)&0xFF;   //X�������Ǹ�8λ
	buf[7]=gx&0xFF;        //X�������ǵ�8λ
	buf[8]=(gy>>8)&0xFF;   //Y�������Ǹ�8λ
	buf[9]=gy&0xFF;        //Y�������ǵ�8λ
	buf[10]=(gz>>8)&0xFF;  //Z�������Ǹ�8λ
	buf[11]=gz&0xFF;       //Z�������ǵ�8λ
	
	ReToANO(0xA1, buf, 12);  //�Զ���֡0xA1����̫���
} 

/*
 * �ϱ�ȫ�����ݣ�����3D��̬��ʾ
 * ���������Ϊ����ô�洢
 */
void ReAllDa(short ax, short ay, short az, short gx, short gy, short gz, short pitch, short roll, short yaw)
{
	u8 buf[28];
	buf[0]=(ax>>8)&0xFF;   //X����ٶȸ�8λ
	buf[1]=ax&0xFF;        //X����ٶȵ�8λ
	buf[2]=(ay>>8)&0xFF;   //Y����ٶȸ�8λ
	buf[3]=ay&0xFF;        //Y����ٶȵ�8λ
	buf[4]=(az>>8)&0xFF;   //Z����ٶȸ�8λ
	buf[5]=az&0xFF;        //Z����ٶȵ�8λ
	buf[6]=(gx>>8)&0xFF;   //X�������Ǹ�8λ
	buf[7]=gx&0xFF;        //X�������ǵ�8λ
	buf[8]=(gy>>8)&0xFF;   //Y�������Ǹ�8λ
	buf[9]=gy&0xFF;        //Y�������ǵ�8λ
	buf[10]=(gz>>8)&0xFF;  //Z�������Ǹ�8λ
	buf[11]=gz&0xFF;       //Z�������ǵ�8λ
	
	buf[18]=(roll>>8)&0xFF;    //����Ǹ�8λ
	buf[19]=roll&0xFF;         //����ǵ�8λ
	buf[20]=(pitch>>8)&0xFF;   //�����Ǹ�8λ
	buf[21]=pitch&0xFF;        //�����ǵ�8λ
	buf[22]=(yaw>>8)&0xFF;     //����Ǹ�8λ
	buf[23]=yaw&0xFF;          //����ǵ�8λ
	
	ReToANO(0xAF, buf, 12);  //�ɿ���ʾ֡0xAF����̫���
}
