#include"DF_Protocol.h"

revBufTypeDef revBuf;

union RxMessage_t   RxMessage;

void DF_Protocol_Parse(void)
{
  if (Load_RxMessage() == TRUE) { //-------------------------------------------��ָ֤���Ƿ���ȷ
    GPIO_SetBit(LED_PORT, LED_PIN);//-----------------------------------ָʾ���յ���ȷָ�ʼ����
    switch (RxMessage.Common.CMD) {
    case 0x02: get_Dis_Handler(); break;//-----------------------------��ȡ��������
    case 0x03: get_Temp_Handler();break;//-----------------------------��ȡ�¶�
    case 0x55: set_Addr_Handler();break;//-----------------------------���õ�ַ
    case 0x08: set_Band_Handler();break;//-----------------------------���ò�����
    default:break;
    }
    GPIO_ClearBit(LED_PORT, LED_PIN);//-----------------------------------ָʾ���յ���ȷָ�ʼ����
  } else {
    if (mystrstr((const uint8_t*)revBuf.dataBuf, "Debug_")) {
      if (mystrstr((const uint8_t*)revBuf.dataBuf, "Debug_Copyright")) {//---------------Ӣ�İ�Ȩ˵��
        printf(" ***************************************************************\r\n");
        printf(" * @products 	URM07-UART\r\n");
        printf(" * @author   	Strictus\r\n");
        printf(" * @version  	1.0\r\n");
        printf(" * @date     	30-August-2016\r\n");
        printf(" * @ Copyright(C) 2016 DFRobot,all right reserved.\r\n");
        printf(" ***************************************************************\r\n");
      } else if (mystrstr((const uint8_t*)revBuf.dataBuf, "Debug_��Ȩ")) {//---------------���İ�Ȩ˵��������̼�����ȡ��Ӣ�İ�Ȩ��Ϣ���ױ��۸�
        printf(" ��������������������������������������������������������������������\r\n");
        printf(" * @��Ʒ���� 	URM07-UART\r\n");
        printf(" * @����     	�Ų���\r\n");
        printf(" * @�汾     	һ����\r\n");
        printf(" * @����     	����һ���������ʮ��\r\n");
        printf(" * @(�й�)�Ϻ���λ�����˹ɷ����޹�˾ ��Ȩ���У�����������Ȩ����\r\n");
        printf(" ��������������������������������������������������������������������\r\n");
      }
    } else {//�������ݻ��������......................
    }
    memset(revBuf.dataBuf, 0, sizeof(revBuf.dataBuf));   // ����ſ����������buf
  }
}

void clear_RxMessage(void)
{
  for (uint8_t i = 0;i < 8;i++) RxMessage.Buffer[i] = 0;
  revBuf.revByteCounter = 0;
  revBuf.revEmptyFlag = TRUE;
}

uint8_t Load_RxMessage(void)
{
  uint8_t Rx_Checksum_Temp = 0;
  for (uint8_t i = 0;i < 8;i++) RxMessage.Buffer[i] = revBuf.dataBuf[i];
  if ((RxMessage.Common.Header != 0xAA55) || ((RxMessage.Common.Address != device_Addr) && (RxMessage.Common.Address != device_CommonAddr)) || (RxMessage.Common.data_Len > 2)) {
    clear_RxMessage();
    return FALSE;
  }

  else {
    for (uint8_t i = 0;i < 5 + RxMessage.Common.data_Len;i++)Rx_Checksum_Temp += RxMessage.Buffer[i];
    if (Rx_Checksum_Temp == RxMessage.Buffer[5 + RxMessage.Common.data_Len]) return TRUE;
    else {
      clear_RxMessage();
      return FALSE;
    }
  }
}

uint8_t Ret_Checksum_Cal(RetMessage_TypeDef aa)
{
  uint32_t Ret_Checksum_Temp = 0;
  for (uint8_t i = 0;i < 5 + aa.Common.data_Len;i++) {
    Ret_Checksum_Temp += aa.Buffer[i];
  }
  return (uint8_t)Ret_Checksum_Temp;
}

void Return_GetDis(uint16_t Distance_Val)
{
  RetMessage_TypeDef RetMessage_t = { .get_Dis_RET = get_Dis_RET_DEFAULT };

  RetMessage_t.get_Dis_RET.Address = device_Addr;
  RetMessage_t.get_Dis_RET.data[0] = Distance_Val / 256;
  RetMessage_t.get_Dis_RET.data[1] = Distance_Val % 256;
  RetMessage_t.get_Dis_RET.Checksum = Ret_Checksum_Cal(RetMessage_t);

  USART_Write(RetMessage_t.Buffer, get_Dis_RET_numByte);
}

void Return_GetTemp(uint16_t Temperature_Val)
{
  RetMessage_TypeDef RetMessage_t = { .get_Temp_RET = get_Temp_RET_DEFAULT };

  RetMessage_t.get_Temp_RET.Address = device_Addr;
  RetMessage_t.get_Temp_RET.data[0] = Temperature_Val / 256;
  RetMessage_t.get_Temp_RET.data[1] = Temperature_Val % 256;
  RetMessage_t.get_Temp_RET.Checksum = Ret_Checksum_Cal(RetMessage_t);
  printf("Temperature_Val = %d \r\n", (int16_t)Temperature_Val);

  USART_Write(RetMessage_t.Buffer, get_Temp_RET_numByte);
}

void Return_SetAddr(uint8_t SetAddr_Status)
{
  uint8_t tem_Status = 0xEE;
  if (SetAddr_Status == TRUE) tem_Status = 0xCC;

  RetMessage_TypeDef RetMessage_t = { .set_addr_RET = set_addr_RET_DEFAULT };

  RetMessage_t.set_addr_RET.Address = device_Addr;
  RetMessage_t.set_addr_RET.status = tem_Status;
  RetMessage_t.set_addr_RET.Checksum = Ret_Checksum_Cal(RetMessage_t);

  USART_Write(RetMessage_t.Buffer, set_addr_RET_numByte);
}

void Return_SetBand(uint8_t SetBand_Status)
{
  uint8_t tem_Status = 0xEE;
  if (SetBand_Status == TRUE) tem_Status = 0xCC;

  RetMessage_TypeDef RetMessage_t = { .set_Band_RET = set_Band_RET_DEFAULT };

  RetMessage_t.set_Band_RET.Address = device_Addr;
  RetMessage_t.set_Band_RET.status = tem_Status;
  RetMessage_t.set_Band_RET.Checksum = Ret_Checksum_Cal(RetMessage_t);
  USART_Write(RetMessage_t.Buffer, set_Band_RET_numByte);
  while (USART_ReadStatusFlag(USART1, USART_FLAG_TXBE) == RESET);
}
