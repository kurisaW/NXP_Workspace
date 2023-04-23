#ifndef __OLED_H
#define __OLED_H			  	 
#include <rtthread.h>
#include <rtdevice.h>
#include "stdlib.h"
#include "pin_mux.h"

#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64

#define CS_PIN 		56		//spi片选	 CS
#define RES_PIN   40		//oled复位 RES
#define DC_PIN		93		//spi数据/命令选择脚 DC	
#define SCK_PIN		10		//spi时钟线 D0						clock
#define DIN_PIN		22		//spi数据线 D1						data

//#define WR_PIN		17
//#define RD_PIN		18

//-----------------OLED端口定义----------------  					   
#define OLED_CS_Clr()  rt_pin_write(CS_PIN,PIN_LOW)//CS
#define OLED_CS_Set()  rt_pin_write(CS_PIN,PIN_HIGH)

#define OLED_RST_Clr() rt_pin_write(RES_PIN,PIN_LOW)//RES
#define OLED_RST_Set() rt_pin_write(RES_PIN,PIN_HIGH)

#define OLED_DC_Clr() rt_pin_write(DC_PIN,PIN_LOW)//DC
#define OLED_DC_Set() rt_pin_write(DC_PIN,PIN_HIGH)

//#define OLED_WR_Clr() rt_pin_write(WR_PIN,PIN_LOW)
//#define OLED_WR_Set() rt_pin_write(WR_PIN,PIN_HIGH)

//#define OLED_RD_Clr() rt_pin_write(RD_PIN,PIN_LOW)
//#define OLED_RD_Set() rt_pin_write(RD_PIN,PIN_HIGH)



//使用4线串行接口时使用 

#define OLED_SCLK_Clr() rt_pin_write(SCK_PIN,PIN_LOW)//CLK
#define OLED_SCLK_Set() rt_pin_write(SCK_PIN,PIN_HIGH)

#define OLED_SDIN_Clr() rt_pin_write(DIN_PIN,PIN_LOW)//DIN
#define OLED_SDIN_Set() rt_pin_write(DIN_PIN,PIN_HIGH)

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(rt_uint8_t dat,rt_uint8_t cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(rt_uint8_t x,rt_uint8_t y,rt_uint8_t t);
void OLED_Fill(rt_uint8_t x1,rt_uint8_t y1,rt_uint8_t x2,rt_uint8_t y2,rt_uint8_t dot);
void OLED_ShowChar(rt_uint8_t x,rt_uint8_t y,rt_uint8_t chr);
void OLED_ShowNum(rt_uint8_t x,rt_uint8_t y,rt_uint32_t num,rt_uint8_t len,rt_uint8_t size);
void OLED_ShowString(rt_uint8_t x,rt_uint8_t y, rt_uint8_t *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(rt_uint8_t x,rt_uint8_t y,rt_uint8_t no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif  
	 



