
/*
 * ion.h
 * Kit Shaw
 * 2019.10.4
 */
 
 #ifndef __ION_H_
 #define __ION_H_
 
 #define ION_PIN P45
 
 void ion_init(void);
 void ion_on(void);
 void ion_off(void);
 unsigned char read_ion_init_status(void);
 void write_ion_to_eeprom(void);
 
 #endif  //__ION_H_
 