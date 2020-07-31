//默认引用：
#include "Drv_OpenMV.h"
#include "Drv_OpenMV2.h"
#include "Drv_Uart.h"
#include "Ws2812.h"
#include "my_task.h"
#include "uartstdio.h"
//设定
#define OPMV_OFFLINE_TIME_MS  1000  //毫秒

//全局变量
u16 offline_check_time2;
u8 openmv2_buf[20];
_openmv_data_st opmv2;
/**********************************************************************************************************
*函 数 名: OpenMV_Byte_Get
*功能说明: OpenMV字节数据获取
*参    数: 字节数据
*返 回 值: 无
**********************************************************************************************************/
void OpenMV2_Byte_Get(u8 bytedata)
{	
	static u8 len = 0,rec_sta;
	u8 check_val=0;
	
	//
	openmv2_buf[rec_sta] = bytedata;
	//
	if(rec_sta==0)
	{
		if(bytedata==0xaa)
		{
			rec_sta++;
		}
		else
		{
			rec_sta=0;
		}
	}
	else if(rec_sta==1)
	{
		if(1)//(bytedata==0x29)未确定
		{
			rec_sta++;
		}	
		else
		{
			rec_sta=0;
		}		
	}
	else if(rec_sta==2)
	{
		if(bytedata==0x05)
		{
			rec_sta++;
		}
		else
		{
			rec_sta=0;
		}		
	}
	else if(rec_sta==3)
	{
		if(bytedata==0x41 || bytedata==0x42)
		{
			rec_sta++;
		}
		else
		{
			rec_sta=0;
		}		
	}
	else if(rec_sta==4)
	{
		//
		len = bytedata;
		if(len<20)
		{
			rec_sta++;
		}		
		else
		{
			rec_sta=0;
		}
	}
	else if(rec_sta==(len+5))
	{
		//
		for(u8 i=0;i<len+5;i++)
		{
			check_val += openmv2_buf[i];
		}
		//
		if(check_val == bytedata)
		{
			//解析成功
			OpenMV2_Data_Analysis(openmv2_buf,len+6);
			//
			rec_sta=0;
		}
		else
		{
			rec_sta=0;
		}		
	}
	else
	{
		//	
		rec_sta++;
	}
	
}

/**********************************************************************************************************
*函 数 名: OpenMV_Data_Analysis
*功能说明: OpenMV数据解析
*参    数: 缓存数据（形参），长度
*返 回 值: 无
**********************************************************************************************************/

#include "Ano_OPMV_Ctrl.h"
#include "Ano_DT.h"
static void OpenMV2_Data_Analysis(u8 *buf_data,u8 len)
{
	if(*(buf_data+3)==0x41)
	{
		opmv2.cb.color_flag = *(buf_data+5);
		opmv2.cb.sta = *(buf_data+6);
		opmv2.cb.pos_x = (s16)((*(buf_data+7)<<8)|*(buf_data+8));
		opmv2.cb.pos_y = (s16)((*(buf_data+9)<<8)|*(buf_data+10));
		opmv2.cb.dT_ms = *(buf_data+11);
		//
		opmv2.mode_sta = 1;
		//
		f.send_omv_ct = 1;
//		UARTprintf("%d %d\r\n",opmv2.cb.pos_x,opmv2.cb.pos_y);
		if(opmv2.cb.sta == 1)
		{
			if(opmv2.cb.pos_y < 0 && GetStage() == 4)
			{
				OpenMvSwitch(2);
				
			}
			if(opmv2.cb.pos_y < 0 && GetStage() == 8)
			{
				OpenMvSwitch(0);
				color(4,4);
			}
		}
//		if(opmv2.cb.sta)
//			RGB_LED_Runing(1);
//		else
//			Write24Bit(0,0,0,1);
//		color(1,1);
	}
	else if(*(buf_data+3)==0x42)
	{
		
		opmv2.lt.sta = *(buf_data+5);
		opmv2.lt.angle = (s16)((*(buf_data+6)<<8)|*(buf_data+7));
		opmv2.lt.deviation = (s16)((*(buf_data+8)<<8)|*(buf_data+9));
		opmv2.lt.p_flag = *(buf_data+10);
		opmv2.lt.pos_x = (s16)((*(buf_data+11)<<8)|*(buf_data+12));
		opmv2.lt.pos_y = (s16)((*(buf_data+13)<<8)|*(buf_data+14));
		opmv2.lt.dT_ms = *(buf_data+15);
		//
		opmv2.mode_sta = 2;
		//
		f.send_omv_lt = 1;
//		if(opmv.lt.sta)
//			RGB_LED_Runing(1);
//		else
//			Write24Bit(0,0,0,1);
//		color(2,2);
	}
	//
	OpenMV2_Check_Reset();
}

/**********************************************************************************************************
*函 数 名: OpenMV_Offline_Check
*功能说明: OpenMV掉线检测，用来检测硬件是否在线
*参    数: 时间（毫秒）
*返 回 值: 无
**********************************************************************************************************/
void OpenMV2_Offline_Check(u8 dT_ms)
{
	if(offline_check_time2<OPMV_OFFLINE_TIME_MS)
	{
		offline_check_time2 += dT_ms;
	}
	else
	{
		opmv2.offline = 1;
		opmv2.mode_sta = 0;
	}
	
}

/**********************************************************************************************************
*函 数 名: OpenMV_Check_Reset
*功能说明: OpenMV掉线检测复位，证明没有掉线
*参    数: 无
*返 回 值: 无
**********************************************************************************************************/
static void OpenMV2_Check_Reset()
{
	offline_check_time2 = 0;
	opmv2.offline = 0;
}



void ANO_OpenMV2_Send_Data(u8 *dataToSend , u8 length)
{

	Drv_Uart6SendBuf(dataToSend, length);

}

static u8 data_to_send[10];	//发送数据缓存

void ANO_DT_SendCmd_OpenMv2(u8 sta)
{
	u8 _cnt=0;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAF;
	data_to_send[_cnt++]=0x05;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0x06;
	
	data_to_send[_cnt++]=sta;

	
	ANO_OpenMV2_Send_Data(data_to_send, _cnt);
}
