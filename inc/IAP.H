//#include "H/Function_Init.H"

#define IapROM  0x00	  //0x00：选择ROM区操作  0x02：选择EEPROM区操作
#define IapEPPROM  0x02	  //0x00：选择ROM区操作  0x02：选择EEPROM区操作
//unsigned char code *IapAddr = 0x00;

unsigned char IAPWrite(unsigned int Addr,unsigned char Value,unsigned char IAPArea);
unsigned char IAPRead(unsigned int Addr,unsigned char IAPArea);
//uchar IapReadData = 0x00;   //存放读取的IAP数据
