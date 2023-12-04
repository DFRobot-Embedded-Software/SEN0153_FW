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
void Restore_Check(void);
void Parameters_Setup(void);

int16_t get_Temp(void);














#endif

