/********************************************************************************* 
  *Copyright(C),UESTC
  *FileName:    定时器配置
  *Author:      Tonge
  *Version:     1.0 
  *Date:        2016.12.2
  *Description:  配置msp430f5529的定时器TimerA0
  *Others:      None
  *Function List:  
        1.void TimerAInit(void)
			 定时器A配置初始化，
			 增计数模式配置为1ms中断，可自行修改
			 上电调用一次即可。
	2.__interrupt void Timer_A0 ( void )
			 中断服务函数，在其中自行编写定时中断时
			 所要执行的命令，注意简洁，不要过多循环判断
			 尽量减少中断服务函数执行时间
  *History:  
     1.Date: 
       Author: 
       Modification:  
**********************************************************************************/  

#include 	"msp430f5529.h"
#include  	"timer.h"
unsigned long int time = 0;
unsigned char refresh = 0;
extern unsigned int fre=0;
extern unsigned int m=20;
float tempfre;
void TimerAInit( void )
{
	TA0CCTL0 = CCIE;	      //TACCR0 interrupt enable
	TA0CCR0  = 2222;             // 增数到4000,定时1ms25
        TA0CTL   = TASSEL_2 + MC_1; //定时器时钟为子系统时钟，
						
	_BIS_SR( GIE );		  //使能系统总中断
}


void TimerAStop( void )
{
      TA0CTL	^= ~MC_0;				//停止模式
}

void TimerAClear( void )
{
      time = 0;
}

#pragma		vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0 ( void )
{
	static int count = 0;
	count ++;
	time++;
	if( count == 1800 )//2222*1800=4000000即1s 定时1s改变led闪烁方向
	{
              count = 0;
              P1OUT ^= BIT0;
              refresh = 1;//刷新显示
              fre=(int)tempfre;
              tempfre=0;
	}
        if( count % m == 0)//m取36，18，12，9控制频率
        {
          P1OUT ^= BIT6;//P1.6产生方波采集
          tempfre+=0.5;//跳变一次频率+0.5
        }
        ADC12CTL0 |= ADC12SC;//使能一次adc转换
}


































