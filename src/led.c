
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
	P1VO |=  (1<<7);  //��P17�ڵ���ʾ�����������
	P2VO |= 0xff;     //P2�ڶ�����led����ʾ
	P0VO |= (1<<1) | (1<<0);  //P11, P01
	//s11-s21
	P3VO |= (1<<3) | (1<<4) |(1<<5) | (1<<6) | (1<<7);  //P33-P37LED����
	//c3, c4, c5, c6, c7	
	OTCON = 0x00;

	LEDRAM[16] |= 0x08;  //0x10 - IONͼ��, 0x08��޹����,   
	//�����F��
	LEDRAM[17] |= 0x08;   //0x10- ���� �� 0x08-�����Ӱ���
	//�����E��
	LEDRAM[18] |= 0x08;   //0x10- ���� , 0x08 - ģʽ����
	//�����D��
	LEDRAM[19] |= 0x08;  //���� ,  ͯ������
	//�����C��
	LEDRAM[20] |= 0x08;  //����, ��ʱ����
	//�����B��
	LEDRAM[21] |= 0x08;  //����, ��Դ����
}

void led_task(void)
//100ms����һ��
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
	if( 1 == filter_flag) LEDRAM[21] |= 0x10;  //����, ��Դ����
	else LEDRAM[21] &= ~0x10;  //����, ��Դ����
}

void led_off(void)
{
	//DDRCON &= ~0x80;
	LED_GREEN = 1;
	LED_BLUE = 1;
	LED_RED = 1;
	
	LEDRAM[11] &= ~0x08;  // ͯ��ͼ��
	LEDRAM[12] &= ~0x18;  //0x10 -P2.5  0x08-8H
	LEDRAM[13] &= ~0x18;  //0x10 - ��, 0x08 - 4H
	LEDRAM[14] &= ~0x18;  //0x10 - ��, 0x08 - 2H  
	//0x80 - ����ܰ�λ��С����,  0x40 ʮλ, 0x20 ��λ
	LEDRAM[15] &= ~0xf8;  //0x10 - ��, 0x08 - 1H
	//�����g��
	LEDRAM[16] &= ~0xf0;  //0x10 - IONͼ��, 0x08��޹����,   
	//�����F��
	LEDRAM[17] &= ~0xf0;   //0x10- ���� �� 0x08-�����Ӱ���
	//�����E��
	LEDRAM[18] &= ~0xf0;   //0x10- ���� , 0x08 - ģʽ����
	//�����D��
	LEDRAM[19] &= ~0xf0;  //���� ,  ͯ������
	//�����C��
	LEDRAM[20] &= ~0xf0;  //����, ��ʱ����
	//�����B��
	LEDRAM[21] &= ~0xf0;  //����, ��Դ����
	//�����A��
}

void led_all_on(void)
{
	//DDRCON &= ~0x80;
	LED_GREEN = 0;
	LED_BLUE = 0;
	LED_RED = 0;
	
	LEDRAM[11] |= 0x08;  // ͯ��ͼ��
	LEDRAM[12] |= 0x18;  //0x10 -P2.5  0x08-8H
	LEDRAM[13] |= 0x18;  //0x10 - ��, 0x08 - 4H
	LEDRAM[14] |= 0x18;  //0x10 - ��, 0x08 - 2H  
	//0x80 - ����ܰ�λ��С����,  0x40 ʮλ, 0x20 ��λ
	LEDRAM[15] |= 0xf8;  //0x10 - ��, 0x08 - 1H
	//�����g��
	LEDRAM[16] |= 0xf8;;  //0x10 - IONͼ��, 0x08��޹����,   
	//�����F��
	LEDRAM[17] |= 0xf8;;   //0x10- ���� �� 0x08-�����Ӱ���
	//�����E��
	LEDRAM[18] |= 0xf8;;   //0x10- ���� , 0x08 - ģʽ����
	//�����D��
	LEDRAM[19] |= 0xf8;;  //���� ,  ͯ������
	//�����C��
	LEDRAM[20] |= 0xf8;;  //����, ��ʱ����
	//�����B��
	LEDRAM[21] |= 0xf8;;  //����, ��Դ����
	//�����A��
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
	LEDRAM[12] |= 0x10;  //PM2.5ͼ��
}

