/*
    这个液晶只需要两根线用模拟或者硬件的I2C都可以 硬件设计非常简单
*/
#include "msp430f5529.h"
#include "i2c.h"
#include "codetab.h"

#define uint8_t  unsigned char 
#define u8       unsigned char
//P6.0  SCL
//P6.1  SDA
#define SDA_INIT    P6DIR |= BIT0
#define SCL_INIT    P6DIR |= BIT1
#define SDA_1       P6OUT |=  BIT1              //SDA = 1
#define SDA_0       P6OUT &=~ BIT1              //SDA = 0
#define SCL_1       P6OUT |=  BIT0              //SCL = 1
#define SCL_0       P6OUT &=~ BIT0              //SCL = 0
#define DIR_IN      P6DIR &=~ BIT1;  SDA_1      //I/O口为输入
#define DIR_OUT     P6DIR |=  BIT1              //I/0口为输出
#define SDA_IN      ((P6IN >> 1) & 0x01)        //Read SDA
unsigned int a[50];

#define BIRGTHNESS          200   //OLED的亮度  00~255

#define OLED_WIDTH    130
#define OLED_HIGH    (8*8)

#define I2C_OLED    0x78

#define I2C_Delay()    __delay_cycles(4)//实现延时1us 
/* 见名知意 移植的时候只需要把以上的宏的具体实现方法写上去 就可以了  
下面的代码用的是普通IO口模拟I2C (速度还是可以的) 这个芯片 不用上拉电阻 好像也能行
*/  

static void I2C_Start(void)
{
          SDA_1;
          I2C_Delay();
          SCL_1;
          I2C_Delay();
          SDA_0;
          I2C_Delay();
          SCL_0;
          I2C_Delay();
}

static void I2C_Stop(void)
{
	  SDA_0;
	  I2C_Delay();
	  SCL_1;
	  I2C_Delay();
	  SDA_1;
	  I2C_Delay();
}

static uint8_t I2C_WaitAck(void)  //0:aCK     1:NoAck
{
   unsigned char i = 0;
  SCL_1;
  I2C_Delay();
  DIR_IN;
  while ((SDA_IN == 0x01 ) && (i < 255))
  {
    i++;
  }
  DIR_OUT;
  SCL_0;
  I2C_Delay();
  return 0;
}

static void I2C_SendByte(uint8_t WriteData)
{
    unsigned char i;
  for (i=0; i<8; i++)
  {
    SCL_0;
    I2C_Delay();
    if (((WriteData >> 7) & 0x01) == 0x01)
    {
      SDA_1;
    }
    else
    {
      SDA_0;
    }
    I2C_Delay();
    SCL_1;
    WriteData = WriteData << 1;
    I2C_Delay();
  }
  SCL_0;
  SDA_1;
  I2C_Delay();
  I2C_Delay();
}

uint8_t OLED_WriteCmd(uint8_t Cmd)
{
    I2C_Start();
    I2C_SendByte(I2C_OLED);
    if(I2C_WaitAck())
        return 1;
    I2C_SendByte(0x00);
    if(I2C_WaitAck())
        return 2;
    I2C_SendByte(Cmd);
    if(I2C_WaitAck())
        return 3;
    I2C_Stop();
    return 0;
}

uint8_t OLED_WriteData(uint8_t Data)
{
    I2C_Start();
    I2C_SendByte(I2C_OLED);
    if(I2C_WaitAck())
        return 1;
    I2C_SendByte(0x40);
    if(I2C_WaitAck())
        return 2;
    I2C_SendByte(Data);
    if(I2C_WaitAck())
        return 3;
    I2C_Stop();
    return 0;
}

uint8_t OLED_SetXY(uint8_t x,uint8_t y)
{
    OLED_WriteCmd(0xb0+y);//y
    OLED_WriteCmd(((0xf0&x)>>4)|0x10);//设置x高4位
    OLED_WriteCmd(((0x0f&x))|0x01);//设置x低4位
    return 0;
}

uint8_t OLED_Clear_x(uint8_t x, uint8_t sw)//按列清屏或显示一列
{
    uint8_t y;
    uint8_t display = 0;
    if(sw != 0)
      display = 0xff;
    for(y=0;y<=OLED_HIGH/8;y++)
    {
        if(OLED_SetXY(x,y))
            return 1;
        if(OLED_WriteData(display))
            return 2; 
    }
    if(OLED_SetXY(x,y))
      return 1;
    return 0;
}

uint8_t OLED_Clear(void)
{
    uint8_t x,y;
    OLED_SetXY(0,0);
    for(y=0;y<OLED_HIGH/8;y++)
    {
        if(OLED_SetXY(0,y))
            return 1;
        for(x=0;x<OLED_WIDTH;x++)
        {
            if(OLED_WriteData(0x00))
                return 2; 
        } 
    }
    return 0;
}


