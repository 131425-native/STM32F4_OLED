#ifndef _BSP_OLEDINIT_H_
#define _BSP_OLEDINIT_H_

#include "stm32f4xx.h"
#include "delay.h"

//-----------------OLED端口定义---------------- 

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_4)//SCL
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

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

/*note：可以用0-1024*1024 相当于1M可以用*/

#define Max_Column	128			// 0-95
#define Max_Row			64			// 0-63

#define LINE(x) ((x) * (24))

//LINEY统一使用汉字字模的高度
#define LINEY(x) ((x) * (WIDTH_CH_CHAR))

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

void OLED_Init(void);
void OLED_DisplayTurn(u8 i);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
void Send_Byte(u8 dat);
void OLED_WR_Byte(u8 dat,u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);


#endif /*_BSP_OLEDINIT_H_*/


