#include "stm32f4xx.h"
#include "delay.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_lcdinit.h"
#include "bsp_lcd.h"
#include "bsp_picture.h"
#include "bsp_touch.h"
#include "w25q128.h"
#include "stdio.h"
#include "stdlib.h"
char read[200];
int main()
{
	delay_init(168);
	
	LCD_Init();	   //液晶屏初始化
	TP_Init();
	LCD_SetBackColor(YELLOW);
	LCD_SetTextColor(PALE_VIOLET_RED);
	LCD_SetCHFont(&HUAWENCAIYUN40x32);
	LCD_SetFont(&ENYOUYUANFont8x16);
	LCD_Clear();
	LCD_DispString_CH(0,0,"我爱你小幺幺亲亲么么哒想你哒",0);
	while(1)
	{
		
	}
}