void led_display_timing_off_level(void)
{
	switch(read_timing_off_level())
	{
		//case 0:  //�ر�
		//	LEDRAM[13] &= ~0x10;  //0x10 - ��, 0x08 - 4H
		//	LEDRAM[14] &= ~0x10;  //0x10 - ��, 0x08 - 2H  
			//0x80 - ����ܰ�λ��С����,  0x40 ʮλ, 0x20 ��λ
		//	LEDRAM[15] &= ~0x10;  //0x10 - ��, 0x08 - 1H
		//break;
		case 1:  // 1H
			LEDRAM[12] &= ~0x08;  //0x10 -P2.5  0x08-8H
			LEDRAM[13] &= ~0x08;  //0x10 - ��, 0x08 - 4H
			LEDRAM[14] &= ~0x08;  //0x10 - ��, 0x08 - 2H  
			//0x80 - ����ܰ�λ��С����,  0x40 ʮλ, 0x20 ��λ
			LEDRAM[15] |= 0x08;  //0x10 - ��, 0x08 - 1H
		break;
		case 2:  // 2H
			LEDRAM[12] &= ~0x08;  //0x10 -P2.5  0x08-8H
			LEDRAM[13] &= ~0x08;  //0x10 - ��, 0x08 - 4H
			LEDRAM[14] |= 0x08;  //0x10 - ��, 0x08 - 2H  
			//0x80 - ����ܰ�λ��С����,  0x40 ʮλ, 0x20 ��λ
			LEDRAM[15] &= ~0x08;  //0x10 - ��, 0x08 - 1H
		break;
		case 3:  // 4H
			LEDRAM[12] &= ~0x08;  //0x10 -P2.5  0x08-8H
			LEDRAM[13] |= 0x08;  //0x10 - ��, 0x08 - 4H
			LEDRAM[14] &= ~0x08;  //0x10 - ��, 0x08 - 2H  
			//0x80 - ����ܰ�λ��С����,  0x40 ʮλ, 0x20 ��λ
			LEDRAM[15] &= ~0x08;  //0x10 - ��, 0x08 - 1H
		break;
		case 4:  // 8H
			LEDRAM[12] |= 0x08;  //0x10 -P2.5  0x08-8H
			LEDRAM[13] &= ~0x08;  //0x10 - ��, 0x08 - 4H
			LEDRAM[14] &= ~0x08;  //0x10 - ��, 0x08 - 2H  
			//0x80 - ����ܰ�λ��С����,  0x40 ʮλ, 0x20 ��λ
			LEDRAM[15] &= ~0x08;  //0x10 - ��, 0x08 - 1H
		break;
		default:
			LEDRAM[12] &= ~0x08;  //0x10 -P2.5  0x08-8H
			LEDRAM[13] &= ~0x08;  //0x10 - ��, 0x08 - 4H
			LEDRAM[14] &= ~0x08;  //0x10 - ��, 0x08 - 2H  
			//0x80 - ����ܰ�λ��С����,  0x40 ʮλ, 0x20 ��λ
			LEDRAM[15] &= ~0x08;  //0x10 - ��, 0x08 - 1H
		break;
	}
}

void led_display_arom_level(void)
{
	switch(read_arom_level())
	{
		//case 0:  //�ر�
		//	LEDRAM[13] &= ~0x10;  //0x10 - ��, 0x08 - 4H
		//	LEDRAM[14] &= ~0x10;  //0x10 - ��, 0x08 - 2H  
			//0x80 - ����ܰ�λ��С����,  0x40 ʮλ, 0x20 ��λ
		//	LEDRAM[15] &= ~0x10;  //0x10 - ��, 0x08 - 1H
		//break;
		case 1:  //��
			LEDRAM[13] &= ~0x10;  //0x10 - ��, 0x08 - 4H
			LEDRAM[14] &= ~0x10;  //0x10 - ��, 0x08 - 2H  
			//0x80 - ����ܰ�λ��С����,  0x40 ʮλ, 0x20 ��λ
			LEDRAM[15] |= 0x10;  //0x10 - ��, 0x08 - 1H
		break;
		case 2:  //��
			LEDRAM[13] &= ~0x10;  //0x10 - ��, 0x08 - 4H
			LEDRAM[14] |= 0x10;  //0x10 - ��, 0x08 - 2H  
			//0x80 - ����ܰ�λ��С����,  0x40 ʮλ, 0x20 ��λ
			LEDRAM[15] &= ~0x10;  //0x10 - ��, 0x08 - 1H
		break;
		case 3:  //��
			LEDRAM[13] |= 0x10;  //0x10 - ��, 0x08 - 4H
			LEDRAM[14] &= ~0x10;  //0x10 - ��, 0x08 - 2H  
			//0x80 - ����ܰ�λ��С����,  0x40 ʮλ, 0x20 ��λ
			LEDRAM[15] &= ~0x10;  //0x10 - ��, 0x08 - 1H
		break;
		default:
			LEDRAM[13] &= ~0x10;  //0x10 - ��, 0x08 - 4H
			LEDRAM[14] &= ~0x10;  //0x10 - ��, 0x08 - 2H  
			//0x80 - ����ܰ�λ��С����,  0x40 ʮλ, 0x20 ��λ
			LEDRAM[15] &= ~0x10;  //0x10 - ��, 0x08 - 1H
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
		LEDRAM[11] &= ~0x08;  // ͯ��ͼ��
	}
	else
	{
		LEDRAM[11] |= 0x08;  // ͯ��ͼ��
	}
}

