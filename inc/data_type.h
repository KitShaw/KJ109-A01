
#ifndef DATA_TYPE_C_HEADER
#define DATA_TYPE_C_HEADER

//#define DEBUG_FAN_RETURN_PULSE

#define VERSION 102
// 101
// 102 为了解决101风速中档调高档的时候, 会出现速度很慢才调上去的问题
// 在风速调整里直接加上风速脉冲值.


typedef bit 			    BOOL; 		// 布尔变量（位变量）
typedef unsigned char		CHAR;		// 字符变量
typedef unsigned char 		INT8U; 		// 无符号8位整型变量  
typedef signed char 		INT8S; 		// 有符号8位整型变量  
typedef unsigned int 		INT16U; 	// 无符号16位整型变量 
typedef signed int 		    INT16S; 	// 有符号16位整型变量
typedef unsigned long 		INT32U; 	// 无符号32位整型变量
typedef signed long 		INT32S; 	// 有符号32位整型变量 
typedef float 			    FP32; 		// 单精度浮点数(32位长度) 
typedef double 			    FP64; 		// 双精度浮点数(64位长度)

typedef struct{
	unsigned char bit0:1;
	unsigned char bit1:1;
	unsigned char bit2:1;
	unsigned char bit3:1;
	unsigned char bit4:1;
	unsigned char bit5:1;
	unsigned char bit6:1;
	unsigned char bit7:1;
}bitval;

#define	uchar	unsigned	char
#define	uint	unsigned	int
#define	uint32	unsigned	double	int

#define		BS(SFR,NUM)		(SFR|=(1<<NUM))		//清bit位
#define		BC(SFR,NUM)		(SFR&=~(1<<NUM))	//bit位置1

#define 	True		1
#define 	False		0
#define 	Enable		1
#define 	Disable		0

#define		BIT0	0x01
#define		BIT1	0x02
#define		BIT2	0x04
#define		BIT3	0x08
#define		BIT4	0x10
#define		BIT5	0x20
#define		BIT6	0x40
#define		BIT7	0x80
#define		BIT8	0x0100
#define		BIT9	0x0200

#endif
