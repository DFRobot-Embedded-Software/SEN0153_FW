#include "func.h"


volatile uint16_t TIMERx_IF_Counter = 0;
volatile uint16_t TIMER3_IF_Counter = 0;

volatile uint16_t Var_Sub = 0;   // 距离测量数据辅助参数
uint8_t shift_ch_CMD = FALSE;   // 似乎标志 数据测量过程
volatile uint16_t Distance = 0;
volatile uint8_t DisTrue_Flag = FALSE;   // 距离获取正确的标志

uint8_t  device_Addr = device_Addr_DEFAULT;
uint32_t Baudrate = Baudrate_DEFAULT;

volatile uint32_t Vspeed = 0;   // 单位为cm/s

const uint16_t NCP18WF104[34] = {   // 根据数据手册 电阻和温度的对应值 计算出的 模拟量和温度的对应值
11,
16,
22,
31,
42,
57,
77,
102,
133,
173,
222,
281,
353,
439,
539,
654,
784,
929,
1088,
1257,
1436,
1621,
1808,
1995,
2178,
2355,
2523,
2680,
2827,
2962,
3085,
3196,
3295,
3385
};


// 发送超声波信号
void Send_40K(uint8_t Num)
{
  // GPIO_SetOutPP(X_PORT, X_PIN);
  uint8_t i = 0, j = 70;
  for (uint8_t n = 0;n < Num;n++) {
    GPIO_ClearBit(X_PORT, X_PIN);   // A- B+
    GPIO_SetBit(Y_PORT, Y_PIN);
    i = j;while (i--);
    GPIO_SetBit(X_PORT, X_PIN);   // A+ B-
    GPIO_ClearBit(Y_PORT, Y_PIN);
    i = j;while (i--);
  }

  GPIO_ClearBit(X_PORT, X_PIN);
  GPIO_ClearBit(Y_PORT, Y_PIN);

  // for (i = 0;i < 100;i++);
  // GPIO_SetInputFloat(X_PORT, X_PIN);
}

void SetVs(void)
{
  uint16_t i = 0;
  volatile uint16_t ADC_Value;
  GPIO_SetBit(TEMP_VCC_PORT, TEMP_VCC_PIN);   // 热敏电阻通电
  ADC_setup();
  ADC_StartConversion();   // 启动转换
  while ((ADC->STS & ADC_FLAG_CS) == RESET);//等待数据有效
  ADC_ReadConversionValue();//获取ADC采样值, 舍弃第一次的测量数据

#if 1
  volatile uint32_t ADC_Buffer = 0;
  for (i = 0;i < 10;i++) {
    while ((ADC->STS & ADC_FLAG_CS) == RESET);//等待数据有效
    ADC_Buffer += (uint16_t)ADC_ReadConversionValue();//获取ADC采样值
  }
  ADC_Value = ADC_Buffer / 10;
#else
  uint16_t dataBuf[11] = { 0 };
  for (i = 0; i < 11; i++) {
    while ((ADC->STS & ADC_FLAG_CS) == RESET);//等待数据有效
    dataBuf[i] = ADC_ReadConversionValue();//获取ADC采样值
  }
  for (uint16_t j = 0; j < 10; j++) {   // 从小到大排序
    for (uint16_t k = 0; k < 10 - j; k++) {
      if (dataBuf[k] > dataBuf[k + 1]) {
        uint16_t temp = dataBuf[k];
        dataBuf[k] = dataBuf[k + 1];
        dataBuf[k + 1] = temp;
      }
    }
  }
  ADC_Value = dataBuf[5];
#endif
  ADC_StopConversion();   // 停止转换

  ADC_Reset();

  GPIO_ClearBit(TEMP_VCC_PORT, TEMP_VCC_PIN);   // 热敏电阻断电

  /*
    // x 是 热敏电阻阻值 kΩ
    x / 56 = (3300 - a) / a
    x = 3300 * 56 / a - 56

    // 温度 和 热敏电阻阻值 多段近似性关系
    ((buf[i] - x) / ((buf[i] - buf[i + 1]) / 5) + i * 5 - 40)
    ((buf[i] - x) / (buf[i] - buf[i + 1]) * 5 + i * 5 - 40)

    // a 是模拟读出来的 分压电阻的 电压 mV
    a / 3300 = 56 / (x + 56)
    a = 3300 * 56 / (x + 56)
    a = ADC_Value / 4096 * 3300
    ADC_Value = 56 * 4096 / (x + 56)

    // 温度 和 ADC测量值 多段近似性关系
    Temp = ((ADC_Value - NCP18WF104[i]) / ((NCP18WF104[i + 1] - NCP18WF104[i]) / 5) + i * 5 - 40)
    Temp = ((ADC_Value - NCP18WF104[i]) / (NCP18WF104[i + 1] - NCP18WF104[i]) * 5 + i * 5 - 40)
    Temp = ((ADC_Value - NCP18WF104[i]) * 5 / (NCP18WF104[i + 1] - NCP18WF104[i]) + i * 5 - 40)
  */
  for (i = 0;i < 33;i++) {
    if ((ADC_Value >= NCP18WF104[i]) && (ADC_Value < NCP18WF104[i + 1]))break;
  }
  Vspeed = 33145 + 61 * ((ADC_Value - NCP18WF104[i]) * 5 / (NCP18WF104[i + 1] - NCP18WF104[i]) + i * 5 - 40);//----cm/s
}

