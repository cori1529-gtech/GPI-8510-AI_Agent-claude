/*
   Filename  :  SET.c

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
#include <stdio.h>

#include "stm32f10x.h"

#include "def.h"
#include "adc.h"
#include "cal.h"
#include "hw_config.h"
#include "i2c_ee.h"
#include "key.h"
#include "lcd.h"
#include "lcd70.h"
#include "rs485.h"
#include "usart.h"
#include "var.h"
#include "timer.h"


//#include "def.h"
//#include "eeprom.h"
//#include "var.h"
//#include "vfd.h"
//#ifdef	 __USE_LCD__
//#include "lcd.h"
//#endif

/**
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
 long temp_weight; 
  
 unsigned long gnlDailyTotal;
 volatile unsigned long gnlPreDailyTotal[7];
 unsigned char gnhDay;
 unsigned char gnhToday;
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
**/

// SET MENU
#define	SET_WEIGHT_BACKUP	1
#define	SET_END_DELAY_INTERVAL	2
#define	SET_SPAN_FACTOR	3
#define	SET_REAL_WEIGHT	4

#define	SET_LOADCELL_NUMBER	10
#define	SET_LOADCELL_COMPENSATION	11
#define	SET_SPAN_FACTOR	12
#define	SET_REAL_WEIGHT	13

#define	SET_SERIAL_NO	20
#define	SET_ID			21
#define	SET_HENHOUSE	22

#define	SET_LTE_ZIGBEE	30

#define	SET_STATUS_INTERVAL	40
#define	SET_DATA_SENDING_TIME_1	41
#define	SET_DATA_SENDING_TIME_2	42
#define	SET_DATA_SENDING_TIME_3	43

#define	SET_DEBUG_DISPLAY	50

void set_date(void);
void set_time(void);


char Value_Changed=0;
char Value_Changed2=0;



const int set_mode_msg_123[] = { 1, 2 };

const int set_mode_msg_0[] = {
	SET_WEIGHT_BACKUP, SET_END_DELAY_INTERVAL, SET_REAL_WEIGHT,
	SET_DATA_SENDING_TIME_1, SET_DATA_SENDING_TIME_2, SET_DATA_SENDING_TIME_3,
	SET_LOADCELL_NUMBER, SET_SPAN_FACTOR, SET_DEBUG_DISPLAY,
	SET_SERIAL_NO, SET_ID, SET_HENHOUSE,
	SET_LTE_ZIGBEE,
	SET_STATUS_INTERVAL, SET_LOADCELL_COMPENSATION
};

const char *set_mode_msg[15] = {
	"%2d. WEIGHT BACK UP       : %6ld",	//1
	"%2d. END DELAY            : %6ld",	//2
//	"%2d. COMPENSATION WEIGHT  : %6ld",
	"%2d. REAL WEIGHT          : %6ld",	//3
	"%2d. DATA SENDING TIME_1  : %6ld",	//4
	"%2d. DATA SENDING TIME_2  : %6ld",	//5
	"%2d. DATA SENDING TIME_3  : %6ld",	//6
	"%2d. LOADCELL NUMBER      : %6ld",	//7
	"%2d. SPAN  FACTOR         : %6ld",	//8
	"%2d. DISPLAY SELECTION    : %6ld",	//9
	
	"%2d. Serial No (MYYMMNNN) : M%07ld",	//10
	"%2d. ID (ADDRESS : M999)  : M%03ld",	//11
	"%2d. HENHOUSE NO          : %02ld",	//12
	
	"%2d. LTE (1:LTE, 0:ZIGBEE): %1ld",		//20

	"%2d. STATUS INTERVAL (MIN): %02ld",

	"%2d. L/C COMPENSATION(0,1): %1ld"
};


const char *set_mode_msg_1[] = {
	"",	//0
	"%2d. WEIGHT BACK UP       : %6ld",	//1
	"%2d. END DELAY            : %6ld",	//2
	"",	//3
	"",	//4
	"",	//5
	"",	//6
	"",	//7
	"",	//8
	"",	//9
	
	"%2d. LOADCELL NUMBER      : %6ld",	//10
	"%2d. L/C COMPENSATION(0,1): %1ld",	//11
	"%2d. SPAN  FACTOR         : %6ld",	//12
//	"%2d. COMPENSATION WEIGHT  : %6ld",
	"%2d. REAL WEIGHT          : %6ld",	//13
	"",	//14
	"",	//15
	"",	//16
	"",	//17
	"",	//18
	"",	//19

	
	"%2d. Serial No (MYYMMNNN) : M%07ld",	//20
	"%2d. ID (ADDRESS : M999)  : M%03ld",	//21
	"%2d. HENHOUSE NO          : %02ld",	//22
	"",	//23
	"",	//24
	"",	//25
	"",	//26
	"",	//27
	"",	//28
	"",	//29
	
	"%2d. LTE (1:LTE, 0:ZIGBEE): %1ld",		//30
	"",	//31
	"",	//32
	"",	//33
	"",	//34
	"",	//35
	"",	//36
	"",	//37
	"",	//38
	"",	//39

	"%2d. STATUS INTERVAL (MIN): %02ld",	//40
	"%2d. DATA SENDING TIME_1  : %6ld",		//41
	"%2d. DATA SENDING TIME_2  : %6ld",		//42
	"%2d. DATA SENDING TIME_3  : %6ld",		//43
	"",	//44
	"",	//45
	"",	//46
	"",	//47
	"",	//48
	"",	//49


	"%2d. DISPLAY SELECTION    : %6ld",	//50
	"",	//51
	"",	//52
	"",	//53
	"",	//54
	"",	//55
	"",	//56
	"",	//57
	"",	//58
	"",	//59
	
	"END OF SET MENU"
};


const char *set_mode_msg_simple[7] = {
	"%2d. WEIGHT BACK UP       : %6ld",
	"%2d. END DELAY            : %6ld",
	"%2d. REAL WEIGHT          : %6ld",
	"%2d. SPAN FACTOR          : %6ld",
	"%2d. DISPLAY SELECTION    : %6ld",
	"%2d. HENHOUSE NO          : %02ld",
	"%2d. LTE (1:LTE, 0:ZIGBEE): %1ld"
};



void dwValueSetting(char number,int x, int y, long init_val, long min_val, long max_val, int max_digit); //1114
void dwValueSetting2(char number,int x, int y, long init_val, long min_val, long max_val, int max_digit); //1114


void dwValueSetting_simple(char number,int x, int y, long init_val, long min_val, long max_val, int max_digit); //1114


/*
#define X_START     10
#define X_INCREMENT 30
#define Y_START     50
#define Y_INCREMENT 55

 */


 
//	lcd43_show_big_num(X_START,Y_START,1);
//	lcd43_show_big_num(X_START+(X_INCREMENT*1),Y_START,':');
	
//	lcd70_show_string_mode_24x24(300, Y_AXIS_SPACE_CAL*8,"(DIVISION-PRESS KEY_1)", 16, 0);





void set_mode_simple ()
{
	union { u32 dw; u16 u[2]; u8 c[4];} temp;
	int	y_start = 70;
	u8 str[30];
	int	y_line_spacing = 35;
	int	idx = 0;
	u8 i;

//	lcd70_clear_screen(WHITE);
//	lcd70_show_string_mode_24x24( 150,  30, "SET MODE", 16, 0);
	Delay_ms(1000);

	dwValueSetting_simple( ++idx, 150, y_start + (idx * y_line_spacing),(long)gxhWeightBackup, MIN_gxhWeightBackup, MAX_gxhWeightBackup, 1);  //weight backup 

	dwValueSetting_simple( ++idx, 150, y_start + (idx * y_line_spacing),(long)gxhEndDelay, MIN_gxhEndDelay, MAX_gxhEndDelay, MAX_gxhEndDelay);  //end delay

    dwValueSetting_simple( ++idx, 150, y_start + (idx * y_line_spacing),(long)gplCompensationWeight, MIN_gplCompensationWeight, MAX_gplCompensationWeight, MAX_gplCompensationWeight);  //compensation

	dwValueSetting_simple( ++idx, 150, y_start + (idx * y_line_spacing),(long)Real_Weight_Factor, 7000, 13000, 13000);  //무게 보정 factor
//   	dwValueSetting_simple( ++idx, 150, y_start + (idx * y_line_spacing),(long)Real_Weight_Factor, MIN_Real_Weight_Factor, MAX_Real_Weight_Factor, MAX_Real_Weight_Factor);  //무게 보정 factor
    

	dwValueSetting_simple( ++idx, 150, y_start + (idx * y_line_spacing),(long)gnlDisplay, MIN_gnlDisplay, MAX_gnlDisplay, 1);  //디스플레이 서렁  

	dwValueSetting_simple( ++idx, 150, y_start + (idx * y_line_spacing),(long)gnlHenhouseNo, MIN_gnlHenhouseNo, MAX_gnlHenhouseNo, MAX_gnlHenhouseNo);

	dwValueSetting_simple( ++idx, 150, y_start + (idx * y_line_spacing),(long)gnlUseLTE, MIN_gnlUseLTE, MAX_gnlUseLTE, MAX_gnlUseLTE);

	Initiaize_eeprom();
	FSMC_DISABLE;
	
	gxhWeightBackup = EE_Write_val( ADR_gxhWeightBackup, gxhWeightBackup, 4);

	gxhEndDelay = EE_Write_val( ADR_gxhEndDelay, gxhEndDelay, 4);

	
	//201708 
 /************************       ******************************************/

//gplCompensationWeight = EE_Write_val( ADR_gplCompensationWeight, gplCompensationWeight, 4);

  for(i=0;i<70;i++)
   raw_data = read_filtered_adc();		// ad 값으 읽어온다ㅣ

   v_adc1_buf = (s32)(gnfFactor * raw_data);	// count 값 입력(무게에 대한)
	 v_i_value  =  v_adc1_buf - v_adc_org;	//현재 카운트 값에서 zero 에서의 카운트갓을 뺀다. 
		
		if (0 <= v_i_value)   //0828
		{
			gxlGrossWeight = (v_i_value + (v_ei_multiply_factor/2));  // 10000+(10/2)=10005
			gxlGrossWeight=(gxlGrossWeight / v_ei_multiply_factor);	//10005/10=1000
			gxlGrossWeight=gxlGrossWeight * v_division;					//1000*5=5000
 	}
		else  
		{
 		  v_i_value=v_i_value*-1;
			gxlGrossWeight = (v_i_value + (v_ei_multiply_factor/2));
			gxlGrossWeight=(gxlGrossWeight / v_ei_multiply_factor);
			gxlGrossWeight=gxlGrossWeight * v_division;
		}

	   sprintf(str, "	VAL: %5ld kg",(long)gxlGrossWeight); // A/D값 
		 Delay_ms(3000);
		
		gplCompensationWeight -= gxlGrossWeight;	
		gplCompensationWeight = EE_Write_val( ADR_gplCompensationWeight, gplCompensationWeight, 4);
		
		
		
		/************************gplCompensationWeight ******************************************/





	if(Real_Weight_Factor>=7000 && Real_Weight_Factor<=13000)
	{
		Real_Weight_Factor = EE_Write_val( ADR_Real_Weight_Factor, Real_Weight_Factor, 4);
	}
	gnlDisplay = EE_Write_val( ADR_gnlDisplay, gnlDisplay, 4);
	//gnlHenhouseNo = EE_Write_val( ADR_gnlHenhouseNo, gnlHenhouseNo, 4); //계사 번호
	gnlUseLTE = EE_Write_val( ADR_gnlUseLTE, gnlUseLTE, 4); //LTE 사용여부(0:지그비, 1:LTE)
	
	FSMC_ENABLE;
   
//	lcd70_show_string_mode_24x24( 150, y_start + (idx++ * y_line_spacing), "SET MODE ENDING", 16, 0);
}
 
