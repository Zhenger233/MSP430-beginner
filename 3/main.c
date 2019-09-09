#include "MSP430F5529.h"
#include "IIC.c"
#include "inv_mpu.c"
#include "inv_mpu_dmp_motion_driver.c"
#include "mpu6050.c"
#include <math.h>

#define Kp 100.0f                        // ��������֧�������������ٶȼ�/��ǿ��

#define Ki 0.002f                // ��������֧���ʵ�������ƫ�����ν�

#define halfT 0.001f                // �������ڵ�һ��

 

float q0 = 1, q1 = 0, q2 = 0, q3 = 0; // ��Ԫ����Ԫ�أ�������Ʒ���
float p=1,r=1,y=1;
float exInt = 0, eyInt = 0, ezInt = 0;        // ��������С�������
short flag=0,rightdown=0;
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);

void LCD_ShowFloat(float f,unsigned char y)
{
  LCD_ShowChar(8,y,':',16);
  LCD_ShowChar(16,y,f<0?'-':' ',16); //��ʾ����
  if(f<0)f=-f;
  LCD_ShowNum(24,y,(unsigned long)f,3,16);//��ʾ��������
  LCD_ShowChar(48,y,'.',16);
  LCD_ShowNum(56,y,(unsigned long)(1000*(f-(int)f)),3,16);//��ʾС������
}

void LCD_ShowShort(short sx,short sy,short sz,unsigned char x)
{
  LCD_ShowChar(x,2,sx<0?'-':' ',16); //��ʾ����
  LCD_ShowChar(x,4,sy<0?'-':' ',16); 
  LCD_ShowChar(x,6,sz<0?'-':' ',16); 
  if(sx<0)sx=-sx;
  if(sy<0)sy=-sy;
  if(sz<0)sz=-sz;
  LCD_ShowNum(x+8,2,(unsigned long)sx,5,16);
  LCD_ShowNum(x+8,4,(unsigned long)sy,5,16);
  LCD_ShowNum(x+8,6,(unsigned long)sz,5,16);
}

void Key_Init()
{
    _EINT();//���ж�����
    
    P1REN |= BIT1; //����P1.1�ڲ�����������
    P1OUT |= BIT1; //����������Ϊ����
    P1DIR &= ~BIT1;//P1.1��Ϊ����(��ʡ��)
    P1IES |= BIT1;//P1.1��Ϊ�½����ж�
    P1IE  |= BIT1;//����P1.1�ж�
    
    P2REN |= BIT1; //����P2.1�ڲ�����������
    P2OUT |= BIT1; //����������Ϊ����
    P2DIR &= ~BIT1;//P2.1��Ϊ����(��ʡ��)
    P2IES |= BIT1;//P2.1��Ϊ�½����ж�
    P2IE  |= BIT1;//����P2.1�ж�
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    unsigned int Push_Key=0;

    Push_Key=P1IFG&(~P1DIR);
    __delay_cycles(10000);//��������
    if((P1IN&Push_Key)==0)
    {   
      flag=1;
      rightdown=1;
    }
    P1IFG=0;                 
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
    unsigned int Push_Key=0;
    short ax,ay,az,gx,gy,gz;
    //float p=1,r=1,y=1;
    Push_Key=P2IFG&(~P2DIR);
    __delay_cycles(10000);//��������
    if((P2IN&Push_Key)==0)
    {
      //mpu_dmp_get_data(&p,&r,&y);
     LCD_Init();
     flag=0;
    }
    P2IFG=0;                 
}

int main( void )
{
  WDTCTL = WDTPW+WDTHOLD;//�رտ��Ź�

  mpu_dmp_init();
  //MPU_Init();

  Key_Init();
  while(1)
  {
       if(flag==0)
       {
         short ax,ay,az,gx,gy,gz;
         // __delay_cycles(100000);
     
      MPU_Get_Accelerometer(&ax,&ay,&az);
     
      MPU_Get_Gyroscope(&gx,&gy,&gz);
     
      IMUupdate( gx, gy,  gz, ax, ay, az);
  // LCD_Init();
     
      LCD_ShowChar(0,0,'p',16);//������
     
      LCD_ShowFloat(p,0);
     
      LCD_ShowChar(0,2,'r',16);//�����
     
      LCD_ShowFloat(r,2);
     
      LCD_ShowChar(0,4,'y',16);//�����
     
      LCD_ShowFloat(y,4);
       }
       else
         if(rightdown==1)
       {
      float t;
      short ax,ay,az,gx,gy,gz;
      t=(float)MPU_Get_Temperature()/100;
      MPU_Get_Accelerometer(&ax,&ay,&az);
      MPU_Get_Gyroscope(&gx,&gy,&gz);
      LCD_Init();
      LCD_ShowChar(0,0,'T',16);//�¶�
      LCD_ShowFloat(t,0);
      LCD_ShowChar(0,2,'A',16);//���ٶ�
      LCD_ShowShort(ax,ay,az,8);
      LCD_ShowChar(64,2,'G',16);//������
      LCD_ShowShort(gx,gy,gz,72);
      rightdown=0;
       }
    }
    
}



