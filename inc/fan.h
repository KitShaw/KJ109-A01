/*
 *motor.h
 */
 
 
#ifndef __FAN_H_
#define __FAN_H_

#include "data_type.h"
#define FAN_SPEED_AUTO  0
#define FAN_SPEED_SLEEP 1
#define FAN_SPEED_MIDDLE 2
#define FAN_SPEED_HIGH	3

#define POWER_OFF_STATUS 0
#define POWER_ON_STATUS 1
//#define FAN_POWER_PIN	P21
#define FAN_PWM_PIN   P42
#define FAN_PC_PIN	P44


#define     CORRECTION_FACTOR              2    //��������
#define     CORRECTION_FACTOR_BIG              30    //����������, Ҫ���ٶȿ����½�������
#define     CORRECTION_FACTOR_BIGGEST              50    //�����������, Ҫ���ٶȿ����½������
#define		FAN_LEVEL1_PULSE_COUNT  60         //  300/60*12 = 60;  //������ص�������
#define		FAN_LEVEL2_PULSE_COUNT  300       //  1500/60*12 = 300;
#define		FAN_LEVEL3_PULSE_COUNT	520			//  260/60*12 = 520;
#define 	FAN_LEVEL12_PULSE_COUNT  200           //�۳���70-100 1000/60*12 = 200
#define     FAN_LEVEL23_PULSE_COUNT 400            //�۳���70-100 2000/60*12 = 400
#define  	FAN_MUST_JUDGE_VALUE  10   //�������ֵ�´η���һ��Ҫ����
#define  	FAN_AUDGE_INIT			20




//#define ION_POWER_CON P01   //����ion�����Ϳ�, �ػ��͹رȽϼ�, �ͺ�fan��һ����
//#define OVER_VOLTAGE_PIN P20  //�жϵ�ѹ�Ƿ�ѹ

void fan_init(void);
//void set_fan_speed(unsigned char speed);
unsigned char read_fan_speed(void);
void power_off(void);
void power_on(void);
void set_power_status(unsigned char sta);
unsigned char read_power_status(void);
void fan_pwm_start(void);
void fan_pwm_stop(void);
void fan_task(void);
void fan_handle(void);  //
void regulate_fan_speed(void);
void store_fan_return_pulse(void);
void fan_level1_speed_judge(void);
void fan_level2_speed_judge(void);
void fan_level3_speed_judge(void);
void fan_level12_speed_judge(void);
void fan_level23_speed_judge(void);

#ifdef DEBUG_FAN_RETURN_PULSE
unsigned int read_disp_fan_return_pulse(void);
#endif

#endif
 