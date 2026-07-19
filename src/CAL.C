/*
   Filename  :  cal.c

   History:
	Ver 1.130 (1998. 1. 7)
	  + cal.c
		- Change Compiler IAR 4.2 to KEIL 5.02
		- Change char hLoop to BYTE  hLoop;
	Ver 1.10 (1997. 12. 4)
	  + cal.c
		- OneShot calibration
		- Changed maximum d from 999 to 9999.
		- Changed maximum Capacity from 50000 to 9999999.
		- Added unsigned int dp_value_set from set.c
	Ver 1.10 (1996. 3. 30) - ??????
	based  ver 1.00 (1996. 2. 2x)
	   1. added cal 6
	   2. when cal 5 is error, Let's go cal 3
*/

/*#include <io51.h>*/       /* required header for output, bit_set, etc. */
//#include <stdio.h>
//#include "adc.h"
#include "cal.h"
#include "key.h"
#include "hw_config.h"
#include "lcd70.h"
#include "usart.h"
#include "rs485.h"
#include "lcd.h"
#include "key.h"
#include "led.h"
#include "spi.h"
#include "rs485.h"
#include "adc.h"
#include "lcd70.h"
#include "global.h"
#include "i2c_ee.h"
#include "cal.h"
#include "var.H"
#include "tcpip_send.h"


#include "i2c_ee.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"	



#include "stm32f10x.h"
#include "stm32f10x_lib.h"
#include "stm32f10x_it.h"
#include "stm32f10x_iwdg.h"
#include "cortexm3_macro.h"
#include "hw_config.h"
#include "stm32f10x_rcc.h"
//#include "stm32f10x_systick.h"


#include "stm32f10x_lib.h"
#include "stm32f10x_it.h"
#include "stm32f10x_rcc.h"




#include "def.h"
#include "Sflash.h"
#include "adc.h"
#include "buzzer.h"
#include "cal.h"
#include "global.h"
#include "i2c_ee.h"
#include "key.h"
#include "lcd.h"
#include "lcd70.h"
#include "led.h"
#include "main.h"
#include "rs485.h"
#include "rtc.h"
#include "var.h"
#include "sms.h"
#include "spi.h"
#include "tcpip_send.h"
#include "timer.h"
#include "usart.h"
#include "type.h"

#include "debug.h"

//#include "def.h"
//#include "eeprom.h"
//#include "var.h"
//#include "vfd.h"
//#ifdef	 __USE_LCD__
//#include "lcd.h"
//#endif

unsigned long	dZeroValue[8];	/* zero calibration data - gain 1 - 64 */
unsigned long	dFullValue[8];	/* full calibration data - gain 1 - 64 */

unsigned  int  v_division,v_division_2;
unsigned  int  v_ei_multiply_factor;
int  v_div ;
char  str_cal[50];
					   
unsigned long  v_zero,v_zero_2;
unsigned long v_full;
unsigned long  v_span,v_span_2;		// ver 1.30
unsigned long  v_e_resolution, v_i_resolution;
unsigned long   v_capacity,v_setting_weight,v_capacity_2;
unsigned long  lc_capacity;
unsigned char   lc_number;
signed long v_adc_org,v_adc_org_2;
unsigned long v_adc1_org;
unsigned char v_decimal_point;
float gnfFactor,gnfFactor_2;
char cal_selection;

   char str_1[50];
    int i_1;
	long zero_1,zero_2,zero_3,zero_4;
	long zero_sum;
	char One_Weight_Ok_1=0,Two_Weight_Ok_1=0,Three_Weight_Ok_1=0,Four_Weight_Ok_1=0;
    char rotation_1=1;; // ㅇ
   
#define	Y_AXIS_SPACE_CAL	10

#define X_START     10
#define X_INCREMENT 30
#define Y_START     50
#define Y_INCREMENT 55


void cal_mode()
{
	 		union { u32 dw; u16 u[2]; u8 c[4];} temp;
  
	  char sT0[50];
	   char ch;
	   char exit_flag=0;
	    long  ret_val;
 	
		lcd_cls();
	  lcd_clear();
		if(gnlLanguage==1)
		GLCD_string (0,0 ,"  무게조정  ");	
		else
	  GLCD_string (0,0 ,"   CAL  MODE  ");
		Delay_ms(1000);
	 // gxhFilter=9;
	
	  v_division = Get_division();// division setting 
	 // v_capacity=Get_capacity(); // capa setting
		
		  v_capacity=Get_Capacity_New(v_capacity); // capa setting(); // capa setting
	
	
	 	v_ei_multiply_factor = 10;	//10; 2016.3.12 // 카운트 값(50g 당 10개의 카운트
		sprintf(sT0, "v_ei_multiply_factor : %d", v_ei_multiply_factor);
		USART1_puts(sT0);
					
		v_e_resolution = v_capacity / v_division;	 
		sprintf(sT0, "v_e_resolution : %d", v_e_resolution);
		USART1_puts(sT0);
					
		v_i_resolution = v_e_resolution * (long)v_ei_multiply_factor;			     // 50000 내부 resolution
		sprintf(sT0, "v_i_resolution : %d", v_i_resolution);
		USART1_puts(sT0);
	  v_zero= cal_zero_span_ad_set(CAL_ZERO_AD_SET);  //zero span
	   v_setting_weight=Set_Weight();  // test weight  
		 lcd_normal();
		 	ret_val = cal_zero_span_ad_set(CAL_SPAN_AD_SET);
			v_full = ret_val; // 분동 무게 a/d  값 
			ret_val=calc_factor();
			ret_val = little_span_adjust();	//무게환산해서 표시해주는곳 
			write_factors();
			 /*
			 
			 
			 #define ADR_gnfFactor             0
       #define ADR_v_zero                4 
      #define ADR_v_adc_org             8 
      #define ADR_v_division           12 //
      #define ADR_v_capacity           16// 상한무게 설정 
      #define ADR_v_span               24    //  빈통무게 설정 

			*/ 
			 
			 
		 
}

void cal_mode_2()
{
	 		union { u32 dw; u16 u[2]; u8 c[4];} temp;
  
	  char sT0[50];
	   char ch;
	   char exit_flag=0;
	    long  ret_val;
 	
		lcd_clear();
	 GLCD_string (0,0 ,"    CAL  MODE   ");
		Delay_ms(1000);
	  gxhFilter=9;
	
	  v_division_2 = Get_division();// division setting 
	  //v_capacity_2=Get_capacity(); // capa setting
			
		v_capacity_2=Get_Capacity_New(v_capacity); // capa setting(); // capa setting
		
			
			
			
	 	v_ei_multiply_factor = 10;	//10; 2016.3.12 // 카운트 값(50g 당 10개의 카운트
		sprintf(sT0, "v_ei_multiply_factor : %d", v_ei_multiply_factor);
		USART1_puts(sT0);
		v_e_resolution = v_capacity_2 / v_division_2;	 
		sprintf(sT0, "v_e_resolution : %d", v_e_resolution);
		USART1_puts(sT0);
		v_i_resolution = v_e_resolution * (long)v_ei_multiply_factor;			     // 50000 내부 resolution
		sprintf(sT0, "v_i_resolution : %d", v_i_resolution);
		USART1_puts(sT0);
	    v_zero_2= cal_zero_span_ad_set(CAL_ZERO_AD_SET);  //zero span
      v_setting_weight=Set_Weight();  // test weight  
		 	ret_val = cal_zero_span_ad_set(CAL_SPAN_AD_SET);
			v_full = ret_val; // 분동 무게 a/d  값 
			ret_val=calc_factor_2();
			ret_val = little_span_adjust_2();	//무게환산해서 표시해주는곳 
			
			
	
	

	 
			write_factors();
			 /*
			 
			 
			 #define ADR_gnfFactor             0
       #define ADR_v_zero                4 
      #define ADR_v_adc_org             8 
      #define ADR_v_division           12 //
      #define ADR_v_capacity           16// 상한무게 설정 
      #define ADR_v_span               24    //  빈통무게 설정 

			*/ 
			 
			 
		 
}



