#ifndef __FUNC_H
#define __FUNC_H

#include "all_include.h"


extern volatile uint16_t Var_Sub;
extern uint8_t shift_ch_CMD;
extern const uint16_t NCP18WF104[34];


void Send_40K(uint8_t Num);

void SetVs(void);

uint16_t get_Dis(void);
int16_t get_Temp(void);

void get_Dis_Handler(void);
void get_Temp_Handler(void);
void set_Addr_Handler(void);
void set_Band_Handler(void);
uint8_t mystrstr(const uint8_t *s,char *t);
void USART_Write(uint8_t *TxData,uint8_t Tx_numByte);
void readPage(void);
void writePage(void);
void Restore_Check(void);
void GPIO_setupRx(void);
void Parameters_Setup(void);

int16_t get_Temp(void);

// #define USER_PAGE    FLASH_WRP_PAGE_60_63 //用户页用于存储参数
#define USER_PAGE    0x0800F000 //用户页用于存储参数

#pragma  pack (1) //1byte

#define page_Size 512
#define EE_Size 12


union  EE_Buffer_Union
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














#endif

