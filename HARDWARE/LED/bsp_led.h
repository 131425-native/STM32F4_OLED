#ifndef _BSP_LED_H_
#define _BSP_LED_H_

#include "stm32f4xx.h"

#define LED_PORT										GPIOA
#define LED0_PIN										GPIO_Pin_6
#define LED1_PIN										GPIO_Pin_7
#define LED_CLOCK_Cmd								RCC_AHB1PeriphClockCmd
#define LED_CLOCK										RCC_AHB1Periph_GPIOA

#define LED0_ON											GPIO_ResetBits(LED_PORT,LED0_PIN);
#define LED0_OFF										GPIO_SetBits(LED_PORT,LED0_PIN);
#define LED1_ON											GPIO_ResetBits(LED_PORT,LED1_PIN);
#define LED1_OFF										GPIO_SetBits(LED_PORT,LED1_PIN);

#define digitalToggle(p,i) {p->ODR ^=i;} //Êä³ö·´×ª×´Ì¬

#define LED0_TOGGLE digitalToggle(LED_PORT,LED0_PIN)
#define LED1_TOGGLE digitalToggle(LED_PORT,LED1_PIN)

void LED_GPIO_Config(void);
#endif  /*_BSP_LED_H_*/


