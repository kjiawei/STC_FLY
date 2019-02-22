/*------------------------------------------------------------
�˲�Ʒ�ǻ���STC�꾧��˾����2015�����¿�����һ��STC15W4K��IAP15W4K��ϵ�е�
8λ��Ƭ������ϵ����STC12ϵ�еĻ����� �����˿��ѹ�Ͷ�·Ӳ��PWM���ܣ�15λ����
������������������еĻ���������󣬾������˶෽��ѧϰ���о��Ĺ����У�
��������������ķ���ԭ�����������Ҫ����Ʋ����ȡ�

�����ǡ��������ᡱ�ĳ������˵����
��Ƭ������STC15W4K48S4-��ǿ�͵�Ƭ����IPA���޸�EEPROM���룩
������ѹΪ��5V	����Ƶ��Ϊ��24M	����ͨѶģ��Ϊ��LT8910(�����շ�)
�������� ������MCU6050����̬���ݲɼ���PID�㷨��Ӳ��4·PWM����MOS���������ı����
ע�⣺�˳���ֻ���ݣ����ı�716�����45MMר�����������Լ��ĵ�������޸�PID������
�ں��ڵĳ�������У�������Ӳ������ﵽԤ�ڷ��й��ܡ������ע��

��Դ����-������ᰮ���߶����Խ����޸��뼼��������
������ڴ˻������ܵ������õķ���Ч���������������ϵ���з��ʵ�ｱ��Ŷ��
ԭ������ߣ���ϵ��ʽQQ��250278069     STC�������Ἴ������Ⱥ��421109612
�Ա���վ��http://shop117773209.taobao.com/shop/view_shop.htm?spm=a1z0e.1.0.0.eqIhKz&mytmenu=mdianpu&utkn=g,meztsobuhe3to1427461542456&user_number_id=669230514&scm=1028.1.1.20001
�������ʱ�䣺2016��8��8��-V2.0��
------------------------------------------------------------*/

#include <STC15W4K48S4.h> //STC15W4K48S4 ר��ͷ�ļ�
#include <intrins.h>			//STC���������������
#include <MPU6050.H>			//MPU6050����������
#include <STC15W4K-PWM.H> //��Ƭ������IO�ڳ�ʼ��-PWM�ڳ�ʼ��
#include <LT8910.h>				//����2.4Gģ��
//#include <STC15W4K-ADC.h>	//STC15W4K-ADC	Ӳ��ADCģ��ת��
#include <IMU.H>					//IMU(���Բ�����Ԫ)���к����㷨
#include <KalmanFilter.h> //�������˲��㷨

//==================================================//
//  LED�� ���Ŷ���
//==================================================//
sbit LEDR = P0 ^ 1; //LED ��ɫ R 0��1��
sbit LEDG = P1 ^ 7; //LED ��ɫ G
sbit LEDB = P1 ^ 6; //LED ��ɫ B

//sbit	KARX=P3^0; //����ӿ� RXD
//sbit	KATX=P3^1; //����ӿ� TXD
//==================================================//
//  ���п��Ʊ���
//==================================================//
unsigned char JieSuo;		//�Ͽ�/���� ��������
unsigned char SSLL;			//ͨѶ״̬ ����
unsigned char ShiLian;	//ʧ������
unsigned char SLJS;			//ʧ����������
unsigned int YouMen;		//���ű���
unsigned int HangXiang; //�������
unsigned int HengGun;		//�������
unsigned int FuYang;		//��������
unsigned char FYHG;			//�����������
unsigned char Xiao1;		//CRC ���Ч���
unsigned char Xiao2;		//CRC ����Ч���

