#include "bsp_key.h"

void KEY_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//开按键的时钟
	KEY_ClOCK_Cmd(KEY0_CLOCK|KEY1_CLOCK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=KEY0_PIN|KEY1_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
}

int Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==KEY_ON)
	{
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==KEY_ON);
		
		return KEY_ON;
	}
	else
		return KEY_OFF;
}

