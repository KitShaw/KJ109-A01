
/*
 *led.c
 */
 

#include "led.h"
#include "SC92F844X_C.h"
#include "data_type.h"
#include "fan.h"

bitval led_flag;
unsigned char night_light_level; //夜灯的档位

//sbit LED_POWER_R = P1^3;  //P1.1, P1.2 ,P1.3,  3个叫位都调试在用了
//sbit LED_POWER_W = P1^4;
//sbit LED_SPEED_W = P1^2;
//sbit LED_SPEED_R = P1^1;
//sbit led_com1 = P0^4;
//sbit led_com2 = P0^5;

void led_init(void)
{
	//P11, 12, 13, 14  P04 05  输出
	//P1PH &= 0xe1;
	P1PH |= 0x1e;
	P1 &= 0xe1; //P11,12, 13, 14输出低
	P1CON &= 0xe1;
	P1CON |= 0x1e; //P11, 12, 13, 14输出
	
	P1PH &= ~(1<<5);
	P1 &= ~(1<<5);
	P1CON |= (1<<5);
		
	P0PH |= ((1<<4) | (1<<5)); // P04 P05
	P0 |= ((1<<4) | (1<<5));
	P0CON &= 0x9f;
	P0CON |= 0x60;
	
	P0 |= ((1<<4) | (1<<5));
	P1 |= 0x1e; //P11,12, 13, 14输出高

	//P5PH |= ((1<<0) | (1<<1)| (1<<2)); // P04 P05
	
	P5CON |= ((1<<0) | (1<<1)| (1<<2)); //
	P5 &= ~((1<<0) | (1<<1)| (1<<2)); 
	P50 = 1;  //B
	P51 = 0; //G
	P52 = 1; //R
	
	//led_power_off();
}

void led_task(void)
//1ms调用一次
{
		static unsigned short over_voltage_flash_count;
		
		/*
		P04 = LED_NIGHT_LIGHT_INDICATOR_FLAG;
		P05 = LED_SLEEP_INDICATOR_FLAG;		
		P14 = LED_POWER_W_INDICATOR_FLAG;
		P12 = LED_SPEED_W_INDICATOR_FLAG;
		P11 = LED_SPEED_R_INDICATOR_FLAG;
		*/
		++over_voltage_flash_count;
	/*
		if(read_over_voltage_flag() == 1){			
				if(++over_voltage_flash_count>=250)
				{ 
					over_voltage_flash_count = 0;
					LED_OVER_VOLTAGE_FLASH_FLAG = ~LED_OVER_VOLTAGE_FLASH_FLAG;					
				}
		}
	*/
		if(1 == LED_SWITCH_FLAG)
	{
		LED_SWITCH_FLAG = 0;
		//P1 |= 0x1e;
		P04 = 1;//led_com1 = 1;
		P05 = 1;//led_com2 = 1;
		//P13 = LED_POWER_R_INDICATOR_FLAG;
		if(read_over_voltage_flag() == 1){
				if(over_voltage_flash_count >= 250)
				{ 
					over_voltage_flash_count = 0;
					LED_OVER_VOLTAGE_FLASH_FLAG = ~LED_OVER_VOLTAGE_FLASH_FLAG;					
				}
				if(LED_ON == LED_OVER_VOLTAGE_FLASH_FLAG){ led_on(3);}
				else {led_off(3);}
		}
		else
		{
			if(LED_ON == LED_POWER_R_INDICATOR_FLAG){ led_on(3);}
			else {led_off(3);}
		}
		if(LED_ON == LED_POWER_W_INDICATOR_FLAG){ led_on(4);}
			else {led_off(4);}
		//P14 = LED_POWER_W_INDICATOR_FLAG;
		P12 = LED_SPEED_W_INDICATOR_FLAG;
		P11 = LED_SPEED_R_INDICATOR_FLAG;
		P04 = 0;
	}
	else
	{
		LED_SWITCH_FLAG = 1;
		//P1 |= 0x1e;
		P04 = 1;//led_com1 = 1;
		P05 = 1;//led_com2 = 1;
		//P13 = LED_NIGHT_LIGHT_INDICATOR_FLAG;
		if(LED_ON == LED_NIGHT_LIGHT_INDICATOR_FLAG){ led_on(4);}
			else {led_off(4);}
		if(LED_ON == LED_SLEEP_INDICATOR_FLAG){ led_on(3);}
			else {led_off(3);}
		//P14 = LED_SLEEP_INDICATOR_FLAG;
		P12 = LED_OFF;
		P11 = LED_OFF;
		P05 = 0;		
		//P0CON 
	}
}

