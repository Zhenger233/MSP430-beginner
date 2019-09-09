#include <msp430f5529.h>  
#include "clk.h"

void ClkInit(void) 
{   
  
  P5SEL   |= BIT2|BIT3; //��IO����ΪXT2����  
  UCSCTL6 &= ~XT2OFF;   //ʹ��XT2  
  
  UCSCTL4  = UCSCTL4&(~(SELA_7))|SELA_1; //�Ƚ�ACLK����ΪVLOCLK  
  UCSCTL3 |= SELREF_2;                  //��REFCLK����ΪREFCLK  
  
  while (SFRIFG1 & OFIFG){  
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);         
    /* �������ʱ�ӱ�־λ  
    *������Ҫ������ֱ�־λ����Ϊ�κ�һ��  
    *��־λ���ὫOFIFG��λ  */
    SFRIFG1 &= ~OFIFG;                // ���ʱ�Ӵ����־λ  
  }  
  UCSCTL4 = UCSCTL4&(~(SELS_7|SELM_7))|SELS_5|SELM_5;//��SMCLK��MCLKʱ��Դ����ΪXT2
                                                     //4MHz
 
}