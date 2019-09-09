#include "MSP430F5529.h"
#include        "ad.h"

unsigned int ADValue[LEN_VALUE];
unsigned int flag = 0;//采集完成标志
unsigned int f=0;
//unsigned int tempf=0;

void ADC12_init()
{
	P6SEL |= BIT2;      // P6.2 ADC option

        ADC12CTL0 |= ADC12REFON + ADC12REF2_5V;//打开参考模块，参考模块输出的参考电压为2.5V
	ADC12CTL0 |= ADC12ON + ADC12SHT0_0; // 打开 ADC12, 设置采样时钟
	ADC12CTL1 |= ADC12SHP + ADC12SSEL_2 + ADC12DIV_0 + ADC12CONSEQ_0; //使用采样时钟
	ADC12MCTL0 = ADC12SREF_0 + ADC12INCH_2 ;//采集6.2的电压数据存储到ADC12MEN0
        ADC12MCTL0 &=~ 6<<4;
        ADC12IE    = ADC12IE0; //中断使能
	ADC12CTL0 |= ADC12ENC;    //使能转换
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
        ADC12CTL0 &= ~ADC12ENC;//结束转换

    }
}





