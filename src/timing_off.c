
/*
 * timing_off.c
 * Kit Shaw
 * 2019.10.4
 */
 
#include "SC92F844X_C.h"
#include "timing_off.h"
#include "fan.h"

unsigned char timing_off_level;  
unsigned int timing_off_count;
void timing_off_init(void)
{
	timing_off_level = 0;
}

void timing_off_task(void)
	// 1s掉用一次
{
	switch(timing_off_level)
	{
		case 1://1H
			if(++timing_off_count>= 3600) power_off();
		break;
		case 2://2H
			if(++timing_off_count>=7200) power_off();
		break;
		case 3://4H
			if(++timing_off_count>=14400) power_off();
		break;
		case 4://8H
			if(++timing_off_count>=28800) power_off();
		break;
		default:
			
		break;
	}
}
void regulate_timing_off_level(void)
//调整转, 掉用一次切换一次, 自动, 低, 中, 高依次循环
{
	if(++timing_off_level> 4) timing_off_level = 0;	
	timing_off_count = 0;
}

void reset_timing_off_level(void)
{
	timing_off_level = 0;
}

unsigned char read_timing_off_level(void)
{
	return timing_off_level;
}



