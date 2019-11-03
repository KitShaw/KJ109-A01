
/*
 *	dust.c
 *  Kit Shaw
 *  2019/10/01
 */
#include "SC92F844X_C.h"
#include "data_type.h"
#include "dust.h"
#include "stdlib.h"
#include "fan.h"

xdata unsigned short dust_adc_value[DUST_SIZE];
xdata unsigned long dust_adc_mean;       //ƽ��ֵ
xdata unsigned short dust_display_value;  //
xdata unsigned short dust_last_display_value;
xdata unsigned char dust_chang_count; //��ϵ2�β�һ���ٱ仯
xdata unsigned char dust_ok_flag;            //adcת����ɱ�־
xdata unsigned char dust_index = 0;
xdata unsigned short dust_delay_count;   //PM25��ʱ��ʾ����
unsigned char dust_level;      //1��, 2��, 3��
//unsigned char dust_size ;
//AD15 P43, ��19��
void dust_init(void)
{	
	P5CON |= ((1<<4)| (1<<3)); //P54, P53���
	//ADCCFG1 |= 1<<7; //AIN15λacd�����
	adc_init(15);
	dust_adc_mean = 0;
	dust_last_display_value = 2;
}

//unsigned short read_dust_adc_value(void)
//{
//	return dust_adc_mean;
//}

unsigned short read_dust_display_value(void)
{
#ifndef DEBUG_FAN_RETURN_PULSE
	return dust_last_display_value;
#else
	return read_disp_fan_return_pulse();
#endif
}
//���3.7V,  ���0.625
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
	// ��ѹ = 0.0056 * PM2.5 + 0.6
	// PM.25�ĵ�λΪug/m3
	//PM2.5 = (��ѹ - 0.6 ) / 0.0056
	//PM2.5 = (��ѹ- 0.6) * 1000/56;
	dust_adc_mean = sum / i;
	dust_voltage = dust_adc_mean * 10000 * 5 /4096 ;
	//if(dust_voltage > 6000) dust_voltage -= 6000;
	//if(dust_voltage > 5500) dust_voltage -= 5500;
	//if(dust_voltage > 4500) dust_voltage -= 4500;
	if(dust_voltage > 3800) dust_voltage -= 3800;
	else dust_voltage = 0;
	dust_display_value = (dust_voltage / 56);  //20�ǲ���ֵ
	//dust_display_value = dust_adc_mean / 10;// * 5 * 133 / 4096;  //dust_adc_mean/4096 *5
	//dust_display_value = (unsigned short)((dust_adc_mean  * 5* 178 / 4096 - 60 * 178)  /100 );  //dust_adc_mean/4096 *5
}


void dust_task(void)
//1ms����һ��
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
		
		if(++dust_delay_count>200)
	{
		dust_delay_count = 0;
		//dust_last_display_value = dust_display_value;  //������ʾֵ
		if((dust_display_value + 20) < dust_last_display_value)
		{
			//if(dust_last_display_value>400);  //������ʾֵ)
			dust_last_display_value -= (rand()%10);  //������ʾֵ
		}
		else 
		{	
			if(dust_display_value > dust_last_display_value )
			{
				if((dust_display_value - dust_last_display_value)> 20)
				{
					dust_last_display_value = dust_display_value;  //������ʾֵ	
				}
			}
			else
			{//if( dust_last_display_value == 0) dust_last_display_value = (rand()%5 + 1);
			
				if(++dust_chang_count>=5)
				{
					dust_chang_count = 0;
					dust_last_display_value = dust_display_value;  //������ʾֵ
					if( dust_last_display_value == 0) dust_last_display_value = (rand()%5 + 1);
				}
			}
			//dust_last_display_value = dust_display_value;  //������ʾֵ
				
		/*
			if((dust_display_value - dust_last_display_value) > 20 )
			{
				dust_last_display_value = dust_display_value;  //������ʾֵ
				//if( dust_last_display_value == 0) dust_last_display_value = (rand()%5 + 1);
			}
			else if(dust_last_display_value != dust_display_value)
			{
				if(++dust_chang_count>=3)
				{
					dust_chang_count = 0;
					dust_last_display_value = dust_display_value;  //������ʾֵ
					if( dust_last_display_value == 0) dust_last_display_value = (rand()%5 + 1);
				}
			}
			*/
		}
		//if(dust_last_display_value>=153) dust_last_display_value = 153;
		if(dust_last_display_value>=999) dust_last_display_value = 999;
//		if( dust_last_display_value == 0) dust_last_display_value = (rand()%5 + 1);
		//dust_last_display_value = 90;
		if(dust_last_display_value <= 100) dust_level = DUST_LEVEL_EXCELLENT;  //С�ڵ���70Ϊ��
		else if((dust_last_display_value > 100) && (dust_last_display_value <= 200))dust_level = DUST_LEVEL_12; 
		else if((dust_last_display_value>200) && (dust_last_display_value <= 300)) dust_level = DUST_LEVEL_MEDIUM;  //С�ڵ���70Ϊ��
		//else if((dust_last_display_value>150) && (dust_last_display_value <= 300)) dust_level = DUST_LEVEL_23;  //
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
	//OP_CTM1 &= ~(1<<7); //�ο���ѹΪVDD = 5V
	
	IE = 0X40;        //??ADC??
}

void adc_start(void)
//����ADCת��
{
	ADCCON |= 0X40;   //����ADת��
}


void ADC_Interrupt(void) interrupt 6
{
	ADCCON &= ~(0X20);  //���жϱ�׼λ
	ADCCON &= ~0X40;   //ֹͣADת��
	dust_ok_flag = 1;
}



