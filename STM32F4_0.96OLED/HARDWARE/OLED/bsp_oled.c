#include "bsp_oled.h"
#include "stdlib.h"
#include "bsp_oledinit.h"
#include "delay.h"
CHFONT *OLED_CHFonts = &YOUYUAN16x16;
ENFONT *OLED_fonts = &ENXINSONGFont8x16;  //Ӣ������

u8 OLED_GRAM[144][8];

//���Ժ���
void OLED_ColorTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xA6,OLED_CMD);//������ʾ
		}
	if(i==1)
		{
			OLED_WR_Byte(0xA7,OLED_CMD);//��ɫ��ʾ
		}
}

//��Ļ��ת180��
void OLED_DisplayTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xC8,OLED_CMD);//������ʾ
			OLED_WR_Byte(0xA1,OLED_CMD);
		}
	if(i==1)
		{
			OLED_WR_Byte(0xC0,OLED_CMD);//��ת��ʾ
			OLED_WR_Byte(0xA0,OLED_CMD);
		}
}
//�����Դ浽OLED	
void OLED_Refresh(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   OLED_WR_Byte(0xb0+i,OLED_CMD); //��������ʼ��ַ
	   OLED_WR_Byte(0x00,OLED_CMD);   //���õ�����ʼ��ַ
	   OLED_WR_Byte(0x10,OLED_CMD);   //���ø�����ʼ��ַ
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
//��������
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   for(n=0;n<128;n++)
			{
			 OLED_GRAM[n][i]=0;//�����������
			}
  }
	OLED_Refresh();//������ʾ
}

//���� 
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

//���һ����
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


//����
//x:0~128
//y:0~64
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2)
{
	u8 i,k,k1,k2;
	if(x1==x2)    //������
	{
			for(i=0;i<(y2-y1);i++)
			{
				OLED_DrawPoint(x1,y1+i);
			}
  }
	else if(y1==y2)   //������
	{
			for(i=0;i<(x2-x1);i++)
			{
				OLED_DrawPoint(x1+i,y1);
			}
  }
	else      //��б��
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
//x,y:Բ������
//r:Բ�İ뾶
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
        num = (a * a + b * b) - r*r;//���㻭�ĵ���Բ�ĵľ���
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}

