#include "stm32f10x.h"                  // Device header
#include "delay.h"


int main(void)
{
	SystemInit();
	delay_init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	while (1)
	{

	}
}
