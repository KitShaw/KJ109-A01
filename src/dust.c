
/*
 *	dust.c
 *  Kit Shaw
 *  2019/10/01
 */
#include "SC92F844X_C.h"
#include "data_type.h"
#include "dust.h"
#include "stdlib.h"

xdata unsigned short dust_adc_value[DUST_SIZE];
xdata unsigned long dust_adc_mean;       //平均值
xdata unsigned short dust_display_value;  //
xdata unsigned short dust_last_display_value;
xdata unsigned char dust_ok_flag;            //adc转换完成标志
xdata unsigned char dust_index = 0;
xdata unsigned char dust_delay_count;   //PM25延时显示计数
unsigned char dust_level;      //1优, 2中, 3差
//unsigned char dust_size ;
//AD15 P43, 第19脚
void dust_init(void)
{	
	P5CON |= ((1<<4)| (1<<3)); //P54, P53输出
	//ADCCFG1 |= 1<<7; //AIN15位acd输入口
	adc_init(15);
	dust_adc_mean = 0;
}

//unsigned short read_dust_adc_value(void)
//{
//	return dust_adc_mean;
//}

unsigned short read_dust_display_value(void)
{
//	if(++dust_delay_count>200)
//	{
//		dust_delay_count = 0;
//		//dust_last_display_value = dust_display_value;  //更新显示值
//		if((dust_display_value + 20) < dust_last_display_value)
//		{
//			dust_last_display_value -= 8;  //更新显示值
//		}
//		else 
//		{
//			dust_last_display_value = dust_display_value;  //更新显示值
//		}
//		
//		//return dust_display_value;
//	}
	//return dust_last_display_value;
	return read_disp_fan_return_pulse();
}
//最高3.7V,  最低0.625
void dust_adc_mean_value(void)
{
	unsigned char i;
	unsigned long sum;
	unsigned long dust_voltage;
	sum = 0;
	for(i = 0; i<DUST_SIZE; i++)
	{
		sum += (unsigned long)dust_adc_value[i];
	}
	// y = 0.0056x + 0.6
	// 电压 = 0.0056 * PM2.5 + 0.6
	// PM.25的单位为ug/m3
	//PM2.5 = (电压 - 0.6 ) / 0.0056
	//PM2.5 = (电压- 0.6) * 1000/56;
	dust_adc_mean = sum / i;
	dust_voltage = dust_adc_mean * 10000 * 5 /4096 ;
	if(dust_voltage > 6000) dust_voltage -= 6000;
	else dust_voltage = 0;
	dust_display_value = dust_voltage / 56;
	//dust_display_value = dust_adc_mean / 10;// * 5 * 133 / 4096;  //dust_adc_mean/4096 *5
	//dust_display_value = (unsigned short)((dust_adc_mean  * 5* 178 / 4096 - 60 * 178)  /100 );  //dust_adc_mean/4096 *5
}


void dust_task(void)
//1ms调用一次
{
		if(1 == dust_ok_flag)
		{
			dust_ok_flag = 0;
			dust_adc_value[dust_index] = (ADCVH<<4)+(ADCVL>>4);
			if(++dust_index > DUST_SIZE)
			{
				dust_adc_mean_value();
				
				dust_index = 0;
			}
		}
		
		if(++dust_delay_count>150)
	{
		dust_delay_count = 0;
		//dust_last_display_value = dust_display_value;  //更新显示值
		if((dust_display_value + 20) < dust_last_display_value)
		{
			dust_last_display_value -= (rand()%20);  //更新显示值
		}
		else 
		{
			dust_last_display_value = dust_display_value;  //更新显示值
		}
		if(dust_last_display_value>=999) dust_last_display_value = 999;
		if( dust_last_display_value == 0) dust_last_display_value = (rand()%5 + 1);
		if(dust_last_display_value <= 70) dust_level = DUST_LEVEL_EXCELLENT;  //小于等于70为优
		else if((dust_last_display_value>70) && (dust_last_display_value <= 150)) dust_level = DUST_LEVEL_MEDIUM;  //小于等于70为优
		else dust_level = DUST_LEVEL_BAD;
	}
}

unsigned char read_dust_level(void)
{
	return dust_level;
}

/*****************************************************
*????:void ADC_Init(uint Channel)
*????:ADC???
*????:void
*????:Channel
*****************************************************/
void adc_init(unsigned int channel)
{
	ADCCON = 0X80|channel;		//??ADC,ADC?????2M ,??Channel?ADC???
	if(channel<8)
	{
		ADCCFG0 = 1<<channel;   //??Channel?????
	}
	else
	{
		ADCCFG1 = 1<<(channel-8);   //??Channel?????
	}
	//OP_CTM1 &= ~(1<<7); //参考电压为VDD = 5V
	
	IE = 0X40;        //??ADC??
}

void adc_start(void)
//启动ADC转换
{
	ADCCON |= 0X40;   //启动AD转换
}


void ADC_Interrupt(void) interrupt 6
{
	ADCCON &= ~(0X20);  //清中断标准位
	ADCCON &= ~0X40;   //停止AD转换
	dust_ok_flag = 1;
}