long little_span_adjust()	//
{
	int  exit_flag=0;	//, sign;
	char  iz;
	long temp_buffer;
	s32  gxlGrossWeight_1;
	char str[30];
	s32  v_i_value;
	long ret_val = 0;
	
	lcd_clear();
  if(gnlLanguage==1)
	{
	GLCD_string(0,0, "현재무게확인");
	lcd_inverse();
	GLCD_string(0,6, "확인키:다음");
	lcd_normal();
	}// english	
	else
	{	
	GLCD_string(0,0, "CURRENT WEIGHT");
	lcd_inverse();
	GLCD_string(0,6, "ENTER:NEXT");
	lcd_normal();
	}
	
	if(which_adc==1)
	{	
	 for(iz=0;iz<10;iz++) raw_data = (float)read_filtered_adc_CS5555_1();
	}
	else
	{	
	for(iz=0;iz<100;iz++) raw_data = (float)read_filtered_adc();
	}
	
	v_adc1_org = v_zero;
	v_adc_org = (long)(((float)v_adc1_org) * gnfFactor);	// 제로에 대한 count 값
	sprintf(str, "raw_data : %f", raw_data); USART1_puts(str);
	sprintf(str, "v_adc1_org : %d", v_adc1_org); USART1_puts(str);
	sprintf(str, "v_adc_org : %d", v_adc_org);	USART1_puts(str);
	sprintf(str, "gnfFactor : %f", gnfFactor);	USART1_puts(str);

	do
	{
		if(keypush_8510())
		{
			 if(KeyCode == UP)
			 {
				 v_adc_org = (long)(((float)v_adc1_org) * gnfFactor);	// 제로
			 }
			 if(KeyCode == DOWN)
			 {
				 v_adc_org = (long)(((float)v_zero) * gnfFactor);	// 제로
			 }
			
			 else if(KeyCode==ENTER)
			 {
				 exit_flag=1;
				// write_factors();
				 ret_val = SET_FORWARD;
//				 cal_state = 7;
			 }
		 }

		/*---------------------- READ A/D -------------------*/



		 //raw_data = (float)read_filtered_adc();		// ad 값으 읽어온다ㅣ 
		if(which_adc==1)
		{
		 raw_data = read_filtered_adc_CS5555_1();		// ad 값으 읽어온다ㅣ
		}
  else		
			{
		 raw_data = read_filtered_adc();		// ad 값으 읽어온다ㅣ
		} 
		 
		v_adc1_buf = (s32)(gnfFactor * raw_data);	// count 값 입력(무게에 대한)
		/*------------ CALCULATE IVALUE, EVALUE -------------*/
		v_i_value  =  v_adc1_buf - v_adc_org;	//현재 카운트 값에서 zero 에서의 카운트갓을 뺀다. 
  /* 
		 
		sprintf(str, "raw_data_2 : %7ld", (long)raw_data);
		USART1_puts(str);
		 
		sprintf(str, "v_adc1_buf_2 : %7ld", (long)v_adc1_buf);
		USART1_puts(str);
*/
		 
		 
		 
		 
		 
		if (0 <= v_i_value) 
		{
			gxlGrossWeight_1 = (v_i_value + (v_ei_multiply_factor/2));  // 10000+(10/2)=10005
			gxlGrossWeight_1=(gxlGrossWeight_1 / v_ei_multiply_factor);	//10005/10=1000
			gxlGrossWeight_1=gxlGrossWeight_1 * v_division;					//1000*5=5000
//			sign=0;
		}
		else  
		{
//			sign=1;
//			v_i_value=v_i_value*-1;
			gxlGrossWeight_1 = (v_i_value + (v_ei_multiply_factor/2));
			gxlGrossWeight_1=(gxlGrossWeight_1 / v_ei_multiply_factor);
			gxlGrossWeight_1=gxlGrossWeight_1 * v_division;
		}

     if(gnlLanguage==1)//korean		
		 sprintf(str, "무게:%6ld kg",(long)gxlGrossWeight_1); // A/D값 
     else 
     sprintf(str, "WEIGHT:%6ld kg",(long)gxlGrossWeight_1); // A/D값 
     
		GLCD_string(0,2, str);
		
/*
		sprintf(str, "gxlGrossWeight_1 : %ld", gxlGrossWeight_1);
		USART1_puts(str);
*/
//		Display_Weight(gxlGrossWeight_1);	  //50.00



	} while (!exit_flag);
//	gxhFilter = gxhFilterBackup;
////#endif
	return ret_val;
} /* little() */

long little_span_adjust_2()	//
{
	int  exit_flag=0;	//, sign;
	char  iz;
	long temp_buffer;
	s32  gxlGrossWeight_1;
	char str[30];
	s32  v_i_value;
	long ret_val = 0;
	
  lcd_clear();
  if(gnlLanguage==1)
	{
	GLCD_string(0,0, "현재무게확인");
	lcd_inverse();
	GLCD_string(0,6, "확인키:다음");
	lcd_normal();
	}// english	
	else
	{	
	GLCD_string(0,0, "CURRENT WEIGHT");
	lcd_inverse();
	GLCD_string(0,6, "ENTER:NEXT");
	lcd_normal();
	}
	
	
	
	
	
	
////	#ifdef dpfdpf 
	if(which_adc==1) 
	{	
	 for(iz=0;iz<100;iz++) raw_data = (float)read_filtered_adc_CS5555_2();
	}
	else
	{	
	 for(iz=0;iz<100;iz++) raw_data = (float)read_filtered_adc_1();
	}
	
	v_adc1_org = v_zero_2;
	v_adc_org = (long)(((float)v_adc1_org) * gnfFactor_2);	// 제로에 대한 count 값

	sprintf(str, "raw_data : %f", raw_data); USART1_puts(str);
	sprintf(str, "v_adc1_org : %d", v_adc1_org); USART1_puts(str);
	sprintf(str, "v_adc_org : %d", v_adc_org);	USART1_puts(str);
	sprintf(str, "gnfFactor_2 : %f", gnfFactor_2);	USART1_puts(str);

	do
	{
		if(keypush_8510())
		{
			 if(KeyCode == UP)
			 {
				 v_adc_org = (long)(((float)v_adc1_org) * gnfFactor_2);	// 제로
			 }
			 if(KeyCode == DOWN)
			 {
				 v_adc_org = (long)(((float)v_zero_2) * gnfFactor_2);	// 제로
			 }
			
			 else if(KeyCode==ENTER)
			 {
				 exit_flag=1;
				// write_factors();
				 ret_val = SET_FORWARD;
//				 cal_state = 7;
			 }
		 }

		/*---------------------- READ A/D -------------------*/



		 //raw_data = (float)read_filtered_adc();		// ad 값으 읽어온다ㅣ 
		
		 
		 if(which_adc==1) 
		 {	 
		    raw_data = read_filtered_adc_CS5555_2();		// ad 값으 읽어온다ㅣ
		 }
		 else
		 {	 
		    raw_data = read_filtered_adc_1();		// ad 값으 읽어온다ㅣ
		 }
		 
		 
		 v_adc1_buf = (s32)(gnfFactor_2 * raw_data);	// count 값 입력(무게에 대한)
		/*------------ CALCULATE IVALUE, EVALUE -------------*/
		v_i_value  =  v_adc1_buf - v_adc_org;	//현재 카운트 값에서 zero 에서의 카운트갓을 뺀다. 
  /* 
		 
		sprintf(str, "raw_data_2 : %7ld", (long)raw_data);
		USART1_puts(str);
		 
		sprintf(str, "v_adc1_buf_2 : %7ld", (long)v_adc1_buf);
		USART1_puts(str);
*/
		 
		 
		 
		 
		 
		if (0 <= v_i_value) 
		{
			gxlGrossWeight_1 = (v_i_value + (v_ei_multiply_factor/2));  // 10000+(10/2)=10005
			gxlGrossWeight_1=(gxlGrossWeight_1 / v_ei_multiply_factor);	//10005/10=1000
			gxlGrossWeight_1=gxlGrossWeight_1 * v_division_2;					//1000*5=5000
//			sign=0;
		}
		else  
		{
//			sign=1;
//			v_i_value=v_i_value*-1;
			gxlGrossWeight_1 = (v_i_value + (v_ei_multiply_factor/2));
			gxlGrossWeight_1=(gxlGrossWeight_1 / v_ei_multiply_factor);
			gxlGrossWeight_1=gxlGrossWeight_1 * v_division_2;
		}

		
		 sprintf(str, "SILO_2:%7ld",(long)gxlGrossWeight_1); // A/D값 
    GLCD_string(0,2, str);
		
/*
		sprintf(str, "gxlGrossWeight_1 : %ld", gxlGrossWeight_1);
		USART1_puts(str);
*/
//		Display_Weight(gxlGrossWeight_1);	  //50.00



	} while (!exit_flag);
//	gxhFilter = gxhFilterBackup;
////#endif
	return ret_val;
} /* little() */




