

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

#define KEY_POWER_FLAG	key_flag.bit0
#define KEY_AROM_FLAG	key_flag.bit1   
#define KEY_SPEED_FLAG	key_flag.bit2
#define KEY_LOCK_FLAG	key_flag.bit3
#define LOCK_FLAG		key_flag.bit6            //ͯ��������־, 0����, 1��ס
#define KEY_ION_FLAG key_flag.bit4
#define KEY_TIMER_FLAG key_flag.bit5

unsigned short key_power_count;
unsigned short key_speed_count;
unsigned short key_lock_count;
unsigned short key_ion_count;
unsigned short key_timer_count;
unsigned short key_arom_count;

unsigned long xdata key_no_move_count;      //����ûҪ���¼���, ���һ����û�ж���,ͯ������ס


INT32U exKeyValueFlag = 0;		//��ǰ�ְ�����־


/**************************************************
*�������ƣ�void  Sys_Scan(void) 
*�������ܣ�ɨ��TK����ʾ
*��ڲ�����void
*���ڲ�����void  
**************************************************/
void Sys_Scan(void)
{      				
	if(SOCAPI_TouchKeyStatus&0x80)	    //��Ҫ����2:  ������ɨ��һ�ֱ�־���Ƿ����TouchKeyScan()һ��Ҫ���ݴ˱�־λ�����
	 {	   																	
		SOCAPI_TouchKeyStatus &= 0x7f;	//��Ҫ����3: �����־λ�� ��Ҫ�ⲿ�����													    
		exKeyValueFlag = TouchKeyScan();//�������ݴ�����    
		if((exKeyValueFlag & 0x0000031e0) != 0)reset_key_no_move_count(); //�м����¾͸�λ��������ֵ
		TouchKeyRestart();				//������һ��ת��																														 			
	}		  	   
}

void key_init(void)
{
	LOCK_FLAG = 0;  
	reset_key_no_move_count();
}

void reset_key_no_move_count(void)
{
	key_no_move_count = 0;
}

//0x0100--power
//0x2000--��޹
//0x1000--ion
//0x0080--ģʽ
//0x0040--ͯ��
//0x0020--timer
//0x31e0

void key_task(void)       
//��������, 1ms����һ��
{	
	if(key_no_move_count >= 180000)
	{
		LOCK_FLAG = 1; 
	}
	else
	{
		//reset_key_no_move_count();
		key_no_move_count++;
	}

	if(((exKeyValueFlag & 0x0000031e0) == 0x000000100) && (0 == LOCK_FLAG))//��Դ��  //�����Ȳ���
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
	
		if((key_power_count>50) && (key_power_count< 5000))
		{
			key_power_com();
		}
		
		KEY_POWER_FLAG = 0;
		key_power_count = 0;
	}
	
	
	
	if(read_power_status() == POWER_OFF_STATUS)return; //�ػ�״ֱ̬�ӷ���
	
	if(((exKeyValueFlag & 0x0000031e0) == 0x000000080) && (0 == LOCK_FLAG))//���ټ� 
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
//		if((key_speed_count>5) && (key_speed_count< 5000))
//		{
//			key_speed_com();
//		}
		KEY_SPEED_FLAG = 0;
		key_speed_count = 0;
	}
	
	if(((exKeyValueFlag & 0x0000031e0) == 0x000001000) && (0 == LOCK_FLAG))//ion
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
		KEY_ION_FLAG = 0;
		key_ion_count = 0;
	}
	
	if(((exKeyValueFlag & 0x0000031e0) == 0x000002000) && (0 == LOCK_FLAG))//arom
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
		KEY_AROM_FLAG = 0;
		key_arom_count = 0;
	}
		
	if((exKeyValueFlag & 0x0000031e0) == 0x000000040)//
	{
		if(0 == KEY_LOCK_FLAG)
		{
			if(++key_lock_count >= 5000)
			{
				KEY_LOCK_FLAG = 1;
				//set_beep_count(10);
				key_lock_com();				
			}
		}
	}
	else 
	{
		KEY_LOCK_FLAG = 0;
		key_lock_count = 0;
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
		KEY_TIMER_FLAG = 0;
		key_timer_count = 0;
	}
}

void key_ion_com(void)
{
	ION_PIN = !ION_PIN;
	set_beep_count(10);
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
	test_i2c();
}

void key_arom_com(void)
{
	regulate_arom_level();
	set_beep_count(10);
}

void key_lock_com(void)
{
	//P52 = ~P52;
	//UNLOCk_FLAG = ~UNLOCk_FLAG;
	//LOCK_FLAG = ~LOCK_FLAG;      //��
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
//������Դ�������������
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


