#ifndef __PID_H  //��������
#define __PID_H	 
#include "sys.h"

#define BalanceAngle -7.5
#define EXTI_Come PAin(12)==0

extern float pitch, roll, yaw, BalanAngle; 		//ŷ���ǣ�ƽ�����
extern short gx, gy, gz, BalanG;  //������ԭʼ���ݣ�ƽ����ٶ�
extern short ax, ay, az;  //���ٶȼ�ԭʼ����
extern int GesturePWM;  //��̬��PWM
extern int SpeedPWM;  //�ٶȻ�PWM
extern int EcA, EcB;  //����������ֵ
extern int APWM;  //���A PWM
extern int BPWM;  //���B PWM
extern float Kp_Gesture, Kd_Gesture;  //��̬��PID����
extern float Kp_Speed, Ki_Speed;  //�ٶȻ�PID����
extern u8 StopShow;  //LCDˢ�¿��Ʊ�־
extern u8 ShowCNT;  //LCDˢ�¼���
extern u8 SpeedControl;  //�ٶȿ��Ʊ�־

int EXTI15_10_IRQHandler(void);  //MPU6050 5ms��ʱ�жϷ��������������̬�����ɴ˺������
int Gesture(float Angle, float Gyro);  //��̬��
int Speed(int EcA, int EcB);  //�ٶȻ�
int Turn(void);  //ת��
void Update(void);  //��̬����

#endif
