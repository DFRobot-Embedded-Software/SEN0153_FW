/*!
 * @file        main.c
 *
 * @brief       Main program body
 *
 * @version     V1.0.0
 *
 * @date        2023-11-17
 *
 * @attention
 *
 *  Copyright (C) 2022 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be useful and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */

 /* Includes */
#include "main.h"
#include "all_include.h"

/*!
 * @brief       Main program
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
int main(void)
{
  /* Init delay function */
  APM_EVAL_DelayInit();   // 最大支持延时时间 : 1 << 24 / 6000 ≈ 2796.2 ms

  RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SYSCFG);

  GPIO_setup();   // 初始化GPIO
  GPIO_SetBit(LED_PORT, LED_PIN);   // LED指示灯输出高电平表示开始初始化芯片

  Restore_Check();   // 检测TX与RX端口上的电平，判断是否需要恢复出厂设置

  USART1_Init();   // 初始化串口

  Parameters_Setup();   // 检查参数，程序第一次运行时初始化出厂默认参数

#if IAP_UART
  appFlagInit();
#endif
// GPIO_Config_T gpioConfig;
  // GPIO_SetOutPP(DAC1_OUT_PORT, DAC1_OUT_PIN);
  // GPIO_SetBit(DAC1_OUT_PORT, DAC1_OUT_PIN);
  // GPIO_ClearBit(DAC1_OUT_PORT, DAC1_OUT_PIN);
  // gpioConfig.mode = GPIO_MODE_AN;
  // gpioConfig.outtype = GPIO_OUT_TYPE_PP;
  // gpioConfig.speed = GPIO_SPEED_10MHz;
  // gpioConfig.pupd = GPIO_PUPD_NO;
  // gpioConfig.pin = DAC1_OUT_PIN;
  // GPIO_Config(DAC1_OUT_PORT, &gpioConfig);

  APM_EVAL_TMR14_Init(500, 48);   // 0.5 ms

  clear_RxMessage();   // 清空协议Buffer

  GPIO_ClearBit(LED_PORT, LED_PIN);   // LED指示初始化完成

  DAC_setup();
  APM_EVAL_DelayMs(1000);
  printf("SEN0153 URM07!\r\n");

  uint16_t dis = 0;
  while (1) {
    dis = get_Dis();
    printf("get_Dis() = %u\r\n\r\n", dis);
    APM_EVAL_DelayMs(300);
    if (revBuf.revEmptyFlag == FALSE) {   // 否定  接收 buf 不为空
      TMR_Disable(TMR14);
      DF_Protocol_Parse();
      clear_RxMessage();   // 清空协议Buffer
    }
  }
}

/*!
 * @brief       Redirect C Library function printf to serial port.
 *              After Redirection, you can use printf function.
 *
 * @param       ch:  The characters that need to be send.
 *
 * @param       *f:  pointer to a FILE that can recording all information
 *              needed to control a stream
 *
 * @retval      The characters that need to be send.
 */
int fputc(int ch, FILE* f)
{
  /** send a byte of data to the serial port */
  USART_TxData(DEBUG_USART, (uint8_t)ch);

  /** wait for the data to be send  */
  while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);

  return (ch);
}
