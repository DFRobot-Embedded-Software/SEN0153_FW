/*!
 * @file        apm32f0xx_int.c
 *
 * @brief       Main Interrupt Service Routines
 *
 * @version     V1.0.0
 *
 * @date        2022-09-29
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
#include "apm32f0xx_int.h"
#include "all_include.h"

/** @addtogroup Examples
  @{
  */

  /** @addtogroup USART_Interrupt
    @{
    */

    /** @defgroup USART_Interrupt_INT_Functions INT_Functions
      @{
      */

      /*!
       * @brief        This function handles NMI exception
       *
       * @param        None
       *
       * @retval       None
       *
       * @note
       */
void NMI_Handler(void)
{
  printf("NMI_Handler\r\n");
}

/*!
 * @brief        This function handles Hard Fault exception
 *
 * @param        None
 *
 * @retval       None
 *
 * @note
 */
void HardFault_Handler(void)
{
  printf("HardFault_Handler\r\n");
  GPIO_SetBit(LED_PORT, LED_PIN);
  while (1);
}

/*!
 * @brief        This function handles SVCall exception
 *
 * @param        None
 *
 * @retval       None
 *
 * @note
 */
void SVC_Handler(void)
{
  printf("SVC_Handler\r\n");
}

/*!
 * @brief        This function handles PendSV_Handler exception
 *
 * @param        None
 *
 * @retval       None
 *
 * @note
 */
void PendSV_Handler(void)
{
  printf("PendSV_Handler\r\n");
}

/*!
 * @brief        This function handles SysTick Handler
 *
 * @param        None
 *
 * @retval       None
 *
 * @note
 */
void SysTick_Handler(void)
{
  // LEU0_RX_Time++;
}

/*!
 * @brief        This function handles TMR14 Handler
 *
 * @param        None
 *
 * @retval       None
 *
 * @note
 */
void TMR14_IRQHandler(void)
{
  if (TMR_ReadIntFlag(TMR14, TMR_INT_FLAG_UPDATE) == SET) {
    TMR_ClearIntFlag(TMR14, TMR_INT_FLAG_UPDATE);
    if (revBuf.revTimeout)
      revBuf.revTimeout--;
    else {
      revBuf.revEmptyFlag = FALSE;
      // revBuf.revByteCounter = 0;
    }
  }
}

/*!
 * @brief        This function handles USART1 RX interrupt Handler
 *
 * @param        None
 *
 * @retval       None
 *
 * @note
 */
void USART1_IRQHandler(void)
{
  if (USART_ReadStatusFlag(USART1, USART_FLAG_RXBNE) == SET) {
    // uint8_t dat = (uint8_t)USART_RxData(USART1);
    // printf("%c", dat);

    uint8_t Rev = (uint8_t)USART_RxData(USART1);
    if (revBuf.revEmptyFlag == TRUE) {
      revBuf.dataBuf[revBuf.revByteCounter] = Rev;
      revBuf.revByteCounter++;
      TMR_Enable(TMR14);
      if (Baudrate >= Baudrate_DEFAULT)
        revBuf.revTimeout = 4; // 2ms timeout 
      else
        revBuf.revTimeout = (uint8_t)(4.0 * 1000.0 * 10 / Baudrate);//4 * 1BYTE tx time
      if (revBuf.revByteCounter == RxBufferSize) {
        revBuf.revByteCounter = 0;
        TMR_Disable(TMR14); //turn off timer4
      }
    }
  }
}

volatile uint8_t IRQ_Counter = 0;

uint16_t TIMER2_IF[6], TIMER2_CO[6];
uint16_t Temp = 11;
volatile uint16_t Timer3_IF = 0;
void ADC1_COMP_IRQHandler(void)
{
  // if ((EINT_ReadIntFlag(EINT_LINE21) != RESET) || (EINT_ReadIntFlag(EINT_LINE22) != RESET)) {
  //   EINT_ClearIntFlag(EINT_LINE21);
  //   EINT_ClearIntFlag(EINT_LINE22);
  //   TIMER2_IF[IRQ_Counter] = TIMER2_IF_Counter;
  //   TIMER2_CO[IRQ_Counter] = TMR_ReadCounter(TMR2);
  //   if (IRQ_Counter > 0) {
  //     if (((TIMER2_IF[IRQ_Counter] - TIMER2_IF[IRQ_Counter - 1]) > 0) ||
  //       (TIMER2_CO[IRQ_Counter] > TIMER2_CO[IRQ_Counter - 1] ? (TIMER2_CO[IRQ_Counter] - TIMER2_CO[IRQ_Counter - 1]) > 160:0)) {
  //       IRQ_Counter = 0;
  //       for (uint8_t i = 0;i < 6;i++) { TIMER2_IF[i] = 0;TIMER2_CO[i] = 0; }
  //       Temp = CMD_Switch;
  //     } else if (IRQ_Counter++ > 4)//----------------干扰滤除指示，接收4个连续回波表示数据有效
  //     {
  //       Distance = (uint16_t)((TIMER2_CO[0] + TIMER2_IF[0] * 4800LL) * Vspeed / 1000000 / 2);
  //       // ACMP0->CTRL &= ~1;
  //       TMR_Disable(TMR2);
  //       TMR_Disable(TMR3);
  //       IRQ_Counter = 0;
  //       for (uint8_t i = 0;i < 6;i++) { TIMER2_IF[i] = 0;TIMER2_CO[i] = 0; }
  //       TIMER3_IF_Counter = 6500;
  //       DisTrue_Flag = TRUE;
  //     }
  //   }

  //   else {
  //     IRQ_Counter++;
  //   }

  // }
}