#ifdef dpfpd 

#define X_START     10
#define X_INCREMENT 50
#define Y_START     50
#define Y_INCREMENT 55


void set_mode_simple ()
{
	union { u32 dw; u16 u[2]; u8 c[4];} temp;
	int	y_start = 70;
	int	y_line_spacing = 35;
	int	idx = 0;

//	lcd70_clear_screen(WHITE);
//	lcd70_show_string_mode_24x24( 150,  30, "SET MODE", 16, 0);
	Delay_ms(1000);

  //  lcd43_show_big_num(X_START,Y_START,1);//
//	lcd43_show_big_num(X_START+(X_INCREMENT*1),Y_START,':');
	dwValueSetting_simple( ++idx, X_START+(X_INCREMENT*2),Y_START ,(long)gxhWeightBackup, MIN_gxhWeightBackup, MAX_gxhWeightBackup, 1);  //weight backup 


  //  lcd43_show_big_num(X_START,Y_START+(Y_INCREMENT*1),2);
//	lcd43_show_big_num(X_START+(X_INCREMENT*1),Y_START+(Y_INCREMENT*1),':');
	dwValueSetting_simple( ++idx, X_START+(X_INCREMENT*2),Y_START+(Y_INCREMENT*1) ,(long)gxhEndDelay, MIN_gxhEndDelay, MAX_gxhEndDelay, MAX_gxhEndDelay);  //end delay



	dwValueSetting_simple( ++idx, 150, y_start + (idx * y_line_spacing),(long)gplCompensationWeight, MIN_gplCompensationWeight, MAX_gplCompensationWeight, MAX_gplCompensationWeight);  //compensation
	dwValueSetting_simple( ++idx, 150, y_start + (idx * y_line_spacing),(long)Real_Weight_Factor, MIN_Real_Weight_Factor, MAX_Real_Weight_Factor, MAX_Real_Weight_Factor);  //무게 보정 factor
	dwValueSetting_simple( ++idx, 150, y_start + (idx * y_line_spacing),(long)gnlDisplay, MIN_gnlDisplay, MAX_gnlDisplay, 1);  //디스플레이 서렁  
	dwValueSetting_simple( ++idx, 150, y_start + (idx * y_line_spacing),(long)gnlHenhouseNo, MIN_gnlHenhouseNo, MAX_gnlHenhouseNo, MAX_gnlHenhouseNo);
	dwValueSetting_simple( ++idx, 150, y_start + (idx * y_line_spacing),(long)gnlUseLTE, MIN_gnlUseLTE, MAX_gnlUseLTE, MAX_gnlUseLTE);

	Initiaize_eeprom();
	FSMC_DISABLE;
	
	gxhWeightBackup = EE_Write_val( ADR_gxhWeightBackup, gxhWeightBackup, 4);

	gxhEndDelay = EE_Write_val( ADR_gxhEndDelay, gxhEndDelay, 4);

	gplCompensationWeight = EE_Write_val( ADR_gplCompensationWeight, gplCompensationWeight, 4);


	if(Real_Weight_Factor>=7000 && Real_Weight_Factor<=13000)
	{
		Real_Weight_Factor = EE_Write_val( ADR_Real_Weight_Factor, Real_Weight_Factor, 4);
	}
	gnlDisplay = EE_Write_val( ADR_gnlDisplay, gnlDisplay, 4);
	gnlHenhouseNo = EE_Write_val( ADR_gnlHenhouseNo, gnlHenhouseNo, 4); //계사 번호
	gnlUseLTE = EE_Write_val( ADR_gnlUseLTE, gnlUseLTE, 4); //LTE 사용여부(0:지그비, 1:LTE)
	FSMC_ENABLE;
   
//	lcd70_show_string_mode_24x24( 150, y_start + (idx++ * y_line_spacing), "SET MODE ENDING", 16, 0);
}

#endif


void set_mode2()//1206
{
	union { u32 dw; u16 u[2]; u8 c[4];} temp;
	u8 i;
	u8  str[30];
	int	y_start = 70;
	int	y_line_spacing = 27;
	int	idx = 0;
  lcd_clear();
	if(gnlLanguage==1)// korean 
	{	
	 GLCD_string (0,0 ,"  설정모드(2)   ");
	 lcd_inverse();
		GLCD_string (0,6 ,"확인키: 다음");
	 lcd_inverse();
  }
 else	
  	{	
	 GLCD_string (0,0 ,"  SET  MODE     ");
	 lcd_inverse();
	 GLCD_string (0,6 ,"NEXT: ENTER    ");
	 lcd_inverse();
  }
	Delay_ms(1000);
	
	Value_Changed=0;
	Value_Changed2=0;
	

	set_date();//날짜설정
	set_time();//시간 설정 
//	Real_Weight_Factor=9000;
	
	lcd_clear();
	dwValueSetting2(++idx,  0,2,(long)gxhWeightBackup, MIN_gxhWeightBackup, MAX_gxhWeightBackup, 1);  lcd_normal();//weight backup 
  dwValueSetting2(++idx,  0,2,(long)gxhFilter, 1, 50, 1); lcd_normal(); //digital filter
	dwValueSetting2( ++idx, 0,2,(long)gxhEndDelay, MIN_gxhEndDelay, MAX_gxhEndDelay, MAX_gxhEndDelay);  lcd_normal();//end delay
  dwValueSetting2(++idx,  0,2,(long)gnlSilo1Selection, 1, 2, 1);  lcd_normal();//silo selection 
  dwValueSetting2( ++idx, 0,2,(long)gplCompensationWeight, MIN_gplCompensationWeight, MAX_gplCompensationWeight, MAX_gplCompensationWeight);  lcd_normal();//compensation
  dwValueSetting2( ++idx, 0,2,(long)gplCompensationWeight2, MIN_gplCompensationWeight, MAX_gplCompensationWeight, MAX_gplCompensationWeight);  lcd_normal();//compensation
	dwValueSetting2( ++idx, 0,2,(long)Real_Weight_Factor, MIN_Real_Weight_Factor, MAX_Real_Weight_Factor, MAX_Real_Weight_Factor); lcd_normal(); //무게 보정 factor
  dwValueSetting2( ++idx, 0,2,(long)Real_Weight_Factor2, MIN_Real_Weight_Factor, MAX_Real_Weight_Factor, MAX_Real_Weight_Factor); lcd_normal(); //무게 보정 factor
	dwValueSetting2( ++idx, 0, 2,(long)gnlMyAddress, MIN_gnlMyAddress, MAX_gnlMyAddress, MAX_gnlMyAddress); lcd_normal(); //strcpy(adr, "M001");
  dwValueSetting2( ++idx, 0, 2,(long)gnlLanguage, 0, 1,1); // 1 korean   0: english
	dwValueSetting2( ++idx, 0, 2,(long)gnlCdmaUse, 0, 1,1); // 1: cdma use   0: not use
	dwValueSetting2( ++idx, 0, 2,(long)gxuPulseSetWeigh, 0, 1,1);
	dwValueSetting2( ++idx, 0, 2,(long)gxuOutputErrorWeigh, 0, 1,1);// 하한에러 
	dwValueSetting2( ++idx, 0, 2,(long)gxuOutputHighErrorWeigh, 0, 1,1);// 배출 상한에러 
	dwValueSetting2( ++idx, 0, 2,(long)gnlSerialNo, 0, 1,1);// 장비제조번호
	dwValueSetting2( ++idx, 0, 2,(long)gnlSendingInterval, 0, 1,1);//이더넷 전송 인터벌 시간 설정 (1분 ~60분) 초기값: 10분
	
  
	
	
	Initiaize_eeprom();
	FSMC_DISABLE;
	
	 gxhWeightBackup = EE_Write_val( ADR_gxhWeightBackup, gxhWeightBackup, 4);Delay_ms(30);  
   gxhFilter = EE_Write_val( ADR_gxhFilter, gxhFilter, 4);Delay_ms(30);
	 gxhEndDelay = EE_Write_val( ADR_gxhEndDelay, gxhEndDelay, 4);Delay_ms(30);
   gnlSilo1Selection = EE_Write_val( ADR_gnlSilo1Selection, gnlSilo1Selection, 4);Delay_ms(30);
  
//  Real_ factor 
 
if(gplCompensationWeight&&Value_Changed)// 보정값이 들어있을때만 
{
 v_adc_org  = (long)((float)v_zero * gnfFactor);  // calibration시 영점값 기억 
  v_ei_multiply_factor = 10;
  gxhFilter=9;
  for(i=0;i<70;i++)
	 {
		 
		 if(which_adc==1) 
		  raw_data =read_filtered_adc_CS5555_1();
		 else
		 raw_data = read_filtered_adc();		// ad 값으 읽어온다ㅣ
		 Delay_ms(50);
		 
		 
    //raw_data = read_filtered_adc();		// ad 값으 읽어온다ㅣ
		 //Delay_ms(50);

   }
		 v_adc1_buf = (s32)(gnfFactor * raw_data);	// count 값 입력(무게에 대한)
	 v_i_value  =  v_adc1_buf - v_adc_org;	//현재 카운트 값에서 zero 에서의 카운트갓을 뺀다. 
		
		if (0 <= v_i_value)   //0828
		{
			gxlGrossWeight = (v_i_value + (v_ei_multiply_factor/2));  // 10000+(10/2)=10005
			gxlGrossWeight=(gxlGrossWeight / v_ei_multiply_factor);	//10005/10=1000
			gxlGrossWeight=gxlGrossWeight * v_division;					//1000*5=5000
 	}
		else  
		{
 		  v_i_value=v_i_value*-1;
			gxlGrossWeight = (v_i_value + (v_ei_multiply_factor/2));
			gxlGrossWeight=(gxlGrossWeight / v_ei_multiply_factor);
			gxlGrossWeight=gxlGrossWeight * v_division;
		}

	   sprintf(str, "	VAL: %5ld kg",(long)gxlGrossWeight); // A/D값 
		 GLCD_string(0,2, str);
		 Delay_ms(2000);
	 gplCompensationWeight -= gxlGrossWeight;	
	 gplCompensationWeight = EE_Write_val( ADR_gplCompensationWeight, gplCompensationWeight, 4);Delay_ms(20);
	 
		 if(gplCompensationWeight>=0)
		 { 
   	 sprintf(str, "	VAL: %5ld",(long)gplCompensationWeight); // A/D값 
		 }
		 else
		 sprintf(str, "	VAL:-%5ld",(long)gplCompensationWeight); // A/D값 
		 
		GLCD_string(0,2, str);
		 Delay_ms(2000);

}



if(gplCompensationWeight2&&Value_Changed2)// 보정값이 들어있을때만 
{
  v_adc_org_2  = (long)((float)v_zero_2 * gnfFactor_2);  // calibration시 영점값 기억 
  v_ei_multiply_factor = 10;
  gxhFilter=9;
  for(i=0;i<70;i++)
	 {
     if(which_adc==1) 
		  raw_data =read_filtered_adc_CS5555_2();
		 else
		 raw_data = read_filtered_adc_1();		// ad 값으 읽어온다ㅣ
		 Delay_ms(50);

   }
		 v_adc1_buf = (s32)(gnfFactor_2 * raw_data);	// count 값 입력(무게에 대한)
	 v_i_value  =  v_adc1_buf - v_adc_org_2;	//현재 카운트 값에서 zero 에서의 카운트갓을 뺀다. 
		
		if (0 <= v_i_value)   //0828
		{
			gxlGrossWeight = (v_i_value + (v_ei_multiply_factor/2));  // 10000+(10/2)=10005
			gxlGrossWeight=(gxlGrossWeight / v_ei_multiply_factor);	//10005/10=1000
			gxlGrossWeight=gxlGrossWeight * v_division;					//1000*5=5000
 	}
		else  
		{
 		  v_i_value=v_i_value*-1;
			gxlGrossWeight = (v_i_value + (v_ei_multiply_factor/2));
			gxlGrossWeight=(gxlGrossWeight / v_ei_multiply_factor);
			gxlGrossWeight=gxlGrossWeight * v_division;
		}

	   sprintf(str, "	VAL: %5ld kg",(long)gxlGrossWeight); // A/D값 
		 GLCD_string(0,2, str);
		 Delay_ms(2000);
	 gplCompensationWeight2 -= gxlGrossWeight;	
	 gplCompensationWeight2 = EE_Write_val( ADR_gplCompensationWeight2, gplCompensationWeight2, 4);Delay_ms(20);
	 
		 if(gplCompensationWeight2>=0)
		 { 
   	 sprintf(str, "	VAL: %5ld",(long)gplCompensationWeight2); // A/D값 
		 }
		 else
		 sprintf(str, "	VAL:-%5ld",(long)gplCompensationWeight2); // A/D값 
		 
		GLCD_string(0,2, str);
		 Delay_ms(2000);

}


		if(Real_Weight_Factor>=7000 && Real_Weight_Factor<=13000)
	{
		Real_Weight_Factor = EE_Write_val( ADR_Real_Weight_Factor, Real_Weight_Factor, 4); Delay_ms(20);
	}

 		if(Real_Weight_Factor2>=7000 && Real_Weight_Factor2<=13000)
	{
		Real_Weight_Factor2 = EE_Write_val( ADR_Real_Weight_Factor2, Real_Weight_Factor2, 4); Delay_ms(20);
	}


	gnlMyAddress = EE_Write_val( ADR_gnlMyAddress, gnlMyAddress, 4); Delay_ms(2);//장비 ID(어드레스)
  gnlLanguage = EE_Write_val( ADR_gnlLanguage, gnlLanguage, 4); Delay_ms(2);//계사 번호
	gnlCdmaUse=EE_Write_val( ADR_gnlCdmaUse, gnlCdmaUse, 4); Delay_ms(2);//계사 번호
	gxuPulseSetWeigh=EE_Write_val( ADR_gxuPulseSetWeigh, gxuPulseSetWeigh, 4); Delay_ms(2);
	gxuOutputErrorWeigh=EE_Write_val( ADR_gxuOutputErrorWeigh, gxuOutputErrorWeigh, 4); Delay_ms(2);
	gxuOutputHighErrorWeigh=EE_Write_val( ADR_gxuOutputHighErrorWeigh, gxuOutputHighErrorWeigh, 4); Delay_ms(2);
	gnlSerialNo = EE_Write_val( ADR_gnlSerialNo, gnlSerialNo, 4);	Delay_ms(2);//장비 시리얼 번호Delay_ms(10);
	gnlSendingInterval== EE_Write_val( ADR_gnlSendingInterval, gnlSendingInterval, 4); //20180730
	FSMC_ENABLE;
   
}



