#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"
#include "Serial.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>

#define VoltageMax 		3.3 //设置PWM为0-VoltageMax电压幅度
#define playbackSpeed	5	//播放速度
#define period			200	//播放周期

#define DebugMODE			1//1-调试	0-发行

/*
PA0-----第一路PWM捕获
PA6-----第二路PWM捕获
PB6-----PWM波形发射器
@显示
PA9-----串口屏(串口RXD)
PA10-----串口屏(串口TXD)
@调试
PB8-----OLED屏幕调试-SCL
PB8-----OLED屏幕调试-SDA
*/


uint8_t TdState =0;
uint8_t PWMSate =0;
uint8_t ClearFlag =0;
float GetVoltageFromDuty(uint8_t duty_percent, float Vhigh);

int main(void)
{
	SystemInit();
	delay_init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	OLED_Init();
	PWM_Init();
	IC_Init();
	Serial_Init(115200);
#if DebugMODE
	OLED_ShowString(1, 1, "Freq:00000Hz");		//频率
	OLED_ShowString(2, 1, "Duty:00%");			//占空比
//	OLED_ShowString(2, 1, "Voltage:");		//电压
	OLED_ShowString(3, 1, "Freq:00000Hz");		
	OLED_ShowString(4, 1, "Duty:00%");			
//	OLED_ShowString(4, 1, "Voltage:");
#endif
	
	Serial_Printf("t2.txt=\"%.1f\"\xff\xff\xff",VoltageMax);
	Serial_Printf("t11.txt=\"%.1f\"\xff\xff\xff",VoltageMax*4/5);
	Serial_Printf("t12.txt=\"%.1f\"\xff\xff\xff",VoltageMax*3/5);
	Serial_Printf("t13.txt=\"%.1f\"\xff\xff\xff",VoltageMax*2/5);
	Serial_Printf("t14.txt=\"%.1f\"\xff\xff\xff",VoltageMax/5);
	Serial_Printf("t15.txt=\"0\"\xff\xff\xff");
	
	while (1)
	{	
#if DebugMODE
		OLED_ShowNum(1, 6, IC1_GetFreq(), 5);//频率
		OLED_ShowNum(2, 6, IC1_GetDuty(), 2);	//占空比
		OLED_ShowNum(2, 10, GetVoltageFromDuty(IC1_GetDuty(), VoltageMax)*1000, 4);//电压
		OLED_ShowNum(3, 6, IC2_GetFreq(), 5);	
		OLED_ShowNum(4, 10, IC2_GetDuty(), 2);
		OLED_ShowNum(4, 10, GetVoltageFromDuty(IC2_GetDuty(), VoltageMax)*1000, 4);
#endif
		if(!ClearFlag)
		{
			if(!PWMSate)//电压表
			{
				Serial_Printf("t2.txt=\"%.1f\"\xff\xff\xff",VoltageMax);
				Serial_Printf("t11.txt=\"%.1f\"\xff\xff\xff",VoltageMax*4/5);
				Serial_Printf("t12.txt=\"%.1f\"\xff\xff\xff",VoltageMax*3/5);
				Serial_Printf("t13.txt=\"%.1f\"\xff\xff\xff",VoltageMax*2/5);
				Serial_Printf("t14.txt=\"%.1f\"\xff\xff\xff",VoltageMax/5);
				Serial_Printf("t15.txt=\"0\"\xff\xff\xff");
			}else{//示波器
				Serial_Printf("t2.txt=\"%\"\xff\xff\xff");
				Serial_Printf("t11.txt=\"\"\xff\xff\xff");
				Serial_Printf("t12.txt=\"\"\xff\xff\xff");
				Serial_Printf("t13.txt=\"\"\xff\xff\xff");
				Serial_Printf("t14.txt=\"\"\xff\xff\xff");
				Serial_Printf("t15.txt=\"\"\xff\xff\xff");
			}
		}
		
		if(!TdState)
		{
			Serial_Printf("t16.txt=\"%.2fV\"\xff\xff\xff",GetVoltageFromDuty(IC1_GetDuty(), VoltageMax));
			Serial_Printf("t9.txt=\"%dHZ\"\xff\xff\xff",IC1_GetFreq());
			Serial_Printf("t10.txt=\"%d%s\"\xff\xff\xff",IC1_GetDuty(),"%");
		}else{
			Serial_Printf("t16.txt=\"%.2fV\"\xff\xff\xff",GetVoltageFromDuty(IC2_GetDuty(), VoltageMax));
			Serial_Printf("t9.txt=\"%dHZ\"\xff\xff\xff",IC2_GetFreq());
			Serial_Printf("t10.txt=\"%d%s\"\xff\xff\xff",IC2_GetDuty(),"%");
		}
		
		if(!PWMSate)
		{
			Serial_Printf("add 1,0,%d\xff\xff\xff",255*IC1_GetDuty()/100);
			Serial_Printf("add 1,1,%d\xff\xff\xff",255*IC2_GetDuty()/100);	
		}else{
			uint8_t PWM_Hight1 = IC1_GetDuty() * period / 100;
			uint8_t PWM_Hight2 = IC2_GetDuty() * period / 100;

			for (int i = 0; i < period; i++)
			{
				delay_ms(playbackSpeed);

				// 路1
				if (i < PWM_Hight1)
					Serial_Printf("add 1,0,200\xff\xff\xff");  // 高电平
				else
					Serial_Printf("add 1,0,50\xff\xff\xff");   // 低电平

				// 路2
				if (i < PWM_Hight2)
					Serial_Printf("add 1,1,200\xff\xff\xff");  // 高电平
				else
					Serial_Printf("add 1,1,50\xff\xff\xff");   // 低电平
			}
		}
		
		
		
		if (Serial_RxFlag == 1)		//如果接收到数据包
		{
#if DebugMODE
//			OLED_ShowString(4, 1, "                ");
//			OLED_ShowString(4, 1, Serial_RxPacket);		
#endif
			if (strcmp(Serial_RxPacket, "TD2") == 0)
			{
				TdState =1;
			}else if (strcmp(Serial_RxPacket, "TD1") == 0)
			{
				TdState =0;
			}else if (strcmp(Serial_RxPacket, "PWMV") == 0)	
			{
				PWMSate =0;
				ClearFlag =0;
			}else if (strcmp(Serial_RxPacket, "PWM") == 0)	
			{
				PWMSate =1;
				ClearFlag =0;
			}else if (strncmp(Serial_RxPacket, "Hz:", 3) == 0) {
				uint16_t freq = 0;
				uint16_t duty = 0;
				// 解析形如 "Hz:%d:%d" 的命令
				if (sscanf(Serial_RxPacket, "Hz:%hu:%hu", &freq, &duty) == 2) {
					PWM_SetPrescaler(freq);
					PWM_SetCompare1(duty);
				}
			}
			
			Serial_RxFlag = 0;			//处理完
		}
		
	}
}


float GetVoltageFromDuty(uint8_t duty_percent, float Vhigh)//占空比， 电压
{
    return (duty_percent / 100.0f) * Vhigh;
}
