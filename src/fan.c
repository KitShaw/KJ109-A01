

/*
 *motor.c
 */
 
#include "SC92F837X_C.h"
#include "led.h"
#include "fan.h"
#include "filter.h"
#include "IAP.h"
#include "ion.H"

unsigned char power_status;
unsigned char fan_speed;  // 1�͵�, 2�ߵ�, 0����
bit over_voltage_flag;   //1��ѹ����, 0����

void set_over_voltage_flag(bit flag)
{
	over_voltage_flag = flag;
}

bit read_over_voltage_flag(void)
{
	return over_voltage_flag;
}

void over_voltage_handle(void)
//��ѹ����15V.3�͹ػ�, ���ҵ�Դ�ĺ����˸
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
	P0PH &= 0xfe;  //�ر�P00������
	P00 = 0;
	P0CON |= 0x01;  //P00���ģʽ
	PWMCFG &= 0x1e; // ʱ��FSys/2 PWM���������
	PWMCFG |= 0x60;   //Fsys/8
	PWMCON |= 0x01; //PWM�ź������P00;
	PWMPRD  = 59;	    //PWM??=(59+1)*(1/Fosc)
	P2PH &= 0xfd;
	P21 = 0;
	P2CON |= 0x02; //p21���ģʽ
	
	fan_pwm_start();
	fan_speed = 1;	
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
	fan_speed = speed;
	switch(fan_speed)
	{
		case 0:
			PWMDTY0 = 18;		//
		break;
		case 1:
			PWMDTY0 = 30;		//PWM0?Duty = 50/100 =1/2
		break;
		case 2:
			PWMDTY0 = 56;		//PWM0?Duty = 50/100 =1/2
		break;
	}
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
	led_power_on();
	ion_on();
}

void power_off(void)
{
	
	power_status = POWER_OFF_STATUS;	
	set_night_light_level(0);
	NIGHT_LIGHT_PIN = 0;
	FAN_POWER_PIN = 0;
	fan_pwm_stop();
	led_power_off();
	ion_off();
}



