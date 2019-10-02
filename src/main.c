 //************************************************************
//  Copyright (c) �����п���˹�Ƽ����޹�˾
//	�ļ�����	: main.c
//	����		: 
//	ģ�鹦��	: 
//  ����������:
// 	�汾		: 
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

// KJ60F-A3��SC92F8372��Ƭ�����Լ��İ���, ��ԭ���İ�����ȸ���,LED�ĳ�6���ڸ���������һ��LED, 

//*****************ȫ�ֱ�����***************************************************

INT8U  Timercount = 0;			//��ʱ������
BOOL   TimerFlag_1ms = 0;		//��ʱ��1MS��־

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
*�������ƣ�void TimerInit(void) 
*�������ܣ���ʱ����ʼ��
*��ڲ�����void
*���ڲ�����void
**************************************************/
void TimerInit(void)
{
	TMCON |= 1;		//bit0: 0ΪFOSC/12,1ΪFOSO

	TMOD = TMOD&0xf0;		  			//���ö�ʱ0��������ʽ0
	TMOD = TMOD|0x00;
	TH0=31;  ///(8192-80)/256 = 31.6875;       			//1000*1=1000us	,1MS
	TL0=176;         //(8192-80)%256 = 176;
	//TH0 = 800;
	//TL0 = 800;
	TF0 = 0;						    //���жϱ�־
	TR0=0;								//�ض�ʱ��0
   	ET0=1;								//ʹ�ܶ�ʱ��0�ж�
	TR0=1;
}

/**************************************************
*�������ƣ�void timer0()interrupt 1 
*�������ܣ���ʱ���жϷ�����
*��ڲ�����void
*���ڲ�����void 
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
*�������ƣ�void  Sys_Init(void) 
*�������ܣ�ϵͳ��ʼ��
*��ڲ�����void
*���ڲ�����void  
**************************************************/
void  Sys_Init(void)
{	
	WDTCON  = 0x10;				    //1--1 -- 00    ��WDT,WDT��0,WDT 524.288ms���;��¼ʱ����Code Optionѡ��ENWDT
	//TK��Ӧ��IO����Ϊǿ�������1
	
	P1CON |= (1<<0) | (1<<4) | (1<<5);                //P10, P14, P15�Ǵ�����
	P3CON |= (1<<0) | (1<<1) | (1<<20);
	led_init();
	//display_init();
	dust_init();
	EA = 1;						//�����ж�	
    TimerInit(); 				//��ʱ����ʼ��
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
*�������ƣ�void main(void)								  
*�������ܣ�������
*��ڲ�����void
*���ڲ�����void  
**************************************************/
void main(void)
{			
	Sys_Init();
	
	//���ذ�����ʼ��
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
*�������ƣ�void timer1()interrupt 1 
*�������ܣ���ʱ���жϷ�����
*��ڲ�����void
*���ڲ�����void 
**************************************************/
//void timer1()interrupt 3
//{
//	TH0 = (8192-1000)/32;       			//2000*1/4us=500us
//	TL0 = (8192-1000)%32;	
//	TimerFlag_1ms = 1;	
//		P52=~P52;
//}








