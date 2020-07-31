#ifndef __DRV_OPENMV2_H
#define __DRV_OPENMV2_H

//==引用
#include "sysconfig.h"
#include "Ano_FcData.h"

//==数据声明
extern _openmv_data_st opmv2;

//==函数声明

//static
static void OpenMV2_Data_Analysis(u8 *buf_data,u8 len);
static void OpenMV2_Check_Reset(void);

//public
void OpenMV2_Offline_Check(u8 dT_ms);
void OpenMV2_Byte_Get(u8 bytedata);


#endif

