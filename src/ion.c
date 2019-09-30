
/*
 * ion.c
 */
 
#include "SC92F837X_C.h"
#include "ion.h"


void ion_init(void)
{
	P0PH &= ~(1<<1);  //关闭P01的上拉
	P01 = 0;
	P0CON |= 1<<1;  //P01输出模式
	ion_off();
}

void ion_on(void)
{
	P01 = 1;
}

void ion_off(void)
{
	P01 = 0;
}