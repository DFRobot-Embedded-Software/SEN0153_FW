#include "setup.h"

GPIO_Config_T gpioConfig;

/*******************************************************************************
* Description    : 初始化TIMER3
* 声速约: 34000 cm/s
* 1cm 大约需要: 29.4 us
* 声波往返: 58.8 us

  // 48000000 / 48 = 1000000 Hz   =   1 us
  // 1 us * 4800 = 4800 us
  // TIMER2_setup(4800, 48);   // 4.8ms
*******************************************************************************/
void TIMER1_setup(uint16_t period, uint16_t div)
{
  /* Enable Clock*/
  RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR1);

  TMR_TimeBase_T  timeBaseConfig;
  /* Set clockDivision = 1 */
  timeBaseConfig.clockDivision = TMR_CKD_DIV1;   // 为什么测试没发现区别
  /* Up-counter */
  timeBaseConfig.counterMode = TMR_COUNTER_MODE_UP;
  /* Set divider */
  timeBaseConfig.div = div - 1;
  /* Set counter = 0xffff */
  timeBaseConfig.period = period - 1;
  /* Repetition counter = 0x0 */
  timeBaseConfig.repetitionCounter = 0;
  TMR_ConfigTimeBase(TMR1, &timeBaseConfig);

  TMR_ICConfig_T icConfig;
  icConfig.channel = TMR_CHANNEL_1;
  icConfig.ICpolarity = TMR_IC_POLARITY_RISING;   // 上升沿触发
  icConfig.ICselection = TMR_IC_SELECTION_DIRECT_TI;
  icConfig.ICprescaler = TMR_ICPSC_DIV1;   // 不分频
  icConfig.ICfilter = 0;   // 不使用输入捕获滤波
  TMR_ICConfig(TMR1, &icConfig);

  TMR_EnableCCxChannel(TMR1, TMR_CHANNEL_1);
  // TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_1);

  TMR_ClearIntFlag(TMR1, TMR_INT_FLAG_UPDATE | TMR_INT_FLAG_CH1);   // 在这里清除中断标志，才会使中断不会误触发一次
  TMR_EnableInterrupt(TMR1, TMR_INT_UPDATE | TMR_INT_CH1);
  // TMR_DisableInterrupt(TMR1, TMR_INT_UPDATE | TMR_INT_CH1);

  NVIC_EnableIRQRequest(TMR1_CC_IRQn, 2);
  NVIC_EnableIRQRequest(TMR1_BRK_UP_TRG_COM_IRQn, 2);
  // NVIC_DisableIRQRequest(TMR1_IRQn);

  // TMR_Enable(TMR1);
  TMR_Disable(TMR1);

}

/*******************************************************************************
* Description    : 初始化TIMER3
* 声速约: 34000 cm/s
* 1cm 大约需要: 29.4 us
* 声波往返: 58.8 us

  // 48000000 / 48 = 1000000 Hz   =   1 us
  // 1 us * 4800 = 4800 us
  // TIMER2_setup(4800, 48);   // 4.8ms
*******************************************************************************/
void TIMER2_setup(uint16_t period, uint16_t div)
{
  /* Enable Clock*/
  RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR2);

  TMR_TimeBase_T  timeBaseConfig;
  /* Set clockDivision = 1 */
  timeBaseConfig.clockDivision = TMR_CKD_DIV1;   // 为什么测试没发现区别
  /* Up-counter */
  timeBaseConfig.counterMode = TMR_COUNTER_MODE_UP;
  /* Set divider */
  timeBaseConfig.div = div - 1;
  /* Set counter = 0xffff */
  timeBaseConfig.period = period - 1;
  /* Repetition counter = 0x0 */
  timeBaseConfig.repetitionCounter = 0;
  TMR_ConfigTimeBase(TMR2, &timeBaseConfig);

  TMR_ICConfig_T icConfig;
  icConfig.channel = TMR_CHANNEL_4;
  icConfig.ICpolarity = TMR_IC_POLARITY_RISING;   // 上升沿触发
  icConfig.ICselection = TMR_IC_SELECTION_DIRECT_TI;
  icConfig.ICprescaler = TMR_ICPSC_DIV1;   // 不分频
  icConfig.ICfilter = 0;   // 不使用输入捕获滤波
  TMR_ICConfig(TMR2, &icConfig);

  TMR_EnableCCxChannel(TMR2, TMR_CHANNEL_4);
  // TMR_DisableCCxChannel(TMR2, TMR_CHANNEL_4);

  TMR_ClearIntFlag(TMR2, TMR_INT_FLAG_UPDATE | TMR_INT_FLAG_CH4);
  TMR_EnableInterrupt(TMR2, TMR_INT_UPDATE | TMR_INT_CH4);   // Enable update interrupt
  // TMR_DisableInterrupt(TMR2, TMR_INT_UPDATE | TMR_INT_CH4);

  NVIC_EnableIRQRequest(TMR2_IRQn, 2);   // 测距计时最优先
  // NVIC_DisableIRQRequest(TMR2_IRQn);

  // TMR_Enable(TMR2);   // Enable TMR2
  TMR_Disable(TMR2);
}