uint8_t OLED_PutPoint(uint8_t x,uint8_t y)
{
  if(x > 127 || y > 63)
    return 1;
  else
  {
     OLED_SetXY(x,y/8);
     OLED_WriteData(0x01<<(y%8));
  }
  return 0;
}

       
       
       
/***************功能描述：显示6*8一组标准ASCII字符串	显示的坐标（x,y），y为页范围0～7****************/
void OLED_P6x8Str(u8 x, u8 y,u8 ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>126){x=0;y++;}
		OLED_SetXY(x,y);
		for(i=0;i<6;i++)
		OLED_WriteData(F6x8[c][i]);
		x+=6;
		j++;
	}
}

/*******************功能描述：显示8*16一组标准ASCII字符串	 显示的坐标（x,y），y为页范围0～7****************/
void OLED_P8x16Str(u8 x,u8 y,u8 ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>120){x=0;y++;}
		OLED_SetXY(x,y);
		for(i=0;i<8;i++)
		OLED_WriteData(F8X16[c*16+i]);
		OLED_SetXY(x,y+1);
		for(i=0;i<8;i++)
		OLED_WriteData(F8X16[c*16+i+8]);
		x+=8;
		j++;
	}
}
/*****************功能描述：显示16*16点阵  显示的坐标（x,y），y为页范围0～7****************************/
void OLED_P16x16Ch(u8 x, u8 y,u8 z)
{
	unsigned char wm=0;
	unsigned int adder=32*z;
	OLED_SetXY(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WriteData(F16x16[adder]);
		adder += 1;
	}
	OLED_SetXY(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WriteData(F16x16[adder]);
		adder += 1;
	} 	  	
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void Draw_BMP(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetXY(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WriteData(BMP[j++]);
	    }
	}
}

uint8_t OLED_Init(void)
{
    SDA_INIT;
    SCL_INIT;
   // 延时一下 再初始化 
    I2C_Delay();    
    if(OLED_WriteCmd(0xae))
        return 1;//--turn off oled panel
    if(OLED_WriteCmd(0x00))
        return 2;//---set low column address
    if(OLED_WriteCmd(0x10))
        return 3;//---set high column address
    if(OLED_WriteCmd(0x40))
        return 4;//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    if(OLED_WriteCmd(0x81))
        return 5;//--set contrast control register
    if(OLED_WriteCmd(BIRGTHNESS))
        return 6; // Set SEG Output Current Brightness
    if(OLED_WriteCmd(0xa1))
        return 7;//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    if(OLED_WriteCmd(0xc8))
        return 8;//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    if(OLED_WriteCmd(0xa6))
        return 9;//--set normal display
    if(OLED_WriteCmd(0xa8))
        return 10;//--set multiplex ratio(1 to 64)
    if(OLED_WriteCmd(0x3f))
                return 11;//--1/64 duty
    if(OLED_WriteCmd(0xd3))
        return 12;//-set display offset        Shift Mapping RAM Counter (0x00~0x3F)
    if(OLED_WriteCmd(0x00))
        return 13;//-not offset
    if(OLED_WriteCmd(0xd5))
        return 14;//--set display clock divide ratio/oscillator frequency
    if(OLED_WriteCmd(0x80))
        return 15;//--set divide ratio, Set Clock as 100 Frames/Sec
    if(OLED_WriteCmd(0xd9))
        return 16;//--set pre-charge period
    if(OLED_WriteCmd(0xf1))
        return 17;//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    if(OLED_WriteCmd(0xda))
        return 18;//--set com pins hardware configuration
    if(OLED_WriteCmd(0x12))
        return 19;
    if(OLED_WriteCmd(0xdb))
        return 20;//--set vcomh
    if(OLED_WriteCmd(0x40))
        return 21;//Set VCOM Deselect Level
    if(OLED_WriteCmd(0x20))
        return 22;//-Set Page Addressing Mode (0x00/0x01/0x02)
    if(OLED_WriteCmd(0x02))
        return 23;//
    if(OLED_WriteCmd(0x8d))
        return 24;//--set Charge Pump enable/disable
    if(OLED_WriteCmd(0x14))
        return 25;//--set(0x10) disable
    if(OLED_WriteCmd(0xa4))
        return 26;// Disable Entire Display On (0xa4/0xa5)
    if(OLED_WriteCmd(0xa6))
        return 27;// Disable Inverse Display On (0xa6/a7) 
    if(OLED_WriteCmd(0xaf))
        return 28;//--turn on oled panel
    if(OLED_Clear())
        return 29; //初始清屏
    if(OLED_SetXY(0,0))
        return 30;
    return 0;
}