void set_mode()
{
	union { u32 dw; u16 u[2]; u8 c[4];} temp;
	u8 i;
	u8  str[30];
	int	y_start = 70;
	int	y_line_spacing = 27;
	int	idx = 0;
	
	
//	   if (!gplCompensationWeight) gxlWeight = gxlGrossWeight;      // 보상무게 처리 
//    else gxlWeight = gxlGrossWeight + gplCompensationWeight;

	Value_Changed=0;
	
  lcd_clear();
	if(gnlLanguage==1)// korean 
	{	
	 GLCD_string (0,0 ,"   설정모드   ");
	 lcd_inverse();
		GLCD_string (0,6 ,"확인키: 다음");
	 lcd_inverse();
  }
 else	
  	{	
	 GLCD_string (0,0 ,"  SET  MODE     ");
	 lcd_inverse();
	 GLCD_string (0,6 ,"NEXT: ENTER    ");
	 lcd_inverse();
  }
	Delay_ms(1000);

	set_date();//날짜설정
	set_time();//시간 설정 
	lcd_clear();
	
	dwValueSetting(++idx,  0,2,(long)gxhWeightBackup, MIN_gxhWeightBackup, MAX_gxhWeightBackup, 1);  lcd_normal();//weight backup 
  dwValueSetting(++idx,  0,2,(long)gxhFilter, 1, 50, 1); lcd_normal(); //digital filter
	
	dwValueSetting( ++idx, 0,2,(long)gxhEndDelay, MIN_gxhEndDelay, MAX_gxhEndDelay, MAX_gxhEndDelay);  lcd_normal();//end delay
  dwValueSetting(++idx,  0,2,(long)gnlSilo1Selection, 1, 2, 1);  lcd_normal();//silo selection 
	dwValueSetting( ++idx, 0,2,(long)gplCompensationWeight, MIN_gplCompensationWeight, MAX_gplCompensationWeight, MAX_gplCompensationWeight);  lcd_normal();//compensation
	dwValueSetting( ++idx, 0,2,(long)Real_Weight_Factor, MIN_Real_Weight_Factor, MAX_Real_Weight_Factor, MAX_Real_Weight_Factor); lcd_normal(); //무게 보정 factor
  dwValueSetting( ++idx, 0, 2,(long)gnlMyAddress, MIN_gnlMyAddress, MAX_gnlMyAddress, MAX_gnlMyAddress); lcd_normal(); //strcpy(adr, "M001");
  dwValueSetting( ++idx, 0, 2,(long)gnlLanguage, 0, 1,1); // 1 korean   0: english
	dwValueSetting( ++idx, 0, 2,(long)gnlCdmaUse, 0, 1,1); // 1: cdma use   0: not use
	dwValueSetting( ++idx, 0, 2,(long)gxuPulseSetWeigh, 0, 1,1);
	dwValueSetting( ++idx, 0, 2,(long)gxuOutputErrorWeigh, 0, 1,1);// 배출 하한에러 
	dwValueSetting( ++idx, 0, 2,(long)gxuOutputHighErrorWeigh, 0, 1,1);// 배출 상한에러 
	dwValueSetting( ++idx, 0, 2,(long)gnlSerialNo, 0, 1,1);//장비번호  M201804000
	dwValueSetting( ++idx, 0, 2,(long)gnlSendingInterval, 0, 1,1);//이더넷 전송 인터벌 시간 설정 (1분 ~60분) 초기값: 10분
	
	
	//gnlSendingInterval
  //gnlSerialNo = EE_Write_val( ADR_gnlSerialNo, gnlSerialNo, 4);	Delay_ms(20);//장비 시리얼 번호Delay_ms(10);
	 
	 
	
	//gxuOutputHighErrorWeigh
	
 // gnlSiloSelection
	
	
	Initiaize_eeprom();
	FSMC_DISABLE;
	
	 gxhWeightBackup = EE_Write_val( ADR_gxhWeightBackup, gxhWeightBackup, 4);Delay_ms(30);  USART1_puts("11111111");
   gxhFilter = EE_Write_val( ADR_gxhFilter, gxhFilter, 4);Delay_ms(30);USART1_puts("2222222");
	 gxhEndDelay = EE_Write_val( ADR_gxhEndDelay, gxhEndDelay, 4);Delay_ms(30);USART1_puts("333333");
   gnlSilo1Selection = EE_Write_val( ADR_gnlSilo1Selection, gnlSilo1Selection, 4);Delay_ms(30);USART1_puts("444444");
  
//  Real_ factor 
 
if(gplCompensationWeight && Value_Changed)// 보정값이 들어있을때만 
{
	USART1_puts("555555");
  v_adc_org  = (long)((float)v_zero * gnfFactor);  // calibration시 영점값 기억 
  v_ei_multiply_factor = 10;
  gxhFilter=9;
 
	for(i=0;i<70;i++)
	 {
		 USART1_puts("866666"); 
		 
		  if(which_adc==1) 
		  raw_data =read_filtered_adc_CS5555_1();
		 else
      raw_data = read_filtered_adc();		// ad 값으 읽어온다ㅣ
		 Delay_ms(50);
		 USART1_puts("7777");

   }
	 
	 USART1_puts("6666666666");
		 v_adc1_buf = (s32)(gnfFactor * raw_data);	// count 값 입력(무게에 대한)
	 v_i_value  =  v_adc1_buf - v_adc_org;	//현재 카운트 값에서 zero 에서의 카운트갓을 뺀다. 
		
		if (0 <= v_i_value)   //0828
		{
			gxlGrossWeight = (v_i_value + (v_ei_multiply_factor/2));  // 10000+(10/2)=10005
			gxlGrossWeight=(gxlGrossWeight / v_ei_multiply_factor);	//10005/10=1000
			gxlGrossWeight=gxlGrossWeight * v_division;					//1000*5=5000
 	}
		else  
		{
 		  v_i_value=v_i_value*-1;
			gxlGrossWeight = (v_i_value + (v_ei_multiply_factor/2));
			gxlGrossWeight=(gxlGrossWeight / v_ei_multiply_factor);
			gxlGrossWeight=gxlGrossWeight * v_division;
		}

	   sprintf(str, "	VAL: %5ld kg",(long)gxlGrossWeight); // A/D값 
		 GLCD_string(0,2, str);
		 Delay_ms(2000);
	 gplCompensationWeight -= gxlGrossWeight;	
	 gplCompensationWeight = EE_Write_val( ADR_gplCompensationWeight, gplCompensationWeight, 4);Delay_ms(20);
	 
		 if(gplCompensationWeight>=0)
		 { 
   	 sprintf(str, "	VAL: %5ld",(long)gplCompensationWeight); // A/D값 
		 }
		 else
		 sprintf(str, "	VAL:-%5ld",(long)gplCompensationWeight); // A/D값 
		 GLCD_string(0,2, str);
		 Delay_ms(2000);
 


}

	
		if(Real_Weight_Factor>=7000 && Real_Weight_Factor<=13000)
	{
		Real_Weight_Factor = EE_Write_val( ADR_Real_Weight_Factor, Real_Weight_Factor, 4); Delay_ms(20);
	}



	//gnlLowerOutputError = EE_Write_val( ADR_gnlLowerOutputError, gnlLowerOutputError, 4);	Delay_ms(20);//장비 시리얼 번호Delay_ms(10);
	//gnlUpperOutputError = EE_Write_val( ADR_gnlUpperOutputError, gnlUpperOutputError, 4); Delay_ms(20);//장비 ID(어드레스)
	//gnlPulseWeight = EE_Write_val( ADR_gnlPulseWeight, gnlPulseWeight, 4); Delay_ms(20);//계사 번호
	//gnlSerialNo = EE_Write_val( ADR_gnlSerialNo, gnlSerialNo, 4);	Delay_ms(20);//장비 시리얼 번호Delay_ms(10);
	gnlMyAddress = EE_Write_val( ADR_gnlMyAddress, gnlMyAddress, 4); Delay_ms(20);//장비 ID(어드레스)
	//gnlHenhouseNo = EE_Write_val( ADR_gnlHenhouseNo, gnlHenhouseNo, 4); Delay_ms(20);//계사 번호
	//gxwStatusSendInterval = EE_Write_val( ADR_gxwStatusSendInterval, gxwStatusSendInterval, 4); Delay_ms(20);//상태정보 전송 시간 설정
  gnlLanguage = EE_Write_val( ADR_gnlLanguage, gnlLanguage, 4); Delay_ms(20);//계사 번호
  gnlCdmaUse=EE_Write_val( ADR_gnlCdmaUse, gnlCdmaUse, 4); Delay_ms(20);//계사 번호
	gxuPulseSetWeigh=EE_Write_val( ADR_gxuPulseSetWeigh, gxuPulseSetWeigh, 4); Delay_ms(20);
	gxuOutputErrorWeigh=EE_Write_val( ADR_gxuOutputErrorWeigh, gxuOutputErrorWeigh, 4); Delay_ms(20);
  gxuOutputHighErrorWeigh=EE_Write_val( ADR_gxuOutputHighErrorWeigh, gxuOutputHighErrorWeigh, 4); Delay_ms(20);
	
	gnlSerialNo = EE_Write_val( ADR_gnlSerialNo, gnlSerialNo, 4);	Delay_ms(20);//장비 제조 번호 M201804001
	
	gnlSendingInterval= EE_Write_val( ADR_gnlSendingInterval, gnlSendingInterval, 4);	Delay_ms(20);//이더넷 전송 시간 설정 
	
	
	
	
	
	// M1802009   ; 18년도 2월 생산분  9번째 생산한제품
	//gxuOutputErrorWeigh
	
	FSMC_ENABLE;
   
}



