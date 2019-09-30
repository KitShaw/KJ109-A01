
/*
 *led.h
 *
 */
 
#ifndef __LED_H_
#define __LED_H_

#define LED_ON   0
#define LED_OFF  1


#define LED_POWER_R_INDICATOR_FLAG led_flag.bit0
#define LED_POWER_W_INDICATOR_FLAG led_flag.bit1
#define LED_SPEED_R_INDICATOR_FLAG led_flag.bit2
#define LED_SPEED_W_INDICATOR_FLAG led_flag.bit3
#define LED_SLEEP_INDICATOR_FLAG 	 led_flag.bit4
#define LED_NIGHT_LIGHT_INDICATOR_FLAG 	 led_flag.bit5
#define LED_OVER_VOLTAGE_FLASH_FLAG 	 led_flag.bit6
#define LED_SWITCH_FLAG						led_flag.bit7

#define NIGHT_LIGHT_PIN P15

void led_init(void);
void led_task(void);
void led_key_speed(void);
void led_power_off(void);
void led_power_on(void);
void led_key_sleep(void);
void led_key_night_light(void);
void set_night_light_level(unsigned char level);
void night_light_level_handle(void);
unsigned char read_night_light_level(void);
void night_light_task(void);
void led_filter_out_time(void);
void reset_led_flter_out_time(void);

#endif