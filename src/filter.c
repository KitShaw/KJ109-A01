
/*
 * filter.c
 *
 */
 
#include "filter.h"
#include "eeprom.h"
#include "led.h"
#include "data_type.h"
#include "fan.h"
#include "sc92f844x_c.h"
//#include "IAP.h"


volatile unsigned long filter_time;  //滤网寿命存储在eerom的12h(高位), 13h, 14h, 15h(地位)
unsigned char filter_out_time_flag; //滤网过期标志, 1,outtime
 
void filter_init(void)
{
	//read_filter_time();
	filter_time = read_filter_time();
	if(filter_time >= FILTER_TOTAL_TIME)filter_out_time_flag = 1;
}

void filter_task(void)
//100ms调用一次
{
	
	static unsigned char count_1s, count2;
	if(read_power_status() == POWER_OFF_STATUS) return;
	if(1 == filter_out_time_flag )
	{
		if(++count2 >= 3)
		{
			count2 = 0;
//			led_filter_out_time();
		}
	}
	if(++count_1s >= 10) 
	{
		count_1s = 0;
		if(filter_time >= FILTER_TOTAL_TIME)
		{
			if(0 == filter_out_time_flag)write_filter_time();  //到期要写入一次
			filter_out_time_flag = 1;
		}
		else {filter_time++; }
		
	}
}

void write_filter_time(void)
{
	unsigned long tmp;
	tmp = filter_time;
	//while(eeprom_write_byte(15, tmp & 0x000000ff) != 1);//如果写入的和读出的不一致就卡住
	//WDTCON = 0x10;
	//IAPWrite(0x0f,(unsigned char)tmp,IapEPPROM); 
	WDTCON = 0x10;
	eeprom_write_byte(15, tmp & 0x000000ff);
	//IAPWrite(0x0f,0x20,IapEPPROM); 
	
	tmp >>= 8;
	WDTCON = 0x10;
	eeprom_write_byte(14, tmp & 0x000000ff);
	tmp >>= 8;
	WDTCON = 0x10;
	eeprom_write_byte(13, tmp & 0x000000ff);
	tmp >>= 8;
	WDTCON = 0x10;
	eeprom_write_byte(12, tmp & 0x000000ff);
	
	
	
}

unsigned long read_filter_time(void)
{
	unsigned long temp = 0;
	//temp = eeprom_read_byte(15);
	//WDTCON = 0x10;
	//temp = IAPRead(0x0f, IapEPPROM);
	
	WDTCON = 0x10;
	temp = eeprom_read_byte(12);
	temp <<= 8;
	WDTCON = 0x10;
	temp += eeprom_read_byte(13);
	temp <<= 8;
	WDTCON = 0x10;
	temp += eeprom_read_byte(14);
	temp <<= 8;
	WDTCON = 0x10;
	temp += eeprom_read_byte(15);
	//temp += IAPRead(0x0f, IapEPPROM);
	return temp;
}



void reset_filter_time(void)
{
	
	WDTCON  = 0x10;	
	eeprom_write_byte(12, 0);
	WDTCON  = 0x10;
	eeprom_write_byte(13, 0);
	WDTCON  = 0x10;
	eeprom_write_byte(14, 0);
	WDTCON  = 0x10;
	eeprom_write_byte(15, 0);
	
	//t2 = IAPRead(0x71, IapEPPROM);
	
	/*
	IAPWrite(12,0x00,IapEPPROM); 
	WDTCON = 0x10;
	IAPWrite(13,0x00,IapEPPROM); 
	WDTCON  = 0x10;	
	IAPWrite(14,0x00,IapEPPROM); 
	WDTCON = 0x10;
	IAPWrite(15,0x00,IapEPPROM); 
	*/
	filter_time = 0;
	filter_out_time_flag = 0;
	
//	reset_led_flter_out_time();
}



