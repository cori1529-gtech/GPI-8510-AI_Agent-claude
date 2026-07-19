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

const char *set_mode_msg[14] = {
	"%2d. WEIGHT BACK UP       : %6ld",
	"%2d. END DELAY            : %6ld",
//	"%2d. COMPENSATION WEIGHT  : %6ld",
	"%2d. REAL WEIGHT          : %6ld",
	"%2d. DATA SENDING TIME_1  : %6ld",
	"%2d. DATA SENDING TIME_2  : %6ld",
	"%2d. DATA SENDING TIME_3  : %6ld",
	"%2d. LOADCELL NUMBER      : %6ld",
	"%2d. SPAN  FACTOR         : %6ld",
	"%2d. DISPLAY SELECTION    : %6ld",
	
	"%2d. Serial No (MYYMMNNN) : M%07ld",
	"%2d. ID (ADDRESS : M999)  : M%03ld",
	"%2d. HENHOUSE NO          : %02ld",
	
	"%2d. LTE (1:LTE, 0:ZIGBEE): %1ld",

	"%2d. STATUS INTERVAL (MIN): %02ld"
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
	int	y_line_spacing = 35;
	int	idx = 0;

	lcd70_clear_screen(WHITE);
	lcd70_show_string_mode_24x24( 150,  30, "SET MODE", 16, 0);
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

	gplCompensationWeight = EE_Write_val( ADR_gplCompensationWeight, gplCompensationWeight, 4);


	if(Real_Weight_Factor>=7000 && Real_Weight_Factor<=13000)
	{
		Real_Weight_Factor = EE_Write_val( ADR_Real_Weight_Factor, Real_Weight_Factor, 4);
	}
	gnlDisplay = EE_Write_val( ADR_gnlDisplay, gnlDisplay, 4);
	gnlHenhouseNo = EE_Write_val( ADR_gnlHenhouseNo, gnlHenhouseNo, 4); //계사 번호
	gnlUseLTE = EE_Write_val( ADR_gnlUseLTE, gnlUseLTE, 4); //LTE 사용여부(0:지그비, 1:LTE)
	
	FSMC_ENABLE;
   
	lcd70_show_string_mode_24x24( 150, y_start + (idx++ * y_line_spacing), "SET MODE ENDING", 16, 0);
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

	lcd70_clear_screen(WHITE);
	lcd70_show_string_mode_24x24( 150,  30, "SET MODE", 16, 0);
	Delay_ms(1000);

    lcd43_show_big_num(X_START,Y_START,1);
	lcd43_show_big_num(X_START+(X_INCREMENT*1),Y_START,':');
	dwValueSetting_simple( ++idx, X_START+(X_INCREMENT*2),Y_START ,(long)gxhWeightBackup, MIN_gxhWeightBackup, MAX_gxhWeightBackup, 1);  //weight backup 


    lcd43_show_big_num(X_START,Y_START+(Y_INCREMENT*1),2);
	lcd43_show_big_num(X_START+(X_INCREMENT*1),Y_START+(Y_INCREMENT*1),':');
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
   
	lcd70_show_string_mode_24x24( 150, y_start + (idx++ * y_line_spacing), "SET MODE ENDING", 16, 0);
}

#endif




