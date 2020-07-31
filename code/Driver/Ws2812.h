#ifndef __Ws2812_H
#define __Ws2812_H
#include "sysconfig.h"

#define RGB_SYSCTL			SYSCTL_PERIPH_GPIOC
#define IMU_RGB_SYSCTL			SYSCTL_PERIPH_GPIOD
#define RGB		  			GPIOC_BASE,GPIO_PIN_4 
#define IMU_RGB		  			GPIOD_BASE,GPIO_PIN_3 

#define RGB_H     				ROM_GPIOPinWrite(RGB, GPIO_PIN_4)//配置对应RGB引脚为 高电平
#define RGB_L      				ROM_GPIOPinWrite(RGB,0)//配置对应RGB引脚为 低电平
#define IMU_RGB_H     		ROM_GPIOPinWrite(IMU_RGB, GPIO_PIN_3)//配置对应RGB引脚为 高电平
#define IMU_RGB_L      		ROM_GPIOPinWrite(IMU_RGB,0)//配置对应RGB引脚为 低电平

void delay_us(int nus);
void Write0(void);
void Write1(void);
void RGB_WByte(uint8_t byte);
void Write24Bit(uint8_t green, uint8_t red, uint8_t blue,int imu);
void RGB_LED_Runing(int imu);
void RGB_LED_WriteColor(uint8_t green, uint8_t red, uint8_t blue);
void WriteRes(void);
void RGB_LED_Off(void);
void Ws2812Init(void);
void color(int rgb,u8 switch_flag);
#define Red		  			0,0xff,0                
#define Orange		  	0xa5,0xff,0x00     
#define Yellow		  	0xff,0xff,0   
#define Green		  		0xff,0,0   
#define Cyan		  		0xff,0,0xff   //青色
#define Blue		  		0,0,0xff   
#define Violet		  	0x00,0xcd,0xcd  //紫色 
#define White		  		0xff,0xff,0xff    

#endif

