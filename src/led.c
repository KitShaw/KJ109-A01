
/*
 *	led.c
 *  Kit Shaw
 *  2019/10/01
 */
 

#include "led.h"
#include "SC92F844X_C.h"
#include "data_type.h"
#include "fan.h"
#include "ion.h"
#include "key.h"
#include "dust.h"
#include "arom.h"
#include "timing_off.h"

unsigned char xdata LEDRAM[30] _at_ 0x700;

void led_init(void)
{	
	P5CON |= ((1<<0) | (1<<1)| (1<<2)); //
	P5 &= ~((1<<0) | (1<<1)| (1<<2)); 
	LED_RED = 1;
	LED_GREEN = 1;
	LED_BLUE = 0;	
	
	DDRCON |= 0xef;
	//IOHCON0 = 0xc0;
	//IOHCON1 = 0x00;
	
	//P1CON |= (1<<7);
	P1VO |=  (1<<7);  //打开P17口的显示驱动输出功能
	P2VO |= 0xff;     //P2口都用做led的显示
	P0VO |= (1<<1) | (1<<0);  //P11, P01
	//s11-s21
	P3VO |= (1<<3) | (1<<4) |(1<<5) | (1<<6) | (1<<7);  //P33-P37LED驱动
	//c3, c4, c5, c6, c7	
	OTCON = 0x00;

	LEDRAM[16] |= 0x08;  //0x10 - ION图标, 0x08香薰按键,   
	//数码管F段
	LEDRAM[17] |= 0x08;   //0x10- 高速 和 0x08-负离子按键
	//数码管E段
	LEDRAM[18] |= 0x08;   //0x10- 中速 , 0x08 - 模式按键
	//数码管D段
	LEDRAM[19] |= 0x08;  //低速 ,  童锁按键
	//数码管C段
	LEDRAM[20] |= 0x08;  //智能, 定时按键
	//数码管B段
	LEDRAM[21] |= 0x08;  //滤网, 电源按键
}

void led_task(void)
//100ms调用一次
{
		if(read_power_status() == POWER_OFF_STATUS)
		{			
			return;
		}
		led_display_ion();
		led_display_arom_level();
		led_display_mode();
		led_display_lock();
	led_display_pm25();
	led_display_dust_level();
	led_display_timing_off_level();
	
}


void led_display_filter_out(unsigned char filter_flag)
{
	if( 1 == filter_flag) LEDRAM[21] |= 0x10;  //滤网, 电源按键
	else LEDRAM[21] &= ~0x10;  //滤网, 电源按键
}

void led_off(void)
{
	//DDRCON &= ~0x80;
	LED_GREEN = 1;
	LED_BLUE = 1;
	LED_RED = 1;
	
	LEDRAM[11] &= ~0x08;  // 童锁图标
	LEDRAM[12] &= ~0x18;  //0x10 -P2.5  0x08-8H
	LEDRAM[13] &= ~0x18;  //0x10 - 高, 0x08 - 4H
	LEDRAM[14] &= ~0x18;  //0x10 - 中, 0x08 - 2H  
	//0x80 - 数码管百位的小数点,  0x40 十位, 0x20 个位
	LEDRAM[15] &= ~0xf8;  //0x10 - 低, 0x08 - 1H
	//数码管g段
	LEDRAM[16] &= ~0xf0;  //0x10 - ION图标, 0x08香薰按键,   
	//数码管F段
	LEDRAM[17] &= ~0xf0;   //0x10- 高速 和 0x08-负离子按键
	//数码管E段
	LEDRAM[18] &= ~0xf0;   //0x10- 中速 , 0x08 - 模式按键
	//数码管D段
	LEDRAM[19] &= ~0xf0;  //低速 ,  童锁按键
	//数码管C段
	LEDRAM[20] &= ~0xf0;  //智能, 定时按键
	//数码管B段
	LEDRAM[21] &= ~0xf0;  //滤网, 电源按键
	//数码管A段
}

