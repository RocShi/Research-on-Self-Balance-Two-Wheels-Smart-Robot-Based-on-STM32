#ifndef __DataReport_H  //��������
#define __DataReport_H	 
#include "sys.h"

void TranChar(u8 ch);  //���ַ����ͺ���
void ReToANO(u8 fun, u8 *data, u8 len);  //�ϱ����ݸ�����������λ����V2.6��
void ReSoDa(short ax, short ay, short az, short gx, short gy, short gz);  //�ϱ������Ǻͼ��ٶȼƵ�ԭʼ���ݣ�������̬���ݲ�����ʾ
void ReAllDa(short ax, short ay, short az, short gx, short gy, short gz, short pitch, short roll, short yaw);  //�ϱ�ȫ�����ݣ�����3D��̬��ʾ

#endif
