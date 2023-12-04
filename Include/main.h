/*!
 * @file        main.h
 *
 * @brief       Header for main.c module
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
#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

  /* Includes */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "apm32f0xx.h"
#include "apm32f0xx_rcm.h"
#include "apm32f0xx_misc.h"
#include "apm32f0xx_tmr.h"
#include "apm32f0xx_gpio.h"
#include "apm32f0xx_usart.h"
#include "apm32f0xx_fmc.h"
#include "apm32f0xx_adc.h"
#include "apm32f0xx_dac.h"
#include "apm32f0xx_comp.h"
#include "apm32f0xx_syscfg.h"
#include "apm32f0xx_eint.h"

#include "bsp_delay.h"


/* printf using USART1  */
#define DEBUG_USART           USART1

#define APP_CALI_ADDRESS      ((uint32_t)0x08001C00)
#define APP_INFO_ADDRESS      ((uint32_t)0x08001C10)

#define DEVICE_VID   0x3343
#define DEVICE_PID   0x0005   // URM07

#define DEVICE_HARD_VERSION   0x1000
#define DEVICE_SOFT_VERSION   0x1000
  
#define IAP_UART 0
#if IAP_UART
#define APPLICATION_ADDRESS   ((uint32_t)0x2000)
#else
#define APPLICATION_ADDRESS   ((uint32_t)0x00)
#endif


#ifdef __cplusplus
}
#endif

#endif   /* __MAIN_H__ */