long calcCompensationWeight(long val)
{
	long ret_val;
//	long compensation_factored_weight;
	
//	char str_1[50];
//	float real_factor;

//USART1_puts("adc 0");	
///	Weight_flag = 0;

/*		
	rotation = 1;	// scan start

	while(!Weight_flag) {
///sprintf( str_1, "R%d%d-%d%d%d%d%d%d ", rotation, Weight_flag, One_Weight_Ok, Two_Weight_Ok, Three_Weight_Ok, Four_Weight_Ok, Five_Weight_Ok, Six_Weight_Ok);
///USART1_puts(str_1);	
		adc_raw = Get_adc_from_module_20170626();
		
///beep(10);		
///sprintf( str_1, "r%d%d-%d%d%d%d%d%d ", rotation, Weight_flag, One_Weight_Ok, Two_Weight_Ok, Three_Weight_Ok, Four_Weight_Ok, Five_Weight_Ok, Six_Weight_Ok);
///USART1_puts(str_1);	
	}
*/
	

	while( !get_weight_wait());

//	real_factor = (float)Real_Weight_Factor/10000.0;

	ret_val = val - gxlRawWeight;
//	ret_val = ret_val + (ret_val * (1.0 - real_factor));
	
	
/*	
	compensation_factored_weight = gxlRawWeight/real_factor;

	ret_val = val - compensation_factored_weight;
	ret_val = ret_val - (ret_val * (real_factor - 1.0));
*/


//	sprintf(str_1, "V:%6ld, RW:%6ld, CW:%6ld.", (long)val,(long)gxlRawWeight, (long)ret_val);
//	lcd70_show_string_mode(0, 0,str_1, 16, 0);



/*	
//USART1_puts("adc 1");		
	Weight_flag = 0;
	rotation = 1;
	while(!Weight_flag) adc_raw=Get_adc_from_module_20170626();
//USART1_puts("adc 2");	
	Weight_flag = 0;
	rotation = 1;
	while(!Weight_flag) adc_raw=Get_adc_from_module_20170626();
//USART1_puts("adc 3");	
	Weight_flag = 0;
	rotation = 1;
	while(!Weight_flag) adc_raw=Get_adc_from_module_20170626();
//USART1_puts("adc 4");	
*/	
	
///USART1_puts("X");	
/**	
	adc_buf = (long)(gnfFactor * adc_raw);
	adc_diff = adc_buf - v_adc_org;
	//diff2 = v_adc1_buf - prev_adc1;	// for stable
	weight = ((adc_diff + 10) / v_ei_multiply_factor) * v_division;
	ret_val = val - weight;
**/
	/*
	if(gplCompensationWeight<0)
	{
	gplCompensationWeight*=-1;
	sprintf(str_1, "comp weight : -%6lu   weight : %6lu",(long)gplCompensationWeight,(long)gxlGrossWeight);

	}
	else
	sprintf(str_1, "comp weight : %6lu   weight : %6lu",(long)gplCompensationWeight,(long)gxlGrossWeight);
	lcd70_show_string_mode(0, 0,str_1, 16, 0);

	Delay_ms(7000);
	*/
	
///USART1_puts("Z");

	return ret_val;
}

void dwValueSetting(char number, int x, int y, long init_val, long min_val, long max_val, int max_digit) //1114
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
	
	START_SETTING:
	
	
	ret_val=init_val; // 기존 값을 저장 
{ 
	value_buffer[0] = (ret_val%100000)/10000;
	value_buffer[1] = (ret_val%10000)/1000;
	value_buffer[2] = (ret_val%1000)/100;
	value_buffer[3] = (ret_val%100)/10;
	value_buffer[4] = (ret_val%10);
}

