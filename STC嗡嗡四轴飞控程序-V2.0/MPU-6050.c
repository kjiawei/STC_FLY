#include <STC15W4K48S4.h> //STC15W4K48S4 ��Ƭ��ͷ�ļ�
#include <intrins.h>
#include <MPU6050.H>

sbit SCL = P3 ^ 4; //IICʱ�����Ŷ���
sbit SDA = P3 ^ 5; //IIC�������Ŷ���

//**************************************
//��ʱ����
//**************************************
void Delay2us()
{
	unsigned char i;
	i = 2;
	while (--i)
		;
}

//I^Cʱ������ʱ���ã�����μ���оƬ�������ֲ�  6050�Ƽ���С1.3us ���ǻ�����⣬������ʱʵ��1.9us����
//**************************************
//I2C��ʼ�ź�
//**************************************
void I2C_Start(void)
{
	SDA = 1; //����������
	SCL = 1; //����ʱ����
	Delay2us();
	SDA = 0; //�����½���
	Delay2us();
	SCL = 0; //����ʱ����
}

//**************************************
//I2Cֹͣ�ź�
//**************************************
void I2C_Stop(void)
{
	SDA = 0; //����������
	SCL = 1; //����ʱ����
	Delay2us();
	SDA = 1; //����������
	Delay2us();
}

//**************************************
//I2C����Ӧ���ź�
//**************************************
bit I2C_RecvACK(void)
{
	SCL = 1; //����ʱ����?
	Delay2us();
	CY = SDA; //��Ӧ���ź�
	SCL = 0;  //����ʱ����
	Delay2us();
	return CY;
}

//**************************************
//��I2C���߷���һ���ֽ�����
//**************************************
void I2C_SendByte(unsigned char dat)
{
	unsigned char i;
	for (i = 0; i < 8; i++) //8λ������
	{
		dat <<= 1; //�Ƴ����ݵ����λ
		SDA = CY;  //�����ݿ�
		SCL = 1;   //����ʱ����
		Delay2us();
		SCL = 0; //����ʱ����
		Delay2us();
	}
	I2C_RecvACK();
}

//**************************************
//��I2C���߽���һ���ֽ�����
//**************************************
unsigned char I2C_RecvByte(void)
{
	unsigned char i;
	unsigned char dat = 0;
	SDA = 1;				//ʹ���ڲ�����,׼����ȡ����,
	for (i = 0; i < 8; i++) //8λ������
	{
		dat <<= 1;
		SCL = 1; //����ʱ����
		Delay2us();
		dat |= SDA; //������
		SCL = 0;	//����ʱ����
		Delay2us();
	}
	return dat;
}

//**************************************
//��I2C�豸д��һ���ֽ�����
//**************************************
void Single_WriteI2C(unsigned char REG_Address, unsigned char REG_data)
{
	I2C_Start();				//��ʼ�ź�
	I2C_SendByte(SlaveAddress); //�����豸��ַ+д�ź�
	I2C_SendByte(REG_Address);  //�ڲ��Ĵ�����ַ��
	I2C_SendByte(REG_data);		//�ڲ��Ĵ������ݣ�
	I2C_Stop();					//����ֹͣ�ź�
}

//**************************************
//��I2C�豸��ȡһ���ֽ�����
//**************************************
/*
unsigned char Single_ReadI2C(unsigned char REG_Address)
{
	unsigned char REG_data;
	I2C_Start();                   //��ʼ�ź�
	I2C_SendByte(SlaveAddress);    //�����豸��ַ+д�ź�
	I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ����0��ʼ
	I2C_Start();                   //��ʼ�ź�
	I2C_SendByte(SlaveAddress+1);  //�����豸��ַ+���ź�
	REG_data=I2C_RecvByte();       //�����Ĵ�������
	
	SDA = 1;                    //дӦ���ź�
	SCL = 1;                    //����ʱ����
	Delay2us();                 //��ʱ
	SCL = 0;                    //����ʱ����
	Delay2us();                 //��ʱ
	
	I2C_Stop();                    //ֹͣ�ź�
	return REG_data;
}
*/

//**************************************
//��ʼ��MPU6050
//**************************************
void Init_MPU6050(void)
{
	Single_WriteI2C(PWR_MGMT_1, 0x00);   //�������״̬
	Single_WriteI2C(SMPLRT_DIV, 0x07);   //������125hz
	Single_WriteI2C(CONFIG, 0x04);		 //21HZ�˲� ��ʱA8.5ms G8.3ms  �˴�ȡֵӦ�൱ע�⣬��ʱ��ϵͳ�������Ϊ��
	Single_WriteI2C(GYRO_CONFIG, 0x08);  //������500��/S 65.5LSB/g
	Single_WriteI2C(ACCEL_CONFIG, 0x18); //���ٶ�+-16g  8192LSB/g
}

//**************************************
//�ϳ�����
//**************************************
/*
int GetData(unsigned char REG_Address)
{
	char H,L;
	H = Single_ReadI2C(REG_Address);
	L = Single_ReadI2C(REG_Address+1);
//	return (H<<8)+L;   //�ϳ�����
	return (((u16)H << 8) + L);   //�ϳ�����
}
*/

//**************************************
//��ȡ����
//**************************************
void Read_MPU6050(unsigned char *buf)
{
	unsigned char i;
	I2C_Start();					//��ʼ�ź�
	I2C_SendByte(SlaveAddress);		//�����豸��ַ+д�ź�
	I2C_SendByte(ACCEL_XOUT_H);		//�ڲ��Ĵ�����ַ��
	I2C_Start();					//��ʼ�ź�
	I2C_SendByte(SlaveAddress + 1); //�����豸��ַ+���ź�
	for (i = 0; i < 13; i++)
	{
		buf[i] = I2C_RecvByte(); //�����Ĵ�������
		SDA = 0;				 //дӦ���ź�
		SCL = 1;				 //����ʱ����
		Delay2us();
		SCL = 0; //����ʱ����
		Delay2us();
	}
	buf[i] = I2C_RecvByte(); //���һ���ֽ�
	SDA = 1;				 //д��Ӧ���ź�
	SCL = 1;				 //����ʱ����
	Delay2us();
	SCL = 0; //����ʱ����
	Delay2us();
	I2C_Stop(); //ֹͣ�ź�
}
