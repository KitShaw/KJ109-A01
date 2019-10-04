
/*
 * led.h
 * Kit Shaw
 * 2019/10/01
 */
 
#ifndef __LED_H_
#define __LED_H_

#define LED_ON   0
#define LED_OFF  1

#define LED_RED	P52
#define LED_GREEN	P51
#define LED_BLUE P50

#define HUNDRED_DIGIT  7
#define TEN_DIGIT	6
#define SINGLE_DIGIT 5



void led_init(void);
void led_task(void);
void led_display_bcd(unsigned char bcd_value, unsigned char digit);
void led_display_mode(void);
void led_display_ion(void);
void led_display_unlock(void);
void led_display_pm25(void);
void led_display_dust_level(void);
void led_off(void);
void led_on(void);


#endif
