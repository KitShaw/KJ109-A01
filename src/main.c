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
#include "beep.h"
//#include "IAP.h"
#include "ion.h"
#include "arom.h"
#include "dust.h"
#include "timing_off.h"
//#include "display.h"

// KJ60F-A3��SC92F8372��Ƭ�����Լ��İ���, ��ԭ���İ�����ȸ���,LED�ĳ�6���ڸ���������һ��LED, 

//*****************ȫ�ֱ�����***************************************************

INT8U  Timercount = 0;			//��ʱ������
BOOL   TimerFlag_1ms = 0;		//��ʱ��1MS��־

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
*�������ƣ�void TimerInit(void) 
*�������ܣ���ʱ����ʼ��
*��ڲ�����void
*���ڲ�����void
**************************************************/
void TimerInit(void)
{
	TMCON |= 0X02;
	TMOD |= 0x10;            
	TL1 = (65536 - 1600)%256;  
	TH1 = (65536 - 1600)/256; 
	TR1 = 0;
	ET1 = 1;//
	
	TMCON |= 0X04;    //------111 ;Timer0��Tiemr1��Tiemr2ѡ��ʱ��Fsys
	//T2����
	T2MOD = 0x00;
	T2CON = 0x00;	 //����Ϊ16λ���ؼĴ���
	RCAP2H = 250; //(65536-1600)/256;     //���ʱ�䣺ʱ��ΪFsys����32000*��1/Fsys��=2ms;
	RCAP2L = 50; //(65536-1600)%256;
	TR2 = 0;
	ET2 = 1;//��ʱ��2����
	TR2 = 1;//�򿪶�ʱ��2	
	
	
	//T0����
	TMOD |= 0x05;                 //0000 0001;Timer0���ù�����ʽ1, ������������ʽ
	//TL0 = (65536 - 1600)%256;    //���ʱ�䣺ʱ��ΪFsys����16000*��1/Fsys��=1ms;
	//TH0 = (65536 - 1600)/256;
	//TR0 = 0;
	ET0 = 0;//�ض�ʱ��0�ж�
	TR0 = 1;//�򿪶�ʱ��0

    //IP |= (1<<1) | (1<<5);      //��ʱ1, ��0�����ȼ�
    //IP &= ~((1<<1) | (1<<5));      //��ʱ1, ��0�����ȼ�
}

void timer1_start(void)
{
	TL1 = (65536 - 4480)%256;    //240s
	TH1 = (65536 - 4480)/256;    //236.9375
	TR1 = 1;
}
/**************************************************
*�������ƣ�void timer0()interrupt 1 
*�������ܣ���ʱ���жϷ�����
*��ڲ�����void
*���ڲ�����void 
**************************************************/
void timer0()interrupt 1
{
/*
	TL0 = 50; //(65536 - 1600)%256 = 192;    //���ʱ�䣺ʱ��ΪFsys����16000*��1/Fsys��=1ms;
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
	//if(++fan_count_1s >= 10000) { fan_count_1s = 0; store_fan_return_pulse();}  //�������1���������
}


 void Timer2Int(void) interrupt 5
 	//��ʱʱ��100us
 {	
 	//RCAP2L = 50;
	//RCAP2H = 250;
	TF2 = 0;   //�������
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
	}  //�������1���������
	led_key_power_count();
 }

 /**************************************************
*�������ƣ�void  Sys_Init(void) 
*�������ܣ�ϵͳ��ʼ��
*��ڲ�����void
*���ڲ�����void  
**************************************************/
void  Sys_Init(void)
{	
	unsigned long delay_count;
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
	arom_init();
	//���ذ�����ʼ��
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
*�������ƣ�void main(void)								  
*�������ܣ�������
*��ڲ�����void
*���ڲ�����void  
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
*�������ƣ�void timer1()interrupt 1 
*�������ܣ���ʱ���жϷ�����
*��ڲ�����void
*���ڲ�����void 
**************************************************/
void timer1()interrupt 3
//������ȡ�۳���������ֵ
{
	TR1 = 0;
	adc_start();
	//P52 =~P52;
}