void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)

{

        float norm;

        float vx, vy, vz;

        float ex, ey, ez;  

 

        // ����������

        norm = sqrt(ax*ax + ay*ay + az*az);      

        ax = ax / norm;                   //��λ��

        ay = ay / norm;

        az = az / norm;      

 

        // ���Ʒ��������

        vx = 2*(q1*q3 - q0*q2);

        vy = 2*(q0*q1 + q2*q3);

        vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

 

        // ���������ͷ��򴫸��������ο�����֮��Ľ���˻����ܺ�

        ex = (ay*vz - az*vy);

        ey = (az*vx - ax*vz);

        ez = (ax*vy - ay*vx);

 

        // ������������������

        exInt = exInt + ex*Ki;

        eyInt = eyInt + ey*Ki;

        ezInt = ezInt + ez*Ki;

 

        // ������������ǲ���

        gx = gx + Kp*ex + exInt;

        gy = gy + Kp*ey + eyInt;

        gz = gz + Kp*ez + ezInt;

 

        // ������Ԫ���ʺ�������

        q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;

        q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;

        q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;

        q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  

 

        // ��������Ԫ

        norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);

        q0 = q0 / norm;

        q1 = q1 / norm;

        q2 = q2 / norm;

        q3 = q3 / norm;

 

        p  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch ,ת��Ϊ����

        r = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // rollv

        y = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;                //�˴�û�м�ֵ��ע��

}



