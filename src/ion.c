
/*
 * ion.c
 */
 
#include "SC92F844X_C.h"
#include "ion.h"


void ion_init(void)
{
//	P1PH &= ~(1<<7);  //关闭P17的上拉
//	P17 = 0;
//	P1CON |= 1<<7;  //P17输出模式
	ion_off();
}

void ion_on(void)
{
	ION_PIN = 1;
}

void ion_off(void)
{
	ION_PIN = 0;
}