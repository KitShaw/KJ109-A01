
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


//#define LED_POWER_R_INDICATOR_FLAG led_flag.bit0


void led_init(void);
void led_task(void);


#endif
