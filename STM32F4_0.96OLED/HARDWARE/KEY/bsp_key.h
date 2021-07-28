#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_

#include "stm32f4xx.h"

#define KEY0_PORT										GPIOE
#define KEY1_PORT										GPIOE

#define KEY0_PIN										GPIO_Pin_4
#define KEY1_PIN										GPIO_Pin_3

#define KEY_ClOCK_Cmd								RCC_AHB1PeriphClockCmd
#define KEY0_CLOCK									RCC_AHB1Periph_GPIOE
#define KEY1_CLOCK									RCC_AHB1Periph_GPIOE

#define KEY_ON											0
#define KEY_OFF											1

void KEY_GPIO_Config(void);
int Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#endif	/*_BSP_KEY_H_*/

