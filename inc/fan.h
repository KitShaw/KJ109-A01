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


#define     CORRECTION_FACTOR              2    //修正因子
#define     CORRECTION_FACTOR_BIG              30    //大修正因子, 要让速度快速下降和上升
#define     CORRECTION_FACTOR_BIGGEST              50    //最大修正因子, 要让速度快速下降和上�
//#define		FAN_LEVEL1_PULSE_COUNT  60         //  300/60*12 = 60;  //风机返回的脉冲数

#define		FAN_LEVEL1_PULSE_COUNT  100         //  500/60*12 = 100;  //风机返回的脉冲数
#define		FAN_LEVEL2_PULSE_COUNT  300       //  1500/60*12 = 300;
#define		FAN_LEVEL3_PULSE_COUNT	520			//  260/60*12 = 520;
//#define 	FAN_LEVEL12_PULSE_COUNT  170           //粉尘在70-100 1000/60*12 = 200
#define 	FAN_LEVEL12_PULSE_COUNT  200           //粉尘在70-100 1000/60*12 = 200

#define     FAN_LEVEL23_PULSE_COUNT 400            //粉尘在70-100 2000/60*12 = 400
#define  	FAN_MUST_JUDGE_VALUE  10   //加上这个值下次风速一定要调整
#define  	FAN_AUDGE_INIT			20

#define		FAN_LEVEL1_PULSE	92   //1档的脉冲数89
#define 	FAN_LEVEL2_PULSE    443  //1500
#define 	FAN_LEVEL3_PULSE	683  //2500
//298 1023
//1500  433
//2047  583
//2583  686


//高档发送的脉冲数为560

//中档:259 
//296-308
//低档:52  
//反馈的脉冲101-105




//#define ION_POWER_CON P01   //由于ion开机就开, 关机就关比较简单, 就和fan放一起了
//#define OVER_VOLTAGE_PIN P20  //判断电压是否超压

void fan_init(void);
//void set_fan_speed(unsigned char speed);
unsigned char read_fan_speed(void);
void power_off(void);
void power_on(void);
void chang_init_up_power(void);  //改变上电初始状态
unsigned char read_init_up_power_status_from_eeprom(void);


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
//void fan_level12_speed_judge(void);
//void fan_level23_speed_judge(void);


//void fan_pulse_count_add(void);

//void fan_pulse_count_dec(void);

unsigned char read_speed(void);
void write_speed_to_eeprom(void);




#ifdef DEBUG_FAN_RETURN_PULSE
unsigned int read_disp_fan_return_pulse(void);
#endif

#endif
 
