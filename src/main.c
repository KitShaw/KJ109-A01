 //************************************************************
//  Copyright (c) 厦门市凯杰斯科技有限公司
//	文件名称	: main.c
//	作者		: 
//	模块功能	: 
//  最后更正日期:
// 	版本		: 
//              :  
//*************************************************************
#include "inc\SC92F844X_C.h"
#include "inc\data_type.h"
#include "lib\SensorMethod.h"
#include "led.h"
#include "fan.h"
#include "key.h"
#include "eeprom.h"
#include "filter.h"
#include "beep.h"
//#include "IAP.h"
#include "ion.h"
#include "arom.h"
#include "dust.h"
#include "timing_off.h"
//#include "display.h"

// KJ60F-A3是SC92F8372单片机加自己的板子, 和原样的板子相比改了,LED改成6个口各单独控制一个LED, 

//*****************全局变量区***************************************************

INT8U  Timercount = 0;			//定时器计数
BOOL   TimerFlag_1ms = 0;		//定时器1MS标志

//unsigned char power_status = POWER_OFF_STATUS;
//unsigned char	led_level = 0;
//unsigned char speed_level = 0;
unsigned int fan_count_1s;
unsigned char task_1s_count;
unsigned char task_1s_flag;

unsigned char task_100ms_count;
unsigned char task_100ms_flag;
unsigned char task_10ms_count;
unsigned char task_10ms_flag;
unsigned char task_1ms_count;
unsigned char task_1ms_flag;
unsigned char dust_count;

void task_1ms(void);
void task_10ms(void);
void task_100ms(void);
void task_1s(void);
void timer1_start(void);




 /**************************************************
*函数名称：void TimerInit(void) 
*函数功能：定时器初始化
*入口参数：void
*出口参数：void
**************************************************/
void TimerInit(void)
{
	TMCON |= 0X02;
	TMOD |= 0x10;            
	TL1 = (65536 - 1600)%256;  
	TH1 = (65536 - 1600)/256; 
	TR1 = 0;
	ET1 = 1;//
	
	TMCON |= 0X04;    //------111 ;Timer0、Tiemr1和Tiemr2选择时钟Fsys
	//T2设置
	T2MOD = 0x00;
	T2CON = 0x00;	 //设置为16位重载寄存器
	RCAP2H = 250; //(65536-1600)/256;     //溢出时间：时钟为Fsys，则32000*（1/Fsys）=2ms;
	RCAP2L = 50; //(65536-1600)%256;
	TR2 = 0;
	ET2 = 1;//定时器2允许
	TR2 = 1;//打开定时器2	
	
	
	//T0设置
	TMOD |= 0x05;                 //0000 0001;Timer0设置工作方式1, 计数器工作方式
	//TL0 = (65536 - 1600)%256;    //溢出时间：时钟为Fsys，则16000*（1/Fsys）=1ms;
	//TH0 = (65536 - 1600)/256;
	//TR0 = 0;
	ET0 = 0;//关定时器0中断
	TR0 = 1;//打开定时器0

    //IP |= (1<<1) | (1<<5);      //定时1, 和0高优先级
    //IP &= ~((1<<1) | (1<<5));      //定时1, 和0高优先级
}