void led_all_on(void)
{
	//DDRCON &= ~0x80;
	LED_GREEN = 0;
	LED_BLUE = 0;
	LED_RED = 0;
	
	LEDRAM[11] |= 0x08;  // 童锁图标
	LEDRAM[12] |= 0x18;  //0x10 -P2.5  0x08-8H
	LEDRAM[13] |= 0x18;  //0x10 - 高, 0x08 - 4H
	LEDRAM[14] |= 0x18;  //0x10 - 中, 0x08 - 2H  
	//0x80 - 数码管百位的小数点,  0x40 十位, 0x20 个位
	LEDRAM[15] |= 0xf8;  //0x10 - 低, 0x08 - 1H
	//数码管g段
	LEDRAM[16] |= 0xf8;;  //0x10 - ION图标, 0x08香薰按键,   
	//数码管F段
	LEDRAM[17] |= 0xf8;;   //0x10- 高速 和 0x08-负离子按键
	//数码管E段
	LEDRAM[18] |= 0xf8;;   //0x10- 中速 , 0x08 - 模式按键
	//数码管D段
	LEDRAM[19] |= 0xf8;;  //低速 ,  童锁按键
	//数码管C段
	LEDRAM[20] |= 0xf8;;  //智能, 定时按键
	//数码管B段
	LEDRAM[21] |= 0xf8;;  //滤网, 电源按键
	//数码管A段
}


void led_on(void)
{
	DDRCON |= 0x80;
}


void led_display_pm25(void)
{
	unsigned short tmp_dust_display_value;
	tmp_dust_display_value = read_dust_display_value();
	led_display_bcd(tmp_dust_display_value / 100, HUNDRED_DIGIT);
	led_display_bcd(tmp_dust_display_value % 100 / 10, TEN_DIGIT);
	led_display_bcd(tmp_dust_display_value % 10, SINGLE_DIGIT);
	LEDRAM[12] |= 0x10;  //PM2.5图标
}

void led_display_timing_off_level(void)
{
	switch(read_timing_off_level())
	{
		//case 0:  //关闭
		//	LEDRAM[13] &= ~0x10;  //0x10 - 高, 0x08 - 4H
		//	LEDRAM[14] &= ~0x10;  //0x10 - 中, 0x08 - 2H  
			//0x80 - 数码管百位的小数点,  0x40 十位, 0x20 个位
		//	LEDRAM[15] &= ~0x10;  //0x10 - 低, 0x08 - 1H
		//break;
		case 1:  // 1H
			LEDRAM[12] &= ~0x08;  //0x10 -P2.5  0x08-8H
			LEDRAM[13] &= ~0x08;  //0x10 - 高, 0x08 - 4H
			LEDRAM[14] &= ~0x08;  //0x10 - 中, 0x08 - 2H  
			//0x80 - 数码管百位的小数点,  0x40 十位, 0x20 个位
			LEDRAM[15] |= 0x08;  //0x10 - 低, 0x08 - 1H
		break;
		case 2:  // 2H
			LEDRAM[12] &= ~0x08;  //0x10 -P2.5  0x08-8H
			LEDRAM[13] &= ~0x08;  //0x10 - 高, 0x08 - 4H
			LEDRAM[14] |= 0x08;  //0x10 - 中, 0x08 - 2H  
			//0x80 - 数码管百位的小数点,  0x40 十位, 0x20 个位
			LEDRAM[15] &= ~0x08;  //0x10 - 低, 0x08 - 1H
		break;
		case 3:  // 4H
			LEDRAM[12] &= ~0x08;  //0x10 -P2.5  0x08-8H
			LEDRAM[13] |= 0x08;  //0x10 - 高, 0x08 - 4H
			LEDRAM[14] &= ~0x08;  //0x10 - 中, 0x08 - 2H  
			//0x80 - 数码管百位的小数点,  0x40 十位, 0x20 个位
			LEDRAM[15] &= ~0x08;  //0x10 - 低, 0x08 - 1H
		break;
		case 4:  // 8H
			LEDRAM[12] |= 0x08;  //0x10 -P2.5  0x08-8H
			LEDRAM[13] &= ~0x08;  //0x10 - 高, 0x08 - 4H
			LEDRAM[14] &= ~0x08;  //0x10 - 中, 0x08 - 2H  
			//0x80 - 数码管百位的小数点,  0x40 十位, 0x20 个位
			LEDRAM[15] &= ~0x08;  //0x10 - 低, 0x08 - 1H
		break;
		default:
			LEDRAM[12] &= ~0x08;  //0x10 -P2.5  0x08-8H
			LEDRAM[13] &= ~0x08;  //0x10 - 高, 0x08 - 4H
			LEDRAM[14] &= ~0x08;  //0x10 - 中, 0x08 - 2H  
			//0x80 - 数码管百位的小数点,  0x40 十位, 0x20 个位
			LEDRAM[15] &= ~0x08;  //0x10 - 低, 0x08 - 1H
		break;
	}
}

