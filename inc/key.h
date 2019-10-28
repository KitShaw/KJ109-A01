
/*
 *key.h
 */
 
#ifndef __KEY_H_
#define __KEY_H_

void key_init(void);
void key_task(void);
void Sys_Scan(void);
void key_speed_com(void);
void key_power_long_com(void);
void key_power_com(void);
void key_ion_com(void);
void key_arom_com(void);
void key_timer_com(void);
void key_lock_com(void);
unsigned char read_lock_flag(void);
void reset_key_no_move_count(void);
void reset_lock_flag(void);
void key_power_speed_com(void);




#endif