/*******************************************************************************
* Description    : 初始化TIMER3
* 声速约: 34000 cm/s
* 1cm 大约需要: 29.4 us
* 声波往返: 58.8 us

  // 48000000 / 12 = 4000000 Hz   =   0.25 us
  // 0.25 us * 235 = 58.75 us
  // TIMER3_setup(235, 12);   // 约 58.8us
*******************************************************************************/
void TIMER3_setup(uint16_t period, uint16_t div)
{
  /* Enable Clock*/
  RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR3);

  TMR_TimeBase_T  timeBaseConfig;
  /* Set clockDivision = 1 */
  timeBaseConfig.clockDivision = TMR_CKD_DIV1;
  /* Up-counter */
  timeBaseConfig.counterMode = TMR_COUNTER_MODE_UP;
  /* Set divider */
  timeBaseConfig.div = div - 1;
  /* Set counter = 0xffff */
  timeBaseConfig.period = period - 1;
  /* Repetition counter = 0x0 */
  timeBaseConfig.repetitionCounter = 0;
  TMR_ConfigTimeBase(TMR3, &timeBaseConfig);

  // TMR_ICConfig_T icConfig;
  // icConfig.channel = TMR_CHANNEL_1;
  // icConfig.ICpolarity = TMR_IC_POLARITY_RISING;  // 上升沿触发
  // icConfig.ICselection = TMR_IC_SELECTION_DIRECT_TI;
  // icConfig.ICprescaler = TMR_ICPSC_DIV1;  // 不分频
  // icConfig.ICfilter = 0;  // 不使用输入捕获滤波
  // TMR_ICConfig(TMR3, &icConfig);

  // TMR_EnableCCxChannel(TMR3, TMR_CHANNEL_1);
  // TMR_DisableCCxChannel(TMR3, TMR_CHANNEL_1);

  TMR_ClearIntFlag(TMR3, TMR_INT_FLAG_UPDATE);
  TMR_EnableInterrupt(TMR3, TMR_INT_UPDATE);
  // TMR_DisableInterrupt(TMR3, TMR_INT_UPDATE);

  NVIC_EnableIRQRequest(TMR3_IRQn, 2);
  // NVIC_DisableIRQRequest(TMR3_IRQn);

  // TMR_Enable(TMR3);
  TMR_Disable(TMR3);
}

/*!
 * @brief       TMR14 init
 *
 * @param       None
 *
 * @retval      None
 *
 */
void APM_EVAL_TMR14_Init(uint16_t period, uint16_t div)
{
  /* Enable Clock*/
  RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR14);

  TMR_TimeBase_T  timeBaseConfig;
  /* Set clockDivision = 1 */
  timeBaseConfig.clockDivision = TMR_CKD_DIV1;
  /* Up-counter */
  timeBaseConfig.counterMode = TMR_COUNTER_MODE_UP;
  /* Set divider */
  timeBaseConfig.div = div - 1;
  /* Set counter = 0xffff */
  timeBaseConfig.period = period - 1;
  /* Repetition counter = 0x0 */
  timeBaseConfig.repetitionCounter = 0;
  TMR_ConfigTimeBase(TMR14, &timeBaseConfig);

  /* Enable update interrupt*/
  TMR_ClearIntFlag(TMR14, TMR_INT_FLAG_UPDATE);
  TMR_EnableInterrupt(TMR14, TMR_INT_UPDATE);
  NVIC_EnableIRQRequest(TMR14_IRQn, 2);

  /*  Enable TMR14  */
  TMR_Enable(TMR14);
}

