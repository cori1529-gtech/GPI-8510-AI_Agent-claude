/*
    SET.H

	VER 1.10 - 1997. 12. 8.  GI-8500
	  1. COMPILER  IAR 4.20

	Copyright (c) 1997 by G-TECH
	All Rights Reserved.
*/

#ifndef __SET_H__
#define __SET_H__


extern unsigned long gxhWeightBackup ;      
extern unsigned long gxhFilter;            
extern unsigned long gxhMotionBand;        
extern unsigned long gxhMotionBand;        
extern unsigned long gxhEndDelay;           
extern unsigned long gxhMyAddress ;         
extern unsigned long gxhBaudRate;           
extern unsigned long gxuPulseSetWeigh;      
extern unsigned long gxuOutputErrorWeigh;    
extern unsigned long gxuOutputHighErrorWeigh;   
extern  signed long gplCompensationWeight;  
extern	unsigned long gnlDailyTotal;
extern unsigned char gnhToday;
extern unsigned char Reg_Tel[20];
extern unsigned char Sms_Time[10];

extern unsigned char Sms_Time_First[4];
extern unsigned char Sms_Time_Second[4];
extern unsigned char Sms_Time_Third[4];
 
extern unsigned char	working_flag ;
 
 
//	unsigned long gnlGrossTotal	_at_	0xF824;
//	unsigned long gnlLossLimit	_at_	0xF828;
//	unsigned long gnlLossFall	_at_	0xF82C;
//	unsigned int  gnwIdCode		_at_	0xF830; /* id code */
//	unsigned char gnhToday		_at_	0xF836;
//	unsigned char gnhLcdWhich	_at_	0xF837;
//	int	      	  iPulseCounter	_at_	0xF838;	// Ver 1.26
//	unsigned char byErrorCounter _at_	0xF83A;	// Ver 1.26
//	unsigned long gnlHighLimit	_at_	0xF83B;	// Ver 1.30
//	unsigned long gnlHighFall	_at_	0xF83F;	// Ver 1.30
extern	unsigned long gnlPreDailyTotal[7];
//	unsigned long gnlPreOutput	_at_	0xF860;	// Ver 1.30 ~ 63
extern	unsigned char gnhDay;
    




/*-------------  SET mode function  ----------------*/
void set_mode(void);
void set_mode_simple(void);

void dwValueSetting(char number,int x, int y); //1114
long Get_Value(int x, int y); //1114




#endif  /*__SET_H__*/
/*--------------------  end of file ---------------------*/