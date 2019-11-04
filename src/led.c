
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
#include "intrins.h"

unsigned char key_power_duty_count_flag;    //1加, 0减
unsigned char key_power_duty_count;  //占空比
unsigned char key_power_period;  //周期
//unsigned char key_power_sum;       //计数总是,

//unsigned char xdata LEDRAM[30] _at_ 0x700;

unsigned int test_count;
/*
unsigned char leddata[]={ 

                0x70,//0xc0,//0x3F,  //"0"  D7 =童锁右 (d6 d6)定时图标 
                0x7f,//0xf9,//0x06,  //"1"  wifi图标(D7 D6) 自动(d5 d4) 睡眠(d3 d2) 童锁左(d1)
                0x7f,//0xa4,//0x5B,  //"2"  40%右(D7) 60%(D6 D5) 80%(D4 D3) 100%(D2 D1)
                0x7f,//0xb0,//0x4F,  //"3"  voc监测(D7 D6) 滤网寿命(D5 D4) 20%(D3 D2) 40%左(D1)
                0x7f,//0x99, //0x66,  //"4" oC D7 湿度(D6) %(D5) 差(D4) 中(D3) 良(D2) 优(D1)
                0x7f,//0x6D,  //"5"   // 定时(D7) PM2.5(D6) 风速(D5) 温度(D4) 小时(D3) ug/m3(D2) 分钟(D1)
                0x73,//0x7D,  //"6"//湿度个位
                0x7b,//0x07,  //"7" //湿度十位
                0xb0,//0x7F,  //"8" //温度个位		//第8位VOC内圈红色LED
                0xf9,//0x6F,  //"9" //温度十位		//第8位VOC内圈绿色LED
                0x79,//0x77,  //"A" //pm25个位   //第8位VOC内圈蓝色LED
                0xed,//0x7C,  //"B" //pm2.5十位  //第8位VOC外圈的蓝色led
                0xb0,//0x39,  //"C" //pm2.5百位  //第8位VOC外圈的白色led背景
                0xff,//0x5E,  //"D"
                0xff,//0x79,  //"E"
                0xff,//0x71,  //"F"
                0xff,//0x76,  //"H"
                0xff,//0x38,  //"L"
                0xff,//0x37,  //"n"
                0xff,//0x3E,  //"u"
                0xff,//0x73,  //"P"
                0xff,//0x5C,  //"o"
                0xff,//0x40,  //"-"
                0xff,//0x00,  //??
                    };
*/

void led_init(void)
{	
	P5CON |= ((1<<0) | (1<<1)| (1<<2)); //
	//P5 &= ~((1<<0) | (1<<1)| (1<<2)); 

	P2CON |= 0x7f; //P2 out.
	LED_RED = 1;
	LED_GREEN = 1;
	LED_BLUE = 1;

	P3CON |= (1<<3) | (1<<4) | (1<<5) | (1<<6) | (1<<7); //P33, 34,35,35,37 out.

	P4CON |= (1<<0) | (1<<1) | (1<<6) | (1<<7);
	P0CON |= (1<<1) | (1<<5) | (1<<6);// | (1<<6) | (1<<7);
	P1CON |= (1<<6) | (1<<7);
	led_off();
	i2c_init();
}

void led_task(void)
//100ms调用一次
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

void led_key_power_count(void)
//做电源按键呼吸灯效果计数用
{
	if(read_power_status() == POWER_OFF_STATUS)
	//关机是才呼吸灯
	{			
		if(++key_power_period >= LED_POWER_PERIOD) key_power_period = 0;
		if(key_power_duty_count>=key_power_period) LED_KEY_POWER = LED_ON;
		else LED_KEY_POWER = LED_OFF;
	}
}

void led_key_power(void)
//10ms调用一次
{
	if(read_power_status() == POWER_OFF_STATUS)
	//关机是才呼吸灯
	{		
		if(1 == key_power_duty_count_flag)key_power_duty_count++;
			else key_power_duty_count--;	
		if(key_power_duty_count >= LED_POWER_PERIOD) key_power_duty_count_flag = 0;
		if(key_power_duty_count<=20) key_power_duty_count_flag = 1;		
	}
}