/******************************************************************************
      ����˵������ʾ�����ַ�
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ���ַ�
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                OLED_fonts->Height �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void OLED_ShowExChar(u16 x, u16 y,uint16_t c)
{
	uint8_t rowCount, bitCount;
	uint8_t ucBuffer [200];	
	u8 sizex;
	u16 TypefaceNum; //һ���ַ���ռ�ֽڴ�С
	u16 x0 = x;
	sizex = OLED_fonts->Height / 2;
	TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * OLED_fonts->Height;
	
	if(OLED_fonts->Height==12)
		Set_OLED_Address(x, y,x + sizex + 1, y + OLED_fonts->Height + 1); //���ù��λ��
	
	if(OLED_fonts->Height == 16 || OLED_fonts->Height == 32)
		Set_OLED_Address(x, y, x+sizex - 1, y + OLED_fonts->Height - 1); //���ù��λ��
	
	if(OLED_fonts->Height == 24)
		Set_OLED_Address(x, y, x+sizex + 3, y + OLED_fonts->Height + 3); //���ù��λ��
	
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


//��ʾ�ַ���
//x,y:�������  
//size1:�����С 
//*chr:�ַ�����ʼ��ַ 
void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
	while((*chr>=' ')&&(*chr<='~'))//�ж��ǲ��ǷǷ��ַ�!
	{
		OLED_ShowExChar(x,y,*chr);
		x+=OLED_fonts->Height/2;
		if(x>135-OLED_fonts->Height)  //����
		{
			x=0;
			y+=OLED_fonts->Height;
    }
		chr++;
  }
}
//��ʾ����
//x,y:�������
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
							OLED_DrawPoint(x, y); //��һ����
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
      ����˵������ʾ�������
      ������ݣ�x,y��ʾ����
                *pStr Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                OLED_fonts->Height �ֺ�
                ziti:  0����  1��Բ
      ����ֵ��  ��
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
		
		pStr += 2;           //һ�����������ֽ� 

	}	   
	
}
/**
 * @brief  �� OLED ��ʾ������ʾһ��Ӣ���ַ���
 * @param  line �����ض�ɨ�跽�����ַ�������ʼY����
  *   ��������ʹ�ú�LINE(0)��LINE(1)�ȷ�ʽָ���������꣬
  *   ��LINE(x)����ݵ�ǰѡ�������������Y����ֵ��
	*		��ʾ������ʹ��LINE��ʱ����Ҫ��Ӣ���������ó�Font8x16
 * @param  pStr ��Ҫ��ʾ��Ӣ���ַ������׵�ַ
 * @retval ��
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
 * @brief  �� OLED ��ʾ������ʾӢ���ַ���(��Y�᷽��)
 * @param  usX �����ض�ɨ�跽�����ַ�����ʼX����
 * @param  usY �����ض�ɨ�跽�����ַ�����ʼY����
 * @param  pStr ��Ҫ��ʾ��Ӣ���ַ������׵�ַ
 * @note ��ʹ��OLED_SetCurrentBackColor��OLED_SetCurrentTextColor��OLED_SetColors����������ɫ
 * @retval ��
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
 * @brief  �� OLED ��ʾ������ʾ��Ӣ���ַ���
 * @param  line �����ض�ɨ�跽�����ַ�������ʼY����
  *   ��������ʹ�ú�LINE(0)��LINE(1)�ȷ�ʽָ���������꣬
  *   ��LINE(x)����ݵ�ǰѡ�������������Y����ֵ��
	*		��ʾ������ʹ��LINE��ʱ����Ҫ��Ӣ���������ó�Font8x16
 * @param  pStr ��Ҫ��ʾ���ַ������׵�ַ
 * @note ��ʹ��OLED_SetCurrentBackColor��OLED_SetCurrentTextColor��OLED_SetColors����������ɫ
 * @retval ��
 */
void OLED_DispStringLine_EN_CH (  uint16_t line, char * pStr)
{
	uint16_t usCh;
	uint16_t usX = 0;
	while( * pStr != '\0' )
	{
		if ( * pStr <= 126 )	           	//Ӣ���ַ�
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
		
		else	                            //�����ַ�
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
			
			pStr += 2;           //һ�����������ֽ� 
		
    }
		
  }	
} 
/******************************************************************************
      ����˵������ʾ��Ӣ���ַ���
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ���ַ�
                CurrentTextColor �ֵ���ɫ
                CurrentBackColor �ֵı���ɫ
                OLED_fonts->Height �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
								 mode:  0����  1��Բ
      ����ֵ��  ��
******************************************************************************/
void OLED_ShowString_CH_EN(u16 x, u16 y, char *pStr)
{
	uint16_t usCh;
	while( * pStr != '\0' )
	{
		if ( * pStr <= 126 )	           	//Ӣ���ַ�
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
		
		else	                            //�����ַ�
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
			
			pStr += 2;           //һ�����������ֽ� 
		
    }
		
  }	
}
/**
 * @brief  �� OLED ��ʾ������ʾ��Ӣ���ַ���(��Y�᷽��)
 * @param  usX �����ض�ɨ�跽�����ַ�����ʼX����
 * @param  usY �����ض�ɨ�跽�����ַ�����ʼY����
 * @param  pStr ��Ҫ��ʾ����Ӣ���ַ������׵�ַ
 * @note ��ʹ��OLED_SetCurrentBackColor��OLED_SetCurrentTextColor��OLED_SetColors����������ɫ
 * @retval ��
 */
