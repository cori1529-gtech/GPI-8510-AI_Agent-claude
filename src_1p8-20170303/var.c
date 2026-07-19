/*
	var.c
*/

#include "def.h"
#include "type.h"
#include "stypes.h"
#include "lcd.h"	//lcd 색상 때문에

 unsigned long gxhWeightBackup ;      
 unsigned long gxhFilter;            
 unsigned long gxhMotionBand;        
 unsigned long gxhEndDelay;           
 unsigned long gxhMyAddress ;         
 unsigned long gxhBaudRate;           
 unsigned long gxuPulseSetWeigh;      
 unsigned long gxuOutputErrorWeigh;    
 unsigned long gxuOutputHighErrorWeigh;   
 signed long gplCompensationWeight; 
 unsigned long gxwStatusSendInterval;	//상태정보 전송 시간(분)
 long temp_weight; 
  
 unsigned long gnlDailyTotal;
 volatile unsigned long gnlPreDailyTotal[7];
 unsigned int gnuDay;
 int		  giToday;
 unsigned char Reg_Tel[20];
 unsigned char Sms_Time[10];
 
 unsigned char Sms_Time_First[4];
 unsigned char Sms_Time_Second[4];
 unsigned char Sms_Time_Third[4];
 long gnlDisplay;

long	gnlSerialNo; 	//장비 시리얼 번호
long	gnlMyAddress;	//장비 ID(어드레스)
long	gnlHenhouseNo;	//계사 번호
long	gnlUseLTE;		//LTE 사용여부 (0:지그비, 1:LTE)

//<<<<<<<< adc.c
u8 rotation=1;
signed short int Adc_Counter;


signed long v_adc1_buf,raw_data,diff1,diff2;


u8 One_Weight_Ok=0;
u8 Two_Weight_Ok=0;
u8 Three_Weight_Ok=0;
u8 Four_Weight_Ok=0;
u8 Five_Weight_Ok=0;
u8 Six_Weight_Ok=0;


u8 One_Weight_Error_Counter=0;
u8 Two_Weight_Error_Counter=0;
u8 Three_Weight_Error_Counter=0;
u8 Four_Weight_Error_Counter=0;
u8 Five_Weight_Error_Counter=0;
u8 Six_Weight_Error_Counter=0;

// adc.c >>>>>>>>>>>>>>>>>>>>>>>


//<<<<<<<<<<<<< tcpip_send 
u8 mon_One_Weight_Ok = 0;
u8 mon_Two_Weight_Ok = 0;
u8 mon_Three_Weight_Ok = 0;
u8 mon_Four_Weight_Ok = 0;
u8 mon_Five_Weight_Ok = 0;
u8 mon_Six_Weight_Ok = 0;

long	mon_gxlGrossWeight = 0;

unsigned long  mon_One_Weight = 99999;
unsigned long  mon_Two_Weight = 99999;
unsigned long  mon_Three_Weight = 99999;
unsigned long  mon_Four_Weight = 99999;
unsigned long  mon_Five_Weight = 99999;
unsigned long  mon_Six_Weight = 99999;

long	mon_raw_adc;

// tcpip_send >>>>>>>>>>>>>>>>>>>>>>>>>>>>

//<<<<<<<< main.c
long gxlGrossWeight;


unsigned char	Weight_flag=0; // 1 // 모듈로 부터 a/d 값 수신 ok

unsigned char	working_flag = 0;		/* working between start and stop */

unsigned long  One_Weight=99999;
unsigned long  Two_Weight=99999;
unsigned long  Three_Weight=99999;
unsigned long  Four_Weight=99999;
unsigned long  Five_Weight=99999;
unsigned long  Six_Weight=99999;

struct jangbi_info jangbi;

//>>>>>>>>>>>>> main.c


//<<<<<<<<<<<<< tcpip_send.c
volatile unsigned long gnlTick = 0;

// tcpip_send.c >>>>>>>>>>>>>>>

//<<<<<<<<<<<<< lcd.h
unsigned int  POINT_COLOR = RED;
unsigned int  BACK_COLOR = 0XFFFF;
// lcd.h >>>>>>>>>>>>>>>>>>>>>>

u32 rtc_year, rtc_month, rtc_date, rtc_hour, rtc_min, rtc_sec;
int	gbGetCdmaTime = 0;

u32	cdma_interval_timer_ms = 10;

//<<<<<<<<<<<<<< periodic event
int	gbSecEvent = 0;
int gbMinEvent = 1;
int gbHourEvent = 0;
int gbDayEvent = 0;
// periodic event >>>>>>>>>>>>>>



//<<<<<<<<<<<<<< websms
int	giWebSMSConnection = 4;
int	giWebSMSPreConnection = 44;
int gbWebSMSConnection = 1;
// websms >>>>>>>>>>>>>>>>>>>>


//<<<<<<<<<<<<<< usart.c
unsigned char	RxBuffer2_sms[MAX_RXBUFFER2_SMS];
unsigned int	RxCounter2_sms;
unsigned char	Usart3_sms_received_flag=0 ;	//sms 받으면 1
char sms_receiving_flag = 0;	//# 로 시작하는 sms 받는중이면 
// usart.c >>>>>>>>>>>>>>>>>>>>


/* end of file */
