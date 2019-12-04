
/*
 * ion.c
 * Kit Shaw
 * 2019.10.4
 */
 
#include "SC92F844X_C.h"
#include "ion.h"
#include "eeprom.h"

unsigned char ion_init_status;    //0xaa是关闭状态, 其他开启

unsigned char read_ion_init_status(void)
	//24
{
	return ion_init_status;
}

void write_ion_to_eeprom(void)
{
	WDTCON = 0x10;
	if(read_ion_init_status() == 0xaa) eeprom_write_byte(24, 0x55);
	else eeprom_write_byte(24, 0xaa);
	ion_init_status = eeprom_read_byte(24);
	if(read_ion_init_status() == 0xaa)ion_off();
	else ion_on();
}

void ion_init(void)
{
	P4CON |= 1<<5;  //P45输出模式
	//P4PH |= 1<<5;  //上拉
	ion_off();
	WDTCON = 0x10;
	ion_init_status = eeprom_read_byte(24);
	//if(ion_init_status)
}

void ion_on(void)
{
	ION_PIN = 0;
}

void ion_off(void)
{
	ION_PIN = 1;
}