void led_on(unsigned char pin)
{
	//P1CON |= (1<<pin);
	//P1PH  &= ~(1<<pin);
	P1 &= ~(1<<pin);
}

void led_off(unsigned char pin)
{
	//P1CON &= ~(1<<pin);
	//P1PH  |= (1<<pin);
	P1 |= (1<<pin);
}

void led_filter_out_time(void)
{
	LED_SPEED_R_INDICATOR_FLAG = ~LED_SPEED_R_INDICATOR_FLAG;
}

void reset_led_flter_out_time(void)
{
	LED_SPEED_R_INDICATOR_FLAG = LED_OFF;
}

void led_key_speed(void)
//按下speed键的显示
{
	unsigned char speed_temp;
	speed_temp = read_fan_speed();
	switch(speed_temp)
	{
		case 0:  //静音
			LED_POWER_W_INDICATOR_FLAG = LED_OFF;
			LED_SPEED_W_INDICATOR_FLAG = LED_OFF;
		  LED_SLEEP_INDICATOR_FLAG	 = LED_ON;
			LED_NIGHT_LIGHT_INDICATOR_FLAG = LED_OFF;  
		break;
		case 1:  //低速
			LED_POWER_R_INDICATOR_FLAG = LED_OFF;
			LED_POWER_W_INDICATOR_FLAG = LED_ON;
			LED_SPEED_W_INDICATOR_FLAG = LED_OFF;
		  LED_SLEEP_INDICATOR_FLAG	 = LED_OFF;
		break;
		case 2:  //高速
			LED_POWER_W_INDICATOR_FLAG = LED_ON;
			LED_SPEED_W_INDICATOR_FLAG = LED_ON;
		  LED_SLEEP_INDICATOR_FLAG	 = LED_OFF;
		break;
	}
}


void set_night_light_level(unsigned char level)
{
	night_light_level = level;
}

void night_light_level_handle(void)
{
	if(++night_light_level>3) night_light_level = 0;
}

unsigned char read_night_light_level(void)
{
	return night_light_level;
}

void night_light_task(void)
//1ms调用一次
{
	static unsigned char night_light_count;
	if(++night_light_count>=8) night_light_count = 0;
	switch(night_light_level)
	{
		case 1:
			if(night_light_count<1) NIGHT_LIGHT_PIN = 1;
			else NIGHT_LIGHT_PIN = 0;
		break;
		case 2:
			if(night_light_count<4) NIGHT_LIGHT_PIN = 1;
			else NIGHT_LIGHT_PIN = 0;
		break;
		case 3:
			NIGHT_LIGHT_PIN = 1;
		break;
		default:
			NIGHT_LIGHT_PIN = 0;
		break;	
	}
	
}

void led_power_off(void)
//关机显示
{
	LED_POWER_W_INDICATOR_FLAG = LED_OFF;
	LED_POWER_R_INDICATOR_FLAG = LED_ON;
	LED_SPEED_R_INDICATOR_FLAG = LED_OFF;
	LED_SPEED_W_INDICATOR_FLAG = LED_OFF;
	LED_SLEEP_INDICATOR_FLAG	 = LED_OFF;
	LED_NIGHT_LIGHT_INDICATOR_FLAG	 = LED_OFF;
}

void led_power_on(void)
//开机显示
{
	LED_POWER_W_INDICATOR_FLAG = LED_ON;
	LED_POWER_R_INDICATOR_FLAG = LED_OFF;
	LED_SPEED_R_INDICATOR_FLAG = LED_OFF;
	LED_SPEED_W_INDICATOR_FLAG = LED_OFF;
	LED_SLEEP_INDICATOR_FLAG	 = LED_OFF;
	LED_NIGHT_LIGHT_INDICATOR_FLAG	 = LED_OFF;
}

void led_key_sleep(void)
{
	LED_POWER_W_INDICATOR_FLAG = LED_OFF;
	LED_POWER_R_INDICATOR_FLAG = LED_OFF;
	LED_SPEED_R_INDICATOR_FLAG = LED_OFF;
	LED_SPEED_W_INDICATOR_FLAG = LED_OFF;
	LED_SLEEP_INDICATOR_FLAG	 = LED_ON;
	LED_NIGHT_LIGHT_INDICATOR_FLAG	 = LED_OFF;
}

void led_key_night_light(void)
{
	if(read_night_light_level() == 0) LED_NIGHT_LIGHT_INDICATOR_FLAG	 = LED_OFF;
	else LED_NIGHT_LIGHT_INDICATOR_FLAG	 = LED_ON;
}