int calc_factor()
{
	char str[30];
	float ratio;
//	u8  exit_flag = 0;
	u32 temp_full;
	u32 low_limit,		/* span low limit */
		  high_limit;		/* span high limit */
//	char  repeat_flag = 0;
//	long  v_full, v_span;
	u8  bAdcLow = 0;			/* zero, full value high */
	u8  bAdcHigh = 0;			/* zero, full value low */
	u8  bAdcSaturation = 0;	/* ad saturation */
	long	ret_val;

  lcd_clear();
	
	
	bAdcLow = bAdcHigh = bAdcSaturation = 0;
	ratio = (float)((float)v_capacity/(float)v_setting_weight);// 250/40 =16
	
	{
	
		v_span = v_full- v_zero;	// 40 kg 에서의 span 값  = 12345-6780=5565  

		USART1_puts("1234567890");
		sprintf(str, "V_ZERO=%ld", v_zero);
		USART1_puts(str);
		sprintf(str, "V_FULL=%ld", v_full);
		USART1_puts(str);
		sprintf(str, "V_SPAN=%ld", v_span);
		USART1_puts(str);
		
			sprintf(str, "ratio=%f", ratio);
		USART1_puts(str);
		
		/*
		sprintf(str, "V_LIMIT=%f", (float)FULL_LIMIT/ratio);
		USART1_puts(str);
		sprintf(str, "F_LIMIT=%ld", FULL_LIMIT);
		USART1_puts(str);
		sprintf(str, "ratio=%f", ratio);
		USART1_puts(str);
*/
/*
vputnumwithled(1,0, v_zero); Delay_ms(10000);
vputnumwithled(1,1, v_full); Delay_ms(10000);
vputnumwithled(1,2, v_span); Delay_ms(10000);
vputnumwithled(1,3, ratio*1000); Delay_ms(10000);
vputnumwithled(1,4, FULL_LIMIT/ratio); Delay_ms(10000);
vputnumwithled(0,0, v_span); Delay_ms(10000);
*/
		/*************************************************************************************************************
		capa 250kg, 1div 50g 일 때 분해도는 1/5000이고 1눈금당 10개의 카운트가 있어야 하므로 디지털 스팬은 50000 이상이어야 한다.
		만약 기준분동을 40kg을 올린다면 디지털 스팬은 50000*(40/250) 이상이어야 한다. 이 값을 low_limit 라는 변수에 담는것이다.
		*************************************************************************************************************/
		low_limit = (float)v_i_resolution/ratio;// v_capacity : v_i_resolution = v_setting_weight : low_limit
		sprintf(str, "low_limit : %d", low_limit);
		USART1_puts(str);

		high_limit = v_span * ratio + v_zero;
		sprintf(str, "high_limit : %d", high_limit);
		USART1_puts(str);

		sprintf(str, "err25 : %f", ((float)FULL_LIMIT/(ratio)));
		USART1_puts(str);

		sprintf(str, "FULL_LIMIT : %d", FULL_LIMIT);
		USART1_puts(str);

		if (v_span < low_limit)
		{
			bAdcLow++;
//			goto mainload;
		} /* if (temp_full) */
		else if(((float)FULL_LIMIT/ratio) < v_span)
		{
///vputnumwithled(0,1, ((float)FULL_LIMIT/ratio)); Delay_ms(10000);
			bAdcHigh++;
//			goto mainload;
		} /* if (temp_full) */
//		else if (ADC_SATURATION <= high_limit)
//		{
//			bAdcSaturation++;
//			goto mainload;
//		}
		else  //  cal에 성공하면 
		{
			
		 sprintf(str, "V_SPAN=%ld", v_span);
		 USART1_puts(str);
			
			sprintf(str, "v_i_resolution : %d", v_i_resolution);
		USART1_puts(str);
			
						sprintf(str, "ratio=%f", ratio);
		USART1_puts(str);
	
			
			
			gnfFactor = (float)(((float)v_i_resolution)/((float)v_span*ratio));
			sprintf(str, "gnfFactor : %f", gnfFactor);
			USART1_puts(str);

			// =50000/(5565*16)=0.56148231   ->span값 (250 kg )에다가 gnfFactor 값을 곱하면 50000( 분해도 *10눈금 ) 
/*
			ctemp.f = gnfFactor;
			write_eep(ADR_gnfFactor, ctemp.u[0]);
			Delay_ms_time2(10);
			write_eep(ADR_gnfFactor+1, ctemp.u[1]);
			Delay_ms_time2(10);
								
			gnfFactor = gnfFactor;
*/			
		//	gnfFactor=1.553181;
			v_adc_org = (long)(((float)v_zero)*gnfFactor);	 // 무부하상태에서의 카운트값 **
			bAdcLow = bAdcHigh = bAdcSaturation = 0;
/*
			sprintf(str, "v_zero = %8ld g ", v_zero); GLCD_string (0, 2, str); Delay_ms(5000);
			sprintf(str, "v_full = %8ld g ", v_full); GLCD_string (0, 2, str); Delay_ms(5000);
			sprintf(str, "ratio  = %8.5f g ", ratio); GLCD_string (0, 2, str); Delay_ms(5000);
			sprintf(str, "v_e_re = %8ld   ", v_e_resolution); GLCD_string (0, 2, str); Delay_ms(5000);
			sprintf(str, "v_i_re = %8ld   ", v_i_resolution); GLCD_string (0, 2, str); Delay_ms(5000);
			sprintf(str, "v_span = %8ld   ", v_span); GLCD_string (0, 2, str); Delay_ms(5000);
			sprintf(str, "factor = %8ld ", (long)(gnfFactor*1000000)); GLCD_string (0, 2, str); Delay_ms(20000);
//          sprintf(str, "factor 01 = %8f ", i, gnfFactor); GLCD_string (0, 2, str); Delay_ms(20000);
*/
//			repeat_flag = 0;
//			exit_flag = 1;
//			cal_state = 6;
			//GLCD_string(0,2, "  SUCCESS      ");
			if(gnlLanguage==2) // korean
			GLCD_string(0,2, "  SUCCESS !! ");
      else if(gnlLanguage==1)
			GLCD_string(0,2, " 무게조정 성공!!");
				

			Delay_ms(2000);
			//	GLCD_string(0,2, " 무게조정 성공");
		//	vputs("OK    ");
			
			
			Delay_ms(2000);
		}
	} /* for (hIndex = 0; hIndex < 7; hIndex++) */
	
	if (bAdcLow)
	{
		GLCD_string(0,1, "ERROR 24 !!!");
		Delay_ms(3000);
		//retry_message();
		///vputs("ERR24 ");
		///Delay_ms(500);
	//	vputerr("ERR24 ");

		//VfdPrintf("GAIN%2u", (u16)hLoop); Delay_ms(1000);
		//fast_evalue(temp_full - v_zero);  Delay_ms(1000);
		//flag = 0;  /* cal 3 */
		//repeat_flag = 1;
		//cal_state = 4;
		
//		repeat_flag = 0;
//		exit_flag = 1;
		ret_val = SET_RESTART;
//		cal_state = 0;
	}
	else if (bAdcHigh)
	{
		GLCD_string(0,2, "ERROR 25 : SPAN HIGH");
		///vputs("ERR25 ");
		//retry_message();
		///Delay_ms(500);
		//vputerr("ERR25 ");

		//VfdPrintf("GAIN%2u", (u16)hLoop); Delay_ms(1000);
		//fast_evalue(temp_full - v_zero);  Delay_ms(1000);
		//flag = 0;  /* cal 3 */
//		repeat_flag = 0;
//		exit_flag = 1;
//		cal_state = 0;
		ret_val = SET_RESTART;
	}
	else if (bAdcSaturation)
	{
		GLCD_string(0,2, "ERROR 29 : AD OVER  ");
		///vputs("ERR29 ");
		//retry_message();
		///Delay_ms(500);
		//vputerr("ERR29 ");

		//VfdPrintf("GAIN%2u", (u16)hLoop); Delay_ms(1000);
		//fast_evalue(temp_full - v_zero);  Delay_ms(1000);
		//flag = 0;  /* cal 3 */
//		repeat_flag = 0;
//		exit_flag = 1;
//		cal_state = 0;
		ret_val = SET_RESTART;
	}
	else
	{
		ret_val = SET_FORWARD;
	}

	return ret_val;
} /* of zero_set() */


int calc_factor_2()
{
	char str[30];
	float ratio;
//	u8  exit_flag = 0;
	u32 temp_full;
	u32 low_limit,		/* span low limit */
		  high_limit;		/* span high limit */
//	char  repeat_flag = 0;
//	long  v_full, v_span;
	u8  bAdcLow = 0;			/* zero, full value high */
	u8  bAdcHigh = 0;			/* zero, full value low */
	u8  bAdcSaturation = 0;	/* ad saturation */
	long	ret_val;

  lcd_clear();
	
	
	bAdcLow = bAdcHigh = bAdcSaturation = 0;
	ratio = (float)((float)v_capacity_2/(float)v_setting_weight);// 250/40 =16
	
	{
	
		v_span = v_full- v_zero_2;	// 40 kg 에서의 span 값  = 12345-6780=5565  

		USART1_puts("1234567890");
		sprintf(str, "V_ZERO=%ld", v_zero_2);
		USART1_puts(str);
		sprintf(str, "V_FULL=%ld", v_full);
		USART1_puts(str);
		sprintf(str, "V_SPAN=%ld", v_span);
		USART1_puts(str);
		
			sprintf(str, "ratio=%f", ratio);
		USART1_puts(str);
		
		/*
		sprintf(str, "V_LIMIT=%f", (float)FULL_LIMIT/ratio);
		USART1_puts(str);
		sprintf(str, "F_LIMIT=%ld", FULL_LIMIT);
		USART1_puts(str);
		sprintf(str, "ratio=%f", ratio);
		USART1_puts(str);
*/
/*
vputnumwithled(1,0, v_zero); Delay_ms(10000);
vputnumwithled(1,1, v_full); Delay_ms(10000);
vputnumwithled(1,2, v_span); Delay_ms(10000);
vputnumwithled(1,3, ratio*1000); Delay_ms(10000);
vputnumwithled(1,4, FULL_LIMIT/ratio); Delay_ms(10000);
vputnumwithled(0,0, v_span); Delay_ms(10000);
*/
		/*************************************************************************************************************
		capa 250kg, 1div 50g 일 때 분해도는 1/5000이고 1눈금당 10개의 카운트가 있어야 하므로 디지털 스팬은 50000 이상이어야 한다.
		만약 기준분동을 40kg을 올린다면 디지털 스팬은 50000*(40/250) 이상이어야 한다. 이 값을 low_limit 라는 변수에 담는것이다.
		*************************************************************************************************************/
		low_limit = (float)v_i_resolution/ratio;// v_capacity : v_i_resolution = v_setting_weight : low_limit
		sprintf(str, "low_limit : %d", low_limit);
		USART1_puts(str);

		high_limit = v_span * ratio + v_zero_2;
		sprintf(str, "high_limit : %d", high_limit);
		USART1_puts(str);

		sprintf(str, "err25 : %f", ((float)FULL_LIMIT/(ratio)));
		USART1_puts(str);

		sprintf(str, "FULL_LIMIT : %d", FULL_LIMIT);
		USART1_puts(str);

		if (v_span < low_limit)
		{
			bAdcLow++;
//			goto mainload;
		} /* if (temp_full) */
		else if(((float)FULL_LIMIT/ratio) < v_span)
		{
///vputnumwithled(0,1, ((float)FULL_LIMIT/ratio)); Delay_ms(10000);
			bAdcHigh++;
//			goto mainload;
		} /* if (temp_full) */
//		else if (ADC_SATURATION <= high_limit)
//		{
//			bAdcSaturation++;
//			goto mainload;
//		}
		else  //  cal에 성공하면 
		{
			
		 sprintf(str, "V_SPAN=%ld", v_span);
		 USART1_puts(str);
			
			sprintf(str, "v_i_resolution : %d", v_i_resolution);
		USART1_puts(str);
			
						sprintf(str, "ratio=%f", ratio);
		USART1_puts(str);
	
			
			
			gnfFactor_2 = (float)(((float)v_i_resolution)/((float)v_span*ratio));
			sprintf(str, "gnfFactor_2 : %f", gnfFactor_2);
			USART1_puts(str);

			// =50000/(5565*16)=0.56148231   ->span값 (250 kg )에다가 gnfFactor 값을 곱하면 50000( 분해도 *10눈금 ) 
/*
			ctemp.f = gnfFactor;
			write_eep(ADR_gnfFactor, ctemp.u[0]);
			Delay_ms_time2(10);
			write_eep(ADR_gnfFactor+1, ctemp.u[1]);
			Delay_ms_time2(10);
								
			gnfFactor = gnfFactor;
*/			
		//	gnfFactor=1.553181;
			v_adc_org_2 = (long)(((float)v_zero_2)*gnfFactor_2);	 // 무부하상태에서의 카운트값 **
			bAdcLow = bAdcHigh = bAdcSaturation = 0;
/*
			sprintf(str, "v_zero = %8ld g ", v_zero); GLCD_string (0, 2, str); Delay_ms(5000);
			sprintf(str, "v_full = %8ld g ", v_full); GLCD_string (0, 2, str); Delay_ms(5000);
			sprintf(str, "ratio  = %8.5f g ", ratio); GLCD_string (0, 2, str); Delay_ms(5000);
			sprintf(str, "v_e_re = %8ld   ", v_e_resolution); GLCD_string (0, 2, str); Delay_ms(5000);
			sprintf(str, "v_i_re = %8ld   ", v_i_resolution); GLCD_string (0, 2, str); Delay_ms(5000);
			sprintf(str, "v_span = %8ld   ", v_span); GLCD_string (0, 2, str); Delay_ms(5000);
			sprintf(str, "factor = %8ld ", (long)(gnfFactor*1000000)); GLCD_string (0, 2, str); Delay_ms(20000);
//          sprintf(str, "factor 01 = %8f ", i, gnfFactor); GLCD_string (0, 2, str); Delay_ms(20000);
*/
//			repeat_flag = 0;
//			exit_flag = 1;
//			cal_state = 6;
			//GLCD_string(0,2, "  SUCCESS      ");
			GLCD_string(0,2, "  SUCCESS      ");
			Delay_ms(2000);
			//	GLCD_string(0,2, " 무게조정 성공");
		//	vputs("OK    ");
			
			
			Delay_ms(2000);
		}
	} /* for (hIndex = 0; hIndex < 7; hIndex++) */
	
	if (bAdcLow)
	{
		GLCD_string(0,1, "ERROR 24 : SPAN LOW ");
		//retry_message();
		///vputs("ERR24 ");
		///Delay_ms(500);
	//	vputerr("ERR24 ");

		//VfdPrintf("GAIN%2u", (u16)hLoop); Delay_ms(1000);
		//fast_evalue(temp_full - v_zero);  Delay_ms(1000);
		//flag = 0;  /* cal 3 */
		//repeat_flag = 1;
		//cal_state = 4;
		
//		repeat_flag = 0;
//		exit_flag = 1;
		ret_val = SET_RESTART;
//		cal_state = 0;
	}
	else if (bAdcHigh)
	{
		GLCD_string(0,2, "ERROR 25 : SPAN HIGH");
		///vputs("ERR25 ");
		//retry_message();
		///Delay_ms(500);
		//vputerr("ERR25 ");

		//VfdPrintf("GAIN%2u", (u16)hLoop); Delay_ms(1000);
		//fast_evalue(temp_full - v_zero);  Delay_ms(1000);
		//flag = 0;  /* cal 3 */
//		repeat_flag = 0;
//		exit_flag = 1;
//		cal_state = 0;
		ret_val = SET_RESTART;
	}
	else if (bAdcSaturation)
	{
		GLCD_string(0,2, "ERROR 29 : AD OVER  ");
		///vputs("ERR29 ");
		//retry_message();
		///Delay_ms(500);
		//vputerr("ERR29 ");

		//VfdPrintf("GAIN%2u", (u16)hLoop); Delay_ms(1000);
		//fast_evalue(temp_full - v_zero);  Delay_ms(1000);
		//flag = 0;  /* cal 3 */
//		repeat_flag = 0;
//		exit_flag = 1;
//		cal_state = 0;
		ret_val = SET_RESTART;
	}
	else
	{
		ret_val = SET_FORWARD;
	}

	return ret_val;
} /* of zero_set() */




