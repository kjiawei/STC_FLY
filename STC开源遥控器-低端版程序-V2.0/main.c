/*------------------------------------------------------------
��Դ����-������ᰮ���߶����Խ����޸��뼼��������
������ڴ˻������ܵ������õķ���Ч���������������ϵ���з��ʵ�ｱ��Ŷ��
ԭ������ߣ���ϵ��ʽQQ��250278069  
STC�������Ἴ������Ⱥ��421109612
�Ա���վ��http://shop117773209.taobao.com/shop/view_shop.htm?spm=a1z0e.1.0.0.eqIhKz&mytmenu=mdianpu&utkn=g,meztsobuhe3to1427461542456&user_number_id=669230514&scm=1028.1.1.20001
�������ʱ�䣺2016��8��8��-V2.0��
��Ƭ���ͺţ�STC15W408AS-TSSOP20	�ڲ�������Ƶ�ʣ�24M
��Ƭ������11.0592M����STC-ISP���������ѡ��
��ע�����ݵͶ̰�ң��������ɫ��PCB-V0.41�汾�� ������V0.3�ġ�
------------------------------------------------------------*/
#include <STC15W408AS.h>	//STC15W408AS ר��ͷ�ļ�
#include "STC15W4K-ADC.h"	//STC15W4K-ADC	Ӳ��ADCģ��ת��
#include "EEPROM.h"			//STC-EEPROM�ڲ��洢
#include "LT8910.h"	    	//2.4G�����շ�ģ��

/***********************************************************************
���ð��� IO�ڶ���
**********************************************************************/
sbit	KAL   =P3^2; //��߰��� L
sbit	KAR   =P5^4; //�ұ߰��� R
sbit	KAS   =P5^5; //�������� S
sbit	Feng  =P3^6; //��Դ������ 
sbit	LED   =P3^7; //����LED ��
/*------------------------------------------------
                  ȫ�ֺ�������
------------------------------------------------*/
unsigned char TxBuf[20]; //���÷��ͳ��ȣ����Ϊ32�ֽ�
/*------------------------------------------------
                  ��ʱ����
------------------------------------------------*/
void Delay(unsigned int s)
{
	unsigned int i,j;
	for(i=0; i<s; i++)
		for(j=0; j<250; j++);
}

//--------------------------------------------------//
//  �ɿ��������ô洢���ݱ�����λ ����
//--------------------------------------------------//
void init_FKCanShu(void) 
{
	unsigned char data DD[10];
	DD[0]=128;//�洢 ��λ��ʼֵ
	DD[1]=128;//�洢 ��λ��ʼֵ
	DD[2]=128;//�洢 ��λ��ʼֵ
	EEPROM_Xie(DD);//�洢 �ֶ�У׼����ֵ��EEPROM
	Feng=0;Delay(4000);Feng=1;
}

/*------------------------------------------------
 //����I/O��ȫ��Ϊ׼˫��������ģʽ           
------------------------------------------------*/
void GPIOINT(void)
{
	P0M0=0x00;P0M1=0x00;
	P1M0=0x00;P1M1=0x00;
	P2M0=0x00;P2M1=0x00;
	P3M0=0x00;P3M1=0x00;
	P4M0=0x00;P4M1=0x00;
	P5M0=0x00;P5M1=0x00;
	P6M0=0x00;P6M1=0x00;
	P7M0=0x00;P7M1=0x00;
}

