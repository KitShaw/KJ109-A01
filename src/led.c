
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
	LEDRAM[11] = 0x08;  // 童锁图标
	LEDRAM[12] = 0x18;  //0x10 -P2.5  0x08-8H
	LEDRAM[13] = 0x18;  //0x10 - 高, 0x08 - 4H
	LEDRAM[14] = 0x18;  //0x10 - 中, 0x08 - 2H  
	//0x80 - 数码管百位的小数点,  0x40 十位, 0x20 个位
	LEDRAM[15] = 0xf8;  //0x10 - 低, 0x08 - 1H
	//数码管g段
	LEDRAM[16] = 0xf8;  //0x10 - ION图标, 0x08香薰按键,   
	//数码管F段
	LEDRAM[17] = 0xf8;   //0x10- 高速 和 0x08-负离子按键
	//数码管E段
	LEDRAM[18] = 0xf8;   //0x10- 中速 , 0x08 - 模式按键
	//数码管D段
	LEDRAM[19] = 0xf8;  //低速 ,  童锁按键
	//数码管C段
	LEDRAM[20] = 0xf8;  //智能, 定时按键
	//数码管B段
	LEDRAM[21] = 0xf8;  //滤网, 电源按键
	//数码管A段

	led_display_bcd(8, HUNDRED_DIGIT);
	led_display_bcd(9, TEN_DIGIT);
	led_display_bcd(7, SINGLE_DIGIT);
}

void led_task(void)
//100ms调用一次
{
		led_display_ion();
		led_display_mode();
}

void led_display_ion(void)
{
	if(1 == ION_PIN)
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



