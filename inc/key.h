
/*
 *key.h
 */
 
#ifndef __KEY_H_
#define __KEY_H_

void key_init(void);
void key_task(void);
void Sys_Scan(void);
void key_speed_com(void);
void key_speed_long_com(void);
void key_power_com(void);
void key_ion_com(void);
void key_arom_com(void);
void key_timer_com(void);
void key_lock_com(void);
bit read_unlock_flag(void);


#endif
