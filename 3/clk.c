#include <msp430f5529.h>  
#include "clk.h"

void ClkInit(void) 
{   
  
  P5SEL   |= BIT2|BIT3; //将IO配置为XT2功能  
  UCSCTL6 &= ~XT2OFF;   //使能XT2  
  
  UCSCTL4  = UCSCTL4&(~(SELA_7))|SELA_1; //先将ACLK配置为VLOCLK  
  UCSCTL3 |= SELREF_2;                  //将REFCLK配置为REFCLK  
  
  while (SFRIFG1 & OFIFG){  
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);         
    /* 清除三类时钟标志位  
    *这里需要清除三种标志位，因为任何一种  
    *标志位都会将OFIFG置位  */
    SFRIFG1 &= ~OFIFG;                // 清除时钟错误标志位  
  }  
  UCSCTL4 = UCSCTL4&(~(SELS_7|SELM_7))|SELS_5|SELM_5;//将SMCLK和MCLK时钟源配置为XT2
                                                     //4MHz
 
}