void led_display_arom_level(void)
{
	switch(read_arom_level())
	{
		//case 0:  //关闭
		//	LEDRAM[13] &= ~0x10;  //0x10 - 高, 0x08 - 4H
		//	LEDRAM[14] &= ~0x10;  //0x10 - 中, 0x08 - 2H  
			//0x80 - 数码管百位的小数点,  0x40 十位, 0x20 个位
		//	LEDRAM[15] &= ~0x10;  //0x10 - 低, 0x08 - 1H
		//break;
		case 1:  //低
			LEDRAM[13] &= ~0x10;  //0x10 - 高, 0x08 - 4H
			LEDRAM[14] &= ~0x10;  //0x10 - 中, 0x08 - 2H  
			//0x80 - 数码管百位的小数点,  0x40 十位, 0x20 个位
			LEDRAM[15] |= 0x10;  //0x10 - 低, 0x08 - 1H
		break;
		case 2:  //中
			LEDRAM[13] &= ~0x10;  //0x10 - 高, 0x08 - 4H
			LEDRAM[14] |= 0x10;  //0x10 - 中, 0x08 - 2H  
			//0x80 - 数码管百位的小数点,  0x40 十位, 0x20 个位
			LEDRAM[15] &= ~0x10;  //0x10 - 低, 0x08 - 1H
		break;
		case 3:  //高
			LEDRAM[13] |= 0x10;  //0x10 - 高, 0x08 - 4H
			LEDRAM[14] &= ~0x10;  //0x10 - 中, 0x08 - 2H  
			//0x80 - 数码管百位的小数点,  0x40 十位, 0x20 个位
			LEDRAM[15] &= ~0x10;  //0x10 - 低, 0x08 - 1H
		break;
		default:
			LEDRAM[13] &= ~0x10;  //0x10 - 高, 0x08 - 4H
			LEDRAM[14] &= ~0x10;  //0x10 - 中, 0x08 - 2H  
			//0x80 - 数码管百位的小数点,  0x40 十位, 0x20 个位
			LEDRAM[15] &= ~0x10;  //0x10 - 低, 0x08 - 1H
		break;
	}
}

void led_display_dust_level(void)
{
	switch(read_dust_level())
	{
		case DUST_LEVEL_EXCELLENT:
			LED_GREEN = 0;
			LED_BLUE = 1;
			LED_RED = 1;
		break;
		case DUST_LEVEL_12:
			LED_GREEN = 0;
			LED_BLUE = 0;
			LED_RED = 1;
		break;
		case DUST_LEVEL_MEDIUM:
			LED_GREEN = 0;
			LED_BLUE = 1;
			LED_RED = 0;
		break;
		/*
		case DUST_LEVEL_23:
			LED_GREEN = 1;
			LED_BLUE = 1;
			LED_RED = 0;
		break;
		*/
		case DUST_LEVEL_BAD:
			LED_GREEN = 1;
			LED_BLUE = 1;
			LED_RED = 0;
		break;
		default:
			LED_GREEN = 0;
			LED_BLUE = 1;
			LED_RED = 1;
		break;
	}
}

void led_display_lock(void)
{
	if( 0 == read_unlock_flag())
	{
		LEDRAM[11] &= ~0x08;  // 童锁图标
	}
	else
	{
		LEDRAM[11] |= 0x08;  // 童锁图标
	}
}

void led_display_ion(void)
{
	if(0 == ION_PIN)
	{
		LEDRAM[16] |= 0x10;  //0x10 - ION图标, 0x08香薰按键, 
	}
	else
	{
		LEDRAM[16] &= ~0x10;  //0x10 - ION图标, 0x08香薰按键, 
	}
}