void timer1_start(void)
{
	TL1 = (65536 - 4480)%256;    //240s
	TH1 = (65536 - 4480)/256;    //236.9375
	TR1 = 1;
}
/**************************************************
*函数名称：void timer0()interrupt 1 
*函数功能：定时器中断服务函数
*入口参数：void
*出口参数：void 
**************************************************/
void timer0()interrupt 1
{
/*
	TL0 = 50; //(65536 - 1600)%256 = 192;    //溢出时间：时钟为Fsys，则16000*（1/Fsys）=1ms;
	TH0 = 250; //(65536 - 1600)/256 = 249;
	if( dust_count == 4) DUST_PIN = 1;
	else if(dust_count >= 100){
		DUST_PIN = 0; 
		dust_count = 0;
		timer1_start();		
	}
	dust_count++;
	if(++task_1ms_count >= 10) {task_1ms_flag = 1; task_1ms_count = 0;}
	*/
	//if(++fan_count_1s >= 10000) { fan_count_1s = 0; store_fan_return_pulse();}  //电机计数1秒的脉冲用
}


 void Timer2Int(void) interrupt 5
 	//定时时间100us
 {	
 	//RCAP2L = 50;
	//RCAP2H = 250;
	TF2 = 0;   //溢出清零
	//P52 = ~P52;
	if( dust_count == 4) DUST_PIN = 1;
	else if(dust_count >= 100){
		DUST_PIN = 0; 
		dust_count = 0;
		timer1_start();		
	}
	dust_count++;
	if(++task_1ms_count >= 10) {task_1ms_flag = 1; task_1ms_count = 0;}
	if(++fan_count_1s >= 10000)
	{ 
		fan_count_1s = 0; 
		//store_fan_return_pulse((TH0<<8) + TL0); 
		store_fan_return_pulse();		
	}  //电机计数1秒的脉冲用
	led_key_power_count();
 }

 /**************************************************
*函数名称：void  Sys_Init(void) 
*函数功能：系统初始化
*入口参数：void
*出口参数：void  
**************************************************/
void  Sys_Init(void)
{	
	unsigned long delay_count;
	WDTCON  = 0x10;				    //1--1 -- 00    开WDT,WDT清0,WDT 524.288ms溢出;烧录时，可Code Option选择ENWDT
	//TK对应的IO设置为强推挽输出1
	
	P1CON |= (1<<0) | (1<<4) | (1<<5);                //P10, P14, P15是触摸脚
	P3CON |= (1<<0) | (1<<1) | (1<<20);
	led_init();
	//display_init();
	dust_init();
	EA = 1;						//开总中断	
    TimerInit(); 				//定时器初始化
	fan_init();
	ion_init();
	arom_init();
	//触控按键初始化
	TouchKeyInit();	
	key_init();	
	eeprom_init();
	filter_init();
	timing_off_init();
	beep_init();
	led_all_on();
	for(delay_count = 0; delay_count <80000; delay_count++)
	{
		WDTCON  = 0x10;
	}
//	test_i2c();
	power_off();
	//power_on();
}
void task_1ms(void)
{
	//filter_task();
	key_task();	
	if(++task_10ms_count>=10) {task_10ms_count = 0; task_10ms_flag = 1;}
	//P52=~P52;
}

void task_10ms(void)
{
	//filter_task();
	
	Sys_Scan();		
	if(++task_100ms_count>=10){task_100ms_count = 0; task_100ms_flag = 1;}
	//P52=~P52;
	dust_task();
	fan_task();
	beep_task();
	led_key_power();
}

void task_100ms(void)
{
	if(++task_1s_count>=10){task_1s_count = 0; task_1s_flag = 1; }
	filter_task();
	led_task();
	//fan_task();
}

void task_1s(void)
{
	//filter_task();
	//P52=~P52;
	arom_task();
	timing_off_task();
}

/**************************************************
*函数名称：void main(void)								  
*函数功能：主函数
*入口参数：void
*出口参数：void  
**************************************************/
void main(void)
{			
	Sys_Init();
	//ion_init();
	//DUST_PWR_PIN = 0;
	while(1)
	{
	   WDTCON  = 0x10;	   
		  if(1 == task_1ms_flag) { task_1ms_flag = 0; task_1ms();}
			if(1 == task_10ms_flag) { task_10ms_flag = 0; task_10ms();}
			if(1 == task_100ms_flag) {task_100ms_flag = 0; task_100ms(); }
			if(1 == task_1s_flag) {task_1s_flag = 0; task_1s();}
			fan_handle();

			I2C_CLK = ~I2C_CLK;
	} 
}


/**************************************************
*函数名称：void timer1()interrupt 1 
*函数功能：定时器中断服务函数
*入口参数：void
*出口参数：void 
**************************************************/
void timer1()interrupt 3
//用来读取粉尘传感器的值
{
	TR1 = 0;
	adc_start();
	//P52 =~P52;
}