void led_display_ion(void)
{
	if(0 == ION_PIN)
	{
		LEDRAM[16] |= 0x10;  //0x10 - IONͼ��, 0x08��޹����, 
	}
	else
	{
		LEDRAM[16] &= ~0x10;  //0x10 - IONͼ��, 0x08��޹����, 
	}
}

void led_display_mode(void)
	//����, �Ƶ�, ����, ��ǿ
{
	switch(read_fan_speed())
	{
		case FAN_SPEED_AUTO:
			LEDRAM[17] &= ~0x10;   //0x10- ���� �� 0x08-�����Ӱ���
			//�����E��
			LEDRAM[18] &= ~0x10;   //0x10- ���� , 0x08 - ģʽ����
			//�����D��
			LEDRAM[19] &= ~0x10;  //���� ,  ͯ������
			//�����C��
			LEDRAM[20] |= 0x10;  //����, ��ʱ����
		break;
		case FAN_SPEED_SLEEP:
			LEDRAM[17] &= ~0x10;   //0x10- ���� �� 0x08-�����Ӱ���
			//�����E��
			LEDRAM[18] &= ~0x10;   //0x10- ���� , 0x08 - ģʽ����
			//�����D��
			LEDRAM[19] |= 0x10;  //���� ,  ͯ������
			//�����C��
			LEDRAM[20] &= ~0x10;  //����, ��ʱ����
		break;
		case FAN_SPEED_MIDDLE:
			LEDRAM[17] &= ~0x10;   //0x10- ���� �� 0x08-�����Ӱ���
			//�����E��
			LEDRAM[18] |= 0x10;   //0x10- ���� , 0x08 - ģʽ����
			//�����D��
			LEDRAM[19] &= ~0x10;  //���� ,  ͯ������
			//�����C��
			LEDRAM[20] &= ~0x10;  //����, ��ʱ����
		break;
		case FAN_SPEED_HIGH:
			LEDRAM[17] |= 0x10;   //0x10- ���� �� 0x08-�����Ӱ���
			//�����E��
			LEDRAM[18] &= ~0x10;   //0x10- ���� , 0x08 - ģʽ����
			//�����D��
			LEDRAM[19] &= ~0x10;  //���� ,  ͯ������
			//�����C��
			LEDRAM[20] &= ~0x10;  //����, ��ʱ����
		break;
		default:
			LEDRAM[17] &= ~0x10;   //0x10- ���� �� 0x08-�����Ӱ���
			//�����E��
			LEDRAM[18] &= ~0x10;   //0x10- ���� , 0x08 - ģʽ����
			//�����D��
			LEDRAM[19] &= ~0x10;  //���� ,  ͯ������
			//�����C��
			LEDRAM[20] |= 0x10;  //����, ��ʱ����
		break;
	}
}

