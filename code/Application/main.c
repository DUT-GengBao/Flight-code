#include "sysconfig.h"
#include "Drv_Bsp.h"
#include "Ano_Scheduler.h"
#include "Ano_FcData.h"
#include "timer.h"
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
//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//不能打印浮点数
//*****************************************************************************
void
ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}


void Timer1IntHandler(void)
{
	INT_1ms_Task();
	ROM_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
}

void Timer1Init(uint32_t load)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	
	// Configure the two 32-bit periodic timers.
	ROM_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
	ROM_TimerLoadSet(TIMER1_BASE, TIMER_A, load);
	
	TimerIntRegister(TIMER1_BASE,TIMER_A, Timer1IntHandler);
	ROM_IntEnable(INT_TIMER1A);
	ROM_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

	ROM_TimerEnable(TIMER1_BASE, TIMER_A);

	ROM_IntMasterEnable();
}


int main(void)
{
	ConfigureUART();
	UARTprintf("uart0 init successful...\r\n");
	Drv_BspInit();
	//
	Timer1Init(80000);			//1MS
	flag.start_ok = 1;
	
	while(1)
	{
		//轮询任务调度器
		Main_Task();
	}
}