void main(void)
{	
	unsigned char GGGXYZ[3];//�ֶ�У׼�޸ı���
	unsigned int AD1;//ADCģ��ת��ҡ�����ֵ0~1023
	unsigned char AD2,AD3,AD4,AD5;//ADCģ��ת��ҡ�����ֵ0~255
	unsigned char HXz,HGz,FYz;//ҡ�˲��� �м�ֵ 0~255
	unsigned char AJin,AJout,AJkkk; //ADC����ֵ ����
	char HXcha,HGcha,FYcha;//����ֵ ��������  -128~128
	unsigned int YMout;				//���Ʋ������ֵ 0~1000
	unsigned char HXout,HGout,FYout;//���Ʋ������ֵ 0~128~255
	unsigned char HangXiang,HengGun,FuYang;//΢������
	unsigned char SZML;//��������״̬���Ʊ���
	unsigned char SSLL;//ͨѶ״̬ ����
	unsigned char Xiao;//����Ч��� ����

	GPIOINT();		//��Ƭ��IO�ڳ�ʼ��
	Delay(10);		//��ʱһ��
	InitADC();		//��Ƭ��Ӳ��ADC��ʼ��
	Delay(100);		//��ʱһ��
	LT8910_Init();	//����2.4Gģ���ʼ��
	Delay(100);		//��ʱһ��
	if(KAR==0) init_FKCanShu();//��������R������ ����΢������������ʼֵ
	//����ҡ���м�ֵ 
	AD2=GetADCResult(1)/4; //����
	AD3=GetADCResult(2)/4; //���
	AD4=GetADCResult(6)/4; //����
	HXz=AD2;  HGz=AD3;  FYz=AD4; //������ǰҡ���м�ֵ
	if(AD2>128)HXcha=128-AD2; else if(AD2<128)HXcha=128-AD2; else HXcha=0; //���� ����ֵ
	if(AD3>128)HGcha=128-AD3; else if(AD3<128)HGcha=128-AD3; else HGcha=0; //��� ����ֵ
	if(AD4>128)FYcha=128-AD4; else if(AD4<128)FYcha=128-AD4; else FYcha=0; //���� ����ֵ
	//------------------------------------------------------------------//
	EEPROM_Du(GGGXYZ);//����EEPROM�� �ֶ�У׼����ֵ
	HengGun  =GGGXYZ[0];	//���� �������
	FuYang   =GGGXYZ[1];	//���� ��������
	HangXiang=GGGXYZ[2];	//���� �������
	SSLL=0;			//ʧ������ ��ʼ��
	SZML=0;			//������� ��ʼ��
	Delay(10);      //��ʱ 10
	while(1)
	{
	AD1=GetADCResult(0);   //����  ģ��ת�� 0-1023 ��10λ���ȣ�
	AD2=GetADCResult(1)/4; //����  ģ��ת�� 0-1023 ����4 �õ�0-255
	AD3=GetADCResult(6)/4; //���  ģ��ת�� 0-1023 ����4 �õ�0-255
	AD4=GetADCResult(2)/4; //����  ģ��ת�� 0-1023 ����4 �õ�0-255
	AD5=GetADCResult(7)/4; //����  ģ��ת�� 0-1023 ����4 �õ�0-255
 
 	//���Ų��� ҡ��ֵ									 //����㷨
	if(AD1>1000)YMout=1000;else YMout=AD1;				 //�����������ֵ 1000
	//������� ҡ��ֵ									 //����㷨
	if(AD2>HXz+5)		HXout=HXz+HXcha+(AD2-(HXz+5))/2; //���ڲο�ֵ ���㷨
	else if(AD2<HXz-5)	HXout=HXz+HXcha-((HXz-5)-AD2)/2; //С�ڲο�ֵ ���㷨
	else  HXout=128;									 //ҡ�˾������ֵ
	//������� ҡ��ֵ									 //����㷨
	if(AD3>HGz+5)		HGout=HGz+HGcha+(AD3-(HGz+5))/2; //���ڲο�ֵ ���㷨
	else if(AD3<HGz-5)	HGout=HGz+HGcha-((HGz-5)-AD3)/2; //С�ڲο�ֵ ���㷨
	else  HGout=128;									 //ҡ�˾������ֵ
	//�������� ҡ��ֵ									 //����㷨							 
	if(AD4>FYz+5)		FYout=FYz+FYcha+(AD4-(FYz+5))/2; //���ڲο�ֵ ���㷨
	else if(AD4<FYz-5)	FYout=FYz+FYcha-((FYz-5)-AD4)/2; //С�ڲο�ֵ ���㷨
	else  FYout=128;									 //ҡ�˾������ֵ

	//ADC����ֵ �����㷨��ֻ֧�ֵ�������, ��֧�ֶ��ͬʱ���£�
	if(AD5>2){AJin=AD5;						//�жϰ����Ƿ񱻰��� ADC>0
		 if(AJin>35&&AJin<43)   AJout=1;    //������ S1
	else if(AJin>70&&AJin<80)   AJout=2;	//������ S2
	else if(AJin>108&&AJin<117) AJout=3;	//������ S3
	else if(AJin>146&&AJin<156) AJout=4;	//������ S4
	else if(AJin>192&&AJin<202) AJout=5;	//������ S5
	else if(AJin>250&&AJin<256) AJout=6;	//������ S6
	}
	else 					    AJout=0;	//�ް���
	if(AJout!=0){ //������� �жϴ���
	if(AJkkk!=AJout&&AJout==1){HangXiang++;Feng=0;Delay(200);Feng=1;}//΢��������� +
	if(AJkkk!=AJout&&AJout==2){HangXiang--;Feng=0;Delay(200);Feng=1;}//΢���������	-
	if(AJkkk!=AJout&&AJout==3){FuYang++;   Feng=0;Delay(200);Feng=1;}//΢����������	+
	if(AJkkk!=AJout&&AJout==4){FuYang--;   Feng=0;Delay(200);Feng=1;}//΢����������	-
	if(AJkkk!=AJout&&AJout==6){HengGun++;  Feng=0;Delay(200);Feng=1;}//΢���������	+
	if(AJkkk!=AJout&&AJout==5){HengGun--;  Feng=0;Delay(200);Feng=1;}//΢���������	-
	}
	AJkkk=AJout;//�����仯����
	
	//����L ������ͣ�ɡ�  ��������ֵ �����հ��������ɷɿش��������
	if(KAL==0){SZML=1;LED=1;Feng=0;Delay(300);Feng=1;}//����ͣ��

	//����S ���������С� ���ű���������͵� ���ɽ�������
	if(YMout<5){if(KAS==0){SZML=5;LED=0;Feng=0;Delay(200);
	                 Feng=1;Delay(200);Feng=0;Delay(200);Feng=1;}}//��������

	if(KAR==0){  	  //����R ִ�н���΢���������桿�� EEPROM
	GGGXYZ[0]=HengGun; 	//���� ����ֶ�ֵ 
	GGGXYZ[1]=FuYang; 	//���� �����ֶ�ֵ
	GGGXYZ[2]=HangXiang;//���� �����ֶ�ֵ
	EEPROM_Xie(GGGXYZ);		//�洢 �ֶ�У׼����ֵ��EEPROM
	Feng=0;Delay(500);Feng=1; //��������һ��
	}

	Xiao=SSLL+SZML+(YMout/0xff)+(YMout%0xff)+HXout+HGout+FYout+HengGun+FuYang+HangXiang;//�����Ч���
	TxBuf[0]=SSLL++;		//���� ʧ������
	TxBuf[1]=SZML;			//���� ����ֵ 1=����  5=����
	TxBuf[2]=YMout/0xff;	//���� ���Ų��� ��2λ 
	TxBuf[3]=YMout%0xff;	//���� ���Ų��� ��8λ
	TxBuf[4]=HXout;			//���� ����ҡ�˲���
	TxBuf[5]=HGout;			//���� ���ҡ�˲���
	TxBuf[6]=FYout;			//���� ����ҡ�˲���
	TxBuf[7]=HengGun;		//���� ���΢������
	TxBuf[8]=FuYang;  		//���� ����΢������
	TxBuf[9]=HangXiang;		//���� ����΢������
	TxBuf[10]=Xiao;			//���� Ч���

	fs_shuju(TxBuf,15);		//ִ�н����ݰ����ͳ�ȥ
	Delay(200);				
	}
}