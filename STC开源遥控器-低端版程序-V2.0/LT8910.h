#ifndef _LT8910_H
#define _LT8910_H
#define uchar unsigned char 
#define uint unsigned int
void LT8910_Init(void);//
void RX_model(void);
/* tx_buf:���͵����� Num:���������ܳ��� */ 
void fs_shuju(uchar *tx_buf, uchar Num);
void js_shuju(uchar *rx_buf);

#endif

