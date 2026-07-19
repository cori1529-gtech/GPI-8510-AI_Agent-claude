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

#include "i2c_ee.h"
//#include "def.h"
//#include "eeprom.h"
//#include "var.h"
//#include "vfd.h"
//#ifdef	 __USE_LCD__
//#include "lcd.h"
//#endif

unsigned long	dZeroValue[8];	/* zero calibration data - gain 1 - 64 */
unsigned long	dFullValue[8];	/* full calibration data - gain 1 - 64 */

unsigned  int  v_division;
unsigned  int  v_ei_multiply_factor;
int  v_div ;
char  str_cal[50];
					   
unsigned long  v_zero;
unsigned long v_full;
unsigned long  v_span;		// ver 1.30
unsigned long  v_e_resolution, v_i_resolution;
unsigned long   v_capacity,v_setting_weight;
unsigned long  lc_capacity;
unsigned char   lc_number;
signed long v_adc_org;
unsigned long v_adc1_org;
unsigned char v_decimal_point;
float gnfFactor;

   char str_1[50];
    int i_1;
	long zero_1,zero_2,zero_3,zero_4;
	long zero_sum;
	char One_Weight_Ok_1=0,Two_Weight_Ok_1=0,Three_Weight_Ok_1=0,Four_Weight_Ok_1=0;
    char rotation_1=1;; // ㅇ
   
#define	Y_AXIS_SPACE_CAL	30
void cal_mode()
{
  union { u32 dw; u16 u[2]; u8 c[4];} temp;
 	float ratio;
	//GPIO_SetBits(GPIOC, GPIO_Pin_0);// BUZZER OFF

	float set_ratio;
	v_ei_multiply_factor = 10;

	lcd70_clear_screen(0Xffff);	 //white
	lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*1,"CALIBRATIION MODE", 16, 0);
	Delay_ms(1000);
   
	lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*2,"1 - DIVISION_SETTING", 16, 0);
	lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*4,"    PRESS 1_KEY SELECT DIVISION", 16, 0);
	
	v_division=Get_division();// 한눈의값 구하기 
	
	lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*4,"2 - CAPA_SETTING                  ", 16, 0);
	lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*6,"    PRESS 1_KEY SELECT CAPA", 16, 0);
	v_capacity=Get_capacity(); // 최대무게값 구하기 
	v_capacity=v_capacity*1000;

	v_e_resolution = (v_capacity) / v_division; //외부분해도 
	
	sprintf(str_1, "3 - External_Resolution = %6lu ",(long)v_e_resolution);
	lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*6, str_1, 16, 0);

	v_i_resolution = v_e_resolution *  v_ei_multiply_factor; //내부분해도 
	sprintf(str_1, "    Internal_Resolution = %6lu ",(long)v_i_resolution);
	lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*7, str_1, 16, 0);
     //RS422ON;

	v_zero=Get_adc_value(0);

	sprintf(str_1, "    Zero_value=%6lu ",(long)v_zero);
//	lcd70_show_string_mode(0, 290,str_1, 16, 0);
	lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*8, str_1, 16, 0);
	Delay_ms(1000);

	v_setting_weight = v_capacity; 
	
	v_setting_weight=dwValueSet();
	sprintf(str_1, "4 - Setting_Weight = %6lu                  ",(long)v_setting_weight);
	//lcd70_show_string_mode(0, 310,str_1, 16, 0);
	lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*9, str_1, 16, 0);
	
	v_full=Get_adc_value(1);
	v_span=v_full-v_zero;
	sprintf(str_1, "5 - v_span = %6lu                          ",(long)v_span);
//	lcd70_show_string_mode(0, 370,str_1, 16, 0);
	lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*11, str_1, 16, 0);


	 ratio = (float)((float)v_capacity/(float)v_setting_weight);
	 gnfFactor = (float)(((float)v_i_resolution)/((float)v_span*ratio));
	 v_adc_org = (long)(((float)v_zero)*gnfFactor);

/*
	sprintf(str_1, "ratio=%3.5f ",(float)ratio);
	lcd70_show_string_mode(300, 10,str_1, 16, 0);
							
	sprintf(str_1, "factor=%3.8f ",(float)gnfFactor);
	lcd70_show_string_mode(300, 30,str_1, 16, 0);
	
	sprintf(str_1, "v_adc_org=%7lu ",(long)v_adc_org);
	lcd70_show_string_mode(300, 50,str_1, 16, 0);  //
*/
	sprintf(str_1, "ratio=%3.5f, factor=%3.8f, v_adc_org=%7lu ",(float)ratio, (float)gnfFactor, (long)v_adc_org);
	lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*12, str_1, 16, 0);


	little_span_adjust();
  
  
  