void set_mode()
{
	union { u32 dw; u16 u[2]; u8 c[4];} temp;
	//int	y_start = 70;
	//int	y_line_spacing = 25;
	int	y_start = 70;
	int	y_line_spacing = 27;
	
	
	int	idx = 0;

	lcd70_clear_screen(WHITE);
	lcd70_show_string_mode_24x24( 150, 30, "SET MODE", 16, 0);
	Delay_ms(1000);

	dwValueSetting( ++idx, 150, y_start + (idx * y_line_spacing),(long)gxhWeightBackup, MIN_gxhWeightBackup, MAX_gxhWeightBackup, 1);  //weight backup 
	dwValueSetting( ++idx, 150, y_start + (idx * y_line_spacing),(long)gxhEndDelay, MIN_gxhEndDelay, MAX_gxhEndDelay, MAX_gxhEndDelay);  //end delay
	dwValueSetting( ++idx, 150, y_start + (idx * y_line_spacing),(long)gplCompensationWeight, MIN_gplCompensationWeight, MAX_gplCompensationWeight, MAX_gplCompensationWeight);  //compensation
	dwValueSetting( ++idx, 150, y_start + (idx * y_line_spacing),(long)gnlSms_Send_Time_First, MIN_gnlSms_Send_Time_First, MAX_gnlSms_Send_Time_First, MAX_gnlSms_Send_Time_First);  //sms 시간 1
	dwValueSetting( ++idx, 150, y_start + (idx * y_line_spacing),(long)gnlSms_Send_Time_Second, MIN_gnlSms_Send_Time_Second, MAX_gnlSms_Send_Time_Second, MAX_gnlSms_Send_Time_Second);  //sms 시간 2 
	dwValueSetting( ++idx, 150, y_start + (idx * y_line_spacing),(long)gnlSms_Send_Time_Third, MIN_gnlSms_Send_Time_Third, MAX_gnlSms_Send_Time_Third, MAX_gnlSms_Send_Time_Third);  //sms 시간 3
	dwValueSetting( ++idx, 150, y_start + (idx * y_line_spacing),(long)Loadcell_Number, MIN_Loadcell_Number, MAX_Loadcell_Number, 1);  //LOADCELL 갯수
	dwValueSetting( ++idx, 150, y_start + (idx * y_line_spacing),(long)Real_Weight_Factor, MIN_Real_Weight_Factor, MAX_Real_Weight_Factor, MAX_Real_Weight_Factor);  //무게 보정 factor
	dwValueSetting( ++idx, 150, y_start + (idx * y_line_spacing),(long)gnlDisplay, MIN_gnlDisplay, MAX_gnlDisplay, 1);  //디스플레이 서렁  
	//2016.4.4	
	dwValueSetting( ++idx, 150, y_start + (idx * y_line_spacing),(long)gnlSerialNo, MIN_gnlSerialNo, MAX_gnlSerialNo, MAX_gnlSerialNo);  //strcpy(sn, "M1603007");
	dwValueSetting( ++idx, 150, y_start + (idx * y_line_spacing),(long)gnlMyAddress, MIN_gnlMyAddress, MAX_gnlMyAddress, MAX_gnlMyAddress);  //strcpy(adr, "M001");
	dwValueSetting( ++idx, 150, y_start + (idx * y_line_spacing),(long)gnlHenhouseNo, MIN_gnlHenhouseNo, MAX_gnlHenhouseNo, MAX_gnlHenhouseNo);
	//2016.4.11
	dwValueSetting( ++idx, 150, y_start + (idx * y_line_spacing),(long)gnlUseLTE, MIN_gnlUseLTE, MAX_gnlUseLTE, MAX_gnlUseLTE);
	//2016.7.4
	dwValueSetting( ++idx, 150, y_start + (idx * y_line_spacing),(long)gxwStatusSendInterval, MIN_gxwStatusSendInterval, MAX_gxwStatusSendInterval, MAX_gxwStatusSendInterval);

	//I2C_EE_BufferRead( temp.c, ADR_gnlDisplay, 4);
	//gnlDisplay=temp.dw;
	
	//ADR_Real_Weight_Factor 
	
	//I2C_EE_BufferRead( temp.c, ADR_Loadcell_Number, 4);
	//Loadcell_Number=temp.dw;

	//long gnlSms_Send_Time_First;
	//long gnlSms_Send_Time_Second;
	//long gnlSms_Send_Time_Third;

	//read_factors();
	//write_factors();
	Initiaize_eeprom();
	FSMC_DISABLE;
	
/*
	temp.dw=gxhWeightBackup;
	I2C_EE_BufferWrite(temp.c, ADR_gxhWeightBackup, 4);
	I2C_EE_BufferRead(temp.c, ADR_gxhWeightBackup, 4);
*/
	gxhWeightBackup = EE_Write_val( ADR_gxhWeightBackup, gxhWeightBackup, 4);

/*
	temp.dw=gxhEndDelay;
	I2C_EE_BufferWrite(temp.c, ADR_gxhEndDelay, 4);
	I2C_EE_BufferRead(temp.c, ADR_gxhEndDelay, 4);
*/
	gxhEndDelay = EE_Write_val( ADR_gxhEndDelay, gxhEndDelay, 4);

/*
	temp.dw=gplCompensationWeight;
	I2C_EE_BufferWrite(temp.c, ADR_gplCompensationWeight, 4);
	I2C_EE_BufferRead(temp.c, ADR_gplCompensationWeight, 4);
*/	
	gplCompensationWeight = EE_Write_val( ADR_gplCompensationWeight, gplCompensationWeight, 4);

    temp.dw=gnlSms_Send_Time_First;
/*
	I2C_EE_BufferWrite(temp.c, ADR_gnlSms_Send_Time_First, 4);
	I2C_EE_BufferRead(temp.c, ADR_gnlSms_Send_Time_First, 4);
*/
	gnlSms_Send_Time_First = EE_Write_val( ADR_gnlSms_Send_Time_First, gnlSms_Send_Time_First, 4);
	Sms_Time_First[0]=temp.c[0];
	Sms_Time_First[1]=temp.c[1];
	Sms_Time_First[2]=temp.c[2];
	Sms_Time_First[3]=temp.c[3];

    temp.dw=gnlSms_Send_Time_Second;
/*	
	I2C_EE_BufferWrite(temp.c, ADR_gnlSms_Send_Time_Second, 4);
	I2C_EE_BufferRead(temp.c, ADR_gnlSms_Send_Time_Second, 4);
*/
	gnlSms_Send_Time_Second = EE_Write_val( ADR_gnlSms_Send_Time_Second, gnlSms_Send_Time_Second, 4);
	Sms_Time_Second[0]=temp.c[0];
	Sms_Time_Second[1]=temp.c[1];
	Sms_Time_Second[2]=temp.c[2];
	Sms_Time_Second[3]=temp.c[3];

   	temp.dw=gnlSms_Send_Time_Third;
/*
	I2C_EE_BufferWrite(temp.c, ADR_gnlSms_Send_Time_Third, 4);
	I2C_EE_BufferRead(temp.c, ADR_gnlSms_Send_Time_Third, 4);
*/
	gnlSms_Send_Time_Third = EE_Write_val( ADR_gnlSms_Send_Time_Third, gnlSms_Send_Time_Third, 4);
	Sms_Time_Third[0]=temp.c[0];
	Sms_Time_Third[1]=temp.c[1];
	Sms_Time_Third[2]=temp.c[2];
	Sms_Time_Third[3]=temp.c[3];

/*
	temp.dw=Loadcell_Number;
	I2C_EE_BufferWrite(temp.c, ADR_Loadcell_Number, 4);
	I2C_EE_BufferRead(temp.c, ADR_Loadcell_Number, 4);
*/
	Loadcell_Number = EE_Write_val( ADR_Loadcell_Number, Loadcell_Number, 4);

	if(Real_Weight_Factor>=7000 && Real_Weight_Factor<=13000)
	{
/*
		temp.dw=Real_Weight_Factor;
		I2C_EE_BufferWrite(temp.c, ADR_Real_Weight_Factor, 4);
		I2C_EE_BufferRead(temp.c, ADR_Real_Weight_Factor, 4);
*/
		Real_Weight_Factor = EE_Write_val( ADR_Real_Weight_Factor, Real_Weight_Factor, 4);
	}
	
/*
	temp.dw=gnlDisplay;
	I2C_EE_BufferWrite(temp.c, ADR_gnlDisplay, 4);
	I2C_EE_BufferRead(temp.c, ADR_gnlDisplay, 4);
*/
	gnlDisplay = EE_Write_val( ADR_gnlDisplay, gnlDisplay, 4);

	//ADR_Real_Weight_Factor
	
	// I2C_EE_BufferRead( temp.c, ADR_gnlSms_Send_Time, 4);
	// gnlSms_Send_Time=temp.dw;

	gnlSerialNo = EE_Write_val( ADR_gnlSerialNo, gnlSerialNo, 4);	//장비 시리얼 번호
	gnlMyAddress = EE_Write_val( ADR_gnlMyAddress, gnlMyAddress, 4); //장비 ID(어드레스)
	gnlHenhouseNo = EE_Write_val( ADR_gnlHenhouseNo, gnlHenhouseNo, 4); //계사 번호
	gnlUseLTE = EE_Write_val( ADR_gnlUseLTE, gnlUseLTE, 4); //LTE 사용여부(0:지그비, 1:LTE)
	gxwStatusSendInterval = EE_Write_val( ADR_gxwStatusSendInterval, gxwStatusSendInterval, 4); //상태정보 전송 시간 설정

	FSMC_ENABLE;
   
	lcd70_show_string_mode_24x24( 140, y_start + (idx++ * y_line_spacing), "SET MODE ENDING", 16, 0);
}