//분동무게 구하기   415
int Set_Weight()
{
	int exit_flag=0;
	long 	ret_val;
	long	blink = 0;
	char sT0[30];
	s16 value_buffer[5];
	u16 digit_buffer = 0;
	lcd_cls();
	lcd_clear();
	if(cal_selection==1)
	ret_val = v_capacity;	//capa 의 100%로 기본 설정
	else
	ret_val = v_capacity_2;	
	
	value_buffer[0] = (ret_val%100000)/10000;
	value_buffer[1] = (ret_val%10000)/1000;
	value_buffer[2] = (ret_val%1000)/100;
	value_buffer[3] = (ret_val%100)/10;
	value_buffer[4] = (ret_val%10);
   
	
 lcd_inverse();
	if(gnlSilo1Selection==1)// silo : 1 ea
	{
   if(gnlLanguage==1)
	 { 
    GLCD_string (0,0 ," 올릴무게 설정");
	  sprintf(sT0, " 무게값:%05ld kg",(long)ret_val); // A/D값 
    GLCD_string(0,4, sT0);
		//lcd_inverse(); 
		 GLCD_string(0,6," 확인키:다음");
		// lcd_normal();
	 }
	  else
		{ 
    GLCD_string (0,0 ," LOAD WEIGHT");
	  sprintf(sT0, "WEIGHT :%05ld kg",(long)ret_val); // A/D값 
    GLCD_string(0,4, sT0);
		// lcd_inverse();	
	  GLCD_string(0,6,"ENTER:NEXT");
		// lcd_normal();
	  }
	 }
	else // silo : 2 ea
  {
   if(gnlLanguage==1)// korean
   {		 
	 if(cal_selection==1) // 1 st silo
   {		 
    GLCD_string (0,0 ," 올릴무게(1)");
	  sprintf(sT0, " 무게값 :%05ld kg",(long)ret_val); // A/D값 
    GLCD_string(0,4, sT0);
		//lcd_inverse(); 
		 GLCD_string(0,6," 확인키:다음");
		// lcd_normal();
   }	
	 else
	 {		 
    GLCD_string (0,0 ," 올릴무게(2)");
	  sprintf(sT0, " 무게값 :%05ld kg",(long)ret_val); // A/D값 
    GLCD_string(0,4, sT0);
		//lcd_inverse(); 
		 GLCD_string(0,6," 확인키:다음");
		// lcd_normal();
   }	
  }
  else//english
  {		 
	 if(cal_selection==1) // 1 st silo
   {		 
    GLCD_string (0,0 ," LOAD WEIGHT(1)");
	  sprintf(sT0, "WEIGHT :%05ld kg",(long)ret_val); // A/D값 
    GLCD_string(0,4, sT0);
		// lcd_inverse();	
	  GLCD_string(0,6,"ENTER:NEXT");
		// lcd_normal();
   }	
	 else
	 {		 
     GLCD_string (0,0 ," LOAD WEIGHT(2)");
	  sprintf(sT0, "WEIGHT :%05ld kg",(long)ret_val); // A/D값 
    GLCD_string(0,4, sT0);
		// lcd_inverse();	
	  GLCD_string(0,6,"ENTER:NEXT");
		// lcd_normal();
   }	
  }
		
	 
}
lcd_normal();
	

	do
	{
		ret_val = value_buffer[0]*10000 + value_buffer[1]*1000 + value_buffer[2]*100 + value_buffer[3]*10 + value_buffer[4]; // capacity value 
		//sprintf(sT0, "WEIGHT:%6ld kg",(long)ret_val); // A/D값 
    //GLCD_string(0,4, sT0);
		
		blink++;
		//if(blink==35000) 
		if(blink==2500) 
		
		{
			//sprintf(sT0, "%1c",value_buffer[4-digit_buffer]); // A/D값 
      
			if(gnlLanguage==1)
				sprintf(sT0, " 무게값:%05ld kg",(long)ret_val); // A/D값 
			 else
			  sprintf(sT0, "WEIGHT :%05ld kg",(long)ret_val); // A/D값 
      
			
			GLCD_string(0,4, sT0);
				
			lcd_inverse();
			sprintf(sT0, "%1d",(int)value_buffer[4-digit_buffer]); // A/D값 
			GLCD_string((12-digit_buffer),4, sT0);
		}
		
		//else if(blink>70000)
			else if(blink>5000)
		{
			blink=0;
			lcd_normal();
			if(gnlLanguage==1)//korean
				 sprintf(sT0, " 무게값:%05ld kg",(long)ret_val); // A/D값 
			 else
		 	sprintf(sT0, "WEIGHT :%05ld kg",(long)ret_val); // A/D값 
     GLCD_string(0,4, sT0);
		
			sprintf(sT0, "%1d",(int)value_buffer[4-digit_buffer]); // A/D값 
		//	lcd_normal();
			GLCD_string((12-digit_buffer),4, sT0);
			
			
		}
		
		
		
	//	goto_xy(3,5);
	//	cursor_on();
	
//		vputnum( 0, 0, ret_val);


/*
		// 자릿수 깜박거리기
		blink++;
		if((blink == 35000)) vputchar(4-digit_buffer, ' '); //자릿수 깜박거리기
		else if(70000 < blink)
		{
			blink = 0;
//			vputnum( 0, 0, ret_val);
			vputnum_ndigit( 0, 0, ret_val, 4-digit_buffer);
		}		
*/
		if(keypush_8510())
		{		 
			switch(KeyCode)
			{
				case UP:
					value_buffer[4-digit_buffer]++;
					value_buffer[4-digit_buffer] %= 10;
					break;

				case DOWN:
					if(value_buffer[4-digit_buffer]) value_buffer[4-digit_buffer]--;
					else value_buffer[4-digit_buffer] = 9;
					break;

				case RIGHT:
					digit_buffer++;
					digit_buffer %= 5;
					break;				 

				//case LEFT:
				//	exit_flag = 1;
//					cal_state = 2;
	//				ret_val = SET_BACKWARD;
				//	break;

				case ENTER:
				//	if(ret_val >v_capacity)// capa보다 크게 설정된 경우
					{
						exit_flag=1;
//						cal_state = 3;
				//		ret_val = SET_AGAIN;
			//		vputerr("ERR23 ");
					}
					
   /*					
					else if(ret_val < (v_capacity*0.1))//capa의 10% 미만으로 설정한 경우
					{
						exit_flag=1;
//분동무게 상관없이 넘어가기  2016.3.30
						if(ret_val < 1) ret_val = SET_AGAIN;
					//	vputerr("ERR26 ");
					}
					else
					{
						exit_flag = 1;
//						cal_state = 4;

						vputnum( 0, 0, ret_val); Delay_ms(300); vcls(); Delay_ms(300);
						vputnum( 0, 0, ret_val); Delay_ms(300); vcls(); Delay_ms(300);

//						ret_val = SET_FORWARD;
					}
			*/
					}// of switch
		}// of  keypush_8510
	} while(!exit_flag);

	return ret_val;
}





