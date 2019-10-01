

/*
 *motor.c
 */
 
#include "SC92F844X_C.h"
#include "led.h"
#include "fan.h"
#include "filter.h"
#include "IAP.h"
#include "ion.H"

unsigned char power_status;
unsigned char fan_speed;  // 1低档, 2高档, 0静音
bit over_voltage_flag;   //1电压超高, 0正常

void set_over_voltage_flag(bit flag)
{
	over_voltage_flag = flag;
}

bit read_over_voltage_flag(void)
{
	return over_voltage_flag;
}

void over_voltage_handle(void)
//电压超过15V.3就关机, 并且电源的红灯闪烁
{
	if(1 == OVER_VOLTAGE_PIN) 
	{
		set_over_voltage_flag(1);
		power_off();
	}
	else set_over_voltage_flag(0);
}

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



void set_fan_speed(unsigned char speed)
{
	
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
	set_fan_speed(1);
	ion_on();
}

void power_off(void)
{
	
	power_status = POWER_OFF_STATUS;	
	FAN_POWER_PIN = 0;
	fan_pwm_stop();
	ion_off();
}



