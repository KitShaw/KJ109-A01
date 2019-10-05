

/*
 *fan.c
 *Kit Shaw
 * 2019.10.2
 */
 
#include "SC92F844X_C.h"
#include "led.h"
#include "fan.h"
#include "filter.h"
#include "IAP.h"
#include "ion.H"
#include "arom.h"
#include "dust.h"
#include "timing_off.h"
#include "key.h"

//unsigned int xdata PWMRD_40  _at_  0x740;
//unsigned int xdata PWMRD_41  _at_  0x742;
unsigned int xdata PWMRD_42  _at_  0x744;
//unsigned int xdata PWMRD_43  _at_  0x746;
//unsigned int xdata PWMRD_50  _at_  0x748;
//unsigned int xdata PWMRD_51  _at_  0x74A;
//unsigned int xdata PWMRD_52  _at_  0x74C;
//unsigned int xdata PWMRD_53  _at_  0x74E;
unsigned int xdata fan_pwm_value;
unsigned int xdata fan_pulse_count;
unsigned int xdata PWMRD_Temp;


unsigned int    fan_return_pulse_count;  //风机返回的脉冲数。
unsigned int    disp_fan_return_pulse;
unsigned char fan_judge; //判断是否要调整转速


unsigned char power_status;
unsigned char fan_speed;  // 0智能, 1静音, 2中, 3高

unsigned char fan_regulate_flag;  // 1要调整了, 0不调整

/*
void set_power_status(unsigned char sta)
{
	power_status = sta;
}
*/
unsigned char read_power_status(void)
{
	return power_status;
}

void fan_init(void)
{
	fan_speed = FAN_SPEED_AUTO;
	P0CON &= ~(1<<2);   //P02输入上拉
	P0PH |= 1<<2;
	
	P1CON &= ~(1<<6);       //P16输入上拉
	P1PH |= 1<<6;

	//INT1F |= 0X40 ;    //xxxx xxxx P16下降沿中断
	//INT1R &= ~0X40 ;    //xxxx xxxx  0关闭 1使能
	//IE  |= 0x04;	//0000 0x0x INT1使能
	//IP |= 0x04;   //INT1低优先级

	P4CON |= (1<<2) | (1<<4);     // p44, P42输出
	P42 = 0;
	P44 = 0;

	//PWMCON = 99;           //周期设置低8位,50us  20k
    //PWMCFG = 0xb0;           //7:开关  5-4：时钟源选择 11:fHRC/8 = 2M  3-0：周期设置高4位	
	PWMCON = 199;           //周期设置低8位,100us, 10K
    PWMCFG = 0xb0;           //7:开关  5-4：时钟源选择 11:fHRC/8 = 2M  3-0：周期设置高4位	
	
	PWMRD_42 = 0x8000 | 20;
}



void fan_pwm_start(void)
{
	FAN_PC_PIN = 1;
	//fan_pwm_value = 23;
	fan_pulse_count = 25;
	PWMRD_42 |= 0x8000;
	PWMRD_Temp = 0;
}

//void fan_pwm_stop(void)
//{
//	PWMCON &= 0x7f;
//	FAN_PWM_PIN = 0;
//}


/*
void set_fan_speed(unsigned char speed)
{
	
}
*/
void regulate_fan_speed(void)
//调整转, 掉用一次切换一次, 自动, 低, 中, 高依次循环
{
	if(++fan_speed> FAN_SPEED_HIGH) fan_speed = FAN_SPEED_AUTO;	
}

unsigned char read_fan_speed(void)
{
	return fan_speed;
}

void fan_task(void)
	//10ms调用一次
{
	//unsigned int tmp;
	//tmp = PWMRD_42 & (~0x8000);
	if(read_power_status() == POWER_ON_STATUS)
	{

		if( (PWMRD_Temp & (~0x8000)) != fan_pulse_count)
		{
			if( (PWMRD_Temp & (~0x8000)) < fan_pulse_count)
			{
				PWMRD_Temp++;
			}
			else PWMRD_Temp--;

			PWMRD_42 = PWMRD_Temp | 0x8000;
		}

		//PWMRD_42 = 20 | 0x8000;
		
	}
	else
	{
		if(fan_pulse_count>0)
                {
                    fan_pulse_count--;
					PWMRD_42 = 0x8000 | fan_pulse_count;                    
                }
                else
                {
                    fan_pulse_count = 0;
                    PWMRD_42 &= ~0x8000;
                    FAN_PWM_PIN = 0;
					FAN_PC_PIN = 0;
                }
	}
}

