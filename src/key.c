

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
#include "arom.h"
#include "timing_off.h"
#include "beep.h"


bitval key_flag;
bitval key_flag2;

#define KEY_POWER_FLAG	key_flag.bit0
#define KEY_AROM_FLAG	key_flag.bit1   
#define KEY_SPEED_FLAG	key_flag.bit2
#define KEY_LOCK_FLAG	key_flag.bit3
#define LOCK_FLAG		key_flag.bit6            //童锁解锁标志, 0解锁, 1锁住
#define KEY_ION_FLAG key_flag.bit4
#define KEY_TIMER_FLAG key_flag.bit5
#define KEY_POWER_SPEED_FLAG key_flag.bit7

#define KEY_POWER_SPEED_FLAG2 key_flag2.bit0 
#define KEY_POWER_SHORT_FLAG key_flag2.bit1
#define KEY_SPEED_TIMER_FLAG key_flag2.bit2
#define KEY_SPEED_AROM_FLAG key_flag2.bit3
#define KEY_POWER_AROM_FLAG key_flag2.bit4
#define KEY_POWER_AROM_FLAG2 key_flag2.bit5
#define KEY_POWER_ION_FLAG key_flag2.bit6
#define KEY_POWER_ION_FLAG2 key_flag2.bit7




//按下电源和风速键后要等2个按键都释放在复位此位

unsigned short key_speed_arom_count;
unsigned short key_speed_timer_count;
unsigned short key_power_count;
unsigned short key_speed_count;
unsigned short key_lock_count;
unsigned short key_ion_count;
unsigned short key_timer_count;
unsigned short key_arom_count;
unsigned short key_power_speed_count;
unsigned short key_power_ion_count;
unsigned short key_power_arom_count;
unsigned short power_up_20s_count;  //上电20s计数, 20秒内可以查看版本号

unsigned long xdata key_no_move_count;      //按键没要按下计数, 如果一分钟没有动作,童锁就锁住


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
		exKeyValueFlag = TouchKeyScan();//按键数据处理函数    
		if((exKeyValueFlag & 0x0000031e0) != 0)reset_key_no_move_count(); //有键按下就复位按键计数值
		TouchKeyRestart();				//启动下一轮转换																														 			
	}		  	   
}

void key_init(void)
{
	LOCK_FLAG = 0;  
	reset_key_no_move_count();
	//KEY_AROM_SPEED_TIMER_FLAG = 0;
	KEY_POWER_FLAG = 0;
	KEY_AROM_FLAG = 0;
	KEY_SPEED_FLAG = 0;
	KEY_LOCK_FLAG = 0;
	LOCK_FLAG = 0;            //童锁解锁标志, 0解锁, 1锁住
	KEY_ION_FLAG = 0;
	KEY_TIMER_FLAG = 0;
	KEY_POWER_SPEED_FLAG = 0;
	KEY_POWER_SPEED_FLAG2  = 0;
	KEY_POWER_SHORT_FLAG = 0;
	KEY_SPEED_TIMER_FLAG = 0;
	power_up_20s_count = 0;
}

