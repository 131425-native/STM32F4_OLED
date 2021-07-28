#include "bsp_oledinit.h"
#include "bsp_oled.h"
#include "w25q128.h"
extern CHFONT *OLED_CHFonts;
extern ENFONT *OLED_fonts;
uint16_t ziti1,daxiao;
//起始信号
void I2C_Start(void)
{
	OLED_SDIN_Set();
	OLED_SCLK_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

//结束信号
void I2C_Stop(void)
{
	OLED_SCLK_Set();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
}

//等待信号响应
void I2C_WaitAck(void) //测数据信号的电平
{
	OLED_SCLK_Set();
	OLED_SCLK_Clr();
}

//写入一个字节
void Send_Byte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		OLED_SCLK_Clr();//将时钟信号设置为低电平
		if(dat&0x80)//将dat的8位从最高位依次写入
		{
			OLED_SDIN_Set();
    }
		else
		{
			OLED_SDIN_Clr();
    }
		OLED_SCLK_Set();//将时钟信号设置为高电平
		OLED_SCLK_Clr();//将时钟信号设置为低电平
		dat<<=1;
  }
}

//发送一个字节
//向SSD1306写入一个字节。
//mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 mode)
{
	I2C_Start();
	Send_Byte(0x78);
	I2C_WaitAck();
	if(mode){Send_Byte(0x40);}
  else{Send_Byte(0x00);}
	I2C_WaitAck();
	Send_Byte(dat);
	I2C_WaitAck();
	I2C_Stop();
}


//开启OLED显示 
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x14,OLED_CMD);//开启电荷泵
	OLED_WR_Byte(0xAF,OLED_CMD);//点亮屏幕
}

//关闭OLED显示 
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x10,OLED_CMD);//关闭电荷泵
	OLED_WR_Byte(0xAF,OLED_CMD);//关闭屏幕
}
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
		sizex = OLED_fonts->Height / 2;
		TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * OLED_fonts->Height;
		zifu=c - ' ';
		pos=zifu * TypefaceNum;
		if(OLED_fonts->ZITI == 0)
		{
			if(OLED_fonts->Height == 12)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANEN12+pos,TypefaceNum); //读取字库数据 
			else if(OLED_fonts->Height == 16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANEN16+pos,TypefaceNum); //读取字库数据 
			else if(OLED_fonts->Height == 24)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANEN24+pos,TypefaceNum); //读取字库数据 
			else
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANEN32+pos,TypefaceNum); //读取字库数据
		}
		else if(OLED_fonts->ZITI == 1)
		{
			if(OLED_fonts->Height == 12)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SONGTIEN12+pos,TypefaceNum); //读取字库数据 
			else if(OLED_fonts->Height == 16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SONGTIEN16+pos,TypefaceNum); //读取字库数据 
			else if(OLED_fonts->Height == 24)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SONGTIEN24+pos,TypefaceNum); //读取字库数据 
			else
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SONGTIEN32+pos,TypefaceNum); //读取字库数据
		}
		else if(OLED_fonts->ZITI == 3)
		{
			if(OLED_fonts->Height == 12)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SHUMA2_12+pos,TypefaceNum); //读取字库数据 
			else if(OLED_fonts->Height == 16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SHUMA2_16+pos,TypefaceNum); //读取字库数据 
			else if(OLED_fonts->Height == 24)
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
	
    pos = ((High8bit-0xa1)*94+Low8bit-0xa1)*OLED_CHFonts->Width*OLED_CHFonts->Height/8;  
		if(OLED_CHFonts->ZITI==0)
		{
			if(OLED_CHFonts->Width==16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANCH16+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //读取字库数据 
			else if(OLED_CHFonts->Width==24)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANCH24+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //读取字库数据 
			else 
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANCH32+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //读取字库数据 
		}			
		if(OLED_CHFonts->ZITI==1)
		{
			if(OLED_CHFonts->Width==16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_XINSONG16+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //读取字库数据 
			else if(OLED_CHFonts->Width==24)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_XINSONG24+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //读取字库数据 
			else 
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_XINSONG32+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //读取字库数据 
		}
		if(OLED_CHFonts->ZITI==2)
		{
			if(OLED_CHFonts->Width==16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_KAITI16+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //读取字库数据 
			else if(OLED_CHFonts->Width==24)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_KAITI24+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //读取字库数据 
			else 
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_KAITI32+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //读取字库数据 
		}
		if(OLED_CHFonts->ZITI==3)
		{
			if(OLED_CHFonts->Width==16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_FANGSONG16+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //读取字库数据 
			else if(OLED_CHFonts->Width==24)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_FANGSONG24+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //读取字库数据 
			else 
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_FANGSONG32+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //读取字库数据 
		}
		if(OLED_CHFonts->ZITI==4)
		{
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_HUAWENXINWEI32+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //读取字库数据 
		}
		if(OLED_CHFonts->ZITI==5)
		{
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_HUAWENCAIYUN32+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //读取字库数据 
		}
		return 0;  
     
}
//OLED的初始化
void OLED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
	
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);
	OLED_Clear();
}