void TMR2_IRQHandler(void)
{
  if (TMR_ReadIntFlag(TMR2, TMR_INT_FLAG_UPDATE) == SET) {
    TMR_ClearIntFlag(TMR2, TMR_INT_FLAG_UPDATE);
    TIMER2_IF_Counter++;
  }
  if (TMR_ReadIntFlag(TMR2, TMR_INT_FLAG_CH4) == SET) {
    TMR_ClearIntFlag(TMR2, TMR_INT_FLAG_CH4);
    TIMER2_CO[IRQ_Counter] = TMR_ReadCaputer4(TMR2);
    TIMER2_IF[IRQ_Counter] = TIMER2_IF_Counter;
    if (IRQ_Counter > 0) {
      // 如果时间间隔太长, 就当做无效数据
      if (((TIMER2_IF[IRQ_Counter] - TIMER2_IF[IRQ_Counter - 1]) > 0) ||
        (TIMER2_CO[IRQ_Counter] > TIMER2_CO[IRQ_Counter - 1] ? (TIMER2_CO[IRQ_Counter] - TIMER2_CO[IRQ_Counter - 1]) > 160:0)) {
        IRQ_Counter = 0;
        for (uint8_t i = 0;i < 6;i++) { TIMER2_IF[i] = 0;TIMER2_CO[i] = 0; }
        Temp = CMD_Switch;
        // printf("c\r\n");
      } else if (IRQ_Counter++ > 3) {   //----------------干扰滤除指示，接收4个连续回波表示数据有效
        Distance = (uint16_t)((TIMER2_CO[0] + TIMER2_IF[0] * 4800LL) * Vspeed / 2000000);
        // ACMP0->CTRL &= ~1;
        COMP_Disable(COMP_SELECT_COMP1);
        COMP_Disable(COMP_SELECT_COMP2);
        TMR_Disable(TMR2);
        TMR_Disable(TMR3);
        TMR_DisableCCxChannel(TMR2, TMR_CHANNEL_4);
        TMR_ClearIntFlag(TMR2, TMR_INT_FLAG_UPDATE | TMR_INT_CH4);
        TMR_ClearIntFlag(TMR3, TMR_INT_FLAG_UPDATE);

        printf("Distance = %u\r\n", Distance);
        printf("TIMER2_IF[0] = %u\r\n", TIMER2_IF[0]);
        printf("TIMER2_CO[0] = %u\r\n", TIMER2_CO[0]);
        printf("TIMER3_IF_Counter = %u\r\n", TIMER3_IF_Counter);
        IRQ_Counter = 0;
        for (uint8_t i = 0;i < 6;i++) { TIMER2_IF[i] = 0;TIMER2_CO[i] = 0; }
        // Timer3_IF = TIMER3_IF_Counter;
        TIMER3_IF_Counter = 6500;
        DisTrue_Flag = TRUE;
      }
    } else {
      IRQ_Counter++;
    }
  }
}