#ifdef DEBUG_FAN_RETURN_PULSE
unsigned int read_disp_fan_return_pulse(void)
{
	//return  disp_fan_return_pulse;
	return fan_pulse_count;
}

#endif

void store_fan_return_pulse(void)
	//把当前的转速值存储起来
{
	//disp_fan_return_pulse = fan_return_pulse_count;
	//disp_fan_return_pulse
	fan_return_pulse_count = (TH0 << 8) + TL0;
	TH0 = 0;
	TL0 = 0;
	fan_regulate_flag = 1; 
	//fan_return_pulse_count = 0;
}


void fan_handle(void)  
	//设置风机的速度，1s调用一次
{
	//static unsigned char tmp=0; 
	if(0 == fan_regulate_flag) return;
	fan_regulate_flag = 0;
	disp_fan_return_pulse = fan_return_pulse_count;
	switch(fan_speed)
	{
		case 0: //auto
			switch(read_dust_level())
			{
				case DUST_LEVEL_EXCELLENT: //优1速档
					if(fan_return_pulse_count<(FAN_LEVEL1_PULSE_COUNT-CORRECTION_FACTOR))  //每0.5s计算一次电机的脉冲数， 没达到要求的转速就调整脉冲数
    				{
		    			if(fan_return_pulse_count<(FAN_LEVEL1_PULSE_COUNT-CORRECTION_FACTOR_BIG))fan_judge += FAN_MUST_JUDGE_VALUE;
    					else fan_judge++;			
			    	} 
					else
    				{
    					if(fan_return_pulse_count>(FAN_LEVEL1_PULSE_COUNT + CORRECTION_FACTOR))
		    			{
    						if(fan_return_pulse_count>(FAN_LEVEL1_PULSE_COUNT + CORRECTION_FACTOR_BIG)) fan_judge -= FAN_MUST_JUDGE_VALUE;
							else fan_judge--;
    					}
						else fan_judge = FAN_AUDGE_INIT;			
			    	}
					    				
				break;
				case DUST_LEVEL_MEDIUM: //中2档
					if(fan_return_pulse_count<(FAN_LEVEL2_PULSE_COUNT-CORRECTION_FACTOR))  //每0.5s计算一次电机的脉冲数， 没达到要求的转速就调整脉冲数
    				{
		    			if(fan_return_pulse_count<(FAN_LEVEL2_PULSE_COUNT-CORRECTION_FACTOR_BIG))fan_judge += FAN_MUST_JUDGE_VALUE;
    					else fan_judge++;			
			    	} else
    				{
    				if(fan_return_pulse_count>(FAN_LEVEL2_PULSE_COUNT + CORRECTION_FACTOR))
	    			{
    					if(fan_return_pulse_count>(FAN_LEVEL2_PULSE_COUNT + CORRECTION_FACTOR_BIG)) fan_judge -= FAN_MUST_JUDGE_VALUE;
						else fan_judge--;
    				}
					else fan_judge = FAN_AUDGE_INIT;			
			    	}					
				break;
				case DUST_LEVEL_BAD: //差3挡
					if(fan_return_pulse_count<(FAN_LEVEL3_PULSE_COUNT-CORRECTION_FACTOR))  //每0.5s计算一次电机的脉冲数， 没达到要求的转速就调整脉冲数
    				{
		    			if(fan_return_pulse_count<(FAN_LEVEL3_PULSE_COUNT-CORRECTION_FACTOR_BIG))fan_judge += FAN_MUST_JUDGE_VALUE;
    					else fan_judge++;			
			    	} else
    				{
    				if(fan_return_pulse_count>(FAN_LEVEL3_PULSE_COUNT + CORRECTION_FACTOR))
	    			{
    					if(fan_return_pulse_count>(FAN_LEVEL3_PULSE_COUNT + CORRECTION_FACTOR_BIG)) fan_judge -= FAN_MUST_JUDGE_VALUE;
						else fan_judge--;
    				}
					else fan_judge = FAN_AUDGE_INIT;			
			    	}  
					
				break;
			}
		break;
		case 1:
			
			if(fan_return_pulse_count<(FAN_LEVEL1_PULSE_COUNT-CORRECTION_FACTOR))  //每0.5s计算一次电机的脉冲数， 没达到要求的转速就调整脉冲数
    		{
    			if(fan_return_pulse_count<(FAN_LEVEL1_PULSE_COUNT-CORRECTION_FACTOR_BIG))fan_judge += FAN_MUST_JUDGE_VALUE;
    			else fan_judge++;			
	    	} else
    		{
    			if(fan_return_pulse_count>(FAN_LEVEL1_PULSE_COUNT + CORRECTION_FACTOR))
    			{
    				if(fan_return_pulse_count>(FAN_LEVEL1_PULSE_COUNT + CORRECTION_FACTOR_BIG)) fan_judge -= FAN_MUST_JUDGE_VALUE;
					else fan_judge--;
    			}
				else fan_judge = FAN_AUDGE_INIT;			
	    	}     	
			
		break;
		case 2: //
		
			if(fan_return_pulse_count<(FAN_LEVEL2_PULSE_COUNT-CORRECTION_FACTOR))  //每0.5s计算一次电机的脉冲数， 没达到要求的转速就调整脉冲数
    		{
    			if(fan_return_pulse_count<(FAN_LEVEL2_PULSE_COUNT-CORRECTION_FACTOR_BIG))fan_judge += FAN_MUST_JUDGE_VALUE;
    			else fan_judge++;			
	    	} else
    		{
    			if(fan_return_pulse_count>(FAN_LEVEL2_PULSE_COUNT + CORRECTION_FACTOR))
    			{
    				if(fan_return_pulse_count>(FAN_LEVEL2_PULSE_COUNT + CORRECTION_FACTOR_BIG)) fan_judge -= FAN_MUST_JUDGE_VALUE;
					else fan_judge--;
    			}
				else fan_judge = FAN_AUDGE_INIT;			
	    	}  
			
		break;
		case 3:
			
			if(fan_return_pulse_count<(FAN_LEVEL3_PULSE_COUNT-CORRECTION_FACTOR))  //每0.5s计算一次电机的脉冲数， 没达到要求的转速就调整脉冲数
    		{
    			if(fan_return_pulse_count<(FAN_LEVEL3_PULSE_COUNT-CORRECTION_FACTOR_BIG))fan_judge += FAN_MUST_JUDGE_VALUE;
    			else fan_judge++;			
	    	} else
    		{
    			if(fan_return_pulse_count>(FAN_LEVEL3_PULSE_COUNT + CORRECTION_FACTOR))
    			{
    				if(fan_return_pulse_count>(FAN_LEVEL3_PULSE_COUNT + CORRECTION_FACTOR_BIG)) fan_judge -= FAN_MUST_JUDGE_VALUE;
					else fan_judge--;
    			}
				else fan_judge = FAN_AUDGE_INIT;			
	    	}  
		break;
		default:
		break;
	}
	
	if(fan_judge>21) 
	{
			fan_judge = FAN_AUDGE_INIT;
		fan_pulse_count++;
		
	}
	if (fan_judge<19) 
	{
			fan_judge = FAN_AUDGE_INIT;
			fan_pulse_count--;
	}
	if(fan_pulse_count>160)
	{		
		fan_pulse_count = 160;		
	}	
	//fan_return_pulse_count = 0;
}


void power_on(void)
{
	power_status = POWER_ON_STATUS;
	led_on();  //开显示
	ion_on();
	//arom_on();
	fan_pwm_start();
	DUST_PWR_PIN = 0;
	reset_key_no_move_count();
}

void power_off(void)
{
	
	power_status = POWER_OFF_STATUS;	
	ion_off();
	set_arom_level(0);
	reset_timing_off_level();
	//arom_off();
	led_off();
	DUST_PWR_PIN = 1;
	reset_key_no_move_count();
	write_filter_time();
	reset_lock_flag();
}






