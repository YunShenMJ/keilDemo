#ifndef __AD_H
#define __AD_H

extern uint16_t AD_Value[2];

void AD_Init(void);

void ADC_Filter_Update(void);
extern float voltage1;
extern float voltage2;

#endif