int Get_division()//  division 구하기   default 5
{
	 char sT0[100];
    u8 exit_flag = 0;
	 u8 iz;
	 s8 which;
	 int test_weight[]={1,2,5,10,20,50};
	 int	ret_val = 0;
//	 char KeyCode;
 
lcd_inverse();
if(gnlLanguage==1)// korean
{
	 if(gnlSilo1Selection==1) // 
   GLCD_string (0,0 ," 한눈의값 설정");

  else if(gnlSilo1Selection==2)// if 2 silo
	 {	 
	 
    if(cal_selection==1) // 1st silo 
			 GLCD_string (0,0 ," 한눈의값 설정");
	     //GLCD_string (0,0 ," ");
   else	
		   GLCD_string (0,0 ," 한눈의값 설정");
       //GLCD_string (0,0 ,"2번 SILO  ");
   }
 
 }
else// english
 {
	 if(gnlSilo1Selection==1) // 
   GLCD_string (0,0 ,"  DIVIION SET ");

  else if(gnlSilo1Selection==2)// if 2 silo
	 {	 
	 
    if(cal_selection==1) 
	    // GLCD_string (0,0 ,"SILO CAL 1      ");
		GLCD_string (0,0 ,"  DIVIION SET ");

   else	
       //GLCD_string (0,0 ,"SILO CAL 2      ");
	 GLCD_string (0,0 ,"  DIVIION SET ");

   }
 }
lcd_normal();
 
 
	//기존 설정값으로 표시하기 위함.
	
 if(cal_selection==1) //silo_1
 {
 for(iz=0; iz<6; iz++) if(test_weight[iz] == v_division) break;
	if(iz < 6) which = iz;
	else which = 0;
 } 
 else
 {
 for(iz=0; iz<6; iz++) if(test_weight[iz] == v_division_2) break;
	if(iz < 6) which = iz;
	else which = 0;
 } 
	 
  if(gnlLanguage==1)//korean 
	{	
	 sprintf(sT0, " 값:%2d kg",(int)test_weight[which]); // A/D값 
   GLCD_string(0,2, sT0);
	 GLCD_string (0,4 ,"위,아래키: 선택");
	 lcd_inverse();
	 GLCD_string (0,6 ," 확인키:  다음");
	 lcd_normal();
	} 
	else
	{	
	 sprintf(sT0, " DIV:%2d kg",(int)test_weight[which]); // A/D값 
   GLCD_string(0,2, sT0);
	 GLCD_string (0,4 ," UP-DOWN:SELECT");
	 lcd_inverse();
   GLCD_string (0,6 ," ENTER: NEXT");
	 lcd_normal();
	}
	do
	{
		//if(keypush())
		if(keypush_8510()) // 키가 눌리면 
		{
			// KeyCode=keypush_8510();
			switch(KeyCode)
			{
				case UP:
					which++;
				 if(which>5)
				 {
					 which=5;
					 retry_message();
				 }	
					 //which %= 6;
					break;

				case DOWN:
					 which--;
				   if(which<0)
					 {
						 retry_message();
						 which=0;
			     }	
				//if(which) which--;
					//else which = 5;
					break;


				case ENTER:
					exit_flag=1;
					ret_val = test_weight[which];
					break;					
			}
       if(gnlLanguage==1)   
				sprintf(sT0, " 값:%2d kg",(int)test_weight[which]); // A/D값 
        else
				sprintf(sT0, " DIV:%2d kg",(int)test_weight[which]); // A/D값	
			
			
			  GLCD_string(0,2, sT0);
		}
	} while(!exit_flag);

	return ret_val;
}

int Get_capacity()//  division 구하기   default 5
{
	 char sT0[100];
    u8 exit_flag = 0;
	 u8 iz;
	 s8 which;
//	 int test_weight[]={5,10,15,20,25,30,35,40,45,50,55,60,65,70};
   	 long test_weight[]={5000,10000,15000,20000,25000,30000,35000,40000,45000,50000,55000,60000,65000,70000,75000,80000};

	 int	ret_val = 0;

	  lcd_cls();
		lcd_clear();
   /*	
		 if(cal_selection==1) 
	 GLCD_string (0,0 ,"SILO1 CAL 1  ");
		else	
	  GLCD_string (0,0 ,"SILO2 CAL 2 ");
	*/	
   lcd_inverse();		 
	 if(gnlLanguage==1)// korean
	 GLCD_string (0,0 ,"최대무게 설정");
   else
	 GLCD_string (0,0 ,"SET CAPACITY");
   lcd_normal();	 

	 //Delay_ms(1000);
	

	//기존 설정값으로 표시하기 위함.
	 if(cal_selection==1) //silo_1
	 {
	for(iz=0; iz<14; iz++) if(test_weight[iz] == v_capacity) break;
	if(iz < 14) which = iz;
	else which = 0;
	 }
	 else
	 {
	for(iz=0; iz<14; iz++) if(test_weight[iz] == v_capacity_2) break;
	if(iz < 14) which = iz;
	else which = 0;
	 }
	 	 
	 if(gnlLanguage==1)// korean
	 { 
		sprintf(sT0, " 값:%5d kg",(long)test_weight[which]); // A/D값 
    GLCD_string(0,2, sT0);
	  GLCD_string (0,4 ,"위,아래키: 선택");
	 lcd_inverse();
	 GLCD_string (0,6 ," 확인키:  다음");
	  lcd_normal();
	 }
	 else//elglish
	 { 
		sprintf(sT0, " CAP:%5d kg",(long)test_weight[which]); // A/D값 
    GLCD_string(0,2, sT0);
		 
	  lcd_inverse();
	  GLCD_string (0,4 ," UP-DOWN:SELECT");
    GLCD_string (0,6 ," ENTER: NEXT");
	  lcd_normal();
	 }
	do
	{
		//if(keypush())
		if(keypush_8510()) // 키가 눌리면 
		{
		//	 KeyCode=keypush_8510();
			switch(KeyCode)
			{
				case UP:
					which++;
				if(which>15)
				{
					which=15; retry_message();
				}
					//which %= 14;
					break;

				case DOWN:
					which--;
				  if(which<0)
					{
						retry_message(); 
						which=0; //retry_message();
					}
					break;
				case ENTER:
					exit_flag=1;
					ret_val = test_weight[which];
					break;					
			}
			//	sprintf(sT0, "CAP:%2d   ton",(int)test_weight[which]); // A/D값 
			 if(gnlLanguage==1)
			   //sprintf(sT0, " CAP:%5d kg",(long)test_weight[which]); // A/D값 
			 sprintf(sT0, " 값:%5d kg",(long)test_weight[which]); // A/D값 
         else
         sprintf(sT0, " CAP:%5d kg",(long)test_weight[which]); // A/D값   					 
			 
			 GLCD_string(0,2, sT0);
		}
	} while(!exit_flag);

	return ret_val;
}


//0907

