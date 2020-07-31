/*==========================================================================
 * ����    ��
						 
 
 * ����ʱ�䣺2019-07-21 
 * ����		 �������ƴ�-Jyoun
 * ����    ��www.anotc.com
 * �Ա�    ��anotc.taobao.com
 * ����QȺ ��190169595
 * ��Ŀ������18084888982��18061373080
============================================================================
 * �����ƴ��ŶӸ�л��ҵ�֧�֣���ӭ��ҽ�Ⱥ���ཻ�������ۡ�ѧϰ��
 * �������������в��õĵط�����ӭ����ש�������
 * �������������ã�����������Ƽ���֧�����ǡ�
 * ������Դ������뻶ӭ�������á��������չ��������ϣ������ʹ��ʱ��ע��������
 * ����̹������С�˳����ݣ��������������ˮ���������ӣ�Ҳ��δ�й�Ĩ��ͬ�е���Ϊ��  
 * ��Դ���ף�����������ף�ϣ����һ������ء����ﻥ������ͬ������
 * ֻ������֧�֣������������ø��á�  
===========================================================================*/

//Ĭ������
#include "Ano_OPMV_LineTracking_Ctrl.h"
#include "Ano_OPMV_CBTracking_Ctrl.h"
#include "Ano_OPMV_CBTracking_Ctrl2.h"
#include "Drv_OpenMV.h"
#include "Ano_OPMV_Ctrl.h"
#include "Ano_Math.h"
#include "Ano_Filter.h"
#include "Ano_ProgramCtrl_User.h"
#include "Drv_OpenMV2.h"
//���ݽӿڶ��壺
//=========mapping===============
//��Ҫ���õ��ļ���
#include "Ano_FlightCtrl.h"
#include "ws2812.h"
#include "uartstdio.h"
u8 openmv_ct_en = 1; //Ĭ�Ͽ���openmv����,�Լ����

//��Ҫ�������õ��ⲿ������
#define RELATIVE_HEIGHT_CM           (jsdata.valid_of_alt_cm)  //��Ը߶�



//��Ҫ������ֵ���ⲿ������


//===============================
//ȫ�ֱ�����
_opmv_ct_sta_st opmv_ct_sta;

u8 openmv_switch_flag = 0;		//�л�2��openmv��1Ϊ���µ�openmv��2Ϊǰ���openmv
//�����趨��


/**********************************************************************************************************
*�� �� ��: ANO_OPMV_Ctrl_Task
*����˵��: �����ƴ�OPMV��������
*��    ��: ����ʱ��(ms)
*�� �� ֵ: ��
**********************************************************************************************************/
void ANO_OPMV_Ctrl_Task(u8 dT_ms)
{
	//�жϸ߶ȱ�ǵ�����
	if( RELATIVE_HEIGHT_CM >40)
	{
		//��ɴ���40���ף������λ
		opmv_ct_sta.height_flag =1;
	}
	//��λ�߶ȱ�ǵ�����
	if(flag.unlock_sta ==0)
	{
		//�������Ǹ�λ
		opmv_ct_sta.height_flag =0;
	}
	
	//�жϿ�����������������ƣ�
	if
	(
		switchs.of_flow_on                   //������Ч
		&& switchs.opmv_on                   //mv��Ч
		//&& opmv_ct_sta.height_flag !=0       //�߶ȱ�ǲ�Ϊ0
		&& flag.flight_mode2 == 1            //AUX2ͨ��ģʽֵΪ1���ο�FlightCtrl.c�ڳ���
		&& openmv_ct_en==1		//�Լ����
	)
	{
		opmv_ct_sta.en = 1;
	}
	else
	{
		opmv_ct_sta.en = 0;
	}
	
	
	//��ͬģʽִ�в�ͬ����
	if(opmv2.mode_sta==1 && openmv_switch_flag == 2 )			//�ҵ�
	{
		color(1,1);
		//
		opmv_ct_sta.reset_flag = 0;
		//
		ANO_CBTracking_Ctrl(&dT_ms,opmv_ct_sta.en);
		//�����û��̿غ�����ֵ������
		Program_Ctrl_User_Set_HXYcmps(ano_opmv_cbt_ctrl.exp_velocity_h_cmps[0],ano_opmv_cbt_ctrl.exp_velocity_h_cmps[1]);
	}
	else if(opmv.mode_sta == 2 && openmv_switch_flag == 1)		//Ѳ��
	{
		color(2,2);
//		UARTprintf("LT...");
		//
		opmv_ct_sta.reset_flag = 0;
		//
		ANO_LTracking_Ctrl(&dT_ms,opmv_ct_sta.en);
		//�����û��̿غ�����ֵ������
		Program_Ctrl_User_Set_HXYcmps(ano_opmv_lt_ctrl.exp_velocity_h_cmps[0],ano_opmv_lt_ctrl.exp_velocity_h_cmps[1]);		
		Program_Ctrl_User_Set_YAWdps(ano_opmv_lt_ctrl.exp_yaw_pal_dps);
	}
	else if(opmv.mode_sta==1 && openmv_switch_flag == 1 )			//�ҵ�  �����붨��    ���opmv1��ģʽΪ׷��  
	{
		color(1,1);
		//
		opmv_ct_sta.reset_flag = 0;
		//
		ANO_CBTracking2_Ctrl(&dT_ms,opmv_ct_sta.en);
		//�����û��̿غ�����ֵ������
		Program_Ctrl_User_Set_HXYcmps(ano_opmv_cbt_ctrl.exp_velocity_h_cmps[0],ano_opmv_cbt_ctrl.exp_velocity_h_cmps[1]);
	}
	else
	{
		//reset
		if(opmv_ct_sta.reset_flag==0)
		{
			opmv_ct_sta.reset_flag = 1;
			Program_Ctrl_User_Set_HXYcmps(0,0);		
			Program_Ctrl_User_Set_YAWdps(0);			
		}
	}
	
}

void OpenMvControl_EN( u8 en)
{
	if(en)
		openmv_ct_en=1;
	else
		openmv_ct_en=0;
}



void OpenMvSwitch(u8 num)
{
	openmv_switch_flag = num;
}


