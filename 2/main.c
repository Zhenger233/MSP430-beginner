/********************************************************************************* 
  *Copyright(C) UESTC 
  *FileName:    ADC DEMO
  *Author:      Tonge
  *Version:     1.0 
  *Date:        2016.12.2
  *Description: ����msp430f5529��ʱ��
                SMCLK��MCLKʱ��ΪXT2   4MHz
               ��������������LED
               ��������led����
               ͬʱ�����˶�ʱ��A1ms�жϣ�
               �ж�1000*1ms = 1s��ɫledȡ����˸
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
unsigned char put1[100];//��ѹ��������
unsigned char put2[100];//��ѹС������
unsigned char put3[100];//Ƶ��

int k=100,i;


void itoa(int n,unsigned char *s)//intת�ַ����������
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
  //unsigned char i;        //��������
  WDTCTL = WDTPW+WDTHOLD;  // Stop WDT
  KeyInit();               //������ʼ��
  ClkInit();               //ʱ�ӳ�ʼ��
  TimerAInit();            //��ʱ����ʼ��
  ADC12_init();            //ADC��ʼ��
  OLED_Init();
  OLED_P8x16Str(40,0,"MSP430");
  OLED_P8x16Str(15,2,"Experiment2");
  OLED_P8x16Str(40,6,"Group 8");
  
  P1DIR |= BIT0 + BIT6;          // P1.0 LED set as output P1.6 �������
  P4DIR |= BIT7;                 //P4.7 LED set as output 

  while(KeyScan() == NOKEY)     //������ʼ����
  {
    ;
  }
  
  OLED_Clear();            //����
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
     

     for(int i=0;i<200;++i)if(ADValue[i]>100)height=(int)(ADValue[127]/4096.0*33);//�����ѹ
       
         itoa(height/10,put1);//��ʾ��ѹ
         itoa(height%10,put2);
         OLED_P8x16Str(0,0,"Amp:");
         OLED_P8x16Str(32,0,put1);
         OLED_P8x16Str(40,0,".");
         OLED_P8x16Str(48,0,put2);
         OLED_P8x16Str(64,0,"V");
       
        itoa(fre,put3);//��ʾƵ��
        OLED_P8x16Str(0,3,"Fre:");
        OLED_P8x16Str(32,3,put3);
        if(fre<100)OLED_P8x16Str(48,3," ");
        OLED_P8x16Str(64,3,"Hz");
    
         refresh = 0;
         ADC12CTL0 |= ADC12ENC;    //ADCת����̶����Ⱦͻ�ֹͣ����ʾ��������ٴκ�ʹ��ת��
   }
  }
}

/*
       height_old = height;     //��һ�ε�ѹ��ֵ
       //height = (unsigned char)((float)ADValue[i]*64.0f/4096.0f);             //���ε�ѹ��ֵ
       //height=(int)(ADValue[i]*2.5/4096.0*1000);

if((height < 5 && height_old >60) || (height > 60 && height_old < 5) )//������������Ϊ�Ƿ�����������
      {
          OLED_Clear_x(i++, 1);//iΪ��һ�У�1��0�����������һ�л��ǻ�����һ��
      }
      else
      {
          OLED_Clear_x(i, 0);
          OLED_PutPoint(i,63-height);//63��ȥ���꣬ʹ������任ԭ��Ϊ���½�
      } 
*/




