void TMR3_IRQHandler(void)
{
  // if (TMR_ReadIntFlag(TMR3, TMR_INT_FLAG_CH1) == SET) {
  //   TMR_ClearIntFlag(TMR3, TMR_INT_FLAG_CH1);
  //   printf("TMR3 = %u \r\n", TMR_ReadCaputer1(TMR3));
  //   // TMR_ReadCaputer4(TMR2);

  // }
  if (TMR_ReadIntFlag(TMR3, TMR_INT_FLAG_UPDATE) == SET) {
    TMR_ClearIntFlag(TMR3, TMR_INT_FLAG_UPDATE);
    TIMER3_IF_Counter++;
    if (shift_ch_CMD == TRUE) {
      switch (TIMER3_IF_Counter) {
      case  15: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(1200));
        Var_Sub = 28; CMD_Switch = 1;   // 使用通道0
        // ACMP_ChannelSet(ACMP0, acmpChannelDAC0Ch0, acmpChannel0);//---------比较器输入设置
        // NVIC_ClearPendingIRQ(ACMP0_IRQn);//--------------------------------清除中断挂起
        // NVIC_EnableIRQ(ACMP0_IRQn);
        // ACMP_IntEnable(ACMP0, ACMP_IEN_EDGE);
        // ACMP_IntClear(ACMP0, ACMP_IFC_EDGE);	//清除中断标记
        // ACMP_Enable(ACMP0);
        COMP_Enable(COMP_SELECT_COMP2);
        TMR_EnableCCxChannel(TMR2, TMR_CHANNEL_4);
        TMR_EnableInterrupt(TMR2, TMR_INT_CH4);
        TMR_ClearIntFlag(TMR2, TMR_INT_FLAG_CH4);
        TMR_ClearStatusFlag(TMR2, TMR_INT_FLAG_CH4 | TMR_FLAG_CH4OC);

      }break;
      case  16: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(1000));
        Var_Sub = 27; CMD_Switch = 2;
      }break;
      case  17: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(800));
        Var_Sub = 26; CMD_Switch = 3;
      }break;
      case  18: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(600));
        Var_Sub = 26; CMD_Switch = 4;
      }break;
      case  19: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(500));
        Var_Sub = 25; CMD_Switch = 5;
      }break;
      case  21: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(400));
        Var_Sub = 25; CMD_Switch = 6;
      }break;
      case  22: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(300));
        Var_Sub = 24; CMD_Switch = 7;
      }break;
      case  23: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(200));
        Var_Sub = 24; CMD_Switch = 8;
      }break;
      case  24: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(150));
        Var_Sub = 23; CMD_Switch = 9;
      }break;
      case  28: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(100));
        Var_Sub = 23; CMD_Switch = 10;
      }break;
      case  31: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(80));
        Var_Sub = 23; CMD_Switch = 11;
      }break;
      case  36: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(60));
        Var_Sub = 23; CMD_Switch = 12;
      }break;
      case  40: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(40));
        Var_Sub = 22; CMD_Switch = 13;
      }break;
      case  80: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(20));
        Var_Sub = 21; CMD_Switch = 14;
      }break;

      case 100: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(500));
        Var_Sub = 21;CMD_Switch = 15;
        // ACMP_ChannelSet(ACMP0, acmpChannelDAC0Ch1, acmpChannel1);//---------比较器输入设置
        // NVIC_ClearPendingIRQ(ACMP0_IRQn);//--------------------------------清除中断挂起
        // NVIC_EnableIRQ(ACMP0_IRQn);
        // ACMP_IntEnable(ACMP0, ACMP_IEN_EDGE);
        // ACMP_IntClear(ACMP0, ACMP_IFC_EDGE);	//清除中断标记
        // ACMP_Enable(ACMP0);
        COMP_Enable(COMP_SELECT_COMP1);   // 初始化模拟比较器1
      }break;
      case 102: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(450));
        Var_Sub = 21; CMD_Switch = 16;
      }break;
      case 104: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(400));
        Var_Sub = 21; CMD_Switch = 17;
      }break;
      case 106: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(350));
        Var_Sub = 21; CMD_Switch = 18;
      }break;
      case 108: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(300));
        Var_Sub = 20; CMD_Switch = 19;
      }break;
      case 110: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(250));
        Var_Sub = 20; CMD_Switch = 20;
      }break;
      case 115: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(200));
        Var_Sub = 20; CMD_Switch = 21;
      }break;
      case 120: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(150));
        Var_Sub = 20; CMD_Switch = 22;
      }break;
      case 125: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(100));
        Var_Sub = 19; CMD_Switch = 23;
      }break;
      case 180: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(100));
        Var_Sub = 18; CMD_Switch = 24;
      }break;
      case 200: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(100));
        Var_Sub = 17; CMD_Switch = 25;
      }break;
      case 240: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(80));
        Var_Sub = 15; CMD_Switch = 26;
      }break;
      case 300: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(80));
        Var_Sub = 15; CMD_Switch = 27;
      }break;
      case 320: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(80));
        Var_Sub = 14; CMD_Switch = 28;
      }break;
      case 380: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(80));
        Var_Sub = 13; CMD_Switch = 29;
      }break;
      case 430: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(80));
        Var_Sub = 12; CMD_Switch = 30;
      }break;
      case 510: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(60));
        Var_Sub = 10; CMD_Switch = 31;
      }break;
      case 570: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(60));
        Var_Sub = 9; CMD_Switch = 32;
      }break;
      case 620: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(50));
        Var_Sub = 8; CMD_Switch = 33;
      }break;
      case 700: {
        DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, COMP_INVERTINGINPUT_TH_VOLT(45));
        Var_Sub = 7; CMD_Switch = 34;
      }break;
      default:break;
      }
    }
  }
}

/**@} end of group USART_Interrupt_INT_Functions */
/**@} end of group USART_Interrupt */
/**@} end of group Examples */
