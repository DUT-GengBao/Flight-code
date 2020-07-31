#include "Drv_Bsp.h"
#include "pwm.h"
#include "Drv_RcIn.h"
#include "Drv_Spi.h"
#include "Drv_Led.h"
#include "Drv_Paramter.h"
#include "Drv_icm20602.h"
#include "drv_ak8975.h"
#include "drv_spl06.h"
#include "Drv_PwmOut.h"
#include "Drv_Adc.h"
#include "Drv_Uart.h"
#include "Ano_FcData.h"
#include "Ano_Sensor_Basic.h"
#include "Ano_RC.h"
#include "Ano_FlightCtrl.h"
#include "Drv_gps.h"
#include "ano_usb.h"
#include "Drv_UP_Flow.h"
#include "Drv_laser.h"
#include "Ws2812.h"
#include "Drv_key.h"
#include "Drv_beep.h"
static uint64_t SysRunTimeMs = 0;

void SysTick_Init(void )
{
	ROM_SysTickPeriodSet(ROM_SysCtlClockGet()/1000);
	ROM_SysTickIntEnable();
	ROM_SysTickEnable();
}
void SysTick_Handler(void)
{
	SysRunTimeMs++;
}
uint32_t GetSysRunTimeMs(void)
{
	return SysRunTimeMs;
}
uint32_t GetSysRunTimeUs(void)
{
	return SysRunTimeMs*1000 + (SysTick->LOAD - SysTick->VAL) * 1000 / SysTick->LOAD;
}

void MyDelayMs(u32 time)
{
	ROM_SysCtlDelay(80000 * time /3);
}

void Drv_SenserCsPinInit(void)
{
	Drv_Icm20602CSPinInit();
	Drv_AK8975CSPinInit();
	Drv_SPL06CSPinInit();
	
//	ROM_SysCtlPeripheralEnable(FLASH_CSPIN_SYSCTL);
//	ROM_GPIOPinTypeGPIOOutput(FLASH_CS_PORT,FLASH_CS_PIN);
//	ROM_GPIOPinWrite(FLASH_CS_PORT, FLASH_CS_PIN,FLASH_CS_PIN);
}

u8 of_init_type;
void Drv_BspInit(void)
{
	/*����ϵͳ��ƵΪ80M*/
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |SYSCTL_OSC_MAIN);
	/*�ж����ȼ��������*/
	NVIC_SetPriorityGrouping(0x03);
	/*�����������㵥Ԫ*/	
	ROM_FPULazyStackingEnable();
	ROM_FPUEnable();
	
	//����USB���⴮�ڳ�ʼ��		//���ø�
	AnoUsbCdcInit();	
	//�ȳ�ʼ���������ڣ��Ա�ͨ��
	Drv_Uart2Init(115200);	//������ �� //ʵ��Ϊ����1
	//���ݳ�ʼ��
	Dvr_ParamterInit();//���ø�
	//��ȡ��ʼ����
	Para_Data_Init();//���ø�
	//�ƹ��ʼ��
	Dvr_LedInit();		//��
	
	//ң�ؽ���ģʽ��ʼ��
	Remote_Control_Init();		//��

	//spiͨ�ų�ʼ��
	Drv_Spi0Init();		//���ø�
	Drv_SenserCsPinInit();//��
	//��ʼ��ICM
	sens_hd_check.acc_ok = sens_hd_check.gyro_ok =
	Drv_Icm20602Init();//���˸��ж����ţ���·����û�У��ж�������������1ms���жϣ������ö�ʱ���ж�ȡ����
	//��ʼ����ѹ��
	sens_hd_check.baro_ok = Drv_Spl0601Init();
	//�������OK���������̲�������㣨ע���˴�û���������Ƿ������ļ�����
	sens_hd_check.mag_ok = 1;       //	
	
	//ADC��ʼ��
	Drv_AdcInit();	//���ø�
	//�δ�ʱ�ӳ�ʼ��
	SysTick_Init();		//���ø�
	//���ڳ�ʼ��
	//
	Drv_Uart3Init(500000);   //��OPMV	�� //ʵ��Ϊ����2

//	Drv_GpsPin_Init();//
	//	
	Drv_Uart4Init(19200);	//���������		//ʵ��Ϊ����7
//	Drv_Uart5Init(115200);//�Ӵ��ʼ���		//ʵ��Ϊ����2
	MyDelayMs(200);	
	//���������ʼ��
	of_init_type = (Drv_OFInit()==0)?0:2;	//�������������������0  ��������󣬵���2
	if(of_init_type==2)//���������ʼ���ɹ�
	{
		//���ʼ����ʼ��
		Drv_Laser_Init();	
	}
	else if(of_init_type==0)//���������ʼ��ʧ��
	{
		Drv_Uart4Init(500000);	//����������
	}
	//====fc
	//�ɿش����������ʼ��
	Sensor_Basic_Init();	//���ø�
	//�ɿ�PID��ʼ��
	All_PID_Init();//���ø�
	//��������ʼ��
	Drv_PwmOutInit();//��
	Ws2812Init();//Ws2812��ʼ��
	Drv_Uart6Init(500000);   //��OPMV2	�� //ʵ��Ϊ����3
	Key_Init();
	beep_init();
}




