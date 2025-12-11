#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;         // 1 MHz 计数频率
    TIM_TimeBaseInitStructure.TIM_Period = 50 - 1;            // 50 μs 周期
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

    TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);  // 产生触发输出
    TIM_Cmd(TIM1, ENABLE);
}
