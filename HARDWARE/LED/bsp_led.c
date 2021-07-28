#include "bsp_led.h"

void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	//打开灯的时钟
	LED_CLOCK_Cmd(LED_CLOCK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=LED0_PIN|LED1_PIN;//配置LED灯的引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//模式为输出
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//上拉输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;//速度为100MHZ
	
	GPIO_Init(LED_PORT,&GPIO_InitStructure);//GPIO初始化
	
	GPIO_SetBits(LED_PORT,LED0_PIN|LED1_PIN);//将灯熄灭
}

