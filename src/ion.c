
/*
 * ion.c
 * Kit Shaw
 * 2019.10.4
 */
 
#include "SC92F844X_C.h"
#include "ion.h"


void ion_init(void)
{
	P4CON |= 1<<5;  //P45输出模式
	//P4PH |= 1<<5;  //上拉
	ion_off();
}

void ion_on(void)
{
	ION_PIN = 0;
}

void ion_off(void)
{
	ION_PIN = 1;
}