/*------------------------------------------------
                  ȫ�ֺ�������
------------------------------------------------*/
unsigned char data RxBuf[20]; //���ý��ճ��ȣ����Ϊ32�ֽ�
//*****************�ɿؿ��Ʋ���**************************
unsigned int YM = 0;																//���ű仯�ٶȿ��ƣ����������Ļ����ٱ仯����ʱ�����ʧ�ٷ�ת��GG
int speed0 = 0, speed1 = 0, speed2 = 0, speed3 = 0; //����ٶȲ���
int PWM0 = 0, PWM1 = 0, PWM2 = 0, PWM3 = 0;					//������PWMģ��Ĳ���
//double g_x=0,g_y=0,g_z=0;       //�����ǽ�������
char a_x = 0, a_y = 0, a_z = 0; //�ǶȽ�������
float IMU_gz;
//*****************MPU-6050 �Ĵ������ݲ���**************************
double Gyro_y = 0, Gyro_x = 0, Gyro_z = 0;					//Y�������������ݴ�
int xdata Angle_ax = 0, Angle_ay = 0, Angle_az = 0; //�ɼ��ٶȼ���ļ��ٶ�(������)
int idata Angle_gy = 0, Angle_gx = 0, Angle_gz = 0; //�ɽ��ٶȼ���Ľ�����(�Ƕ���)
int data AngleX = 0, AngleY = 0;										//��Ԫ���������ŷ����(�ɻ���б��,10��)  ,AngleZ=0
unsigned char data tp[16];
//==================================================//
//   PID�㷨����
//==================================================//
float data PID_Output;				 //PID���������
float xdata Last_Angle_gx = 0; //�⻷PI�����  ��һ������������
float xdata Last_Angle_gy = 0;
float xdata ERRORX_In = 0;	//�ڻ�P �ڻ�I �ڻ�D �ڻ�������
float xdata ERRORX_Out = 0; //�⻷P �⻷I       �⻷������
float xdata ERRORY_In = 0;
float xdata ERRORY_Out = 0;
float xdata ERRORZ_Out = 0;
float xdata Anglezlate = 0; //Z�����
float xdata Last_Ax = 0;
float xdata Last_Ay = 0;
float xdata Last_gx = 0;
float xdata Last_gy = 0;

/////////////////////////////////////////////////////////////
float xdata Ax = 0, Ay = 0;
Az = 0;										//����ң������������ĽǶ�
int xdata gx = 0, gy = 0; //����ң������������Ľ�?
long xdata g_x_aver = 0;
long xdata g_y_aver = 0;
long xdata g_z_aver = 0;
int delta_rc_x = 0;
int delta_rc_y = 0;
int delta_rc_z = 0;
long idata PID_P;
long idata PID_I;
long idata PID_D;

#define Q15(X) \
	((X < 0.0) ? (int)(32768 * (X)-0.5) : (int)(32767 * (X) + 0.5))
#define KALMAN_Q Q15(0.20)
#define KALMAN_R Q15(0.80)
//==================================================//
//   PID �ֶ�΢������ֵ
//   �޸�PID���²��� ���԰ɷɿص��ĸ���ƽ��
//   ������Է����Ųο�ѧϰ����
//==================================================//
#define Out_XP 12.0f		 //�⻷P
#define Out_XI Q15(0.12) //�⻷I
#define Out_XD 5.2f			 //�⻷D
#define In_XP Q15(0.31)	//�ڻ�P
#define In_XI Q15(0.012) //�ڻ�I
#define In_XD 4.3f			 //�ڻ�D
#define Out_YP 12.0f		 //�⻷P
#define Out_YI Q15(0.12) //�⻷I
#define Out_YD 5.2f			 //�⻷D
#define In_YP Q15(0.31)	//�ڻ�P
#define In_YI Q15(0.012) //�ڻ�I
#define In_YD 4.3f			 //�ڻ�D
#define ZP 5.0f			 //Z�� P
#define ZI Q15(0.24) //Z�� I
#define ZD 4.0f			 //Z�� D
#define ERR_MAX 800 //�����޷�

//--------------------------------------------------//
//  ��ʱ��0 ��ʼ������
//--------------------------------------------------//
void Timer0Init(void) //12����@24.000MHz
{
	AUXR &= 0x7F; //��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0; //���ö�ʱ��ģʽ
	TL0 = 0x40;		//���ö�ʱ��ֵ
	TH0 = 0xA2;		//���ö�ʱ��ֵ
	TF0 = 0;			//���TF0��־
	TR0 = 1;			//��ʱ��0��ʼ��ʱ
	ET0 = 1;			//Timer0 Interrupt Enable
}

//--------------------------------------------------//
//  ʱ����ʱ ����
//--------------------------------------------------//
void Delay(unsigned int x)
{
	unsigned int i, j;
	for (i = 0; i < x; i++)
		for (j = 0; j < 250; j++)
			;
}

