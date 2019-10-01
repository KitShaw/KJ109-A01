
/*
 *	led.c
 *  Kit Shaw
 *  2019/10/01
 */
 

#include "led.h"
#include "SC92F844X_C.h"
#include "data_type.h"
#include "fan.h"

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
	LEDRAM[14] = 0x78;  //0x10 - 中, 0x08 - 2H  
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
}

void led_task(void)
//1ms调用一次
{
		
}



