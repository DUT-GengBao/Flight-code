#ifndef __MY_TASK_H
#define __MY_TASK_H

//==ÒýÓÃ
#include "sysconfig.h"
#include "Ano_FcData.h"

#define NOTSTART 0
#define WORKING 1
#define FINISH  2
typedef struct
{
	u8 state;
}_task_flag;



void Inspection_Task(u8 dT_ms);
	void Loc_redpoint(u8 dT_ms);
void My_Game_Task(u8 dT_ms);

void StageTask1(u32 timestamp);
void StageTask2(u32 timestamp);
void StageTask3(u32 timestamp);
void StageTask4(u32 timestamp);
void StageTask5(u32 timestamp);
void StageTask6(u32 timestamp);
void StageTask7(u32 timestamp);
void StageTask8(u32 timestamp);
void StageTask9(u32 timestamp);
void StageTask10(u32 timestamp);
void StageTask11(u32 timestamp);
u8 GetStage(void);
#endif

