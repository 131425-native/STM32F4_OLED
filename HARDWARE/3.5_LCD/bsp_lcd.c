#include "bsp_lcd.h"
#include "bsp_lcdinit.h"
#include "bsp_font.h"
#include "w25q128.h"
#include "string.h"
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

CHFONT *LCD_CHFonts = &YOUYUAN16x16;
ENFONT *LCD_fonts = &ENYOUYUANFont8x16;  //英文字体
static uint16_t CurrentTextColor   = BLACK;//前景色
static uint16_t CurrentBackColor   = WHITE;//背景色

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//设置光标位置 
	Lcd_WriteData_16Bit(CurrentTextColor); 
}

/*******************************************************************
 * @name       :void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
 * @date       :2018-08-09 
 * @function   :fill the specified area
 * @parameters :sx:the bebinning x coordinate of the specified area
                sy:the bebinning y coordinate of the specified area
								ex:the ending x coordinate of the specified area
								ey:the ending y coordinate of the specified area
								color:the filled color value
 * @retvalue   :None
********************************************************************/
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey)
{  	
	u16 i,j;			
	u16 width=ex-sx+1; 		//得到填充的宽度
	u16 height=ey-sy+1;		//高度
	LCD_SetWindows(sx,sy,ex,ey);//设置显示窗口
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		Lcd_WriteData_16Bit(CurrentTextColor);	//写入数据 	 
	}
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口设置为全屏
}
/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void LCD_Clear(void)
{
  unsigned int i,m;  
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);   
	LCD_CS_CLR;
	LCD_RS_SET;
	for(i=0;i<lcddev.height;i++)
	{
    for(m=0;m<lcddev.width;m++)
    {	
			Lcd_WriteData_16Bit(CurrentBackColor);
		}
	}
	 LCD_CS_SET;
} 
/*******************************************************************
 * @name       :void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
 * @date       :2018-08-09 
 * @function   :Draw a line between two points
 * @parameters :x1:the bebinning x coordinate of the line
                y1:the bebinning y coordinate of the line
								x2:the ending x coordinate of the line
								y2:the ending y coordinate of the line
 * @retvalue   :None
********************************************************************/
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 
/*****************************************************************************
 * @name       :void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
 * @date       :2018-08-09 
 * @function   :Draw a rectangle
 * @parameters :x1:the bebinning x coordinate of the rectangle
                y1:the bebinning y coordinate of the rectangle
								x2:the ending x coordinate of the rectangle
								y2:the ending y coordinate of the rectangle
 * @retvalue   :None
******************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}  
/*****************************************************************************
 * @name       :void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
 * @date       :2018-08-09 
 * @function   :Filled a rectangle
 * @parameters :x1:the bebinning x coordinate of the filled rectangle
                y1:the bebinning y coordinate of the filled rectangle
								x2:the ending x coordinate of the filled rectangle
								y2:the ending y coordinate of the filled rectangle
 * @retvalue   :None
******************************************************************************/  
void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_Fill(x1,y1,x2,y2);
}