if(number==13)// device serial ,장비제조 번호
{ 
	value_buffer[0] = (ret_val%10000000)/1000000;
	value_buffer[1] = (ret_val%1000000)/100000;
	value_buffer[2] = (ret_val%100000)/10000;
	value_buffer[3] = (ret_val%10000)/1000;
	value_buffer[4] = (ret_val%1000)/100;
  value_buffer[5] = (ret_val%100)/10;
	value_buffer[6] = (ret_val%10);
 
}





	if((1 <= number) && (number <= 14)) // setting number 16
	{
		sprintf( str_1, set_mode_msg[number-1], number, curr_num);
	}
	
		switch(number)
	{	
		case 1: 
			      if(gnlLanguage==1)
		         GLCD_string (0,2 ,"정전시 중량기억");
						else
						 GLCD_string (0,2 ,"WEIGHT BACKUP   ");
		        sprintf(str_1, "VALUE : %5ld   ",(long)ret_val);		
	       		break;
	  case 2:
            if(gnlLanguage==1)
		         GLCD_string (0,2 ,"표시속도 설정  ");
						else
				    GLCD_string (0,2 ,"DIGITAL FILTER  ");
		        sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
	 case 3: 
		         if(gnlLanguage==1)
		         GLCD_string (0,2 ,"완료지연시간설정");
						else
			       GLCD_string (0,2 ,"END DELAY       ");
		        sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
	 case 4: 
		         if(gnlLanguage==1)
		         GLCD_string (0,2 ,"호파갯수 설정   ");
						else
			       GLCD_string (0,2 ,"SILO SELECTION  ");
		        sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
	 case 5: 
		         if(gnlLanguage==1)
		         GLCD_string (0,2 ,"실무게 입력설정");
						else
			       GLCD_string (0,2 ,"CURRENT WEIGHT ");
	           sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
	           break;
	 case 6: 
		        if(gnlLanguage==1)
		         GLCD_string (0,2 ,"무게미세 조정   ");
						else
			       GLCD_string (0,2 ,"WEIGHT FACTOR  ");
	           sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
	           break;
		case 7: 
			       if(gnlLanguage==1)
		     	 GLCD_string (0,2 ,"장비번호(RS-422)");
						else
			   	GLCD_string (0,2 ,"ADDRESS(RS-422)");
		         sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
		case 8: 
			      if(gnlLanguage==1)
		         GLCD_string (0,2 ,"    언어설정    ");
						else
			       GLCD_string (0,2 ,"LANGUAGE        ");
		         sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
	 case 9: 
			      if(gnlLanguage==1)
						  GLCD_string (0,2 ,"    전화사용    ");
						else
						   GLCD_string (0,2 ,"CDMA            ");
		         sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
   case 10: 
			      if(gnlLanguage==1)
						  GLCD_string (0,2 ," 펄스 무게 설정 ");
						else
						 //GLCD_string (0,2 ,"CDMA            ");
						   GLCD_string (0,2 ,"PULSE SET WEIGHT");
		         sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
   case 11: 
			      if(gnlLanguage==1)
						  GLCD_string (0,2 ," 하한 배출 에러 ");
						else
						   GLCD_string (0,2 ,"LOW OUPUT ERROR ");
		         sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
	           break;
	case 12: 
			      if(gnlLanguage==1)
						  GLCD_string (0,2 ," 상한 배출 에러 ");
						else
						   GLCD_string (0,2 ,"HIGH OUPUT ERROR");
		         sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
 case 13: 
			      if(gnlLanguage==1)
						  GLCD_string (0,2 ," 장비 제조 번호 ");
						else
						   GLCD_string (0,2 ,"DEVICE SERIAL   ");
		         sprintf(str_1, "VALUE : M%7ld",(long)ret_val); // A/D값 
	           break;
	case 14: 
			       if(gnlLanguage==1)
						    GLCD_string (0,2 ," 전송 시간 설정 ");
						 else
						     GLCD_string (0,2 ,"SENDING TIME    ");
		         sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값  
				     break;
						default: break;
	}
			GLCD_string(0,4, str_1);
	
	blink==2000;
	do
	{
		
	  ret_val = value_buffer[0]*10000 + value_buffer[1]*1000 + value_buffer[2]*100 + value_buffer[3]*10 + value_buffer[4]; // capacity value 
		if(number==13) // device serial
		{	
  	ret_val = value_buffer[0]*1000000 + value_buffer[1]*100000 + value_buffer[2]*10000 + value_buffer[3]*1000 + value_buffer[4]*100+
		          value_buffer[5]*10 + value_buffer[6]; 		
    }
	 blink++;
		if(blink==3000)
		{
  		
			if(number==13) // device serial
			{
			  sprintf(str_1, "VALUE : M%7ld",(long)ret_val); // A/D값 		 
			  GLCD_string(0,4, str_1);
	      lcd_inverse();
			  sprintf(str_1, "%1d",(int)value_buffer[6-digit_buffer]); // A/D값 
			  GLCD_string((15-digit_buffer),4, str_1);
				
			}
  			
	  else
		{			
			
			if(number==5 ||number==6||number==12||number==11) 
			sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
			 else
		  sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 		 
			GLCD_string(0,4, str_1);
			lcd_inverse();
			sprintf(str_1, "%1d",(int)value_buffer[4-digit_buffer]); // A/D값 
			GLCD_string((12-digit_buffer),4, str_1);
		 }
			 
		}
		
		else if(blink>6000)
		{
			blink=0;
			lcd_normal();
	switch(number)
	{	
		case 1: 
				    if(gnlLanguage==1)
		         GLCD_string (0,2 ,"정전시 중량기억");
						else
			      GLCD_string (0,2 ,"WEIGHT BACKUP   ");
		        sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
	  case 2: 
			      if(gnlLanguage==1)
		         GLCD_string (0,2 ,"표시속도 설정  ");
						else
			      GLCD_string (0,2 ,"DIGITAL FILTER  ");
		        sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
	 case 3: 
		        if(gnlLanguage==1)
		         GLCD_string (0,2 ,"완료지연시간설정");
						else
			       GLCD_string (0,2 ,"END DELAY       ");
		        sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
	 case 4: 
		        if(gnlLanguage==1)
		         GLCD_string (0,2 ,"호파갯수 설정   ");
						else
			       GLCD_string (0,2 ,"SILO SELECTION  ");
		        sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
	 case 5: 
		       if(gnlLanguage==1)
		         GLCD_string (0,2 ,"실무게 입력설정");
						else
			       GLCD_string (0,2 ,"CURRENT WEIGHT ");
	           sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
	           break;
	 case 6: 
		        if(gnlLanguage==1)
		         GLCD_string (0,2 ,"무게미세 조정   ");
						else
			       GLCD_string (0,2 ,"WEIGHT FACTOR  ");
	           sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
	           break;
		case 7:
    if(gnlLanguage==1)
		     	 GLCD_string (0,2 ,"장비번호(RS-422)");
						else
			   	GLCD_string (0,2 ,"ADDRESS(RS-422)");
						sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값
		     			
			/*
			      if(gnlLanguage==1)
		         GLCD_string (0,2 ,"장비번호 설정   ");
						else
			       GLCD_string (0,2 ,"DEVICE ADDRESS");
		         sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	         */
    break;
		case 8: 
		       if(gnlLanguage==1)
		         GLCD_string (0,2 ,"    언어설정    ");
						else
				   GLCD_string (0,2 ,"LANGUAGE        ");
		         sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
	 case 9: 
			      if(gnlLanguage==1)
						  GLCD_string (0,2 ,"    전화사용    ");
						else
						   GLCD_string (0,2 ,"CDMA            ");
		         sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;

	   case 10: 
			      if(gnlLanguage==1)
						  GLCD_string (0,2 ," 펄스 무게 설정 ");
						else
						   GLCD_string (0,2 ,"PULSE SET WEIGHT");
		         sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
					
		 case 11: 
			      if(gnlLanguage==1)
						  GLCD_string (0,2 ," 하한 배출 에러 ");
						else
						   GLCD_string (0,2 ,"LOW OUPUT ERROR ");
		         sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
	           break;
		case 12: 
			      if(gnlLanguage==1)
						  GLCD_string (0,2 ," 상한 배출 에러 ");
						else
						   GLCD_string (0,2 ,"HIGH OUPUT ERROR");
		         sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
	           break;
	  case 13: 
			      if(gnlLanguage==1)
						  GLCD_string (0,2 ," 장비 제조 번호 ");
						else
						   GLCD_string (0,2 ,"DEVICE SERIAL   ");
		         //sprintf(str_1, "VALUE : %7ld   ",(long)ret_val); // A/D값 
						  if(gnlLanguage==1)
						  GLCD_string (0,2 ," 장비 제조 번호 ");
						else
						   GLCD_string (0,2 ,"DEVICE SERIAL   ");
		         //sprintf(str_1, "VALUE :M%7ld",(long)ret_val); // A/D값 
	           sprintf(str_1, "VALUE : M%7ld",(long)ret_val); // A/D값 
	           break;
		case 14: 
			       if(gnlLanguage==1)
						    GLCD_string (0,2 ," 전송 시간 설정 ");
						 else
						     GLCD_string (0,2 ,"SENDING TIME    ");
		         sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값  
				     break;
		default: break;
	}
	  
	 GLCD_string(0,4, str_1);
	 if(number==13) // device serial
		{
     	sprintf(str_1, "%1d",(int)value_buffer[6-digit_buffer]); // A/D값 
			GLCD_string((15-digit_buffer),4, str_1);
		}	
   else
		{
     	sprintf(str_1, "%1d",(int)value_buffer[4-digit_buffer]); // A/D값 
			GLCD_string((12-digit_buffer),4, str_1);
		}	
 
	}
	
	
	
	
	
	
		if(keypush_8510())
		{		 
			switch(KeyCode)
			{
				case UP:
				  if(number==13)//device serial
          {						
					  value_buffer[6-digit_buffer]++;
					  value_buffer[6-digit_buffer] %= 10;
				  }
          else
          {						
					 value_buffer[4-digit_buffer]++;
						
						
					if(number==1 || number==8 ||number==9  ) //weight backup, 언어선택, 전화선택
          {						
           if(value_buffer[4-digit_buffer]>1)
					 {
						 retry_message();
						 value_buffer[4-digit_buffer]=1;
					 }	 
				 }
					
					else if(number==4) //SILO SELCETION
          {						
           if(value_buffer[4-digit_buffer]>2)
					 {
						 retry_message();
						 value_buffer[4-digit_buffer]=2;
					 }	 
				 }
					else
					 value_buffer[4-digit_buffer] %= 10;
				 

					}
						
						break;

				case DOWN:
					if(number==13)//device serial
					{	
				  if(value_buffer[6-digit_buffer]) value_buffer[6-digit_buffer]--;
					else value_buffer[6-digit_buffer] = 9;
					}
					else
				  {	
				   
					if(number==1 ||number==8 ||number==9 )//silo selction, weight backup, 언어선택, 전화선택
					 {
					  //if(value_buffer[4-digit_buffer]) value_buffer[4-digit_buffer]--;
						  value_buffer[4-digit_buffer]--;
						 
						 if(value_buffer[4-digit_buffer]<0)
						 {
							retry_message();
							value_buffer[4-digit_buffer]=0; 
							 
						 }	 
					 }
						
						
						else if(number==4 )//silo selction, weight backup, 언어선택, 전화선택
					 {
					  if(value_buffer[4-digit_buffer]) value_buffer[4-digit_buffer]--;
						 if(value_buffer[4-digit_buffer]<1)
						 {
							retry_message();
							value_buffer[4-digit_buffer]=1; 
							 
						 }	 
					 } 	 
						 
            else
            {							
					  if(value_buffer[4-digit_buffer]) value_buffer[4-digit_buffer]--;
						else
						value_buffer[4-digit_buffer] = 9;
					  }
					
					}		
				
				   break;

				case RIGHT:
					digit_buffer++;
				  switch(number)
          {
						case 1: digit_buffer %= 1;break; //weight backup
						case 2: digit_buffer %= 2;break; // digital filter
						case 3: digit_buffer %= 3;break;
						case 4: digit_buffer %= 1;break;
						case 5: digit_buffer %= 5;break;
						case 6: digit_buffer %= 5;break;
						case 7: digit_buffer %= 2;break;
						case 8: digit_buffer %= 1;break;
						case 9: digit_buffer %= 1;break;
						case 10: digit_buffer%= 2;break;
						case 11: digit_buffer%= 4;break;
						case 12: digit_buffer%= 5;break;
						case 13: digit_buffer%= 7;break;
						case 14: digit_buffer%= 2;break;
						
						default: break;
          }//of switch 						
				  break;				 
/*
				case LEFT:
					exit_flag = 1;
					break;
*/
				case ENTER:
			
				  if(number==5)// 실무게 입력이면 
						{
							 if(init_val!=ret_val) //집어넣은 값과 기존값이 다르면 
							   Value_Changed=1;
						  exit_flag=1;
						}	
	    	else if(number==2) //digital filter
					{
             if(ret_val>50)
						 {ret_val=50;
						   retry_message();
							// ret_val=50;
						 }
           else						 
             exit_flag=1;
					}
       else if(number==3) // enddelay
					{
             if(ret_val>500)
						 {ret_val=500;
						   retry_message();
							// ret_val=50;
						 }
           else						 
             exit_flag=1;
					}
				else if(number==10) // pulse set weight
					{
             if(ret_val>99)
						 {ret_val=99;
						   retry_message();
							// ret_val=50;
						 }
           else						 
             exit_flag=1;
					}
       else if(number==6) //Real_Weight_Factor
					{
             if(ret_val<7000 || ret_val>13000)
						 {ret_val=10000;
						   retry_message();
							// ret_val=50;
						 }
           else						 
             exit_flag=1;
					}					
					
				else if(number==14) // sending interval
					{
             if(ret_val>60)
						 {ret_val=10;
						   retry_message();
							// ret_val=50;
						 }
           else						 
             exit_flag=1;
					}	
					
         else
          exit_flag=1;					 
						 
						 
					break;
			
			
			
			}// of switch
		}// of  keypush_8510
	
	}while (!exit_flag);

	//if (changed)
	{
		switch(number)
		{
		   
			case 1: gxhWeightBackup = ret_val; break;
		  case 2: gxhFilter = ret_val; break;
		  case 3: gxhEndDelay = ret_val; break;
			case 4: gnlSilo1Selection = ret_val; break;
	    case 5: gplCompensationWeight = ret_val; break;
			case 6: Real_Weight_Factor=ret_val; break;
			case 7: gnlMyAddress=ret_val; break;
			case 8: gnlLanguage = ret_val; break;
			case 9: gnlCdmaUse = ret_val; break;
			case 10: gxuPulseSetWeigh = ret_val; break;
			case 11: gxuOutputErrorWeigh = ret_val; break;
			case 12: gxuOutputHighErrorWeigh=ret_val; break;
			case 13: gnlSerialNo=ret_val; break;
			case 14: gnlSendingInterval=ret_val; break;
			
			//gnlSerialNo
			default: break;
		} // of switch
	}
	
	lcd_normal();
}  /* of dwValue_set */


//0220

void dwValueSetting2(char number, int x, int y, long init_val, long min_val, long max_val, int max_digit) //1114
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
	
	ret_val=init_val; // 기존 값을 저장 
{ 
	value_buffer[0] = (ret_val%100000)/10000;
	value_buffer[1] = (ret_val%10000)/1000;
	value_buffer[2] = (ret_val%1000)/100;
	value_buffer[3] = (ret_val%100)/10;
	value_buffer[4] = (ret_val%10);
}


