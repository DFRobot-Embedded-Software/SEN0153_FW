#ifndef __FLASH_H__
#define __FLASH_H__

#include "main.h"

// #define USER_PAGE    FLASH_WRP_PAGE_60_63 //用户页用于存储参数
#define USER_PAGE    0x0800F000 //用户页用于存储参数

#pragma  pack (1) //1byte

#define page_Size 512
#define EE_Size 12

union EE_Buffer_Union
{
 uint8_t data[EE_Size];//512byte，每4个byte一个字
 struct
	{
		uint16_t addr;
		uint16_t mode;
		uint32_t baudRate;
		uint32_t factory_Flag;
	}EEpage;
};

extern union EE_Buffer_Union EE_Buffer;

void readPage(void);
void writePage(void);

void appFlagInit(void);

#endif /* __FLASH_H__ */
