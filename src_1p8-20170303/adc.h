/*
	adc.h
*/

#ifndef __ADC_H
#define __ADC_H


//#define  OPTION_POWER_COUNTER  20 // about 10 초  10초정도 날아오지 않으면  ad 모듈전원을 강제로 off 한다. 
#define  OPTION_POWER_COUNTER  80 // about 10 초  10초정도 날아오지 않으면  ad 모듈전원을 강제로 off 한다. 


long Get_adc_from_module(void); // ad module 로부터 무게값을 읽어오는 함수 
long Get_adc_from_module_20160629(void); // ad module 로부터 무게값을 읽어오는 함수 

#endif

/* end of file */