if(number==15)// device serial ,장비제조 번호
{ 
	value_buffer[0] = (ret_val%10000000)/1000000;
	value_buffer[1] = (ret_val%1000000)/100000;
	value_buffer[2] = (ret_val%100000)/10000;
	value_buffer[3] = (ret_val%10000)/1000;
	value_buffer[4] = (ret_val%1000)/100;
  value_buffer[5] = (ret_val%100)/10;
	value_buffer[6] = (ret_val%10);
 
}



	if((1 <= number) && (number <= 16)) // setting number 16
	{
		sprintf( str_1, set_mode_msg[number-1], number, curr_num);
	}
	
		switch(number)
	{	
		case 1: 
			      if(gnlLanguage==1)
		         GLCD_string (0,2 ,"정전시 중량기억");
						else
						 GLCD_string (0,2 ,"WEIGHT BACKUP   ");
		        sprintf(str_1, "VALUE : %5ld   ",(long)ret_val);		
	       		break;
	  case 2:
            if(gnlLanguage==1)
		         GLCD_string (0,2 ,"표시속도 설정  ");
						else
				    GLCD_string (0,2 ,"DIGITAL FILTER  ");
		        sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
	 case 3: 
		         if(gnlLanguage==1)
		         GLCD_string (0,2 ,"완료지연시간설정");
						else
			       GLCD_string (0,2 ,"END DELAY       ");
		        sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
	 case 4: 
		         if(gnlLanguage==1)
		         GLCD_string (0,2 ,"호파갯수 설정   ");
						else
			       GLCD_string (0,2 ,"SILO SELECTION  ");
		        sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
	 case 5: 
		         if(gnlLanguage==1)
		         GLCD_string (0,2 ,"실무게 입력설정1");
						else
			       GLCD_string (0,2 ,"CURRENT WEIGHT1 ");
	           sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
	           break;
	 
	 case 6: 
		         if(gnlLanguage==1)
		         GLCD_string (0,2 ,"실무게 입력설정2");
						else
			       GLCD_string (0,2 ,"CURRENT WEIGHT2 ");
	           sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
	           break;
	 
	 case 7: 
		        if(gnlLanguage==1)
		         GLCD_string (0,2 ,"무게미세 조정1  ");
						else
			       GLCD_string (0,2 ,"WEIGHT FACTOR1  ");
	           sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
	           break;
		
	 case 8: 
		        if(gnlLanguage==1)
		         GLCD_string (0,2 ,"무게미세 조정2  ");
						else
			       GLCD_string (0,2 ,"WEIGHT FACTOR2  ");
	           sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
	           break;
		 
	 //20180730
		case 9: 
			     /*
			       if(gnlLanguage==1)
		         GLCD_string (0,2 ,"장비번호 설정  ");
						else
			       GLCD_string (0,2 ,"DEVICE ADDRESS");
          */
        if(gnlLanguage==1)
		     	 GLCD_string (0,2 ,"장비번호(RS-422)");
						else
			   	GLCD_string (0,2 ,"ADDRESS(RS-422) ");
		         sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	     		
		sprintf(str_1, "VALUE : %2ld   ",(long)ret_val); // A/D값 
	           break;
		case 10: 
			      if(gnlLanguage==1)
		         GLCD_string (0,2 ,"    언어설정    ");
						else
			       GLCD_string (0,2 ,"LANGUAGE        ");
		         sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
	  case 11:
      if(gnlLanguage==1)
						  GLCD_string (0,2 ,"    전화사용    ");
						else
						   GLCD_string (0,2 ,"CDMA            ");
		         sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
						
	 case 12: 
			      if(gnlLanguage==1)
						  GLCD_string (0,2 ," 펄스 무게 설정 ");
						else
						 //GLCD_string (0,2 ,"CDMA            ");
						   GLCD_string (0,2 ,"PULSE SET WEIGHT");
		         sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
   case 13: 
			      if(gnlLanguage==1)
						  GLCD_string (0,2 ," 하한 배출 에러 ");
						else
						   GLCD_string (0,2 ,"LOW OUPUT ERROR ");
		         sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
	           break;
	case 14: 
			      if(gnlLanguage==1)
						  GLCD_string (0,2 ," 상한 배출 에러 ");
						else
						   GLCD_string (0,2 ,"HIGH OUPUT ERROR");
		         sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
	           break;
   case 15: 
			      if(gnlLanguage==1)
						  GLCD_string (0,2 ," 장비 제조 번호 ");
						else
						   GLCD_string (0,2 ,"DEVICE SERIAL   ");
		         sprintf(str_1, "VALUE : M%7ld",(long)ret_val); // A/D값 
	           break;
	 case 16:
         	   if(gnlLanguage==1)
						    GLCD_string (0,2 ," 전송 시간 설정 ");
						 else
						     GLCD_string (0,2 ,"SENDING TIME    ");
		         sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값  
				     break;
		
 	 
						
					
						

		default: break;
	}
			GLCD_string(0,4, str_1);
	
	blink==2000;
	do
	{
		
	  ret_val = value_buffer[0]*10000 + value_buffer[1]*1000 + value_buffer[2]*100 + value_buffer[3]*10 + value_buffer[4]; // capacity value 
		
	if(number==15) // device serial
		{	
  	ret_val = value_buffer[0]*1000000 + value_buffer[1]*100000 + value_buffer[2]*10000 + value_buffer[3]*1000 + value_buffer[4]*100+
		          value_buffer[5]*10 + value_buffer[6]; 		
    }
	
		 

	 blink++;
		if(blink==3000)
		{
		if(number==15) // device serial
			{
			  sprintf(str_1, "VALUE : M%7ld",(long)ret_val); // A/D값 		 
			  GLCD_string(0,4, str_1);
	      lcd_inverse();
			  sprintf(str_1, "%1d",(int)value_buffer[6-digit_buffer]); // A/D값 
			  GLCD_string((15-digit_buffer),4, str_1);
				
			}	
  		
		 else
     {			 
			if(number==5 ||number==6 ||number==7 ||number==8||number==13||number==14 ) 
			sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
			 else
		  sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 		 
			
			
			GLCD_string(0,4, str_1);
			lcd_inverse();
			sprintf(str_1, "%1d",(int)value_buffer[4-digit_buffer]); // A/D값 
			GLCD_string((12-digit_buffer),4, str_1);
		 }
		
		
		}
		
		else if(blink>6000)
		{
			blink=0;
			lcd_normal();
	switch(number)
	{	
		case 1: 
				    if(gnlLanguage==1)
		         GLCD_string (0,2 ,"정전시 중량기억");
						else
			      GLCD_string (0,2 ,"WEIGHT BACKUP   ");
		        sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
	  case 2: 
			      if(gnlLanguage==1)
		         GLCD_string (0,2 ,"표시속도 설정  ");
						else
			      GLCD_string (0,2 ,"DIGITAL FILTER  ");
		        sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
	 case 3: 
		        if(gnlLanguage==1)
		         GLCD_string (0,2 ,"완료지연시간설정");
						else
			       GLCD_string (0,2 ,"END DELAY       ");
		        sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
	 case 4: 
		        if(gnlLanguage==1)
		         GLCD_string (0,2 ,"호파갯수 설정   ");
						else
			       GLCD_string (0,2 ,"SILO SELECTION  ");
		        sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
						
	  case 5: 
		         if(gnlLanguage==1)
		         GLCD_string (0,2 ,"실무게 입력설정1");
						else
			       GLCD_string (0,2 ,"CURRENT WEIGHT1 ");
	           sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
	           break;
	 
	 case 6: 
		         if(gnlLanguage==1)
		         GLCD_string (0,2 ,"실무게 입력설정2");
						else
			       GLCD_string (0,2 ,"CURRENT WEIGHT2 ");
	           sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
	           break;
	 
	 case 7: 
		        if(gnlLanguage==1)
		         GLCD_string (0,2 ,"무게미세 조정1  ");
						else
			       GLCD_string (0,2 ,"WEIGHT FACTOR1  ");
	           sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
	           break;
		
	 case 8: 
		        if(gnlLanguage==1)
		         GLCD_string (0,2 ,"무게미세 조정2  ");
						else
			       GLCD_string (0,2 ,"WEIGHT FACTOR2  ");
	           sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
	           break;
		 
	 
		case 9: 
			      /*
			       if(gnlLanguage==1)
		         GLCD_string (0,2 ,"장비번호 설정  ");
						else
			       GLCD_string (0,2 ,"DEVICE ADDRESS");
		        */  
		
		       sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
		case 10: 
			      if(gnlLanguage==1)
		         GLCD_string (0,2 ,"    언어설정    ");
						else
			       GLCD_string (0,2 ,"LANGUAGE        ");
		         sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
    case 11:
						if(gnlLanguage==1)
						 GLCD_string (0,2 ,"    전화사용    ");
						else
						 GLCD_string (0,2 ,"CDMA            ");
		         sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
		case 12: 
			      if(gnlLanguage==1)
						  GLCD_string (0,2 ," 펄스 무게 설정 ");
						else
						 //GLCD_string (0,2 ,"CDMA            ");
						   GLCD_string (0,2 ,"PULSE SET WEIGHT");
		         sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값 
	           break;
   case 13: 
			      if(gnlLanguage==1)
						  GLCD_string (0,2 ," 하한 배출 에러 ");
						else
						   GLCD_string (0,2 ,"LOW OUPUT ERROR ");
		         sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
	           break;
	case 14: 
			      if(gnlLanguage==1)
						  GLCD_string (0,2 ," 상한 배출 에러 ");
						else
						   GLCD_string (0,2 ,"HIGH OUPUT ERROR");
		         sprintf(str_1, "VALUE : %05ld   ",(long)ret_val); // A/D값 
	           break;
	 case 15: 
			      if(gnlLanguage==1)
						  GLCD_string (0,2 ," 장비 제조 번호 ");
						else
						   GLCD_string (0,2 ,"DEVICE SERIAL   ");
		         //sprintf(str_1, "VALUE : %7ld   ",(long)ret_val); // A/D값 
				
	           sprintf(str_1, "VALUE : M%7ld",(long)ret_val); // A/D값 
	           break;
					
		case 16:
         	   if(gnlLanguage==1)
						    GLCD_string (0,2 ," 전송 시간 설정 ");
						 else
						     GLCD_string (0,2 ,"SENDING TIME    ");
		         sprintf(str_1, "VALUE : %5ld   ",(long)ret_val); // A/D값  
				     break;
				
						
						
						
	 
		default: break;
	}
			GLCD_string(0,4, str_1);
			if(number==15) // device serial
		{
     	sprintf(str_1, "%1d",(int)value_buffer[6-digit_buffer]); // A/D값 
			GLCD_string((15-digit_buffer),4, str_1);
		}
			
	    else
			{	
	    sprintf(str_1, "%1d",(int)value_buffer[4-digit_buffer]); // A/D값 
			GLCD_string((12-digit_buffer),4, str_1);
			}
  }
	
	
	
	
	
	
		if(keypush_8510())
		{		 
			switch(KeyCode)
			{
				case UP:
					/*
					value_buffer[4-digit_buffer]++;
					value_buffer[4-digit_buffer] %= 10;
				  */
      if(number==15)//device serial
          {						
					 value_buffer[6-digit_buffer]++;
					 value_buffer[6-digit_buffer] %= 10;
				  }
          else
          {						
					 value_buffer[4-digit_buffer]++;
					 if(number==4) //silo selction
					 {
             if(value_buffer[4-digit_buffer]>2)
						 {
              retry_message();
							value_buffer[4-digit_buffer]=2;
             }							 
						 
					 } 
					 
					 if(number==1|| number==10||number==11) //weight backup, 언어선택, 전화선택
					 {
             if(value_buffer[4-digit_buffer]>1)
						 {
              retry_message();
							value_buffer[4-digit_buffer]=1;
             }							 
						 
					 }
					 
					 
					 
					 

          else
					value_buffer[4-digit_buffer] %= 10;
				  }				
				break;

				case DOWN:
				if(number==15)//device serial
					{	
				  if(value_buffer[6-digit_buffer]) value_buffer[6-digit_buffer]--;
					else value_buffer[6-digit_buffer] = 9;
					}
					else
				  {	
					if(number==4)//silo selction
					 {
					  if(value_buffer[4-digit_buffer]) value_buffer[4-digit_buffer]--;
						 if(value_buffer[4-digit_buffer]<1)
						 {
							retry_message();
							value_buffer[4-digit_buffer]=1; 
							 
						 }	 
					 } 	 
					 else if(number==1 ||number==10 ||number==11 )//silo selction, weight backup, 언어선택, 전화선택
					 {
					  //if(value_buffer[4-digit_buffer]) value_buffer[4-digit_buffer]--;
						  value_buffer[4-digit_buffer]--;
						 
						 if(value_buffer[4-digit_buffer]<0)
						 {
							retry_message();
							value_buffer[4-digit_buffer]=0; 
							 
						 }	 
					 }
						 
            else
            {							
					  if(value_buffer[4-digit_buffer]) value_buffer[4-digit_buffer]--;
						else
						value_buffer[4-digit_buffer] = 9;
					  }	
				 
					}
				 /*	
				if(value_buffer[4-digit_buffer]) value_buffer[4-digit_buffer]--;
					else value_buffer[4-digit_buffer] = 9;
				 */	
				break;

				case RIGHT:
					digit_buffer++;
				  switch(number)
          {
						case 1: digit_buffer %= 1;break; //weight backup
						case 2: digit_buffer %= 2;break; // digital filter
						case 3: digit_buffer %= 3;break;
						case 4: digit_buffer %= 1;break;
						case 5: digit_buffer %= 5;break;
						case 6: digit_buffer %= 5;break;
						case 7: digit_buffer %= 5;break;
						case 8: digit_buffer %= 5;break;
						case 9: digit_buffer %= 2;break;
						case 10: digit_buffer %= 1;break;
						case 11: digit_buffer %= 1;break;
						case 12: digit_buffer %= 2;break;
						case 13: digit_buffer %= 4;break;
						case 14: digit_buffer %= 5;break;
						case 15: digit_buffer %= 7;break;
						case 16: digit_buffer %= 2;break;
						
						default: break;
          }//of switch 						
				  break;				 
/*
				case LEFT:
					exit_flag = 1;
					break;
*/
				case ENTER:
						if(number==5)// 실무게 입력이면 
						{
							if(init_val!=ret_val) //집어넣은 값과 기존값이 다르면 
							  Value_Changed=1;
							  exit_flag=1;
						}
          else if(number==6)// 실무게 입력이면 
						{
							if(init_val!=ret_val) //집어넣은 값과 기존값이 다르면 
							  Value_Changed2=1;
							 exit_flag=1;
						}						
						else if(number==2) //digital filter
					{
             if(ret_val>50)
						 {ret_val=50;
						   retry_message();
							// ret_val=50;
						 }
           else						 
             exit_flag=1;
					}
       else if(number==3) // enddelay
					{
             if(ret_val>500)
						 {ret_val=500;
						   retry_message();
							// ret_val=50;
						 }
           else						 
             exit_flag=1;
					}
				else if(number==11) // pulse set weight
					{
             if(ret_val>99)
						 {ret_val=99;
						   retry_message();
							// ret_val=50;
						 }
           else						 
             exit_flag=1;
					}
       else if(number==7 || number==8 ) //Real_Weight_Factor
					{
             if(ret_val<7000 || ret_val>13000)
						 {ret_val=10000;
						   retry_message();
							// ret_val=50;
						 }
           else						 
             exit_flag=1;
					}					
					
				else if(number==16) // sending interval
					{
             if(ret_val>60)
						 {ret_val=10;
						   retry_message();
							// ret_val=50;
						 }
           else						 
             exit_flag=1;
					}		
						
			
       else
				 exit_flag=1;





						break;
					}// of switch
		}// of  keypush_8510
	
	}while (!exit_flag);

	//if (changed)
	{
		switch(number)
		{
		   
			case 1: gxhWeightBackup = ret_val; break;
		  case 2: gxhFilter = ret_val; break;
		  case 3: gxhEndDelay = ret_val; break;
			case 4: gnlSilo1Selection = ret_val; break;
		  case 5: gplCompensationWeight = ret_val; break;
			case 6: gplCompensationWeight2 = ret_val; break;
			
			
			case 7: Real_Weight_Factor=ret_val; break;
			case 8: Real_Weight_Factor2=ret_val; break;
			
			case 9: gnlMyAddress=ret_val; break;
			case 10: gnlLanguage = ret_val; break;
			case 11: gnlCdmaUse = ret_val; break;
			case 12: gxuPulseSetWeigh = ret_val; break;
			case 13: gxuOutputErrorWeigh = ret_val; break;
			case 14: gxuOutputHighErrorWeigh=ret_val; break;
			case 15: gnlSerialNo=ret_val; break;
			case 16: gnlSendingInterval=ret_val; break;
			//20180730
		
			
			
			
			
			default: break;
		} // of switch
	}
	lcd_normal();
	
}  /* of dwValue_set */





