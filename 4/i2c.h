#ifndef		__I2C_H_
		#define		__I2C_H_
	#define uint8_t unsigned char 
        #define u8 unsigned char
	uint8_t OLED_Init(void);
	uint8_t OLED_Clear(void);
        uint8_t OLED_Clear_x(uint8_t x, uint8_t sw);//按列清屏或显示一列
	uint8_t OLED_SetXY(uint8_t x,uint8_t y);
	uint8_t OLED_WriteData(uint8_t Data);
	uint8_t OLED_WriteCmd(uint8_t Cmd);
/***************
功能描述：打一个点	
显示的坐标（x,y），x为0~126 y为0到63
****************/	
uint8_t OLED_PutPoint(uint8_t x,uint8_t y);	
/***************
功能描述：显示6*8一组标准ASCII字符串	
显示的坐标（x,y），y为页范围0～7
****************/
void OLED_P6x8Str(u8 x, u8 y,u8 ch[]);
/*******************
功能描述：显示8*16一组标准ASCII字符串	 
显示的坐标（x,y），y为页范围0～7
****************/
void OLED_P8x16Str(u8 x,u8 y,u8 ch[]);
/*****************
功能描述：显示16*16点阵  
显示的坐标（x,y），y为页范围0～7
****************************/
void OLED_P16x16Ch(u8 x, u8 y,u8 z);
/***********
功能描述：显示显示BMP图片128×64起始点坐标(x,y),
x的范围0～127，y为页的范围0～7
*****************/
//void Draw_BMP(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]);
        

#endif