void led_display_mode(void)
	//智能, 酒店, 中速, 超强
{
	switch(read_fan_speed())
	{
		case FAN_SPEED_AUTO:
			LEDRAM[17] &= ~0x10;   //0x10- 高速 和 0x08-负离子按键
			//数码管E段
			LEDRAM[18] &= ~0x10;   //0x10- 中速 , 0x08 - 模式按键
			//数码管D段
			LEDRAM[19] &= ~0x10;  //低速 ,  童锁按键
			//数码管C段
			LEDRAM[20] |= 0x10;  //智能, 定时按键
		break;
		case FAN_SPEED_SLEEP:
			LEDRAM[17] &= ~0x10;   //0x10- 高速 和 0x08-负离子按键
			//数码管E段
			LEDRAM[18] &= ~0x10;   //0x10- 中速 , 0x08 - 模式按键
			//数码管D段
			LEDRAM[19] |= 0x10;  //低速 ,  童锁按键
			//数码管C段
			LEDRAM[20] &= ~0x10;  //智能, 定时按键
		break;
		case FAN_SPEED_MIDDLE:
			LEDRAM[17] &= ~0x10;   //0x10- 高速 和 0x08-负离子按键
			//数码管E段
			LEDRAM[18] |= 0x10;   //0x10- 中速 , 0x08 - 模式按键
			//数码管D段
			LEDRAM[19] &= ~0x10;  //低速 ,  童锁按键
			//数码管C段
			LEDRAM[20] &= ~0x10;  //智能, 定时按键
		break;
		case FAN_SPEED_HIGH:
			LEDRAM[17] |= 0x10;   //0x10- 高速 和 0x08-负离子按键
			//数码管E段
			LEDRAM[18] &= ~0x10;   //0x10- 中速 , 0x08 - 模式按键
			//数码管D段
			LEDRAM[19] &= ~0x10;  //低速 ,  童锁按键
			//数码管C段
			LEDRAM[20] &= ~0x10;  //智能, 定时按键
		break;
		default:
			LEDRAM[17] &= ~0x10;   //0x10- 高速 和 0x08-负离子按键
			//数码管E段
			LEDRAM[18] &= ~0x10;   //0x10- 中速 , 0x08 - 模式按键
			//数码管D段
			LEDRAM[19] &= ~0x10;  //低速 ,  童锁按键
			//数码管C段
			LEDRAM[20] |= 0x10;  //智能, 定时按键
		break;
	}
}

