//*************************************************************************************************
//  Copyright (c) 	��������Ԫ΢�������޹�˾
//	�ļ�����	:  S_TouchKeyCFG.h
//	����		: 
//	ģ�鹦��	:  ���ؼ������ļ�
// 	�汾		:  V0.2
// 	���ļ�¼	:
//************************************************************************************************
#ifndef __S_TOUCHKEYCFG_H__
#define __S_TOUCHKEYCFG_H__
#define		    SOCAPI_SET_TOUCHKEY_TOTAL					  4
#define			SOCAPI_SET_TOUCHKEY_CHANNEL					  0x00000F00
unsigned int  code TKCFG[17] = {0,0,8,3,10,3000,200,100,2,0,0,4,0,65535,65535,65535,20}; 
unsigned char code TKChannelCfg[4][8]={
0x02,0x2e,0x04,0x20,0x1b,0x01,0x00,0x08,
0x02,0x2e,0x04,0x08,0x2f,0x05,0x00,0x91,
0x02,0x2e,0x04,0x08,0x2f,0x05,0x00,0x82,
0x02,0x2e,0x04,0x20,0x1b,0x02,0x00,0x0f,
};
#endif
