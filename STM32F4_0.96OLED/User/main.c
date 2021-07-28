#include "stm32f4xx.h"
#include "delay.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_oled.h"
#include "bsp_oledinit.h"

int main()
{
	delay_init(168);
	KEY_GPIO_Config();
	LED_GPIO_Config();
	OLED_Init();
	OLED_SetCHFont(&HUAWENCAIYUN40x32);
	OLED_DisplayTurn(0);
	OLED_ColorTurn(0);
	OLED_Clear();
	
	OLED_DispString_CH(0,0,"ÎÒ°®Äã");
	OLED_Refresh();
	
	while(1)
	{
		if(Key_Scan(KEY0_PORT,KEY0_PIN)==KEY_ON)
			LED0_TOGGLE;
		if(Key_Scan(KEY1_PORT,KEY1_PIN)==KEY_ON)
			LED1_TOGGLE;
	}
}


