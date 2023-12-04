#include "flash.h"

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

union EE_Buffer_Union  EE_Buffer;

void readPage(void)//读取参数
{
  uint32_t* p = (uint32_t*)malloc(EE_Size);   // 因为直接使用联合体, 会进入HardFault_Handler
  // uint32_t* p = (uint32_t*)&EE_Buffer;
  for (uint32_t i = 0; i < EE_Size / 4; i++) {
    p[i] = *(__IO uint32_t*)(USER_PAGE + i * 4);
  }
  memcpy(&EE_Buffer, p, EE_Size);
  free(p);
  p = NULL;
}

void writePage(void)//写入参数
{
  uint32_t* p = (uint32_t*)malloc(EE_Size);   // 因为直接使用联合体, 会进入HardFault_Handler
  memcpy(p, &EE_Buffer, EE_Size);
  FMC_Unlock();
  FMC_ErasePage(USER_PAGE);
  for (uint32_t i = 0; i < EE_Size / 4; i++) {
    FMC_ProgramWord(USER_PAGE + i * 4, p[i]);
  }
  FMC_Lock();
  free(p);
  p = NULL;
}
