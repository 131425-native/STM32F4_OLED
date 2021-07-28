#include "bsp_oled.h"
#include "stdlib.h"
#include "bsp_oledinit.h"
#include "delay.h"
CHFONT *OLED_CHFonts = &YOUYUAN16x16;
ENFONT *OLED_fonts = &ENXINSONGFont8x16;  //英文字体

u8 OLED_GRAM[144][8];

//反显函数
void OLED_ColorTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
		}
	if(i==1)
		{
			OLED_WR_Byte(0xA7,OLED_CMD);//反色显示
		}
}

//屏幕旋转180度
void OLED_DisplayTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xC8,OLED_CMD);//正常显示
			OLED_WR_Byte(0xA1,OLED_CMD);
		}
	if(i==1)
		{
			OLED_WR_Byte(0xC0,OLED_CMD);//反转显示
			OLED_WR_Byte(0xA0,OLED_CMD);
		}
}
//更新显存到OLED	
void OLED_Refresh(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   OLED_WR_Byte(0xb0+i,OLED_CMD); //设置行起始地址
	   OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
	   OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
	   for(n=0;n<128;n++)
		 OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
  }
}
void Set_OLED_Address(u16 x1, u16 y1, u16 x2, u16 y2)
{
	OLED_WR_Byte(0xb0,OLED_CMD);
	OLED_WR_Byte(x1,OLED_CMD);
	OLED_WR_Byte(x2,OLED_CMD);
	OLED_WR_Byte(0x00,OLED_CMD);
	OLED_WR_Byte(y1,OLED_CMD);
	OLED_WR_Byte(y2,OLED_CMD);
}
//清屏函数
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   for(n=0;n<128;n++)
			{
			 OLED_GRAM[n][i]=0;//清除所有数据
			}
  }
	OLED_Refresh();//更新显示
}

//画点 
//x:0~127
//y:0~63
void OLED_DrawPoint(u8 x,u8 y)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	OLED_GRAM[x][i]|=n;
}

//清除一个点
//x:0~127
//y:0~63
void OLED_ClearPoint(u8 x,u8 y)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	OLED_GRAM[x][i]|=n;
	OLED_GRAM[x][i]=~OLED_GRAM[x][i];
}


//画线
//x:0~128
//y:0~64
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2)
{
	u8 i,k,k1,k2;
	if(x1==x2)    //画竖线
	{
			for(i=0;i<(y2-y1);i++)
			{
				OLED_DrawPoint(x1,y1+i);
			}
  }
	else if(y1==y2)   //画横线
	{
			for(i=0;i<(x2-x1);i++)
			{
				OLED_DrawPoint(x1+i,y1);
			}
  }
	else      //画斜线
	{
		k1=y2-y1;
		k2=x2-x1;
		k=k1*10/k2;
		for(i=0;i<(x2-x1);i++)
			{
			  OLED_DrawPoint(x1+i,y1+i*k/10);
			}
	}
}
//x,y:圆心坐标
//r:圆的半径
void OLED_DrawCircle(u8 x,u8 y,u8 r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED_DrawPoint(x + a, y - b);
        OLED_DrawPoint(x - a, y - b);
        OLED_DrawPoint(x - a, y + b);
        OLED_DrawPoint(x + a, y + b);
 
        OLED_DrawPoint(x + b, y + a);
        OLED_DrawPoint(x + b, y - a);
        OLED_DrawPoint(x - b, y - a);
        OLED_DrawPoint(x - b, y + a);
        
        a++;
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}

/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fc 字的颜色
                bc 字的背景色
                OLED_fonts->Height 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void OLED_ShowExChar(u16 x, u16 y,uint16_t c)
{
	uint8_t rowCount, bitCount;
	uint8_t ucBuffer [200];	
	u8 sizex;
	u16 TypefaceNum; //一个字符所占字节大小
	u16 x0 = x;
	sizex = OLED_fonts->Height / 2;
	TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * OLED_fonts->Height;
	
	if(OLED_fonts->Height==12)
		Set_OLED_Address(x, y,x + sizex + 1, y + OLED_fonts->Height + 1); //设置光标位置
	
	if(OLED_fonts->Height == 16 || OLED_fonts->Height == 32)
		Set_OLED_Address(x, y, x+sizex - 1, y + OLED_fonts->Height - 1); //设置光标位置
	
	if(OLED_fonts->Height == 24)
		Set_OLED_Address(x, y, x+sizex + 3, y + OLED_fonts->Height + 3); //设置光标位置
	
	GetGBKENCode_from_EXFlash(ucBuffer,c);
	for(rowCount=0;rowCount<TypefaceNum;rowCount++)
		{
			for(bitCount = 0; bitCount < 8; bitCount ++)
				{
						if(ucBuffer[rowCount] & ( 0x80 >> bitCount ))	
							OLED_DrawPoint(x,y);
								x++;
						if((x - x0) == OLED_fonts->Width)
						{
							x = x0;
							y++;
							break;
						}
				}
		}
	
}


