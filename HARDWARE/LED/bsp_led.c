#include "bsp_led.h"

void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	//�򿪵Ƶ�ʱ��
	LED_CLOCK_Cmd(LED_CLOCK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=LED0_PIN|LED1_PIN;//����LED�Ƶ�����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//ģʽΪ���
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//��������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;//�ٶ�Ϊ100MHZ
	
	GPIO_Init(LED_PORT,&GPIO_InitStructure);//GPIO��ʼ��
	
	GPIO_SetBits(LED_PORT,LED0_PIN|LED1_PIN);//����Ϩ��
}

