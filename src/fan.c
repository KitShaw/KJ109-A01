

/*
 *fan.c
 *Kit Shaw
 * 2019.10.2
 */
 
#include "SC92F844X_C.h"
#include "led.h"
#include "fan.h"
#include "filter.h"
#include "IAP.h"
#include "ion.H"

unsigned char power_status;
unsigned char fan_speed;  // 0智能, 1静音, 2中, 3高

/*
void set_power_status(unsigned char sta)
{
	power_status = sta;
}
*/
unsigned char read_power_status(void)
{
	return power_status;
}

void fan_init(void)
{
	fan_speed = FAN_SPEED_AUTO;
}



void fan_pwm_start(void)
{
	PWMCON |= 0x80; //
}

void fan_pwm_stop(void)
{
	PWMCON &= 0x7f;
	FAN_PWM_PIN = 0;
}


/*
void set_fan_speed(unsigned char speed)
{
	
}
*/
void regulate_fan_speed(void)
//调整转, 掉用一次切换一次, 自动, 低, 中, 高依次循环
{
	if(++fan_speed> FAN_SPEED_HIGH) fan_speed = FAN_SPEED_AUTO;
	//led_display_mode();
}

unsigned char read_fan_speed(void)
{
	return fan_speed;
}

void power_on(void)
{
	power_status = POWER_ON_STATUS;
	FAN_POWER_PIN = 1;
	fan_pwm_start();
//	set_fan_speed(1);
	ion_on();
}

void power_off(void)
{
	
	power_status = POWER_OFF_STATUS;	
	FAN_POWER_PIN = 0;
	fan_pwm_stop();
	ion_off();
}



