#include "w25q128.h" 
#include "spi.h"
#include "delay.h"	   
#include "usart.h"	
//////////////////////////////////////////////////////////////////////////////////	 
extern char read[200];
uint16_t n;
//W25Q128 ��������	   
//STM32F4����ģ��-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com										  
////////////////////////////////////////////////////////////////////////////////// 	
extern __IO uint8_t FLAG_StartFlowData; //������ģʽ��ʼ/���������ʶ
extern __IO uint8_t FLAG_FLASH_WRITE;   //flashд���ʶ 1��ʾ����д��һ���ֽڣ���flashд�뺯�����㴮���ж���λ
extern __IO uint8_t count;              //���ռ��� 
 
u16 W25Q128_TYPE=W25Q128;	//Ĭ����W25Q128

													 
//��ʼ��SPI FLASH��IO��
void W25Q128_Init(void)
{ 
	uint8_t data[64];
  GPIO_InitTypeDef  GPIO_InitStructure;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
 

	  //GPIOB14
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PB0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��


	W25Q128_CS=1;			//SPI FLASH��ѡ��
	SPI1_Init();		   			//��ʼ��SPI
	SPI1_SetSpeed(SPI_BaudRatePrescaler_2);		//����Ϊ42Mʱ��,����ģʽ 
	W25Q128_TYPE=W25Q128_ReadID(data);	//��ȡFLASH ID.
}  

