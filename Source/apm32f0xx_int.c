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
uint16_t TIMERx_IF[6], TIMERx_CO[6];
void ADC1_COMP_IRQHandler(void)
{
}

#ifdef THIS_USE_TMR1
void TMR1_BRK_UP_TRG_COM_IRQHandler(void)
{
  if (TMR_ReadIntFlag(TMR1, TMR_INT_FLAG_UPDATE) == SET) {
    TMR_ClearIntFlag(TMR1, TMR_INT_FLAG_UPDATE);
    TIMERx_IF_Counter++;
  }
}

void TMR1_CC_IRQHandler(void)
{
  if (TMR_ReadIntFlag(TMR1, TMR_INT_FLAG_CH1) == SET) {
    TMR_ClearIntFlag(TMR1, TMR_INT_FLAG_CH1);
    TIMERx_CO[IRQ_Counter] = TMR_ReadCaputer1(TMR1);
    TIMERx_IF[IRQ_Counter] = TIMERx_IF_Counter;
    if (IRQ_Counter > 0) {
      // 如果时间间隔太长, 就当做无效数据
      if (((TIMERx_IF[IRQ_Counter] - TIMERx_IF[IRQ_Counter - 1]) > 0) ||
        (TIMERx_CO[IRQ_Counter] > TIMERx_CO[IRQ_Counter - 1] ? (TIMERx_CO[IRQ_Counter] - TIMERx_CO[IRQ_Counter - 1]) > 160:0)) {
        IRQ_Counter = 0;
        for (uint8_t i = 0;i < 6;i++) { TIMERx_IF[i] = 0;TIMERx_CO[i] = 0; }
      } else if (IRQ_Counter++ > 3) {   //----------------干扰滤除指示，接收4个连续回波表示数据有效
        IRQ_Counter = 0;
        DisTrue_Flag = TRUE;
        COMP_Disable(COMP_SELECT_COMP2);
        COMP_Disable(COMP_SELECT_COMP1);
        TMR_Disable(TMR1);
        TMR_Disable(TMR3);

        Distance = (uint16_t)((TIMERx_CO[0] + TIMERx_IF[0] * 4800LL) * Vspeed / 2000000);
        IRQ_Counter = 0;
        for (uint8_t i = 0;i < 6;i++) { TIMERx_IF[i] = 0;TIMERx_CO[i] = 0; }
        // TIMER3_IF_Counter = 6500;
        DisTrue_Flag = TRUE;
      }
    } else {
      IRQ_Counter++;
    }
  }
  
}
#else
void TMR2_IRQHandler(void)
{
  if (TMR_ReadIntFlag(TMR2, TMR_INT_FLAG_UPDATE) == SET) {
    TMR_ClearIntFlag(TMR2, TMR_INT_FLAG_UPDATE);
    TIMERx_IF_Counter++;
  }
  if (TMR_ReadIntFlag(TMR2, TMR_INT_FLAG_CH4) == SET) {
    TMR_ClearIntFlag(TMR2, TMR_INT_FLAG_CH4);
    TIMERx_CO[IRQ_Counter] = TMR_ReadCaputer4(TMR2);
    TIMERx_IF[IRQ_Counter] = TIMERx_IF_Counter;
    if (IRQ_Counter > 0) {
      // 如果时间间隔太长, 就当做无效数据
      if (((TIMERx_IF[IRQ_Counter] - TIMERx_IF[IRQ_Counter - 1]) > 0) ||
        (TIMERx_CO[IRQ_Counter] > TIMERx_CO[IRQ_Counter - 1] ? (TIMERx_CO[IRQ_Counter] - TIMERx_CO[IRQ_Counter - 1]) > 160:0)) {
        IRQ_Counter = 0;
        for (uint8_t i = 0;i < 6;i++) { TIMERx_IF[i] = 0;TIMERx_CO[i] = 0; }   // 原始采样数据清零
      } else if (IRQ_Counter++ > 3) {   // 干扰滤除指示，接收4个连续回波表示数据有效
        COMP_Disable(COMP_SELECT_COMP1);
        COMP_Disable(COMP_SELECT_COMP2);
        TMR_Disable(TMR2);
        TMR_Disable(TMR3);

        Distance = (uint16_t)((TIMERx_CO[0] + TIMERx_IF[0] * 4800LL) * Vspeed / 2000000);
        IRQ_Counter = 0;
        for (uint8_t i = 0;i < 6;i++) { TIMERx_IF[i] = 0;TIMERx_CO[i] = 0; }   // 原始采样数据清零
        // TIMER3_IF_Counter = 6500;
        DisTrue_Flag = TRUE;
      }
    } else {
      IRQ_Counter++;
    }
  }
}
#endif