long cal_zero_span_ad_set(int which)
{
	char i;
	int	iz;
	int	exit_flag = 0;
	long	ret_val = 0;
	u32  gxhFilter_backup;		/* gxhFilter Backup */
	char sT0[100];
	gxhFilter_backup = gxhFilter;
	lcd_clear();
	if(gnlSilo1Selection==1)// silo 1 ea
	{
 	//lcd_inverse();	
	if(gnlLanguage==1) // korean
	{	
	if(which==0)// zero cal
	{	lcd_inverse();	
	  	GLCD_string (0,0 ,"  영점조정");
      lcd_normal();
	}		
  else // span cal
	{  lcd_inverse();
     GLCD_string (0,0 ,"  스판조정");
		 lcd_normal();
		sprintf(sT0, "%5ld kg올리세요",(long)v_setting_weight); // A/D값 
		GLCD_string (0,2 ,sT0);
		lcd_inverse();		
		
  }
  lcd_inverse();
	GLCD_string (0,6 ," 확인 :시작");
	lcd_normal();
 }
 else // english
 {	
	if(which==0)// zero cal
	{		
		  GLCD_string (0,0 ," ZERO CAL");		 
  }		
  else // span cal
	{
 	  GLCD_string (0,0 ," SPAN CAL");		 
  }
	
	lcd_normal();
	sprintf(sT0, " UPLOAD %5ld kg",(long)v_setting_weight); // A/D값 
	GLCD_string (0,2 ,sT0);
  lcd_inverse();
	GLCD_string (0,6 ," ENTER:START");
	lcd_normal();
 }
} // of silo 1
	

else// silo 2 ea	
{	
	lcd_normal();
	if(gnlLanguage==1) // korean
	{	
	if(which==0)// zero cal
	{		
	  if(cal_selection==1)
			GLCD_string (0,0 ," 영점조정(1)");
    else
		  GLCD_string (0,0 ," 영점조정(2)");		 
	}		
  else // span cal
	{
    if(cal_selection==1)
			GLCD_string (0,0 ,"스판조정(1)");
    else
		  GLCD_string (0,0 ,"스판조정(2)");		 
		
					lcd_normal();
	sprintf(sT0, "%5ld kg올리세요",(long)v_setting_weight); // A/D값 
	GLCD_string (0,2 ,sT0);
  }
	

  lcd_inverse();
	GLCD_string (0,6 ," 확인 :시작");
	lcd_normal();
 }
 else
 {	
	if(which==0)// zero cal
	{		
	  if(cal_selection==1)
			GLCD_string (0,0 ," ZERO CAL(1)");
    else
		  GLCD_string (0,0 ," ZERO CAL(2)");		 
  }		
  else // span cal
	{
    if(cal_selection==1)
			GLCD_string (0,0 ," SPAN CAL(1)");
    else
		  GLCD_string (0,0 ," SPAN CAL(2)");		 
		lcd_normal();
	sprintf(sT0, " UPLOAD %5ld kg",(long)v_setting_weight); // A/D값 
	GLCD_string (0,2 ,sT0);
		
		
  }
  lcd_inverse();
	GLCD_string (0,6 ," ENTER:START");
	lcd_normal();
 }

}
 
if(gnlSilo1Selection==1)// silo 1 ea
{	
if(which_adc==1) 
{	

for(i=0;i<20;i++)
	ret_val=read_filtered_adc_CS5555_1();
} 

else
	{	

   for(i=0;i<20;i++)
	ret_val=read_filtered_adc();
 } 
}
 
 
	//Delay_ms(1000);
	 
	
	
	
//mainload:
	/*
	if(which == CAL_ZERO_AD_SET) vputmsg("ULOAD ");	//2016.3.29 vputs("AD    ");
	else if(which == CAL_SPAN_AD_SET) vputmsg("UP    ");	//2016.3.29 vputs("UP    ");
	else
	{	
		exit_flag = -1;
		ret_val = SET_RESTART;
	}
	Delay_ms(500);
 */
	do
	{
	 //	vputnumwithled( 0, 0, read_filtered_adc()/10);
		if(cal_selection==1)	//SILO_1
		{	
		
			if(which_adc==1) 
			ret_val=read_filtered_adc_CS5555_1();
			else
			ret_val=read_filtered_adc();	
				
	 



			if(gnlLanguage==1)		
		  sprintf(sT0, " 출력값:%7ld",(long)ret_val); // A/D값 
    else
	  	sprintf(sT0, "ADC:%7ld",(long)ret_val); // A/D값 
  		
		}
   else		
   		{	
		  //ret_val=read_filtered_adc_1();
				if(which_adc==1) 
			ret_val=read_filtered_adc_CS5555_2();
			else
			ret_val=read_filtered_adc_1();	
				
	    if(gnlLanguage==1)		
		  sprintf(sT0, "출력(2):%7ld",(long)ret_val); // A/D값 
     else
	  	sprintf(sT0, "ADC(2):%7ld",(long)ret_val); // A/D값 
  
			}
		
		GLCD_string(0,4, sT0);
		
		
		if(keypush_8510())
		{
			switch(KeyCode)
			{
				/* 
				case KEY_CANCEL:
					exit_flag = -1;
					ret_val = SET_BACKWARD;
					break;
         */
				case  ENTER:
				    exit_flag = 1;
					ret_val = 0;
	      if(cal_selection==1)
				{	
 				
				if(which_adc==1) 
        {					
				for (iz=0; iz<CAL_SAMPLING_NUMBER; iz++) 
					ret_val += read_filtered_adc_CS5555_1(); 
				}
				else
				{					
				for (iz=0; iz<CAL_SAMPLING_NUMBER; iz++) 
					ret_val += read_filtered_adc(); 
				}
				
				}
				else
					{
					if(which_adc==1) 
					{						
 				    for (iz=0; iz<CAL_SAMPLING_NUMBER; iz++) 
				    ret_val += read_filtered_adc_CS5555_2(); 
					}
					
				else
					{						
 				    for (iz=0; iz<CAL_SAMPLING_NUMBER; iz++) 
				    ret_val += read_filtered_adc_1(); 
					}
					
					
					}

  				ret_val = ret_val / CAL_SAMPLING_NUMBER;
					break;

				default:
					retry_message();
					break;
			}
		}
		
	} while(!exit_flag);
	
	/*
	if(0 < ret_val)
	{
		vputnumwithled( 0, 0, ret_val/10); Delay_ms(300); vcls(); Delay_ms(300);
		vputnumwithled( 0, 0, ret_val/10); Delay_ms(300); vcls(); Delay_ms(300);
		vputnumwithled( 0, 0, ret_val/10); Delay_ms(300); vcls(); Delay_ms(300);
	}
*/
	gxhFilter = gxhFilter_backup;

	return ret_val;
} /* of zero_set() */

















#ifdef d;jf;d 
void zero_span_set(unsigned char flag)  /* 0 : zero set,    1: span set  */
{
	int	  i,j;
	float ratio;
	BYTE  exit_flag = 0;
	DWORD sum_temp = 0L, temp, temp_buff, temp_full, i_diff;
	DWORD low_limit,		/* span low limit */
		  high_limit;		/* span high limit */
	char  repeat_flag = 0;
	long  v_full, v_span;
	BYTE  hLoop;
	char  hIndex;		 	/* zero, full value save index */
	BYTE  hBUgxhFilter;		/* gxhFilter Backup */
	BOOL  bAdcLow;			/* zero, full value high */
	BOOL  bAdcHigh;			/* zero, full value low */
	BOOL  bAdcSaturation;	/* ad saturation */
	/* unsigned char p_string[23], q_string[23], temp_string[7]; */

	adc_initial();

	SIGNFLAG = 0;
	hBUgxhFilter = gxhFilter;
	gxhFilter = 9;
mainload:
	temp_full =  v_full;
	temp      =  v_zero;
	if(flag)  vputs("CAL 5 ");  else vputs("CAL 3 ");
	//delay(200);
	if(flag) fip_say("  UP  "); else fip_say("UNLOAD");

	do
	{
		if (keypush())
		{
			v_key = key_disp[KEYCODE];
			switch (v_key)
			{
				case SET_K:
				case ESC_K:
					exit_flag = 1;
					break;
				default:  /* ANY key */
					cls(); 	vputs("      ");
					for (hIndex = 0, hLoop = 1; hLoop < 65; hLoop *= 2, hIndex++)
					{
						gxhGain = hLoop;
						adc_initial();

						sum_temp = 0;
						for (i=0; i < 20; i++) temp_buff = read_filtered_adc();
						for (j=0; j<CAL_SAMPLING_NUMBER; j++)
						{
							switch (j/3)
							{
								case 0:	lputc(hIndex, 0x80); break; /* a ON */
								case 1: lputc(hIndex, 0x10); break; /* d ON */
								case 2: lputc(hIndex, 0x02); break; /* g ON */
							}
							temp = read_filtered_adc(); /* delay(50); */
							if (temp_buff > temp) i_diff = temp_buff - temp;
							else i_diff = temp - temp_buff;
							if (i_diff > 2500)
							{
								//vputs("ERR 28");  /*  unstable  */
								//retry_message();
								delay(1000);
								goto mainload;
							}  /* if (i_diff > 2500) */
							sum_temp += temp;
						} /* for j */
						temp = sum_temp / CAL_SAMPLING_NUMBER;
						if (flag)	//cal 5
						{	dFullValue[hIndex] = temp;
//							fast_evalue(temp); // delay(5000);
////0003					fast_evalue(dFullValue[hIndex]-dZeroValue[hIndex]);
						//	sprintf(str, "      AD=%8ld", dZeroValue[hIndex]);
						//	sprintf(str1,"G=%2d,AD=%8ld", (int)gxhGain, temp);
						}
						else	//cal 3
						{	dZeroValue[hIndex] = temp;
////0003					fast_evalue(temp); // delay(5000);
						//	sprintf(str, "G=%2d,AD=%8ld", (int)gxhGain, temp);
							//sprintf(str1,"                ");
						}
					//	lcd_display(str, str1, BOTH_line);

//						VfdPrintf("G=%5u", (UINT)gxhGain); delay(550);
//						fast_evalue(temp);  delay(500);
					} /* for (hLoop = 1; hLoop < 8; hLoop++) */

					if (flag) /* Span calibration cal 5 */
					{
						bAdcLow = bAdcHigh = bAdcSaturation = 0;
						for (hIndex=0,hLoop=1; hIndex<7; hIndex++,hLoop*=2)
						{
							v_zero = dZeroValue[hIndex];
							if (v_zero == ZERO_ERROR_LIMIT) continue; //???
							temp_full = dFullValue[hIndex];
							low_limit =(long)((float)v_i_resolution/((float)v_capacity/(float)v_setting_weight));
							high_limit = temp_full-v_zero;
							high_limit *= ((float)v_capacity/(float)v_setting_weight);
							high_limit += v_zero;

							if ((temp_full - v_zero) < low_limit)
							{
								bAdcLow++;
								continue;
							} // if (temp_full)
							else if((temp_full-v_zero) > ((float)FULL_LIMIT/((float)v_capacity/(float)v_setting_weight)))
							{
								bAdcHigh++;
								continue;
							} // if (temp_full)
							else if (ADC_SATURATION <= high_limit)
							{
								bAdcSaturation++;
								continue;
							}
							else
							{
								v_full = temp_full;
								v_span = v_full- v_zero;
								ratio = (float)((float)v_capacity/(float)v_setting_weight);
								gnfFactor = (float)(((float)v_i_resolution)/((float)v_span*ratio));
//								v_adc_org = (long)(((float)v_adc1_org)*gnfFactor);
								v_adc_org = (long)(((float)v_zero)*gnfFactor);
								bAdcLow = bAdcHigh = bAdcSaturation = 0;
								repeat_flag = 0;
								exit_flag = 1;
								gxhGain = hLoop;

						//sprintf(str, "G=%2d            ", (int)gxhGain);
						//sprintf(str1,"    AD=%8ld ", dFullValue[hIndex]-dZeroValue[hIndex]);
						//lcd_display(str, str1, BOTH_line);

						//		vputs("SUCCES");  delay(300);

								delay(1000);

								break;
							}
						} /* for (hIndex = 0; hIndex < 7; hIndex++) */
						if (bAdcLow)
						{
						//	vputs("ERR 24"); /*   span low  */
							retry_message();
							delay(500);
						//	VfdPrintf("GAIN%2u", (UINT)hLoop); delay(1000);
						//	fast_evalue(temp_full - v_zero);  delay(1000);
							flag = 0;  /* cal 3 */
							repeat_flag = 1;
						}
						else if (bAdcHigh)
						{
							//vputs("ERR 25");  /* span high */
						//	retry_message();
							delay(500);
							//VfdPrintf("GAIN%2u", (UINT)hLoop); delay(1000);
							//fast_evalue(temp_full - v_zero);  delay(1000);
							flag = 0;  /* cal 3 */
							repeat_flag = 1;
						}
						else if (bAdcSaturation)
						{
							//vputs("ERR 29");  /* ad saturation */
							//retry_message();
							delay(500);
							//VfdPrintf("GAIN%2u", (UINT)hLoop); delay(1000);
							//fast_evalue(temp_full - v_zero);  delay(1000);
							flag = 0;  /* cal 3 */
							repeat_flag = 1;
						}
					} /* of if(flag)  */
					else  /* zero calibration */
					{
						bAdcLow = bAdcHigh = 0;
						for (hIndex = 0; hIndex < 7; hIndex++)
						{
							if (dZeroValue[hIndex] < ZERO_LOW_LIMIT)
							{	bAdcLow++;
								dZeroValue[hIndex] = ZERO_ERROR_LIMIT;
							}
							else if (ZERO_HIGH_LIMIT < dZeroValue[hIndex])
							{	bAdcHigh++;
								dZeroValue[hIndex] = ZERO_ERROR_LIMIT;
							}
						}
						if (bAdcLow == 7)	/* berow zero low limit */
						{
							//vputs("ERR 27"); delay(1000);  /* zero low */
							//VfdPrintf("GAIN%2u", (UINT)hIndex); delay(1000);
							//fast_evalue(temp);  delay(1000);
							goto mainload;
						}
						else if (bAdcHigh == 7)	/* over zero high limit */
						{
							//vputs("ERR 26"); delay(1000); /* zero high */
							//VfdPrintf("GAIN%2u", (UINT)hIndex); delay(1000);
							//fast_evalue(temp);  delay(1000);
							goto mainload;
						}

						vputs("SUCCES");  delay(300);
/*						v_zero = temp; */
						if (repeat_flag)
						{
							repeat_flag = 0;
							flag = 1;   /* cal 5 */
							goto mainload;
						}
						v_key = ROUTINE_INC_K;  exit_flag =1; /* go to LOAD */
					} /* of else  */
			} /* switch (v_key) */
		} /* of if (keypush()) */
		else { if (flag) vputs("  UP  ");  else vputs("UNLOAD"); }
	} while (!exit_flag);
	gxhFilter = hBUgxhFilter;
} /* of zero_set() */

  #endif