/*
  
 int Get_division(void)//  한눈의값 구하기  
  {
    
	u8 str[50];
	u16 test_division[4]={1,2,5,10};
	u8 which; // means 140 kg 
	u8 exit_flag=0;	 
	

//		sprintf(str, "division=%2d",(int)test_division[which]);
//	lcd70_show_string_mode(200, 70,str, 16, 0);

	if(v_division==1)
	   which=0;
    if(v_division==2)
	   which=1;
    if(v_division==5)
	   which=2;
    if(v_division==10)
	   which=3;

	sprintf(str, "division=%2d",(int)test_division[which]);
	lcd70_show_string_mode(0, 70,str, 16, 0);
	
	
	 do
	  {

	   if (keypush())
      {
       if(KEYCODE==66)// 1번키가  눌리면 
	    {
	   	  which++;
		  if(which>3)
		  which=0;
	      sprintf(str, "division=%2d",(int)test_division[which]);
	      lcd70_show_string_mode(0, 70,str, 16, 0);
	 	 }
		 if(KEYCODE==20)// 확인키 
		  exit_flag=1;
		
	   //cal_mode();
      }
	 
	  }while(!exit_flag);
  
	 return test_division[which] ;

  }

  

  
 int Get_capacity(void)
  {
    
	u8 str[50];
	u16 test_capacity[10]={5,10,15,20,25,30,35,40,45,50};// total 10 r
	u8 which; // means 140 kg 
	u8 exit_flag=0;	 
	
	if(v_capacity==5)
	   which=0;
    if(v_capacity==10)
	   which=1;
    if(v_capacity==15)
	   which=2;
    if(v_capacity==20)
	   which=3;

    if(v_capacity==25)
	   which=4;
    if(v_capacity==30)
	   which=5;
    if(v_capacity==35)
	   which=6;
    if(v_capacity==40)
	   which=7;

    if(v_capacity==45)
	   which=8;
    if(v_capacity==50)
	   which=9;
	  



	sprintf(str, "capacity=%2d ton",(int)test_capacity[which]);
	lcd70_show_string_mode(0, 90,str, 16, 0);
	
	
	 do
	  {

	   if (keypush())
      {
       if(KEYCODE==66)// 1번키가  눌리면 
	    {
	   	  which++;
		  if(which>9)
		  which=0;
	      sprintf(str, "capacity=%2d ton",(int)test_capacity[which]);
	      lcd70_show_string_mode(0, 90,str, 16, 0);
	 	 }
		 if(KEYCODE==20)// 확인키 
		  exit_flag=1;
		
	   //cal_mode();
      }
	 
	  }while(!exit_flag);
  
	 return test_capacity[which] ;

  }
 
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
	   delay_ms(200);   // critical !!
	
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
		
	 if(One_Weight_Ok_1&& Two_Weight_Ok_1 && Three_Weight_Ok_1 &&Four_Weight_Ok_1) // 무게 4개가 다알아오면 
	  // if( Three_Weight_Ok)
	   {	
		  
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
	 	 
	 	 

		  zero_sum=(zero_1+zero_2+zero_3+zero_4);
		  
		  if(temp) // span
		  {
		  sprintf(str_1, "full_sum=%6lu ",(long)zero_sum);
		  lcd70_show_string_mode(0, 430,str_1, 16, 0);

		  
		  if((zero_sum-v_zero)>=0)
		  {
		  sprintf(str_1, "span_sum=%6lu ",(long)(zero_sum-v_zero));
		  lcd70_show_string_mode(0, 450,str_1, 16, 0);
		  }
		  }
		  else
		  {
		   sprintf(str_1, "zero_sum=%6lu ",(long)zero_sum);
	       lcd70_show_string_mode(0, 270,str_1, 16, 0);
	 	  }
		  One_Weight_Ok_1=0;
		  Two_Weight_Ok_1=0;
		  Three_Weight_Ok_1=0;
		  Four_Weight_Ok_1=0;
        }

       for(i_1=0;i_1<15;i_1++)
	    {
		 cmd_2[i_1]=0xff;
		 RxBuffer2[i_1]=0xff;
		}

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



unsigned long gxhWeightBackup ;      
 unsigned long gxhFilter;            
 unsigned long gxhMotionBand;        
 unsigned long gxhMotionBand;        
 unsigned long gxhEndDelay;           
 unsigned long gxhMyAddress ;         
 unsigned long gxhBaudRate;           
 unsigned long gxuPulseSetWeigh;      
 unsigned long gxuOutputErrorWeigh;    
 unsigned long gxuOutputHighErrorWeigh;   
 unsigned long gplCompensationWeight;      
*/

