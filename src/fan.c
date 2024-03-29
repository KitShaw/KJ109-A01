

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
#include "eeprom.h"
#include "beep.h"

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
unsigned char fan_init_speed;

unsigned char fan_regulate_flag;  // 1要调整了, 0不调整

//unsigned char init_up_power_status;  //上电时的初始状态, 0xaa为上电后是开机状态, 其他为关机状态

/*
void set_power_status(unsigned char sta)
{
	power_status = sta;
}
*/

//unsigned char read_power_up_init_status(void)
//{
//	return power_up_init_status;
//}

unsigned char read_init_up_power_status_from_eeprom(void)
{
	return eeprom_read_byte(21);
}

void chang_init_up_power(void) //改变上电初始状态
{
	//init_up_power_status = read_init_up_power_status_from_eeprom();
	if(read_init_up_power_status_from_eeprom() == 0xaa)
	{
		eeprom_write_byte(21, 0x55);	
		if(read_init_up_power_status_from_eeprom() == 0x55) set_beep_count(10);
	}
	else
	{
		eeprom_write_byte(21, 0xaa);
		if(read_init_up_power_status_from_eeprom() == 0xaa) set_beep_count(10);
	}
}
unsigned char read_power_status(void)
{
	return power_status;
}

void fan_init(void)
{
	fan_speed = FAN_SPEED_AUTO;
	P0CON &= ~(1<<2);   //P02输入上拉
	P0PH |= 1<<2;
	
	//P1CON &= ~(1<<6);       //P16输入上拉
	//P1PH |= 1<<6;

	//INT1F |= 0X40 ;    //xxxx xxxx P16下降沿中断
	//INT1R &= ~0X40 ;    //xxxx xxxx  0关闭 1使能
	//IE  |= 0x04;	//0000 0x0x INT1使能
	//IP |= 0x04;   //INT1低优先级

	P4CON |= (1<<2) | (1<<4);     // p44, P42输出
	P42 = 0;
	P44 = 0;

	//PWMCON = 99;           //周期设置低8位,50us  20k
    //PWMCFG = 0xb0;           //7:开关  5-4：时钟源选择 11:fHRC/8 = 2M  3-0：周期设置高4位	
	//PWMCON = 199;           //周期设置低8位,100us, 10K
    //PWMCFG = 0xb0;           //7:开关  5-4：时钟源选择 11:fHRC/8 = 2M  3-0：周期设置高4位	
	PWMCON = 1000%256;           //周期设置低8位,1000us, 1K
    PWMCFG = 0xb3;           //7:开关  5-4：时钟源选择 11:fHRC/8 = 2M  3-0：周期设置高4位	
	
	PWMRD_42 = 0x8000 | 20;
	fan_init_speed = read_speed();
}

void write_speed_to_eeprom(void)
{
	WDTCON = 0x10;
	eeprom_write_byte(20, fan_speed);
	fan_init_speed = fan_speed;
}


unsigned char read_speed(void)
{
	unsigned char tmp;
	WDTCON = 0x10;
	tmp = eeprom_read_byte(20);
	if((tmp == 1) || (tmp==2) || (tmp == 3))return tmp;
	else return 0;
}