uint16_t get_Dis(void)
{
  volatile uint16_t dis = 0;

  SetVs();   // 测量温度，计算声波速度

  DAC_setup();
  // SET_DAC_INM(1200);
  // APM_EVAL_DelayMs(100);
  GPIO_ClearBit(OPA_VCC_PORT, OPA_VCC_PIN);   // 运放供电
  ACMP_setup();   // 初始化模拟比较器

  TIMER3_setup(235, 12);   // 58.75us
  TIMERx_setup(4800, 48);   // 4.8 ms

  GPIO_Config_T gpioConfig;
  GPIO_SetOutPP(X_PORT, X_PIN);   // 设置为输出, 发送声波
  __disable_irq();
  Send_40K(8);   // 40Khz 发送8个脉冲
  __enable_irq();

  TMR_Enable(TMR3);
  TMR_Enable(USE_TMRx);

  while (TIMER3_IF_Counter < 14);
  COMP_Enable(COMP_SELECT_COMP2);   // 开启模拟比较器2
  GPIO_SetInputFloat(X_PORT, X_PIN);   // 设置为输入, 接收回波

  shift_ch_CMD = TRUE;
  while ((TIMER3_IF_Counter < 900) && (DisTrue_Flag == FALSE));   // 大于8米多未检测到回波（这里是定时器2计数存在误差）
  shift_ch_CMD = FALSE;

  if ((Distance != 0) && (DisTrue_Flag != FALSE)) {
    dis = Distance + 20 - Var_Sub;
  } else {
    dis = 0xFFFF;//---------------------------------------------未检测到回波时输出0xFFFF
  }

  TMR_Reset(USE_TMRx);
  TMR_Reset(TMR3);
  DAC_Reset();
  COMP_Reset();

  GPIO_SetBit(OPA_VCC_PORT, OPA_VCC_PIN);   // 运放断电

  printf("TIMER3_IF_Counter = %u\r\n", TIMER3_IF_Counter);
  TIMERx_IF_Counter = 0;
  TIMER3_IF_Counter = 0;

  Distance = 0;   // 捕获的距离信息清空
  DisTrue_Flag = FALSE;

  return dis;
}

int16_t get_Temp(void)//获取温度
{
  uint16_t i = 0;
  volatile uint16_t ADC_Value;
  GPIO_SetBit(TEMP_VCC_PORT, TEMP_VCC_PIN);   // 热敏电阻通电
  ADC_setup();
  ADC_StartConversion();   // 启动转换
  while ((ADC->STS & ADC_FLAG_CS) == RESET);//等待数据有效
  ADC_ReadConversionValue();//获取ADC采样值, 舍弃第一次的测量数据

#if 0
  volatile uint32_t ADC_Buffer = 0;
  for (i = 0;i < 10;i++) {
    while ((ADC->STS & ADC_FLAG_CS) == RESET);//等待数据有效
    ADC_Buffer += (uint16_t)ADC_ReadConversionValue();//获取ADC采样值
  }
  ADC_Value = ADC_Buffer / 10;
#else
  uint16_t dataBuf[11] = { 0 };
  for (i = 0; i < 11; i++) {
    while ((ADC->STS & ADC_FLAG_CS) == RESET);//等待数据有效
    dataBuf[i] = ADC_ReadConversionValue();//获取ADC采样值
  }
  for (uint16_t j = 0; j < 10; j++) {   // 从小到大排序
    for (uint16_t k = 0; k < 10 - j; k++) {
      if (dataBuf[k] > dataBuf[k + 1]) {
        uint16_t temp = dataBuf[k];
        dataBuf[k] = dataBuf[k + 1];
        dataBuf[k + 1] = temp;
      }
    }
  }
  ADC_Value = dataBuf[5];
#endif

  ADC_StopConversion();   // 停止转换
  ADC_Reset();
  GPIO_ClearBit(TEMP_VCC_PORT, TEMP_VCC_PIN);   // 热敏电阻断电

  for (i = 0;i < 33;i++) {
    if ((ADC_Value >= NCP18WF104[i]) && (ADC_Value < NCP18WF104[i + 1]))break;
  }

  return (ADC_Value - NCP18WF104[i]) * 50 / (NCP18WF104[i + 1] - NCP18WF104[i]) + i * 50 - 400;
}

void get_Dis_Handler(void)
{
  Return_GetDis(get_Dis());
}

