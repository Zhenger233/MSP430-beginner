#include <msp430f5529.h>
#include "mpu6050.h"
#define Delay_1us() __delay_cycles(4)

void Delay_ms(unsigned int time)
{
  for(unsigned int i=0;i<time;i++)
    for(unsigned int j=0;j<1000;j++)
      Delay_1us();
}

unsigned char MPU_Init()
{
  unsigned char res;
  MPU_Write_Byte(MPU_PWR_MGMT1_REG,0x80);
  Delay_ms(100);
  MPU_Write_Byte(MPU_PWR_MGMT1_REG,0x00);
  MPU_Write_Byte(MPU_GYRO_CFG_REG,3<<3);
  MPU_Write_Byte(MPU_ACCEL_CFG_REG,0<<3);
  MPU_Set_Rate(50);
  MPU_Write_Byte(MPU_INT_EN_REG,0X00);
  MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);
  MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);
  MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);
  res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
  if(res==MPU_ADDR)
  {
    MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);
    MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);
    MPU_Set_Rate(50);
   }
  else return 1;
  return 0;
}

unsigned char MPU_Set_LPF(unsigned int lpf)
{
	unsigned char data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU_CFG_REG,data);
}

unsigned char MPU_Set_Rate(unsigned int rate)
{
  unsigned char data;
  if(rate>1000)rate=1000;
  if(rate<4)rate=4;
  data=1000/rate-1;
  data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	
  return MPU_Set_LPF(rate/2);	
}

unsigned char MPU_Write_Byte(unsigned char reg, unsigned char data)
{
   while( UCB0CTL1& UCTXSTP );
  UCB0CTL1 |= UCTR;                // 写模式
  UCB0CTL1 |= UCTXSTT;             // 发送启动位
 
  UCB0TXBUF = reg;                 // 发送字节地址
  while(!(UCB0IFG& UCTXIFG))
  {
    if( UCB0IFG& UCNACKIFG )      // 若无应答 UCNACKIFG=1
    {
       return 1;
    }
  }  
 
  UCB0TXBUF = data;                // 发送字节内容
  while(!(UCB0IFG& UCTXIFG));      // 等待UCTXIFG=1
 
  UCB0CTL1 |= UCTXSTP;
  while(UCB0CTL1& UCTXSTP);        // 等待发送完成
 
  return 0;
}

unsigned char MPU_Read_Byte(unsigned char reg)
{
  unsigned char res;
  UCB0CTL1 |= UCTR;                // 写模式
  UCB0CTL1 |= UCTXSTT;             // 发送启动位和写控制字节
 
  UCB0TXBUF = reg;           //发送字节地址，必须要先填充TXBUF
  while(!(UCB0IFG& UCTXIFG))
  {
    if( UCB0IFG& UCNACKIFG )       // 若无应答 UCNACKIFG=1
    {
      return 0;
    }
  }                       
 
  UCB0CTL1 &= ~UCTR;               //读模式
  UCB0CTL1 |= UCTXSTT;             // 发送启动位和读控制字节
 
  while(UCB0CTL1& UCTXSTT);        // 等待UCTXSTT=0
  // 若无应答 UCNACKIFG = 1
  UCB0CTL1 |= UCTXSTP;             // 先发送停止位
 
  while(!(UCB0IFG& UCRXIFG));      // 读取字节内容
  res = UCB0RXBUF;                 // 读取BUF寄存器在发送停止位之后
 
  while( UCB0CTL1& UCTXSTP );
 
  return res;
}

unsigned char MPU_Write_Len(unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf)
{
  while( UCB0CTL1& UCTXSTP );
  UCB0CTL1 |= UCTR;                // 写模式
  UCB0CTL1 |= UCTXSTT;             // 发送启动位
 
  UCB0TXBUF = reg;           // 发送字节地址
  // 等待UCTXIFG=1与UCTXSTT=0 同时变化等待一个标志位即可
  while(!(UCB0IFG& UCTXIFG))
  {
    if( UCB0IFG& UCNACKIFG )      // 若无应答 UCNACKIFG=1
    {
      return 1;
    }
  }  
 
  for(unsigned char i= 0; i < len; i++)
  {
    UCB0TXBUF = *buf++;      // 发送寄存器内容
    while(!(UCB0IFG& UCTXIFG));   // 等待UCTXIFG=1   
  }
 
  UCB0CTL1 |= UCTXSTP;
  while(UCB0CTL1& UCTXSTP);       // 等待发送完成
 
  return 0;
}

unsigned char MPU_Read_Len(unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf)
{
  while( UCB0CTL1& UCTXSTP );
  UCB0CTL1 |= UCTR;                // 写模式
  UCB0CTL1 |= UCTXSTT;             // 发送启动位和写控制字节
 
  UCB0TXBUF = reg;           // 发送字节地址
  // 等待UCTXIFG=1与UCTXSTT=0 同时变化等待一个标志位即可
  while(!(UCB0IFG& UCTXIFG))
  {
    if( UCB0IFG& UCNACKIFG )      // 若无应答 UCNACKIFG=1
    {
      return 1;
    }
  }  
 
  UCB0CTL1 &= ~UCTR;               // 读模式
  UCB0CTL1 |= UCTXSTT;             // 发送启动位和读控制字节
 
  while(UCB0CTL1& UCTXSTT);       // 等待UCTXSTT=0
  // 若无应答 UCNACKIFG = 1
 
  for(unsigned char i= 0; i< len -1 ; i++)
  {
    while(!(UCB0IFG& UCRXIFG));   // 读取字节内容，不包括最后一个字节内容
    *buf++= UCB0RXBUF;
  }
 
  UCB0CTL1 |= UCTXSTP;             // 在接收最后一个字节之前发送停止位
 
  while(!(UCB0IFG& UCRXIFG));     // 读取最后一个字节内容
  *buf = UCB0RXBUF;
 
  while( UCB0CTL1& UCTXSTP );
 
  return 0;
}



short MPU_Get_Temperature(void)
{
   unsigned char buf[2]; 
   short raw;
   float temp;
   MPU_Read_Len(0x68,MPU_TEMP_OUTH_REG,2,buf); 
   raw=((unsigned short)buf[0]<<8)|buf[1];  
   temp=36.53+((double)raw)/340;  
   return (short)temp*100;
}

unsigned char MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
  unsigned char buf[6],res;  
  res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
  if(res==0)
  {
    *ax=((unsigned short)buf[0]<<8)|buf[1];  
    *ay=((unsigned short)buf[2]<<8)|buf[3];  
    *az=((unsigned short)buf[4]<<8)|buf[5];
  } 	
  return res;
}

unsigned char MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    unsigned char buf[6],res;  
    res=MPU_Read_Len(0x68,MPU_GYRO_XOUTH_REG,6,buf);
    if(res==0)
    {
        *gx=((unsigned short)buf[0]<<8)|buf[1];  
        *gy=((unsigned short)buf[2]<<8)|buf[3];  
        *gz=((unsigned short)buf[4]<<8)|buf[5];
    } 	
    return res;
}


