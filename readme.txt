/*!
 * @file        readme.txt
 *
 * @brief       This file is routine instruction
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


&par Example Description

This example shows how to use USART1 Peripheral to send data and receive data from 
serial assistant by interrupt.
Press KEY1 to write data to serial assistant. And you can send data to device use 
serial assistant.

&par Hardware Description
The USART1 is configured as follows:
    - TX:PA9, RX:PA10
    - BaudRate = 115200 baud
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled
    
    - Close J9 and J10

&par Directory contents

  - USART/USART_Interrupt/Source/apm32f0xx_int.c     Interrupt handlers
  - USART/USART_Interrupt/Source/main.c              Main program

&par IDE environment

  - MDK-ARM V5.36
  - EWARM V8.50.4.26131

&par Hardware and Software environment

  - This example runs on APM32F051 EVAL Devices.