void led_display_filter_out(unsigned char filter_flag)
{
	if( 1 == filter_flag) 
	{
	//LEDRAM[21] |= 0x10;  //滤网, 电源按键
		LED_FILTER = LED_ON;
	}
	else
	{
		//	LEDRAM[21] &= ~0x10;  //滤网, 电源按键
		LED_FILTER = LED_OFF;
	}
}

void led_off(void)
{
	//DDRCON &= ~0x80;
	LED_GREEN = 1;
	LED_BLUE = 1;
	LED_RED = 1;
	LED_PM2_5 = 1;
	LED_TIMER_1H = 1;
 	LED_TIMER_2H = 1; 
	LED_TIMER_4H = 1;
	LED_TIMER_8H = 1;
	LED_KEY_TIMER = 1;
	LED_FILTER = 1;
	LED_ION = 1;
	LED_KEY_ION  = 1;
	LED_KEY_AROM  = 1;
	LED_AROM_LOW = 1;
	LED_AROM_MIDDLE = 1;
	LED_AROM_HIGH  = 1;
	LED_SPEED_LOW = 1;
	LED_SPEED_MIDDLE = 1;
	LED_SPEED_HIGH = 1;
	LED_SPEED_AUTO = 1;
	LED_KEY_MODE = 1;
	LED_LOCK = 1;
	LED_KEY_LOCK = 1;
	LED_KEY_POWER = 1;
	tm1650_set(0x6E, 0x00);   //
	tm1650_set(0x6c, 0x00);   //
	tm1650_set(0x6A, 0x00);   //
	/*
	LEDRAM[11] &= ~0x08;  // 童锁图标
	LEDRAM[12] &= ~0x18;  //0x10 -P2.5  0x08-8H
	LEDRAM[13] &= ~0x18;  //0x10 - 高, 0x08 - 4H
	LEDRAM[14] &= ~0x18;  //0x10 - 中, 0x08 - 2H  
	//0x80 - 数码管百位的小数点,  0x40 十位, 0x20 个位
	LEDRAM[15] &= ~0xf8;  //0x10 - 低, 0x08 - 1H
	//数码管g段
	LEDRAM[16] &= ~0xf0;  //0x10 - ION图标, 0x08香薰按键,   
	//数码管F段
	LEDRAM[17] &= ~0xf0;   //0x10- 高速 和 0x08-负离子按键
	//数码管E段
	LEDRAM[18] &= ~0xf0;   //0x10- 中速 , 0x08 - 模式按键
	//数码管D段
	LEDRAM[19] &= ~0xf0;  //低速 ,  童锁按键
	//数码管C段
	LEDRAM[20] &= ~0xf0;  //智能, 定时按键
	//数码管B段
	LEDRAM[21] &= ~0xf0;  //滤网, 电源按键
	//数码管A段
	*/
}

void led_all_on(void)
{
	//DDRCON &= ~0x80;
	LED_GREEN = 0;
	LED_BLUE = 0;
	LED_RED = 0;

	LED_PM2_5 = 0;
	LED_TIMER_1H = 0;
 	LED_TIMER_2H = 0; 
	LED_TIMER_4H = 0;
	LED_TIMER_8H = 0;
	LED_FILTER = 0;
	LED_ION = 0;	
	LED_AROM_LOW = 0;
	LED_AROM_MIDDLE = 0;
	LED_AROM_HIGH  = 0;
	LED_SPEED_LOW = 0;
	LED_SPEED_MIDDLE = 0;
	LED_SPEED_HIGH = 0;
	LED_SPEED_AUTO = 0;
	LED_LOCK = 0;

	LED_KEY_TIMER = 0;
	LED_KEY_MODE = 0;	
	LED_KEY_LOCK = 0;
	LED_KEY_POWER = 0;
	LED_KEY_ION  = 0;
	LED_KEY_AROM  = 0;

	tm1650_set(0x6E, 0xff);   //
	tm1650_set(0x6c, 0xff);   //
	tm1650_set(0x6A, 0xff);   //
}


void led_key_on(void)
	//开机后按键的灯亮起来
{
	LED_KEY_TIMER = 0;
	LED_KEY_MODE = 0;	
	LED_KEY_LOCK = 0;
	LED_KEY_POWER = 0;
	LED_KEY_ION  = 0;
	LED_KEY_AROM  = 0;
}


