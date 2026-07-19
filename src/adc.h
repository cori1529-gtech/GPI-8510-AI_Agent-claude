/*
	adc.h
*/

#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"
#include "type.h"
//#define  OPTION_POWER_COUNTER  20 // about 10 초  10초정도 날아오지 않으면  ad 모듈전원을 강제로 off 한다. 
#define  OPTION_POWER_COUNTER  80 // about 10 초  10초정도 날아오지 않으면  ad 모듈전원을 강제로 off 한다. 


#define		ADC_SATURATION      1048575L	/* ad saturation value */
#define	ADC_SATURATION      1048575L	/* ad saturation value */
#define	ADPERSEC			200			/* ad number per second */
#define	MAX_FILTER     200 

//#define		ADPERSEC			10			 ad number per second */
//#define MAX_FILTER	   10


// unsigned char gxhFilter;

extern unsigned long  ad_buf[200];
extern unsigned long  ad_buf_1[200];

extern unsigned char which_adc;


void           adc_initial();


unsigned long  adc_value(void);
unsigned long  read_adc( unsigned char );
unsigned long  read_filtered_adc(void);
unsigned long  read_filtered_adc_nowait(void);
void           write_adc( unsigned long);
unsigned long RDDATA(void);
void  ADWRITE(unsigned long cmd);
unsigned long RDDATA_NOWAIT(unsigned char which_reg);
unsigned long adc_value_nowait(void);

unsigned long  adc_value_1(void);
unsigned long  read_adc_1( unsigned char );
unsigned long  read_filtered_adc_1(void);
unsigned long  read_filtered_adc_nowait_1(void);
void           write_adc_1( unsigned long);
unsigned long RDDATA_1(void);
void  ADWRITE_1(unsigned long cmd);
unsigned long RDDATA_NOWAIT_1(unsigned char which_reg);
unsigned long adc_value_nowait_1(void);
long Get_adc_from_module(void); // ad module 로부터 무게값을 읽어오는 함수 
long Get_adc_from_module_20160629(void); // ad module 로부터 무게값을 읽어오는 함수 


/****** 2022년 CS5555****************/

void adc_delay(int time);
void CS5555_Clk_1(void); 
void CS5555_Clk_2(void); 


unsigned long Read_AD_Value(void); 

unsigned long Read_AD_Value_CS5555_1(void); 
unsigned long Read_AD_Value_CS5555_2(void); 	

unsigned long read_filtered_adc_CS5555_1(void);
unsigned long read_filtered_adc_CS5555_2(void);
unsigned long RDDATA_ads1251(void);
void INIT_CS5555(void);            //CS5555れ쯮ㅖ

extern unsigned long adc_timer;





/********2022년  CS5555******************/






#endif

/* end of file */
