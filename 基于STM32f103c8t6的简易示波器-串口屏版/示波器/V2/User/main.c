#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"

int main(void)
{
	SystemInit();
	delay_init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	OLED_Init();
	PWM_Init();
	IC_Init();
	
	OLED_ShowString(1, 1, "Freq:00000Hz");		//1行1列显示字符串Freq:00000Hz
	OLED_ShowString(2, 1, "Duty:00%");			//2行1列显示字符串Duty:00%
	OLED_ShowString(3, 1, "Freq:00000Hz");		//1行1列显示字符串Freq:00000Hz
	OLED_ShowString(4, 1, "Duty:00%");			//2行1列显示字符串Duty:00%
	PWM_SetPrescaler(72-1);
	while (1)
	{
		OLED_ShowNum(1, 6, IC1_GetFreq(), 5);	//不断刷新显示输入捕获测得的频率
		OLED_ShowNum(2, 6, IC1_GetDuty(), 2);	//不断刷新显示输入捕获测得的占空比
		OLED_ShowNum(3, 6, IC2_GetFreq(), 5);	//不断刷新显示输入捕获测得的频率
		OLED_ShowNum(4, 6, IC2_GetDuty(), 2);	//不断刷新显示输入捕获测得的占空比
	}
}