void led_display_bcd(unsigned char bcd_value, unsigned char digit)
//digit, ��7λ�ǰ�λ, 6λ��10λ, 5λ�Ǹ�λ
{
	switch(bcd_value)
	{
		case 0:
			LEDRAM[15] &= (~(1<<digit));
			//�����g��
			LEDRAM[16] |= (1<<digit);  //  
			//�����F��
			LEDRAM[17] |= (1<<digit);   //
			//�����E��
			LEDRAM[18] |= (1<<digit);   //
			//�����D��
			LEDRAM[19] |= (1<<digit);  //
			//�����C��
			LEDRAM[20] |= (1<<digit);  //
			//�����B��
			LEDRAM[21] |= (1<<digit);  //
			//�����A��
		break;
		case 1:
			LEDRAM[15] &= (~(1<<digit));
			//�����g��
			LEDRAM[16] &= (~(1<<digit));  //  
			//�����F��
			LEDRAM[17] &= (~(1<<digit));  //
			//�����E��
			LEDRAM[18] &= (~(1<<digit));  //
			//�����D��
			LEDRAM[19] |= (1<<digit);  //
			//�����C��
			LEDRAM[20] |= (1<<digit);  //
			//�����B��
			LEDRAM[21] &= (~(1<<digit));  //
			//�����A��
		break;
		case 2:
			LEDRAM[15] |= (1<<digit);
			//�����g��
			LEDRAM[16] &= (~(1<<digit));  //  
			//�����F��
			LEDRAM[17] |= (1<<digit);   //
			//�����E��
			LEDRAM[18] |= (1<<digit);   //
			//�����D��
			LEDRAM[19] &= ~(1<<digit);  //
			//�����C��
			LEDRAM[20] |= (1<<digit);  //
			//�����B��
			LEDRAM[21] |= (1<<digit);  //
			//�����A��
		break;
		case 3:
			LEDRAM[15] |= (1<<digit);
			//�����g��
			LEDRAM[16] &= (~(1<<digit)) ; //  
			//�����F��
			LEDRAM[17] &= ~(1<<digit);   //
			//�����E��
			LEDRAM[18] |= (1<<digit);   //
			//�����D��
			LEDRAM[19] |= (1<<digit);  //
			//�����C��
			LEDRAM[20] |= (1<<digit);  //
			//�����B��
			LEDRAM[21] |= (1<<digit);  //
			//�����A��
		break;
		case 4:
			LEDRAM[15] |= (1<<digit);
			//�����g��
			LEDRAM[16] |= (1<<digit);  //  
			//�����F��
			LEDRAM[17] &= ~(1<<digit);   //
			//�����E��
			LEDRAM[18] &= ~(1<<digit);   //
			//�����D��
			LEDRAM[19] |= (1<<digit);  //
			//�����C��
			LEDRAM[20] |= (1<<digit);  //
			//�����B��
			LEDRAM[21] &= ~(1<<digit);  //
			//�����A��
		break;
		case 5:
			LEDRAM[15] |= (1<<digit);
			//�����g��
			LEDRAM[16] |= (1<<digit);  //  
			//�����F��
			LEDRAM[17] &= ~(1<<digit);   //
			//�����E��
			LEDRAM[18] |= (1<<digit);   //
			//�����D��
			LEDRAM[19] |= (1<<digit);  //
			//�����C��
			LEDRAM[20] &= ~(1<<digit);  //
			//�����B��
			LEDRAM[21] |= (1<<digit);  //
			//�����A��
		break;
		case 6:
			LEDRAM[15] |= (1<<digit);
			//�����g��
			LEDRAM[16] |= (1<<digit);  //  
			//�����F��
			LEDRAM[17] |= (1<<digit);   //
			//�����E��
			LEDRAM[18] |= (1<<digit);   //
			//�����D��
			LEDRAM[19] |= (1<<digit);  //
			//�����C��
			LEDRAM[20] &= ~(1<<digit);  //
			//�����B��
			LEDRAM[21] |= (1<<digit);  //
			//�����A��
		break;
		case 7:
			LEDRAM[15] &= ~(1<<digit);
			//�����g��
			LEDRAM[16] &= ~(1<<digit);  //  
			//�����F��
			LEDRAM[17] &= ~(1<<digit);   //
			//�����E��
			LEDRAM[18] &= ~(1<<digit);   //
			//�����D��
			LEDRAM[19] |= (1<<digit);  //
			//�����C��
			LEDRAM[20] |= (1<<digit);  //
			//�����B��
			LEDRAM[21] |= (1<<digit);  //
			//�����A��
		break;
		case 8:
			LEDRAM[15] |= (1<<digit);
			//�����g��
			LEDRAM[16] |= (1<<digit);  //  
			//�����F��
			LEDRAM[17] |= (1<<digit);   //
			//�����E��
			LEDRAM[18] |= (1<<digit);   //
			//�����D��
			LEDRAM[19] |= (1<<digit);  //
			//�����C��
			LEDRAM[20] |= (1<<digit);  //
			//�����B��
			LEDRAM[21] |= (1<<digit);  //
			//�����A��
		break;
		case 9:
			LEDRAM[15] |= (1<<digit);
			//�����g��
			LEDRAM[16] |= (1<<digit);  //  
			//�����F��
			LEDRAM[17] &= ~(1<<digit);   //
			//�����E��
			LEDRAM[18] |= (1<<digit);   //
			//�����D��
			LEDRAM[19] |= (1<<digit);  //
			//�����C��
			LEDRAM[20] |= (1<<digit);  //
			//�����B��
			LEDRAM[21] |= (1<<digit);  //
			//�����A��
		break;
		default:
		break;				
	}
}



