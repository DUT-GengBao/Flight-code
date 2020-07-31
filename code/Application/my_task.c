#include "my_task.h"
#include "Ano_OPMV_Ctrl.h"
#include "Ano_ProgramCtrl_User.h"
#include "Ano_RC.h"
#include "Ano_FlyCtrl.h"
#include "uartstdio.h"
#include "Drv_key.h"
#include "Ws2812.h"
#include "Drv_OpenMV.h"

_task_flag task_flag1,task_flag2,task_flag3,task_flag4,task_flag5;

static u8 stage=1,loc_state=1;
static u32 Timestamp;

u8 my_game_flag = 0;		//默认为巡检模式

void My_Game_Task(u8 dT_ms)
{
	if(my_game_flag == 1)
		Inspection_Task(dT_ms);				//巡检模式
	else if(my_game_flag == 2)
		Loc_redpoint(dT_ms);					//100g砝码定点模式
}
void Loc_redpoint(u8 dT_ms)
{
	Timestamp++;
	if(loc_state ==1)
	{
		OpenMvSwitch(1);  //openmv切换到1
		//向openmv发送指令切换到定点
//		if(opmv.mode_sta != 0x41)	//向openmv发送命令  直到opmv切换到定点模式
//		{
//			OpenMv_working_mode_set(0x31);
//		}else
//		{
			if(fly_automation_flag == 6)
			{
				if(Timestamp < 60)
				{
					if(Timestamp < 20)
						color(5,5);//橘黄
					else if(Timestamp < 40)
						color(20,20);
					else
						color(5,5);//橘黄
				//	UARTprintf("take off Countdown:%d \r\n",6- Timestamp/20);
	//				return;
				}
				else
				{
					fly_automation_flag = 7;
				//	UARTprintf("fly_automation_flag = 7 \r\n");
				}
			}else
			{
				Timestamp=0;
			}
				
			if(fly_automation_flag == 7)
			{
				if(CH_N[AUX4]>300 && flag.unlock_sta  == 0)
				{
					Ano_CmdCtrl(0,0,1);//起飞
					color(20,20);		//关灯
			//		UARTprintf("take off !\r\n");
				}
				else if(flag.auto_take_off_land == AUTO_TAKE_OFF_FINISH)
				{
					loc_state = 2;		//自动起飞完成后，进入阶段2
					Timestamp = 0;
			//		UARTprintf("stage1 finished !\r\n");
				}
			}
//		}
		}else if(loc_state ==2)
		{
			if(Timestamp < 240)//12s
				return;
			else
			{
				Ano_CmdCtrl(0,0,2);		//12s后降落
				loc_state = 3;
			}
		}else if(loc_state ==3)
		{
			color(6,6);
		}
	}



void Inspection_Task(u8 dT_ms)
{
	Timestamp ++;
	if(stage == 1)
	{
//		OpenMvSwitch(1);  //openmv切换到1
//		//向openmv发送指令切换到定点
//		if(opmv.mode_sta != 0x42)	//向openmv发送命令  直到opmv切换到定点模式
//		{
//			OpenMv_working_mode_set(0x32);
//		}else
//		{
			StageTask1(Timestamp);
//		}
	}
	else if(stage == 2)
	{
		StageTask2(Timestamp);
	}else if(stage == 3)
	{
		StageTask3(Timestamp);
	}else if(stage == 4)
	{
		StageTask4(Timestamp);
	}else if(stage == 5)
	{
		StageTask5(Timestamp);
	}else if(stage == 6)
	{
		StageTask6(Timestamp);
	}else if(stage == 7)
	{
		StageTask7(Timestamp);
	}
	else if(stage == 8)
	{
		StageTask8(Timestamp);
	}
		else if(stage == 9)
	{
		StageTask9(Timestamp);
	}
		else if(stage == 10)
	{
		StageTask10(Timestamp);
	}
	else
	{
		Timestamp = 0;
	}
}
//一键起飞
void StageTask1(u32 timestamp)
{
	if(fly_automation_flag == 2)
	{
		if(Timestamp < 60)
		{
			if(Timestamp < 20)
				color(6,6);//green
			else if(Timestamp < 40)
				color(20,20);
			else
				color(6,6);//green
		//	UARTprintf("take off Countdown:%d \r\n",6- Timestamp/20);
			return;
		}
		else
		{
			fly_automation_flag = 3;
			UARTprintf("fly_automation_flag = 3 \r\n");
		}
	}else
	{
		Timestamp = 0; //起飞前不进行计时
	}
	
	if(fly_automation_flag == 3)
	{
		if(CH_N[AUX4]>300 && flag.unlock_sta  == 0)
		{
			Ano_CmdCtrl(0,0,1);//起飞
			color(20,20);		//关灯
	//		UARTprintf("take off !\r\n");
		}
		else if(flag.auto_take_off_land == AUTO_TAKE_OFF_FINISH)
		{
			Ano_CmdCtrl(25,25,7); //左移25
			
			stage = 2;		//自动起飞完成后，进入阶段2
			Timestamp = 0;
		//	UARTprintf("stage1 finished !\r\n");
		}
	}else
	{
		 
	}
	
}