//-------------------------------------------------//
//  PID�㷨�ɿ���ƽ�� ����	��ʱ��0�ж� 12����ִ��һ��
//--------------------------------------------------//
void Flight(void) interrupt 1
{
	RX_model();					 //����ģʽ
	js_shuju(RxBuf, 15); //��ȡ���ݰ�
	Read_MPU6050(tp); //ֱ�Ӷ�ȡMPU6050�����Ǻͼ��ٶȵ����ݰ�

	Angle_ax = KalmanFilter_ax(((int *)&tp)[0], KALMAN_Q, KALMAN_R); //�Ż��㷨
	Angle_ay = KalmanFilter_ay(((int *)&tp)[1], KALMAN_Q, KALMAN_R);
	Angle_az = KalmanFilter_az(((int *)&tp)[2], KALMAN_Q, KALMAN_R);
	Angle_gx = ((float)(((int *)&tp)[4])) / 65.5; //�����Ǵ���	�����λ�� +-��
	Angle_gy = ((float)(((int *)&tp)[5])) / 65.5; //���������� +-500��/S, 1��/�� ��Ӧ���� 65.536
	Angle_gz = KalmanFilter_gyroz(((int *)&tp)[6], Q15(0.2), Q15(0.8));
	IMU_gz = Angle_gz / 65.5;
	Last_Angle_gx = Angle_gx; //������һ�ν��ٶ�����
	Last_Angle_gy = Angle_gy;

	//*********************************** ��Ԫ������ ***********************************
	IMUupdate(Angle_gx * 0.0174533f, Angle_gy * 0.0174533f, IMU_gz * 0.0174533f, Angle_ax, Angle_ay, Angle_az); //��̬���㣬����0.1��
	///���͵�ң����
	//	TxBuf[0]=(AngleX+900)/0xff; // ��ֵ�� 48~1752 = 0-360��
	//	TxBuf[1]=(AngleX+900)%0xff;	// ��ֵ�� 48~1752 = 0-360��
	//	TxBuf[2]=(AngleY+900)/0xff;	// ��ֵ�� 48~1752 = 0-360��
	//	TxBuf[3]=(AngleY+900)%0xff;	// ��ֵ�� 48~1752 = 0-360��
	
	//�ɿ�ʧ���ж� �Զ������㷨,����ң���������Ĳ��ϸ������� �ж�����ͨѶ�Ƿ�����
	if (SSLL == ShiLian) //���SSLL������û�и��¼�ʧ��
	{
		if (++SLJS >= 20)
		{
			SLJS = 19;			 //״̬��ʶ
			HangXiang = 128; //�������
			HengGun = 128;	 //�������
			FuYang = 128;		 //��������
			if(YM > 20) YM--; //������ԭֵ�𽥼�С
		}
	}else{
		SLJS = 0;
		if(YouMen > 1001) YouMen = 1000; //������0-1000���ֵ,�����Ż��㷨 ��������ҡ�˵Ŀ��Ʒ��ȴ�60%���ӵ�90%�����㷨������
		else
		{ //ҡ������50ִ��
			if(YouMen > 50) YM = (YouMen + 300) / 1.3;//ҡ�������㷨
			else YM = YouMen; //ҡ�˵���ֱ�Ӹ�ֵ
		}
	}
	ShiLian = SSLL; //ʧ����������
	YM = YouMen;
	//��б�Ƕȼ��޿���,����ֵ:30��
	if((AngleX + 900) > 1200)
	{
		LEDR = 0;
		YM = 0;
	} //�ɿ�������б
	else if ((AngleX + 900) < 500)
	{
		LEDR = 0;
		YM = 0;
	} //�ɿ�������б
	else if ((AngleY + 900) > 1200)
	{
		LEDR = 0;
		YM = 0;
	} //�ɿ���ǰ��б
	else if ((AngleY + 900) < 500)
	{
		LEDR = 0;
		YM = 0;
	} //�ɿ������б
	else LEDR = 1; //����

	//���п����㷨,MPU6050 X��ָ��
	delta_rc_x = ((float)HengGun - 128) * 2; //�õ� ������ݱ���
	Ax = -AngleX - delta_rc_x + a_x * 5;		 //
	//	Ax =-AngleX+a_x*5;
	if (YM > 20) ERRORX_Out += Ax; //�⻷����(����С��ĳ��ֵʱ������)
	else ERRORX_Out = 0; //����С�ڶ�ֵʱ�������ֵ
	if (ERRORX_Out > ERR_MAX) ERRORX_Out = ERR_MAX; //�����޷�
	else if (ERRORX_Out < -ERR_MAX) ERRORX_Out = -ERR_MAX; //�����޷�

	PID_P = (long)Ax * Out_YP;
	PID_I = ((long)ERRORX_Out * Out_XI) >> 15;
	PID_D = ((Angle_gy + Last_Angle_gy) / 2) * Out_XD;
	PID_Output = (PID_P + PID_I + PID_D + 5) / 10; //�⻷PID

	Last_Ax = Ax;
	gx = PID_Output - Angle_gy; //�⻷ -   ������Y��

	if (YM > 20) ERRORX_In += gx; //�ڻ�����(����С��ĳ��ֵʱ������)
	else ERRORX_In = 0; //����С�ڶ�ֵʱ�������ֵ
	if (ERRORX_In > ERR_MAX) ERRORX_In = ERR_MAX;
	else if (ERRORX_In < -ERR_MAX) ERRORX_In = -ERR_MAX; //�����޷�

	PID_P = ((long)gx * In_XP) >> 15;
	PID_I = ((long)ERRORX_In * In_XI) >> 15;
	PID_D = ((long)gx - Last_gx) * In_XD;
	PID_Output = PID_P + PID_I + PID_D; //�ڻ�PID
	Last_gx = gx;

	if(PID_Output > 1000) PID_Output = 1000; //������޷�
	if(PID_Output < -1000) PID_Output = -1000;
	speed0 = 0 + PID_Output;
	speed1 = 0 - PID_Output;
	speed3 = 0 + PID_Output;
	speed2 = 0 - PID_Output;
	//**************MPU6050 Y��ָ��**************************************************
	delta_rc_y = ((float)FuYang - 128) * 2; //�õ� �������ݱ���
	Ay = -AngleY - delta_rc_y - a_y * 5;
	//Ay  =-AngleY-a_y*5;
	if (YM > 20) ERRORY_Out += Ay; //�⻷����(����С��ĳ��ֵʱ������)
	else ERRORY_Out = 0; //����С�ڶ�ֵʱ�������ֵ

	if(ERRORY_Out > ERR_MAX) ERRORY_Out = ERR_MAX;
	else if(ERRORY_Out < -ERR_MAX) ERRORY_Out = -ERR_MAX; //�����޷�

	PID_P = (long)Ay * Out_YP;
	PID_I = ((long)ERRORY_Out * Out_YI) >> 15;
	PID_D = ((Angle_gx + Last_Angle_gx) / 2) * Out_YD;
	PID_Output = (PID_P + PID_I + PID_D + 5) / 10; //�⻷PID����+5��Ϊ����������?

	Last_Ay = Ay;
	gy = PID_Output - Angle_gx;

	if (YM > 20) ERRORY_In += gy; //�ڻ�����(����С��ĳ��ֵʱ������)
	else ERRORY_In = 0; //����С�ڶ�ֵʱ�������ֵ
	if(ERRORY_In > ERR_MAX) ERRORY_In = ERR_MAX;
	else if(ERRORY_In < -ERR_MAX) ERRORY_In = -ERR_MAX; //�����޷�

	PID_P = ((long)gy * In_YP) >> 15;
	PID_I = ((long)ERRORY_In * In_YI) >> 15;
	PID_D = ((long)gy - Last_gy) * In_YD;
	PID_Output = PID_P + PID_I + PID_D;
	Last_gy = gy;

	if(PID_Output > 1000) PID_Output = 1000; //������޷�
	if(PID_Output < -1000) PID_Output = -1000;
	speed0 = speed0 + PID_Output;
	speed1 = speed1 + PID_Output; //���ص��ٶȲ���
	speed3 = speed3 - PID_Output;
	speed2 = speed2 - PID_Output;

	//************** MPU6050 Z��ָ�� *****************************
	delta_rc_z = -Angle_gz + ((float)HangXiang - 128) * 65 + a_z * 20; //�õ� �������ݱ��� ������
	if(YM > 20) ERRORZ_Out += delta_rc_z;
	else ERRORZ_Out = 0;
	if(ERRORZ_Out > 50000) ERRORZ_Out = 50000;
	else if (ERRORZ_Out < -50000) ERRORZ_Out = -50000; //�����޷�
	PID_P = ((long)delta_rc_z) * ZP;
	PID_I = ((long)ERRORZ_Out * ZI) >> 15;
	PID_D = ((long)delta_rc_z - Anglezlate) * ZD;
	PID_Output = (PID_P + PID_I + PID_D) >> 6;

	Anglezlate = delta_rc_z;
	speed0 = speed0 + PID_Output;
	speed1 = speed1 - PID_Output;
	speed3 = speed3 - PID_Output;
	speed2 = speed2 + PID_Output;
	//�ٶȲ�������(�ٶȲ���������PWM)��ֹ����PWM������Χ0-1000��X����Ч��
	PWM0 = (YM + speed0);
	if(PWM0 > 1000) PWM0 = 1000;
	else if (PWM0 < 0) PWM0 = 0;
	PWM1 = (YM + speed1);
	if(PWM1 > 1000) PWM1 = 1000;
	else if (PWM1 < 0) PWM1 = 0;
	PWM2 = (YM + speed2);
	if(PWM2 > 1000) PWM2 = 1000;
	else if (PWM2 < 0) PWM2 = 0;
	PWM3 = (YM + speed3);
	if(PWM3 > 1000) PWM3 = 1000;
	else if (PWM3 < 0) PWM3 = 0;
	//��������:(������2.4G=5�����Ŵ���30�����ܿ��Ƶ��)
	if(JieSuo == 5 && YM >= 30) PWM(1000 - PWM1, 1000 - PWM2, 1000 - PWM3, 1000 - PWM0);//����PWM
	else PWM(1000, 1000, 1000, 1000);//�ر�PWM
}