void reset_key_no_move_count(void)
{
	key_no_move_count = 0;
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
	if(key_no_move_count >= 180000)
	{
		LOCK_FLAG = 1; 
	}
	else
	{
		//reset_key_no_move_count();
		if(POWER_ON_STATUS == read_power_status())key_no_move_count++;
	}
	

	

	if(((exKeyValueFlag & 0x0000031e0) == 0x000000100) && (0 == KEY_POWER_SPEED_FLAG2)		
		&& (0 == KEY_POWER_ION_FLAG2) && (0 == KEY_POWER_AROM_FLAG2) && (0 == LOCK_FLAG))//电源键  //灵敏度不够
	{
		if(0 == KEY_POWER_FLAG)
		{			
			if(++key_power_count >= 5000)
			{
				KEY_POWER_FLAG = 1;		
				key_power_long_com();
			}
		}
	}
	else 
	{
		//if((exKeyValueFlag & 0x0000031e0) != 0x000000180) 
		if((exKeyValueFlag & 0x0000031e0) == 0) 
		{
			if((key_power_count>50) && (key_power_count< 5000) )//&& ( 0 == KEY_POWER_SHORT_FLAG))
			{
				//KEY_POWER_SHORT_FLAG = 1;
				key_power_com();
			}
		}
		//if((exKeyValueFlag & 0x0000031e0) == 0x000000000) KEY_POWER_SHORT_FLAG = 0;
		KEY_POWER_FLAG = 0;
		key_power_count = 0;		
	}
	
	if(power_up_20s_count < 20000) power_up_20s_count++;
	
	if(read_power_status() == POWER_OFF_STATUS)
	{
		if(((exKeyValueFlag & 0x0000031e0) == 0x0000002080) )  //80(s 2000a 20t
		{
			if(0 == KEY_SPEED_AROM_FLAG)
			{
				if(++key_speed_arom_count >= 5000)
					//显示软件版本号
				{
					KEY_SPEED_AROM_FLAG = 1;
					chang_init_up_power();  //改变上电初始状态
				}
			}			
		}
		else
		{
			key_speed_arom_count = 0;
			KEY_SPEED_AROM_FLAG = 0;
		}

		if(((exKeyValueFlag & 0x0000031e0) == 0x0000000a0) && (power_up_20s_count<20000))  //80(s 2000a 20t
		{
			if(0 == KEY_SPEED_TIMER_FLAG)
			{
				if(++key_speed_timer_count >= 15000)
					//显示软件版本号
				{
					KEY_SPEED_TIMER_FLAG = 1;
					led_display_version();
				}
			}
			
		}
		
		return; //关机状态直接返回
	}
	if(((exKeyValueFlag & 0x0000031e0) == 0x000000180) && (0 == LOCK_FLAG))
	//风速加电源按键
	{
		if(0 == KEY_POWER_SPEED_FLAG)
		{			
			if(++key_power_speed_count >= 50)
			{
				KEY_POWER_SPEED_FLAG = 1;	
				KEY_POWER_SPEED_FLAG2 = 1;
				key_power_speed_com();
			}
		}
	}
	else 
	{		
		if(((exKeyValueFlag & 0x000000180) == 0)) KEY_POWER_SPEED_FLAG2 = 0;
		KEY_POWER_SPEED_FLAG = 0;
		key_power_speed_count = 0;
	}

	
	
	if(((exKeyValueFlag & 0x0000031e0) == 0x000000080)&& (0 == KEY_POWER_SPEED_FLAG2)
		&& (0 == LOCK_FLAG) )//风速键 
	{
		if(0 == KEY_SPEED_FLAG)
		{
			if(++key_speed_count >= 50)
			{
				KEY_SPEED_FLAG = 1;
				key_speed_com();
			}
		}
	}
	else 
	{
		if((exKeyValueFlag & 0x000000080) == 0)
		{
			KEY_SPEED_FLAG = 0;
			key_speed_count = 0;
		}
	}

	if(((exKeyValueFlag & 0x0000031e0) == 0x0000001100) && (0 == LOCK_FLAG))
	//电源按键加负离子
	{
		if(0 == KEY_POWER_ION_FLAG)
		{			
			if(++key_power_ion_count >= 50)
			{
				KEY_POWER_ION_FLAG = 1;	
				KEY_POWER_ION_FLAG2 = 1;
				write_ion_to_eeprom();
				set_beep_count(10);
				//key_power_speed_com();
			}
		}
	}
	else 
	{		
		if(((exKeyValueFlag & 0x0000001100) == 0)) KEY_POWER_ION_FLAG2 = 0;
		KEY_POWER_ION_FLAG = 0;
		key_power_ion_count = 0;
	}
	
	if(((exKeyValueFlag & 0x0000031e0) == 0x000001000) && (0 == KEY_POWER_ION_FLAG2)
		&& (0 == LOCK_FLAG))//ion
	{
		if(0 == KEY_ION_FLAG)
		{
			if(++key_ion_count >= 50)
			{
				KEY_ION_FLAG = 1;
				key_ion_com();				
			}
		}
	}
	else 
	{
		if((exKeyValueFlag & 0x000001000) == 0)
		{
			KEY_ION_FLAG = 0;
			key_ion_count = 0;
		}
	}


	if(((exKeyValueFlag & 0x0000031e0) == 0x0000002100) && (0 == LOCK_FLAG))
	//电源按键加香薰
	{
		if(0 == KEY_POWER_AROM_FLAG)
		{			
			if(++key_power_arom_count >= 50)
			{
				KEY_POWER_AROM_FLAG = 1;	
				KEY_POWER_AROM_FLAG2 = 1;
				//write_ion_to_eeprom();				
				key_power_arom_com();
			}
		}
	}
	else 
	{		
		if(((exKeyValueFlag & 0x0000002100) == 0)) KEY_POWER_AROM_FLAG2 = 0;
		KEY_POWER_AROM_FLAG = 0;
		key_power_arom_count = 0;
	}
	
	if(((exKeyValueFlag & 0x0000031e0) == 0x000002000) && (0 == KEY_POWER_AROM_FLAG2)
		&& (0 == LOCK_FLAG))//arom
	{
		if(0 == KEY_AROM_FLAG)
		{
			if(++key_arom_count >= 50)
			{
				KEY_AROM_FLAG = 1;
				key_arom_com();				
			}
		}
	}
	else 
	{
		if((exKeyValueFlag & 0x000002000) == 0)
		{
			KEY_AROM_FLAG = 0;
			key_arom_count = 0;
		}
	}
		
	if((exKeyValueFlag & 0x0000031e0) == 0x000000040)//
	{
		if(0 == KEY_LOCK_FLAG)
		{
			if(++key_lock_count >= 3000)
			{
				KEY_LOCK_FLAG = 1;
				//set_beep_count(10);
				key_lock_com();				
			}
		}
	}
	else 
	{
		if((exKeyValueFlag & 0x000000040) == 0x000000000)
		{
			KEY_LOCK_FLAG = 0;
			key_lock_count = 0;
		}
	}
	if(((exKeyValueFlag & 0x0000031e0) == 0x000000020) && (0 == LOCK_FLAG))//
	{
		if(0 == KEY_TIMER_FLAG)
		{
			if(++key_timer_count >= 50)
			{
				KEY_TIMER_FLAG = 1;
				key_timer_com();				
			}
		}
	}
	else 
	{
		if((exKeyValueFlag & 0x000000020) == 0x000000000)
		{
			KEY_TIMER_FLAG = 0;
			key_timer_count = 0;
		}
	}
}

