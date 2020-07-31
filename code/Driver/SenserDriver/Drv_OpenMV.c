//Ĭ�����ã�
#include "Drv_OpenMV.h"
#include "Drv_Uart.h"
#include "Ws2812.h"
//�趨
#define OPMV_OFFLINE_TIME_MS  1000  //����

//ȫ�ֱ���
u16 offline_check_time;
u8 openmv_buf[20];
_openmv_data_st opmv;
/**********************************************************************************************************
*�� �� ��: OpenMV_Byte_Get
*����˵��: OpenMV�ֽ����ݻ�ȡ
*��    ��: �ֽ�����
*�� �� ֵ: ��
**********************************************************************************************************/
void OpenMV_Byte_Get(u8 bytedata)
{	
	static u8 len = 0,rec_sta;
	u8 check_val=0;
	
	//
	openmv_buf[rec_sta] = bytedata;
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
		if(1)//(bytedata==0x29)δȷ��
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
			check_val += openmv_buf[i];
		}
		//
		if(check_val == bytedata)
		{
			//�����ɹ�
			OpenMV_Data_Analysis(openmv_buf,len+6);
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
*�� �� ��: OpenMV_Data_Analysis
*����˵��: OpenMV���ݽ���
*��    ��: �������ݣ��βΣ�������
*�� �� ֵ: ��
**********************************************************************************************************/

#include "Drv_beep.h"
#include "Ano_DT.h"
static void OpenMV_Data_Analysis(u8 *buf_data,u8 len)
{
	if(*(buf_data+3)==0x41)
	{
		opmv.cb.color_flag = *(buf_data+5);
		opmv.cb.sta = *(buf_data+6);
		opmv.cb.pos_x = (s16)((*(buf_data+7)<<8)|*(buf_data+8));
		opmv.cb.pos_y = (s16)((*(buf_data+9)<<8)|*(buf_data+10));
		opmv.cb.dT_ms = *(buf_data+11);
		//
		opmv.mode_sta = 1;
		//
		f.send_omv_ct = 1;
		if(opmv.cb.sta)
			RGB_LED_Runing(1);
		else
			Write24Bit(0,0,0,1);
//		color(1,1);
	}
	else if(*(buf_data+3)==0x42 )
	{
		
		opmv.lt.sta = *(buf_data+5);
		opmv.lt.angle = (s16)((*(buf_data+6)<<8)|*(buf_data+7));
		opmv.lt.deviation = (s16)((*(buf_data+8)<<8)|*(buf_data+9));
		opmv.lt.p_flag = *(buf_data+10);
		opmv.lt.pos_x = (s16)((*(buf_data+11)<<8)|*(buf_data+12));
		opmv.lt.pos_y = (s16)((*(buf_data+13)<<8)|*(buf_data+14));
		opmv.lt.dT_ms = *(buf_data+15);
		//
		opmv.mode_sta = 2;
		//
		f.send_omv_lt = 1;
		if(opmv.lt.sta)
			RGB_LED_Runing(1);
		else
			Write24Bit(0,0,0,1);
		
		
		if(opmv.lt.sta == 4)
		{
			beep_switch(1);
		}
			
		
	}
	//
	OpenMV_Check_Reset();
}

/**********************************************************************************************************
*�� �� ��: OpenMV_Offline_Check
*����˵��: OpenMV���߼�⣬�������Ӳ���Ƿ�����
*��    ��: ʱ�䣨���룩
*�� �� ֵ: ��
**********************************************************************************************************/
void OpenMV_Offline_Check(u8 dT_ms)
{
	if(offline_check_time<OPMV_OFFLINE_TIME_MS)
	{
		offline_check_time += dT_ms;
	}
	else
	{
		opmv.offline = 1;
		opmv.mode_sta = 0;
	}
	
}

/**********************************************************************************************************
*�� �� ��: OpenMV_Check_Reset
*����˵��: OpenMV���߼�⸴λ��֤��û�е���
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
static void OpenMV_Check_Reset()
{
	offline_check_time = 0;
	opmv.offline = 0;
}



void ANO_OpenMV_Send_Data(u8 *dataToSend , u8 length)
{

	Drv_Uart3SendBuf(dataToSend, length);

}
#include "uartstdio.h"
static u8 data_to_send[10];	//�������ݻ���

void ANO_DT_SendCmd_OpenMv(u8 sta)
{
	u8 _cnt=0;	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAF;
	data_to_send[_cnt++]=0x05;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0x06;
	
	data_to_send[_cnt++]=sta;

	
	ANO_OpenMV_Send_Data(data_to_send, _cnt);
}

u8 OpenMv_working_mode_set(u8 mode)
{
	
	if(mode == 0x31)
	{
		if(opmv.mode_sta != 0x41)
		{
			ANO_DT_SendCmd_OpenMv(mode);		//�ҵ�ģʽ	
			UARTprintf("send cmd 0x31\r\n");
		}
	}else if(mode == 0x32)
	{
		if(opmv.mode_sta != 0x42)
		{
			ANO_DT_SendCmd_OpenMv(mode);		//Ѳ��ģʽ	
			UARTprintf("send cmd 0x32\r\n");
		}
	}
	return opmv.mode_sta;
}