
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
	// low ����1����ͣ12����
	// mid ����1����ͣ8����
	// high ����1����ͣ4����
{
	switch(arom_level)
	{
		case 1://�͵�����10����ͣ10����
		 if(arom_work_count == 0)arom_on();
		 if(arom_work_count == 60)arom_off();
		 if(++arom_work_count>=780) arom_work_count = 0;
		break;
		case 2://�е� : ����30����ͣ10����
			if(arom_work_count == 0)arom_on();
		 	if(arom_work_count == 60)arom_off();
		 	if(++arom_work_count>=540) arom_work_count = 0;
		break;
		case 3://�ߵ�: ��������
			if(arom_work_count == 0)arom_on();
		 	if(arom_work_count == 60)arom_off();
		 	if(++arom_work_count>=300) arom_work_count = 0;
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



