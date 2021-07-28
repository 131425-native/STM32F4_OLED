#include "bsp_oledinit.h"
#include "bsp_oled.h"
#include "w25q128.h"
extern CHFONT *OLED_CHFonts;
extern ENFONT *OLED_fonts;
uint16_t ziti1,daxiao;
//��ʼ�ź�
void I2C_Start(void)
{
	OLED_SDIN_Set();
	OLED_SCLK_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

//�����ź�
void I2C_Stop(void)
{
	OLED_SCLK_Set();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
}

//�ȴ��ź���Ӧ
void I2C_WaitAck(void) //�������źŵĵ�ƽ
{
	OLED_SCLK_Set();
	OLED_SCLK_Clr();
}

//д��һ���ֽ�
void Send_Byte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		OLED_SCLK_Clr();//��ʱ���ź�����Ϊ�͵�ƽ
		if(dat&0x80)//��dat��8λ�����λ����д��
		{
			OLED_SDIN_Set();
    }
		else
		{
			OLED_SDIN_Clr();
    }
		OLED_SCLK_Set();//��ʱ���ź�����Ϊ�ߵ�ƽ
		OLED_SCLK_Clr();//��ʱ���ź�����Ϊ�͵�ƽ
		dat<<=1;
  }
}

//����һ���ֽ�
//��SSD1306д��һ���ֽڡ�
//mode:����/�����־ 0,��ʾ����;1,��ʾ����;
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


//����OLED��ʾ 
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//��ɱ�ʹ��
	OLED_WR_Byte(0x14,OLED_CMD);//������ɱ�
	OLED_WR_Byte(0xAF,OLED_CMD);//������Ļ
}

//�ر�OLED��ʾ 
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//��ɱ�ʹ��
	OLED_WR_Byte(0x10,OLED_CMD);//�رյ�ɱ�
	OLED_WR_Byte(0xAF,OLED_CMD);//�ر���Ļ
}
int GetGBKENCode_from_EXFlash( uint8_t * pBuffer, uint16_t c)
{
		unsigned int pos;
		static uint8_t everRead=0;
		u16 zifu;
		u16 TypefaceNum; //һ���ַ���ռ�ֽڴ�С
		u8 sizex;
	/*��һ��ʹ�ã���ʼ��FLASH*/
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
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANEN12+pos,TypefaceNum); //��ȡ�ֿ����� 
			else if(OLED_fonts->Height == 16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANEN16+pos,TypefaceNum); //��ȡ�ֿ����� 
			else if(OLED_fonts->Height == 24)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANEN24+pos,TypefaceNum); //��ȡ�ֿ����� 
			else
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANEN32+pos,TypefaceNum); //��ȡ�ֿ�����
		}
		else if(OLED_fonts->ZITI == 1)
		{
			if(OLED_fonts->Height == 12)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SONGTIEN12+pos,TypefaceNum); //��ȡ�ֿ����� 
			else if(OLED_fonts->Height == 16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SONGTIEN16+pos,TypefaceNum); //��ȡ�ֿ����� 
			else if(OLED_fonts->Height == 24)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SONGTIEN24+pos,TypefaceNum); //��ȡ�ֿ����� 
			else
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SONGTIEN32+pos,TypefaceNum); //��ȡ�ֿ�����
		}
		else if(OLED_fonts->ZITI == 3)
		{
			if(OLED_fonts->Height == 12)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SHUMA2_12+pos,TypefaceNum); //��ȡ�ֿ����� 
			else if(OLED_fonts->Height == 16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SHUMA2_16+pos,TypefaceNum); //��ȡ�ֿ����� 
			else if(OLED_fonts->Height == 24)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SHUMA2_24+pos,TypefaceNum); //��ȡ�ֿ����� 
			else
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_SHUMA2_32+pos,TypefaceNum); //��ȡ�ֿ�����
		}
		return 0;
}

/*ʹ��FLASH��ģ*/
//��ģGB2312_H1616���׵ĺ���

//�����ֿ�洢��FLASH����ʼ��ַ ��
//GBKCODE_START_ADDRESS ��fonts.h�ļ�����
/**
  * @brief  ��ȡFLASH������ʾ�ֿ�����
	* @param  pBuffer:�洢�ֿ����Ļ�����
	* @param  c �� Ҫ��ȡ������
  * @retval None.
  */
int GetGBKCode_from_EXFlash( uint8_t * pBuffer, uint16_t c)
{ 
    unsigned char High8bit,Low8bit;
    unsigned int pos;
		static uint8_t everRead=0;
		/*��һ��ʹ�ã���ʼ��FLASH*/
		if(everRead == 0)
		{
			W25Q128_Init();
			everRead = 1;
		}
	
	  High8bit= c >> 8;     /* ȡ��8λ���� */
    Low8bit= c & 0x00FF;  /* ȡ��8λ���� */		
	  	
		/*GB2312 ��ʽ*/
	
    pos = ((High8bit-0xa1)*94+Low8bit-0xa1)*OLED_CHFonts->Width*OLED_CHFonts->Height/8;  
		if(OLED_CHFonts->ZITI==0)
		{
			if(OLED_CHFonts->Width==16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANCH16+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //��ȡ�ֿ����� 
			else if(OLED_CHFonts->Width==24)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANCH24+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //��ȡ�ֿ����� 
			else 
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_YOUYUANCH32+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //��ȡ�ֿ����� 
		}			
		if(OLED_CHFonts->ZITI==1)
		{
			if(OLED_CHFonts->Width==16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_XINSONG16+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //��ȡ�ֿ����� 
			else if(OLED_CHFonts->Width==24)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_XINSONG24+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //��ȡ�ֿ����� 
			else 
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_XINSONG32+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //��ȡ�ֿ����� 
		}
		if(OLED_CHFonts->ZITI==2)
		{
			if(OLED_CHFonts->Width==16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_KAITI16+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //��ȡ�ֿ����� 
			else if(OLED_CHFonts->Width==24)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_KAITI24+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //��ȡ�ֿ����� 
			else 
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_KAITI32+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //��ȡ�ֿ����� 
		}
		if(OLED_CHFonts->ZITI==3)
		{
			if(OLED_CHFonts->Width==16)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_FANGSONG16+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //��ȡ�ֿ����� 
			else if(OLED_CHFonts->Width==24)
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_FANGSONG24+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //��ȡ�ֿ����� 
			else 
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_FANGSONG32+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //��ȡ�ֿ����� 
		}
		if(OLED_CHFonts->ZITI==4)
		{
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_HUAWENXINWEI32+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //��ȡ�ֿ����� 
		}
		if(OLED_CHFonts->ZITI==5)
		{
				W25Q128_Read(pBuffer,GBKCODE_ADDRESS_HUAWENCAIYUN32+pos,OLED_CHFonts->Width*OLED_CHFonts->Height/8); //��ȡ�ֿ����� 
		}
		return 0;  
     
}
//OLED�ĳ�ʼ��
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
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
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




