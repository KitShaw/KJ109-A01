
/*
 *	dust.c
 *  Kit Shaw
 *  2019/10/01
 */
#include "SC92F844X_C.h"
#include "data_type.h"
#include "dust.h"

unsigned short dust_adc_value[10];
unsigned short dust_adc_mean;       //平均值
unsigned char dust_ok_flag;            //adc转换完成标志
unsigned char dust_index = 0;
//unsigned char dust_size ;
//AD15 P43, 第19脚
void dust_init(void)
{	
	P5CON |= ((1<<4)| (1<<3)); //P54, P53输出
	//ADCCFG1 |= 1<<7; //AIN15位acd输入口
	adc_init(15);
	dust_adc_mean = 0;
}

unsigned short read_dust_adc_value(void)
{
	return dust_adc_mean;
}

void dust_adc_mean_value(void)
{
	unsigned char i;
	unsigned short sum;
	sum = 0;
	for(i = 0; i<DUST_SIZE; i++)
	{
		sum += dust_adc_value[i];
	}
	
	dust_adc_mean /= i;
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