void dwValueSetting_simple(char number, int x, int y, long init_val, long min_val, long max_val, int max_digit) //1114
{
	char  exit_flag =0;
	char  changed = 0;
	char  index = 0;	//only HIDE mode, instead number to '-' 
	long  curr_num, tmp_num;
	char v_key;
	char str_1[50];
    long temp_mim,temp_max;
   
   	curr_num = init_val;

	if((1 <= number) && (number <= 13))
	{
		sprintf( str_1, set_mode_msg_simple[number-1], number, curr_num);
	}
//	lcd70_show_string_mode(x, y,str_1, 16, 0);
//	lcd70_show_string_mode_24x24( x,  y, str_1, 16, 0);		   //201605
//	lcd43_show_big_num(x,y,init_val);
	
	
	do
	{
		if (keypush())
		{
			switch (v_key = key_disp[KEYCODE])
			{
				case 0:  case 1:  case 2:  case 3:  case 4:
				case 5:  case 6:  case 7:  case 8:  case 9:

					if(max_digit <= 1) tmp_num = v_key;
					else tmp_num = curr_num * 10 + v_key;
					
					/*
					if((tmp_num < min_val) || (max_val < tmp_num))
					{
						retry_message();
					}
					else
					{
						changed = 1;
						if(max_digit <= 1) curr_num = v_key;
						else 
						{	curr_num = curr_num * 10 + v_key;
							if (curr_num)index++;
						}
					}
					 */
                  		changed = 1;
						if(max_digit <= 1) curr_num = v_key;
						else 
						{	curr_num = curr_num * 10 + v_key;
							if (curr_num)index++;
						}
					break;
						
				case KEY_CLEAR:
					changed = 1;
					curr_num =0;
					index = 0;
					break;
				
				case KEY_ENTER: /* ON/OFF Key */
					exit_flag =1;
					break;
				
				default: /*retry_message();*/  break;
			} /* of switch */

			if(changed)
			{
				if((1 <= number) && (number <= 13))
				{
					sprintf( str_1, set_mode_msg_simple[number-1], number, curr_num);
				}
//				lcd70_show_string_mode_24x24( x,  y, str_1, 16, 0);		   //201605
				//lcd70_show_string_mode(x, y,str_1, 16, 0);
				//lcd43_show_big_num(x,y,curr_num);
			}
			//lcd70_show_string_mode(0, 310,str_1, 16, 0);
		} /* of if (keypush())*/
	}while (!exit_flag);

	if (changed)
	{
		switch(number)
		{
		   case 1: gxhWeightBackup=curr_num; break;
		   case 2: gxhEndDelay=curr_num; break;
	       case 3: gplCompensationWeight = calcCompensationWeight(curr_num);
/*			   
				   gplCompensationWeight=curr_num;
  				   while(!Weight_flag) temp_weight=Get_adc_from_module_20170626();
				   while(!Weight_flag) temp_weight=Get_adc_from_module_20170626();
				   while(!Weight_flag) temp_weight=Get_adc_from_module_20170626();
				   while(!Weight_flag) temp_weight=Get_adc_from_module_20170626();
	               v_adc1_buf = (long)(gnfFactor * temp_weight);
			       diff1 = v_adc1_buf - v_adc_org;
			       gxlGrossWeight = ((diff1 + 10) / v_ei_multiply_factor) * v_division;
				   gplCompensationWeight -= gxlGrossWeight;
*/			
				   break;
			case 4: Real_Weight_Factor=curr_num; break;
			case 5: gnlDisplay=curr_num; break;
			case 6: gnlHenhouseNo = curr_num; break;
			case 7: gnlUseLTE = curr_num; break;

			default: break;
		} // of switch
	}
}  /* of dwValue_set */

/*
	dwValueSetting_simple( ++idx, 150, y_start + (idx * y_line_spacing),(long)gxhWeightBackup, MIN_gxhWeightBackup, MAX_gxhWeightBackup, 1);  //weight backup 
	dwValueSetting_simple( ++idx, 150, y_start + (idx * y_line_spacing),(long)gxhEndDelay, MIN_gxhEndDelay, MAX_gxhEndDelay, MAX_gxhEndDelay);  //end delay
	dwValueSetting_simple( ++idx, 150, y_start + (idx * y_line_spacing),(long)gplCompensationWeight, MIN_gplCompensationWeight, MAX_gplCompensationWeight, MAX_gplCompensationWeight);  //compensation
	dwValueSetting_simple( ++idx, 150, y_start + (idx * y_line_spacing),(long)Real_Weight_Factor, MIN_Real_Weight_Factor, MAX_Real_Weight_Factor, MAX_Real_Weight_Factor);  //무게 보정 factor
	dwValueSetting_simple( ++idx, 150, y_start + (idx * y_line_spacing),(long)gnlDisplay, MIN_gnlDisplay, MAX_gnlDisplay, 1);  //디스플레이 서렁  
	dwValueSetting_simple( ++idx, 150, y_start + (idx * y_line_spacing),(long)gnlHenhouseNo, MIN_gnlHenhouseNo, MAX_gnlHenhouseNo, MAX_gnlHenhouseNo);
	dwValueSetting_simple( ++idx, 150, y_start + (idx * y_line_spacing),(long)gnlUseLTE, MIN_gnlUseLTE, MAX_gnlUseLTE, MAX_gnlUseLTE);

  
  
  */

 long Get_Value(int x, int y)
 {
	char  exit_flag =0;
	char  changed = 0;
	char  index = 0;		// only HIDE mode, instead number to '-' 
	long  curr_num = 0;
	char v_key;
	char str_1[50];
//	union { u32 dw; u16 u[2]; u8 c[4];} temp;
	long value=0;
	
	
	 sprintf(str_1, "VALUE=%6lu ",(long)value);
//	 lcd70_show_string_mode(x, y,str_1, 16, 0);



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

			//sprintf(str_1, "settint_weight=%6lu ",(long)curr_num);
            if(changed)
			{
			  sprintf(str_1, "VALUE=%6lu ",(long)curr_num);
//	          lcd70_show_string_mode(x, y,str_1, 16, 0);
			
			}
	        //lcd70_show_string_mode(0, 310,str_1, 16, 0);
		} /* of if (keypush())*/
	}while (!exit_flag);
	if (changed)
	 {
	  	return curr_num;
	 }
	 
}  /* of dwValue_set */



//#endif



long get_key( int x, int y, char *msg, long val, int max_digit)
{
	char  exit_flag =0;
	char  changed = 0;
	char  index = 0;	//only HIDE mode, instead number to '-' 
	long  curr_num, tmp_num;
	char	v_key;
	int		display_flag = 1;
	char str_1[100];
//    long temp_mim,temp_max;
   
   	curr_num = val;

	do
	{
		if (display_flag) {
			display_flag = 0;
			sprintf( str_1, "%s %1ld   ", msg, curr_num);
//			lcd70_show_string_mode_24x24( x,  y, str_1, 16, 0);		   //201605
		}
		
		if (keypush())
		{
			switch (v_key = key_disp[KEYCODE])
			{
				case 0:  case 1:  case 2:  case 3:  case 4:
				case 5:  case 6:  case 7:  case 8:  case 9:

					if(max_digit <= 1) tmp_num = v_key;
					else tmp_num = curr_num * 10 + v_key;
					
					changed = 1;
					if(max_digit <= 1) curr_num = v_key;
					else 
					{	curr_num = curr_num * 10 + v_key;
						if (curr_num)index++;
					}
					break;
						
				case KEY_CLEAR:
					changed = 1;
					curr_num =0;
					index = 0;
					break;
				
				case KEY_ENTER: /* ON/OFF Key */
					exit_flag =1;
					break;
				
				default: retry_message();  break;
			} /* of switch */

			display_flag = 1;
		} /* of if (keypush())*/
	} while (!exit_flag);

//	lcd70_show_string_mode_24x24( x,  y, "                              ", 16, 0);	
	
	return curr_num;
}






