#include "MSP430F5529.h"
#include "clk.h"
#include "key.h"
#include "timer.h"
#include "IIC.h"

/****************需要显示的一系列字符串***************/
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
  
  unsigned int duty_num=50; //用于控制占空比的变量
  unsigned int fre_num=50;  //用于控制频率的变量
  float    cal_pos=0;        //用于变量类型转换方便计算
  float    cal_period=0;     //用于变量类型转换方便计算
  float    duty=0;           //用于计算占空比
  
  
  
  WDTCTL = WDTPW+WDTHOLD;  // Stop WDT
  ClkInit();               //时钟初始化
  KeyInit();               //独立按键初始化
  LCD_Init();              //显示屏初始化
  PWM_Init();              //PWM波输出初始化
  Capture_Init();          //输入捕获初始化
  
  
  /*初始化界面*/
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
            fre_num+=50;					//频率+50
            if(fre_num>1000)fre_num=50;		//频率超过1000时重置
            break;
          }      
        case KEYLEFT :   
          {
            duty_num=duty_num+5;			//占空比+5
            if(duty_num>90)duty_num=10;		//超过90时重置
            break;
          }
      }
      TA0CCR0=4000000/fre_num;				//对TA0CCR0进行操作来改变频率
      TA0CCR1=4000000/fre_num/100*duty_num;   //对TA0CCR1进行操作来改变占空比
    }
    
    if(flag==3)
    {
      cal_pos=pos_time;
      period=TA0CCR0;				//设置周期
      cal_period=period;
      
      duty=cal_pos/cal_period;		//计算占空比
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
	  
      LCD_ShowNum(87,3,tempfre,3,12);		//显示频率
      LCD_ShowNum(93,5,duty*100,2,12);		//显示占空比
      
      flag=0;
      //__delay_cycles(100000);
      TA1CCTL1|=CCIE;
    }
  }
}