void fan_pwm_start(void)
{
	FAN_PC_PIN = 1;
	//fan_pwm_value = 23;
	fan_pulse_count = 1;
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
	switch(fan_speed)
	{
		case 0: //auto
			switch(read_dust_level())
			{
				case DUST_LEVEL_EXCELLENT: //优1速档
					fan_pulse_count = FAN_LEVEL1_PULSE;
				break;
				case DUST_LEVEL_MEDIUM: //中2档
					fan_pulse_count = FAN_LEVEL2_PULSE;
				break;
				case DUST_LEVEL_BAD: //差3挡
					fan_pulse_count = FAN_LEVEL3_PULSE;
				break;
				case DUST_LEVEL_12: //
					fan_pulse_count = FAN_LEVEL2_PULSE;
				break;
				case DUST_LEVEL_23: //
					fan_pulse_count = FAN_LEVEL2_PULSE;
				break;
			}
		break;
		case 1:		
			fan_pulse_count = FAN_LEVEL1_PULSE;
		break;
		case 2: //
			fan_pulse_count = FAN_LEVEL2_PULSE;
		break;
		case 3:
			fan_pulse_count = FAN_LEVEL3_PULSE;		
		break;
		default:
		break;
	}
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
	if(read_power_status() == POWER_OFF_STATUS) return; //关机状态直接返回
	if(0 == fan_regulate_flag) return;
	fan_regulate_flag = 0;
	disp_fan_return_pulse = fan_return_pulse_count;
	switch(fan_speed)
	{
		case 0: //auto
			switch(read_dust_level())
			{
				case DUST_LEVEL_EXCELLENT: //优1速档
					fan_level1_speed_judge();					    				
				break;
				case DUST_LEVEL_MEDIUM: //中2档
					fan_level2_speed_judge();				
				break;
				case DUST_LEVEL_BAD: //差3挡
					fan_level3_speed_judge();
					
				break;
				case DUST_LEVEL_12: //
					//fan_level12_speed_judge();
					fan_level2_speed_judge();	
				break;
				case DUST_LEVEL_23: //
					//fan_level23_speed_judge();
					fan_level2_speed_judge();	
				break;
			}
		break;
		case 1:						   	
			fan_level1_speed_judge();
		break;
		case 2: //
			fan_level2_speed_judge();
			/*
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
			*/
		break;
		case 3:
			fan_level3_speed_judge();
			/*
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
	    	*/
		break;
		default:
		break;
	}
	
	if(fan_judge>22) 
	{
			fan_judge = FAN_AUDGE_INIT;
		fan_pulse_count++;
		
	}
	if (fan_judge<18) 
	{
			fan_judge = FAN_AUDGE_INIT;
			fan_pulse_count--;
	}
	if(fan_pulse_count>800)
	{		
		fan_pulse_count = 800;		
	}	
	//fan_return_pulse_count = 0;
}


void fan_level1_speed_judge(void)
//1档转速调整
{
	//只有正负10的值可以调整,
	if(fan_return_pulse_count<(FAN_LEVEL1_PULSE_COUNT-(CORRECTION_FACTOR + 3)))  //每0.5s计算一次电机的脉冲数， 没达到要求的转速就调整脉冲数
	//  低档晃动的厉害, 改CORRECTION_FACTOR+3
    {
    	if(fan_return_pulse_count <(FAN_LEVEL1_PULSE_COUNT - (CORRECTION_FACTOR_BIGGEST)))
    	{
    		//fan_pulse_count += 20;
    		fan_pulse_count = FAN_LEVEL1_PULSE;
    	}
		else if(fan_return_pulse_count<(FAN_LEVEL1_PULSE_COUNT-CORRECTION_FACTOR_BIG))
		{
			fan_judge += FAN_MUST_JUDGE_VALUE;
			//fan_pulse_count = FAN_LEVEL1_PULSE;
		}
    	else 
    	{
			if(fan_pulse_count < (FAN_LEVEL1_PULSE + 10))fan_judge++;			
    	}
	} 
	else
    {
    	if(fan_return_pulse_count >(FAN_LEVEL1_PULSE_COUNT + (CORRECTION_FACTOR_BIGGEST + 100)))
    	{
    		//fan_pulse_count -= 20;
    		fan_pulse_count = FAN_LEVEL1_PULSE;
    	}
		else if(fan_return_pulse_count>(FAN_LEVEL1_PULSE_COUNT + CORRECTION_FACTOR))
    	{
    		if(fan_return_pulse_count>(FAN_LEVEL1_PULSE_COUNT + CORRECTION_FACTOR_BIG))
    		{
				fan_judge -= FAN_MUST_JUDGE_VALUE;
				//fan_pulse_count = FAN_LEVEL1_PULSE;
    		}
			else 
			{
				if(fan_pulse_count > (FAN_LEVEL1_PULSE - 10))fan_judge--;
			}
    	}
		else fan_judge = FAN_AUDGE_INIT;			
	 }  
}