//显示字符串
//x,y:起点坐标  
//size1:字体大小 
//*chr:字符串起始地址 
void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
	while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
	{
		OLED_ShowExChar(x,y,*chr);
		x+=OLED_fonts->Height/2;
		if(x>135-OLED_fonts->Height)  //换行
		{
			x=0;
			y+=OLED_fonts->Height;
    }
		chr++;
  }
}
//显示汉字
//x,y:起点坐标
void OLED_ShowChinese(u16 x, u16 y, uint16_t c)
{
	uint8_t rowCount, bitCount;
	uint8_t ucBuffer [200];	
	u16 x0 = x;
	Set_OLED_Address(x, y, x + OLED_CHFonts->Width - 1, y + OLED_CHFonts->Height - 1);
	
	
	GetGBKCode ( ucBuffer, c );
	
	for(rowCount=0;rowCount<OLED_CHFonts->Height*OLED_CHFonts->Width/8;rowCount++)
		{
			for(bitCount = 0; bitCount < 8; bitCount ++)
				{
						if(ucBuffer[rowCount] & ( 0x80 >> bitCount ))	
							OLED_DrawPoint(x, y); //画一个点
								x++;
						if((x - x0) == OLED_CHFonts->Width)
						{
							x = x0;
							y++;
							break;
						}
				}
		}
}

/******************************************************************************
      函数说明：显示多个汉字
      入口数据：x,y显示坐标
                *pStr 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                OLED_fonts->Height 字号
                ziti:  0宋体  1幼圆
      返回值：  无
******************************************************************************/
void OLED_DispString_CH ( 	uint16_t usX , uint16_t usY, char * pStr)
{	
	uint16_t usCh;
	uint16_t sizex,sizey;
	sizex=OLED_CHFonts->Width;
	sizey=OLED_CHFonts->Height;
	while( * pStr != '\0' )
	{		
		if ( ( usX - 0 + sizex ) > Max_Column )
		{
			usX = 0;
			usY += OLED_CHFonts->Height;
		}
		
		if ( ( usY - 0 + sizey ) > Max_Row )
		{
			usX = 0;
			usY = 0;
		}	
		
		usCh = * ( uint16_t * ) pStr;	
	  usCh = ( usCh << 8 ) + ( usCh >> 8 );

		OLED_ShowChinese( usX, usY, usCh);
		
		usX += sizex;
		
		pStr += 2;           //一个汉字两个字节 

	}	   
	
}
/**
 * @brief  在 OLED 显示器上显示一行英文字符串
 * @param  line ：在特定扫描方向下字符串的起始Y坐标
  *   本参数可使用宏LINE(0)、LINE(1)等方式指定文字坐标，
  *   宏LINE(x)会根据当前选择的字体来计算Y坐标值。
	*		显示中文且使用LINE宏时，需要把英文字体设置成Font8x16
 * @param  pStr ：要显示的英文字符串的首地址
 * @retval 无
 */
void OLED_DispStringLine_EN (  uint16_t line,  char * pStr)
{
	uint16_t usX = 0;
	while ( * pStr != '\0' )
	{
		if ( ( usX - 0 + OLED_fonts->Width ) > Max_Column )
		{
			usX = 0;
			line += OLED_fonts->Height;
		}
		
		if ( ( line - 0 + OLED_fonts->Height ) > Max_Row )
		{
			usX = 0;
			line = 0;
		}
		
		OLED_ShowExChar ( usX, line, * pStr);
		
		pStr ++;
		
		usX += OLED_fonts->Width;
		
	}
	
}
/**
 * @brief  在 OLED 显示器上显示英文字符串(沿Y轴方向)
 * @param  usX ：在特定扫描方向下字符的起始X坐标
 * @param  usY ：在特定扫描方向下字符的起始Y坐标
 * @param  pStr ：要显示的英文字符串的首地址
 * @note 可使用OLED_SetCurrentBackColor、OLED_SetCurrentTextColor、OLED_SetColors函数设置颜色
 * @retval 无
 */