void key_ion_com(void)
{
	ION_PIN = !ION_PIN;
	set_beep_count(10);
	//fan_pulse_count_add();
}

void reset_lock_flag(void)
{
	LOCK_FLAG = 0;
	reset_key_no_move_count();
	set_beep_count(10);
}

void key_timer_com(void)
{
	regulate_timing_off_level();
	set_beep_count(10);
}

void key_arom_com(void)
{
	regulate_arom_level();
	set_beep_count(10);
	//fan_pulse_count_dec();
}

void key_power_speed_com(void)
{
	regulate_fan_speed();//速度加1
	write_speed_to_eeprom();//写入eeprom
	set_beep_count(10);
}

void key_power_arom_com(void)
{
	regulate_arom_level();
	write_arom_level_to_eeprom();
	set_beep_count(10);
}

void key_lock_com(void)
{
	//P52 = ~P52;
	//UNLOCk_FLAG = ~UNLOCk_FLAG;
	//LOCK_FLAG = ~LOCK_FLAG;      //解
	reset_lock_flag();	
	
}
unsigned char read_lock_flag(void)
{
	return LOCK_FLAG;
}

void key_speed_com(void)
{
	regulate_fan_speed();
	set_beep_count(10);
	
}

void key_power_long_com(void)
//长按电源键清楚滤网寿命
{
	reset_filter_time();
	set_beep_count(10);
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
	//P50 = ~P50;
	set_beep_count(10);
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