void fan_level2_speed_judge(void)
//2档转速调整
{
	//只有正负10的值可以调整,
	if(fan_return_pulse_count<(FAN_LEVEL2_PULSE_COUNT-CORRECTION_FACTOR))  //每0.5s计算一次电机的脉冲数， 没达到要求的转速就调整脉冲数
	//  低档晃动的厉害, 改CORRECTION_FACTOR+3
    {
    	if(fan_return_pulse_count <(FAN_LEVEL2_PULSE_COUNT - (CORRECTION_FACTOR_BIGGEST)))
    	{
    		//fan_pulse_count += 20;
    		fan_pulse_count = FAN_LEVEL2_PULSE;
    	}
		else if(fan_return_pulse_count<(FAN_LEVEL2_PULSE_COUNT-CORRECTION_FACTOR_BIG))
		{
			fan_judge += FAN_MUST_JUDGE_VALUE;
			//fan_pulse_count = FAN_LEVEL2_PULSE;
		}
    	else 
    	{
			if(fan_pulse_count < (FAN_LEVEL2_PULSE + 10))fan_judge++;			
    	}
	} 
	else
    {
    	if(fan_return_pulse_count >(FAN_LEVEL2_PULSE_COUNT + (CORRECTION_FACTOR_BIGGEST + 100)))
    	{
    		//fan_pulse_count -= 20;
    		fan_pulse_count = FAN_LEVEL2_PULSE;
    	}
		else if(fan_return_pulse_count>(FAN_LEVEL2_PULSE_COUNT + CORRECTION_FACTOR))
    	{
    		if(fan_return_pulse_count>(FAN_LEVEL2_PULSE_COUNT + CORRECTION_FACTOR_BIG))
    		{
				fan_judge -= FAN_MUST_JUDGE_VALUE;
				//fan_pulse_count = FAN_LEVEL2_PULSE;
    		}
			else 
			{
				if(fan_pulse_count > (FAN_LEVEL2_PULSE - 10))fan_judge--;
			}
    	}
		else fan_judge = FAN_AUDGE_INIT;			
	 }  
}


void fan_level3_speed_judge(void)
//2档转速调整
{
	//只有正负10的值可以调整,
	if(fan_return_pulse_count<(FAN_LEVEL3_PULSE_COUNT-CORRECTION_FACTOR))  //每0.5s计算一次电机的脉冲数， 没达到要求的转速就调整脉冲数
	//  低档晃动的厉害, 改CORRECTION_FACTOR+3
    {
    	if(fan_return_pulse_count <(FAN_LEVEL3_PULSE_COUNT - (CORRECTION_FACTOR_BIGGEST)))
    	{
    		//fan_pulse_count += 20;
    		fan_pulse_count = FAN_LEVEL3_PULSE;
    	}
		else if(fan_return_pulse_count<(FAN_LEVEL3_PULSE_COUNT-CORRECTION_FACTOR_BIG))
		{
			fan_judge += FAN_MUST_JUDGE_VALUE;
			//fan_pulse_count = FAN_LEVEL3_PULSE;
		}
    	else 
    	{
			if(fan_pulse_count < (FAN_LEVEL3_PULSE + 10))fan_judge++;			
    	}
	} 
	else
    {
    	if(fan_return_pulse_count >(FAN_LEVEL3_PULSE_COUNT + (CORRECTION_FACTOR_BIGGEST + 100)))
    	{
    		//fan_pulse_count -= 20;
    		fan_pulse_count = FAN_LEVEL3_PULSE;
    	}
		else if(fan_return_pulse_count>(FAN_LEVEL3_PULSE_COUNT + CORRECTION_FACTOR))
    	{
    		if(fan_return_pulse_count>(FAN_LEVEL3_PULSE_COUNT + CORRECTION_FACTOR_BIG))
    		{
				fan_judge -= FAN_MUST_JUDGE_VALUE;
				//fan_pulse_count = FAN_LEVEL3_PULSE;
    		}
			else 
			{
				if(fan_pulse_count > FAN_LEVEL3_PULSE - 10)fan_judge--;
			}
    	}
		else fan_judge = FAN_AUDGE_INIT;			
	 }  
}


