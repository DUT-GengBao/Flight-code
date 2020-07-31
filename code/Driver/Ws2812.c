#include "Ws2812.h"
#include "Ano_FcData.h"
void delay_us(int nus)
{
ROM_SysCtlDelay(120/3*nus);
}
static u32 Run_buf[][4] = 
{
{0xFFA500,0xFFFF00,0xFFFFFF,0xFFA500,},//��ɫ
{0xFFA500,0x00FF00,0xFFFFFF,0xFFA500,},//��ɫ
{0xFFA500,0x00FF00,0xFF00FF,0xFFA500,},//��ɫ
{0xFFA500,0x00FF00,0xFF00FF,0x00FFFF,},//��ɫ
{0x0000FF,0x00FF00,0xFF00FF,0x00FFFF,},//��ɫ
{0x0000FF,0xFFFF00,0xFF00FF,0x00FFFF,},//��ɫ
{0x0000FF,0xFFFF00,0xFFFFFF,0x00FFFF,},//��ɫ
{0x0000FF,0xFFFF00,0xFFFFFF,0xFFA500,},
};

/*
дws2812b��0��
*/
void Write0(void)
{
RGB_H;
__nop();__nop();__nop();__nop();__nop();__nop();
RGB_L;
__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
__nop();__nop();
}
/*
дws2812b��1��
*/
void Write1(void)
{
RGB_H;
__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
__nop();__nop();

RGB_L;
__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
__nop();__nop();
}
/*
дIMU_ws2812b��1��
*/
void IMU_Write1(void)
{
IMU_RGB_H;
__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
__nop();__nop();

IMU_RGB_L;
__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
__nop();__nop();
}
/*
дIMU_ws2812b��0��
*/
void IMU_Write0(void)
{
IMU_RGB_H;
__nop();__nop();__nop();__nop();__nop();__nop();
IMU_RGB_L;
__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
__nop();__nop();
}
/*
imu_ws2812b��reset��
*/
void IMU_WriteRes(void)
{
IMU_RGB_L;
delay_us(300);
}
/*
дws2812b��reset��
*/
void WriteRes(void)
{
RGB_L;
delay_us(300);
}
/*
дws2812b��1��byte
*/
void RGB_WByte(uint8_t byte)
{
uint8_t i=0;
for(i=0;i<8;i++)
{
if((byte<<i)&0xfe)
Write1();
else
Write0();
}
}
/*
дws2812b��1��byte
*/
void IMU_RGB_WByte(uint8_t byte)
{
uint8_t i=0;
for(i=0;i<8;i++)
{
if((byte<<i)&0xfe)
IMU_Write1();
else
IMU_Write0();
}
}

/*
дws2812b��24bit���ݣ�����һ����ɫ
*/
void Write24Bit(uint8_t green, uint8_t red, uint8_t blue,int imu)
{
if(imu)
{
IMU_RGB_WByte(green);
IMU_RGB_WByte(red);
IMU_RGB_WByte(blue);
}
else
{
RGB_WByte(green);
RGB_WByte(red);
RGB_WByte(blue);

}
}
/*
ws2812b��ˮ��
*/
void RGB_LED_Runing(int imu)
{
	uint8_t i,red=0,green=0,blue=0;
	static uint8_t cnt = 0,wcnt = 0,RgbOff=0,times=0;
	if(imu)//flag.unlock_cmd ==1&&
	{
		if(times==10)
		{
		Write24Bit(Violet,1);//�ϳ���ɫ
		}
		if(times==20)
		{
			times=0;
		Write24Bit(0,0,0,1);//�ϳ���ɫ
		}
			times++;	
		}

	else
	{
	if(flag.unlock_cmd ==0)
	{RgbOff=0;
	for(i=0;i<4;i++)
	{
	if(cnt>3) cnt = 0;
	red   = ((Run_buf[wcnt][cnt]>>16)&0xff);
	green = ((Run_buf[wcnt][cnt]>>8)&0xff); 
	blue  = ((Run_buf[wcnt][cnt]>>0)&0xff);
	Write24Bit(green,red,blue,0);//�ϳ���ɫ
	cnt++;
	}
	wcnt++;
	if(wcnt==7) wcnt = 0;  
	}
	else if(flag.unlock_cmd ==1&&RgbOff==0)
	{
	for(i=0;i<4;i++)
	{
	Write24Bit(0,0,0,0);//�ϳ���ɫ
	}
	RgbOff=1;
	}
	}
}
void color(int rgb,u8 switch_flag)
{
	static u8 switchflag;
	
	
	if(switchflag != switch_flag)
	{
		if(rgb==1)//flag.unlock_cmd ==1&&
		{

			Write24Bit(Red,0);//�ϳ���ɫ
					Write24Bit(Red,0);//�ϳ���ɫ
					Write24Bit(Red,0);//�ϳ���ɫ
					Write24Bit(Red,0);//�ϳ���ɫ
			}
		else if(rgb==2)
		{
		
			
			Write24Bit(Blue,0);//�ϳ���ɫ
				Write24Bit(Blue,0);//�ϳ���ɫ
			Write24Bit(Blue,0);//�ϳ���ɫ
				Write24Bit(Blue,0);//�ϳ���ɫ
		
		}
		else if(rgb==3)
		{
			Write24Bit(Yellow,0);//�ϳ���ɫ
			Write24Bit(Yellow,0);//�ϳ���ɫ
			Write24Bit(Yellow,0);//�ϳ���ɫ
			Write24Bit(Yellow,0);//�ϳ���ɫ
		}else if(rgb==4)
		{
			Write24Bit(Violet,0);//�ϳ���ɫ
			Write24Bit(Violet,0);//�ϳ���ɫ
			Write24Bit(Violet,0);//�ϳ���ɫ
			Write24Bit(Violet,0);//�ϳ���ɫ
		}else if(rgb==5)
		{
			Write24Bit(Orange,0);//�ϳ���ɫ
			Write24Bit(Orange,0);//�ϳ���ɫ
			Write24Bit(Orange,0);//�ϳ���ɫ
			Write24Bit(Orange,0);//�ϳ���ɫ
		}else if(rgb==6)
		{
			Write24Bit(Green,0);//�ϳ���ɫ
			Write24Bit(Green,0);//�ϳ���ɫ
			Write24Bit(Green,0);//�ϳ���ɫ
			Write24Bit(Green,0);//�ϳ���ɫ
		}else
		{
			Write24Bit(0,0,0,0);//�ϳ���ɫ
			Write24Bit(0,0,0,0);
			Write24Bit(0,0,0,0);
			Write24Bit(0,0,0,0);
		}
	}
	switchflag =switch_flag;
}
/*
ws2812b��ʾһ����ɫ
*/
void RGB_LED_WriteColor(uint8_t green, uint8_t red, uint8_t blue)
{
Write24Bit(green,red,blue,0);

}
/*
ws2812b�ر�
*/
void RGB_LED_Off(void)
{
Write24Bit(0,0,0,0);
}
void Ws2812Init(void){
ROM_SysCtlPeripheralEnable(RGB_SYSCTL);
ROM_GPIOPinTypeGPIOOutput(RGB);

ROM_SysCtlPeripheralEnable(IMU_RGB_SYSCTL);
ROM_GPIOPinTypeGPIOOutput(IMU_RGB);
}