/*******************************************************************************
* Function Name  : GPIO_setup
* Description    : 初始化GPIO
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_setup(void)
{
  /* Enable GPIO clock */
  RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_GPIOA);
  RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_GPIOB);

  //-----运放增益端口配置
  // PA8---GAIN0   PA9---GAIN1   PA10---GAIN2   PA15---GAIN3   PB4---GAIN4
  gpioConfig.mode = GPIO_MODE_OUT;
  gpioConfig.outtype = GPIO_OUT_TYPE_OD;
  gpioConfig.speed = GPIO_SPEED_50MHz;
  gpioConfig.pupd = GPIO_PUPD_NO;
  gpioConfig.pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_15;
  GPIO_Config(GPIOA, &gpioConfig);
  GPIO_ClearBit(GPIOA, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_15);
  // GPIO_SetBit(GPIOA, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_15);
  // GPIO_ClearBit(GPIOA, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);
  gpioConfig.pin = GPIO_PIN_4;
  GPIO_Config(GPIOB, &gpioConfig);
  // GPIO_ClearBit(GPIOB, GPIO_PIN_4);
  GPIO_SetBit(GPIOB, GPIO_PIN_4);

  //-----LED端口配置
  gpioConfig.outtype = GPIO_OUT_TYPE_PP;
  gpioConfig.pin = LED_PIN;
  GPIO_Config(LED_PORT, &gpioConfig);
  GPIO_ClearBit(LED_PORT, LED_PIN);

  //-----温度测量端口配置
  gpioConfig.pin = TEMP_VCC_PIN;
  GPIO_Config(TEMP_VCC_PORT, &gpioConfig);
  GPIO_ClearBit(TEMP_VCC_PORT, TEMP_VCC_PIN);

  //-----运放供电端口配置
  gpioConfig.pin = OPA_VCC_PIN;
  GPIO_Config(OPA_VCC_PORT, &gpioConfig);
  GPIO_SetBit(OPA_VCC_PORT, OPA_VCC_PIN);   // 运放断电

  //-----超声波探头驱动端口配置
  gpioConfig.pin = X_PIN;
  GPIO_Config(X_PORT, &gpioConfig);
  GPIO_ClearBit(X_PORT, X_PIN);
  gpioConfig.pin = Y_PIN;
  GPIO_Config(Y_PORT, &gpioConfig);
  GPIO_ClearBit(Y_PORT, Y_PIN);
}

/*******************************************************************************
* Function Name  : USART1_Init
* Description    : 初始化USART1
*******************************************************************************/
void USART1_Init(void)
{
  /* Connect PXx to USARTx_Tx */
  GPIO_ConfigPinAF(USART1_TX_PORT, GPIO_PIN_SOURCE_6, GPIO_AF_PIN0);
  /* Connect PXx to USARRX_Rx */
  GPIO_ConfigPinAF(USART1_RX_PORT, GPIO_PIN_SOURCE_7, GPIO_AF_PIN0);

  /* Configure USART Tx as alternate function push-pull */
  gpioConfig.mode = GPIO_MODE_AF;
  gpioConfig.pin = USART1_TX_PIN;
  gpioConfig.speed = GPIO_SPEED_50MHz;
  gpioConfig.outtype = GPIO_OUT_TYPE_PP;
  gpioConfig.pupd = GPIO_PUPD_PU;
  GPIO_Config(USART1_TX_PORT, &gpioConfig);
  /* Configure USART Rx as input floating */
  gpioConfig.pin = USART1_RX_PIN;
  GPIO_Config(USART1_RX_PORT, &gpioConfig);

  /* Enable USART1 clock */
  RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_USART1);
  // RCM_ConfigUSARTCLK(RCM_USART1CLK_HSI);
  USART_Config_T usartConfigStruct;
  /* USARTs configured as follow: */
  usartConfigStruct.baudRate = Baudrate;
  /* Receive and transmit enabled  */
  usartConfigStruct.mode = USART_MODE_TX_RX;
  /* Hardware flow control disabled (RTS and CTS signals) */
  usartConfigStruct.hardwareFlowCtrl = USART_FLOW_CTRL_NONE;
  /* No parity */
  usartConfigStruct.parity = USART_PARITY_NONE;
  /* One Stop Bit */
  usartConfigStruct.stopBits = USART_STOP_BIT_1;
  /* Word Length = 8 Bits  */
  usartConfigStruct.wordLength = USART_WORD_LEN_8B;
  /* USART_Config */
  USART_Config(USART1, &usartConfigStruct);

  /* Enable USART_Interrupt_RXBNEIE */
  USART_RxData(USART1);
  USART_EnableInterrupt(USART1, USART_INT_RXBNEIE);

  NVIC_EnableIRQRequest(USART1_IRQn, 2);

  // USART_ConfigStopModeWakeUpSource(USART1, USART_WAKEUP_SOURCE_START);
  /* Enable USART */
  USART_Enable(USART1);
  // USART_EnableStopMode(USART1);
}

