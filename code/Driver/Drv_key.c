#include "Drv_key.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_gpio.h"

#include "pin_map.h"
#include "rom.h"
#include "sysctl.h"
#include "uart.h"
#include "uartstdio.h"

u16 fly_automation_flag;
void Drv_Key1IrqHandler(void)
{
	u8 key_sta;
	//清除中断标记
	GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_2);
	if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4) == 0)
	{
//		MyDelayMs(10);
//		if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4) == 0)
//		{
//			
//		}
	}
}
void Drv_Key2IrqHandler(void)
{
	//清除中断标记
	GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);
	
}

void Key_Init(void)
{
	 SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	//
	// Unlock PF0 so we can change it to a GPIO input
	// Once we have enabled (unlocked) the commit register then re-lock it
	// to prevent further changes.  PF0 is muxed with NMI thus a special case.
	//
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	GPIODirModeSet(GPIO_PORTE_BASE, GPIO_PIN_2,GPIO_DIR_MODE_IN);//SW1
	GPIOPadConfigSet(GPIO_PORTE_BASE,GPIO_PIN_2,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4,GPIO_DIR_MODE_IN);//SW2
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	
//	ROM_GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_2 , GPIO_RISING_EDGE);
//	ROM_GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4 , GPIO_RISING_EDGE);
//	
//	//GPIO注册中断
//	GPIOIntRegister(GPIO_PORTE_BASE, Drv_Key1IrqHandler);
//	GPIOIntRegister(GPIO_PORTF_BASE, Drv_Key2IrqHandler);
//	//使能中断
//	GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_2);
//	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);
//	//设置中断优先级
//	ROM_IntPrioritySet(INT_GPIOE, USER_INT7);
//	ROM_IntPrioritySet(INT_GPIOF, USER_INT7);
  
}


u8 Key_Scan_Task(void)
{
	if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4) == 0)
		return 2;
	else if(GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_2) == 0)
		return 1;
	else
		return 0;
}
extern u8 my_game_flag ;
void Key_task(u8 dT_ms)
{
	static u16 key_sta,key_sta_old,confirm_times;
	key_sta = Key_Scan_Task();
	if(key_sta != key_sta_old)
	{
		if(key_sta == 1)
		{
			if(fly_automation_flag == 0)
			{
				fly_automation_flag =1;
				color(5,5);	//Orange
			//	UARTprintf("fly_automation_flag = 1 \r\n");
			}
			
			if(fly_automation_flag == 5)
			{
				fly_automation_flag = 6;
				my_game_flag = 2;
			//	UARTprintf("fly_automation_flag = 6 \r\n");
			}
		}else if(key_sta == 2)
		{
			if(fly_automation_flag ==1)
			{
				fly_automation_flag= 2;
				my_game_flag = 1;
			//	UARTprintf("fly_automation_flag = 2 \r\n");
			}
			
			if(fly_automation_flag == 0)
			{
				fly_automation_flag = 5;
				color(6,6);		//绿灯
			//	UARTprintf("fly_automation_flag = 5 \r\n");
			}
		}else
		{
		
		}
	}
	key_sta_old = key_sta;
}