/*----- LAMPSTT -------------------------------------------------
  addr:    38     39     3A     3B     3C     3D     3E     3F
  lamp:   none  stable   -     hold   print   -     tare   zero
  ---------------------------------------------------------------*/
 #ifdef dfjd
void factor_access(void)
{
	DWORD dwTemp;

	VfdPrintf("GAIN%2u", (UINT)gxhGain); delay(500);

	if (dwValueSet("FACTOR", HIDE, 0, 99999) == 8200)
	{
		dwTemp = (DWORD)(gnfFactor * (float)power(10, 7));
		vputs("111111");delay(500);

		VfdPrintf ("%6ld", dwTemp);
		delay(300);
		dwTemp = dwValueSet("", SHOW, dwTemp, 9999999);
		gnfFactor = (float)dwTemp / (float)power(10, 7);
		gnfFactor = gnfFactor + (2.0e-8);
		v_adc_org = (long)(((float)v_adc1_org) * gnfFactor);
	} /* of if */
} /* of factor_access() */
#endif


  
 
  

 long Get_adc_value(char status)
 {
   char temp;
   temp=status;
 while(1)
	{
       usart_transmit_byte(USART2,'@');   
	   usart_transmit_byte(USART2,'-');   
	   usart_transmit_byte(USART2,rotation_1+0x30);   
	   usart_transmit_byte(USART2,'R');   
	   usart_transmit_byte(USART2,0x0d);   
	   usart_transmit_byte(USART2,0x0a);
	   rotation_1++;
	   if(rotation_1>4)
	   rotation_1=1;
  //	 delay_ms(20000);   // critical !!
      Delay_ms(200);   // critical !!
	
	if(Usart2_Interrupt_flag) // 무게가 날아오면 
	{
		Usart2_Interrupt_flag=0;
		RxCounter2=0;	     

		switch(cmd_2[1]) // address
		{
			case '1':
		    	zero_1=(cmd_2[2]-0x30)*100000+(cmd_2[3]-0x30)*10000+(cmd_2[4]-0x30)*1000+(cmd_2[5]-0x30)*100+(cmd_2[6]-0x30)*10+(cmd_2[7]-0x30);
		    	One_Weight_Ok_1=1;
		   		break;

			case '2':
		    	zero_2=(cmd_2[2]-0x30)*100000+(cmd_2[3]-0x30)*10000+(cmd_2[4]-0x30)*1000+(cmd_2[5]-0x30)*100+(cmd_2[6]-0x30)*10+(cmd_2[7]-0x30);
		    	Two_Weight_Ok_1=1;
		   		break;

          	case '3':
				zero_3=(cmd_2[2]-0x30)*100000+(cmd_2[3]-0x30)*10000+(cmd_2[4]-0x30)*1000+(cmd_2[5]-0x30)*100+(cmd_2[6]-0x30)*10+(cmd_2[7]-0x30);
		    	Three_Weight_Ok_1=1;
		   		break;

          	case '4':
		  		zero_4=(cmd_2[2]-0x30)*100000+(cmd_2[3]-0x30)*10000+(cmd_2[4]-0x30)*1000+(cmd_2[5]-0x30)*100+(cmd_2[6]-0x30)*10+(cmd_2[7]-0x30);
		  		Four_Weight_Ok_1=1;
		   		break;

		   	default:
				break;
		}
		
	//// if(One_Weight_Ok_1&& Two_Weight_Ok_1 && Three_Weight_Ok_1 &&Four_Weight_Ok_1) // 무게 4개가 다알아오면 
	  // if( Three_Weight_Ok)
	   {	
		  if(!temp) // zero
		  {			  
	 	  }

		  else// span
		  {
//		   sprintf(str_1, "SPAN_ADC 1=%6lu, 2=%6lu, 3=%6lu, 4=%6lu", (long)zero_1, (long)zero_2, (long)zero_3, (long)zero_4);
//	       lcd70_show_string_mode(0, 350,str_1, 16, 0);
	 	  }
		  
/**********************************************************		   
		  if(!temp) // zero
		  {			  
		   sprintf(str_1, "ZERO_ADC_1=%6lu ",(long)zero_1);
	       lcd70_show_string_mode(0, 190,str_1, 16, 0);
		 
		  sprintf(str_1, "ZERO_ADC_2=%6lu ",(long)zero_2);
	      lcd70_show_string_mode(0, 210,str_1, 16, 0);
	 	 
		  sprintf(str_1, "ZERO_ADC_3=%6lu ",(long)zero_3);
	      lcd70_show_string_mode(0, 230,str_1, 16, 0);
	 	 
		  sprintf(str_1, "ZERO_ADC_4=%6lu ",(long)zero_4);
	      lcd70_show_string_mode(0, 250,str_1, 16, 0);
	 	  }

		  else// span
		  {
		   sprintf(str_1, "SPAN_ADC_1=%6lu ",(long)zero_1);
	       lcd70_show_string_mode(0, 350,str_1, 16, 0);
		 
		  sprintf(str_1, "SPAN_ADC_2=%6lu ",(long)zero_2);
	      lcd70_show_string_mode(0, 370,str_1, 16, 0);
	 	 
		  sprintf(str_1, "SPAN_ADC_3=%6lu ",(long)zero_3);
	      lcd70_show_string_mode(0, 390,str_1, 16, 0);
	 	 
		  sprintf(str_1, "SPAN_ADC_4=%6lu ",(long)zero_4);
	      lcd70_show_string_mode(0, 410,str_1, 16, 0);
	 	  }
*********************************************************/
	 	 

		  zero_sum=(zero_1+zero_2+zero_3+zero_4);
		  
		  if(temp) // span
		  {
//			  sprintf(str_1, "full_sum=%6lu ",(long)zero_sum);
			  sprintf(str_1, "SPAN SUM=%6lu, 1=%6lu, 2=%6lu, 3=%6lu, 4=%6lu", (long)zero_sum, (long)zero_1, (long)zero_2, (long)zero_3, (long)zero_4);
//			  lcd70_show_string_mode(0, 430,str_1, 16, 0);

			  if((zero_sum-v_zero)>=0)
			  {
//				  sprintf(str_1, "span_sum=%6lu ",(long)(zero_sum-v_zero));
				  sprintf(str_1, "(%6lu,%6lu,%6lu,%6lu)", (long)zero_1, (long)zero_2, (long)zero_3, (long)zero_4);
				  //lcd70_show_string_mode(0, 450,str_1, 16, 0);
			  }
		//	  lcd70_show_string_mode_24x24(300, Y_AXIS_SPACE_CAL*30, str_1, 16, 0);
		//	  lcd70_write_weight(X_START+(X_INCREMENT*2),Y_START+(Y_INCREMENT*4),zero_sum);





		  }
		  else
		  {
			///sprintf(str_1, "ZERO SUM=%6lu, 1=%6lu, 2=%6lu, 3=%6lu, 4=%6lu", (long)zero_sum, (long)zero_1, (long)zero_2, (long)zero_3, (long)zero_4);
			//lcd70_show_string_mode(0, 250, str_1, 16, 0);
			
			sprintf(str_1, "(%6lu,%6lu,%6lu,%6lu)", (long)zero_1, (long)zero_2, (long)zero_3, (long)zero_4);
		  //  lcd70_show_string_mode_24x24(300, Y_AXIS_SPACE_CAL*20, str_1, 16, 0);
          //  lcd70_write_weight(X_START+(X_INCREMENT*2),Y_START+(Y_INCREMENT*2),zero_sum);

 

	 	  }
		  One_Weight_Ok_1=0;
		  Two_Weight_Ok_1=0;
		  Three_Weight_Ok_1=0;
		  Four_Weight_Ok_1=0;
        }

			Initial_cmd_2_RxBuffer2();

	  }
	 //v_zero=Get_adc_value();// 짐판의 영점 a/d 값 읽오기     
	 
	 if(keypush())
	 {
	  //return 
	  if(KEYCODE==20)// 확인키 
	   {
	    return zero_sum;
		  break;
       }

	 }
	 	
	};
  }




 
 //long Get_adc_value(char status)
 




