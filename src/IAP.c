//#include "H/Function_Init.H"
#include "sc92f837x_c.h"
#include "IAP.H"
#include "intrins.h"

//#define IapROM  0x00	  //0x00��ѡ��ROM������  0x02��ѡ��EEPROM������
//#define IapEPPROM  0x02	  //0x00��ѡ��ROM������  0x02��ѡ��EEPROM������

volatile unsigned char code *IapAddr = 0x00;

//void IAPWrite(unsigned int Addr,unsigned char Value,unsigned char IAPArea);
unsigned char IAPRead(unsigned int Addr,unsigned char IAPArea);
unsigned char IapReadData = 0x00;   //��Ŷ�ȡ��IAP����

/*****************************************************
*�������ƣ�void IAP_Test(uchar Address,uchar IAPArea)
*�������ܣ�IAP����
*��ڲ�����Address,IAPArea
*���ڲ�����void
*****************************************************/
/*
void IAP_Test(uint Address,uchar IAPArea)
{
	IAPWrite(Address,0xff,IAPArea);
	IapReadData = IAPRead(Address,IAPArea);
	IAPADE = 0x00;       //IAPָ��ָ��ROM��
	while(1)
	{
		if(IapReadData == 0xff)
		{
			P02 = ~P02;
		}
		else
		{
			P03 = ~P03;
		}
	}
}
*/
/*****************************************************
*�������ƣ�void IAPWrite(uchar Addr,uchar Value,uchar IAPArea)
*�������ܣ�IAPд��ʼ��
*��ڲ�����Addr,Value,IAPArea
*���ڲ�����void
*****************************************************/
//IAPд����
unsigned char IAPWrite(unsigned int Addr,unsigned char Value,unsigned char IAPArea)
{	
	EA = 0;
	IAPDAT = Value;      //д������Data
	IAPADH = ((Addr>>8)&0X7F);   //д���ַ�ĸ�5λ
	IAPADL = Addr;               //д���ַ�ĵ�8λ
	
	IAPADE = IAPArea;  //IAPArea=0x00��ѡ��ROM������  IAPArea=0x02��ѡ��EEPROM������
	IAPKEY = 240;		   //IAP������������ʱ��
	IAPCTL = 0x06;     //CPU hold timeΪ2ms��д���������
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	IAPADE = 0x00;      //MOVCָ��ROM
	EA = 1;
	
	if(IAPRead(Addr, IapEPPROM) == Value) return 1;
		return 0;
}
/*****************************************************
*�������ƣ�uchar IAPRead(uchar Addr,uchar IAPArea)
*�������ܣ�IAP����ʼ��
*��ڲ�����Addr,IAPArea
*���ڲ�����ReadValue
*****************************************************/
unsigned char IAPRead(unsigned int Addr, unsigned char IAPArea)
{
	unsigned char ReadValue = 0x00;
	EA = 0;
	IAPADE = IAPArea;
	ReadValue = *(IapAddr+Addr); //��ȡ������
	IAPADE = 0x00;               //MOVCָ��ROM
	EA = 1;
	return ReadValue;
}