/*****************************************************************************
 * @name       :void _draw_circle_8(int xc, int yc, int x, int y, u16 c)
 * @date       :2018-08-09 
 * @function   :8 symmetry circle drawing algorithm (internal call)
 * @parameters :xc:the x coordinate of the Circular center 
                yc:the y coordinate of the Circular center 
								x:the x coordinate relative to the Circular center 
								y:the y coordinate relative to the Circular center 
								c:the color value of the circle
 * @retvalue   :None
******************************************************************************/  
void _draw_circle_8(int xc, int yc, int x, int y)
{
	LCD_DrawPoint(xc + x, yc + y);

	LCD_DrawPoint(xc - x, yc + y);

	LCD_DrawPoint(xc + x, yc - y);

	LCD_DrawPoint(xc - x, yc - y);

	LCD_DrawPoint(xc + y, yc + x);

	LCD_DrawPoint(xc - y, yc + x);

	LCD_DrawPoint(xc + y, yc - x);

	LCD_DrawPoint(xc - y, yc - x);
}
/*****************************************************************************
 * @name       :void gui_circle(int xc, int yc,u16 c,int r, int fill)
 * @date       :2018-08-09 
 * @function   :Draw a circle of specified size at a specified location
 * @parameters :xc:the x coordinate of the Circular center 
                yc:the y coordinate of the Circular center 
								r:Circular radius
								fill:1-filling,0-no filling
 * @retvalue   :None
******************************************************************************/  
void gui_circle(int xc, int yc,int r, int fill)
{
	int x = 0, y = r, yi, d;

	d = 3 - 2 * r;


	if (fill) 
	{
		// 如果填充（画实心圆）
		while (x <= y) {
			for (yi = x; yi <= y; yi++)
				_draw_circle_8(xc, yc, x, yi);

			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	} else 
	{
		// 如果不填充（画空心圆）
		while (x <= y) {
			_draw_circle_8(xc, yc, x, y);
			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
}

/*****************************************************************************
 * @name       :void Draw_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
 * @date       :2018-08-09 
 * @function   :Draw a triangle at a specified position
 * @parameters :x0:the bebinning x coordinate of the triangular edge 
                y0:the bebinning y coordinate of the triangular edge 
								x1:the vertex x coordinate of the triangular
								y1:the vertex y coordinate of the triangular
								x2:the ending x coordinate of the triangular edge 
								y2:the ending y coordinate of the triangular edge 
 * @retvalue   :None
******************************************************************************/ 
void Draw_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
{
	LCD_DrawLine(x0,y0,x1,y1);
	LCD_DrawLine(x1,y1,x2,y2);
	LCD_DrawLine(x2,y2,x0,y0);
}

static void _swap(u16 *a, u16 *b)
{
	u16 tmp;
  tmp = *a;
	*a = *b;
	*b = tmp;
}

/*****************************************************************************
 * @name       :void Fill_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
 * @date       :2018-08-09 
 * @function   :filling a triangle at a specified position
 * @parameters :x0:the bebinning x coordinate of the triangular edge 
                y0:the bebinning y coordinate of the triangular edge 
								x1:the vertex x coordinate of the triangular
								y1:the vertex y coordinate of the triangular
								x2:the ending x coordinate of the triangular edge 
								y2:the ending y coordinate of the triangular edge 
 * @retvalue   :None
******************************************************************************/ 
void Fill_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
{
	u16 a, b, y, last;
	int dx01, dy01, dx02, dy02, dx12, dy12;
	long sa = 0;
	long sb = 0;
 	if (y0 > y1) 
	{
    _swap(&y0,&y1); 
		_swap(&x0,&x1);
 	}
 	if (y1 > y2) 
	{
    _swap(&y2,&y1); 
		_swap(&x2,&x1);
 	}
  if (y0 > y1) 
	{
    _swap(&y0,&y1); 
		_swap(&x0,&x1);
  }
	if(y0 == y2) 
	{ 
		a = b = x0;
		if(x1 < a)
    {
			a = x1;
    }
    else if(x1 > b)
    {
			b = x1;
    }
    if(x2 < a)
    {
			a = x2;
    }
		else if(x2 > b)
    {
			b = x2;
    }
		LCD_Fill(a,y0,b,y0);
    return;
	}
	dx01 = x1 - x0;
	dy01 = y1 - y0;
	dx02 = x2 - x0;
	dy02 = y2 - y0;
	dx12 = x2 - x1;
	dy12 = y2 - y1;
	
	if(y1 == y2)
	{
		last = y1; 
	}
  else
	{
		last = y1-1; 
	}
	for(y=y0; y<=last; y++) 
	{
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
    sb += dx02;
    if(a > b)
    {
			_swap(&a,&b);
		}
		LCD_Fill(a,y,b,y);
	}
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++) 
	{
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		if(a > b)
		{
			_swap(&a,&b);
		}
		LCD_Fill(a,y,b,y);
	}
}



/******************************************************************************
      函数说明：显示单个16x16汉字
      入口数据：x,y显示坐标
                c 要显示的字符
                TextColor 字的颜色
                BackColor 字的背景色
								mode  0，非叠加；1，叠加
      返回值：  无
******************************************************************************/
void LCD_ShowExChar(u16 x, u16 y,uint16_t c,u8 mode)
{
	uint8_t rowCount, bitCount;
	uint8_t ucBuffer [200];	
	u8 sizex;
	u16 TypefaceNum; //一个字符所占字节大小
	u16 x0 = x;
	sizex = LCD_fonts->Height / 2;
	TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * LCD_fonts->Height;
	
		if(LCD_fonts->Height==12)
			LCD_SetWindows(x, y,x + sizex + 1, y + LCD_fonts->Height + 1); //设置光标位置
		
		if(LCD_fonts->Height == 16 || LCD_fonts->Height == 32)
			LCD_SetWindows(x, y, x+sizex - 1, y + LCD_fonts->Height - 1); //设置光标位置
		
		if(LCD_fonts->Height == 24)
			LCD_SetWindows(x, y, x+sizex + 3, y + LCD_fonts->Height + 3); //设置光标位置
			GetGBKENCode_from_EXFlash(ucBuffer,c);
//	for(i=0;i<144;i++)
//	printf("0x%x ",ucBuffer[i]);
	for(rowCount=0;rowCount<TypefaceNum;rowCount++)
		{
			for(bitCount = 0; bitCount < 8; bitCount ++)
				{
					if(!mode)
					{
						if ( ucBuffer[rowCount] & ( 0x80 >> bitCount ) )  //高位在前 
							Lcd_WriteData_16Bit(CurrentTextColor);
						else 
							Lcd_WriteData_16Bit(CurrentBackColor);
					}
					else//叠加模式
					{
						if(ucBuffer[rowCount] & ( 0x80 >> bitCount ))	
							LCD_DrawPoint(x,y);
								x++;
						if((x - x0) == LCD_fonts->Width)
						{
							x = x0;
							y++;
							break;
						}
					}
				}
		}
	
}

/******************************************************************************
      函数说明：显示单个数码管数字
      入口数据：x,y显示坐标
                c 要显示的字符
                TextColor 字的颜色
                BackColor 字的背景色
								mode  0，非叠加；1，叠加
      返回值：  无
******************************************************************************/
void LCD_ShowExSHUMAChar(u16 x, u16 y,u8 c,u8 mode)
{
	uint8_t rowCount, bitCount;
	uint8_t ucBuffer [200];	
	u16 x0 = x;
	u16 pos,num,Width,Hight,one = 0;
	if(one == 0)
	{
		W25Q128_Init();
		one = 1;
	}
	if(LCD_fonts->ZITI == 2)
	{
		if(LCD_fonts->Width == 10)
		{
			Width = 15;
			Hight = 23;
			num = 32;
			pos = (c - '0')*num;
			
			W25Q128_Read(ucBuffer,GBKCODE_ADDRESS_SHUMA16 + pos,num);
		}
		if(LCD_fonts->Width == 12)
		{
			Width = 15;
			Hight = 22;
			num = 44;
			pos = (c - '0')*num; 
			W25Q128_Read(ucBuffer,GBKCODE_ADDRESS_SHUMA24 + pos,num);
		}
		if(LCD_fonts->Width == 16)
		{
			Width = 15;
			Hight = 34;
			num = 58;
			pos = (c - '0')*num; 
			W25Q128_Read(ucBuffer,GBKCODE_ADDRESS_SHUMA32 + pos,num);
		}
	}
	LCD_SetWindows(x,y,x+Width,y+Hight);
	
	
	for(rowCount=0;rowCount<num;rowCount++)
		{
			for(bitCount = 0; bitCount < 8; bitCount ++)
				{
					if(!mode)
					{
						if ( ucBuffer[rowCount] & ( 0x80 >> bitCount ) )  //高位在前 
							Lcd_WriteData_16Bit(CurrentTextColor);
						else 
							Lcd_WriteData_16Bit(CurrentBackColor);
					}
					else
					{
						if(ucBuffer[rowCount] & ( 0x80 >> bitCount ))	
							LCD_DrawPoint(x,y);
								x++;
						if((x - x0) == LCD_fonts->Width)
						{
							x = x0;
							y++;
							break;
						}
					}
				}
			}
	
}
/******************************************************************************
      函数说明：显示单个字符不在FLASH中
      入口数据：x,y显示坐标
                c 要显示的字符
                TextColor 字的颜色
                BackColor 字的背景色
								mode  0，非叠加；1，叠加
      返回值：  无
******************************************************************************/
void LCD_ShowChar(u16 x,u16 y, u8 num,u8 mode)
{  
    u8 temp;
    u8 pos,t;     
	num=num-' ';//得到偏移后的值
	LCD_SetWindows(x,y,x+LCD_fonts->Height/2-1,y+LCD_fonts->Height-1);//设置单个文字显示窗口
	if(!mode) //非叠加方式
	{		
		for(pos=0;pos<LCD_fonts->Height;pos++)
		{
			if(LCD_fonts->Height==12)temp=asc2_1206[num][pos];//调用1206字体
			else temp=asc2_1608[num][pos];		 //调用1608字体
			for(t=0;t<LCD_fonts->Height/2;t++)
		    {                 
		        if(temp&0x01)Lcd_WriteData_16Bit(CurrentTextColor); 
				else Lcd_WriteData_16Bit(CurrentBackColor); 
				temp>>=1; 
				
		    }
			
		}	
	}else//叠加方式
	{
		for(pos=0;pos<LCD_fonts->Height;pos++)
		{
			if(LCD_fonts->Height==12)temp=asc2_1206[num][pos];//调用1206字体
			else temp=asc2_1608[num][pos];		 //调用1608字体
			for(t=0;t<LCD_fonts->Height/2;t++)
		    {                
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//画一个点    
		        temp>>=1; 
		    }
		}
	}
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏    	   	 	  
}
/******************************************************************************
      函数说明：显示字符串不在FLASH中
      入口数据：x,y显示坐标
                c 要显示的字符
                TextColor 字的颜色
                BackColor 字的背景色
								mode  0，非叠加；1，叠加
      返回值：  无
******************************************************************************/
void LCD_ShowString(u16 x,u16 y,u8 *p,u8 mode)
{         
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {   
		if(x>(lcddev.width-1)||y>(lcddev.height-1)) 
		return;     
        LCD_ShowChar(x,y,*p,mode);
        x+=LCD_fonts->Height/2;
		if(x > 315)
		{
			x = 0;
			y = LCD_fonts->Height + y;
		}
        p++;
    }  
} 
/******************************************************************************
      函数说明：显示单个汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                TextColor 字的颜色
                BackColor 字的背景色
                ziti:  0宋体  1幼圆
								mode  0，非叠加；1，叠加
      返回值：  无
******************************************************************************/
void LCD_ShowChinese(u16 x, u16 y, uint16_t c,u8 mode)
{
	uint8_t rowCount, bitCount;
	u16 x0 = x;
	uint8_t ucBuffer[200];
	LCD_SetWindows(x, y, x + LCD_CHFonts->Width - 1, y + LCD_CHFonts->Width - 1);
	
	GetGBKCode ( ucBuffer, c );
	for(rowCount=0;rowCount<LCD_CHFonts->Width*LCD_CHFonts->Height/8;rowCount++)
		{
			for(bitCount = 0; bitCount < 8; bitCount ++)
				{
					if(!mode)
					{
						if ( ucBuffer[rowCount] & ( 0x80 >> bitCount ) )  //高位在前 
							Lcd_WriteData_16Bit(CurrentTextColor);
						else 
							Lcd_WriteData_16Bit(CurrentBackColor);
					}
					else//叠加模式
					{
						if(ucBuffer[rowCount] & ( 0x80 >> bitCount ))	LCD_DrawPoint(x, y); //画一个点
								x++;
						if((x - x0) == LCD_CHFonts->Width)
						{
							x = x0;
							y++;
							break;
						}
					}
				}
		}
}


/******************************************************************************
      函数说明：显示多个16x16汉字
      入口数据：x,y显示坐标
                *pStr 要显示的汉字
                TextColor 字的颜色
                BackColor 字的背景色
                sizey 字号
                ziti:  0宋体  1幼圆
								mode  0，非叠加；1，叠加
      返回值：  无
******************************************************************************/
void LCD_DispString_CH ( 	uint16_t usX , uint16_t usY, char * pStr,u8 mode)
{	
	uint16_t usCh;
	while( * pStr != '\0' )
	{		
		if ( ( usX - 0 + LCD_CHFonts->Width ) > LCD_W )
		{
			usX = 0;
			usY += LCD_CHFonts->Height;
		}
		
		if ( ( usY - 0 + LCD_CHFonts->Height ) > LCD_H )
		{
			usX = 0;
			usY = 0;
		}	
		
		usCh = * ( uint16_t * ) pStr;	
	  usCh = ( usCh << 8 ) + ( usCh >> 8 );

		LCD_ShowChinese( usX, usY, usCh,mode);
		
		usX +=LCD_CHFonts->Width;
		
		pStr += 2;           //一个汉字两个字节 

	}	   
	
}


/**
 * @brief  在 OLED 显示器上显示英文字符串
 * @param  line ：在特定扫描方向下字符串的起始Y坐标
  *   本参数可使用宏LINE(0)、LINE(1)等方式指定文字坐标，
  *   宏LINE(x)会根据当前选择的字体来计算Y坐标值。
	*		显示中文且使用LINE宏时，需要把英文字体设置成Font8x16
 * @param  pStr ：要显示的英文字符串的首地址
 * @param  mode:  0非叠加模式  1叠加模式
 * @retval 无
 */
void LCD_DispStringLine_EN (  uint16_t line,  char * pStr,uint16_t mode)
{
	uint16_t usX = 0;
	
	while ( * pStr != '\0' )
	{
		if ( ( usX - 0 + LCD_fonts->Width ) > LCD_W )
		{
			usX = 0;
			line += LCD_fonts->Height;
		}
		
		if ( ( line - 0 + LCD_fonts->Height ) > LCD_H )
		{
			usX = 0;
			line = 0;
		}
		if(LCD_fonts->ZITI == 2)
			LCD_ShowExSHUMAChar(usX,line,*pStr,mode);
		else
			LCD_ShowChar( usX, line, * pStr,mode);
		
		pStr ++;
		
		usX += LCD_fonts->Width;
		
	}
	
}
/**
 * @brief  在 OLED 显示器上显示英文字符串(沿Y轴方向)
 * @param  usX ：在特定扫描方向下字符的起始X坐标
 * @param  usY ：在特定扫描方向下字符的起始Y坐标
 * @param  pStr ：要显示的英文字符串的首地址
 * @param  mode ：mode:  0非叠加模式  1叠加模式
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void LCD_DispString_EN_YDir (	 uint16_t usX,uint16_t usY ,  char * pStr,uint16_t mode )
{	
	while ( * pStr != '\0' )
	{
		
		if ( ( usY - 0 + LCD_fonts->Height ) >LCD_H  )
		{
			usY = 0;
			usX += LCD_fonts->Width;
		}
		
		if ( ( usX - 0 + LCD_fonts->Width ) >  LCD_W)
		{
			usX = 0;
			usY = 0;
		}
		if(LCD_fonts->ZITI == 2)
			LCD_ShowExSHUMAChar(usX,usY,*pStr,mode);
		else
			LCD_ShowChar ( usX, usY, * pStr,mode);
		
		pStr ++;
		
		usY += LCD_fonts->Height;		
	}	
}
/**
 * @brief  在 OLED 显示器上显示中英文字符串
 * @param  line ：在特定扫描方向下字符串的起始Y坐标
  *   本参数可使用宏LINE(0)、LINE(1)等方式指定文字坐标，
  *   宏LINE(x)会根据当前选择的字体来计算Y坐标值。
	*		显示中文且使用LINE宏时，需要把英文字体设置成Font8x16
 * @param  pStr ：要显示的字符串的首地址
 * @param  mode ：mode:  0非叠加模式  1叠加模式
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void LCD_DispStringLine_EN_CH (  uint16_t line, char * pStr , u8 mode)
{
	uint16_t usCh;
	uint16_t usX = 0;
	
	while( * pStr != '\0' )
	{
		if ( * pStr <= 126 )	           	//英文字符
		{
			if ( ( usX - 0 + LCD_fonts->Width ) >LCD_W )
			{
				usX = 0;
				line += LCD_fonts->Height;
			}
			
			if ( ( line - 0 + LCD_fonts->Height ) > LCD_H )
			{
				usX = 0;
				line = 0;
			}			
		
			if(LCD_fonts->ZITI == 2)
				LCD_ShowExSHUMAChar(usX,line,*pStr,mode);
			else
				LCD_ShowChar ( usX, line, * pStr,mode);
			
			usX +=  LCD_fonts->Width;
		
		  pStr ++;

		}
		
		else	                            //汉字字符
		{
			if ( ( usX - 0 + LCD_CHFonts->Width ) > LCD_W )
			{
				usX = 0;
				line += LCD_CHFonts->Height;
			}
			
			if ( ( line - 0 + LCD_CHFonts->Height ) > LCD_H )
			{
				usX = 0;
				line = 0;
			}	
			
			usCh = * ( uint16_t * ) pStr;	
			
			usCh = ( usCh << 8 ) + ( usCh >> 8 );		

			LCD_ShowChinese( usX, line, usCh,mode);
			
			usX += LCD_CHFonts->Width;
			
			pStr += 2;           //一个汉字两个字节 
		
    }
		
  }	
} 
/******************************************************************************
      函数说明：显示中英文字符串
      入口数据：x,y显示坐标
                num 要显示的字符
                TextColor 字的颜色
                BackColor 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
								ziti:  0宋体  1幼圆
      返回值：  无
******************************************************************************/
void LCD_ShowString_CH_EN(u16 x, u16 y, u8 *pStr, u8 mode)
{
	uint16_t usCh;
	while( * pStr != '\0' )
	{
		if ( * pStr <= 126 )	           	//英文字符
		{
			if(LCD_fonts->ZITI == 2)
				LCD_ShowExSHUMAChar(x,y,*pStr,mode);
			else
				LCD_ShowChar(x, y, *pStr, mode);
			x += LCD_fonts->Height /2;
			if ( ( x+LCD_fonts->Height /2) > LCD_W )
			{
				x = 0;
				y += LCD_fonts->Height;
			}
		
			if ( ( y - 0 + LCD_fonts->Height ) > LCD_H )
			{
				x = 0;
				y = 0;
			}	
				pStr++;

		}
		
		else	                            //汉字字符
		{
			if ( ( x - 0 +LCD_CHFonts->Width ) > LCD_W )
			{
				x = 0;
				y += LCD_CHFonts->Height;
			}
			
			if ( ( y - 0 + LCD_CHFonts->Height) > LCD_H )
			{
				x = 0;
				y = 0;
			}	
			
			usCh = * ( uint16_t * ) pStr;	
			
			usCh = ( usCh << 8 ) + ( usCh >> 8 );		

			LCD_ShowChinese( x, y, usCh,mode);
			
			x += LCD_CHFonts->Width;
			
			pStr += 2;           //一个汉字两个字节 
		
    }
		
  }	
}

/**
 * @brief  在 LCD 显示器上显示中英文字符串(沿Y轴方向)
 * @param  usX ：在特定扫描方向下字符的起始X坐标
 * @param  usY ：在特定扫描方向下字符的起始Y坐标
 * @param  pStr ：要显示的中英文字符串的首地址
 * @param	 mode:  0非叠加模式  1叠加模式
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void LCD_DispString_EN_CH_YDir (  uint16_t usX,uint16_t usY , char * pStr, u8 mode)
{
	uint16_t usCh;
	while( * pStr != '\0' )
	{			
			//统一使用汉字的宽高来计算换行
			if ( ( usY - 0 + LCD_CHFonts->Height ) >LCD_H  )
			{
				usY = 0;
				usX += LCD_CHFonts->Width;
			}			
			if ( ( usX - 0 + LCD_CHFonts->Width ) >  LCD_W)
			{
				usX = 0;
				usY = 0;
			}
			
		//显示	
		if ( * pStr <= 126 )	           	//英文字符
		{			
			if(LCD_fonts->ZITI == 2)
				LCD_ShowExSHUMAChar(usX,usY,*pStr,mode);
			else
				LCD_ShowChar ( usX, usY, * pStr,mode);
			
			pStr ++;
			
			usY += LCD_fonts->Height;		
		}
		else	                            //汉字字符
		{			
			usCh = * ( uint16_t * ) pStr;	
			
			usCh = ( usCh << 8 ) + ( usCh >> 8 );		

			LCD_ShowChinese( usX, usY, usCh,mode);
			
			usY += LCD_CHFonts->Height;
			
			pStr += 2;           //一个汉字两个字节 
		
    }
		
  }	
} 


/*****************************************************************************
 * @name       :void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
 * @date       :2018-08-09 
 * @function   :Centered display of English and Chinese strings
 * @parameters :x:the bebinning x coordinate of the Chinese and English strings
                y:the bebinning y coordinate of the Chinese and English strings
								fc:the color value of Chinese and English strings
								bc:the background color of Chinese and English strings
								str:the start address of the Chinese and English strings
								size:the size of Chinese and English strings
								mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/ 
void LCD_StrCenter(u16 x, u16 y, u8 *str,u8 mode)
{
	u16 len=strlen((const char *)str);
	u16 x1=(lcddev.width-len*8)/2;
	LCD_ShowString_CH_EN(x1,y,str,mode);
} 
 
/*****************************************************************************
 * @name       :void Gui_Drawbmp16(u16 x,u16 y,const unsigned char *p)
 * @date       :2018-08-09 
 * @function   :Display a 16-bit BMP image
 * @parameters :x:the bebinning x coordinate of the BMP image
                y:the bebinning y coordinate of the BMP image
								p:the start address of image array
 * @retvalue   :None
******************************************************************************/ 
void LCD_DrawPicture(u16 x,u16 y,u16 width,u16 hight,const unsigned char *p) //显示图片
{
  	int i; 
	unsigned char picH,picL; 
	LCD_SetWindows(x,y,x+width-1,y+hight-1);//窗口设置
    for(i=0;i<width*hight;i++)
	{	
	 	picL=*(p+i*2);	//数据低位在前
		picH=*(p+i*2+1);				
		Lcd_WriteData_16Bit(picH<<8|picL);  						
	}	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复显示窗口为全屏	
}
/*使用FLASH字模*/
//字模GB2312_H1616配套的函数

//中文字库存储在FLASH的起始地址 ：
//GBKCODE_START_ADDRESS 在fonts.h文件定义
/**
  * @brief  获取FLASH字符显示字库数据
	* @param  pBuffer:存储字库矩阵的缓冲区
	* @param  c ： 要获取的字符
  * @retval None.
  */
int GetGBKENCode_from_EXFlash( uint8_t * pBuffer, uint16_t c)
{
		unsigned int pos;
		static uint8_t everRead=0;
		u16 zifu;
		u16 TypefaceNum; //一个字符所占字节大小
		u8 sizex;
	/*第一次使用，初始化FLASH*/
		if(everRead == 0)
		{
			
			W25Q128_Init();
			everRead = 1;
		}
		sizex = LCD_fonts->Height / 2;
		TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * LCD_fonts->Height;
		zifu=c - ' ';
		pos=zifu * TypefaceNum;
		if(LCD_fonts->ZITI == 0)
		{
			if(LCD_fonts->Height == 12)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANEN12+pos,TypefaceNum); //读取字库数据 
			else if(LCD_fonts->Height == 16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANEN16+pos,TypefaceNum); //读取字库数据 
			else if(LCD_fonts->Height == 24)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANEN24+pos,TypefaceNum); //读取字库数据 
			else
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANEN32+pos,TypefaceNum); //读取字库数据
		}
		else if(LCD_fonts->ZITI == 1)
		{
			if(LCD_fonts->Height == 12)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SONGTIEN12+pos,TypefaceNum); //读取字库数据 
			else if(LCD_fonts->Height == 16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SONGTIEN16+pos,TypefaceNum); //读取字库数据 
			else if(LCD_fonts->Height == 24)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SONGTIEN24+pos,TypefaceNum); //读取字库数据 
			else
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SONGTIEN32+pos,TypefaceNum); //读取字库数据
		}
		else if(LCD_fonts->ZITI == 3)
		{
			if(LCD_fonts->Height == 12)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SHUMA2_12+pos,TypefaceNum); //读取字库数据 
			else if(LCD_fonts->Height == 16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SHUMA2_16+pos,TypefaceNum); //读取字库数据 
			else if(LCD_fonts->Height == 24)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SHUMA2_24+pos,TypefaceNum); //读取字库数据 
			else
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SHUMA2_32+pos,TypefaceNum); //读取字库数据
		}
		return 0;
}
/*使用FLASH字模*/
//字模GB2312_H1616配套的函数