/*****************************************************************  
	lcd70_clear_screen(0Xffff);	 //white
	lcd70_show_string_mode(0, 10," CALIBRATIION MODE ", 16, 0);
  
	Delay_ms(1000);
   
   	lcd70_show_string_mode(0, 30,"DIVISION_SETTING ", 16, 0);
	lcd70_show_string_mode(0, 50,"PRESS 1_KEY SELECT DIVISION", 16, 0);
	
	v_division=Get_division();// 한눈의값 구하기 
	
	lcd70_show_string_mode(0, 110,"CAPA_SETTING ", 16, 0);
	lcd70_show_string_mode(0, 130,"PRESS 1_KEY SELECT CAPA", 16, 0);
	v_capacity=Get_capacity(); // 최대무게값 구하기 
	v_capacity=v_capacity*1000;
    v_e_resolution = (v_capacity) / v_division; //외부분해도 

	 sprintf(str_1, "external_resolution=%6lu ",(long)v_e_resolution);
	 lcd70_show_string_mode(0, 150,str_1, 16, 0);


	v_i_resolution = v_e_resolution *  v_ei_multiply_factor; //내부분해도 
	sprintf(str_1, "internal_resolution=%6lu ",(long)v_i_resolution);
	lcd70_show_string_mode(0, 170,str_1, 16, 0);
     //RS422ON;


	v_zero=Get_adc_value(0);


	sprintf(str_1, "zero_value=%6lu ",(long)v_zero);
	lcd70_show_string_mode(0, 290,str_1, 16, 0);
	


	Delay_ms(1000);

	v_setting_weight = v_capacity; 
	
	v_setting_weight=dwValueSet();
	sprintf(str_1, "setting_weight=%6lu ",(long)v_setting_weight);
	lcd70_show_string_mode(0, 330,str_1, 16, 0);
	
	v_full=Get_adc_value(1);
	v_span=v_full-v_zero;
	sprintf(str_1, "v_span=%6lu ",(long)v_span);
	lcd70_show_string_mode(0, 370,str_1, 16, 0);
	



	 ratio = (float)((float)v_capacity/(float)v_setting_weight);
	 gnfFactor = (float)(((float)v_i_resolution)/((float)v_span*ratio));
	 v_adc_org = (long)(((float)v_zero)*gnfFactor);


	sprintf(str_1, "ratio=%3.5f ",(float)ratio);
	lcd70_show_string_mode(300, 10,str_1, 16, 0);
							
	sprintf(str_1, "factor=%3.8f ",(float)gnfFactor);
	lcd70_show_string_mode(300, 30,str_1, 16, 0);
	
	sprintf(str_1, "v_adc_org=%7lu ",(long)v_adc_org);
	lcd70_show_string_mode(300, 50,str_1, 16, 0);  //

	little_span_adjust();
*************************************************************/
	//write_factors();  /*  adc_normal_mode() */
	//read_factors();

/* 2016.4.20
     Initiaize_eeprom();
  FSMC_DISABLE;
  
	Delay_ms(1000);

	temp.dw=v_zero;
	I2C_EE_BufferWrite(temp.c, ADR_v_zero, 4);
	I2C_EE_BufferRead( temp.c, ADR_v_zero, 4);

	temp.dw = v_span;
	I2C_EE_BufferWrite(temp.c, ADR_v_span, 4);
	I2C_EE_BufferRead( temp.c, ADR_v_span, 4);
	
	temp.dw = v_capacity;
    I2C_EE_BufferWrite(temp.c, ADR_v_capacity, 4);
	I2C_EE_BufferRead( temp.c, ADR_v_capacity, 4);

	temp.dw = v_division;
    I2C_EE_BufferWrite(temp.c, ADR_v_division, 4);
	I2C_EE_BufferRead( temp.c, ADR_v_division, 4);


    temp.dw = (long)(gnfFactor * (float)power(10, 9));
	I2C_EE_BufferWrite(temp.c, ADR_gnfFactor, 4);
	I2C_EE_BufferRead( temp.c, ADR_gnfFactor, 4);
 
	FSMC_ENABLE;
*/

	v_zero = EE_Write_val(ADR_v_zero, v_zero, 4);
	v_span = EE_Write_val(ADR_v_span, v_span, 4);
	v_capacity = EE_Write_val(ADR_v_capacity, v_capacity, 4);
	v_division = EE_Write_val(ADR_v_division, v_division, 4);
	temp.dw = (long)(gnfFactor * (float)power(10, 9));
	temp.dw = EE_Write_val(ADR_gnfFactor, temp.dw, 4);