void OLED_DispString_EN_YDir (	 uint16_t usX,uint16_t usY ,  char * pStr)
{	
	while ( * pStr != '\0' )
	{
		
		if ( ( usY - 0 + OLED_fonts->Height ) >Max_Row  )
		{
			usY = 0;
			usX += OLED_fonts->Width;
		}
		
		if ( ( usX - 0 + OLED_fonts->Width ) >  Max_Column)
		{
			usX = 0;
			usY = 0;
		}
		
		OLED_ShowExChar ( usX, usY, * pStr);
		
		pStr ++;
		
		usY += OLED_fonts->Height;		
	}	
}
/**
 * @brief  在 OLED 显示器上显示中英文字符串
 * @param  line ：在特定扫描方向下字符串的起始Y坐标
  *   本参数可使用宏LINE(0)、LINE(1)等方式指定文字坐标，
  *   宏LINE(x)会根据当前选择的字体来计算Y坐标值。
	*		显示中文且使用LINE宏时，需要把英文字体设置成Font8x16
 * @param  pStr ：要显示的字符串的首地址
 * @note 可使用OLED_SetCurrentBackColor、OLED_SetCurrentTextColor、OLED_SetColors函数设置颜色
 * @retval 无
 */
void OLED_DispStringLine_EN_CH (  uint16_t line, char * pStr)
{
	uint16_t usCh;
	uint16_t usX = 0;
	while( * pStr != '\0' )
	{
		if ( * pStr <= 126 )	           	//英文字符
		{
			if ( ( usX - 0 + OLED_CHFonts->Width ) >Max_Column )
			{
				usX = 0;
				line += OLED_CHFonts->Height;
			}
			
			if ( ( line - 0 + OLED_CHFonts->Height ) > Max_Row )
			{
				usX = 0;
				line = 0;
			}			
		
		  OLED_ShowExChar ( usX, line, * pStr);
			
			usX +=  OLED_CHFonts->Width;
		
		  pStr ++;

		}
		
		else	                            //汉字字符
		{
			if ( ( usX - 0 + OLED_CHFonts->Width ) > Max_Column )
			{
				usX = 0;
				line += OLED_CHFonts->Height;
			}
			
			if ( ( line - 0 + OLED_CHFonts->Height ) > Max_Row )
			{
				usX = 0;
				line = 0;
			}	
			
			usCh = * ( uint16_t * ) pStr;	
			
			usCh = ( usCh << 8 ) + ( usCh >> 8 );		

			OLED_ShowChinese( usX, line, usCh);
			
			usX += OLED_CHFonts->Width;
			
			pStr += 2;           //一个汉字两个字节 
		
    }
		
  }	
} 
/******************************************************************************
      函数说明：显示中英文字符串
      入口数据：x,y显示坐标
                num 要显示的字符
                CurrentTextColor 字的颜色
                CurrentBackColor 字的背景色
                OLED_fonts->Height 字号
                mode:  0非叠加模式  1叠加模式
								 mode:  0宋体  1幼圆
      返回值：  无
******************************************************************************/
void OLED_ShowString_CH_EN(u16 x, u16 y, char *pStr)
{
	uint16_t usCh;
	while( * pStr != '\0' )
	{
		if ( * pStr <= 126 )	           	//英文字符
		{
			OLED_ShowExChar(x, y, *pStr);
			x += OLED_fonts->Height /2;
			if ( ( x+OLED_fonts->Height /2) > Max_Column )
			{
				x = 0;
				y +=OLED_fonts->Height;
			}
		
			if ( ( y - 0 + OLED_fonts->Height ) > Max_Row )
			{
				x = 0;
				y = 0;
			}	
				pStr++;

		}
		
		else	                            //汉字字符
		{
			if ( ( x - 0 + OLED_CHFonts->Width ) > Max_Column )
			{
				x = 0;
				y += OLED_CHFonts->Height;
			}
			
			if ( ( y - 0 + OLED_CHFonts->Height ) > Max_Row )
			{
				x = 0;
				y = 0;
			}	
			
			usCh = * ( uint16_t * ) pStr;	
			
			usCh = ( usCh << 8 ) + ( usCh >> 8 );		

			OLED_ShowChinese( x, y, usCh);
			
			x += OLED_CHFonts->Width;
			
			pStr += 2;           //一个汉字两个字节 
		
    }
		
  }	
}
/**
 * @brief  在 OLED 显示器上显示中英文字符串(沿Y轴方向)
 * @param  usX ：在特定扫描方向下字符的起始X坐标
 * @param  usY ：在特定扫描方向下字符的起始Y坐标
 * @param  pStr ：要显示的中英文字符串的首地址
 * @note 可使用OLED_SetCurrentBackColor、OLED_SetCurrentTextColor、OLED_SetColors函数设置颜色
 * @retval 无
 */
