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
//#include "IAP.h"
#include "ion.h"
#include "dust.h"
//#include "display.h"

// KJ60F-A3是SC92F8372单片机加自己的板子, 和原样的板子相比改了,LED改成6个口各单独控制一个LED, 

//*****************全局变量区***************************************************

INT8U  Timercount = 0;			//定时器计数
BOOL   TimerFlag_1ms = 0;		//定时器1MS标志

//unsigned char power_status = POWER_OFF_STATUS;
unsigned char	led_level = 0;
unsigned char speed_level = 0;
unsigned char task_1s_count;
unsigned char task_1s_flag;

unsigned char task_100ms_count;
unsigned char task_100ms_flag;
unsigned char task_10ms_count;
unsigned char task_10ms_flag;
unsigned char task_1ms_count;
unsigned char task_1ms_flag;

void task_1ms(void);
void task_10ms(void);
void task_100ms(void);
void task_1s(void);



 /**************************************************
*函数名称：void TimerInit(void) 
*函数功能：定时器初始化
*入口参数：void
*出口参数：void
**************************************************/
void TimerInit(void)
{
	TMCON |= 1;		//bit0: 0为FOSC/12,1为FOSO

	TMOD = TMOD&0xf0;		  			//设置定时0，工作方式0
	TMOD = TMOD|0x00;
	TH0=31;  ///(8192-80)/256 = 31.6875;       			//1000*1=1000us	,1MS
	TL0=176;         //(8192-80)%256 = 176;
	//TH0 = 800;
	//TL0 = 800;
	TF0 = 0;						    //清中断标志
	TR0=0;								//关定时器0
   	ET0=1;								//使能定时器0中断
	TR0=1;
}

/**************************************************
*函数名称：void timer0()interrupt 1 
*函数功能：定时器中断服务函数
*入口参数：void
*出口参数：void 
**************************************************/
void timer0()interrupt 1
{
	//TH0 = (8192-1000)/32;       			//2000*1/4us=500us
	//TL0 = (8192-1000)%32;	
	TH0=31;//31;             //(8192-80)/256;       			//1000*1=1000us	,1MS
	TL0=176;                //(8192-80)%256;
//	TimerFlag_1ms = 1;	
//		P52=~P52;
	
	task_1ms_flag = 1;
}
 /**************************************************
*函数名称：void  Sys_Init(void) 
*函数功能：系统初始化
*入口参数：void
*出口参数：void  
**************************************************/
void  Sys_Init(void)
{	
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
	power_off();
}
void task_1ms(void)
{
	//filter_task();
	key_task();	
	if(++task_10ms_count>=9) {task_10ms_count = 0; task_10ms_flag = 1;}
}

void task_10ms(void)
{
	//filter_task();
	
	Sys_Scan();		
	led_task();
	if(++task_100ms_count>=10){task_100ms_count = 0; task_100ms_flag = 1;}
	P52=~P52;
}

void task_100ms(void)
{
	if(++task_1s_count>=10){task_1s_count = 0; task_1s_flag = 1; }
	filter_task();
	dust_task();
	
}

void task_1s(void)
{
	//filter_task();
	//P52=~P52;
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
	
	//触控按键初始化
	TouchKeyInit();	
	key_init();
	//fan_init();
	eeprom_init();

	filter_init();
	//ion_init();
	
	while(1)
	{
	   WDTCON  = 0x10;	   
		  if(1 == task_1ms_flag) { task_1ms_flag = 0; task_1ms();}
			if(1 == task_10ms_flag) { task_10ms_flag = 0; task_10ms();}
			if(1 == task_100ms_flag) {task_100ms_flag = 0; task_100ms(); }
			if(1 == task_1s_flag) {task_1s_flag = 0; task_1s();}
	} 
}


/**************************************************
*函数名称：void timer1()interrupt 1 
*函数功能：定时器中断服务函数
*入口参数：void
*出口参数：void 
**************************************************/
//void timer1()interrupt 3
//{
//	TH0 = (8192-1000)/32;       			//2000*1/4us=500us
//	TL0 = (8192-1000)%32;	
//	TimerFlag_1ms = 1;	
//		P52=~P52;
//}








