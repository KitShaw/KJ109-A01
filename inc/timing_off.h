
/*
 * timing_off.h
 * Kit Shaw
 * 2019.10.4
 */
 
 #ifndef __TIMING_OFF_H_
 #define __TIMING_OFF_H_
 
 void timing_off_init(void);
 void timing_off_task(void);
 void regulate_timing_off_level(void);
 unsigned char read_timing_off_level(void);
 void reset_timing_off_level(void);
 
 #endif  //__TIMING_OFF_H_
 