void led_on(void)
{
	//DDRCON |= 0x80;
}


void led_display_pm25(void)
{
	unsigned short tmp_dust_display_value;
	tmp_dust_display_value = read_dust_display_value();
	//led_display_bcd(tmp_dust_display_value / 100, HUNDRED_DIGIT);
	//led_display_bcd(tmp_dust_display_value % 100 / 10, TEN_DIGIT);
	//led_display_bcd(tmp_dust_display_value % 10, SINGLE_DIGIT);

	tm1650_set(0x6E, led_display_bcd(tmp_dust_display_value / 100));   //
	tm1650_set(0x6c, led_display_bcd(tmp_dust_display_value % 100 / 10));   //
	tm1650_set(0x6A, led_display_bcd(tmp_dust_display_value % 10));   //
	LED_PM2_5 = LED_ON; //PM2.5图标
}

void led_display_timing_off_level(void)
{
	switch(read_timing_off_level())
	{
		//case 0:  //关闭
		//	LEDRAM[13] &= ~0x10;  //0x10 - 高, 0x08 - 4H
		//	LEDRAM[14] &= ~0x10;  //0x10 - 中, 0x08 - 2H  
			//0x80 - 数码管百位的小数点,  0x40 十位, 0x20 个位
		//	LEDRAM[15] &= ~0x10;  //0x10 - 低, 0x08 - 1H
		//break;
		case 1:  // 1H
			//LEDRAM[12] &= ~0x08;  //0x10 -P2.5  0x08-8H
			//LEDRAM[13] &= ~0x08;  //0x10 - 高, 0x08 - 4H
			//LEDRAM[14] &= ~0x08;  //0x10 - 中, 0x08 - 2H  
			//LEDRAM[15] |= 0x08;  //0x10 - 低, 0x08 - 1H
			LED_TIMER_1H = LED_ON;
			LED_TIMER_2H = LED_OFF;
			LED_TIMER_4H = LED_OFF;
			LED_TIMER_8H = LED_OFF;
		break;
		case 2:  // 2H
			LED_TIMER_1H = LED_OFF;
			LED_TIMER_2H = LED_ON;
			LED_TIMER_4H = LED_OFF;
			LED_TIMER_8H = LED_OFF;
		break;
		case 3:  // 4H
			LED_TIMER_1H = LED_OFF;
			LED_TIMER_2H = LED_OFF;
			LED_TIMER_4H = LED_ON;
			LED_TIMER_8H = LED_OFF;
		break;
		case 4:  // 8H
			LED_TIMER_1H = LED_OFF;
			LED_TIMER_2H = LED_OFF;
			LED_TIMER_4H = LED_OFF;
			LED_TIMER_8H = LED_ON;
		break;
		default:
			LED_TIMER_1H = LED_OFF;
			LED_TIMER_2H = LED_OFF;
			LED_TIMER_4H = LED_OFF;
			LED_TIMER_8H = LED_OFF;
		break;
	}
}