//中文字库存储在FLASH的起始地址 ：
//GBKCODE_START_ADDRESS 在fonts.h文件定义
/**
  * @brief  获取FLASH中文显示字库数据
	* @param  pBuffer:存储字库矩阵的缓冲区
	* @param  c ： 要获取的文字
  * @retval None.
  */
int GetGBKCode_from_EXFlash( uint8_t * pBuffer, uint16_t c)
{ 
    unsigned char High8bit,Low8bit;
    unsigned int pos;
		static uint8_t everRead=0;
		/*第一次使用，初始化FLASH*/
		if(everRead == 0)
		{
			
			W25Q128_Init();
			everRead = 1;
		}
	
	  High8bit= c >> 8;     /* 取高8位数据 */
    Low8bit= c & 0x00FF;  /* 取低8位数据 */		
	  	
		/*GB2312 公式*/
		
    pos = ((High8bit-0xa1)*94+Low8bit-0xa1)*LCD_CHFonts->Width*LCD_CHFonts->Height/8;
		if(LCD_CHFonts->ZITI==0)
		{
			if(LCD_CHFonts->Width==16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANCH16+pos,LCD_CHFonts->Width*LCD_CHFonts->Height/8); //读取字库数据 
			else if(LCD_CHFonts->Width==24)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANCH24+pos,LCD_CHFonts->Width*LCD_CHFonts->Height/8); //读取字库数据 
			else 
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANCH32+pos,LCD_CHFonts->Width*LCD_CHFonts->Height/8); //读取字库数据 
		}			
		if(LCD_CHFonts->ZITI==1)
		{
			if(LCD_CHFonts->Width==16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_XINSONG16+pos,LCD_CHFonts->Width*LCD_CHFonts->Height/8); //读取字库数据 
			else if(LCD_CHFonts->Width==24)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_XINSONG24+pos,LCD_CHFonts->Width*LCD_CHFonts->Height/8); //读取字库数据 
			else 
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_XINSONG32+pos,LCD_CHFonts->Width*LCD_CHFonts->Height/8); //读取字库数据 
		}
		if(LCD_CHFonts->ZITI==2)
		{
			if(LCD_CHFonts->Width==16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_KAITI16+pos,LCD_CHFonts->Width*LCD_CHFonts->Height/8); //读取字库数据 
			else if(LCD_CHFonts->Width==24)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_KAITI24+pos,LCD_CHFonts->Width*LCD_CHFonts->Height/8); //读取字库数据 
			else 
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_KAITI32+pos,LCD_CHFonts->Width*LCD_CHFonts->Height/8); //读取字库数据 
		}
		if(LCD_CHFonts->ZITI==3)
		{
			if(LCD_CHFonts->Width==16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_FANGSONG16+pos,LCD_CHFonts->Width*LCD_CHFonts->Height/8); //读取字库数据 
			else if(LCD_CHFonts->Width==24)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_FANGSONG24+pos,LCD_CHFonts->Width*LCD_CHFonts->Height/8); //读取字库数据 
			else 
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_FANGSONG32+pos,LCD_CHFonts->Width*LCD_CHFonts->Height/8); //读取字库数据 
		}
		if(LCD_CHFonts->ZITI==4)
		{
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_HUAWENXINWEI32+pos,LCD_CHFonts->Width*LCD_CHFonts->Height/8); //读取字库数据 
		}
		if(LCD_CHFonts->ZITI==5)
		{
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_HUAWENCAIYUN32+pos,LCD_CHFonts->Width*LCD_CHFonts->Height/8); //读取字库数据 
		}
		return 0;  
     
}

