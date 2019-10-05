
/*
 * beep.c
 * Kit Shaw
 * 2019.10.5
 */
 
#include "SC92F844X_C.h"
#include "beep.h"

unsigned char xdata  beep_count;

void beep_init(void)
{
	P0CON &= ~(1<<7);  //P07输出模式
	P0CON |= 1<<3;  //03输出
	P03 = 0;
	P0PH |= (1<<7);
	set_beep_count(20);
}

void beep_task(void)
//100ms调用一次
{
	//BEEP_PIN = 0;
	//return;
	if(beep_count>0)
	{
		BEEP_PIN = 1;
		beep_count--;
	}
	else
	{
		BEEP_PIN = 0;
	}
}

void set_beep_count(unsigned char beep_count_val)
{
	beep_count = beep_count_val;
}
