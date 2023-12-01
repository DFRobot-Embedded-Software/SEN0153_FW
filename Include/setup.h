#ifndef __SETUP_H
#define __SETUP_H

#include "all_include.h"


void TIMER2_setup(uint16_t period, uint16_t div);
void TIMER3_setup(uint16_t period, uint16_t div);
void APM_EVAL_TMR14_Init(uint16_t period, uint16_t div);
void GPIO_setup(void);
void DAC_setup(void);
void ACMP_setup(void);
void ADC_setup(void);
void USART1_Init(void);

#endif /* __EFM_TG_SETUP_H */


