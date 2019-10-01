

/*
 * eeprom.c
 */

#include "SC92F844X_C.h"
#include "intrins.h"
#include "eeprom.h"

unsigned char dd_add;
unsigned char ee_data;
unsigned char code *point = 0x0000;
//unsigned char code *point_flash = 0x0000;

void eeprom_init(void)
{
	
}

unsigned char eeprom_write_byte(unsigned char addr, unsigned char e_data)
//写一个字节操作
{
	EA = 0;
	IAPADE = 0x02;
	IAPDAT = e_data;
	IAPADH = 0x00;
	IAPADL = addr;
	IAPKEY = 0xF0;
	IAPCTL = 0x0A;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	IAPADE = 0x00;
	EA = 1;
	if(eeprom_read_byte(addr) == e_data) return 1;//写入的数据和读的相同返回1, 否则返回0
	else return 0;
}

unsigned char eeprom_read_byte(unsigned char addr)
//读一个字节
{
	unsigned char data_buf;
	EA = 0;
	IAPADE = 0x02;
	data_buf = *(point + addr) ;
	IAPADE = 0x00;  //返回code区
	EA = 1;
	return data_buf;
}

/*
unsigned char flash_write_byte(unsigned short addr, unsigned char e_data)
//写一个字节操作
{
	//EA = 0;
	IAPADE = 0x00;
	IAPDAT = e_data;
	IAPADH = 0x1c;
	IAPADL = addr;
	IAPKEY = 0xF0;
	IAPCTL = 0x0A;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	//IAPADE = 0x00;
	//EA = 1;
	if(flash_read_byte(addr) == e_data) return 1;//写入的数据和读的相同返回1, 否则返回0
	else return 0;
}

unsigned char flash_read_byte(unsigned short addr)
//读一个字节
{
	unsigned char data_buf;
	EA = 0;
	//IAPADE = 0x00;
	data_buf = *((unsigned short *)addr + 0x1c00) ;
	//IAPADE = 0x00;  //返回code区
	EA = 1;
	return data_buf;
}
*/