#ifndef __OLEDFONT_H
#define __OLEDFONT_H

#include "stm32f4xx.h"

typedef struct _tFont
{
	uint16_t ZITI;
	uint16_t Width;
  uint16_t Height;
}ENFONT;

typedef struct _cFont
{
	uint16_t ZITI;
	uint16_t Width;
  uint16_t Height;
}CHFONT;


extern ENFONT ENXINSONGFont6x12;
extern ENFONT ENXINSONGFont8x16;
extern ENFONT ENXINSONGFont12x24;
extern ENFONT ENXINSONGFont16x32;

extern ENFONT ENSHUMAFont6x12;
extern ENFONT ENSHUMAFont8x16;
extern ENFONT ENSHUMAFont12x24;
extern ENFONT ENSHUMAFont16x32;

extern CHFONT YOUYUAN16x16;
extern CHFONT YOUYUAN24x24;
extern CHFONT YOUYUAN32x32;
extern CHFONT XINSONG16x16;
extern CHFONT XINSONG24x24;
extern CHFONT XINSONG32x32;
extern CHFONT KAITI16x16;
extern CHFONT KAITI24x24;
extern CHFONT KAITI32x32;
extern CHFONT FANGSONG16x16;
extern CHFONT FANGSONG24x24;
extern CHFONT FANGSONG32x32;
extern CHFONT HUAWENXINWEI40x32;
extern CHFONT HUAWENCAIYUN40x32;


/*获取字库的函数*/
	//定义获取中文字符字模数组的函数名，ucBuffer为存放字模数组名，usChar为中文字符（国标码）
#define      GetGBKCode( ucBuffer, usChar )  GetGBKCode_from_EXFlash( ucBuffer, usChar )  
#define 		 GetGBKENCode( ucBuffer, usChar )		GetGBKENCode_from_EXFlash( uint8_t * pBuffer, uint16_t c)
int GetGBKCode_from_EXFlash( uint8_t * pBuffer, uint16_t c);
int GetGBKENCode_from_EXFlash( uint8_t * pBuffer, uint16_t c);
/*----------------------------------------------------------------------------------------------*/

#endif