unsigned long dwValueSet(void) //1114
{
	char  exit_flag =0;
	char  changed = 0;
	char  index = 0;		/* only HIDE mode, instead number to '-' */
	unsigned long  curr_num = 0;
	char v_key;
	sprintf(str_1, "4 - Setting_Weight = %6lu ",(long)v_setting_weight);
	//lcd70_show_string_mode(0, 310,str_1, 16, 0);
//	lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*9, str_1, 16, 0);

	 //_write_weight(X_START+(X_INCREMENT*2),Y_START+(Y_INCREMENT*3),v_setting_weight);

	curr_num = v_setting_weight;

	do
	{
	  if (keypush())
		 {
		   switch (v_key = key_disp[KEYCODE])
			{
			   case 0:  case 1:  case 2:  case 3:  case 4:
			   case 5:  case 6:  case 7:  case 8:  case 9:
					changed = 1;

					curr_num = curr_num * 10 + v_key;
				   if (curr_num )index++;
					/*
					if (limit > 9)
					{   curr_num = curr_num * 10 + v_key;
						if (curr_num )index++;
					}
					else { curr_num = v_key; index = 1; }
					if( curr_num > limit)
					{
						curr_num = 0;
						index = 0;
						retry_message();
					}
					*/
					break;
				case KEY_CLEAR:
					curr_num = 0;
					index = 0;
					break;
				case KEY_ENTER: /* ON/OFF Key */
					exit_flag =1;
					break;
				default:
					/*retry_message();*/  break;
			} /* of switch */

//			sprintf(str_1, "settint_weight=%6lu ",
//	        lcd70_show_string_mode(0, 310,str_1, 16, 0);
			sprintf(str_1, "4 - Settint_Weight = %6lu ", (long)curr_num);
			//lcd70_show_string_mode(0, 310,str_1, 16, 0);
			//lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*9, str_1, 16, 0);
	//		lcd70_write_weight(X_START+(X_INCREMENT*2),Y_START+(Y_INCREMENT*3),curr_num);
			
		} /* of if (keypush())*/
	}while (!exit_flag);
	if (changed) return curr_num;
}  /* of dwValue_set */


long  Get_Capacity_New( long init_val) // capacity  설정하는 함수
{
	char  exit_flag =0;
	char  changed = 0;
	char  index = 0;	//only HIDE mode, instead number to '-' 
	long  curr_num, tmp_num;
	char v_key;
	char str_1[50];
	int blink;
	long ret_val;
	s16 value_buffer[7];
	u16 digit_buffer = 0;
	lcd_clear();
	lcd_inverse();
	if(gnlLanguage==1)
	GLCD_string (0,0 ," 최대무게 설정  ");
	 else
  GLCD_string (0,0 ,"  CAPACITY      ");
		 
	lcd_normal();
	
	
	ret_val=init_val; // 기존 값을 저장 
{ 
	value_buffer[0] = (ret_val%100000)/10000;
	value_buffer[1] = (ret_val%10000)/1000;
	value_buffer[2] = (ret_val%1000)/100;
	value_buffer[3] = (ret_val%100)/10;
	value_buffer[4] = (ret_val%10);
}

	blink==2000;
	do
	{
		
	  ret_val = value_buffer[0]*10000 + value_buffer[1]*1000 + value_buffer[2]*100 + value_buffer[3]*10 + value_buffer[4]; // capacity value 
	 blink++;
		if(blink==3000)
		{
  		 sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 		 
			GLCD_string(0,4, str_1);
			lcd_inverse();
			sprintf(str_1, "%1d",(int)value_buffer[4-digit_buffer]); // A/D값 
			GLCD_string((12-digit_buffer),4, str_1);
			 
		}
		
		else if(blink>6000)
		{
			blink=0;
			lcd_normal();
			 sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
			GLCD_string(0,4, str_1);
	   	sprintf(str_1, "%1d",(int)value_buffer[4-digit_buffer]); // A/D값 
			GLCD_string((12-digit_buffer),4, str_1);
	
	 }
	
	
	
	
	
	
		if(keypush_8510())
		{		 
			switch(KeyCode)
			{
				case UP:
					    value_buffer[4-digit_buffer]++;
				      value_buffer[4-digit_buffer] %= 10;
				  	   break;

				case DOWN:
				      if(value_buffer[4-digit_buffer]) value_buffer[4-digit_buffer]--;
						 else
						   value_buffer[4-digit_buffer] = 9;
				     break;

				case RIGHT:
					digit_buffer++;
				  digit_buffer %= 5;
				  						
				  break;				 
/*
				case LEFT:
					exit_flag = 1;
					break;
*/
				case ENTER:
						exit_flag=1;
					   break;
					}// of switch
		}// of  keypush_8510
	
	}while (!exit_flag);

//	v_capacity=ret_val;
	//lcd_normal();
 return ret_val;

}  /* of dwValue_set */






/*
unsigned int  dp_value_set (unsigned int init_value, char *flag, unsigned int limit)
{
	BYTE exit_flag =0;
	BYTE changed = 0;
	BYTE leading_zero = 0;
	BYTE index = 0;
	int  curr_num = 0;

	
	switch (v_decimal_point)
	{
		case 0:
			VfdPrintf ("%2s%4u", flag, init_value);
			draw_dp(0);
			break;
		case 1:
			VfdPrintf ("%2s  %02u", flag, init_value);
			draw_dp(0);
			draw_dp( 5);  
			break;
		case 2:
			VfdPrintf ("%2s %03u", flag, init_value);
			draw_dp(0);
			draw_dp(4);  
			break;
		case 3:
			VfdPrintf ("%2s%04u", flag, init_value);
			draw_dp(0);
			draw_dp(3);  
			break;
		case 4:
			VfdPrintf ("%2s%04u", flag, init_value);
			draw_dp(0);
			draw_dp(2);  
			break;
		default:
			draw_dp(0);
			VfdPrintf ("%2s%4u", flag, init_value);
			break;
	}

	do
	{
		if (keypush())
		{
			v_key = key_disp[KEYCODE];
			if (!v_key && !curr_num) leading_zero = 1;
			switch (v_key)
			{
				case 0:  case 1:  case 2:  case 3:  case 4:
				case 5:  case 6:  case 7:  case 8:  case 9:
					changed = 1;
					curr_num = curr_num * 10 + v_key;
					index++;
					if (curr_num > limit)
					{
						index = 0;
						curr_num = 0;
						leading_zero = 0;
						draw_dp(0);
						retry_message();
					}
					break;
				case CLEAR_K:
					index = 0;
					curr_num = 0;
					leading_zero = 0;
					draw_dp(0);
					break;
				case SET_K: 
					if (curr_num > limit)
					{
						vputs("ERR 51");     
						retry_message();  delay(100);
						curr_num = 0;
						exit_flag = 0;
						break;
					}
					exit_flag =1;
					break;
				default:
					retry_message(); break;
			} 
			switch (index)
			{
				case 0:
				case 1:
					VfdPrintf("%2s%4u", flag, curr_num);
					break;
				case 2:
					VfdPrintf("%2s  %02u", flag, curr_num);
					break;
				case 3:
					VfdPrintf("%2s %03u", flag, curr_num);
					break;
				case 4:
					VfdPrintf("%2s%04u", flag, curr_num);
					break;
				default:
					index = 0;
					VfdPrintf("%2s%4u", flag, curr_num);
					break;
			}
			if (leading_zero)
			{
				switch (index)
				{
					case 0:
						draw_dp(0);
						draw_dp(6);
						break;
					case 1:
						draw_dp(0);
						draw_dp(6);
						break;
					case 2:
						draw_dp(0);
						draw_dp(5);
						break;
					case 3:
						draw_dp(0);
						draw_dp(4);
						break;
					case 4:
						draw_dp(0);
						draw_dp(3);
						break;
					default:
						break;
				}
			}
		} 
	} while (!exit_flag);

	if (changed)
	{
		if (leading_zero)  v_decimal_point = index - 1;
		else v_decimal_point = 0;
		return (unsigned int)curr_num;
	}
	else  return (unsigned int)init_value;
} 
*/

 /* of value_set */