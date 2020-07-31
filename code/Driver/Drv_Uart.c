#include "Drv_Uart.h"
#include "uart.h"
#include "hw_ints.h"
#include "hw_types.h"
#include "hw_gpio.h"
#include "Ano_DT.h"
#include "Drv_OpenMV.h"
#include "Drv_laser.h"

#include "sysctl.h"
#include "hw_memmap.h"
//TM4C的串口0对应底板串口1
//TM4C的串口2对应底板串口5
//TM4C的串口4对应底板串口2
//TM4C的串口5对应底板串口3
//TM4C的串口7对应底板串口4
//这里都以底板的串口标号为准，比如Drv_Uart1Init，为底板串口1的初始化，即TM4C的串口0
/////////////////////////////////////////////////////////////////////////////////////////////////
#include "Drv_gps.h"
u8 U1TxDataTemp[256];
u8 U1TxInCnt = 0;
u8 U1TxOutCnt = 0;
void UART1_IRQHandler(void)
{
	uint8_t com_data;
	/*获取中断标志 原始中断状态 不屏蔽中断标志*/		
	uint32_t flag = ROM_UARTIntStatus(UART0_BASE,1);
	/*清除中断标志*/	
	ROM_UARTIntClear(UART0_BASE,flag);		
	/*判断FIFO是否还有数据*/		
	while(ROM_UARTCharsAvail(UART0_BASE))		
	{			
		com_data=ROM_UARTCharGet(UART0_BASE);
		Drv_GpsGetOneByte(com_data);
	}
	if(flag & UART_INT_TX)
	{
		Drv_Uart1TxCheck();
	}
}
void Drv_Uart1Init(uint32_t baudrate)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	
	/*GPIO的UART模式配置*/
	ROM_GPIOPinConfigure(UART0_RX);
	ROM_GPIOPinConfigure(UART0_TX);
	ROM_GPIOPinTypeUART(UART0_PORT, UART0_PIN_TX | UART0_PIN_RX);
	/*配置串口号波特率和时钟源*/		
	ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), baudrate,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	/*FIFO设置*/
	ROM_UARTFIFOLevelSet(UART0_BASE,UART_FIFO_TX7_8,UART_FIFO_RX7_8);
	ROM_UARTFIFOEnable(UART0_BASE);
	/*使能串口*/
	ROM_UARTEnable( UART0_BASE );
	/*使能UART0接收中断*/			
	UARTIntRegister(UART0_BASE,UART1_IRQHandler);			
	ROM_IntPrioritySet(INT_UART0, USER_INT2);
	ROM_UARTTxIntModeSet(UART0_BASE,UART_TXINT_MODE_EOT);
	ROM_UARTIntEnable(UART0_BASE,UART_INT_RX | UART_INT_RT | UART_INT_TX);
}
void Drv_Uart1SendBuf(u8 *data, u8 len)
{
	for(u8 i=0; i<len; i++)
	{
		U1TxDataTemp[U1TxInCnt++] = * ( data + i );
	}
	Drv_Uart1TxCheck();
}
void Drv_Uart1TxCheck(void)
{
	while( (U1TxOutCnt != U1TxInCnt) && (ROM_UARTCharPutNonBlocking(UART0_BASE,U1TxDataTemp[U1TxOutCnt])) )
		U1TxOutCnt++;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
u8 U2TxDataTemp[256];
u8 U2TxInCnt = 0;
u8 U2TxOutCnt = 0;
void UART2_IRQHandler(void)
{
	uint8_t com_data;
	/*获取中断标志 原始中断状态 不屏蔽中断标志*/		
	uint32_t flag = ROM_UARTIntStatus(UART1_BASE,1);
	/*清除中断标志*/	
	ROM_UARTIntClear(UART1_BASE,flag);		
	/*判断FIFO是否还有数据*/		
	while(ROM_UARTCharsAvail(UART1_BASE))		
	{			
		com_data=ROM_UARTCharGet(UART1_BASE);
		ANO_DT_Data_Receive_Prepare(com_data);
	}
	if(flag & UART_INT_TX)
	{
		Drv_Uart2TxCheck();
	}
}
void Drv_Uart2Init(uint32_t baudrate)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	
	/*GPIO的UART模式配置*/
	ROM_GPIOPinConfigure(GPIO_PB0_U1RX);
	ROM_GPIOPinConfigure(GPIO_PB1_U1TX);
	ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	/*配置串口号波特率和时钟源*/		
	ROM_UARTConfigSetExpClk(UART1_BASE, ROM_SysCtlClockGet(), baudrate,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	/*FIFO设置*/
	ROM_UARTFIFOLevelSet(UART1_BASE,UART_FIFO_TX7_8,UART_FIFO_RX7_8);
	ROM_UARTFIFOEnable(UART1_BASE);
	/*使能串口*/
	ROM_UARTEnable( UART1_BASE );
	/*使能UART0接收中断*/			
	UARTIntRegister(UART1_BASE,UART2_IRQHandler);			
	ROM_IntPrioritySet(INT_UART1, USER_INT2);
	ROM_UARTTxIntModeSet(UART1_BASE,UART_TXINT_MODE_EOT);
	ROM_UARTIntEnable(UART1_BASE,UART_INT_RX | UART_INT_RT | UART_INT_TX);
}
void Drv_Uart2SendBuf(u8 *data, u8 len)
{
	for(u8 i=0; i<len; i++)
	{
		U2TxDataTemp[U2TxInCnt++] = * ( data + i );
	}
	Drv_Uart2TxCheck();
}
void Drv_Uart2TxCheck(void)
{
	while( (U2TxOutCnt != U2TxInCnt) && (ROM_UARTCharPutNonBlocking(UART1_BASE,U2TxDataTemp[U2TxOutCnt])) )
		U2TxOutCnt++;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
u8 U3TxDataTemp[256];
u8 U3TxInCnt = 0;
u8 U3TxOutCnt = 0;
void UART3_IRQHandler(void)
{
	uint8_t com_data;
	/*获取中断标志 原始中断状态 不屏蔽中断标志*/		
	uint32_t flag = ROM_UARTIntStatus(UART2_BASE,1);
	/*清除中断标志*/	
	ROM_UARTIntClear(UART2_BASE,flag);	
//	UARTprintf("enter1\r");
//	UARTprintf("enter u2\r\n");	
	/*判断FIFO是否还有数据*/		
	while(ROM_UARTCharsAvail(UART2_BASE))		
	{			
		com_data=ROM_UARTCharGet(UART2_BASE);
		OpenMV_Byte_Get(com_data);
		
//		UARTprintf("openmv dat\r\n");
	}
	if(flag & UART_INT_TX)
	{
		Drv_Uart3TxCheck();
	}
}
void Drv_Uart3Init(uint32_t baudrate)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	
	/*PD7解锁操作*/
	HWREG(UART2_PORT + GPIO_O_LOCK) = GPIO_LOCK_KEY; 
	HWREG(UART2_PORT + GPIO_O_CR) = UART2_PIN_TX;
	HWREG(UART2_PORT + GPIO_O_LOCK) = 0x00;
	
	/*GPIO的UART模式配置*/
	ROM_GPIOPinConfigure(UART2_RX);
	ROM_GPIOPinConfigure(UART2_TX);
	ROM_GPIOPinTypeUART(UART2_PORT, UART2_PIN_TX | UART2_PIN_RX);
	/*配置串口号波特率和时钟源*/		
	ROM_UARTConfigSetExpClk(UART2_BASE, ROM_SysCtlClockGet(), baudrate,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	/*FIFO设置*/
	ROM_UARTFIFOLevelSet(UART2_BASE,UART_FIFO_TX7_8,UART_FIFO_RX7_8);
	ROM_UARTFIFOEnable(UART2_BASE);
	/*使能串口*/
	ROM_UARTEnable( UART2_BASE );
	/*使能UART0接收中断*/			
	UARTIntRegister(UART2_BASE,UART3_IRQHandler);			
	ROM_IntPrioritySet(INT_UART2, USER_INT2);
	ROM_UARTTxIntModeSet(UART2_BASE,UART_TXINT_MODE_EOT);
	ROM_UARTIntEnable(UART2_BASE,UART_INT_RX | UART_INT_RT | UART_INT_TX);
}
void Drv_Uart3SendBuf(u8 *data, u8 len)
{
	for(u8 i=0; i<len; i++)
	{
		U3TxDataTemp[U3TxInCnt++] = * ( data + i );
	}
	Drv_Uart3TxCheck();
}
void Drv_Uart3TxCheck(void)
{
	while( (U3TxOutCnt != U3TxInCnt) && (ROM_UARTCharPutNonBlocking(UART2_BASE,U3TxDataTemp[U3TxOutCnt])) )
		U3TxOutCnt++;
}
//入口参数：								字符串首位指针    				 字节数
void Usart3_Send(const uint8_t *pui8Buffer, uint32_t ui32Count)
{

    while(ui32Count--)
    {

        UARTCharPut(UART2_BASE, *pui8Buffer++);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////
#include "Ano_OF.h"
#include "Drv_UP_Flow.h"
u8 U4TxDataTemp[256];
u8 U4TxInCnt = 0;
u8 U4TxOutCnt = 0;
void UART4_IRQHandler(void)
{
	uint8_t com_data;
	/*获取中断标志 原始中断状态 不屏蔽中断标志*/		
	uint32_t flag = ROM_UARTIntStatus(UART7_BASE,1);
	/*清除中断标志*/	
	ROM_UARTIntClear(UART7_BASE,flag);		
	/*判断FIFO是否还有数据*/		
	while(ROM_UARTCharsAvail(UART7_BASE))		
	{			
		com_data=ROM_UARTCharGet(UART7_BASE);
		//匿名光流解析
		if(of_init_type!=2)
		{
			AnoOF_GetOneByte(com_data);
//			UARTprintf("aof dat\r\n");
		}
		//优像光流解析
		if(of_init_type!=1)
		{
			OFGetByte(com_data);
		}
	}
	if(flag & UART_INT_TX)
	{
		Drv_Uart4TxCheck();
	}
}
void Drv_Uart4Init(uint32_t baudrate)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	
	/*GPIO的UART模式配置*/
	ROM_GPIOPinConfigure(UART7_RX);
	ROM_GPIOPinConfigure(UART7_TX);
	ROM_GPIOPinTypeUART(UART7_PORT, UART7_PIN_TX | UART7_PIN_RX);
	/*配置串口号波特率和时钟源*/		
	ROM_UARTConfigSetExpClk(UART7_BASE, ROM_SysCtlClockGet(), baudrate,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	/*FIFO设置*/
	ROM_UARTFIFOLevelSet(UART7_BASE,UART_FIFO_TX7_8,UART_FIFO_RX7_8);
	ROM_UARTFIFOEnable(UART7_BASE);
	/*使能串口*/
	ROM_UARTEnable( UART7_BASE );
	/*使能UART0接收中断*/			
	UARTIntRegister(UART7_BASE,UART4_IRQHandler);			
	ROM_IntPrioritySet(INT_UART7, USER_INT2);
	ROM_UARTTxIntModeSet(UART7_BASE,UART_TXINT_MODE_EOT);
	ROM_UARTIntEnable(UART7_BASE,UART_INT_RX | UART_INT_RT | UART_INT_TX);
}
void Drv_Uart4SendBuf(u8 *data, u8 len)
{
	for(u8 i=0; i<len; i++)
	{
		U4TxDataTemp[U4TxInCnt++] = * ( data + i );
	}
	Drv_Uart4TxCheck();
}
void Drv_Uart4TxCheck(void)
{
	while( (U4TxOutCnt != U4TxInCnt) && (ROM_UARTCharPutNonBlocking(UART7_BASE,U4TxDataTemp[U4TxOutCnt])) )
		U4TxOutCnt++;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
u8 U5TxDataTemp[256];
u8 U5TxInCnt = 0;
u8 U5TxOutCnt = 0;
void UART5_IRQHandler(void)
{
	uint8_t com_data;
	/*获取中断标志 原始中断状态 不屏蔽中断标志*/		
	uint32_t flag = ROM_UARTIntStatus(UART2_BASE,1);
	/*清除中断标志*/	
	ROM_UARTIntClear(UART2_BASE,flag);		
	/*判断FIFO是否还有数据*/		
	while(ROM_UARTCharsAvail(UART2_BASE))		
	{			
		com_data=ROM_UARTCharGet(UART2_BASE);
		//ANO_DT_Data_Receive_Prepare(com_data);
		Drv_Laser_GetOneByte(com_data);
	}
	if(flag & UART_INT_TX)
	{
		Drv_Uart5TxCheck();
	}
}
void Drv_Uart5Init(uint32_t baudrate)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	
	/*PD7解锁操作*/
	HWREG(UART2_PORT + GPIO_O_LOCK) = GPIO_LOCK_KEY; 
	HWREG(UART2_PORT + GPIO_O_CR) = UART2_PIN_TX;
	HWREG(UART2_PORT + GPIO_O_LOCK) = 0x00;
	/*GPIO的UART模式配置*/
	ROM_GPIOPinConfigure(UART2_RX);
	ROM_GPIOPinConfigure(UART2_TX);
	ROM_GPIOPinTypeUART(UART2_PORT, UART2_PIN_TX | UART2_PIN_RX);
	/*配置串口号波特率和时钟源*/		
	ROM_UARTConfigSetExpClk(UART2_BASE, ROM_SysCtlClockGet(), baudrate,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	/*FIFO设置*/
	ROM_UARTFIFOLevelSet(UART2_BASE,UART_FIFO_TX7_8,UART_FIFO_RX7_8);
	ROM_UARTFIFOEnable(UART2_BASE);
	/*使能串口*/
	ROM_UARTEnable( UART2_BASE );
	/*使能UART0接收中断*/			
	UARTIntRegister(UART2_BASE,UART5_IRQHandler);			
	ROM_IntPrioritySet(INT_UART2, USER_INT2);
	ROM_UARTTxIntModeSet(UART2_BASE,UART_TXINT_MODE_EOT);
	ROM_UARTIntEnable(UART2_BASE,UART_INT_RX | UART_INT_RT | UART_INT_TX);
}
void Drv_Uart5SendBuf(u8 *data, u8 len)
{
	for(u8 i=0; i<len; i++)
	{
		U5TxDataTemp[U5TxInCnt++] = * ( data + i );
	}
	Drv_Uart5TxCheck();
}
void Drv_Uart5TxCheck(void)
{
	while( (U5TxOutCnt != U5TxInCnt) && (ROM_UARTCharPutNonBlocking(UART2_BASE,U5TxDataTemp[U5TxOutCnt])) )
		U5TxOutCnt++;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
u8 U6TxDataTemp[256];
u8 U6TxInCnt = 0;
u8 U6TxOutCnt = 0;
void UART6_IRQHandler(void)
{
	uint8_t com_data;
	/*获取中断标志 原始中断状态 不屏蔽中断标志*/		
	uint32_t flag = ROM_UARTIntStatus(UART3_BASE,1);
	/*清除中断标志*/	
	ROM_UARTIntClear(UART3_BASE,flag);		
//	UARTprintf("enter2\r");
	/*判断FIFO是否还有数据*/		
	while(ROM_UARTCharsAvail(UART3_BASE))		
	{			
		com_data=ROM_UARTCharGet(UART3_BASE);
		//ANO_DT_Data_Receive_Prepare(com_data);
		OpenMV2_Byte_Get(com_data);
		
	}
	if(flag & UART_INT_TX)
	{
		Drv_Uart6TxCheck();
	}
}
void Drv_Uart6Init(uint32_t baudrate)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	

	/*GPIO的UART模式配置*/
	ROM_GPIOPinConfigure(UART3_RX);
	ROM_GPIOPinConfigure(UART3_TX);
	ROM_GPIOPinTypeUART(UART3_PORT, UART3_PIN_TX | UART3_PIN_RX);
	/*配置串口号波特率和时钟源*/		
	ROM_UARTConfigSetExpClk(UART3_BASE, ROM_SysCtlClockGet(), baudrate,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	/*FIFO设置*/
	ROM_UARTFIFOLevelSet(UART3_BASE,UART_FIFO_TX7_8,UART_FIFO_RX7_8);
	ROM_UARTFIFOEnable(UART3_BASE);
	/*使能串口*/
	ROM_UARTEnable( UART3_BASE );
	/*使能UART0接收中断*/			
	UARTIntRegister(UART3_BASE,UART6_IRQHandler);			
	ROM_IntPrioritySet(INT_UART3, USER_INT3);
	ROM_UARTTxIntModeSet(UART3_BASE,UART_TXINT_MODE_EOT);
	ROM_UARTIntEnable(UART3_BASE,UART_INT_RX | UART_INT_RT | UART_INT_TX);
}
void Drv_Uart6SendBuf(u8 *data, u8 len)
{
	for(u8 i=0; i<len; i++)
	{
		U6TxDataTemp[U6TxInCnt++] = * ( data + i );
	}
	Drv_Uart6TxCheck();
}
void Drv_Uart6TxCheck(void)
{
	while( (U6TxOutCnt != U6TxInCnt) && (ROM_UARTCharPutNonBlocking(UART3_BASE,U6TxDataTemp[U6TxOutCnt])) )
		U6TxOutCnt++;
}