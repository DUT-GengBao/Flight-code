#include "Drv_key.h"
#include "Drv_beep.h"
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


u8 beep_flag;

void beep_init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	
	ROM_GPIOPinTypeGPIOOutput(GPIOB_BASE, GPIO_PIN_2);
	
	beep_switch(1);
}

void beep_switch(u8 en)
{
	if(en)
	{
		beep_flag=1;
		ROM_GPIOPinWrite(GPIOB_BASE, GPIO_PIN_2, 0);
	}
	else
	{
		beep_flag=0;
		ROM_GPIOPinWrite(GPIOB_BASE, GPIO_PIN_2, GPIO_PIN_2);
	}
}

void beep_task(u8 dT_ms)
{
	static u16 beep_time;
	if(beep_flag == 1)
	{
		beep_time+=dT_ms;
	}else
	{
		beep_time = 0;
	}
	
	if(beep_time > 3000)
	{
		beep_switch(0);
	}
}