void OLED_DispString_EN_CH_YDir (  uint16_t usX,uint16_t usY , char * pStr)
{
	uint16_t usCh;
	while( * pStr != '\0' )
	{			
			//ͳһʹ�ú��ֵĿ�������㻻��
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
			
		//��ʾ	
		if ( * pStr <= 126 )	           	//Ӣ���ַ�
		{			
			OLED_ShowExChar ( usX, usY, * pStr);
			
			pStr ++;
			
			usY += OLED_CHFonts->Height;		
		}
		else	                            //�����ַ�
		{			
			usCh = * ( uint16_t * ) pStr;	
			
			usCh = ( usCh << 8 ) + ( usCh >> 8 );		

			OLED_ShowChinese( usX, usY, usCh);
			
			usY += OLED_CHFonts->Height;
			
			pStr += 2;           //һ�����������ֽ� 
		
    }
		
  }	
} 
//num ��ʾ���ֵĸ���
//space ÿһ����ʾ�ļ��
void OLED_ScrollDisplay(u8 num,u8 space)
{
	u8 i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
	    OLED_ShowChinese(128,24,t); //д��һ�����ֱ�����OLED_GRAM[][]������
			t++;
		}
		if(t==num)
			{
				for(r=0;r<16*space;r++)      //��ʾ���
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
		for(i=0;i<144;i++)   //ʵ������
		{
			for(n=0;n<8;n++)
			{
				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
			}
		}
		OLED_Refresh();
	}
}

//����д�����ݵ���ʼλ��
void OLED_WR_BP(u8 x,u8 y)
{
	OLED_WR_Byte(0xb0+y,OLED_CMD);//��������ʼ��ַ
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD);
}

//x0,y0���������
//x1,y1���յ�����
//BMP[]��Ҫд���ͼƬ����
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
  * @brief  ����Ӣ����������
  * @param  fonts: ָ��Ҫѡ�������
	*		����Ϊ����ֵ֮һ
  * 	@arg��&ENFont6x12;
  * 	@arg��&ENFont8x16;
  * 	@arg��&ENFont12x24;
  * 	@arg��&ENFont16x32;
  * @retval None
  */
void OLED_SetFont(ENFONT *fonts)
{
  OLED_fonts = fonts;
}

/**
  * @brief  ��ȡ��ǰ��������
  * @param  None.
  * @retval ���ص�ǰ��������
  */
ENFONT *OLED_GetFont(void)
{
  return OLED_fonts;
}
/**
  * @brief  ���ú�����������
  * @param  fonts: ָ��Ҫѡ�������
	*		����Ϊ����ֵ֮һ
  * 	@arg��&CHFONT YOUYUAN16x16;
  * 	@arg��&CHFONT YOUYUAN24x24;
  * 	@arg��&CHFONT YOUYUAN32x32;
  * 	@arg��&CHFONT XINSONG16x16;
  * 	@arg��&CHFONT XINSONG24x24;
  * 	@arg��&CHFONT XINSONG32x32;
  * 	@arg��&CHFONT HEITI16x16;
  * 	@arg��&CHFONT HEITI24x24;
  * 	@arg��&CHFONT HEITI32x32;
  * 	@arg��&CHFONT LISHU24x24;
  * 	@arg��&CHFONT LISHU32x32;
  * 	@arg��&CHFONT KAITI24x24;
  * 	@arg��&CHFONT KAITI32x32;
  * 	@arg��&CHFONT FANGZHENGYAOTI32x32;
  * 	@arg��&CHFONT FANGZHENGSHUTI32x32;
  * @retval None
  */

void OLED_SetCHFont(CHFONT *fonts)
{
	OLED_CHFonts = fonts;
}
/**
  * @brief  ��ȡ��ǰ��������
  * @param  None.
  * @retval ���ص�ǰ��������
  */
CHFONT *OLED_GetCHFont(void)
{
  return OLED_CHFonts;
}
