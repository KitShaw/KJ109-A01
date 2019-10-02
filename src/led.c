
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
	P1VO |=  (1<<7);  //��P17�ڵ���ʾ�����������
	P2VO |= 0xff;     //P2�ڶ�����led����ʾ
	P0VO |= (1<<1) | (1<<0);  //P11, P01
	//s11-s21
	P3VO |= (1<<3) | (1<<4) |(1<<5) | (1<<6) | (1<<7);  //P33-P37LED����
	//c3, c4, c5, c6, c7	
	OTCON = 0x00;
	LEDRAM[11] = 0x08;  // ͯ��ͼ��
	LEDRAM[12] = 0x18;  //0x10 -P2.5  0x08-8H
	LEDRAM[13] = 0x18;  //0x10 - ��, 0x08 - 4H
	LEDRAM[14] = 0x18;  //0x10 - ��, 0x08 - 2H  
	//0x80 - ����ܰ�λ��С����,  0x40 ʮλ, 0x20 ��λ
	LEDRAM[15] = 0xf8;  //0x10 - ��, 0x08 - 1H
	//�����g��
	LEDRAM[16] = 0xf8;  //0x10 - IONͼ��, 0x08��޹����,   
	//�����F��
	LEDRAM[17] = 0xf8;   //0x10- ���� �� 0x08-�����Ӱ���
	//�����E��
	LEDRAM[18] = 0xf8;   //0x10- ���� , 0x08 - ģʽ����
	//�����D��
	LEDRAM[19] = 0xf8;  //���� ,  ͯ������
	//�����C��
	LEDRAM[20] = 0xf8;  //����, ��ʱ����
	//�����B��
	LEDRAM[21] = 0xf8;  //����, ��Դ����
	//�����A��

	led_display_bcd(8, HUNDRED_DIGIT);
	led_display_bcd(9, TEN_DIGIT);
	led_display_bcd(7, SINGLE_DIGIT);
}

void led_task(void)
//100ms����һ��
{
		led_display_ion();
		led_display_mode();
}

void led_display_ion(void)
{
	if(1 == ION_PIN)
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



