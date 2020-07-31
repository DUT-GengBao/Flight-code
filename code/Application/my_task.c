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

u8 my_game_flag = 0;		//Ĭ��ΪѲ��ģʽ

void My_Game_Task(u8 dT_ms)
{
	if(my_game_flag == 1)
		Inspection_Task(dT_ms);				//Ѳ��ģʽ
	else if(my_game_flag == 2)
		Loc_redpoint(dT_ms);					//100g���붨��ģʽ
}
void Loc_redpoint(u8 dT_ms)
{
	Timestamp++;
	if(loc_state ==1)
	{
		OpenMvSwitch(1);  //openmv�л���1
		//��openmv����ָ���л�������
//		if(opmv.mode_sta != 0x41)	//��openmv��������  ֱ��opmv�л�������ģʽ
//		{
//			OpenMv_working_mode_set(0x31);
//		}else
//		{
			if(fly_automation_flag == 6)
			{
				if(Timestamp < 60)
				{
					if(Timestamp < 20)
						color(5,5);//�ٻ�
					else if(Timestamp < 40)
						color(20,20);
					else
						color(5,5);//�ٻ�
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
					Ano_CmdCtrl(0,0,1);//���
					color(20,20);		//�ص�
			//		UARTprintf("take off !\r\n");
				}
				else if(flag.auto_take_off_land == AUTO_TAKE_OFF_FINISH)
				{
					loc_state = 2;		//�Զ������ɺ󣬽���׶�2
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
				Ano_CmdCtrl(0,0,2);		//12s����
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
//		OpenMvSwitch(1);  //openmv�л���1
//		//��openmv����ָ���л�������
//		if(opmv.mode_sta != 0x42)	//��openmv��������  ֱ��opmv�л�������ģʽ
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
//һ�����
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
		Timestamp = 0; //���ǰ�����м�ʱ
	}
	
	if(fly_automation_flag == 3)
	{
		if(CH_N[AUX4]>300 && flag.unlock_sta  == 0)
		{
			Ano_CmdCtrl(0,0,1);//���
			color(20,20);		//�ص�
	//		UARTprintf("take off !\r\n");
		}
		else if(flag.auto_take_off_land == AUTO_TAKE_OFF_FINISH)
		{
			Ano_CmdCtrl(25,25,7); //����25
			
			stage = 2;		//�Զ������ɺ󣬽���׶�2
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
		Ano_CmdCtrl(110,25,5);		//ǰ��110cm
		Timestamp = 0;
		stage =3;
	//	UARTprintf("stage2 finished !\r\n");
	}
	
}


void StageTask3(u32 timestamp)//�ڶ���������
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


void StageTask4(u32 timestamp)//�ȴ��Ƹ�
{
	if(openmv_switch_flag == 2)//����B
	{
		if(Timestamp < 100)
			return;
		else
		{
			Timestamp = 0;
			Ano_CmdCtrl(270,6,9);//����270��
			stage = 5;
	//		UARTprintf("stage4 finished !\r\n");
		}
	}
}


void StageTask5(u32 timestamp)//����90��
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


void StageTask6(u32 timestamp)//ǰ��60cm
{
	if(Timestamp < 50)//9000/50
		return;
	else
	{
		Timestamp = 0;
		Ano_CmdCtrl(60,25,6);		//����
		stage = 7;
	//	UARTprintf("stage6 finished !\r\n");
	}
}
void StageTask7(u32 timestamp)//��ƽ��20cm
{
	if(Timestamp < 32)//1600/50
		return;
	else
	{
		Timestamp = 0;
		Ano_CmdCtrl(25,25,8);
		OpenMvSwitch(1);//�л�������ͷ1
		stage = 8;
	//	UARTprintf("stage7 finished !\r\n");
	}
}
void StageTask8(u32 timestamp)//Ѳ�߷��أ��ȴ���⵽A����20cm
{
	if(openmv_switch_flag == 0)//�ȴ���⵽A
	{
		if(Timestamp < 16)//800/50
			return;
		else
		{		
			//��⵽A
			Timestamp = 0;
			Ano_CmdCtrl(30,25,8);//��������
			stage = 9;
	//		UARTprintf("stage8 finished !\r\n");
		}
	}
}
void StageTask9(u32 timestamp)//����
{
	if(Timestamp < 30)//800/50
		return;
	else
	{
		//ǰ��//����
		Ano_CmdCtrl(50,25,6);	
		stage = 10;
	//	UARTprintf("stage9 finished !\r\n");
	}
}
void StageTask10(u32 timestamp)//����
{
	if(Timestamp < 50)//800/50
		return;
	else
	{
	//����
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