//��ȡW25Q128��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
u8 W25Q128_ReadSR(void)   
{  
	u8 byte=0;   
	W25Q128_CS=0;                            //ʹ������   
	SPI1_ReadWriteByte(W25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI1_ReadWriteByte(0Xff);             //��ȡһ���ֽ�  
	W25Q128_CS=1;                            //ȡ��Ƭѡ     
	return byte;   
} 
//дW25Q128״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void W25Q128_Write_SR(u8 sr)   
{   
	W25Q128_CS=0;                            //ʹ������   
	SPI1_ReadWriteByte(W25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������    
	SPI1_ReadWriteByte(sr);               //д��һ���ֽ�  
	W25Q128_CS=1;                            //ȡ��Ƭѡ     	      
}   
//W25Q128дʹ��	
//��WEL��λ   
void W25Q128_Write_Enable(void)   
{
	W25Q128_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(W25X_WriteEnable);      //����дʹ��  
	W25Q128_CS=1;                            //ȡ��Ƭѡ     	      
} 
//W25Q128д��ֹ	
//��WEL����  
void W25Q128_Write_Disable(void)   
{  
	W25Q128_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(W25X_WriteDisable);     //����д��ָֹ��    
	W25Q128_CS=1;                            //ȡ��Ƭѡ     	      
} 		
//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪW25Q128    
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
u16 W25Q128_ReadID(uint8_t *id)
{
	u16 Temp = 0;	  
	W25Q128_CS=0;				    
	SPI1_ReadWriteByte(0x90);//���Ͷ�ȡID����	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 			   
	*id++ = SPI1_ReadWriteByte(0xFF)<<8;  
	*id = SPI1_ReadWriteByte(0xFF);	 
	Temp|= SPI1_ReadWriteByte(0xFF)<<8;
	Temp|= SPI1_ReadWriteByte(0xFF);
	W25Q128_CS=1;

	return Temp;	
} 

//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void W25Q128_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
		u32 i;   										    
		W25Q128_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(W25X_ReadData);         //���Ͷ�ȡ����   
    SPI1_ReadWriteByte((u8)((ReadAddr)>>16));  //����24bit��ַ    
    SPI1_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPI1_ReadWriteByte((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
		{ 
      pBuffer[i]=SPI1_ReadWriteByte(0XFF);   //ѭ������  
		}
		W25Q128_CS=1;  				    	      
}  
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void W25Q128_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
    W25Q128_Write_Enable();                  //SET WEL 
	W25Q128_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(W25X_PageProgram);      //����дҳ����   
    SPI1_ReadWriteByte((u8)((WriteAddr)>>16)); //����24bit��ַ    
    SPI1_ReadWriteByte((u8)((WriteAddr)>>8));   
    SPI1_ReadWriteByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPI1_ReadWriteByte(pBuffer[i]);//ѭ��д��  
	W25Q128_CS=1;                            //ȡ��Ƭѡ 
	W25Q128_Wait_Busy();					   //�ȴ�д�����
} 
//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void W25Q128_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		W25Q128_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 
//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���65535)   
u8 W25Q128_BUFFER[4096];		 
void W25Q128_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod�������࣬��writeAddr��SPI_FLASH_PageSize��������������AddrֵΪ0*/
  Addr = WriteAddr % SPI_FLASH_PageSize;
	
	/*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
  count = SPI_FLASH_PageSize - Addr;
	/*�����Ҫд��������ҳ*/
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
	
	/* Addr=0,��WriteAddr �պð�ҳ���� aligned  */
  if (Addr == 0)
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
			W25Q128_Write_Page(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    { 
			/*�Ȱ�����ҳ��д��*/
      while (NumOfPage--)
      {
        W25Q128_Write_Page(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      W25Q128_Write_Page(pBuffer, WriteAddr, NumOfSingle);
    }
  }
	/* ����ַ�� SPI_FLASH_PageSize ������  */
  else 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0)
    {
			/*��ǰҳʣ���count��λ�ñ�NumOfSingleС��һҳд����*/
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
				/*��д����ǰҳ*/
        W25Q128_Write_Page(pBuffer, WriteAddr, count);
				
        WriteAddr +=  count;
        pBuffer += count;
				/*��дʣ�������*/
        W25Q128_Write_Page(pBuffer, WriteAddr, temp);
      }
      else /*��ǰҳʣ���count��λ����д��NumOfSingle������*/
      {
        W25Q128_Write_Page(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*��ַ����������count�ֿ������������������*/
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
			
			/* ��д��count�����ݣ�Ϊ��������һ��Ҫд�ĵ�ַ���� */
      W25Q128_Write_Page(pBuffer, WriteAddr, count);
			
			/* ���������ظ���ַ�������� */
      WriteAddr +=  count;
      pBuffer += count;
			/*������ҳ��д��*/
      while (NumOfPage--)
      {
        W25Q128_Write_Page(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      if (NumOfSingle != 0)
      {
        W25Q128_Write_Page(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}
//��������оƬ		  
//�ȴ�ʱ�䳬��...
void W25Q128_Erase_Chip(void)   
{                                   
    W25Q128_Write_Enable();                  //SET WEL 
    W25Q128_Wait_Busy();   
  	W25Q128_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(W25X_ChipErase);        //����Ƭ��������  
	W25Q128_CS=1;                            //ȡ��Ƭѡ     	      
	W25Q128_Wait_Busy();   				   //�ȴ�оƬ��������
}   
//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ��ɽ��������ʱ��:150ms
void W25Q128_Erase_Sector(u32 Dst_Addr)   
{  
	//����falsh�������,������   
		printf("fe:%x\r\n",Dst_Addr);	  
		Dst_Addr*=4096;
    W25Q128_Write_Enable();                  //SET WEL 	 
    W25Q128_Wait_Busy();   
  	W25Q128_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(W25X_SectorErase);      //������������ָ�� 
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>16));  //����24bit��ַ    
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPI1_ReadWriteByte((u8)Dst_Addr);  
		W25Q128_CS=1;                            //ȡ��Ƭѡ     	      
    W25Q128_Wait_Busy();   				   //�ȴ��������
}  
//�ȴ�����
void W25Q128_Wait_Busy(void)   
{   
	while((W25Q128_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}  
//�������ģʽ
void W25Q128_PowerDown(void)   
{ 
  	W25Q128_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(W25X_PowerDown);        //���͵�������  
		W25Q128_CS=1;                            //ȡ��Ƭѡ     	      
    delay_us(3);                               //�ȴ�TPD  
}   
//����
void W25Q128_WAKEUP(void)   
{  
  	W25Q128_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
		W25Q128_CS=1;                            //ȡ��Ƭѡ     	      
    delay_us(3);                               //�ȴ�TRES1
}   

























