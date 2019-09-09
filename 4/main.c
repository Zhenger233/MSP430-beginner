#include "MSP430F5529.h"
#include "clk.h"
#include "key.h"
#include "timer.h"
#include "IIC.h"

/****************��Ҫ��ʾ��һϵ���ַ���***************/
  unsigned char string1[]={"PWM&Capture"};
  unsigned char string2[]={"Frequency:"};
  unsigned char string3[]={"post_duty:"};
  unsigned char string4[]={"Hz"};
  unsigned char string5[]={"%"};

int main( void )
{
  enum key_type key;
  unsigned long zhengshu;
  unsigned long xiaoshu;
  unsigned long int_duty;
  
  unsigned int duty_num=50; //���ڿ���ռ�ձȵı���
  unsigned int fre_num=50;  //���ڿ���Ƶ�ʵı���
  float    cal_pos=0;        //���ڱ�������ת���������
  float    cal_period=0;     //���ڱ�������ת���������
  float    duty=0;           //���ڼ���ռ�ձ�
  
  
  
  WDTCTL = WDTPW+WDTHOLD;  // Stop WDT
  ClkInit();               //ʱ�ӳ�ʼ��
  KeyInit();               //����������ʼ��
  LCD_Init();              //��ʾ����ʼ��
  PWM_Init();              //PWM�������ʼ��
  Capture_Init();          //���벶���ʼ��
  
  
  /*��ʼ������*/
  LCD_ShowString(0,0,string1,16);
  LCD_ShowString(0,3,string2,12);
  LCD_ShowString(0,5,string3,12);
  LCD_ShowString(112,3,string4,12);
  LCD_ShowString(112,5,string5,12);
  
  while(1)
  {
    key = KeyScan();
    if( key != NOKEY )
    {
      switch(key)
      {
        case KEYRIGHT  : 
          {
            fre_num+=50;					//Ƶ��+50
            if(fre_num>1000)fre_num=50;		//Ƶ�ʳ���1000ʱ����
            break;
          }      
        case KEYLEFT :   
          {
            duty_num=duty_num+5;			//ռ�ձ�+5
            if(duty_num>90)duty_num=10;		//����90ʱ����
            break;
          }
      }
      TA0CCR0=4000000/fre_num;				//��TA0CCR0���в������ı�Ƶ��
      TA0CCR1=4000000/fre_num/100*duty_num;   //��TA0CCR1���в������ı�ռ�ձ�
    }
    
    if(flag==3)
    {
      cal_pos=pos_time;
      period=TA0CCR0;				//��������
      cal_period=period;
      
      duty=cal_pos/cal_period;		//����ռ�ձ�
      //int_duty=duty*1000;
      //zhengshu=int_duty/10;
      //xiaoshu=int_duty%10;
      int tempfre=4000000/period;
      
      //LCD_ShowNum(0,0,fre_num,4,16);
      //LCD_ShowNum(56,0,duty_num,3,16);
      //LCD_ShowNum(81,5,zhengshu,2,12); 
      //LCD_ShowNum(87,3,fre_num,3,12);
      //LCD_ShowNum(81,5,duty_num,2,12);
      //LCD_ShowNum(99,5,0,1,12);
      //LCD_ShowChar(93,5,'.',12);       
      //LCD_ShowNum(99,5,xiaoshu,1,12); 
	  
      LCD_ShowNum(87,3,tempfre,3,12);		//��ʾƵ��
      LCD_ShowNum(93,5,duty*100,2,12);		//��ʾռ�ձ�
      
      flag=0;
      //__delay_cycles(100000);
      TA1CCTL1|=CCIE;
    }
  }
}