#include "sys.h"  //This file has beem modified by the Lubuntu at 2017.8.4
#include "led.h"
#include "Key.h"
#include "lcd.h"
#include "usart.h"
#include "delay.h"
#include "MPU6050.h"
#include "Motor.h"
#include "Encoder.h"
#include "PID.h"

u16 t=0;
float pitch, roll, yaw, BalanAngle; 		//ŷ���ǣ�ƽ�����
short gx, gy, gz, BalanG;  //������ԭʼ���ݣ�ƽ����ٶ�
short ax, ay, az;  //���ٶȼ�ԭʼ����
int GesturePWM;  //��̬��PWM
int SpeedPWM;  //�ٶȻ�PWM
int EcA, EcB;  //����������ֵ
int APWM;  //���A PWM
int BPWM;  //���B PWM
float Kp_Gesture=360, Kd_Gesture=1.76;  //��̬��PID������360��1.8��1.5��Σ�2�ᶶ
float Kp_Speed=30, Ki_Speed=0.35;  //�ٶȻ�PID������30,0.3���������ȡ�ø�ֵ����������ˡ���������������
u8 StopShow;  //LCDˢ�¿��Ʊ�־
u8 ShowCNT;  //LCDˢ�¼���
u8 SpeedControl=0;  //�ٶȿ��Ʊ�־
short temp;  //�м����

int main(void)
{
	delay_init();  //��ʱ������ʼ��
	LED_Init();  //����LED���GPIO�ĳ�ʼ��
	Key_Init();  //������ʼ��
	Motor_Init(7199, 0);  //���PWM��Ƶ��f=72000/(7199+1)=10KHz
	Encoder_Init();  //������������ʼ��
	MPU_Init();
	uart_init(9600);  //���ڳ�ʼ��
	LCD_Init();  //LCD��ʼ����LCD��ʼ��Ӧ�����������ʼ����ɺ���У��������ְ��ӵ����λ��LCD�����������޷��������е�����
	
	LCD_Fill(0,0,538,538,WHITE);
	POINT_COLOR=RED;
	while(MPU_DMP_Init())
	{
		LCD_ShowString(10, 150, 200, 16, 16, "CAN NOT INITIAL MPU6050!");
		delay_ms(500);
		LCD_Fill(0,0,538,538,WHITE);
	}
	LCD_Fill(0,0,538,538,WHITE);
	POINT_COLOR=GREEN;
	LCD_ShowString(10, 150, 200, 16, 16, "MPU6050 IS READY!");
	LCD_Fill(0,0,538,538,WHITE);
	POINT_COLOR=BLUE;
	LCD_ShowString(10, 80, 200, 16, 16, "The current data is:");
	LCD_ShowString(10, 120, 200, 16, 16, " Temp:    . C");  //�¶ȡ�ÿ�����֡��ַ�ռһ��8*8�����ؿ飬�����ڼ�����Ļ��ʾ�������������Ҫ
	LCD_ShowString(10, 140, 200, 16, 16, "Pitch:    . ");  //Pitch
	LCD_ShowString(10, 160, 200, 16, 16, " Roll:    . ");  //Roll
	LCD_ShowString(10, 180, 200, 16, 16, "  Yaw:    . ");  //Yaw
	
	MPU_EXTI_Init();  //MPU6050�����жϳ�ʼ��
	
	while(1)
	{
		temp=MPU_Get_Temp();  //��ȡ�¶ȵ�100��
		MPU_DMP_GetData(&pitch, &roll, &yaw);  //��ȡŷ����
		if(temp<0)
		{
			LCD_ShowChar(10+48, 120, '-', 16, 0);		//��ʾ����
			temp=-temp;  //���¶�ֵת��Ϊ��ֵ
		} else LCD_ShowChar(10+48, 120, ' ', 16, 0);	//����ʾ����
		LCD_ShowNum(10+48+8, 120, temp/100, 3, 16);		//��ʾ��������	    
		LCD_ShowNum(10+48+40, 120, ((temp-temp%10)/10)%10, 1, 16);	//��ʾС�����֡�Ư��̫����
		
		temp=pitch*10;  //����õ���ŷ���Ǳ���1λС��
		if(temp<0)
		{
			LCD_ShowChar(10+48, 140, '-', 16, 0);		//��ʾ����
			temp=-temp;		//��pitchת��Ϊ����
		}else LCD_ShowChar(10+48, 140, ' ', 16, 0);		//����ʾ���� 
		LCD_ShowNum(10+48+8,140, temp/10, 3, 16);		  //��ʾ��������	    
		LCD_ShowNum(10+48+40,140, temp%10, 1, 16);		//��ʾС������ 
		
		temp=roll*10;
		if(temp<0)
		{
			LCD_ShowChar(10+48,160, '-', 16, 0);		//��ʾ����
			temp=-temp;		//��rollתΪ����
		}else LCD_ShowChar(10+48,240,' ', 16, 0);		//����ʾ���� 
		LCD_ShowNum(10+48+8,160, temp/10, 3, 16);		//��ʾ��������	    
		LCD_ShowNum(10+48+40,160, temp%10, 1, 16);	//��ʾС������ 
		
		temp=yaw*10;
		if(temp<0)
		{
			LCD_ShowChar(10+48, 180, '-', 16, 0);		//��ʾ����
			temp=-temp;		//��yawתΪ����
		}else LCD_ShowChar(10+48,180, ' ', 16, 0);		//����ʾ���� 
		LCD_ShowNum(10+48+8,180, temp/10, 3, 16);		  //��ʾ��������	    
		LCD_ShowNum(10+48+40,180, temp%10, 1, 16);		//��ʾС������  
		t++;
		
		if(t==2)  //��˸����T=50ms*20=1s
		{
			LED1=!LED1;  //LED��˸����ָʾ��������״̬
			printf("���A�ĵ�ǰ������Ϊ %d \r\n\n",Encoder_Read(2));  //�򴮿ڴ�ӡ���A������
			printf("���B�ĵ�ǰ������Ϊ %d \r\n\n",Encoder_Read(3));  //�򴮿ڴ�ӡ���B������
			t=0;
		}
		StopShow=1;
		ShowCNT=0;
		while(StopShow);		
	}
}
