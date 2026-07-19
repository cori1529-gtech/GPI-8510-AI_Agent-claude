/*
	var.h
*/


#ifndef	__VAR_H___
#define	__VAR_H___

#include "type.h"
#include "stypes.h"

#define	SEND_BAICHUL	1
#define	SEND_SETVALUE	2
#define	SEND_STATUS		3
#define	SMS_RECEIVED	59


//<<<<<<<<< main.c
extern s16 Adc_Counter;

extern unsigned char	working_flag;		/* working between start and stop */

extern unsigned long  One_Weight;
extern unsigned long  Two_Weight;
extern unsigned long  Three_Weight;
extern unsigned long  Four_Weight;
extern unsigned long  Five_Weight;
extern unsigned long  Six_Weight;

extern	struct jangbi_info jangbi;

//>>>>>>>>>>>>> main.c

//<<<<<< adc.c 


extern u8 One_Weight_Error_Counter;
extern u8 Two_Weight_Error_Counter;
extern u8 Three_Weight_Error_Counter;
extern u8 Four_Weight_Error_Counter;
extern u8 Five_Weight_Error_Counter;
extern u8 Six_Weight_Error_Counter;
			
extern u8 One_Weight_Ok;
extern u8 Two_Weight_Ok;  
extern u8 Three_Weight_Ok;
extern u8 Four_Weight_Ok;
extern u8 Five_Weight_Ok;
extern u8 Six_Weight_Ok;


extern u8 rotation;

extern long gxlGrossWeight;
extern  u8  Weight_flag; // 1 // 모듈로 부터 a/d 값 수신 ok

extern signed long v_adc1_buf,raw_data,diff1,diff2;

//<<<<<< adc.c >>>>>>>>>>>>>>>>>>>

//<<<<<<<<<<<<< tcpip_send 
extern u8 mon_One_Weight_Ok;
extern u8 mon_Two_Weight_Ok;
extern u8 mon_Three_Weight_Ok;
extern u8 mon_Four_Weight_Ok;
extern u8 mon_Five_Weight_Ok;
extern u8 mon_Six_Weight_Ok;

extern long	mon_gxlGrossWeight;

extern unsigned long  mon_One_Weight;
extern unsigned long  mon_Two_Weight;
extern unsigned long  mon_Three_Weight;
extern unsigned long  mon_Four_Weight;
extern unsigned long  mon_Five_Weight;
extern unsigned long  mon_Six_Weight;

extern long	mon_raw_adc;

// tcpip_send >>>>>>>>>>>>>>>>>>>>>>>>>>>>

//<<<<<<<<< set.c
extern unsigned long gxhWeightBackup ;      
extern unsigned long gxhFilter;            
extern unsigned long gxhMotionBand;        
extern unsigned long gxhEndDelay;           
extern unsigned long gxhMyAddress ;         
extern unsigned long gxhBaudRate;           
extern unsigned long gxuPulseSetWeigh;      
extern unsigned long gxuOutputErrorWeigh;    
extern unsigned long gxuOutputHighErrorWeigh;   
extern signed long gplCompensationWeight; 
extern unsigned long gxwStatusSendInterval;	//상태정보 전송 시간(분)
extern long temp_weight; 
  
extern unsigned long gnlDailyTotal;
extern volatile unsigned long gnlPreDailyTotal[7];
extern unsigned int gnuDay;
extern int			giToday;
extern unsigned char Reg_Tel[20];
extern unsigned char Sms_Time[10];
 
extern unsigned char Sms_Time_First[4];
extern unsigned char Sms_Time_Second[4];
extern unsigned char Sms_Time_Third[4];
extern long gnlDisplay;

extern long	gnlSerialNo; 	//장비 시리얼 번호
extern long	gnlMyAddress;	//장비 ID(어드레스)
extern long	gnlHenhouseNo;	//계사 번호
extern long	gnlUseLTE;		//LTE 사용여부 (0:지그비, 1:LTE)

//>>>>>>>>>>>>> set.c



//<<<<<<<<<<<<< tcpip_send.c
extern volatile unsigned long gnlTick;

// tcpip_send.c >>>>>>>>>>>>>>>

//<<<<<<<<<<<<< lcd.h
extern unsigned int  POINT_COLOR;
extern unsigned int  BACK_COLOR;
// lcd.h >>>>>>>>>>>>>>>>>>>>>>

extern u32 rtc_year, rtc_month, rtc_date, rtc_hour, rtc_min, rtc_sec;
extern int	gbGetCdmaTime;

extern unsigned long	cdma_interval_timer_ms;

//<<<<<<<<<<<<<< periodic event
extern int gbSecEvent;
extern int gbMinEvent;
extern int gbHourEvent;
extern int gbDayEvent;
// periodic event >>>>>>>>>>>>>

//<<<<<<<<<<<<<< websms
extern int	giWebSMSConnection;
extern int	giWebSMSPreConnection;
extern int gbWebSMSConnection;
// websms >>>>>>>>>>>>>>>>>>>>

//<<<<<<<<<<<<<< usart.c
extern unsigned char	RxBuffer2_sms[];
extern unsigned int		RxCounter2_sms;
extern unsigned char	Usart3_sms_received_flag;	//sms 받으면 1
extern char sms_receiving_flag;	//# 로 시작하는 sms 받는중이면 
// usart.c >>>>>>>>>>>>>>>>>>>>

#endif	//__VAR_H___

/* end of file */