//void fan_pulse_count_add(void)
//{
//	fan_pulse_count++;
//}

//void fan_pulse_count_dec(void)
//{
//	fan_pulse_count--;
//}



//void fan_level12_speed_judge(void)
//{
//	if(fan_return_pulse_count<(FAN_LEVEL12_PULSE_COUNT-CORRECTION_FACTOR))  //每0.5s计算一次电机的脉冲数， 没达到要求的转速就调整脉冲数
//    {
//    	if(fan_return_pulse_count <(FAN_LEVEL12_PULSE_COUNT - CORRECTION_FACTOR_BIGGEST))
//    	{
//    		fan_pulse_count += 20;
//    	}
//		else if(fan_return_pulse_count<(FAN_LEVEL12_PULSE_COUNT-CORRECTION_FACTOR_BIG))
//		{
//			fan_judge += FAN_MUST_JUDGE_VALUE;
//		}
//    	else fan_judge++;			
//	} 
//	else
//    {
//    	if(fan_return_pulse_count >(FAN_LEVEL12_PULSE_COUNT + CORRECTION_FACTOR_BIGGEST))
//    	{
//    		fan_pulse_count -= 20;
//    	}
//		else if(fan_return_pulse_count>(FAN_LEVEL12_PULSE_COUNT + CORRECTION_FACTOR))
//    	{
//    		if(fan_return_pulse_count>(FAN_LEVEL12_PULSE_COUNT + CORRECTION_FACTOR_BIG)) fan_judge -= FAN_MUST_JUDGE_VALUE;
//			else fan_judge--;
//    	}
//		else fan_judge = FAN_AUDGE_INIT;			
//	 } 
//}
//void fan_level23_speed_judge(void)
//{
//	if(fan_return_pulse_count<(FAN_LEVEL23_PULSE_COUNT-CORRECTION_FACTOR))  //每0.5s计算一次电机的脉冲数， 没达到要求的转速就调整脉冲数
//    {
//    	if(fan_return_pulse_count <(FAN_LEVEL23_PULSE_COUNT - CORRECTION_FACTOR_BIGGEST))
//    	{
//    		fan_pulse_count += 20;
//    	}
//		else if(fan_return_pulse_count<(FAN_LEVEL23_PULSE_COUNT-CORRECTION_FACTOR_BIG))
//		{
//			fan_judge += FAN_MUST_JUDGE_VALUE;
//		}
//    	else fan_judge++;			
//	} 
//	else
//    {
//    	if(fan_return_pulse_count >(FAN_LEVEL23_PULSE_COUNT + CORRECTION_FACTOR_BIGGEST))
//    	{
//    		fan_pulse_count -= 20;
//    	}
//		else if(fan_return_pulse_count>(FAN_LEVEL23_PULSE_COUNT + CORRECTION_FACTOR))
//    	{
//    		if(fan_return_pulse_count>(FAN_LEVEL23_PULSE_COUNT + CORRECTION_FACTOR_BIG)) fan_judge -= FAN_MUST_JUDGE_VALUE;
//			else fan_judge--;
//    	}
//		else fan_judge = FAN_AUDGE_INIT;			
//	 } 
//}



void power_on(void)
{
	power_status = POWER_ON_STATUS;
	led_on();  //开显示
	if(read_ion_init_status() == 0xaa) ion_off();
	else ion_on();
	//arom_on();
	set_arom_level(read_arom_level_form_eeprom());
	fan_pwm_start();
	DUST_PWR_PIN = 0;
	reset_key_no_move_count();
	fan_speed = fan_init_speed;
	
	led_key_on();
	//fan_pulse_count = FAN_LEVEL2_PULSE;
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