/*
#include "MSP430F5529.h"
#include "IIC.c"
#include "inv_mpu.c"
#include "inv_mpu_dmp_motion_driver.c"
#include "mpu6050.c"
#include <math.h>

#define Kp 100.0f                        // ��������֧�������������ٶȼ�/��ǿ��
#define Ki 0.002f                // ��������֧���ʵ�������ƫ�����ν�
#define halfT 0.001f                // �������ڵ�һ��

 

float q0 = 1, q1 = 0, q2 = 0, q3 = 0; // ��Ԫ����Ԫ�أ�������Ʒ���
float p=1,r=1,y=1;
float t;
float exInt = 0, eyInt = 0, ezInt = 0;  // ��������С�������
short ax,ay,az,gx,gy,gz;

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);

void LCD_ShowFloat(float f,unsigned char y)
{
  LCD_ShowChar(8,y,':',16);
  LCD_ShowChar(16,y,f<0?'-':' ',16); //��ʾ����
  if(f<0)f=-f;
  LCD_ShowNum(24,y,(unsigned long)f,3,16);//��ʾ��������
  LCD_ShowChar(48,y,'.',16);
  LCD_ShowNum(56,y,(unsigned long)(1000*(f-(int)f)),3,16);//��ʾС������
}

void LCD_ShowShort(short sx,short sy,short sz,unsigned char x)
{
  LCD_ShowChar(x,2,sx<0?'-':' ',16); //��ʾ����
  LCD_ShowChar(x,4,sy<0?'-':' ',16); 
  LCD_ShowChar(x,6,sz<0?'-':' ',16); 
  if(sx<0)sx=-sx;
  if(sy<0)sy=-sy;
  if(sz<0)sz=-sz;
  LCD_ShowNum(x+8,2,(unsigned long)sx,5,16);
  LCD_ShowNum(x+8,4,(unsigned long)sy,5,16);
  LCD_ShowNum(x+8,6,(unsigned long)sz,5,16);
}

void Key_Init()
{
    _EINT();//���ж�����
    
    P1REN |= BIT1; //����P1.1�ڲ�����������
    P1OUT |= BIT1; //����������Ϊ����
    P1DIR &= ~BIT1;//P1.1��Ϊ����(��ʡ��)
    P1IES |= BIT1;//P1.1��Ϊ�½����ж�
    P1IE  |= BIT1;//����P1.1�ж�
    
    P2REN |= BIT1; //����P2.1�ڲ�����������
    P2OUT |= BIT1; //����������Ϊ����
    P2DIR &= ~BIT1;//P2.1��Ϊ����(��ʡ��)
    P2IES |= BIT1;//P2.1��Ϊ�½����ж�
    P2IE  |= BIT1;//����P2.1�ж�
}



int main( void )
{
  WDTCTL = WDTPW+WDTHOLD;//�رտ��Ź�

  mpu_dmp_init();
  //MPU_Init();
  LCD_Init();
  Key_Init();
  while(1)
  {
  
    
      //mpu_dmp_get_data(&p,&r,&y);
      MPU_Get_Accelerometer(&ax,&ay,&az);
      MPU_Get_Gyroscope(&gx,&gy,&gz);
      IMUupdate( gx, gy,  gz, ax, ay, az);
      t=(float)MPU_Get_Temperature()/100;
      if((P2IN&P2IFG&(~P2DIR))==0)
      {
      LCD_ShowChar(0,0,'p',16);//������
      LCD_ShowFloat(p,0);
      LCD_ShowChar(0,2,'r',16);//�����
      LCD_ShowFloat(r,2);
      LCD_ShowChar(0,4,'y',16);//�����
      LCD_ShowFloat(y,4);
      }
      if((P1IN&P1IFG&(~P1DIR))==0)
      {   
        LCD_ShowChar(0,0,'T',16);//�¶�
        LCD_ShowFloat(t,0);
        LCD_ShowChar(0,2,'A',16);//���ٶ�
        LCD_ShowShort(ax,ay,az,8);
        LCD_ShowChar(64,2,'G',16);//������
        LCD_ShowShort(gx,gy,gz,72);
      }
  };
}



void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)

{

        float norm;

        float vx, vy, vz;

        float ex, ey, ez;  

 

        // ����������

        norm = sqrt(ax*ax + ay*ay + az*az);      

        ax = ax / norm;                   //��λ��

        ay = ay / norm;

        az = az / norm;      

 

        // ���Ʒ��������

        vx = 2*(q1*q3 - q0*q2);

        vy = 2*(q0*q1 + q2*q3);

        vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

 

        // ���������ͷ��򴫸��������ο�����֮��Ľ���˻����ܺ�

        ex = (ay*vz - az*vy);

        ey = (az*vx - ax*vz);

        ez = (ax*vy - ay*vx);

 

        // ������������������

        exInt = exInt + ex*Ki;

        eyInt = eyInt + ey*Ki;

        ezInt = ezInt + ez*Ki;

 

        // ������������ǲ���

        gx = gx + Kp*ex + exInt;

        gy = gy + Kp*ey + eyInt;

        gz = gz + Kp*ez + ezInt;

 

        // ������Ԫ���ʺ�������

        q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;

        q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;

        q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;

        q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  

 

        // ��������Ԫ

        norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);

        q0 = q0 / norm;

        q1 = q1 / norm;

        q2 = q2 / norm;

        q3 = q3 / norm;

 

        p  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch ,ת��Ϊ����

        r = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // rollv

        y = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;                //�˴�û�м�ֵ��ע��

}
*/
/*
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    unsigned int Push_Key=0;
    float t;
    short ax,ay,az,gx,gy,gz;
    Push_Key=P1IFG&(~P1DIR);
    __delay_cycles(10000);//��������
    if((P1IN&Push_Key)==0)
    {   
      t=(float)MPU_Get_Temperature()/100;
      MPU_Get_Accelerometer(&ax,&ay,&az);
      MPU_Get_Gyroscope(&gx,&gy,&gz);
      LCD_Init();
      LCD_ShowChar(0,0,'T',16);//�¶�
      LCD_ShowFloat(t,0);
      LCD_ShowChar(0,2,'A',16);//���ٶ�
      LCD_ShowShort(ax,ay,az,8);
      LCD_ShowChar(64,2,'G',16);//������
      LCD_ShowShort(gx,gy,gz,72);
    }
    P1IFG=0;                 
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
    unsigned int Push_Key=0;
    short ax,ay,az,gx,gy,gz;
    //float p=1,r=1,y=1;
    Push_Key=P2IFG&(~P2DIR);
    __delay_cycles(10000);//��������
    if((P2IN&Push_Key)==0)
    {
      //mpu_dmp_get_data(&p,&r,&y);
      MPU_Get_Accelerometer(&ax,&ay,&az);
      MPU_Get_Gyroscope(&gx,&gy,&gz);
      IMUupdate( gx, gy,  gz, ax, ay, az);
      LCD_Init();
      LCD_ShowChar(0,0,'p',16);//������
      LCD_ShowFloat(p,0);
      LCD_ShowChar(0,2,'r',16);//�����
      LCD_ShowFloat(r,2);
      LCD_ShowChar(0,4,'y',16);//�����
      LCD_ShowFloat(y,4);
    }
    P2IFG=0;                 
}
*/