void StageTask2(u32 timestamp)
{
	if(Timestamp < 30)//30
		return;
	else
	{
//		Ano_CmdCtrl(0,0,2);
		Ano_CmdCtrl(110,25,5);		//前进110cm
		Timestamp = 0;
		stage =3;
	//	UARTprintf("stage2 finished !\r\n");
	}
	
}


void StageTask3(u32 timestamp)//第二次向左走
{
	if(Timestamp < 100)
		return;
	else
	{
		Ano_CmdCtrl(20,5,7);
		OpenMvSwitch(1);
		Timestamp = 0;
		stage = 4;
	//	UARTprintf("stage3 finished !\r\n");
	}
}


void StageTask4(u32 timestamp)//等待绕杆
{
	if(openmv_switch_flag == 2)//发现B
	{
		if(Timestamp < 100)
			return;
		else
		{
			Timestamp = 0;
			Ano_CmdCtrl(270,6,9);//左旋270度
			stage = 5;
	//		UARTprintf("stage4 finished !\r\n");
		}
	}
}


void StageTask5(u32 timestamp)//右旋90度
{
	if(Timestamp < 900)//27000/50
		return;
	else
	{	
		Timestamp = 0;
		Ano_CmdCtrl(90,45,0x0A);
		OpenMvSwitch(0);
		stage = 6;
//		UARTprintf("stage5 finished !\r\n");
	}
}


void StageTask6(u32 timestamp)//前进60cm
{
	if(Timestamp < 50)//9000/50
		return;
	else
	{
		Timestamp = 0;
		Ano_CmdCtrl(60,25,6);		//后退
		stage = 7;
	//	UARTprintf("stage6 finished !\r\n");
	}
}
void StageTask7(u32 timestamp)//左平移20cm
{
	if(Timestamp < 32)//1600/50
		return;
	else
	{
		Timestamp = 0;
		Ano_CmdCtrl(25,25,8);
		OpenMvSwitch(1);//切换到摄像头1
		stage = 8;
	//	UARTprintf("stage7 finished !\r\n");
	}
}
void StageTask8(u32 timestamp)//巡线返回，等待检测到A右移20cm
{
	if(openmv_switch_flag == 0)//等待检测到A
	{
		if(Timestamp < 16)//800/50
			return;
		else
		{		
			//检测到A
			Timestamp = 0;
			Ano_CmdCtrl(30,25,8);//右移命令
			stage = 9;
	//		UARTprintf("stage8 finished !\r\n");
		}
	}
}
void StageTask9(u32 timestamp)//降落
{
	if(Timestamp < 30)//800/50
		return;
	else
	{
		//前进//后退
		Ano_CmdCtrl(50,25,6);	
		stage = 10;
	//	UARTprintf("stage9 finished !\r\n");
	}
}
void StageTask10(u32 timestamp)//降落
{
	if(Timestamp < 50)//800/50
		return;
	else
	{
	//降落
		Ano_CmdCtrl(0,0,2);	
		stage = 11;
	//	UARTprintf("stage10 finished !\r\n");
	}
}
u8 GetStage(void)
{
	return stage;
}

//ANO_DT_SendString("5s...");

