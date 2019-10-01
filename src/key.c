

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
//#include "IAP.h"


bitval key_flag;

#define KEY_POWER_FLAG	key_flag.bit0
#define KEY_SPEED_FLAG	key_flag.bit1
#define KEY_SLEEP_FLAG	key_flag.bit2
#define KEY_NIGHT_LIGHT_FLAG	key_flag.bit3
unsigned char key_power_count;
unsigned short key_speed_count;
unsigned char key_sleep_count;
unsigned char key_night_light_count;

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
		exKeyValueFlag = TouchKeyScan();//�������ݴ�����    ��Դ����0x0800, ���ټ���0x0400, ˯�߼���0x0200
		TouchKeyRestart();				//������һ��ת��																														 			
	}		  	   
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
	
	//if(read_over_voltage_flag() == 1) return;  //��ѹ��������������ֱ�ӷ���
	if((exKeyValueFlag & 0x0000031e0) == 0x000000100)//��Դ��  //�����Ȳ���
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
	
	if((exKeyValueFlag & 0x000000f00) == 0x000000100)//ҹ�Ƽ� 
	{
		if(0 == KEY_NIGHT_LIGHT_FLAG)
		{
			if(++key_night_light_count >= 50)
			{
				KEY_NIGHT_LIGHT_FLAG = 1;
				key_night_light_com();
			}
		}
	}
	else 
	{
		KEY_NIGHT_LIGHT_FLAG = 0;
		key_night_light_count = 0;
	}
	
	if(read_power_status() == POWER_OFF_STATUS)return; //�ػ�״ֱ̬�ӷ���
	
	if((exKeyValueFlag & 0x000000f00) == 0x000000400)//���ټ� 
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
		if((key_speed_count>50) && (key_speed_count< 5000))
		{
			key_speed_com();
		}
		KEY_SPEED_FLAG = 0;
		key_speed_count = 0;
	}
	
	if((exKeyValueFlag & 0x000000f00) == 0x000000200)//˯�߼� 
	{
		if(0 == KEY_SLEEP_FLAG)
		{
			if(++key_sleep_count >= 50)
			{
				KEY_SLEEP_FLAG = 1;
				key_sleep_com();
			}
		}
	}
	else 
	{
		KEY_SLEEP_FLAG = 0;
		key_sleep_count = 0;
	}
		
}

void key_night_light_com(void)
{
	//night_light_level_handle();
	
}


void key_speed_com(void)
{
	if(2 == read_fan_speed())set_fan_speed(1);
	else set_fan_speed(2);
	//if(flash_write_byte(10, 10) == 1)
	//led_key_speed();
}

void key_speed_long_com(void)
//�������ټ������������
{
	reset_filter_time();
}

void key_sleep_com(void)
{	
	P50 = ~P50;
	if(read_fan_speed() != 0) 
	{
		set_fan_speed(0);
		//set_night_light_level(0);
		//led_key_sleep();
	}
	else 
	{
		set_fan_speed(1);
		//led_key_speed();
	}
	
}



void key_power_com(void)
{
	//unsigned long filter_time_temp;
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


