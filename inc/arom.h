
/*
 * arom.h
 * Kit Shaw
 * 2019.10.4
 */
 
 #ifndef __AROM_H_
 #define __AROM_H_
 
 #define AROM_PIN P55
 
 void arom_init(void);
 void arom_on(void);
 void arom_off(void);
 void arom_task(void);
 void regulate_arom_level(void);
 unsigned char read_arom_level(void);
 void set_arom_level(unsigned char level);
 
 #endif  //__ION_H_
 