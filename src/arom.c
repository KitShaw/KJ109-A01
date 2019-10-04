
/*
 * arom.c
 * Kit Shaw
 * 2019.10.4
 */
 
#include "SC92F844X_C.h"
#include "arom.h"

unsigned char arom_level;  
unsigned int arom_work_count;
void arom_init(void)
{
	P5CON |= 1<<5;  //P55���ģʽ
	arom_off();
	arom_level = 0;
}

void arom_task(void)
	// 1s����һ��
{
	switch(arom_level)
	{
		case 1://�͵�����10����ͣ10����
		 if(arom_work_count == 0)arom_on();
		 if(arom_work_count == 600)arom_off();
		 if(++arom_work_count>=1200) arom_work_count = 0;
		break;
		case 2://�е� : ����30����ͣ10����
			if(arom_work_count == 0)arom_on();
		 	if(arom_work_count == 1800)arom_off();
		 	if(++arom_work_count>=2400) arom_work_count = 0;
		break;
		case 3://�ߵ�: ��������
			arom_on();
		break;
		default:
			arom_off();
		break;
	}
}

void arom_on(void)
{
	AROM_PIN = 1;
}

void arom_off(void)
{
	AROM_PIN = 0;
}

void regulate_arom_level(void)
//����ת, ����һ���л�һ��, �Զ�, ��, ��, ������ѭ��
{
	if(++arom_level> 3) arom_level = 0;	
	arom_work_count = 0;
}

void set_arom_level(unsigned char level)
{
	arom_level = level;
}

unsigned char read_arom_level(void)
{
	return arom_level;
}