void set_date()
{
	int i;
	char loc=0;
	int  count;
	int  blink;
	unsigned char	digit_max[6] = { 9, 9, 1, 9, 3, 9 };
	unsigned char	exit_flag =0;
	unsigned char	curr_num = 0;
	char			tbuf[6];
	char			year, month, date;
	char  str[50];
	lcd_normal();
	lcd_clear();
	lcd_normal();
	if(gnlLanguage==1)//국문
	GLCD_string (0,2 ," 현재날짜설정 ");
	else
		GLCD_string (0,2 ,"DATE SETTING ");
	
	
	Get1381();// 시간을 읽어오고  
	sprintf(str, "date:20%02x-%02X-%02X  time:%02x-%02X-%02X ", Gettimebuf[6],Gettimebuf[4],Gettimebuf[3],Gettimebuf[2],Gettimebuf[1],Gettimebuf[0]);	USART1_puts(str);
  
	GLCD_string (0,4 ,"현재날짜");
  sprintf(str, "20%02x-%02X-%02X-", Gettimebuf[6],Gettimebuf[4],Gettimebuf[3]);	
	/*
	lcd_inverse();
	GLCD_string (0,2 ,str);
  lcd_normal();
	*/
	tbuf[0] = Gettimebuf[6] / 16;
	tbuf[1] = Gettimebuf[6] % 16;
	tbuf[2] = Gettimebuf[4] / 16;
	tbuf[3] = Gettimebuf[4] % 16;
	tbuf[4] = Gettimebuf[3] / 16;
	tbuf[5] = Gettimebuf[3] % 16;

  sprintf(str, "20%1x%1x-%1x%1x-%1x%1x",tbuf[0],tbuf[1],tbuf[2],tbuf[3],tbuf[4],tbuf[5] );	
	GLCD_string (0,4 ,str);
 
	loc = 0;
	blink=2000;
	do
	{
		blink++;
		if(blink==2500)
		{lcd_normal();
		 switch(loc)
		 {
			 case 0:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (2,4 ,str); 			 
       break;			 
	     case 1:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (3,4 ,str); 			 
       break;			 
			 case 2:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (5,4 ,str); 			 
       break;			 
			 case 3:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (6,4 ,str); 			 
       break;			 
			 case 4:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (8,4 ,str); 			 
       break;			 
			 case 5:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (9,4 ,str); 			 
       break;			 
			 default: break; 
		 }	//of switch 
			
		}

    else if(blink>5000)
		{
    blink=0;
		lcd_inverse();
			switch(loc)
		 {
		//	 lcd_inverse();
		
			 case 0:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (2,4 ,str); 			 
       break;			 
	     case 1:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (3,4 ,str); 			 
       break;			 
			 case 2:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (5,4 ,str); 			 
       break;			 
			 case 3:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (6,4 ,str); 			 
       break;			 
			 case 4:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (8,4 ,str); 			 
       break;			 
			 case 5:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (9,4 ,str); 			 
       break;			 
			 default: break; 
		 }	//of switch 
     lcd_normal();			


    }			
		
		if (keypush_8510()) // 0830
		{
			
			switch(KeyCode) //99년 12월 31일 
			{
				case UP:
					tbuf[loc]++;
				 if(tbuf[loc]>9)
				   {
				   tbuf[loc]=9;
						retry_message(); 
			     }
					/*
					tbuf[loc]++;
				 if(loc==0 |loc==1|loc==5 )
         {					 
				 if(tbuf[loc]>9)
				   {
				   tbuf[loc]=9;
						retry_message(); 
			     }
				 }
         if(loc==2)
				 {
			    	if(tbuf[loc]>1)
						{
						retry_message();
						 tbuf[loc]=1;
						}
				 }	 
				 if(loc==3 )
				 {
			    	if(tbuf[loc]>2)
						{
						retry_message();
						 tbuf[loc]=2;
						}
				 }
				 if(loc==4 )
				 {
			    	if(tbuf[loc]>3)
						{
						retry_message();
						 tbuf[loc]=3;
						}
				 }
				 */
					break;
				
        case DOWN:
					if(tbuf[loc]!=0) 
					tbuf[loc]--;
					else
						retry_message();
				  break;
				
				case LEFT:
					Delay_ms(1);
				  break;
				case RIGHT:
					 loc++;
				   if(loc>=6)
						  loc=0;
				  break;
				case ENTER:
					exit_flag=1;
				  break;
				default: break;
			} // of switch 
			
			
			sprintf(str, "20%1x%1x-%1x%1x-%1x%1x",tbuf[0],tbuf[1],tbuf[2],tbuf[3],tbuf[4],tbuf[5] );	
	    GLCD_string (0,4 ,str);

		}// of keypush	
	} while (!exit_flag);
	
	  Settimebuf[6] =tbuf[0]*16+tbuf[1]; //年 
    Settimebuf[4] =tbuf[2]*16+tbuf[3]; 0x12; //月 
 	  Settimebuf[3] =tbuf[4]*16+tbuf[5]; 0x01; //日 
    Settimebuf[2] =Gettimebuf[2]; //? 
    Settimebuf[1] =Gettimebuf[1]; //分 
    Settimebuf[0] =Gettimebuf[0];//秒 
		Set1381(Settimebuf);//setting current data & time  
		
} /* of set_date */


void set_time()
{
	int i;
	char loc=0;
	int  count;
	int  blink;
	unsigned char	digit_max[6] = { 9, 9, 1, 9, 3, 9 };
	unsigned char	exit_flag =0;
	unsigned char	curr_num = 0;
	char			tbuf[6];
	char			year, month, date;
	char  str[50];
	lcd_clear();
	if(gnlLanguage==1)//국문
	GLCD_string (0,2 ," 현재시간설정 ");
  else
  GLCD_string (0,2 ,"TIME SETTING");

	
	Get1381();// 시간을 읽어오고  
	sprintf(str, "time:  %02x-%02X-%02X  time:%02x-%02X-%02X ", Gettimebuf[6],Gettimebuf[4],Gettimebuf[3],Gettimebuf[2],Gettimebuf[1],Gettimebuf[0]);	USART1_puts(str);
  
	GLCD_string (0,4 ,"현재시간");
  sprintf(str, "  %02x-%02X-%02X-", Gettimebuf[2],Gettimebuf[1],Gettimebuf[0]);	
	/*
	lcd_inverse();
	GLCD_string (0,4 ,str);
  lcd_normal();
	*/
	tbuf[0] = Gettimebuf[2] / 16;
	tbuf[1] = Gettimebuf[2] % 16;
	tbuf[2] = Gettimebuf[1] / 16;
	tbuf[3] = Gettimebuf[1] % 16;
	tbuf[4] = Gettimebuf[0] / 16;
	tbuf[5] = Gettimebuf[0] % 16;

  sprintf(str, "  %1x%1x-%1x%1x-%1x%1x",tbuf[0],tbuf[1],tbuf[2],tbuf[3],tbuf[4],tbuf[5] );	
	GLCD_string (0,4 ,str);
 
	loc = 0;
	blink=2000;
	do
	{
		blink++;
		if(blink==2500)
		{
		 switch(loc)
		 {
			 case 0:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (2,4 ,str); 			 
       break;			 
	     case 1:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (3,4 ,str); 			 
       break;			 
			 case 2:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (5,4 ,str); 			 
       break;			 
			 case 3:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (6,4 ,str); 			 
       break;			 
			 case 4:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (8,4 ,str); 			 
       break;			 
			 case 5:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (9,4 ,str); 			 
       break;			 
			 default: break; 
		 }	//of switch 
			
		}

    else if(blink>5000)
		{
    blink=0;
		lcd_inverse();
			switch(loc)
		 {
			 case 0:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (2,4 ,str); 			 
       break;			 
	     case 1:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (3,4 ,str); 			 
       break;			 
			 case 2:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (5,4 ,str); 			 
       break;			 
			 case 3:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (6,4 ,str); 			 
       break;			 
			 case 4:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (8,4 ,str); 			 
       break;			 
			 case 5:
				 sprintf(str, "%1x",tbuf[loc]); // A/D값
         GLCD_string (9,4 ,str); 			 
       break;			 
			 default: break; 
		 }	//of switch 
     lcd_normal();			


    }			
		
		if (keypush_8510()) // 0830
		{
			
			switch(KeyCode) //23시59분59초 
			{
				case UP:
					tbuf[loc]++;
				 if(loc==3 |loc==5 |loc==1)
         {					 
				 if(tbuf[loc]>9)
				   {
				   tbuf[loc]=9;
						retry_message(); 
			     }
				 }
         if(loc==0)
				 {
			    	if(tbuf[loc]>2)
						{
						retry_message();
						 tbuf[loc]=2;
						}
				 }	 
				
				 if(loc==4 | loc==2 )
				 {
			    	if(tbuf[loc]>5)
						{
						retry_message();
						 tbuf[loc]=5;
						}
				 }
					break;
				
        case DOWN:
					if(tbuf[loc]!=0) 
					tbuf[loc]--;
					else
						retry_message();
				  break;
				
				case LEFT:
					Delay_ms(1);
				  break;
				case RIGHT:
					 loc++;
				   if(loc>=6)
						  loc=0;
				  break;
				case ENTER:
					exit_flag=1;
				  break;
				default: break;
			} // of switch 
			
			
			sprintf(str, "  %1x%1x-%1x%1x-%1x%1x",tbuf[0],tbuf[1],tbuf[2],tbuf[3],tbuf[4],tbuf[5] );	
	    GLCD_string (0,4 ,str);

		}// of keypush	
	} while (!exit_flag);
	
	  Settimebuf[6]=Gettimebuf[6];
	  Settimebuf[4]=Gettimebuf[4];
	  Settimebuf[3]=Gettimebuf[3];
	
	
	  Settimebuf[2] =tbuf[0]*16+tbuf[1]; //年 
    Settimebuf[1] =tbuf[2]*16+tbuf[3]; 0x12; //月 
 	  Settimebuf[0] =tbuf[4]*16+tbuf[5]; 0x01; //日 
    //Settimebuf[2] = 0x13; //? 
    //Settimebuf[1] = 0x51; //分 
    //Settimebuf[0] = 0x30;//秒 
		Set1381(Settimebuf);//setting current data & time  
		
} /* of set_date */




#ifdef dpfdpf 
void set_time()
{
	int i;
	char   loc;
	int count;
	unsigned char digit_max[6] = { 2, 9, 5, 9, 5, 9 };
	unsigned char exit_flag =0;
	unsigned char curr_num = 0;
	unsigned char tbuf[6];
	byte hours, minutes, seconds;

	rtc_init();
	vputs("F2TIME");   delay(500);
	loc = 0;
	tbuf[0] = (unsigned char)(RTCHOURS / 10);
	tbuf[1] = (unsigned char)(RTCHOURS % 10);
	tbuf[2] = (unsigned char)(RTCMINUTES / 10);
	tbuf[3] = (unsigned char)(RTCMINUTES % 10);
	tbuf[4] = (unsigned char)(RTCSECONDS / 10);
	tbuf[5] = (unsigned char)(RTCSECONDS % 10);

	do
	{
		if (keypush())
		{
			v_key = key_disp[KEYCODE];
			switch(v_key)
			{
				case 0:  case 1:  case 2:  case 3:  case 4:
				case 5:  case 6:  case 7:  case 8:  case 9:
					if (loc > 5)  break;
					if (v_key <= digit_max[loc] )
					{	tbuf[loc] = v_key;
						++loc;
						if (loc > 5) loc = 0;
					}
					else retry_message();
					break;
				case CLEAR_K:  /*  left  */
					loc++;
					if (loc > 5) loc = 0;
					break;
				case SET_K:   /* Enter Key (PF_key) */
					hours   = tbuf[0] * 10 + tbuf[1];
					minutes = tbuf[2] * 10 + tbuf[3];
					seconds = tbuf[4] * 10 + tbuf[5];
					if((hours > 23) || (minutes > 59) || (seconds > 59))
					{
						vputs("ERR 52");      /* 12:00:00 */
						hours = 12; minutes = 0; seconds = 0;
						retry_message();  delay(100);
						exit_flag =0; loc = 0;
						break;
					}
					RTCHOURS = hours;
					RTCMINUTES = minutes;
					RTCSECONDS = seconds;
					exit_flag =1;
					break;
				default:
					retry_message(); break;
			} /* of switch */
		} /* of if */
		if (!exit_flag)
		{	for(i = 0; i < 6; ++i)
			{
				lputch (i, tbuf[i]);
				if(++count > 250) lputc (loc, 0);
				draw_dp(2);
				draw_dp(4);
			}
		}
		if (count > 500)  count = 0;
	} while ( !(exit_flag));
} /* of set_date */

#endif









 /* of value_set */
