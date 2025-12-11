#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "OLED.h"
#include "PWM.h"

int main(void)
{
	SystemInit();
	delay_init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	OLED_Init();
	
	
	OLED_ShowString(1, 1, "Hello");
	while (1)
	{

	}
}