void led_display_bcd(unsigned char bcd_value, unsigned char digit)
//digit, 第7位是百位, 6位是10位, 5位是个位
{
	switch(bcd_value)
	{
		case 0:
			LEDRAM[15] &= (~(1<<digit));
			//数码管g段
			LEDRAM[16] |= (1<<digit);  //  
			//数码管F段
			LEDRAM[17] |= (1<<digit);   //
			//数码管E段
			LEDRAM[18] |= (1<<digit);   //
			//数码管D段
			LEDRAM[19] |= (1<<digit);  //
			//数码管C段
			LEDRAM[20] |= (1<<digit);  //
			//数码管B段
			LEDRAM[21] |= (1<<digit);  //
			//数码管A段
		break;
		case 1:
			LEDRAM[15] &= (~(1<<digit));
			//数码管g段
			LEDRAM[16] &= (~(1<<digit));  //  
			//数码管F段
			LEDRAM[17] &= (~(1<<digit));  //
			//数码管E段
			LEDRAM[18] &= (~(1<<digit));  //
			//数码管D段
			LEDRAM[19] |= (1<<digit);  //
			//数码管C段
			LEDRAM[20] |= (1<<digit);  //
			//数码管B段
			LEDRAM[21] &= (~(1<<digit));  //
			//数码管A段
		break;
		case 2:
			LEDRAM[15] |= (1<<digit);
			//数码管g段
			LEDRAM[16] &= (~(1<<digit));  //  
			//数码管F段
			LEDRAM[17] |= (1<<digit);   //
			//数码管E段
			LEDRAM[18] |= (1<<digit);   //
			//数码管D段
			LEDRAM[19] &= ~(1<<digit);  //
			//数码管C段
			LEDRAM[20] |= (1<<digit);  //
			//数码管B段
			LEDRAM[21] |= (1<<digit);  //
			//数码管A段
		break;
		case 3:
			LEDRAM[15] |= (1<<digit);
			//数码管g段
			LEDRAM[16] &= (~(1<<digit)) ; //  
			//数码管F段
			LEDRAM[17] &= ~(1<<digit);   //
			//数码管E段
			LEDRAM[18] |= (1<<digit);   //
			//数码管D段
			LEDRAM[19] |= (1<<digit);  //
			//数码管C段
			LEDRAM[20] |= (1<<digit);  //
			//数码管B段
			LEDRAM[21] |= (1<<digit);  //
			//数码管A段
		break;
		case 4:
			LEDRAM[15] |= (1<<digit);
			//数码管g段
			LEDRAM[16] |= (1<<digit);  //  
			//数码管F段
			LEDRAM[17] &= ~(1<<digit);   //
			//数码管E段
			LEDRAM[18] &= ~(1<<digit);   //
			//数码管D段
			LEDRAM[19] |= (1<<digit);  //
			//数码管C段
			LEDRAM[20] |= (1<<digit);  //
			//数码管B段
			LEDRAM[21] &= ~(1<<digit);  //
			//数码管A段
		break;
		case 5:
			LEDRAM[15] |= (1<<digit);
			//数码管g段
			LEDRAM[16] |= (1<<digit);  //  
			//数码管F段
			LEDRAM[17] &= ~(1<<digit);   //
			//数码管E段
			LEDRAM[18] |= (1<<digit);   //
			//数码管D段
			LEDRAM[19] |= (1<<digit);  //
			//数码管C段
			LEDRAM[20] &= ~(1<<digit);  //
			//数码管B段
			LEDRAM[21] |= (1<<digit);  //
			//数码管A段
		break;
		case 6:
			LEDRAM[15] |= (1<<digit);
			//数码管g段
			LEDRAM[16] |= (1<<digit);  //  
			//数码管F段
			LEDRAM[17] |= (1<<digit);   //
			//数码管E段
			LEDRAM[18] |= (1<<digit);   //
			//数码管D段
			LEDRAM[19] |= (1<<digit);  //
			//数码管C段
			LEDRAM[20] &= ~(1<<digit);  //
			//数码管B段
			LEDRAM[21] |= (1<<digit);  //
			//数码管A段
		break;
		case 7:
			LEDRAM[15] &= ~(1<<digit);
			//数码管g段
			LEDRAM[16] &= ~(1<<digit);  //  
			//数码管F段
			LEDRAM[17] &= ~(1<<digit);   //
			//数码管E段
			LEDRAM[18] &= ~(1<<digit);   //
			//数码管D段
			LEDRAM[19] |= (1<<digit);  //
			//数码管C段
			LEDRAM[20] |= (1<<digit);  //
			//数码管B段
			LEDRAM[21] |= (1<<digit);  //
			//数码管A段
		break;
		case 8:
			LEDRAM[15] |= (1<<digit);
			//数码管g段
			LEDRAM[16] |= (1<<digit);  //  
			//数码管F段
			LEDRAM[17] |= (1<<digit);   //
			//数码管E段
			LEDRAM[18] |= (1<<digit);   //
			//数码管D段
			LEDRAM[19] |= (1<<digit);  //
			//数码管C段
			LEDRAM[20] |= (1<<digit);  //
			//数码管B段
			LEDRAM[21] |= (1<<digit);  //
			//数码管A段
		break;
		case 9:
			LEDRAM[15] |= (1<<digit);
			//数码管g段
			LEDRAM[16] |= (1<<digit);  //  
			//数码管F段
			LEDRAM[17] &= ~(1<<digit);   //
			//数码管E段
			LEDRAM[18] |= (1<<digit);   //
			//数码管D段
			LEDRAM[19] |= (1<<digit);  //
			//数码管C段
			LEDRAM[20] |= (1<<digit);  //
			//数码管B段
			LEDRAM[21] |= (1<<digit);  //
			//数码管A段
		break;
		default:
		break;				
	}
}



