#ifndef		__I2C_H_
		#define		__I2C_H_
	#define uint8_t unsigned char 
        #define u8 unsigned char
	uint8_t OLED_Init(void);
	uint8_t OLED_Clear(void);
        uint8_t OLED_Clear_x(uint8_t x, uint8_t sw);//������������ʾһ��
	uint8_t OLED_SetXY(uint8_t x,uint8_t y);
	uint8_t OLED_WriteData(uint8_t Data);
	uint8_t OLED_WriteCmd(uint8_t Cmd);
/***************
������������һ����	
��ʾ�����꣨x,y����xΪ0~126 yΪ0��63
****************/	
uint8_t OLED_PutPoint(uint8_t x,uint8_t y);	
/***************
������������ʾ6*8һ���׼ASCII�ַ���	
��ʾ�����꣨x,y����yΪҳ��Χ0��7
****************/
void OLED_P6x8Str(u8 x, u8 y,u8 ch[]);
/*******************
������������ʾ8*16һ���׼ASCII�ַ���	 
��ʾ�����꣨x,y����yΪҳ��Χ0��7
****************/
void OLED_P8x16Str(u8 x,u8 y,u8 ch[]);
/*****************
������������ʾ16*16����  
��ʾ�����꣨x,y����yΪҳ��Χ0��7
****************************/
void OLED_P16x16Ch(u8 x, u8 y,u8 z);
/***********
������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),
x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7
*****************/
//void Draw_BMP(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]);
        

#endif
