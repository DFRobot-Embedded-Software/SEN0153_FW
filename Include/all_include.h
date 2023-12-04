#ifndef __ALL_INCLUDE_H__
#define __ALL_INCLUDE_H__

#include "main.h"

#include "setup.h"
#include "DF_Protocol.h"
#include "func.h"
#include "flash.h"

#define THIS_USE_TMR1
#ifdef THIS_USE_TMR1
#define COMP_OUTPUT_SET   COMP_OUTPUT_TIM1IC1
#define USE_TMRx   TMR1
#define TIMERx_setup(x, y)   {TIMER1_setup(x, y);}
#else
#define COMP_OUTPUT_SET   COMP_OUTPUT_TIM2IC4
#define USE_TMRx   TMR2
#define TIMERx_setup(x, y)   {TIMER2_setup(x, y);}
#endif

#define COMP_INVERTINGINPUT_TH_VOLT(x)   ( (uint16_t)((x) * 4096.0 / 3.3 ) )

// #define COMP_INPUT_TH(x)   ( (uint16_t)((x) * 0.3788 * 8) )   // 1250 / 3300
#define COMP_INPUT_TH(x)   ( (uint16_t)((x) * 1250 / 3300 * 3) )   // 1.25V 范围的 1200/4096
// #define COMP_INPUT_TH(x)   ( (uint16_t)((x) * 4096.0 / 3300 ) )   // 1200 mV
// #define COMP_INPUT_TH(x)   ( x )   // 1200

// #define SET_DAC_INM(x)   {DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INPUT_TH(x));}
// #define SET_DAC_INM(x)   
#define SET_DAC_INM(x)   {DAC->DH12R1 = COMP_INPUT_TH(x);}
// #define SET_DAC_INM(x)   {DAC->DH12R1 = x;}

#define Baudrate_DEFAULT       19200
#define device_Addr_DEFAULT    0x11
#define factory_Flag_DEFAULT   0xDF+20160830

#define BootMode   0x55AA
#define RunMode    0xAA55

#define TRUE    ((uint8_t)1)
#define FALSE   ((uint8_t)0)

#define GPIO_SetInputFloat(GPIOx, Pinx)   {\
                                            gpioConfig.mode  = GPIO_MODE_IN;\
                                            gpioConfig.speed = GPIO_SPEED_50MHz;\
                                            gpioConfig.outtype = GPIO_OUT_TYPE_PP;\
                                            gpioConfig.pupd  = GPIO_PUPD_NO;\
                                            gpioConfig.pin   = Pinx;\
                                            \
                                            GPIO_Config(GPIOx, &gpioConfig);\
                                          }

#define GPIO_SetOutPP(GPIOx, Pinx)        {\
                                            gpioConfig.mode  = GPIO_MODE_OUT;\
                                            gpioConfig.speed = GPIO_SPEED_50MHz;\
                                            gpioConfig.outtype = GPIO_OUT_TYPE_PP;\
                                            gpioConfig.pupd  = GPIO_PUPD_NO;\
                                            gpioConfig.pin   = Pinx;\
                                            \
                                            GPIO_Config(GPIOx, &gpioConfig);\
                                            GPIOx->BR = Pinx;\
                                          }

/*****************************************************************************************************************
 *超声波探头用到的IO口:  输入IO----PB3
 *                      接地IO----PB1
 *****************************************************************************************************************/
#define X_PORT   GPIOB
#define X_PIN    GPIO_PIN_3
#define Y_PORT   GPIOB
#define Y_PIN    GPIO_PIN_1

 /*****************************************************************************************************************
 *运放供电用到的IO口:----PB0   (原PC15, 现在拉低供电)
 *****************************************************************************************************************/
#define OPA_VCC_PORT   GPIOB
#define OPA_VCC_PIN    GPIO_PIN_0

 /*****************************************************************************************************************
  *PA4_DAC_OUT用到的IO口:  DAC1----PA4
  *****************************************************************************************************************/
#define DAC1_OUT_PORT   GPIOA
#define DAC1_OUT_PIN    GPIO_PIN_4

  /*****************************************************************************************************************
  *ACMP1用到的IO口:  ACMP1_CH1----PA0_COMP1_INM6   (原PC0)
  *                  ACMP1_CH1----PA1_COMP1_INP   (原PC1)
  *ACMP2用到的IO口:  ACMP2_CH2----PA2_COMP2_INM6
  *                  ACMP2_CH2----PA3_COMP2_INP
  *ACMP_OUT用到的IO口:  ACMP_Out----PA11-T7   PA12-T6   (原PD6)
  *****************************************************************************************************************/
#define ACMP1_INM_PORT   GPIOA   //-----------模拟比较器1输入 基准 引脚
#define ACMP1_INM_PIN    GPIO_PIN_0
#define ACMP1_INP_PORT   GPIOA   //-----------模拟比较器1输入 信号 引脚
#define ACMP1_INP_PIN    GPIO_PIN_1

#define ACMP2_INM_PORT   GPIOA   //-----------模拟比较器2输入 基准 引脚
#define ACMP2_INM_PIN    GPIO_PIN_2
#define ACMP2_INP_PORT   GPIOA   //-----------模拟比较器2输入 信号 引脚
#define ACMP2_INP_PIN    GPIO_PIN_3

#define ACMP1_OUT_PORT   GPIOA   //-----------模拟比较器1输出引脚PA11
#define ACMP1_OUT_PIN    GPIO_PIN_11
#define ACMP2_OUT_PORT   GPIOA   //-----------模拟比较器2输出引脚PA12
#define ACMP2_OUT_PIN    GPIO_PIN_12

  /*****************************************************************************************************************
   *指示灯用到的IO口:----PA5
   *****************************************************************************************************************/
#define LED_PORT   GPIOA
#define LED_PIN    GPIO_PIN_5

   /*****************************************************************************************************************
    *温度测量用到的IO口:  供电----PB2
    *                    接地----
    *ADC输入引脚:----PA6   ADC_IN6
    *****************************************************************************************************************/
#define TEMP_VCC_PORT  GPIOB
#define TEMP_VCC_PIN   GPIO_PIN_2

#define ADC1_IN6_PORT GPIOA
#define ADC1_IN6_PIN  GPIO_PIN_6

    /*****************************************************************************************************************
     *USART1用到的IO口:  TX0----PB6
     *                    RX0----PB7
     *****************************************************************************************************************/
#define USART1_TX_PORT   GPIOB
#define USART1_TX_PIN    GPIO_PIN_6
#define USART1_RX_PORT   GPIOB
#define USART1_RX_PIN    GPIO_PIN_7


extern uint32_t Baudrate;
extern uint8_t  device_Addr;

extern volatile uint32_t Vspeed;   // 单位为cm/s

extern volatile uint16_t TIMERx_IF_Counter;
extern volatile uint16_t TIMER3_IF_Counter;
extern volatile uint16_t Distance;
extern volatile uint8_t DisTrue_Flag;   // 距离获取正确的标志

#endif /* __ALL_INCLUDE_H__ */
