
/*
 * ion.c
 * Kit Shaw
 * 2019.10.4
 */
 
#include "SC92F844X_C.h"
#include "ion.h"


void ion_init(void)
{
	P4CON |= 1<<5;  //P45Êä³öÄ£Ê½
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