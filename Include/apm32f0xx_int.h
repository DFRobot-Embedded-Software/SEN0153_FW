/*!
 * @file        apm32f0xx_int.h
 *
 * @brief       This file contains the headers of the interrupt handlers
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

/* Define to prevent recursive inclusion */
#ifndef __APM32F0XX_INT_H
#define __APM32F0XX_INT_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes */
#include "main.h"

/** @addtogroup Examples
  @{
  */

/** @addtogroup USART_Interrupt
  @{
  */

/** @defgroup USART_Interrupt_Functions INT_Functions
  @{
  */

void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /*__APM32F0XX_INT_H */

/**@} end of group USART_Interrupt_INT_Functions */
/**@} end of group USART_Interrupt */
/**@} end of group Examples */
