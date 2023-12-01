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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "all_include.h"

/* printf using USART1  */
#define DEBUG_USART           USART1

#define IAP_UART              (0)

#define APPLICATION_ADDRESS   ((uint32_t)0x08002000)

#define APP_CALI_ADDRESS      ((uint32_t)0x08001C00)
#define APP_INFO_ADDRESS      ((uint32_t)0x08001C10)

typedef struct {
  uint8_t pid_h;            //模块PID
  uint8_t pid_l;            //模块PID
  uint8_t vid_h;            //模块VID
  uint8_t vid_l;            //模块VID
  uint8_t version1_h;      //版本信息高位(V1)
  uint8_t version1_l;      //版本信息高位(V1)
  uint8_t version2_h;      //版本信息高位(V1)
  uint8_t version2_l;      //版本信息低位(.1.1)
  uint8_t sku1_h;          //SKU高位 0000:sen,0100:dfr
  uint8_t sku1_l;          //SKU低位
  uint8_t sku2_h;          //SKU高位 0000:sen,0100:dfr
  uint8_t sku2_l;          //SKU低位
} sFirmwareInfo_t;

typedef struct {
  uint8_t buf[4];
  uint8_t reserve[12];
  sFirmwareInfo_t info;
} sAllFlashInfo_t;


void appFlagInit(void)   // 用于固件更新的信息填写
{
  sAllFlashInfo_t flashData = {
    .buf = { 0x03,0x02,0x01,0x00 },
    .reserve = {0},
    .info.pid_h = DEVICE_PID >> 8,
    .info.pid_l = DEVICE_PID & 0xFF,
    .info.vid_h = DEVICE_VID >> 8,
    .info.vid_l = DEVICE_VID & 0xFF,
    .info.version1_h = (DEVICE_SOFT_VERSION & 0xF000) >> 12,
    .info.version1_l = (DEVICE_SOFT_VERSION & 0xF00) >> 8,
    .info.version2_h = (DEVICE_SOFT_VERSION & 0xF0) >> 4,
    .info.version2_l = (DEVICE_SOFT_VERSION & 0xF),
    .info.sku1_h = 0x00,   // SEN0153
    .info.sku1_l = 0x00,
    .info.sku2_h = 0x00,
    .info.sku2_l = 0x99,
  };

  // FLASH_Unlock(FLASH_MEMTYPE_PROG); 
  // bsp_flash_write(APP_CALI_ADDRESS, sizeof(sAllFlashInfo_t), (uint8_t *)&flashData);
  // FLASH_Lock(FLASH_MEMTYPE_PROG); 
  uint32_t* p = (uint32_t*)&flashData;
  FMC_Unlock();
  FMC_ErasePage(APP_CALI_ADDRESS);
  for (uint32_t i = 0; i < sizeof(sAllFlashInfo_t) / 4; i++) {
    FMC_ProgramWord(APP_CALI_ADDRESS + i * 4, p[i]);
  }
  FMC_Lock();
}

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

#if IAP_UART   // 中断向量
  for (uint8_t i = 0; i < 48; i++) {
    *(uint32_t*)(0x20000000 + (i << 2)) = *(__IO uint32_t*)(APPLICATION_ADDRESS + (i << 2));
  }
  /* Enable the SYSCFG peripheral clock*/
  // RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  /* Remap SRAM at 0x00000000 */
  // SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
  SYSCFG_MemoryRemapSelect(SYSCFG_MEMORY_REMAP_SRAM);
#endif

  GPIO_setup(); //--------------------------------------------------------初始化GPIO
  GPIO_SetBit(LED_PORT, LED_PIN);//------------------------------------LED指示灯输出高电平表示开始初始化芯片

  Restore_Check();//------------------------------------------------------检测TX与RX端口上的电平，判断是否需要恢复出厂设置

  USART1_Init();//------------------------------------------------------初始化串口

  Parameters_Setup();//---------------------------------------------------检查参数，程序第一次运行时初始化出厂默认参数

  appFlagInit();

  APM_EVAL_TMR14_Init(500, 48);   // 0.5 ms
  TIMER2_setup(4800, 48);   // 4.8ms
  TIMER3_setup(235, 12);   // 58.75us
  DAC_setup();   // 初始化DAC输出基准电压
  ACMP_setup();   // 初始化模拟比较器

  // NVIC_SetPriorityGrouping(5);//------------------------------------------中断分组第5组
  // NVIC_SetPriority(TIMER2_IRQn,		   NVIC_EncodePriority(5,0,0));//-------定时器0中断，用于测量计时，最高优先级	
  // NVIC_SetPriority(TIMER3_IRQn, 	   NVIC_EncodePriority(5,0,1));//-------定时器1，抢占优先级0，响应优先级1
  // NVIC_SetPriority(LEUART0_IRQn,     NVIC_EncodePriority(5,1,0));//-------串口0，  抢占优先级1，响应优先级0
  // NVIC_SetPriority(ACMP0_IRQn, 		   NVIC_EncodePriority(5,1,1));//-------比较器， 抢占优先级1，响应优先级1
  // NVIC_SetPriority(GPIO_ODD_IRQn,    NVIC_EncodePriority(5,2,0));//-------IO口，   抢占优先级2，响应优先级0

  GPIO_ClearBit(LED_PORT, LED_PIN);//----------------------------------LED指示初始化完成

  clear_RxMessage();//--------------------------------------------------清空协议Buffer

  // APM_EVAL_DelayMs(1000);
  printf("SEN0153 URM07!\r\n");

  uint16_t dis = 0;
  while (1) {
    dis = get_Dis();
    printf("get_Dis() = %u\r\n\r\n", dis);
    APM_EVAL_DelayMs(1000);
    if (revBuf.revEmptyFlag == FALSE) {   // 否定   接收 buf 不为空
      TMR_Disable(TMR14);
      DF_Protocol_Parse();
      clear_RxMessage();//--------------------------------------------------清空协议Buffer
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