unsigned long calcCompensationWeight(long val)
{
	unsigned long ret_val;
	long adc_buf, adc_diff, adc_raw, weight;
	
//USART1_puts("adc 0");	
	Weight_flag = 0;
	rotation = 1;
	while(!Weight_flag) adc_raw=Get_adc_from_module_20160629();
//USART1_puts("adc 1");		
	Weight_flag = 0;
	rotation = 1;
	while(!Weight_flag) adc_raw=Get_adc_from_module_20160629();
//USART1_puts("adc 2");	
	Weight_flag = 0;
	rotation = 1;
	while(!Weight_flag) adc_raw=Get_adc_from_module_20160629();
//USART1_puts("adc 3");	
	Weight_flag = 0;
	rotation = 1;
	while(!Weight_flag) adc_raw=Get_adc_from_module_20160629();
//USART1_puts("adc 4");	
	
	adc_buf = (long)(gnfFactor * adc_raw);
	adc_diff = adc_buf - v_adc_org;
	//diff2 = v_adc1_buf - prev_adc1;	/* for stable */
	weight = ((adc_diff + 10) / v_ei_multiply_factor) * v_division;
	ret_val = val - weight;

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
	curr_num = init_val;

	if((1 <= number) && (number <= 14))
	{
		sprintf( str_1, set_mode_msg[number-1], number, curr_num);
	}
	//lcd70_show_string_mode(x, y,str_1, 16, 0);

    lcd70_show_string_mode_24x24( x,  y, str_1, 16, 0);		   //201605

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
//// ver 1.05b1	because span factor
////				if((tmp_num < min_val) || (max_val < tmp_num))
					if(max_val < tmp_num)
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
					break;
				case KEY_CLEAR:
					changed = 1;
					// ver 1.05b1
					//curr_num = min_val;
					curr_num = 0;
					index = 0;
					break;
				case KEY_ENTER: /* ON/OFF Key */
					//// ver 1.05b1  add if
					if((curr_num < min_val) || (max_val < curr_num))
					{
						retry_message();
					}
					else exit_flag =1;
					break;
				default: /*retry_message();*/  break;
			} /* of switch */

			if(changed)
			{
			/***
				switch(number)
				{
					case 1: sprintf(str_1, "WEIGHT BACK UP       : %6ld",(long)gxhWeightBackup); break;
					case 2: sprintf(str_1, "END DELAY            : %6ld",(long)gxhEndDelay); break;
					case 3: sprintf(str_1, "COMPENSATION WEIGHT  : %6ld",(long)gplCompensationWeight); break;
					case 4: sprintf(str_1, "DATA SENDING TIME_1  : %6ld",(long)gnlSms_Send_Time_First); break;
					case 5: sprintf(str_1, "DATA SENDING TIME_2  : %6ld",(long)gnlSms_Send_Time_Second); break;
					case 6: sprintf(str_1, "DATA SENDING TIME_3  : %6ld",(long)gnlSms_Send_Time_Third); break;
					case 7: sprintf(str_1, "LOADCELL NUMBER      : %6ld",(long)Loadcell_Number); break;
					case 8: sprintf(str_1, "REAL WEIGHT FACTOR   : %6ld",(long)Real_Weight_Factor); break;
					case 9: sprintf(str_1, "DISPLAY SELECTION    : %6ld",(long)gnlDisplay); break;
					//serial no 2016.4.4
					case 10: sprintf(str_1, "Serial No (MYYMMNNN) : M%07ld",(long)gnlSerialNo); break;
					//jangbi id(address) 2016.4.4
					case 11: sprintf(str_1, "ID (ADDRESS : M999)  : M%03ld",(long)gnlMyAddress); break;
					default: break;
				}// of switch
			***/
				if((1 <= number) && (number <= 14))
				{
					sprintf( str_1, set_mode_msg[number-1], number, curr_num);
				}

				//	lcd70_show_string_mode(x, y,str_1, 16, 0);
				lcd70_show_string_mode_24x24( x,  y, str_1, 16, 0);
			}
			//lcd70_show_string_mode(0, 310,str_1, 16, 0);
		} /* of if (keypush())*/
	}while (!exit_flag);

	if (changed)
	{
		switch(number)
		{
		   case 1: gxhWeightBackup = curr_num; break;
		   case 2: gxhEndDelay = curr_num; break;
	       case 3: gplCompensationWeight = calcCompensationWeight(curr_num);
					/*
					gplCompensationWeight=curr_num;
					while(!Weight_flag) temp_weight=Get_adc_from_module();
					while(!Weight_flag) temp_weight=Get_adc_from_module();
					while(!Weight_flag) temp_weight=Get_adc_from_module();
					while(!Weight_flag) temp_weight=Get_adc_from_module();

					v_adc1_buf = (long)(gnfFactor * temp_weight);
					diff1 = v_adc1_buf - v_adc_org;
					//diff2 = v_adc1_buf - prev_adc1;
					gxlGrossWeight = ((diff1 + 10) / v_ei_multiply_factor) * v_division;
					gplCompensationWeight -= gxlGrossWeight;
					*/	  
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
				   break;
			case 4: gnlSms_Send_Time_First=curr_num; break;
			case 5: gnlSms_Send_Time_Second=curr_num; break;
			case 6: gnlSms_Send_Time_Third=curr_num; break;
			case 7: Loadcell_Number=curr_num; break;
			case 8: Real_Weight_Factor=curr_num; break;
			case 9: gnlDisplay=curr_num; break;
			//serial no 2016.4.4	
			case 10: gnlSerialNo = curr_num; break;
			//jangbi id(address) 2016.4.4				
			case 11: gnlMyAddress = curr_num; break;
			case 12: gnlHenhouseNo = curr_num; break;
			case 13: gnlUseLTE = curr_num; break;
			case 14: gxwStatusSendInterval = curr_num; break;

			default: break;
		} // of switch
	}
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
	lcd70_show_string_mode_24x24( x,  y, str_1, 16, 0);		   //201605
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
   	           lcd70_show_string_mode_24x24( x,  y, str_1, 16, 0);		   //201605
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
	       case 3: gplCompensationWeight=curr_num;
  				   while(!Weight_flag) temp_weight=Get_adc_from_module_20160629();
				   while(!Weight_flag) temp_weight=Get_adc_from_module_20160629();
				   while(!Weight_flag) temp_weight=Get_adc_from_module_20160629();
				   while(!Weight_flag) temp_weight=Get_adc_from_module_20160629();
	               v_adc1_buf = (long)(gnfFactor * temp_weight);
			       diff1 = v_adc1_buf - v_adc_org;
			       gxlGrossWeight = ((diff1 + 10) / v_ei_multiply_factor) * v_division;
				   gplCompensationWeight -= gxlGrossWeight;
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
	 lcd70_show_string_mode(x, y,str_1, 16, 0);



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
	          lcd70_show_string_mode(x, y,str_1, 16, 0);
			
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



 /* of value_set */
