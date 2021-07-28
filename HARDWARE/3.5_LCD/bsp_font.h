#ifndef _BSP_FONT_H_
#define _BSP_FONT_H_

#include "stm32f4xx.h"
#include "bsp_lcdinit.h"
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

extern ENFONT ENYOUYUANFont6x12;
extern ENFONT ENYOUYUANFont8x16;
extern ENFONT ENYOUYUANFont12x24;
extern ENFONT ENYOUYUANFont16x32;

extern ENFONT ENSONGTIFont6x12;
extern ENFONT ENSONGTIFont8x16;
extern ENFONT ENSONGTIFont12x24;
extern ENFONT ENSONGTIFont16x32;

extern ENFONT ENSHUMAFont10x16;
extern ENFONT ENSHUMAFont12x24;
extern ENFONT ENSHUMAFont16x32;

extern ENFONT ENSHUMA2Font6x12;
extern ENFONT ENSHUMA2Font8x16;
extern ENFONT ENSHUMA2Font12x24;
extern ENFONT ENSHUMA2Font16x32;

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

extern const unsigned char asc2_1206[95][12];
extern const unsigned char asc2_1608[95][16];

#endif /*_BSP_FONT_H_*/


