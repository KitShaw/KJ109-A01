
/*
 * arom.c
 * Kit Shaw
 * 2019.10.4
 */
 
#include "SC92F844X_C.h"
#include "arom.h"

unsigned char arom_level;  
unsigned int arom_work_count;
void arom_init(void)
{
	P5CON |= 1<<5;  //P55输出模式
	arom_off();
	arom_level = 0;
}

void arom_task(void)
	// 1s掉用一次
{
	switch(arom_level)
	{
		case 1://低档工作10分钟停10分钟
		 if(arom_work_count == 0)arom_on();
		 if(arom_work_count == 600)arom_off();
		 if(++arom_work_count>=1200) arom_work_count = 0;
		break;
		case 2://中档 : 工作30分钟停10分钟
			if(arom_work_count == 0)arom_on();
		 	if(arom_work_count == 1800)arom_off();
		 	if(++arom_work_count>=2400) arom_work_count = 0;
		break;
		case 3://高档: 连续工作
			arom_on();
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



