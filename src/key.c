

/*
 * key.c
 */

#include <key.h>
#include "inc\SC92F844x_C.h"
#include "data_type.h"
#include "lib\SensorMethod.h"
#include "fan.h"
#include "led.h"
#include "eeprom.h"
#include "filter.h"
#include "ion.h"
//#include "IAP.h"


bitval key_flag;

#define KEY_POWER_FLAG	key_flag.bit0
#define KEY_AROM_FLAG	key_flag.bit1   
#define KEY_SPEED_FLAG	key_flag.bit2
#define KEY_LOCK_FLAG	key_flag.bit3
#define KEY_ION_FLAG key_flag.bit4
#define KEY_TIMER_FLAG key_flag.bit5
unsigned char key_power_count;
unsigned short key_speed_count;
unsigned char key_lock_count;
unsigned char key_ion_count;
unsigned char key_timer_count;
unsigned char key_arom_count;


INT32U exKeyValueFlag = 0;		//当前轮按键标志


/**************************************************
*函数名称：void  Sys_Scan(void) 
*函数功能：扫描TK和显示
*入口参数：void
*出口参数：void  
**************************************************/
void Sys_Scan(void)
{      				
	if(SOCAPI_TouchKeyStatus&0x80)	    //重要步骤2:  触摸键扫描一轮标志，是否调用TouchKeyScan()一定要根据此标志位置起后
	 {	   																	
		SOCAPI_TouchKeyStatus &= 0x7f;	//重要步骤3: 清除标志位， 需要外部清除。													    
		exKeyValueFlag = TouchKeyScan();//按键数据处理函数    电源键是0x0800, 风速键是0x0400, 睡眠键是0x0200
		TouchKeyRestart();				//启动下一轮转换																														 			
	}		  	   
}


//0x0100--power
//0x2000--香薰
//0x1000--ion
//0x0080--模式
//0x0040--童锁
//0x0020--timer
//0x31e0

void key_task(void)       
//按键任务, 1ms调用一次
{	
	if((exKeyValueFlag & 0x0000031e0) == 0x000000100)//电源键  //灵敏度不够
	{
		if(0 == KEY_POWER_FLAG)
		{
			if(++key_power_count >= 50)
			{
				KEY_POWER_FLAG = 1;
				key_power_com();				
			}
		}
	}
	else 
	{
		KEY_POWER_FLAG = 0;
		key_power_count = 0;
	}
	
	
	
//	if(read_power_status() == POWER_OFF_STATUS)return; //关机状态直接返回
	
	if((exKeyValueFlag & 0x0000031e0) == 0x000000080)//风速键 
	{
		if(0 == KEY_SPEED_FLAG)
		{
			if(++key_speed_count >= 5000)
			{
				KEY_SPEED_FLAG = 1;
				key_speed_long_com();
			}
		}
	}
	else 
	{
		if((key_speed_count>5) && (key_speed_count< 5000))
		{
			key_speed_com();
		}
		KEY_SPEED_FLAG = 0;
		key_speed_count = 0;
	}
	
	if((exKeyValueFlag & 0x0000031e0) == 0x000001000)//ion
	{
		if(0 == KEY_ION_FLAG)
		{
			if(++key_ion_count >= 10)
			{
				KEY_ION_FLAG = 1;
				key_ion_com();				
			}
		}
	}
	else 
	{
		KEY_ION_FLAG = 0;
		key_ion_count = 0;
	}
	
	if((exKeyValueFlag & 0x0000031e0) == 0x000002000)//arom
	{
		if(0 == KEY_AROM_FLAG)
		{
			if(++key_arom_count >= 10)
			{
				KEY_AROM_FLAG = 1;
				key_arom_com();				
			}
		}
	}
	else 
	{
		KEY_AROM_FLAG = 0;
		key_arom_count = 0;
	}
		
	if((exKeyValueFlag & 0x0000031e0) == 0x000000040)//
	{
		if(0 == KEY_LOCK_FLAG)
		{
			if(++key_lock_count >= 10)
			{
				KEY_LOCK_FLAG = 1;
				key_lock_com();				
			}
		}
	}
	else 
	{
		KEY_LOCK_FLAG = 0;
		key_lock_count = 0;
	}
	if((exKeyValueFlag & 0x0000031e0) == 0x000000020)//
	{
		if(0 == KEY_TIMER_FLAG)
		{
			if(++key_timer_count >= 10)
			{
				KEY_TIMER_FLAG = 1;
				key_timer_com();				
			}
		}
	}
	else 
	{
		KEY_TIMER_FLAG = 0;
		key_timer_count = 0;
	}
}

void key_ion_com(void)
{
	ION_PIN = !ION_PIN;
}

void key_timer_com(void)
{
	P50 = ~P50;
}

void key_arom_com(void)
{
	P51 = ~P51;
}

void key_lock_com(void)
{
	P52 = ~P52;
}

void key_speed_com(void)
{
	regulate_fan_speed();
}

void key_speed_long_com(void)
//长按风速键清楚滤网寿命
{
	reset_filter_time();
	P51 = ~P51;
}

//void key_sleep_com(void)
//{	
//	P50 = ~P50;
//	if(read_fan_speed() != 0) 
//	{
/////		set_fan_speed(0);
//		//set_night_light_level(0);
//		//led_key_sleep();
//	}
//	else 
//	{
////		set_fan_speed(1);
//		//led_key_speed();
//	}
//	
//}



void key_power_com(void)
{
	//unsigned long filter_time_temp;
	P50 = ~P50;
	if(POWER_ON_STATUS == read_power_status())
	{
		power_off();
		//if((read_filter_time()) & 0x000000ff == 0) led_power_on();
		
		//filter_time_temp = read_filter_time();
		//WDTCON  = 0x10;
		//IAPWrite(0x0f,(unsigned char)(filter_time_temp & 0x000000ff),IapEPPROM); 
		//IAPWrite(0x0f,0x20,IapEPPROM); 
		//IAPWrite(0x0f,31,IapEPPROM); 
		write_filter_time();
	}
	else
	{
		power_on();
	}
}