/**
  * @brief  设置英文字体类型
  * @param  fonts: 指定要选择的字体
	*		参数为以下值之一
  * 	@arg：&ENYOUYUANFont6x12;
  * 	@arg：&ENYOUYUANFont8x16;
  * 	@arg：&ENYOUYUANFont12x24;
  * 	@arg：&ENYOUYUANFont16x32;
  * @retval None
  */
void LCD_SetFont(ENFONT *fonts)
{
  LCD_fonts = fonts;
}

/**
  * @brief  获取当前字体类型
  * @param  None.
  * @retval 返回当前字体类型
  */
ENFONT *LCD_GetFont(void)
{
  return LCD_fonts;
}
/**
  * @brief  设置汉字字体类型
  * @param  fonts: 指定要选择的字体
	*		参数为以下值之一
  * 	@arg：&CHFONT YOUYUAN16x16;
  * 	@arg：&CHFONT YOUYUAN24x24;
  * 	@arg：&CHFONT YOUYUAN32x32;
  * 	@arg：&CHFONT XINSONG16x16;
  * 	@arg：&CHFONT XINSONG24x24;
  * 	@arg：&CHFONT XINSONG32x32;
  * 	@arg：&CHFONT HEITI16x16;
  * 	@arg：&CHFONT HEITI24x24;
  * 	@arg：&CHFONT HEITI32x32;
  * 	@arg：&CHFONT LISHU24x24;
  * 	@arg：&CHFONT LISHU32x32;
  * 	@arg：&CHFONT KAITI24x24;
  * 	@arg：&CHFONT KAITI32x32;
  * 	@arg：&CHFONT FANGZHENGYAOTI32x32;
  * 	@arg：&CHFONT FANGZHENGSHUTI32x32;
  * @retval None
  */

