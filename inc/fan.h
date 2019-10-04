/*
 *motor.h
 */
 
 
#ifndef __FAN_H_
#define __FAN_H_

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
#define		FAN_LEVEL1_PULSE_COUNT  60         //  300/60*12 = 60;  //������ص�������
#define		FAN_LEVEL2_PULSE_COUNT  120//102		//  600/60*12 = 120;
#define		FAN_LEVEL3_PULSE_COUNT	176			//  880/60*12 = 176;
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
unsigned int read_disp_fan_return_pulse(void);


#endif
 