//	lcd70_show_string_mode(300, 230,
//	lcd70_show_string_mode(300, 250,
	
	lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*14, "Calibration Finished!!", 16, 0);
	lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*15, "Press Enter key !!", 16, 0);
	

	while(1)
	{
		if(keypush())
		{
			//return 
			if(KEYCODE==20)// 확인키 
			{
				//write_factors();
				break;
			}
		}	
	};
} /* of cal_mode */

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


  
 int Get_division(void)//  한눈의값 구하기  
  {
    
	u8 str[50];
	u16 test_division[6]={1,2,5,10,20,50};
	u8 which; // means 140 kg 
	u8 exit_flag=0;	 
	

//		sprintf(str, "division=%2d",(int)test_division[which]);
//	lcd70_show_string_mode(200, 70,str, 16, 0);

	if(v_division==1) which=0;
    if(v_division==2) which=1;
    if(v_division==5) which=2;
    if(v_division==10) which=3;
    if(v_division==20) which=4;
    if(v_division==50) which=5;

	sprintf(str, "division=%2d",(int)test_division[which]);
//	lcd70_show_string_mode(0, 70,str, 16, 0);
	lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*3,str, 16, 0);
	
	
	 do
	  {

	   if (keypush())
      {
       if(KEYCODE==66)// 1번키가  눌리면 
	    {
	   	  which++;
		  if(which>5)
		  which=0;
	      sprintf(str, "division=%2d",(int)test_division[which]);
	      lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*3,str, 16, 0);
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
	lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*5, str, 16, 0);
	
	
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
	      lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*5, str, 16, 0);
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
				  sprintf(str_1, "SPAN SUM=%6lu, 1=%6lu, 2=%6lu, 3=%6lu, 4=%6lu", (long)(zero_sum-v_zero), (long)zero_1, (long)zero_2, (long)zero_3, (long)zero_4);
				  //lcd70_show_string_mode(0, 450,str_1, 16, 0);
			  }
			  lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*11, str_1, 16, 0);
		  }
		  else
		  {
			sprintf(str_1, "ZERO SUM=%6lu, 1=%6lu, 2=%6lu, 3=%6lu, 4=%6lu", (long)zero_sum, (long)zero_1, (long)zero_2, (long)zero_3, (long)zero_4);
			//lcd70_show_string_mode(0, 250, str_1, 16, 0);
			lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*9, str_1, 16, 0);
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




 
 //long Get_adc_value(char status)
 void little_span_adjust()
 {
   char temp;
   long raw_data ,v_adc1_buf , v_i_value;
   long lWeight;
  // temp=status;
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
		  
		 
		  sprintf(str_1, "ADC_1=%6lu ",(long)zero_1);
	      lcd70_show_string_mode(500, 70,str_1, 16, 0);
		 
		  sprintf(str_1, "ADC_2=%6lu ",(long)zero_2);
	      lcd70_show_string_mode(500, 90,str_1, 16, 0);
	 	 
		  sprintf(str_1, "ADC_3=%6lu ",(long)zero_3);
	      lcd70_show_string_mode(500, 110,str_1, 16, 0);
	 	 
		  sprintf(str_1, "ADC_4=%6lu ",(long)zero_4);
	      lcd70_show_string_mode(500, 130,str_1, 16, 0);
	 	 
		  raw_data=(zero_1+zero_2+zero_3+zero_4);

		  sprintf(str_1, "TOTAL_ADC=%6lu ",(long)raw_data);
		  lcd70_show_string_mode(500, 150,str_1, 16, 0);


		  v_adc1_buf = (long)(gnfFactor * raw_data);
		  v_i_value  =  v_adc1_buf - v_adc_org;

		  sprintf(str_1, "AD_COUNT=%6lu ",(long)v_i_value);
		  lcd70_show_string_mode(500, 170,str_1, 16, 0); //1115

		  lWeight= ((v_i_value)/v_ei_multiply_factor)*v_division;
		  
		  sprintf(str_1, "Weight=%6lu  kg",(long)lWeight);
		  lcd70_show_string_mode(500, 190,str_1, 16, 0); //1115




		  /*
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
		  */
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

	    //write_factors();
	 	  break;
       }

	 }
	 	
	};
  }





unsigned long dwValueSet(void) //1114
{
	char  exit_flag =0;
	char  changed = 0;
	char  index = 0;		/* only HIDE mode, instead number to '-' */
	unsigned long  curr_num = 0;
	char v_key;
	sprintf(str_1, "4 - Setting_Weight = %6lu ",(long)v_setting_weight);
	//lcd70_show_string_mode(0, 310,str_1, 16, 0);
	lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*9, str_1, 16, 0);

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
			lcd70_show_string_mode_24x24(0, Y_AXIS_SPACE_CAL*9, str_1, 16, 0);
			
		} /* of if (keypush())*/
	}while (!exit_flag);
	if (changed) return curr_num;
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