void LCD_SetCHFont(CHFONT *fonts)
{
	LCD_CHFonts = fonts;
}
/**
  * @brief  获取当前字体类型
  * @param  None.
  * @retval 返回当前字体类型
  */
CHFONT *LCD_GetCHFont(void)
{
  return LCD_CHFonts;
}
/**
  * @brief  设置LCD的前景(字体)及背景颜色,RGB565
  * @param  TextColor: 指定前景(字体)颜色
  * @param  BackColor: 指定背景颜色
  * @retval None
  */
void LCD_SetColors(uint16_t TextColor, uint16_t BackColor) 
{
  CurrentTextColor = TextColor; 
  CurrentBackColor = BackColor;
}

/**
  * @brief  获取LCD的前景(字体)及背景颜色,RGB565
  * @param  TextColor: 用来存储前景(字体)颜色的指针变量
  * @param  BackColor: 用来存储背景颜色的指针变量
  * @retval None
  */
void LCD_GetColors(uint16_t *TextColor, uint16_t *BackColor)
{
  *TextColor = CurrentTextColor;
  *BackColor = CurrentBackColor;
}

/**
  * @brief  设置LCD的前景(字体)颜色,RGB565
  * @param  Color: 指定前景(字体)颜色 
  * @retval None
  */
void LCD_SetTextColor(uint16_t Color)
{
  CurrentTextColor = Color;
}

/**
  * @brief  设置LCD的背景颜色,RGB565
  * @param  Color: 指定背景颜色 
  * @retval None
  */
void LCD_SetBackColor(uint16_t Color)
{
  CurrentBackColor = Color;
}

