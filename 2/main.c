/********************************************************************************* 
  *Copyright(C) UESTC 
  *FileName:    ADC DEMO
  *Author:      Tonge
  *Version:     1.0 
  *Date:        2016.12.2
  *Description: 配置msp430f5529的时钟
                SMCLK和MCLK时钟为XT2   4MHz
               独立按键和两个LED
               按键控制led反向，
               同时配置了定时器A1ms中断，
               中断1000*1ms = 1s红色led取反闪烁
                  MSP430F5529
              -----------------
         /|\|             P1.0|-->LED RED
         |  |             P4.7|-->LED GREEN
         -->|RST              |
            |             P2.1|<--KEYLEFT
            |             P1.1|<--KEYRIGHT
      ADC-->| P6.6        P1.6| -->Square Wave 50Hz
            |                 | 
  *Others:      None
  *Function List:  
  *History:  
     1.Date: 
       Author: 
       Modification:  
**********************************************************************************/  
#include "MSP430F5529.h"
#include "stdio.h"
#include "clk.h"
#include "key.h"
#include "timer.h"
#include "i2c.h"
#include <string.h>
#include "ad.h"

unsigned char height = 0;
unsigned char height_old = 0;
unsigned char put1[100];//电压整数部分
unsigned char put2[100];//电压小数部分
unsigned char put3[100];//频率

int k=100,i;


void itoa(int n,unsigned char *s)//int转字符串方便输出
{
  char temp[100];
  int i=0,j=0;
  do
  {
    temp[i++]=n%10+'0';
    n/=10;
  }while(n);
  i--;
  while(i>=0){s[j++]=temp[i];i--;}
  s[j]=' ';
  s[j+1]=' ';
  s[j+2]=0;
}



int main( void )
{
  //unsigned char i;        //计数变量
  WDTCTL = WDTPW+WDTHOLD;  // Stop WDT
  KeyInit();               //按键初始化
  ClkInit();               //时钟初始化
  TimerAInit();            //定时器初始化
  ADC12_init();            //ADC初始化
  OLED_Init();
  OLED_P8x16Str(40,0,"MSP430");
  OLED_P8x16Str(15,2,"Experiment2");
  OLED_P8x16Str(40,6,"Group 8");
  
  P1DIR |= BIT0 + BIT6;          // P1.0 LED set as output P1.6 方波输出
  P4DIR |= BIT7;                 //P4.7 LED set as output 

  while(KeyScan() == NOKEY)     //按键开始测量
  {
    ;
  }
  
  OLED_Clear();            //清屏
  while(1)               // continuous loop
  {
    if(KeyScan()==KEYLEFT)k++;
    if(KeyScan()==KEYRIGHT)k--;
    switch(k%4)
    {
    case 0:m=36;break;//25
    case 1:m=18;break;//50
    case 2:m=12;break;//75
    case 3:m=9;break;//100
    }
    
   if(refresh != 0)
   {
     

     for(int i=0;i<200;++i)if(ADValue[i]>100)height=(int)(ADValue[127]/4096.0*33);//计算电压
       
         itoa(height/10,put1);//显示电压
         itoa(height%10,put2);
         OLED_P8x16Str(0,0,"Amp:");
         OLED_P8x16Str(32,0,put1);
         OLED_P8x16Str(40,0,".");
         OLED_P8x16Str(48,0,put2);
         OLED_P8x16Str(64,0,"V");
       
        itoa(fre,put3);//显示频率
        OLED_P8x16Str(0,3,"Fre:");
        OLED_P8x16Str(32,3,put3);
        if(fre<100)OLED_P8x16Str(48,3," ");
        OLED_P8x16Str(64,3,"Hz");
    
         refresh = 0;
         ADC12CTL0 |= ADC12ENC;    //ADC转换完固定长度就会停止，显示处理完成再次后使能转换
   }
  }
}

/*
       height_old = height;     //上一次电压幅值
       //height = (unsigned char)((float)ADValue[i]*64.0f/4096.0f);             //本次电压幅值
       //height=(int)(ADValue[i]*2.5/4096.0*1000);

if((height < 5 && height_old >60) || (height > 60 && height_old < 5) )//两次相差过大认为是方波补齐竖线
      {
          OLED_Clear_x(i++, 1);//i为哪一列，1或0代表是清空那一列还是画满那一列
      }
      else
      {
          OLED_Clear_x(i, 0);
          OLED_PutPoint(i,63-height);//63减去坐标，使坐标轴变换原点为左下角
      } 
*/




