void led_display_arom_level(void)
{
	switch(read_arom_level())
	{
		case 1:  //低
			LED_AROM_LOW= LED_ON;
			LED_AROM_MIDDLE= LED_OFF;
			LED_AROM_HIGH= LED_OFF;			
		break;
		case 2:  //中
			LED_AROM_LOW= LED_OFF;
			LED_AROM_MIDDLE= LED_ON;
			LED_AROM_HIGH= LED_OFF;	
		break;
		case 3:  //高
			LED_AROM_LOW= LED_OFF;
			LED_AROM_MIDDLE= LED_OFF;
			LED_AROM_HIGH= LED_ON;	
		break;
		default:
			LED_AROM_LOW= LED_OFF;
			LED_AROM_MIDDLE= LED_OFF;
			LED_AROM_HIGH= LED_OFF;	
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
	if( 0 == read_lock_flag())
	{
		LED_LOCK = LED_OFF;
	}
	else
	{
		LED_LOCK = LED_ON;
	}
}

void led_display_ion(void)
{
	if(0 == ION_PIN)
	{
		//LEDRAM[16] |= 0x10;  //0x10 - ION图标, 0x08香薰按键, 
		LED_ION = LED_ON;
	}
	else
	{
		LED_ION = LED_OFF;
		//LEDRAM[16] &= ~0x10;  //0x10 - ION图标, 0x08香薰按键, 
	}
}

void led_display_mode(void)
	//智能, 酒店, 中速, 超强
{
	switch(read_fan_speed())
	{
		case FAN_SPEED_AUTO:
			LED_SPEED_AUTO = LED_ON;
			LED_SPEED_LOW = LED_OFF;
			LED_SPEED_MIDDLE= LED_OFF;
			LED_SPEED_HIGH= LED_OFF;
		break;
		case FAN_SPEED_SLEEP:
			LED_SPEED_AUTO = LED_OFF;
			LED_SPEED_LOW = LED_ON;
			LED_SPEED_MIDDLE= LED_OFF;
			LED_SPEED_HIGH= LED_OFF;
		break;
		case FAN_SPEED_MIDDLE:
			LED_SPEED_AUTO = LED_OFF;
			LED_SPEED_LOW = LED_OFF;
			LED_SPEED_MIDDLE= LED_ON;
			LED_SPEED_HIGH= LED_OFF;
		break;
		case FAN_SPEED_HIGH:
			LED_SPEED_AUTO = LED_OFF;
			LED_SPEED_LOW = LED_OFF;
			LED_SPEED_MIDDLE= LED_OFF;
			LED_SPEED_HIGH= LED_ON;
		break;
		default:
			LED_SPEED_AUTO = LED_OFF;
			LED_SPEED_LOW = LED_OFF;
			LED_SPEED_MIDDLE= LED_OFF;
			LED_SPEED_HIGH= LED_OFF;
		break;
	}
}

unsigned char led_display_bcd(unsigned char bcd_value)
// 返回的数据
{
//8-e 7-d 6-dp 5-c 4-g 3-b 2-f 1-a
	unsigned char tmp;
	switch(bcd_value)
	{
		case 0:
			//tmp = 0xaf;
			tmp = 0xd7;
		break;
		case 1:
			tmp = 0x14;
		break;
		case 2:
			tmp = 0xcd;
		break;
		case 3:
			tmp = 0x5d;
		break;
		case 4:
			tmp = 0x1e;
		break;
		case 5://a
			tmp = 0x5b;
		break;
		case 6:
			tmp = 0xdb;
		break;
		case 7:
			tmp = 0x15;
		break;
		case 8:
			tmp = 0xdf;
		break;
		case 9:
			tmp = 0x5f;
		break;
		default:
			tmp = 0x00;
		break;				
	}
	return tmp;
}


void i2c_init(void)
{
	unsigned int i;
	P0CON |= (1<<0);  //P00输出
	P2CON |= (1<<7);  //P27输出
	//delay_us(10000);
	for(i = 0; i<8000; i++)
	{
		WDTCON  = 0x10;
	}
	i2c_clk_low();
	i2c_din_high();
	tm1650_set(0x48, 0x05);   //开显示0段数码管,灰度1
	//test_i2c();
}

void tm1650_set(unsigned char add,unsigned char dat) //数码管显示
{
        //写显存必须从高地址开始写
        i2c_start();
        i2c_write_byte(add); //第一个显存地址
        i2c_waik_ack();
        i2c_write_byte(dat);
        i2c_waik_ack();
        i2c_stop();
}


void sda_mode(unsigned char mode)
	// 1输出, 0输入
{
	if(IO_MODE_IN == mode)
	{
		P2CON &= ~(1<<7);  //P27输入
	}
	else
	{
		P2CON |= (1<<7);  //P27输出
	}
}

/*
void test_i2c(void)
{
	unsigned short i;
	unsigned char addr;
	//delay_ms(1000);
	for(i = 0; i< 10000; i++)WDTCON  = 0x10;
	i2c_start();
	//i2c_write_byte(0x80);  //写数据命令
	i2c_write_byte(0x48);  //写数据命令
	//i2c_stop();
	i2c_waik_ack();
	
	//for(i = 0; i<60; i++) WDTCON = 0x10;
	//i2c_start();
	
	i2c_write_byte(0x05);  //7段显示, 开显示
	i2c_waik_ack();
	i2c_stop();
	
	for(i = 0; i<60; i++) WDTCON = 0x10;
	i2c_start();
	addr = 0x68;
	i2c_write_byte(addr);  //设置地址命令00
	i2c_waik_ack();
	//i2c_write_byte(0xff);  //设置数据为0
	
	//for(i = 0; i<4; i++)
	//{
		
	//	i2c_write_byte(0x0f);  //写数据命令
	//}
	i2c_write_byte(0x0f);  //写数据命令
	i2c_waik_ack();
	i2c_stop();

	for(i = 0; i<60; i++) WDTCON = 0x10;
	i2c_start();
	addr = 0x6A;
	i2c_write_byte(addr);  //设置地址命令00
	i2c_waik_ack();
	//i2c_write_byte(0xff);  //设置数据为0
	
	//for(i = 0; i<4; i++)
	//{
		
	//	i2c_write_byte(0x0f);  //写数据命令
	//}
	i2c_write_byte(0xaa);  //写数据命令
	i2c_waik_ack();
	i2c_stop();	
}
*/
void delay_us(unsigned char val)
{
	unsigned char i;
	for(i = 0; i < val; i++)
	{
		//_nop_();
		_nop_();
	}
}


void  i2c_waik_ack(void)
{
   uchar EEPROM_err_count;
   
   //SDA_MODEL = 1; 
   delay_us(5);
   I2C_CLK = 1;    
   //I2C_DIN = 1;      
   delay_us(5); 
   I2C_CLK = 0;
   sda_mode(IO_MODE_IN);
   while( 1 == I2C_DIN)
   {
      EEPROM_err_count++;
      if(EEPROM_err_count>=240)
      {
         EEPROM_err_count=0;
         //EEPROM_err_flag=1;
         ION_PIN = ~ION_PIN;
		 sda_mode(IO_MODE_OUT);
         break;
      }
   }     
   //SDA_MODEL = 0;  
   sda_mode(IO_MODE_OUT);
   //SCL = 0; 
   I2C_CLK = 0;
   //Delay1us();
   delay_us(5); 
   //I2C_DIN = 0;
   //SDA = 0; 
   
}


void i2c_write_byte(unsigned char val)
	//写一个字节
{
		unsigned char val_count;
		unsigned char write_data;
		write_data = val;
		delay_us(5);
		
		//i2c_din_low();
		i2c_clk_low();
		for(val_count = 0; val_count < 8; val_count++)
		{
			if((write_data & 0x80) > 0)
			{
				
				i2c_din_high();
				delay_us(5);
				i2c_clk_high();
				delay_us(5);
				i2c_clk_low();
				//i2c_din_low();
			}
			else
			{
				//i2c_clk_low();
				//i2c_delay();
				i2c_din_low();
				i2c_clk_high();
				delay_us(5);
				i2c_clk_low();
				//i2c_din_low();
			}
			write_data <<= 1;
		}
		//i2c_stop();
		delay_us(5);
		
}

void i2c_start(void)
{
	i2c_din_high();
	//delay_5us(1);
	i2c_clk_high();
	delay_us(5);
	i2c_din_low();
	delay_us(5);
	i2c_clk_low();
	delay_us(5);
}

void i2c_stop(void)
{
	i2c_din_low();
	//delay_5us(1);
	i2c_clk_high();
	delay_us(5);
	i2c_din_high();
	delay_us(5);
	i2c_clk_low();
	delay_us(5);
}


void i2c_clk_high(void)
{
	//GPIO_SetBits(GPIOB, GPIO_Pin_5);   //将LED端口拉高，熄灭所有LED  clk
	I2C_CLK = 1;
}

void i2c_clk_low(void)
{
	//GPIO_ResetBits(GPIOB, GPIO_Pin_5);   //将LED端口拉高，熄灭所有LED  clk
	I2C_CLK = 0;
}

void i2c_din_high(void)
{
	//GPIO_SetBits(GPIOB, GPIO_Pin_4);   //将LED端口拉高，熄灭所有LED  clk
	I2C_DIN = 1;
}

void i2c_din_low(void)
{
	//GPIO_ResetBits(GPIOB, GPIO_Pin_4);   //将LED端口拉高，熄灭所有LED  clk
	I2C_DIN = 0;
}




