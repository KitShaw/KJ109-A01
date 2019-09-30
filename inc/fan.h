/*
 *motor.h
 */
 
 
#ifndef __FAN_H_
#define __FAN_H_

#define POWER_OFF_STATUS 0
#define POWER_ON_STATUS 1
#define FAN_POWER_PIN	P21
#define FAN_PWM_PIN   P00
//#define ION_POWER_CON P01   //����ion�����Ϳ�, �ػ��͹رȽϼ�, �ͺ�fan��һ����
#define OVER_VOLTAGE_PIN P20  //�жϵ�ѹ�Ƿ�ѹ

void set_over_voltage_flag(bit flag);
bit read_over_voltage_flag(void);
void over_voltage_handle(void);  //��ѹ��������
void fan_init(void);
void set_fan_speed(unsigned char speed);
unsigned char read_fan_speed(void);
void power_off(void);
void power_on(void);
void set_power_status(unsigned char sta);
unsigned char read_power_status(void);
void fan_pwm_start(void);
void fan_pwm_stop(void);


#endif
 