void TMR3_IRQHandler(void)
{
  // if (TMR_ReadIntFlag(TMR3, TMR_INT_FLAG_CH1) == SET) {
  //   TMR_ClearIntFlag(TMR3, TMR_INT_FLAG_CH1);
  //   printf("TMR3 = %u \r\n", TMR_ReadCaputer1(TMR3));
  // }
  if (TMR_ReadIntFlag(TMR3, TMR_INT_FLAG_UPDATE) == SET) {
    TMR_ClearIntFlag(TMR3, TMR_INT_FLAG_UPDATE);
    TIMER3_IF_Counter++;
    if (shift_ch_CMD == TRUE) {
      switch (TIMER3_IF_Counter) {
      case  15: {
        SET_DAC_INM(1200); Var_Sub = 28;
      }break;
      case  16: {
        SET_DAC_INM(1000); Var_Sub = 27;
      }break;
      case  17: {
        SET_DAC_INM(800); Var_Sub = 26;
      }break;
      case  18: {
        SET_DAC_INM(600); Var_Sub = 26;
      }break;
      case  19: {
        SET_DAC_INM(500); Var_Sub = 25;
      }break;
      case  21: {
        SET_DAC_INM(400); Var_Sub = 25;
      }break;
      case  22: {
        SET_DAC_INM(300); Var_Sub = 24;
      }break;
      case  23: {
        SET_DAC_INM(200); Var_Sub = 24;
      }break;
      case  24: {
        SET_DAC_INM(150); Var_Sub = 23;
      }break;
      case  28: {
        SET_DAC_INM(100); Var_Sub = 23;
      }break;
      case  31: {
        SET_DAC_INM(80); Var_Sub = 23;
      }break;
      case  36: {
        SET_DAC_INM(60); Var_Sub = 23;
      }break;
      case  40: {
        SET_DAC_INM(40); Var_Sub = 22;
      }break;
      case  80: {
        SET_DAC_INM(20); Var_Sub = 21;
      }break;

      case 100: {
        SET_DAC_INM(500); Var_Sub = 21;   // 152 mV
        // COMP_Disable(COMP_SELECT_COMP2);   // 模拟比较器2
        // COMP_Enable(COMP_SELECT_COMP1);   // 开启模拟比较器1
      }break;
      case 102: {
        SET_DAC_INM(450); Var_Sub = 21;
      }break;
      case 104: {
        SET_DAC_INM(400); Var_Sub = 21;
      }break;
      case 106: {
        SET_DAC_INM(350); Var_Sub = 21;
      }break;
      case 108: {
        SET_DAC_INM(300); Var_Sub = 20;
      }break;
      case 110: {
        SET_DAC_INM(250); Var_Sub = 20;
      }break;
      case 115: {
        SET_DAC_INM(200); Var_Sub = 20;
      }break;
      case 120: {
        SET_DAC_INM(150); Var_Sub = 20;
      }break;
      case 125: {
        SET_DAC_INM(100); Var_Sub = 19;   // 30.5 mV
      }break;
      case 180: {
        SET_DAC_INM(100); Var_Sub = 18;
      }break;
      case 200: {
        SET_DAC_INM(100); Var_Sub = 17;
      }break;
      case 240: {
        SET_DAC_INM(80); Var_Sub = 15;
      }break;
      case 300: {
        SET_DAC_INM(80); Var_Sub = 15;
      }break;
      case 320: {
        SET_DAC_INM(80); Var_Sub = 14;
      }break;
      case 380: {
        SET_DAC_INM(80); Var_Sub = 13;
      }break;
      case 430: {
        SET_DAC_INM(80); Var_Sub = 12;
      }break;
      case 510: {
        SET_DAC_INM(60); Var_Sub = 10;
      }break;
      case 570: {
        SET_DAC_INM(60); Var_Sub = 9;
      }break;
      case 620: {
        SET_DAC_INM(50); Var_Sub = 8;
      }break;
      case 700: {
        SET_DAC_INM(45); Var_Sub = 7;
      }break;
      default:break;
      }
    }
  }
}

/**@} end of group USART_Interrupt_INT_Functions */
/**@} end of group USART_Interrupt */
/**@} end of group Examples */
