
/*
 * arom.c
 * Kit Shaw
 * 2019.10.4
 */
 
#include "SC92F844X_C.h"
#include "arom.h"
#include "eeprom.h"

unsigned char arom_init_speed;
unsigned char arom_level;  
unsigned int arom_work_count;
void arom_init(void)
{
	P5CON |= 1<<5;  //P55输出模式
	arom_off();
	arom_level = read_arom_level_form_eeprom();
}


void write_arom_level_to_eeprom(void)
{
	WDTCON = 0x10;
	eeprom_write_byte(26, arom_level);
	arom_init_speed = arom_level;
}




unsigned char read_arom_level_form_eeprom(void)
{
	unsigned char tmp;
	WDTCON = 0x10;
	tmp = eeprom_read_byte(26);
	if((tmp == 1) || (tmp==2) || (tmp == 3))return tmp;
	else return 0;
}


void arom_task(void)
	// 1s掉用一次
	// low 工作1分钟停12分钟
	// mid 工作1分钟停8分钟
	// high 工作1分钟停4分钟
{
	switch(arom_level)
	{
		case 1://低档工作10分钟停10分钟
		 if(arom_work_count == 0)arom_on();
		 if(arom_work_count == 60)arom_off();
		 if(++arom_work_count>=780) arom_work_count = 0;
		break;
		case 2://中档 : 工作30分钟停10分钟
			if(arom_work_count == 0)arom_on();
		 	if(arom_work_count == 60)arom_off();
		 	if(++arom_work_count>=540) arom_work_count = 0;
		break;
		case 3://高档: 连续工作
			if(arom_work_count == 0)arom_on();
		 	if(arom_work_count == 60)arom_off();
		 	if(++arom_work_count>=300) arom_work_count = 0;
		break;
		default:
			arom_off();
		break;
	}
}

void arom_on(void)
{
	AROM_PIN = 1;
}

void arom_off(void)
{
	AROM_PIN = 0;
}

void regulate_arom_level(void)
//调整转, 掉用一次切换一次, 自动, 低, 中, 高依次循环
{
	if(++arom_level> 3) arom_level = 0;	
	arom_work_count = 0;
}

void set_arom_level(unsigned char level)
{
	arom_level = level;
}

unsigned char read_arom_level(void)
{
	return arom_level;
}



