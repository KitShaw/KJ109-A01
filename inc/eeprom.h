/*
 *eeprom.h
 *
 */

#ifndef __EEPROM_H_
#define __EEPROM_H_

void eeprom_init(void);
unsigned char eeprom_write_byte(unsigned char add, unsigned char e_data);
unsigned char eeprom_read_byte(unsigned char addr);

unsigned char flash_write_byte(unsigned short add, unsigned char e_data);
unsigned char flash_read_byte(unsigned short addr);

#endif           //__EEPROM_H_