/***************************************************************************//**
* @brief
*   Configure ADC usage for this application.
*******************************************************************************/
void ADC_setup(void)
{
  /* GPIO Configuration */
  gpioConfig.pin = ADC1_IN6_PIN;
  gpioConfig.mode = GPIO_MODE_AN;
  gpioConfig.pupd = GPIO_PUPD_PU;
  GPIO_Config(ADC1_IN6_PORT, &gpioConfig);

  /* RCM Enable*/
  RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_ADC1);

  ADC_Config_T  adcConfig;
  ADC_ConfigStructInit(&adcConfig);
  /* Set resolution*/
  adcConfig.resolution = ADC_RESOLUTION_12B;
  /* Set dataAlign*/
  adcConfig.dataAlign = ADC_DATA_ALIGN_RIGHT;
  /* Set scanDir*/
  adcConfig.scanDir = ADC_SCAN_DIR_UPWARD;
  /* Set convMode continous*/
  adcConfig.convMode = ADC_CONVERSION_CONTINUOUS;
  /* Set extTrigConv*/
  adcConfig.extTrigConv = ADC_EXT_TRIG_CONV_TRG0;
  /* Set TrigEdge*/
  adcConfig.extTrigEdge = ADC_EXT_TRIG_EDGE_NONE;
  ADC_Config(&adcConfig);

  ADC_ConfigChannel(ADC_CHANNEL_6, ADC_SAMPLE_TIME_1_5);

  /* Enable Interrupt*/
  // ADC_EnableInterrupt(ADC_INT_CS);
  // NVIC_EnableIRQRequest(ADC1_COMP_IRQn, 2);

  /* Calibration*/
  ADC_ReadCalibrationFactor();

  /* Enable ADC*/
  ADC_Enable();
  APM_EVAL_DelayMs(3);   // 必要的延时
}

/*******************************************************************************
* Function Name  : DAC_setup
* Description    : 初始化DAC
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DAC_setup(void)
{
  /* DAC1 out PA4 pin configuration */
  GPIO_ConfigStructInit(&gpioConfig);
  gpioConfig.mode = GPIO_MODE_AN;
  gpioConfig.outtype = GPIO_OUT_TYPE_PP;
  gpioConfig.speed = GPIO_SPEED_10MHz;
  gpioConfig.pupd = GPIO_PUPD_NO;
  gpioConfig.pin = DAC1_OUT_PIN;
  GPIO_Config(DAC1_OUT_PORT, &gpioConfig);

  // 1. 初始化 DAC 模块
  RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_DAC);

  // 2. 配置 DAC 通道 1 的参数
  DAC_Config_T dacConfig;
  DAC_ConfigStructInit(&dacConfig);
  dacConfig.trigger = DAC_TRIGGER_NONE;                   /*!< DAC trigger selection */
  dacConfig.waveGeneration = DAC_WAVE_GENERATION_NONE;    /*!< DAC output buffer disable */
  dacConfig.maskAmplitudeSelect = DAC_LFSRUNAMASK_BIT0;   /*!< DAC noise/triangle wave generation selection */
  dacConfig.outputBuff = DAC_OUTPUTBUFF_ENABLE;           /*!< DAC mask/amplitude selector */
  DAC_Config(DAC_CHANNEL_1, &dacConfig);

  // 3. 使能 DAC 通道 1
  DAC_Enable(DAC_CHANNEL_1);

  // 4. （可选）配置 DAC 数据对齐方式和初始值
  // DAC_ConfigChannel1Data(DAC_ALIGN_12B_R, 0x800);  // 示例中的 0x800 是 12 位数据的一半
  SET_DAC_INM_0(1200);
}

