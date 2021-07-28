#ifndef __OLED_H
#define __OLED_H 

#include "sys.h"
#include "stdlib.h"	
#include "bsp_font.h"

#define u8 unsigned char
#define u32 unsigned int

void OLED_ClearPoint(u8 x,u8 y);
void OLED_ColorTurn(u8 i);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2);
void OLED_DrawCircle(u8 x,u8 y,u8 r);
void OLED_ShowExChar(u16 x, u16 y,uint16_t c);
void OLED_ShowString(u8 x,u8 y,u8 *chr);
void OLED_ShowChinese(u16 x, u16 y, uint16_t c);
void OLED_DispString_CH ( 	uint16_t usX , uint16_t usY, char * pStr);
void OLED_DispStringLine_EN (  uint16_t line,  char * pStr);
void OLED_DispString_EN_YDir (	 uint16_t usX,uint16_t usY ,  char * pStr);
void OLED_DispStringLine_EN_CH (  uint16_t line, char * pStr);
void OLED_ShowString_CH_EN(u16 x, u16 y, char *pStr);
void OLED_DispString_EN_CH_YDir (  uint16_t usX,uint16_t usY , char * pStr);
void OLED_ScrollDisplay(u8 num,u8 space);
void OLED_WR_BP(u8 x,u8 y);
void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]);


void OLED_SetFont(ENFONT *fonts);
ENFONT *OLED_GetFont(void);
void OLED_SetCHFont(CHFONT *fonts);
CHFONT *OLED_GetCHFont(void);
#endif


