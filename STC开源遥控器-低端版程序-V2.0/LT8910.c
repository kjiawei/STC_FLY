#include <STC15F2K60S2.h>	//STC15W4K48S4 ר��ͷ�ļ�
#include "LT8910.h"
#define uchar unsigned char 
#define uint unsigned int
uchar RegH;
uchar RegL;
/* ====================== LT8910�˿ڶ��� ====================== */ 
sbit RST     = P3^4;
sbit SPI_SS  = P3^5;
sbit SPI_CLK = P1^5;   //SPI ʱ��
sbit MOSI    = P1^4;   //��SPI ���������
sbit MISO    = P1^3;   //��SPI ���������
sbit PKT     = P3^3;

/* ======================= us����ʱ���� ======================= */ 
void Delay_us(uint n)
{
    for(;n>0;n--);
}
/* ========================= д�Ĵ��� ========================= */ 
void SPI_WriteReg(uchar addr,uchar H,uchar L)
{
    int i;
    SPI_SS = 0;
    for(i = 0; i < 8; ++ i)
 {
    MOSI = addr & 0x80;
    SPI_CLK = 1; 
    SPI_CLK = 0;
    addr = addr << 1; 
 }
    for(i = 0; i < 8; ++i)
 {
    MOSI = H & 0x80;
    SPI_CLK = 1;
    SPI_CLK = 0;
    H = H << 1;
 }
    for(i = 0; i < 8; ++i)
 {  
    MOSI = L & 0x80;
    SPI_CLK = 1;
    SPI_CLK = 0;
    L = L << 1;
 }
    SPI_SS = 1;
}
/* ========================= ���Ĵ��� ========================= *
void SPI_ReadReg(uchar addr)
{
    int i;	
    SPI_SS = 0;
    addr += 0x80;
    for(i = 0; i < 8; ++ i)
 {
    MOSI = addr & 0x80;
    SPI_CLK = 1;
    SPI_CLK = 0;
    addr = addr << 1;
 }
    for(i = 0; i < 8; ++ i)
 {
    SPI_CLK = 1;
    SPI_CLK = 0;
    RegH = RegH << 1;  
    RegH |= MISO;
 }
    for(i = 0; i < 8; ++ i)
 {
    SPI_CLK = 1;
    SPI_CLK = 0;
    RegL = RegL << 1; 
    RegL |= MISO;
 }
    SPI_SS = 1;
}
/* ========================== ��ʼ�� ========================== */
void LT8910_Init(void)
{
    RST = 0;	   //��λ ����
    Delay_us(500); //��ʱ 500΢��
    RST = 1;	   //��λ ����
    Delay_us(500); //��ʱ 500΢��
    PKT = 1;	   //�շ� ��־λ ����
    SPI_WriteReg( 0, 0x6f, 0xef );	//
    SPI_WriteReg( 1, 0x56, 0x81 );	//
    SPI_WriteReg( 2, 0x66, 0x17 );	//
    SPI_WriteReg( 4, 0x9c, 0xc9 );	//
    SPI_WriteReg( 5, 0x66, 0x37 );	//
    SPI_WriteReg( 7, 0x00, 0x00 );	//�趨RF Ƶ��������Ƶ��Ϊ��f=2402+ RF_PLL_CH_NO
    SPI_WriteReg( 8, 0x6c, 0x90 );	//
    SPI_WriteReg( 9, 0x48, 0x00 );	//PA ������� -12.2dbm
    SPI_WriteReg(10, 0x7f, 0xfd );	//��������
    SPI_WriteReg(11, 0x00, 0x08 );	//RSSI
    SPI_WriteReg(12, 0x00, 0x00 );	//У׼VCO
    SPI_WriteReg(13, 0x48, 0xbd );	//
    SPI_WriteReg(22, 0x00, 0xff );	//
    SPI_WriteReg(23, 0x80, 0x05 );	//
    SPI_WriteReg(24, 0x00, 0x67 );	//
    SPI_WriteReg(25, 0x16, 0x59 );	//
    SPI_WriteReg(26, 0x19, 0xe0 );	//
    SPI_WriteReg(27, 0x13, 0x00 );	//
    SPI_WriteReg(28, 0x18, 0x00 );	//
    SPI_WriteReg(32, 0x58, 0x00 );	//LEN(��Ҫ����)
    SPI_WriteReg(33, 0x3f, 0xc7 );	//
    SPI_WriteReg(34, 0x20, 0x00 );	//
    SPI_WriteReg(35, 0x0a, 0x00 );  //�ط�����Ϊ9�� һ������10��
    SPI_WriteReg(36, 0x00, 0x01 );	//SYNC_WORD[15:0]
    SPI_WriteReg(37, 0x00, 0x01 );	//SYNC_WORD[31:16]
    SPI_WriteReg(38, 0x00, 0x01 );	//SYNC_WORD[47:32]
    SPI_WriteReg(39, 0x00, 0x01 );	//SYNC_WORD[63:48]
    SPI_WriteReg(40, 0x44, 0x02 );	//FIFO ��ֵ
    SPI_WriteReg(41, 0xb8, 0x00 );  //CRC=ON;scramble=OFF;AUTO_ACK=ON
    SPI_WriteReg(42, 0xfd, 0xff );  //RSSI ɨ����ŵ�����
    SPI_WriteReg(43, 0x00, 0x0f );  //��ʼɨ��RSSI
//    SPI_WriteReg(44, 0x10, 0x00 );  //ͨѶ���� 62.5Kbps~1M
//    SPI_WriteReg(45, 0x04, 0x80 );  //	
}
/* ========================= �����ֽ� ========================= */ 
void SPI_WriteReg8(uchar add,uchar H)
{
    uchar i;
    SPI_SS = 0;
    for(i = 0; i < 8; ++i)
 {
    MOSI = add & 0x80;
    SPI_CLK = 1; 
    SPI_CLK = 0;
    add = add << 1;
 }
    for(i = 0; i < 8; ++i) 
 {		
    MOSI = H & 0x80;
    SPI_CLK = 1;
    SPI_CLK = 0;
    H = H << 1;
 }
    SPI_SS = 1;
}

/* ========================= �������� ========================= */ 
/* tx_buf�����͵�����                    Num��   ���������ܳ��� */ 
void fs_shuju(uchar *tx_buf, uchar Num)
{
    uchar i=0;
    SPI_WriteReg(52, 0x80, 0x80);		//���FIFO
    SPI_WriteReg(7,  0x00, 0x00);		//���뷢��ģʽ
    SPI_WriteReg(50, Num,0);			//��ʾ�������ֽ���

    for(i=0;i<Num;i++)
    	SPI_WriteReg8(50, tx_buf[i]);		//��ȡ����

    SPI_WriteReg(7, 0x01, 0x20);        //��ʼ����
    while (PKT== 0);                    //�ȴ����ݷ������
}