void main(void)
{
	PWMGO();										 //��ʼ��PWM
	PWM(1000, 1000, 1000, 1000); //�رյ��
	LEDR = 0;
	LEDG = 1;
	LEDB = 1;		
	Delay(100); 
	LEDR = 1;
	LEDG = 0;
	LEDB = 1;		
	Delay(100); 
	LEDR = 1;
	LEDG = 1;
	LEDB = 0;		
	Delay(100);
	LEDR = 1;
	LEDG = 1;
	LEDB = 1;				 
	Delay(10);				 
	//	InitADC();		//ADCģ��ת�� ��ʼ�������ڿ�����
	//	Delay(10);    // ��ʱ 100
	Init_MPU6050();	//��ʼ��MPU-6050
	Delay(10);			 // ��ʱ 100
	LT8910_Init();	 //����2.4Gģ���ʼ��
	Delay(100);			 //��ʱһ�� 1S
	Timer0Init();		 //��ʼ����ʱ��
	Delay(100);			 //��ʱһ�� 1S
	YouMen = 0;			 //��ʼ�����ű���
	HangXiang = 128; //��ʼ���������
	HengGun = 128;	 //��ʼ���������
	FuYang = 128;		 //��ʼ����������
	LEDB = 0;				 //��������
	a_x = 0;				 // ����ֶ�ֵ
	a_y = 0;				 // �����ֶ�ֵ
	a_z = 0;				 // �����ֶ�ֵ
	//Flight();//�����2��������˵ �ɿغ����ж��� ���������������Ҫ���᡿
	EA = 1; //�����ж�
	while (1)
	{
		//Ҫ�ŵ���ʱ���ж�������
		//	RX_model();				//����ģʽ
		//	js_shuju(RxBuf,15);		//��ȡ���ݰ�
		Xiao1 = RxBuf[0] + RxBuf[1] + RxBuf[2] + RxBuf[3] + RxBuf[4] + RxBuf[5] + RxBuf[6] + RxBuf[7] + RxBuf[8] + RxBuf[9];
		Xiao2 = RxBuf[10];
		if (Xiao1 == Xiao2)
		{
			SSLL = RxBuf[0];										 //���� ʧ������
			JieSuo = RxBuf[1];									 //���� ����ֵ
			YouMen = RxBuf[2] * 0xff + RxBuf[3]; //���� ���ű���
			HangXiang = RxBuf[4];								 //���� �������
			HengGun = RxBuf[5];									 //���� �������
			FuYang = RxBuf[6];									 //���� ��������
			a_x = RxBuf[7] - 128;								 //���� X�ᱣ��ֵ
			a_y = RxBuf[8] - 128;								 //���� Y�ᱣ��ֵ
			a_z = RxBuf[9] - 128;								 //���� Z�ᱣ��ֵ
			LEDG = 1;														 //LED ��
		}else LEDG = 0; //����
		if(JieSuo == 1) LEDB = 1;//����
		if(JieSuo == 5) LEDB = 0;//����
		Delay(3); //��ʱһ��
		//ADC��ѹ��ѹ�����ͣ��������:δʵ��
	}
}
