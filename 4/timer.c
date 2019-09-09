/************************************************************
  �ļ����ƣ�                timer.c
  �ļ�������                ��TA0���PWM�� TA1�������벶������ù���
  ������ͷ�ļ���������      timer.h ���������������ֺ궨��                         
  ����������                
                            TA0 PWM�������   ��P1.2
                            TA1 ���벶������ڣ�P2.0
*************************************************************/ 


#include 	"msp430f5529.h"
#include  	"timer.h"

char flag=0;                       //���ڱ�ǵ�ǰ����״̬ 0:���� 1������һ�������� 2������һ���½��� 
unsigned int pos_time=0;          //���ڼ�¼������
unsigned int period  =0;          //���ڼ�¼����


unsigned int start   =0;          //��ʼ����������
unsigned int pause   =0;


void PWM_Init( void )
{
        //GPIO SET
	P1SEL |= BIT2;             //����P1.2����Ϊ��ʱ��TA0.1
    P1DIR |= BIT2;             //����P1.2Ϊ���    
    //PWM  SET     
    TA0CTL=TASSEL_2+MC_1;  //ʹ��SMCLKΪʱ��Դ UP����ģʽ
    //TA0CTL|=ID_3;
    TA0CCTL1=OUTMOD_7;          //TA0���ģʽ7
    TA0CCR0=80000;               //�����Ĵ���������80000 PWMƵ��50Hz
    TA0CCR1=40000;               //��ʼ��ռ�ձ�Ϊ50%
}


void Capture_Init(void)
{
        //GPIO SET
        P2SEL |= BIT0;             //����P2.0����Ϊ��ʱ��TA1.1
        P2DIR &=~BIT0;             //����P2.0Ϊ����
        
        //CAPTURE SET
        TA1CTL=TASSEL_2+MC_2+TACLR+TAIE;   //ʹ��SMCLKΪʱ��Դ ��������ģʽ 8��Ƶ
        TA1CCTL1=CM_1+SCS+CAP+CCIE+CCIS_0;      //����ģʽ �����ز��� ͬ�� �ж�ʹ�� CCI1A(P 2.0)
        _BIS_SR( GIE );		                //ʹ��ϵͳ���ж�
}


#pragma	vector = TIMER1_A1_VECTOR
__interrupt void Timer_A1 ( void )
{
    switch(TA1IV)
    {
       case 2:
        if(TA1CCTL1&CM_1)                 //����������
        {
          if(flag==0)                     //��һ�β���������
          {
            flag=1;                       //��¼����һ��������
            TA1CCTL1=(TA1CCTL1&(~CM_1))|CM_2; //�ı䴥����ʽΪ�½�����
            start=TA1CCR1;
          }
          else if(flag==2)              //֮ǰ�����������غ��½���
          {
            flag=3;                      //��¼�ڶ��β��������� ���Խ�����ʾ
            period=TA1CCR1-start;        //��¼����
            TA1CCTL1&=~CCIE;             //�رն�ʱ���ж�ʹ��
            TA1CCR1=0;                   //��ʱ������
          }
        }
        else if(TA1CCTL1&CM_2)                
        {
          if(flag==1)                    //֮ǰ������������       
          {
            flag=2;                      //��¼����һ���½���
            pause=TA1CCR1;         
            pos_time=pause-start;        //�ó��������ʱ��
            TA1CCTL1=(TA1CCTL1&(~CM_2))|CM_1; //�ı䴥����ʽΪ��������
          }
        }
        break;
    }
}