void OLED_DispString_EN_CH_YDir (  uint16_t usX,uint16_t usY , char * pStr)
{
	uint16_t usCh;
	while( * pStr != '\0' )
	{			
			//统一使用汉字的宽高来计算换行
			if ( ( usY - 0 + OLED_CHFonts->Height ) >Max_Row  )
			{
				usY = 0;
				usX += OLED_CHFonts->Width;
			}			
			if ( ( usX - 0 + OLED_CHFonts->Width ) >  Max_Column)
			{
				usX = 0;
				usY = 0;
			}
			
		//显示	
		if ( * pStr <= 126 )	           	//英文字符
		{			
			OLED_ShowExChar ( usX, usY, * pStr);
			
			pStr ++;
			
			usY += OLED_CHFonts->Height;		
		}
		else	                            //汉字字符
		{			
			usCh = * ( uint16_t * ) pStr;	
			
			usCh = ( usCh << 8 ) + ( usCh >> 8 );		

			OLED_ShowChinese( usX, usY, usCh);
			
			usY += OLED_CHFonts->Height;
			
			pStr += 2;           //一个汉字两个字节 
		
    }
		
  }	
} 
//num 显示汉字的个数
//space 每一遍显示的间隔
void OLED_ScrollDisplay(u8 num,u8 space)
{
	u8 i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
	    OLED_ShowChinese(128,24,t); //写入一个汉字保存在OLED_GRAM[][]数组中
			t++;
		}
		if(t==num)
			{
				for(r=0;r<16*space;r++)      //显示间隔
				 {
					for(i=0;i<144;i++)
						{
							for(n=0;n<8;n++)
							{
								OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
							}
						}
           OLED_Refresh();
				 }
        t=0;
      }
		m++;
		if(m==16){m=0;}
		for(i=0;i<144;i++)   //实现左移
		{
			for(n=0;n<8;n++)
			{
				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
			}
		}
		OLED_Refresh();
	}
}

//配置写入数据的起始位置
void OLED_WR_BP(u8 x,u8 y)
{
	OLED_WR_Byte(0xb0+y,OLED_CMD);//设置行起始地址
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD);
}

//x0,y0：起点坐标
//x1,y1：终点坐标
//BMP[]：要写入的图片数组
void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[])
{
	u32 j=0;
	u8 x=0,y=0;
	if(y%8==0)y=0;
	else y+=1;
	for(y=y0;y<y1;y++)
	 {
		 OLED_WR_BP(x0,y);
		 for(x=x0;x<x1;x++)
		 {
			 OLED_WR_Byte(BMP[j],OLED_DATA);
			 j++;
     }
	 }
}

/**
  * @brief  设置英文字体类型
  * @param  fonts: 指定要选择的字体
	*		参数为以下值之一
  * 	@arg：&ENFont6x12;
  * 	@arg：&ENFont8x16;
  * 	@arg：&ENFont12x24;
  * 	@arg：&ENFont16x32;
  * @retval None
  */
void OLED_SetFont(ENFONT *fonts)
{
  OLED_fonts = fonts;
}

/**
  * @brief  获取当前字体类型
  * @param  None.
  * @retval 返回当前字体类型
  */
ENFONT *OLED_GetFont(void)
{
  return OLED_fonts;
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

void OLED_SetCHFont(CHFONT *fonts)
{
	OLED_CHFonts = fonts;
}
/**
  * @brief  获取当前字体类型
  * @param  None.
  * @retval 返回当前字体类型
  */
CHFONT *OLED_GetCHFont(void)
{
  return OLED_CHFonts;
}
