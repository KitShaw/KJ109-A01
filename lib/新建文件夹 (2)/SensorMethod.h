//*************************************************************************************************
//  Copyright (c) 	��������Ԫ΢�������޹�˾
//	�ļ�����	:  SensorMethod.h
//	����		: 
//	ģ�鹦��	:  �����㷨ͷ�ļ�
// 	�汾	    :
// 	���ļ�¼	:
//	ע������	:    
//************************************************************************************************
#ifndef	_SENSORMETHOD_C_H
#define	_SENSORMETHOD_C_H

//�ⲿ����
extern	unsigned	char	xdata	SOCAPI_TouchKeyStatus;			//API?��?�����䨬?��obit7-��???����?������3������??								
extern 	unsigned 	char	idata	ConfirmTouchCnt;							
extern 	unsigned 	int		xdata 	RawData[];			 				
extern 	unsigned 	int		xdata	BaseLine[];	
extern	unsigned  	int	 	xdata  	FilterData[];					 
extern	unsigned 	int 	idata 	CurrentSensorKey;		 				
extern 	unsigned	char  	idata 	RestAreaCnt[]; 					 
extern 	unsigned	char  	idata 	TouchCnt[];
extern 	unsigned	char  	idata 	NoTouchCnt[];  			  
extern  unsigned	char 	xdata 	CurrentChannel[];			 
extern  unsigned    char    idata   LowFingerDataCnt[]; 
extern 				int     xdata 	DifferAccum[];
extern  unsigned	char  	xdata	FloatAreaCnt[]; 
extern 				char	xdata	SetNoiseThreshold;	
extern  unsigned 	char	idata	MultipleDealTpye;
extern  unsigned 	char    xdata   ICHG_RawData_Ratio;

//�ⲿ���ú���
extern	void  TouchKeyInit(void);									//����������ʼ��
extern	void  TouchKeyRestart(void);								//������һ�ִ���ɨ��
extern	unsigned  int  TouchKeyScan(void);					    	//�����������ݴ���				

#endif 
