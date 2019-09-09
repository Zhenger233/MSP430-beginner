#include "MSP430F5529.h"
#include        "ad.h"

unsigned int ADValue[LEN_VALUE];
unsigned int flag = 0;//�ɼ���ɱ�־
unsigned int f=0;
//unsigned int tempf=0;

void ADC12_init()
{
	P6SEL |= BIT2;      // P6.2 ADC option

        ADC12CTL0 |= ADC12REFON + ADC12REF2_5V;//�򿪲ο�ģ�飬�ο�ģ������Ĳο���ѹΪ2.5V
	ADC12CTL0 |= ADC12ON + ADC12SHT0_0; // �� ADC12, ���ò���ʱ��
	ADC12CTL1 |= ADC12SHP + ADC12SSEL_2 + ADC12DIV_0 + ADC12CONSEQ_0; //ʹ�ò���ʱ��
	ADC12MCTL0 = ADC12SREF_0 + ADC12INCH_2 ;//�ɼ�6.2�ĵ�ѹ���ݴ洢��ADC12MEN0
        ADC12MCTL0 &=~ 6<<4;
        ADC12IE    = ADC12IE0; //�ж�ʹ��
	ADC12CTL0 |= ADC12ENC;    //ʹ��ת��
}

#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR(void)
{
    static int  j = 0;
    ADValue[j] = ADC12MEM0;
    j++;

    if(j == 199)
    {
	j   =1;
	flag=1;
        ADC12CTL0 &= ~ADC12ENC;//����ת��

    }
}





