//*************************************************************************************************
//  Copyright (c) 	深圳市赛元微电子有限公司
//	文件名称	:  S_TouchKeyCFG.h
//	作者		: 
//	模块功能	:  触控键配置文件
// 	版本		:  V0.2
// 	更改记录	:
//************************************************************************************************
#ifndef __S_TOUCHKEYCFG_H__
#define __S_TOUCHKEYCFG_H__
#define		    SOCAPI_SET_TOUCHKEY_TOTAL					  4
#define			SOCAPI_SET_TOUCHKEY_CHANNEL					  0x00000F00
unsigned int  code TKCFG[17] = {0,0,8,5,10,3000,200,100,2,0,0,4,0,65535,65535,65535,20}; 
	
unsigned char code TKChannelCfg[4][8]={
0x06,0x16,0x09,0x14,0x1d,0x05,0x00,0x4e,
0x06,0x2e,0x06,0x0f,0x19,0x05,0x00,0x72,
0x03,0x2e,0x06,0x0f,0x2d,0x05,0x00,0xb5,
0x06,0x16,0x09,0x12,0x1d,0x05,0x00,0x4e,
};
	
	/*
unsigned char code TKChannelCfg[4][8]={
0x09,0x2e,0x0c,0x10,0x1b,0x04,0x00,0x26,
0x04,0x2e,0x09,0x0f,0x2f,0x05,0x00,0x39,
0x05,0x2e,0x0c,0x0f,0x2f,0x05,0x00,0x31,
0x09,0x2e,0x0c,0x10,0x1c,0x05,0x00,0x26,
};
	*/

/*
	
	unsigned char code TKChannelCfg[4][8]={
0x09,0x2e,0x0c,0x0f,0x1b,0x04,0x00,0x26,
0x04,0x2e,0x09,0x0f,0x2f,0x05,0x00,0x39,
0x05,0x2e,0x0c,0x0f,0x2f,0x05,0x00,0x31,
0x07,0x2e,0x0a,0x1e,0x1c,0x05,0x00,0x54,
};
	*/
	/*
unsigned char code TKChannelCfg[4][8]={
0x08,0x2e,0x0b,0x1e,0x1b,0x05,0x00,0x5d,
0x02,0x2e,0x04,0x14,0x2f,0x05,0x00,0x82,
0x02,0x2e,0x04,0x14,0x2f,0x05,0x00,0x77,
0x07,0x2e,0x0a,0x1e,0x1c,0x05,0x00,0x55,
};
	*/
#endif