void get_Temp_Handler(void)
{
  Return_GetTemp(get_Temp());
}

void set_Addr_Handler(void)
{
  readPage();
  EE_Buffer.EEpage.addr = RxMessage.set_addr_CMD.addr_New;
  writePage();
  device_Addr = RxMessage.set_addr_CMD.addr_New;
  Return_SetAddr(TRUE);
}

void set_Band_Handler(void)
{
  uint8_t Status = FALSE;
  uint32_t Baudrate_t = 0;
  switch (RxMessage.set_Band_CMD.Data) {
  case 0x00:Baudrate_t = 1200;	Status = TRUE; break;
  case 0x01:Baudrate_t = 2400;	Status = TRUE; break;
  case 0x02:Baudrate_t = 4800;	Status = TRUE; break;
  case 0x03:Baudrate_t = 9600;	Status = TRUE; break;
  case 0x04:Baudrate_t = 14400;	Status = TRUE; break;
  case 0x05:Baudrate_t = 19200;	Status = TRUE; break;
  case 0x06:Baudrate_t = 28800;	Status = TRUE; break;
  case 0x07:Baudrate_t = 38400;	Status = TRUE; break;
  case 0x08:Baudrate_t = 57600;	Status = TRUE; break;
  case 0x09:Baudrate_t = 115200;Status = TRUE; break;
  case 0x0A:Baudrate_t = 128000;Status = TRUE; break;
  case 0x0B:Baudrate_t = 256000;Status = TRUE; break;

  default:                    Status = FALSE;break;
  }
  if (Status == TRUE) {
    readPage();
    EE_Buffer.EEpage.baudRate = Baudrate_t;
    Baudrate = Baudrate_t;
    writePage();
  }
  Return_SetBand(Status);
  USART1_Init();
}

void Restore_Check(void)
{
  if (((USART1_TX_PORT->IDATA & USART1_TX_PIN) == BIT_SET) &&
    ((USART1_RX_PORT->IDATA & USART1_RX_PIN) == BIT_RESET)) {
    APM_EVAL_DelayMs(10);
    for (uint8_t i = 0;i < 90;i++) {
      if (((USART1_TX_PORT->IDATA & USART1_TX_PIN) == BIT_RESET) ||
        ((USART1_RX_PORT->IDATA & USART1_RX_PIN) == BIT_SET))
        return;
      else APM_EVAL_DelayMs(10);
    }
    device_Addr = device_Addr_DEFAULT;EE_Buffer.EEpage.addr = device_Addr_DEFAULT;//模块地址复位
    Baudrate = Baudrate_DEFAULT;EE_Buffer.EEpage.baudRate = Baudrate_DEFAULT;//波特率复位
    EE_Buffer.EEpage.factory_Flag = factory_Flag_DEFAULT;//出厂标志复位
    EE_Buffer.EEpage.mode = RunMode;
    writePage();
    for (uint16_t j = 0;j < EE_Size;j++)EE_Buffer.data[j] = 0x00;

    GPIO_Config_T gpioConfig;
    GPIO_SetInputFloat(USART1_TX_PORT, USART1_TX_PIN);
    GPIO_SetInputFloat(USART1_RX_PORT, USART1_RX_PIN);

    while (1) {
      LED_PORT->ODATA ^= LED_PIN;
      APM_EVAL_DelayMs(50);
    }
  }
}

void Parameters_Setup(void)
{
  readPage();
  if (EE_Buffer.EEpage.factory_Flag != factory_Flag_DEFAULT)//出厂固件烧录后第一次运行时初始化出厂参数
  {
    device_Addr = device_Addr_DEFAULT;EE_Buffer.EEpage.addr = device_Addr_DEFAULT;
    Baudrate = Baudrate_DEFAULT;EE_Buffer.EEpage.baudRate = Baudrate_DEFAULT;
    EE_Buffer.EEpage.factory_Flag = factory_Flag_DEFAULT;
    EE_Buffer.EEpage.mode = RunMode;
    writePage();
  } else {
    device_Addr = EE_Buffer.EEpage.addr;
    Baudrate = EE_Buffer.EEpage.baudRate;
  }
}

void USART_Write(uint8_t* TxData, uint8_t Tx_numByte)
{
  for (uint8_t i = 0; i < Tx_numByte; i++) {
    while (USART_ReadStatusFlag(USART1, USART_FLAG_TXBE) == RESET);
    USART_TxData(USART1, TxData[i]);
  }
}

uint8_t mystrstr(const uint8_t* s, char* t)
{
  uint16_t i = 0, s_num_max = 100;

  for (;i < s_num_max; s++, i++) {
    while (*t == *s) {
      if (*++t == '\0') {
        // GPIO_SetBit(LED_PORT, LED_PIN);
        return 1;
      } else if ((i++ == s_num_max) || (*++s == '\0')) {
        return NULL;
      }
    }
  }
  return NULL;
}
