#ifndef _BSP_LCD_H_
#define _BSP_LCD_H_

#include "stm32f4xx.h"
#include "bsp_font.h"

//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	 wramcmd;		//开始写gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
/////////////////////////////////////用户配置区///////////////////////////////////	 
#define USE_HORIZONTAL  	 0//定义液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转

//////////////////////////////////////////////////////////////////////////////////	  
//定义LCD的尺寸
#define LCD_W 320
#define LCD_H 480

#define LINE(x) ((x) * (40))
#define SWAP(x, y) \
	(y) = (x) + (y); \
	(x) = (y) - (x); \
	(y) = (y) - (x);
//LINEY统一使用汉字字模的高度
#define LINEY(x) ((x) * (WIDTH_CH_CHAR))

void LCD_DrawPoint(u16 x,u16 y);
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey);
void LCD_Clear(void);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void _draw_circle_8(int xc, int yc, int x, int y);
void gui_circle(int xc, int yc,int r, int fill);
void Draw_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
void Fill_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
void LCD_ShowExChar(u16 x, u16 y,uint16_t c,u8 mode);
void LCD_ShowExSHUMAChar(u16 x, u16 y,u8 c,u8 mode);
void LCD_ShowChar(u16 x,u16 y, u8 num,u8 mode);
void LCD_ShowString(u16 x,u16 y,u8 *p,u8 mode);
void LCD_ShowChinese(u16 x, u16 y, uint16_t c,u8 mode);
void LCD_DispString_CH ( 	uint16_t usX , uint16_t usY, char * pStr,u8 mode);
void LCD_DispStringLine_EN (  uint16_t line,  char * pStr,uint16_t mode);
void LCD_DispString_EN_YDir (	 uint16_t usX,uint16_t usY ,  char * pStr,uint16_t mode );
void LCD_DispStringLine_EN_CH (  uint16_t line, char * pStr , u8 mode);
void LCD_ShowString_CH_EN(u16 x, u16 y, u8 *pStr, u8 mode);
void LCD_DispString_EN_CH_YDir (  uint16_t usX,uint16_t usY , char * pStr, u8 mode);
void LCD_StrCenter(u16 x, u16 y, u8 *str,u8 mode);
void LCD_DrawPicture(u16 x,u16 y,u16 width,u16 hight,const unsigned char *p); //显示图片


void LCD_SetFont(ENFONT *fonts);
ENFONT *LCD_GetFont(void);
void LCD_SetColors(uint16_t TextColor, uint16_t BackColor);
void LCD_GetColors(uint16_t *TextColor, uint16_t *BackColor);
void LCD_SetTextColor(uint16_t Color);
void LCD_SetBackColor(uint16_t Color);
void LCD_SetCHFont(CHFONT *fonts);
CHFONT *LCD_GetCHFont(void);

/*获取字库的函数*/
	//定义获取中文字符字模数组的函数名，ucBuffer为存放字模数组名，usChar为中文字符（国标码）
#define      GetGBKCode( ucBuffer, usChar )  GetGBKCode_from_EXFlash( ucBuffer, usChar )  
#define 		 GetGBKENCode( ucBuffer, usChar )		GetGBKENCode_from_EXFlash( uint8_t * pBuffer, uint16_t c)
int GetGBKCode_from_EXFlash( uint8_t * pBuffer, uint16_t c);
int GetGBKENCode_from_EXFlash( uint8_t * pBuffer, uint16_t c);

/*--------------------宋体字符地址----------------------------*/
#define GBKCODE_ADDRESS_SONGTIEN12				 0x895212//12
#define GBKCODE_ADDRESS_SONGTIEN16				 0x895692//16
#define GBKCODE_ADDRESS_SONGTIEN24				 0x895c92//24
#define GBKCODE_ADDRESS_SONGTIEN32				 0x896e92//32

/*-----------Conslor字符地址----------------------------------*/
#define GBKCODE_ADDRESS_YOUYUANEN12				 0x100001//12
#define GBKCODE_ADDRESS_YOUYUANEN16				 0x100481//16
#define GBKCODE_ADDRESS_YOUYUANEN24				 0x100a81//24
#define GBKCODE_ADDRESS_YOUYUANEN32				 0x101c81//32

/*------------数码字符地址-------------------------------------*/
#define GBKCODE_ADDRESS_SHUMA16				 		 0x89cfd6//16
#define GBKCODE_ADDRESS_SHUMA24						 0x89d116//24
#define GBKCODE_ADDRESS_SHUMA32						 0x89d2ce//32

/*-----------数码2字符地址-------------------------------------*/
#define GBKCODE_ADDRESS_SHUMA2_12					 0x89d515//12
#define GBKCODE_ADDRESS_SHUMA2_16					 0x89e667//16
#define GBKCODE_ADDRESS_SHUMA2_24					 0x8a1667//24
#define GBKCODE_ADDRESS_SHUMA2_32					 0x8a2867//32


/*---------------------------------------------------------------------*/
#define	GBKCODE_ADDRESS_YOUYUANCH16  			 0x103481//16
#define	GBKCODE_ADDRESS_YOUYUANCH24  			 0x1432c3//24
#define	GBKCODE_ADDRESS_YOUYUANCH32 			 0x1d2ed5//32

#define	GBKCODE_ADDRESS_XINSONG16 				 0x2d27d7//16
#define	GBKCODE_ADDRESS_XINSONG24 				 0x312619//24
#define	GBKCODE_ADDRESS_XINSONG32 				 0x3a222b//32

#define GBKCODE_ADDRESS_KAITI16						 0x4a1b2d//16
#define	GBKCODE_ADDRESS_KAITI24 					 0x4e196f//24
#define	GBKCODE_ADDRESS_KAITI32 					 0x571581//32

#define GBKCODE_ADDRESS_FANGSONG16				 0x670e83//16
#define	GBKCODE_ADDRESS_FANGSONG24  			 0x6b0cc5//24
#define	GBKCODE_ADDRESS_FANGSONG32  			 0x7408d7//32

#define GBKCODE_ADDRESS_HUAWENXINWEI32  	 0xabf05d//32

#define GBKCODE_ADDRESS_HUAWENCAIYUN32  	 0x97f91b//32

/*note：0x894000到0x97f91b相当于941KB可以用0-1024*1024 相当于1M可以用*/

#endif /*_BSP_LCD_H_*/

