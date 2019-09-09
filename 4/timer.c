/************************************************************
  文件名称：                timer.c
  文件概述：                对TA0输出PWM波 TA1进行输入捕获的配置工作
  包含的头文件及概述：      timer.h ：函数声明及部分宏定义                         
  引脚声明：                
                            TA0 PWM波输出口   ：P1.2
                            TA1 输入捕获输入口：P2.0
*************************************************************/ 


#include 	"msp430f5529.h"
#include  	"timer.h"

char flag=0;                       //用于标记当前捕获状态 0:就绪 1：捕获到一个上升沿 2：捕获到一个下降沿 
unsigned int pos_time=0;          //用于记录正脉宽
unsigned int period  =0;          //用于记录周期


unsigned int start   =0;          //开始测量上升沿
unsigned int pause   =0;


void PWM_Init( void )
{
        //GPIO SET
	P1SEL |= BIT2;             //配置P1.2复用为定时器TA0.1
    P1DIR |= BIT2;             //配置P1.2为输出    
    //PWM  SET     
    TA0CTL=TASSEL_2+MC_1;  //使用SMCLK为时钟源 UP计数模式
    //TA0CTL|=ID_3;
    TA0CCTL1=OUTMOD_7;          //TA0输出模式7
    TA0CCR0=80000;               //计数寄存器计数到80000 PWM频率50Hz
    TA0CCR1=40000;               //初始化占空比为50%
}


void Capture_Init(void)
{
        //GPIO SET
        P2SEL |= BIT0;             //配置P2.0复用为定时器TA1.1
        P2DIR &=~BIT0;             //配置P2.0为输入
        
        //CAPTURE SET
        TA1CTL=TASSEL_2+MC_2+TACLR+TAIE;   //使用SMCLK为时钟源 连续计数模式 8分频
        TA1CCTL1=CM_1+SCS+CAP+CCIE+CCIS_0;      //捕获模式 上升沿捕获 同步 中断使能 CCI1A(P 2.0)
        _BIS_SR( GIE );		                //使能系统总中断
}


#pragma	vector = TIMER1_A1_VECTOR
__interrupt void Timer_A1 ( void )
{
    switch(TA1IV)
    {
       case 2:
        if(TA1CCTL1&CM_1)                 //捕获到上升沿
        {
          if(flag==0)                     //第一次捕获上升沿
          {
            flag=1;                       //记录捕获到一个上升沿
            TA1CCTL1=(TA1CCTL1&(~CM_1))|CM_2; //改变触发方式为下降触发
            start=TA1CCR1;
          }
          else if(flag==2)              //之前捕获了上升沿和下降沿
          {
            flag=3;                      //记录第二次捕获到上升沿 可以进行显示
            period=TA1CCR1-start;        //记录周期
            TA1CCTL1&=~CCIE;             //关闭定时器中断使能
            TA1CCR1=0;                   //定时器清零
          }
        }
        else if(TA1CCTL1&CM_2)                
        {
          if(flag==1)                    //之前捕获了上升沿       
          {
            flag=2;                      //记录捕获到一个下降沿
            pause=TA1CCR1;         
            pos_time=pause-start;        //得出正脉宽的时间
            TA1CCTL1=(TA1CCTL1&(~CM_2))|CM_1; //改变触发方式为上升触发
          }
        }
        break;
    }
}




