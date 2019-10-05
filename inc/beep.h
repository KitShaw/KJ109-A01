
/*
 * beep.h
 * Kit Shaw
 * 2019.10.5
 */
 
 #ifndef __BEEP_H_
 #define __BEEP_H_
 
// #define BEEP_PIN P07
 #define BEEP_PIN P03
 
 void beep_init(void);
 void beep_task(void);
 void set_beep_count(unsigned char beep_count_val);
 
 #endif  //__ION_H_
 