/*----------------------------

----------------------------*/
extern void          _nop_     (void);
#define ADC_POWER   0x80            //ADC��Դ����λ
#define ADC_SPEEDLL 0x00            //540��ʱ��
/////-----��ʼ��ADC-----/////
void InitADC(void)
{
    P1ASF = 0xC7;                   //����ΪAD��:P1.0,P1.1,P1.2,P1.6,P1.7.
    ADC_RES = 0;                    //�������Ĵ���
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
//  Delay(2);                       //ADC�ϵ粢��ʱ
}
/*----------------------------
��ȡADC���
extern void          _pop_     (unsigned char _sfr);
extern void          _nop_     (void);
----------------------------*/
typedef unsigned char BYTE;
#define ADC_FLAG    0x10            //ADC��ɱ�־
#define ADC_START   0x08            //ADC��ʼ����λ
/////-----��ȡADCĳһ���˿�-----/////
unsigned int GetADCResult(BYTE ch)
{	unsigned int ADC10;
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //�ȴ�4��NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//�ȴ�ADCת�����
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC
	ADC10=ADC_RES*4+ADC_RESL; 
    return ADC10;                   //����ADC���
}
