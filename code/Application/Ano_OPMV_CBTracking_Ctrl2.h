#ifndef __ANO_OPMV_CBTRACKING_CTRL2_H
#define __ANO_OPMV_CBTRACKING_CTRL2_H

//==引用
#include "sysconfig.h"
#include "Ano_FcData.h"



//==数据声明
extern _ano_opmv_cbt_ctrl_st ano_opmv_cbt2_ctrl;

//==函数声明

//static
static void ANO_CBTracking2_Decoupling(u8 *dT_ms,float rol_degs,float pit_degs);
static void ANO_CBTracking2_Calcu(u8 *dT_ms,s32 relative_height_cm);
//public
void ANO_CBTracking2_Task(u8 dT_ms);
void ANO_CBTracking2_Ctrl(u8 *dT_ms,u8 en);




#endif
