#ifndef __DF_PROTOCOL_H__
#define __DF_PROTOCOL_H__

#include "all_include.h"

#define  RxBufferSize 100

typedef struct {

  uint8_t revTimeout;
  uint8_t revByteCounter;
  uint8_t dataBuf[RxBufferSize];
  uint8_t revEmptyFlag;

}revBufTypeDef;
extern revBufTypeDef revBuf;

union RxMessage_t
{
  uint8_t Buffer[8];

  struct { uint16_t Header; uint8_t Address; uint8_t data_Len; uint8_t CMD;                  uint8_t Checksum; } get_Dis_CMD;   //测量距离

  struct { uint16_t Header; uint8_t Address; uint8_t data_Len; uint8_t CMD;                  uint8_t Checksum; } get_Temp_CMD;  //获取温度值

  struct { uint16_t Header; uint8_t Address; uint8_t data_Len; uint8_t CMD; uint8_t addr_New;uint8_t Checksum; } set_addr_CMD;  //设置地址

  struct { uint16_t Header; uint8_t Address; uint8_t data_Len; uint8_t CMD; uint8_t Data;    uint8_t Checksum; } set_Band_CMD;  //设置波特率

  struct { uint16_t Header; uint8_t Address; uint8_t data_Len; uint8_t CMD; uint8_t Data[3]; } Common;//---------------------公共结构
};


typedef union
{
  uint8_t Buffer[8];

  struct { uint16_t Header; uint8_t Address; uint8_t data_Len; uint8_t CMD; uint8_t data[2]; uint8_t Checksum; } get_Dis_RET;   //测量距离

  struct { uint16_t Header; uint8_t Address; uint8_t data_Len; uint8_t CMD; uint8_t data[2]; uint8_t Checksum; } get_Temp_RET;  //获取温度值

  struct { uint16_t Header; uint8_t Address; uint8_t data_Len; uint8_t CMD; uint8_t status;  uint8_t Checksum; } set_addr_RET;  //设置地址

  struct { uint16_t Header; uint8_t Address; uint8_t data_Len; uint8_t CMD; uint8_t status;  uint8_t Checksum; } set_Band_RET;  //设置波特率

  struct { uint16_t Header; uint8_t Address; uint8_t data_Len; uint8_t CMD; uint8_t Data[3]; } Common;//---------------------公共结构
}RetMessage_TypeDef;

extern union RxMessage_t  RxMessage;

#define  get_Dis_RET_numByte  8
#define get_Temp_RET_numByte  8
#define set_addr_RET_numByte  7
#define set_Band_RET_numByte  7

#define device_CommonAddr 0xAB

#define get_Dis_RET_DEFAULT     \
  { 0xAA55,           /* 数据帧头         	*/\
    0x11,             /* 地址.          	*/\
    0x02,             /* 数据长度.      	  */\
    0x02,             /* 命令. 						*/\
    0x00,             /* 高位. 						*/\
    0x00,             /* 低位. 						*/\
    0x00,             /* CRC检验值. 				*/\
  }
#define get_Temp_RET_DEFAULT    \
  { 0xAA55,           /* 数据帧头					*/\
    0x11,             /* 地址. 						*/\
    0x02,             /* 数据长度. 				*/\
    0x03,             /* 命令. 						*/\
    0x00,             /* 高位. 						*/\
    0x00,             /* 低位. 						*/\
    0x00,             /* CRC检验值. 				*/\
  }
#define set_addr_RET_DEFAULT    \
  { 0xAA55,           /* 数据帧头 					*/\
    0x11,             /* 地址. 						*/\
    0x01,             /* 数据长度. 				*/\
    0x55,             /* 命令. 						*/\
    0xEE,             /* 状态. 						*/\
    0x00,             /* CRC检验值. 				*/\
  }

#define set_Band_RET_DEFAULT    \
  { 0xAA55,           /* 数据帧头 					*/\
    0x11,             /* 地址. 						*/\
    0x01,             /* 数据长度. 				*/\
    0x08,             /* 命令. 						*/\
    0xEE,             /* 状态. 						*/\
    0x00,             /* CRC检验值. 				*/\
  }

void DF_Protocol_Parse(void);

void clear_RxMessage(void);
uint8_t Load_RxMessage(void);

void Return_GetDis(uint16_t Distance_Val);
void Return_GetTemp(uint16_t Temperature_Val);
void Return_SetAddr(uint8_t SetAddr_Status);
void Return_SetBand(uint8_t SetBand_Status);

#endif /* __DF_PROTOCOL_H__ */
