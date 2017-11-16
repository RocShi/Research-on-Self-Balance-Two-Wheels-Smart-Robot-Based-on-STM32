#include "PID.h"
#include "inv_mpu.h"
#include "MPU6050.h"
#include "Motor.h"
#include "Encoder.h"

//MPU6050 �����жϣ�5ms��ʱ�����������������̬�����ɴ˺������
int EXTI15_10_IRQHandler(void)
{
	if (EXTI_Come)  //MPU6050�жϵ���
	{
		EXTI_ClearITPendingBit(EXTI_Line12);  //����жϱ�־λ
		SpeedControl=!SpeedControl;  //�ٶȿ��Ʊ�־ȡ��
		
		ShowCNT+=1;
		if (ShowCNT==10)  //��MPU6050��5ms�����ж�����Ϊ��׼��ΪLCD�ṩ��50ms��ˢ������
		{
			StopShow=0;  //����ˢ��LCD��
			ShowCNT=0;
		}
		
		if (SpeedControl==1)  //�˴��б�Ĵ��ھ������ٶȵ�������Ϊ5ms*20=100ms
		{
			Update();  //������̬����
//			GesturePWM=Gesture(BalanAngle,BalanG);  //��̬��PWM
//			APWM=GesturePWM-0;  //���A PWM
//			BPWM=GesturePWM-0;  //���B PWM
//			Motor(APWM, BPWM);
			return 0;
		}
		
		EcA=-Encoder_Read(2);
		EcB=Encoder_Read(3);
		Update();  //������̬����
		GesturePWM=Gesture(BalanAngle,BalanG);  //��̬��PWM
		SpeedPWM=Speed(EcA,EcB);  //�ٶȻ�PWM
		APWM=GesturePWM-SpeedPWM;  //���A PWM
		BPWM=GesturePWM-SpeedPWM;  //���B PWM
		Motor(APWM, BPWM);
	}
	return 0;
}

//��̬����PD������
int Gesture(float Angle, float Gyro)
{
	float AngleBias;  //��̬��ƫ��
	int GPWM;  //��̬��PWM���
	AngleBias=Angle-BalanceAngle;  //������̬��ƫ��
	GPWM=Kp_Gesture*AngleBias+Kd_Gesture*Gyro;  //������̬��PWM���
	return GPWM;
}

//�ٶȻ���PI������
int Speed(int EcA, int EcB)
{
	static float EncoderBias=0,	EB_Integral=0;  //�ٶ�ƫ����ƫ�����
	int SPWM;  //�ٶȻ�PWM���
	EncoderBias=EncoderBias*0.8+(EcA+EcB-0)*0.2;  //�ٶ�ƫ���ͨ�˲�
	EB_Integral+=EncoderBias;  //ƫ�����
	if (EB_Integral>10000) EB_Integral=10000;  //�����޷�
	if (EB_Integral<-10000) EB_Integral=-10000;  //�����޷�
	SPWM=Kp_Speed*EncoderBias+Ki_Speed*EB_Integral;  //�����ٶȻ�PWM���
	return SPWM;
}

//ת��
int Turn()
{
	
}

//��̬����
void Update(void)
{
	MPU_DMP_GetData(&pitch, &roll, &yaw);  //��ȡȫ��ŷ��������
	MPU_Get_Gyro(&gx, &gy, &gz);  //��ȡȫ������������
	BalanAngle=roll;  //����ƽ�����
	BalanG=gx;  //����ƽ����ٶ�
}
