

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

unsigned int xdata PWMRD_40  _at_  0x740;
unsigned int xdata PWMRD_41  _at_  0x742;
unsigned int xdata PWMRD_42  _at_  0x744;
unsigned int xdata PWMRD_43  _at_  0x746;
unsigned int xdata PWMRD_50  _at_  0x748;
unsigned int xdata PWMRD_51  _at_  0x74A;
unsigned int xdata PWMRD_52  _at_  0x74C;
unsigned int xdata PWMRD_53  _at_  0x74E;


unsigned char power_status;
unsigned char fan_speed;  // 0����, 1����, 2��, 3��

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
	P0CON &= ~(1<<2);   //P02��������
	P0PH |= 1<<2;
	P1CON &= ~(1<<6);       //P16��������
	P1PH |= 1<<6;

	P4CON |= (1<<2) | (1<<4);     // p44, P42���
	P42 = 0;
	P44 = 0;

	//����������Ϊ200us��ռ�ձ�Ϊ50%��PWM����
	PWMCON = 99;           //�������õ�8λ,200us
    PWMCFG = 0xb0;           //7:����  5-4��ʱ��Դѡ�� 11:fHRC/8 = 2M  3-0���������ø�4λ	
	//PWMRD_Temp = 0x8000 | 50;     //PWM��ռ�ձȵ��ڼĴ�ֻ��д,���Կɽ��ñ���PWMRD_temp����дռ�ձȵ�ֵ
	//PWMRD_41 = PWMRD_Temp;
	PWMRD_42 = 0x8000 | 20;
	//ENP42 = 1;
}



void fan_pwm_start(void)
{
	PWMCON |= 0x80; //
}

void fan_pwm_stop(void)
{
	PWMCON &= 0x7f;
//	FAN_PWM_PIN = 0;
}


/*
void set_fan_speed(unsigned char speed)
{
	
}
*/
void regulate_fan_speed(void)
//����ת, ����һ���л�һ��, �Զ�, ��, ��, ������ѭ��
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
//	FAN_POWER_PIN = 1;
	//fan_pwm_start();
//	set_fan_speed(1);
	led_on();  //����ʾ
	ion_on();
}

void power_off(void)
{
	
	power_status = POWER_OFF_STATUS;	
	//FAN_POWER_PIN = 0;
	//fan_pwm_stop();
	ion_off();
	led_off();
}



