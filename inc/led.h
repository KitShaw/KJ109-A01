
/*
 * led.h
 * Kit Shaw
 * 2019/10/01
 */
 
#ifndef __LED_H_
#define __LED_H_

#define LED_ON   0
#define LED_OFF  1

#define LED_RED	P24
#define LED_GREEN	P25
#define LED_BLUE P26
#define LED_PM2_5 P01
#define LED_TIMER_1H P51
#define LED_TIMER_2H P50
#define LED_TIMER_4H P06
#define LED_TIMER_8H P05
#define LED_KEY_TIMER P40

#define LED_FILTER  P53
#define LED_ION P16
#define LED_KEY_ION P17
#define LED_KEY_AROM P20
#define LED_AROM_LOW P21
#define LED_AROM_MIDDLE P22
#define LED_AROM_HIGH P23

#define LED_SPEED_LOW P47
#define LED_SPEED_MIDDLE P46
#define LED_SPEED_HIGH P36
#define LED_SPEED_AUTO P35
#define LED_KEY_MODE P34

#define LED_LOCK	P41
#define LED_KEY_LOCK P37

#define LED_KEY_POWER P33


#define HUNDRED_DIGIT  7
#define TEN_DIGIT	6
#define SINGLE_DIGIT 5



void led_init(void);
void led_task(void);
void led_display_bcd(unsigned char bcd_value, unsigned char digit);
void led_display_mode(void);
void led_display_ion(void);
void led_display_lock(void);
void led_display_pm25(void);
void led_display_dust_level(void);
void led_off(void);
void led_on(void);
void led_all_on(void);

void led_display_arom_level(void);
void led_display_timing_off_level(void);
void led_display_filter_out(unsigned char filter_flag);




#endif
