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

  APM_EVAL_TMR14_Init(500, 48);   // 0.5 ms

  clear_RxMessage();   // 清空协议Buffer

  GPIO_ClearBit(LED_PORT, LED_PIN);   // LED指示初始化完成

  // DAC_setup();
  // APM_EVAL_DelayMs(1000);
  CUSTOM_DBG("SEN0153 URM07!\r\n");

  // 电源管理时钟初始化
  // PMU_Reset();
  RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_PMU);

  while (1) {
    // uint16_t dis = get_Dis();
    // CUSTOM_DBG("get_Dis() = %u\r\n\r\n", dis);
    // APM_EVAL_DelayMs(300);
    if (revBuf.revEmptyFlag == FALSE) {   // 否定  接收 buf 不为空
      TMR_Disable(TMR14);
      DF_Protocol_Parse();
      clear_RxMessage();   // 清空协议Buffer
    }
    // GPIO_SetInputFloat()

    /* Before entering the USART in STOP mode the REACK flag must be checked to ensure the USART RX is ready */
  //   while (USART_ReadStatusFlag(USART1, USART_FLAG_RXENACKF) == RESET);
  //   USART_EnableStopMode(USART1);
  //   USART_EnableInterrupt(USART1, USART_INT_WAKEUP);

    // APM_EVAL_DelayMs(1);
  //   /* Enter Sleep Mode, and Wake up by interrupt*/
    PMU_EnterSleepMode(PMU_SLEEPENTRY_WFI);
  //   /* Enter STOP Mode in WFI*/
  // RCM_DisableAPB2PeriphClock(RCM_APB2_PERIPH_SYSCFG);
    // PMU_EnterSTOPMode(PMU_REGULATOR_LowPower, PMU_STOPENTRY_WFI);
  // RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SYSCFG);
  //   /* Clear flag bit to prevent repeated entry interrupt*/
  //   // PMU_ClearStatusFlag(PMU_FLAG_STDBYF);
  //   // PMU_ClearStatusFlag(PMU_FLAG_WUPF);
  //   /* Enter StandBy Mode*/
    // PMU_EnterSTANDBYMode();
  //   /* reset System*/
  //   // NVIC_SystemReset();

  //   /* Waiting Wake Up interrupt */
  //   while (InterruptCounter == 0x00);
  //   USART_DisableStopMode(USART1);
  //   // USART1_Init();   // 初始化串口
  //   CUSTOM_DBG("4123456789!\r\n");
    // CUSTOM_DBG("123456789!\r\n");
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