/*******************************************************************************
* Function Name  : ACMP_setup
* Description    : 初始化ACMP
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ACMP_setup(void)
{
  //-----COMP端口配置
  /* Connect PA11 to COMP1_OUT */
  GPIO_ConfigPinAF(ACMP1_OUT_PORT, GPIO_PIN_SOURCE_11, GPIO_AF_PIN7);
  /* Connect PA12 to COMP2_OUT */
  GPIO_ConfigPinAF(ACMP2_OUT_PORT, GPIO_PIN_SOURCE_12, GPIO_AF_PIN7);

  GPIO_ConfigStructInit(&gpioConfig);
  gpioConfig.pupd = GPIO_PUPD_PU;
  gpioConfig.speed = GPIO_SPEED_50MHz;
  gpioConfig.outtype = GPIO_OUT_TYPE_PP;
  gpioConfig.mode = GPIO_MODE_AF;
  gpioConfig.pin = ACMP1_OUT_PIN;
  GPIO_Config(ACMP1_OUT_PORT, &gpioConfig);
  gpioConfig.pin = ACMP2_OUT_PIN;
  GPIO_Config(ACMP2_OUT_PORT, &gpioConfig);

  /* Configure PA0 is connected to COMP1 inverting input */
  /* Configure PA1 is connected to COMP1 non inverting input */
  /* Configure PA2 is connected to COMP2 inverting input */
  /* Configure PA3 is connected to COMP2 non inverting input */
  gpioConfig.mode = GPIO_MODE_AN;
  gpioConfig.pupd = GPIO_PUPD_NO;
  gpioConfig.pin = ACMP1_INM_PIN;
  GPIO_Config(ACMP1_INM_PORT, &gpioConfig);
  gpioConfig.pin = ACMP1_INP_PIN;
  GPIO_Config(ACMP1_INP_PORT, &gpioConfig);
  gpioConfig.pin = ACMP2_INM_PIN;
  GPIO_Config(ACMP2_INM_PORT, &gpioConfig);
  gpioConfig.pin = ACMP2_INP_PIN;
  GPIO_Config(ACMP2_INP_PORT, &gpioConfig);

  // 配置 COMP
  COMP_Config_T compConfig;
  COMP_ConfigStructInit(&compConfig);  // 使用默认值初始化配置结构体
  compConfig.invertingInput = COMP_INVERTING_INPUT_IO;   /*!< 比较器反相输入选择 */
  compConfig.output = COMP_OUTPUT_SET;                  /*!< 比较器输出选择 */
  compConfig.outputPol = COMP_OUTPUTPOL_NONINVERTED;        /*!< 比较器输出极性 */
  compConfig.hysterrsis = COMP_HYSTERRSIS_NO;                /*!< 比较器迟滞 */
  compConfig.mode = COMP_MODE_HIGHSPEED;               /*!< Comparator mode */
  COMP_Config(COMP_SELECT_COMP1, &compConfig);
  compConfig.invertingInput = COMP_INVERTING_INPUT_IO;   /*!< 比较器反相输入选择 */
  compConfig.output = COMP_OUTPUT_SET;                  /*!< 比较器输出选择 */
  COMP_Config(COMP_SELECT_COMP2, &compConfig);

  // EINT_Config_T eintConfig;
  // eintConfig.line = EINT_LINE21;
  // eintConfig.mode = EINT_MODE_INTERRUPT;
  // eintConfig.trigger = EINT_TRIGGER_ALL;
  // eintConfig.lineCmd = ENABLE;
  // EINT_Config(&eintConfig);
  // eintConfig.line = EINT_LINE22;
  // EINT_Config(&eintConfig);
  // EINT_ClearIntFlag(EINT_LINE21);
  // EINT_ClearIntFlag(EINT_LINE22);
  // NVIC_EnableIRQRequest(ADC1_COMP_IRQn, 2);

  // 启用 COMP
  // COMP_Enable(COMP_SELECT_COMP1);
  COMP_Disable(COMP_SELECT_COMP1);
  // COMP_Enable(COMP_SELECT_COMP2);
  COMP_Disable(COMP_SELECT_COMP2);
}
