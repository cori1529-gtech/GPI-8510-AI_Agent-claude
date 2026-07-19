/*
	adc.c
*/

#include <stdio.h>

#include "stm32f10x.h"
#include "hw_config.h"

#include "global.h"

#include "adc.h"
#include "i2c_ee.h"
#include "var.h"
#include "usart.h"
#include "board_gpi8510.h"

extern long gnlDisplay;


//#define   MAX_FILTER   20

//u8 gxhFilter;
unsigned long  ad_buf[200];
unsigned long  ad_buf_1[200];

unsigned char test[50];
u8 i;

	 unsigned char which_adc=0;  //  1: CS5555 사용, 	 
   
#define  DELAY_TIMER1    80	// original 100 10	//130 // VERY VERY CRITICAL !!!!!!!
 
 
 /***************CS5555*******************/

#define  ADC_DELAY 1
#define  ADC_SLCK_HIGH_1                     GPIO_SetBits(LC1_SCLK_PORT, LC1_SCLK_PIN)
#define  ADC_SLCK_LOW_1                     GPIO_ResetBits(LC1_SCLK_PORT, LC1_SCLK_PIN)

#define  ADC_SLCK_HIGH_2                     GPIO_SetBits(LC2_SCLK_PORT, LC2_SCLK_PIN)
#define  ADC_SLCK_LOW_2                     GPIO_ResetBits(LC2_SCLK_PORT, LC2_SCLK_PIN)


void adc_delay(int time)
{
	int	 duration;

	for(duration=0; duration<time; duration++) {}
}


void INIT_CS5555(void)            //CS5555れ쯮ㅖ
{
	
	adc_delay(100);
  ADC_SLCK_LOW_1;
	ADC_SLCK_LOW_2;
	Delay_ms(1000);


}

void CS5555_CLK_1(void) 
{ ADC_SLCK_HIGH_1;  adc_delay(ADC_DELAY); // Delay_ms(1); //  adc_delay(ADC_DELAY); 
  ADC_SLCK_LOW_1;   adc_delay(ADC_DELAY); //  Delay_ms(1); // adc_delay(ADC_DELAY);
}

void CS5555_CLK_2(void) 
{ ADC_SLCK_HIGH_2;  adc_delay(ADC_DELAY); // Delay_ms(1); //  adc_delay(ADC_DELAY); 
  ADC_SLCK_LOW_2;   adc_delay(ADC_DELAY); //  Delay_ms(1); // adc_delay(ADC_DELAY);
}


unsigned long Read_AD_Value_CS5555_1(void) 
{ 
 u32 data_ready=0;
 u32 ADvalue=0;
 char AD_NEG=0;
 char ADi=0;
	
//	USART1_puts("step1");
	while(!GPIO_ReadInputDataBit(LC1_DOUT_PORT, LC1_DOUT_PIN));	
 // USART1_puts("step2");
  ADC_SLCK_LOW_1; ADvalue=0; AD_NEG=0;
	while(GPIO_ReadInputDataBit(LC1_DOUT_PORT, LC1_DOUT_PIN));
  //	USART1_puts("step3");

 for(ADi=0;ADi<24;ADi++) 
    {
		 ADvalue=ADvalue<<1; CS5555_CLK_1(); //
     ////ADC_SLCK_HIGH;  adc_delay(ADC_DELAY); // 300 nsec
		 if(GPIO_ReadInputDataBit(LC1_DOUT_PORT, LC1_DOUT_PIN) == 0x01 &&ADi==0 ){AD_NEG=1;}
     if(GPIO_ReadInputDataBit(LC1_DOUT_PORT, LC1_DOUT_PIN) ==0X01)
			      {
		       //adc_delay(ADC_DELAY);
		      //   if(GPIO_ReadInputDataBit(LC3_2_SCLK_PORT, LC3_2_SCLK_PIN3) ==0X01)
			       ADvalue++;
						
						}
    //// ADC_SLCK_LOW;adc_delay(ADC_DELAY);
		 
		} // of for 
 	
		CS5555_CLK_1();//  force to DOUT high
 	

return ADvalue;

} // of function                       //쪿CS5555탊캿뎡?셸퍇?ㅈ 



unsigned long read_filtered_adc_CS5555_1(void)
	{
	unsigned long lxdAdcValue;
    float NewValue,Difference,Average;
	int i;
	gxhFilter=5;
	lxdAdcValue = Read_AD_Value_CS5555_1();


  
	if (gxhFilter)
	{
	 for (i = 100 - 1; i > 0; i--)  ad_buf[i] = ad_buf[i-1];
    // for (i = 10 - 1; i > 0; i--)  ad_buf[i] = ad_buf[i-1];
	 	 
		ad_buf[0] = lxdAdcValue;
	  for (i = gxhFilter - 1; i > 0; i--)
		{
		  lxdAdcValue += ad_buf[i];
		}
	   lxdAdcValue = lxdAdcValue / gxhFilter;
	}
	else  gxhFilter = 1;

	//return lxdAdcValue/150; // span value = 31000 
    //return lxdAdcValue/65;  // span value = 73920
	
	//return lxdAdcValue/31;  // span about 15,000

	//return lxdAdcValue/15;	//2016.2.26 span 1/30,000  2mv/V loadcell
	//return (lxdAdcValue/50);//2016.08.03 인장압축용
 //	return (lxdAdcValue/10);//2016.08.03 인장압축용

	return (lxdAdcValue/25); 
}

unsigned long Read_AD_Value_CS5555_2(void) 
{ 
 u32 data_ready=0;
 u32 ADvalue=0;
 char AD_NEG=0;
 char ADi=0;
	//	USART1_puts("step11");
	while(!GPIO_ReadInputDataBit(LC2_DOUT_PORT, LC2_DOUT_PIN));	
 // USART1_puts("step22");
	ADC_SLCK_LOW_1; ADvalue=0; AD_NEG=0;
 while(GPIO_ReadInputDataBit(LC2_DOUT_PORT, LC2_DOUT_PIN));
 // USART1_puts("step33");
 for(ADi=0;ADi<24;ADi++) 
    {
		 ADvalue=ADvalue<<1; CS5555_CLK_2(); //
     ////ADC_SLCK_HIGH;  adc_delay(ADC_DELAY); // 300 nsec
		 if(GPIO_ReadInputDataBit(LC2_DOUT_PORT, LC2_DOUT_PIN) == 0x01 &&ADi==0 ){AD_NEG=1;}
     if(GPIO_ReadInputDataBit(LC2_DOUT_PORT, LC2_DOUT_PIN) ==0X01)
			      {
		       //adc_delay(ADC_DELAY);
		      //   if(GPIO_ReadInputDataBit(LC3_2_SCLK_PORT, LC3_2_SCLK_PIN3) ==0X01)
			       ADvalue++;
						
						}
    //// ADC_SLCK_LOW;adc_delay(ADC_DELAY);
		 
		} // of for 
 	
		CS5555_CLK_2();//  force to DOUT high
 	

return ADvalue;

} // of function                       //쪿CS5555탊캿뎡?셸퍇?ㅈ 



unsigned long read_filtered_adc_CS5555_2(void)
	{
	unsigned long lxdAdcValue;
    float NewValue,Difference,Average;
	int i;
	gxhFilter=5;
	lxdAdcValue = Read_AD_Value_CS5555_2();


  
	if (gxhFilter)
	{
	 for (i = 100 - 1; i > 0; i--)  ad_buf_1[i] = ad_buf_1[i-1];
    // for (i = 10 - 1; i > 0; i--)  ad_buf[i] = ad_buf[i-1];
	 	 
		ad_buf_1[0] = lxdAdcValue;
	  for (i = gxhFilter - 1; i > 0; i--)
		{
		  lxdAdcValue += ad_buf_1[i];
		}
	   lxdAdcValue = lxdAdcValue / gxhFilter;
	}
	else  gxhFilter = 1;

	//return lxdAdcValue/150; // span value = 31000 
    //return lxdAdcValue/65;  // span value = 73920
	
	//return lxdAdcValue/31;  // span about 15,000

	//return lxdAdcValue/15;	//2016.2.26 span 1/30,000  2mv/V loadcell
	//return (lxdAdcValue/50);//2016.08.03 인장압축용
	return (lxdAdcValue/25);//2016.08.03 인장압축용
}
/***************CS5555*******************/
 
 

  void adc_initial(void)   /* self-Calibration */
{
    unsigned int b;
//	gxhFilter=30;
//	gxhFilter=45;	//2012년 9월 25일 
//	gxhFilter=35;	//2012년 11월 15일 
//    gxhFilter=9;	//2014년 5월 30일 



//	gxhFilter=50;	//2013년 5월 28일 지그용  

    for (b=0; b < MAX_FILTER; b++) ad_buf[b] = 0;
    for (b=0; b < MAX_FILTER; b++) ad_buf_1[b] = 0;

// IC15 ADS1251  Initialization 
// PC1 : CLK      PC2: DOUT      PC3: SCLK     

// IC18 ADS1251  Initialization 
// PF10 : CLK      PF9: DOUT      PF8: SCLK     

 
   GPIO_ResetBits(LC3_SCLK_PORT, LC3_SCLK_PIN);  // SCLK LOW  , NO SYNCMODE
   
   GPIO_ResetBits(LC3_2_SCLK_PORT, LC3_2_SCLK_PIN);  // SCLK LOW  , NO SYNCMODE
     


} // of adc_initial() 
/*--------------------------------------------------------------------------
;       ADS1251 BASIC INPUT/OUTPUT SUBROUTINE (AD7710 BIOS)
;---------------------------------------------------------------------------
 
 GSE-1600
 CLK (PB_7)
 Dout , /DRDY(PE_4)
 SCLK (PE3)

  E7  E6  E5    E4            E3    E2   E1   E0      
			   Dout, /DRDY	  SCLK 
*/

unsigned long RDDATA_ads1251(void) // 어떤 adc 컨버터가 사용되었는지 확인하는 
{
   	char ch;
    unsigned long ad_value = 0;
	  unsigned long expire_timer=0;
	
	which_adc=0;

	GPIO_ResetBits(LC3_SCLK_PORT, LC3_SCLK_PIN);  // SCLK LOW  , NO SYNCMODE
	// GLCD_string(0,6, "11111");

   	while(!GPIO_ReadInputDataBit(LC3_DOUT_PORT, LC3_DOUT_PIN))
		{
      expire_timer++;
		 if(expire_timer >100000)
		 {
			 which_adc=1; // ads 1251에서 실패했으니까  cs5555 라고 판다.ㄴ
			//  GLCD_string(4,6, "22222"); Delay_ms(2000);
			   goto FINISH_ADC;
			 break;
		 } 

    };			
		//GLCD_string(6,6, "33333");

  if(which_adc==1) // 현재 설치된 adc 가 cs5555 이면		
	  goto FINISH_ADC;
	
	
	 expire_timer=0;
	
	//while( GPIO_ReadInputDataBit(LC3_DOUT_PORT, LC3_DOUT_PIN));		//24 * clk
  while(GPIO_ReadInputDataBit(LC3_DOUT_PORT, LC3_DOUT_PIN))
		{
      expire_timer++;
		 if(expire_timer >100000)
		 {
			 // GLCD_string(8,6, "4444"); Delay_ms(2000);
			 which_adc=1; // ads 1251에서 실패했으니까  cs5555 라고 판다.ㄴ
			 break;
		 } 

    };			
  if(which_adc==1) // 현재 설치된 adc 가 cs5555 이면		
	  goto FINISH_ADC;
	
	
	  //GLCD_string(10,6, "55555"); Delay_ms(2000);
	
	

  //	while(!GPIO_ReadInputDataBit(LC3_DOUT_PORT, LC3_DOUT_PIN));		//6 * clk
  // 	while( GPIO_ReadInputDataBit(LC3_DOUT_PORT, LC3_DOUT_PIN));		//6 * clk
    
   	for (ch = 0; ch < 24; ch++)
   	{
      	GPIO_SetBits(LC3_SCLK_PORT, LC3_SCLK_PIN);  // SCLK HIGH
        Delay_us(DELAY_TIMER1);
			
		ad_value <<= 1;
		ad_value += (unsigned long)(GPIO_ReadInputDataBit(LC3_DOUT_PORT, LC3_DOUT_PIN));			//						222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222
		Delay_us(10);
		GPIO_ResetBits(LC3_SCLK_PORT, LC3_SCLK_PIN);  // SCLK LOW  , NO SYNCMODE
    	Delay_us(DELAY_TIMER1);
  	}
		
		FINISH_ADC:
	return 	ad_value;
}

unsigned long RDDATA(void)
{
 //  #define  DELAY_TIMER1    500	// original 100 10	//130 // VERY VERY CRITICAL !!!!!!!
   
 //   #define  DELAY_TIMER1    500	// original 100 10	//130 // VERY VERY CRITICAL !!!!!!!
 
   	char ch;
    unsigned long ad_value = 0;

	GPIO_ResetBits(LC3_SCLK_PORT, LC3_SCLK_PIN);  // SCLK LOW  , NO SYNCMODE
 
   	while(!GPIO_ReadInputDataBit(LC3_DOUT_PORT, LC3_DOUT_PIN));		
   	while( GPIO_ReadInputDataBit(LC3_DOUT_PORT, LC3_DOUT_PIN));		//24 * clk
   	while(!GPIO_ReadInputDataBit(LC3_DOUT_PORT, LC3_DOUT_PIN));		//6 * clk
   	while( GPIO_ReadInputDataBit(LC3_DOUT_PORT, LC3_DOUT_PIN));		//6 * clk
   
	
   	for (ch = 0; ch < 24; ch++)
   	{
      	GPIO_SetBits(LC3_SCLK_PORT, LC3_SCLK_PIN);  // SCLK HIGH
			Delay_us(150);                                   //very very critical  20170825
    //  Delay_us(DELAY_TIMER1);
			
		ad_value <<= 1;
		ad_value += (unsigned long)(GPIO_ReadInputDataBit(LC3_DOUT_PORT, LC3_DOUT_PIN));			//						222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222
		//Delay_us(10);
		GPIO_ResetBits(LC3_SCLK_PORT, LC3_SCLK_PIN);  // SCLK LOW  , NO SYNCMODE
    Delay_us(50);                                    // very very critical  20170825
  	}

/*	
     sprintf(test, "La=%8ld",(int)ad_value);
   	 for(i=0;i<11;i++)
	   {
      	USART_SendData(USART1, test[i]);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
       }
   	   	USART_SendData(USART1, 0x0d);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
        
		USART_SendData(USART1, 0x0a);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
   	
  */

	    
	return 	ad_value;
}

unsigned long RDDATA_1(void)
{
   
   // #define  DELAY_TIMER1    500	//10	//130 // VERY VERY CRITICAL !!!!!!!
 
   	char ch;
    unsigned long ad_value_1 = 0;

	GPIO_ResetBits(LC3_2_SCLK_PORT, LC3_2_SCLK_PIN);  // SCLK LOW  , NO SYNCMODE

   	while(!GPIO_ReadInputDataBit(LC3_2_DOUT_PORT, LC3_2_DOUT_PIN));		
   	while( GPIO_ReadInputDataBit(LC3_2_DOUT_PORT, LC3_2_DOUT_PIN));		//24 * clk		   f
   	while(!GPIO_ReadInputDataBit(LC3_2_DOUT_PORT, LC3_2_DOUT_PIN));		//6 * clk
   	while( GPIO_ReadInputDataBit(LC3_2_DOUT_PORT, LC3_2_DOUT_PIN));		//6 * clk
    
   	for (ch = 0; ch < 24; ch++)
   	{
      	GPIO_SetBits(LC3_2_SCLK_PORT, LC3_2_SCLK_PIN);  // SCLK HIGH
        Delay_us(150);
		   ad_value_1 <<= 1;
		   ad_value_1 += (unsigned long)(GPIO_ReadInputDataBit(LC3_2_DOUT_PORT, LC3_2_DOUT_PIN));
		  // Delay_us(10);
		   GPIO_ResetBits(LC3_2_SCLK_PORT, LC3_2_SCLK_PIN);  // SCLK LOW  , NO SYNCMODE
    	Delay_us(50);
  	}
   //	 return ((long)((float) ad_value_1) * R_factor);
   	/*
     sprintf(test, "Ra=%8ld",(int)ad_value_1);
   	 for(i=0;i<11;i++)
	   {
      	USART_SendData(USART1, test[i]);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
       }
   	   	USART_SendData(USART1, 0x0d);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
        
		USART_SendData(USART1, 0x0a);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
   	
	 */

  	  return ad_value_1;
  
}



unsigned long adc_value(void)
{

	//return RDDATA() >> 4;
    // return RDDATA() >> 3;
	//return RDDATA() >> 5;
	return RDDATA();


}

unsigned long adc_value_1(void)
{

	//return RDDATA_1() >> 4;
	return RDDATA_1();
	////return RDDATA_1() >> 5;
}


/*
unsigned long read_filtered_adc(void)
{
	unsigned long lxdAdcValue;
	int i;

	lxdAdcValue = adc_value();

	if (gxhFilter)
	{
		
		for (i = MAX_FILTER - 1; i > 0; i--)  ad_buf[i] = ad_buf[i-1];
		ad_buf[0] = lxdAdcValue;
		for (i = gxhFilter - 1; i > 0; i--)
		{
		  lxdAdcValue += ad_buf[i];
		}
		lxdAdcValue = lxdAdcValue / gxhFilter;
	}
	else  gxhFilter = 1;

	return lxdAdcValue;
}
  */
unsigned long read_filtered_adc(void)
{
 
	unsigned long lxdAdcValue;
	int i;
	u32 adc_sum;
	u32 adc_average;
	adc_sum=0;
	lxdAdcValue = adc_value();
	
	
	if (gxhFilter)
	{
	  for (i = MAX_FILTER - 1; i > 0; i--)  ad_buf[i] = ad_buf[i-1];
	  ad_buf[0] = lxdAdcValue;
	  for (i = gxhFilter - 1; i > 0; i--)
		{
		  lxdAdcValue += ad_buf[i];
		}
	   lxdAdcValue = lxdAdcValue / gxhFilter;
	}
	
	else  gxhFilter = 1;

	/*
      sprintf(test, "Sa=%5ld",(int)(lxdAdcValue/46));
   	  for(i=0;i<8;i++)
	   {
      	USART_SendData(USART1, test[i]);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
       }
   	   	USART_SendData(USART1, 0x0d);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
        
		USART_SendData(USART1, 0x0a);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
	  */


   return lxdAdcValue/20; // old 23
	 
 
}



unsigned long read_filtered_adc_1(void)
{
  	unsigned long lxdAdcValue;
	int i;
	u32 adc_sum;
	u32 adc_average;
	adc_sum=0;
	lxdAdcValue = adc_value_1();
	
	
	if (gxhFilter)
	{
	  for (i = MAX_FILTER - 1; i > 0; i--)  ad_buf_1[i] = ad_buf_1[i-1];
	  ad_buf_1[0] = lxdAdcValue;
	  for (i = gxhFilter - 1; i > 0; i--)
		{
		  lxdAdcValue += ad_buf_1[i];
		}
	   lxdAdcValue = lxdAdcValue / gxhFilter;
	}
	
	else  gxhFilter = 1;

  


   return lxdAdcValue/20; // old 23

  
  /*
	unsigned long lxdAdcValue;
	int i;

	lxdAdcValue = adc_value_1();

	if (gxhFilter)
	{
		
		for (i = MAX_FILTER - 1; i > 0; i--)  ad_buf_1[i] = ad_buf_1[i-1];
		ad_buf_1[0] = lxdAdcValue;
		for (i = gxhFilter - 1; i > 0; i--)
		{
		  lxdAdcValue += ad_buf_1[i];
		}
		lxdAdcValue = lxdAdcValue / gxhFilter;
	}
	else  gxhFilter = 1;

 	return lxdAdcValue;
  */
}











long Get_adc_from_module_20160629_notused(void) //1116
{
	int	iz;
	int	 adc_complete = 0;	//ad 를 전부 스캔했는지 ?
	long weight;
	long Display_Weight = -1;
	char sT0[50];	//0116
	
	if(rotation == 1)
	{
		Weight_flag	= 0;
/* 2016.10.12		
		One_Weight = 99999;
		Two_Weight = 99999;
		Three_Weight = 99999;
		Four_Weight = 99999;
		Five_Weight = 99999;
		Six_Weight = 99999;
*/		
		One_Weight_Ok=0;
		Two_Weight_Ok=0;
		Three_Weight_Ok=0;
		Four_Weight_Ok=0;	
		Five_Weight_Ok=0;	
		Six_Weight_Ok=0;			
	}
	
	// 422 송신 프로토콜 	
	sprintf(sT0, "@-%cR", rotation+0x30); 
	USART2_puts(sT0);
//USART1_print(sT0);
  
//	rotation++;
//	if(rotation>4) rotation=1;
	
	for(iz=0; iz<100; iz++) //typical response time 45ms~47ms
	{  	
		//delay_ms(100);   // critical !! module로부터 응답을 기다리는 시간, 굉장히 중요함  
		delay_ms(1);
		if(Usart2_Interrupt_flag)
		{
			delay_ms(1);   // critical !! module로부터 응답을 기다리는 시간, 굉장히 중요함  
			break;
		}
	}
//sprintf(sT0, " iz=%d.", iz); 
//USART1_print(sT0);

	
   	// 1,2,3,4 순서대로 ad 모듈을 스캐닝한다 
//==============================
	if(Usart2_Interrupt_flag) // a/d 모듈이  
	{
	    Usart2_Interrupt_flag = 0;
		RxCounter2 = 0;
		
		//junction power off delay counter
        Adc_Counter = OPTION_POWER_COUNTER; // 20 을 기본으로 무조건 넣는다. 
		
		weight = 0;
///		weight = (cmd_2[2]-0x30)*100000+(cmd_2[3]-0x30)*10000+(cmd_2[4]-0x30)*1000+(cmd_2[5]-0x30)*100+(cmd_2[6]-0x30)*10+(cmd_2[7]-0x30);
		for(iz=2; iz<=7; iz++)
		{
			if(('0'<=cmd_2[iz] && cmd_2[iz]<='9') || ('A'<=cmd_2[iz] && cmd_2[iz]<='F'))
			{	
				weight = (weight * 10) + cmd_2[iz]-'0';
				continue;
			}
			else break;
		}
		
		if(iz < 8) weight = -1;
		else
		{
			//address check
			if( !strncmp(cmd_2, "-1", 2)) 
			{
				One_Weight = weight;
				One_Weight_Ok=1;
				One_Weight_Error_Counter=0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
			}
			else if( !strncmp(cmd_2, "-2", 2)) 
			{
				Two_Weight = weight;
				Two_Weight_Ok=1;
				Two_Weight_Error_Counter=0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
			}
			else if( !strncmp(cmd_2, "-3", 2)) 
			{
				Three_Weight = weight;
				Three_Weight_Ok=1;
				Three_Weight_Error_Counter=0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
			}
			else if( !strncmp(cmd_2, "-4", 2)) 
			{
				Four_Weight = weight;
				Four_Weight_Ok=1;
				Four_Weight_Error_Counter=0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
			}
		}
			
//sprintf(sT0, " w=%ld %s", weight, cmd_2);
//lcd70_show_string_mode(400, 30+(cmd_2[1]-'0')*30, (const unsigned char *)sT0, 20, 0);
//USART1_print(sT0);
			 
			Initial_cmd_2_RxBuffer2();

	} //if(Usart2_Interrupt_flag) // a/d 모듈이	
	else  // return이 없으면 
	{
		if(Adc_Counter) Adc_Counter--;
	}

//	if(rotation==1) // module전부를 한cycle scaning 이 끝나면 	 
	if(rotation == 4) // module전부를 한cycle scaning 이 끝나면 	 
	{
//sprintf(sT0, "%d, %d, %d, %d.", One_Weight_Ok, Two_Weight_Ok, Three_Weight_Ok, Four_Weight_Ok);
//lcd70_show_string_mode(400, 30+(5*30), (const unsigned char *)sT0, 20, 0);
		if( 1 || One_Weight_Ok && Two_Weight_Ok && Three_Weight_Ok && Four_Weight_Ok) // 무게 4개가 다알아오면 
	   	{
	  		Display_Weight=(One_Weight+Two_Weight+Three_Weight+Four_Weight);
/*			
        	One_Weight_Ok=0;
		  	Two_Weight_Ok=0;
		  	Three_Weight_Ok=0;
		  	Four_Weight_Ok=0;	
*/			
///	  		Weight_flag=1;
			adc_complete = 1;
	   	} // 무게가 4개 다날아오면 
	 	///////////////////// 통신에러가 발생시 ///////////////////////
	  	else if(!One_Weight_Ok && Two_Weight_Ok && Three_Weight_Ok && Four_Weight_Ok) // 1번 로드셀이 통신두절이면  
		{
			// 에러갯수 조건 집어넣어야하고
			// cal시 비율 조건 계산해야하고 
			Display_Weight=(Two_Weight*2+Three_Weight+Four_Weight);
			
			One_Weight_Error_Counter++;
			if(One_Weight_Error_Counter>=2)
			{
				One_Weight_Error_Counter=0;
			}
/*			
        	One_Weight_Ok=0;
		  	Two_Weight_Ok=0;
		  	Three_Weight_Ok=0;
		  	Four_Weight_Ok=0;	
*/			
///			Weight_flag=1;
			adc_complete = 1;
		}
		// 2번이 통신 두절되면 
		else if(One_Weight_Ok && !Two_Weight_Ok && Three_Weight_Ok && Four_Weight_Ok) // 2번 로드셀이 통신두절이면  
		{
			// 에러갯수 조건 집어넣어야하고	
			// cal시 비율 조건 계산해야하고 
			Display_Weight=(One_Weight*2+Three_Weight+Four_Weight);

			Two_Weight_Error_Counter++;
			if(Two_Weight_Error_Counter>=2)
			{
				Two_Weight_Error_Counter=0;
			}
/*			
        	One_Weight_Ok=0;
		  	Two_Weight_Ok=0;
		  	Three_Weight_Ok=0;
		  	Four_Weight_Ok=0;	
*/			
///			Weight_flag=1;
			adc_complete = 1;
		} // 2번이 통신 두절되면  
		// 3번 로드셀이 통신두절이면  
		else if(One_Weight_Ok && Two_Weight_Ok && !Three_Weight_Ok && Four_Weight_Ok) // 3번 로드셀이 통신두절이면  
		{
			// 에러갯수 조건 집어넣어야하고
			// cal시 비율 조건 계산해야하고 
			Display_Weight=(One_Weight+Two_Weight+Four_Weight*2);

			Three_Weight_Error_Counter++;
			if(Three_Weight_Error_Counter>=2)
			{
				Three_Weight_Error_Counter=0;
			}
/*			
        	One_Weight_Ok=0;
		  	Two_Weight_Ok=0;
		  	Three_Weight_Ok=0;
		  	Four_Weight_Ok=0;	
*/			
///			Weight_flag=1;
			adc_complete = 1;
		} // 3번이 통신 두절되면 
		// 4번 로드셀이 통신두절이면  
	  	else if(One_Weight_Ok && Two_Weight_Ok && Three_Weight_Ok && !Four_Weight_Ok) // 4번 로드셀이 통신두절이면  
   		{
			// 에러갯수 조건 집어넣어야하고
			// cal시 비율 조건 계산해야하고 
			Display_Weight=(One_Weight+Two_Weight+Three_Weight*2);

			Four_Weight_Error_Counter++;
			if(Four_Weight_Error_Counter>=2)
			{
				Four_Weight_Error_Counter=0;
			}
/*			
        	One_Weight_Ok=0;
		  	Two_Weight_Ok=0;
		  	Three_Weight_Ok=0;
		  	Four_Weight_Ok=0;	
*/			
///			Weight_flag=1;
			adc_complete = 1;
		} // 4번이 통신 두절되면  
		// 1,2번 로드셀 통신두절이 되면   
		else if(!One_Weight_Ok && !Two_Weight_Ok && Three_Weight_Ok && Four_Weight_Ok) // 1,2번 로드셀 통신두절이 되면   
		{
			// 에러갯수 조건 집어넣어야하고
			// cal시 비율 조건 계산해야하고 
			Display_Weight=(Three_Weight+Four_Weight)*2;  //3번4번 출력으로만 무게계산 

			One_Weight_Error_Counter++;
			if(One_Weight_Error_Counter>=2)
			{
				One_Weight_Error_Counter=0;
			}
/*			
        	One_Weight_Ok=0;
		  	Two_Weight_Ok=0;
		  	Three_Weight_Ok=0;
		  	Four_Weight_Ok=0;	
*/			
///			Weight_flag=1;
			adc_complete = 1;
		} // 1,3 번이 통신 두절되면 
		// 1,3번 로드셀 통신두절이 되면   
		else if(!One_Weight_Ok && Two_Weight_Ok && !Three_Weight_Ok && Four_Weight_Ok) // 1,3번 로드셀 통신두절이 되면   
		{
			// 에러갯수 조건 집어넣어야하고
			// cal시 비율 조건 계산해야하고 
			Display_Weight=(Two_Weight+Four_Weight)*2;  //2번4번 출력으로만 무게계산 

			One_Weight_Error_Counter++;
			if(One_Weight_Error_Counter>=2)
			{
				One_Weight_Error_Counter=0;
			}
/*			
        	One_Weight_Ok=0;
		  	Two_Weight_Ok=0;
		  	Three_Weight_Ok=0;
		  	Four_Weight_Ok=0;	
*/			
///			Weight_flag=1;
			adc_complete = 1;
		} // 1,3 번이 통신 두절되면 
		// 1,4번 로드셀 통신두절이 되면   
		else if( !One_Weight_Ok && Two_Weight_Ok && Three_Weight_Ok && !Four_Weight_Ok) // 1,4번 로드셀 통신두절이 되면   
		{
			// 에러갯수 조건 집어넣어야하고
			// cal시 비율 조건 계산해야하고 
			Display_Weight=(Two_Weight+Three_Weight)*2;  //2번3번 출력으로만 무게계산 

			One_Weight_Error_Counter++;
			if(One_Weight_Error_Counter>=2)
			{
				One_Weight_Error_Counter=0;
			}
/*			
        	One_Weight_Ok=0;
		  	Two_Weight_Ok=0;
		  	Three_Weight_Ok=0;
		  	Four_Weight_Ok=0;	
*/			
///			Weight_flag=1;
			adc_complete = 1;
		} // 1,4 번이 통신 두절되면 
		// 2,3번 로드셀 통신두절이 되면   
		else if( One_Weight_Ok && !Two_Weight_Ok && !Three_Weight_Ok && Four_Weight_Ok) // 2,3번 로드셀 통신두절이 되면   
		{
			// 에러갯수 조건 집어넣어야하고
			// cal시 비율 조건 계산해야하고 
			Display_Weight=(One_Weight+Four_Weight)*2;  //1번4번 출력으로만 무게계산 

			Two_Weight_Error_Counter++;
			if(Two_Weight_Error_Counter>=2)
			{
				Two_Weight_Error_Counter=0;
			}
/*			
        	One_Weight_Ok=0;
		  	Two_Weight_Ok=0;
		  	Three_Weight_Ok=0;
		  	Four_Weight_Ok=0;	
*/			
///			Weight_flag=1;
			adc_complete = 1;
		} // 2,3 번이 통신 두절되면 
		// 2,4번 로드셀 통신두절이 되면   
		else if( One_Weight_Ok && !Two_Weight_Ok && Three_Weight_Ok && !Four_Weight_Ok) // 2,4번 로드셀 통신두절이 되면   
		{
			// 에러갯수 조건 집어넣어야하고
			// cal시 비율 조건 계산해야하고 
			Display_Weight=(One_Weight+Three_Weight)*2;  //1번3번 출력으로만 무게계산 

			Two_Weight_Error_Counter++;
			if(Two_Weight_Error_Counter>=2)
			{
				Two_Weight_Error_Counter=0;
			}
/*			
        	One_Weight_Ok=0;
		  	Two_Weight_Ok=0;
		  	Three_Weight_Ok=0;
		  	Four_Weight_Ok=0;	
*/			
///			Weight_flag=1;
			adc_complete = 1;
		} // 2,4 번이 통신 두절되면 
		// 3,4번 로드셀 통신두절이 되면   
		else if( One_Weight_Ok && Two_Weight_Ok && !Three_Weight_Ok && !Four_Weight_Ok) // 3,4번 로드셀 통신두절이 되면   
		{
			// 에러갯수 조건 집어넣어야하고
			// cal시 비율 조건 계산해야하고 
			Display_Weight=(One_Weight+Two_Weight)*2;  //1번2번 출력으로만 무게계산 

			Three_Weight_Error_Counter++;
			if(Three_Weight_Error_Counter>=2)
			{
				Three_Weight_Error_Counter=0;
			}
/*			
        	One_Weight_Ok=0;
		  	Two_Weight_Ok=0;
		  	Three_Weight_Ok=0;
		  	Four_Weight_Ok=0;	
*/			
///			Weight_flag=1;
			adc_complete = 1;
		} // 1,2 번이 통신 두절되면 
		// 1,2,3번 로드셀 통신두절이 되면   
		else if( !One_Weight_Ok && !Two_Weight_Ok && !Three_Weight_Ok && Four_Weight_Ok) // 1, 2, 3번 로드셀 통신두절이 되면   
		{
			Display_Weight=(Four_Weight)*4;  //4번 출력으로만 무게계산 
			adc_complete = 1;
		}
		// 1,3,4번 로드셀 통신두절이 되면   
		else if( !One_Weight_Ok && Two_Weight_Ok && !Three_Weight_Ok && !Four_Weight_Ok) // 1,3,4번 로드셀 통신두절이 되면   
		{
			Display_Weight=(Two_Weight)*4;  //2번 출력으로만 무게계산 
			adc_complete = 1;
		}
		// 1,2,4번 로드셀 통신두절이 되면   
		else if( !One_Weight_Ok && !Two_Weight_Ok && Three_Weight_Ok && !Four_Weight_Ok) // 1,2,4번 로드셀 통신두절이 되면   
		{
			Display_Weight=(Three_Weight)*4;  //3번 출력으로만 무게계산 
			adc_complete = 1;
		}
		// 2,3,4번 로드셀 통신두절이 되면   
		else if( One_Weight_Ok && !Two_Weight_Ok && !Three_Weight_Ok && !Four_Weight_Ok) // 2,3,4번 로드셀 통신두절이 되면   
		{
			Display_Weight=(One_Weight)*4;  //1번 출력으로만 무게계산 
			adc_complete = 1;
		} // 2,3,4 번이 통신 두절되면 

//sprintf(sT0, "%ld:%ld, %ld, %ld, %ld %ld.", Weight_flag, One_Weight, Two_Weight, Three_Weight, Four_Weight, Display_Weight);
//USART1_puts(sT0); 

if(gnlDisplay)
{
//	sprintf(sT0, "%ld, %ld, %ld, %ld %ld.", One_Weight, Two_Weight, Three_Weight, Four_Weight, Display_Weight);
	sprintf(sT0, " (%u:%u) %ld, %d..%d,%d,%d,%d,%d,%d, %ld, %ld, %ld, %ld  ", rotation, Weight_flag, Display_Weight, adc_complete,
	One_Weight_Ok, Two_Weight_Ok, Three_Weight_Ok, Four_Weight_Ok, Five_Weight_Ok, Six_Weight_Ok, One_Weight, Two_Weight, Three_Weight, Four_Weight
	);
	//lcd70_show_string_mode(300, 150+(5*30), (const unsigned char *)sT0, 20, 0);
	//lcd70_show_string_mode(10, 400, (const unsigned char *)sT0, 20, 0);
}
	} // if rotation==1 이면 4번 다하고나면 


	rotation++;
	if(rotation>4) rotation=1;
	
	
//2016.7.26
	if(adc_complete) 
	{
		Weight_flag = 1;
		return Display_Weight;
	}
	else
	{
		return mon_raw_adc;
	}
}





// Loadcell_Number 적용
long Get_adc_from_module_20170626(void)
{
	int	iz;
	int	 adc_complete = 0;	//ad 를 전부 스캔했는지 ?
	long weight;
	long Display_Weight = -1;
	long ret_val;
	
	char sT0[150];	//0116
	
	if(rotation == 1)
	{
		Weight_flag	= 0;
/* 2016.10.12		
		One_Weight = 99999;
		Two_Weight = 99999;
		Three_Weight = 99999;
		Four_Weight = 99999;
		Five_Weight = 99999;
		Six_Weight = 99999;
*/		
		One_Weight_Ok = 0;
		Two_Weight_Ok = 0;
		Three_Weight_Ok = 0;
		Four_Weight_Ok = 0;	
		Five_Weight_Ok = 0;	
		Six_Weight_Ok = 0;			
	}
	
	// 422 송신 프로토콜 	
	sprintf(sT0, "@-%cR", rotation+0x30); 
	USART2_puts(sT0);
//USART1_print(sT0);
  
//	rotation++;
//	if(rotation>4) rotation=1;
	
	for(iz=0; iz<100; iz++) //typical response time 45ms~47ms
	{  	
		//delay_ms(100);   // critical !! module로부터 응답을 기다리는 시간, 굉장히 중요함  
		delay_ms(1);
///USART1_puts("a");		
		if(Usart2_Interrupt_flag)
		{
			delay_ms(1);   // critical !! module로부터 응답을 기다리는 시간, 굉장히 중요함  
///USART1_puts("b");		
			break;
		}
	}
sprintf(sT0, " l%d=%d.", rotation, iz); 
USART1_print(sT0);

	
   	// 1,2,3,4 순서대로 ad 모듈을 스캐닝한다 
	//==============================
	if( !Usart2_Interrupt_flag) // a/d 모듈이 응답이 없으면
	{
		if(Adc_Counter) Adc_Counter--;
	}
	else  // 응답이 있으면
	{
	    Usart2_Interrupt_flag = 0;
		RxCounter2 = 0;
		
		//junction power off delay counter
        Adc_Counter = OPTION_POWER_COUNTER; // 20 을 기본으로 무조건 넣는다. 
		
		weight = 0;
///		weight = (cmd_2[2]-0x30)*100000+(cmd_2[3]-0x30)*10000+(cmd_2[4]-0x30)*1000+(cmd_2[5]-0x30)*100+(cmd_2[6]-0x30)*10+(cmd_2[7]-0x30);
		for(iz=2; iz<=7; iz++)
		{
			if(('0'<=cmd_2[iz] && cmd_2[iz]<='9') || ('A'<=cmd_2[iz] && cmd_2[iz]<='F'))
			{	
				weight = (weight * 10) + cmd_2[iz]-'0';
///USART1_puts("c");				
				continue;
			}
			else break;
		}
		
		if(iz < 8) weight = -1;	// 못받아 왔으면
		else
		{
			//address check
			if( !strncmp(cmd_2, "-1", 2)) 
			{
				One_Weight = weight;
				One_Weight_Ok = 1;
				One_Weight_Error_Counter = 0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
			}
			else if( !strncmp(cmd_2, "-2", 2)) 
			{
				Two_Weight = weight;
				Two_Weight_Ok = 1;
				Two_Weight_Error_Counter = 0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
			}
			else if( !strncmp(cmd_2, "-3", 2)) 
			{
				Three_Weight = weight;
				Three_Weight_Ok = 1;
				Three_Weight_Error_Counter = 0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
			}
			else if( !strncmp(cmd_2, "-4", 2)) 
			{
				Four_Weight = weight;
				Four_Weight_Ok = 1;
				Four_Weight_Error_Counter = 0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
			}
			// add 1.9 2016.6.26
			else if( !strncmp(cmd_2, "-5", 2)) 
			{
				Five_Weight = weight;
				Five_Weight_Ok = 1;
				Five_Weight_Error_Counter = 0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
			}
			else if( !strncmp(cmd_2, "-6", 2)) 
			{
				Six_Weight = weight;
				Six_Weight_Ok = 1;
				Six_Weight_Error_Counter = 0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
			}
		}
			
//sprintf(sT0, " w=%ld %s", weight, cmd_2);
//lcd70_show_string_mode(400, 30+(cmd_2[1]-'0')*30, (const unsigned char *)sT0, 20, 0);
//USART1_print(sT0);
			 
		Initial_cmd_2_RxBuffer2();

	} //if(Usart2_Interrupt_flag) // a/d 모듈이	


	
	
//	if(rotation==1) // module전부를 한cycle scaning 이 끝나면 	 
// 1.9 2016.6.26	if(rotation == 4) // module전부를 한cycle scaning 이 끝나면 
	if(rotation == Loadcell_Number) // module전부를 한cycle scaning 이 끝나면 
	{
		if (Loadcell_Number == 6)
		{
		//sprintf(sT0, "%d, %d, %d, %d.", One_Weight_Ok, Two_Weight_Ok, Three_Weight_Ok, Four_Weight_Ok);
		//lcd70_show_string_mode(400, 30+(5*30), (const unsigned char *)sT0, 20, 0);
			if( !gnlLcCompensation || (One_Weight_Ok && Two_Weight_Ok && Three_Weight_Ok && Four_Weight_Ok && Five_Weight_Ok && Six_Weight_Ok)) // 무게 6개가 다알아오면 
			{
				Display_Weight = One_Weight + Two_Weight + Three_Weight + Four_Weight + Five_Weight + Six_Weight;
				adc_complete = 1;
//				Weight_flag = 1;
			} // 무게가 4개 다날아오면 
			///////////////////// 통신에러가 발생시 ///////////////////////
			else if(!One_Weight_Ok && Two_Weight_Ok && Three_Weight_Ok && Four_Weight_Ok && Five_Weight_Ok && Six_Weight_Ok) // 1번 로드셀이 통신두절이면  
			{
				// 에러갯수 조건 집어넣어야하고
				// cal시 비율 조건 계산해야하고 

				Display_Weight = Two_Weight + Two_Weight + Three_Weight + Four_Weight + Five_Weight + Six_Weight;
				
				One_Weight_Error_Counter++;
				if(One_Weight_Error_Counter>=2)
				{
					One_Weight_Error_Counter=0;
				}
	/*			
				One_Weight_Ok=0;
				Two_Weight_Ok=0;
				Three_Weight_Ok=0;
				Four_Weight_Ok=0;	
	*/			
//				Weight_flag = 1;
				adc_complete = 1;
			}
			// 2번이 통신 두절되면 
			else if(One_Weight_Ok && !Two_Weight_Ok && Three_Weight_Ok && Four_Weight_Ok && Five_Weight_Ok && Six_Weight_Ok) // 2번 로드셀이 통신두절이면  
			{
				// 에러갯수 조건 집어넣어야하고	
				// cal시 비율 조건 계산해야하고 
				Display_Weight = One_Weight + One_Weight + Three_Weight + Four_Weight + Five_Weight + Six_Weight;

				Two_Weight_Error_Counter++;
				if(Two_Weight_Error_Counter>=2)
				{
					Two_Weight_Error_Counter=0;
				}
	/*			
				One_Weight_Ok=0;
				Two_Weight_Ok=0;
				Three_Weight_Ok=0;
				Four_Weight_Ok=0;	
	*/			
//				Weight_flag = 1;
				adc_complete = 1;
			} // 2번이 통신 두절되면  
			// 3번 로드셀이 통신두절이면  
			else if(One_Weight_Ok && Two_Weight_Ok && !Three_Weight_Ok && Four_Weight_Ok && Five_Weight_Ok && Six_Weight_Ok) // 3번 로드셀이 통신두절이면  
			{
				// 에러갯수 조건 집어넣어야하고
				// cal시 비율 조건 계산해야하고 
				Display_Weight = One_Weight + Two_Weight + Four_Weight + Four_Weight + Five_Weight + Six_Weight;
				Three_Weight_Error_Counter++;
				if(Three_Weight_Error_Counter>=2)
				{
					Three_Weight_Error_Counter=0;
				}
	/*			
				One_Weight_Ok=0;
				Two_Weight_Ok=0;
				Three_Weight_Ok=0;
				Four_Weight_Ok=0;	
	*/			
//				Weight_flag = 1;
				adc_complete = 1;
			} // 3번이 통신 두절되면 
			// 4번 로드셀이 통신두절이면  
			else if(One_Weight_Ok && Two_Weight_Ok && Three_Weight_Ok && !Four_Weight_Ok && Five_Weight_Ok && Six_Weight_Ok) // 4번 로드셀이 통신두절이면  
			{
				// 에러갯수 조건 집어넣어야하고
				// cal시 비율 조건 계산해야하고 
				Display_Weight = One_Weight + Two_Weight + Three_Weight + Three_Weight + Five_Weight + Six_Weight;

				Four_Weight_Error_Counter++;
				if(Four_Weight_Error_Counter>=2)
				{
					Four_Weight_Error_Counter=0;
				}
	/*			
				One_Weight_Ok=0;
				Two_Weight_Ok=0;
				Three_Weight_Ok=0;
				Four_Weight_Ok=0;	
	*/			
//				Weight_flag = 1;
				adc_complete = 1;
			} // 4번이 통신 두절되면  
			// 5번 로드셀이 통신두절이면  
			else if(One_Weight_Ok && Two_Weight_Ok && Three_Weight_Ok && Four_Weight_Ok && !Five_Weight_Ok && Six_Weight_Ok) // 5번 로드셀이 통신두절이면  
			{
				// 에러갯수 조건 집어넣어야하고
				// cal시 비율 조건 계산해야하고 
				Display_Weight = One_Weight + Two_Weight + Three_Weight + Four_Weight + Six_Weight + Six_Weight;

				Five_Weight_Error_Counter++;
				if(Five_Weight_Error_Counter>=2)
				{
					Five_Weight_Error_Counter=0;
				}
	/*			
				One_Weight_Ok=0;
				Two_Weight_Ok=0;
				Three_Weight_Ok=0;
				Four_Weight_Ok=0;	
	*/			
//				Weight_flag = 1;
				adc_complete = 1;
			} // 5번이 통신 두절되면  
			// 6번 로드셀이 통신두절이면  
			else if(One_Weight_Ok && Two_Weight_Ok && Three_Weight_Ok && Four_Weight_Ok && Five_Weight_Ok && !Six_Weight_Ok) // 6번 로드셀이 통신두절이면  
			{
				// 에러갯수 조건 집어넣어야하고
				// cal시 비율 조건 계산해야하고 
				Display_Weight = One_Weight + Two_Weight + Three_Weight + Four_Weight + Five_Weight + Five_Weight;

				Six_Weight_Error_Counter++;
				if(Six_Weight_Error_Counter>=2)
				{
					Six_Weight_Error_Counter=0;
				}
	/*			
				One_Weight_Ok=0;
				Two_Weight_Ok=0;
				Three_Weight_Ok=0;
				Four_Weight_Ok=0;	
	*/			
//				Weight_flag = 1;
				adc_complete = 1;
			} // 6번이 통신 두절되면
			
		} else { // L/C NOT 6
			
	//sprintf(sT0, "%d, %d, %d, %d.", One_Weight_Ok, Two_Weight_Ok, Three_Weight_Ok, Four_Weight_Ok);
	//lcd70_show_string_mode(400, 30+(5*30), (const unsigned char *)sT0, 20, 0);
			if( !gnlLcCompensation || (One_Weight_Ok && Two_Weight_Ok && Three_Weight_Ok && Four_Weight_Ok)) // 무게 4개가 다알아오면 
			{
				Display_Weight=(One_Weight+Two_Weight+Three_Weight+Four_Weight);
	/*			
				One_Weight_Ok=0;
				Two_Weight_Ok=0;
				Three_Weight_Ok=0;
				Four_Weight_Ok=0;	
	*/			
//				Weight_flag = 1;
				adc_complete = 1;
			} // 무게가 4개 다날아오면 
			///////////////////// 통신에러가 발생시 ///////////////////////
			else if(!One_Weight_Ok && Two_Weight_Ok && Three_Weight_Ok && Four_Weight_Ok) // 1번 로드셀이 통신두절이면  
			{
				// 에러갯수 조건 집어넣어야하고
				// cal시 비율 조건 계산해야하고 
				Display_Weight=(Two_Weight*2+Three_Weight+Four_Weight);
				
				One_Weight_Error_Counter++;
				if(One_Weight_Error_Counter>=2)
				{
					One_Weight_Error_Counter=0;
				}
	/*			
				One_Weight_Ok=0;
				Two_Weight_Ok=0;
				Three_Weight_Ok=0;
				Four_Weight_Ok=0;	
	*/			
//				Weight_flag = 1;
				adc_complete = 1;
			}
			// 2번이 통신 두절되면 
			else if(One_Weight_Ok && !Two_Weight_Ok && Three_Weight_Ok && Four_Weight_Ok) // 2번 로드셀이 통신두절이면  
			{
				// 에러갯수 조건 집어넣어야하고	
				// cal시 비율 조건 계산해야하고 
				Display_Weight=(One_Weight*2+Three_Weight+Four_Weight);

				Two_Weight_Error_Counter++;
				if(Two_Weight_Error_Counter>=2)
				{
					Two_Weight_Error_Counter=0;
				}
	/*			
				One_Weight_Ok=0;
				Two_Weight_Ok=0;
				Three_Weight_Ok=0;
				Four_Weight_Ok=0;	
	*/			
//				Weight_flag = 1;
				adc_complete = 1;
			} // 2번이 통신 두절되면  
			// 3번 로드셀이 통신두절이면  
			else if(One_Weight_Ok && Two_Weight_Ok && !Three_Weight_Ok && Four_Weight_Ok) // 3번 로드셀이 통신두절이면  
			{
				// 에러갯수 조건 집어넣어야하고
				// cal시 비율 조건 계산해야하고 
				Display_Weight=(One_Weight+Two_Weight+Four_Weight*2);

				Three_Weight_Error_Counter++;
				if(Three_Weight_Error_Counter>=2)
				{
					Three_Weight_Error_Counter=0;
				}
	/*			
				One_Weight_Ok=0;
				Two_Weight_Ok=0;
				Three_Weight_Ok=0;
				Four_Weight_Ok=0;	
	*/			
//				Weight_flag = 1;
				adc_complete = 1;
			} // 3번이 통신 두절되면 
			// 4번 로드셀이 통신두절이면  
			else if(One_Weight_Ok && Two_Weight_Ok && Three_Weight_Ok && !Four_Weight_Ok) // 4번 로드셀이 통신두절이면  
			{
				// 에러갯수 조건 집어넣어야하고
				// cal시 비율 조건 계산해야하고 
				Display_Weight=(One_Weight+Two_Weight+Three_Weight*2);

				Four_Weight_Error_Counter++;
				if(Four_Weight_Error_Counter>=2)
				{
					Four_Weight_Error_Counter=0;
				}
	/*			
				One_Weight_Ok=0;
				Two_Weight_Ok=0;
				Three_Weight_Ok=0;
				Four_Weight_Ok=0;	
	*/			
//				Weight_flag = 1;
				adc_complete = 1;
			} // 4번이 통신 두절되면  
			// 1,2번 로드셀 통신두절이 되면   
			else if(!One_Weight_Ok && !Two_Weight_Ok && Three_Weight_Ok && Four_Weight_Ok) // 1,2번 로드셀 통신두절이 되면   
			{
				// 에러갯수 조건 집어넣어야하고
				// cal시 비율 조건 계산해야하고 
				Display_Weight=(Three_Weight+Four_Weight)*2;  //3번4번 출력으로만 무게계산 

				One_Weight_Error_Counter++;
				if(One_Weight_Error_Counter>=2)
				{
					One_Weight_Error_Counter=0;
				}
	/*			
				One_Weight_Ok=0;
				Two_Weight_Ok=0;
				Three_Weight_Ok=0;
				Four_Weight_Ok=0;	
	*/			
//				Weight_flag = 1;
				adc_complete = 1;
			} // 1,3 번이 통신 두절되면 
			// 1,3번 로드셀 통신두절이 되면   
			else if(!One_Weight_Ok && Two_Weight_Ok && !Three_Weight_Ok && Four_Weight_Ok) // 1,3번 로드셀 통신두절이 되면   
			{
				// 에러갯수 조건 집어넣어야하고
				// cal시 비율 조건 계산해야하고 
				Display_Weight=(Two_Weight+Four_Weight)*2;  //2번4번 출력으로만 무게계산 

				One_Weight_Error_Counter++;
				if(One_Weight_Error_Counter>=2)
				{
					One_Weight_Error_Counter=0;
				}
	/*			
				One_Weight_Ok=0;
				Two_Weight_Ok=0;
				Three_Weight_Ok=0;
				Four_Weight_Ok=0;	
	*/			
//				Weight_flag = 1;
				adc_complete = 1;
			} // 1,3 번이 통신 두절되면 
			// 1,4번 로드셀 통신두절이 되면   
			else if( !One_Weight_Ok && Two_Weight_Ok && Three_Weight_Ok && !Four_Weight_Ok) // 1,4번 로드셀 통신두절이 되면   
			{
				// 에러갯수 조건 집어넣어야하고
				// cal시 비율 조건 계산해야하고 
				Display_Weight=(Two_Weight+Three_Weight)*2;  //2번3번 출력으로만 무게계산 

				One_Weight_Error_Counter++;
				if(One_Weight_Error_Counter>=2)
				{
					One_Weight_Error_Counter=0;
				}
	/*			
				One_Weight_Ok=0;
				Two_Weight_Ok=0;
				Three_Weight_Ok=0;
				Four_Weight_Ok=0;	
	*/			
//				Weight_flag = 1;
				adc_complete = 1;
			} // 1,4 번이 통신 두절되면 
			// 2,3번 로드셀 통신두절이 되면   
			else if( One_Weight_Ok && !Two_Weight_Ok && !Three_Weight_Ok && Four_Weight_Ok) // 2,3번 로드셀 통신두절이 되면   
			{
				// 에러갯수 조건 집어넣어야하고
				// cal시 비율 조건 계산해야하고 
				Display_Weight=(One_Weight+Four_Weight)*2;  //1번4번 출력으로만 무게계산 

				Two_Weight_Error_Counter++;
				if(Two_Weight_Error_Counter>=2)
				{
					Two_Weight_Error_Counter=0;
				}
	/*			
				One_Weight_Ok=0;
				Two_Weight_Ok=0;
				Three_Weight_Ok=0;
				Four_Weight_Ok=0;	
	*/			
//				Weight_flag = 1;
				adc_complete = 1;
			} // 2,3 번이 통신 두절되면 
			// 2,4번 로드셀 통신두절이 되면   
			else if( One_Weight_Ok && !Two_Weight_Ok && Three_Weight_Ok && !Four_Weight_Ok) // 2,4번 로드셀 통신두절이 되면   
			{
				// 에러갯수 조건 집어넣어야하고
				// cal시 비율 조건 계산해야하고 
				Display_Weight=(One_Weight+Three_Weight)*2;  //1번3번 출력으로만 무게계산 

				Two_Weight_Error_Counter++;
				if(Two_Weight_Error_Counter>=2)
				{
					Two_Weight_Error_Counter=0;
				}
	/*			
				One_Weight_Ok=0;
				Two_Weight_Ok=0;
				Three_Weight_Ok=0;
				Four_Weight_Ok=0;	
	*/			
//				Weight_flag = 1;
				adc_complete = 1;
			} // 2,4 번이 통신 두절되면 
			// 3,4번 로드셀 통신두절이 되면   
			else if( One_Weight_Ok && Two_Weight_Ok && !Three_Weight_Ok && !Four_Weight_Ok) // 3,4번 로드셀 통신두절이 되면   
			{
				// 에러갯수 조건 집어넣어야하고
				// cal시 비율 조건 계산해야하고 
				Display_Weight=(One_Weight+Two_Weight)*2;  //1번2번 출력으로만 무게계산 

				Three_Weight_Error_Counter++;
				if(Three_Weight_Error_Counter>=2)
				{
					Three_Weight_Error_Counter=0;
				}
	/*			
				One_Weight_Ok=0;
				Two_Weight_Ok=0;
				Three_Weight_Ok=0;
				Four_Weight_Ok=0;	
	*/			
//				Weight_flag = 1;
				adc_complete = 1;
			} // 1,2 번이 통신 두절되면 
			// 1,2,3번 로드셀 통신두절이 되면   
			else if( !One_Weight_Ok && !Two_Weight_Ok && !Three_Weight_Ok && Four_Weight_Ok) // 1, 2, 3번 로드셀 통신두절이 되면   
			{
				Display_Weight=(Four_Weight)*4;  //4번 출력으로만 무게계산 
//				Weight_flag = 1;
				adc_complete = 1;
			}
			// 1,3,4번 로드셀 통신두절이 되면   
			else if( !One_Weight_Ok && Two_Weight_Ok && !Three_Weight_Ok && !Four_Weight_Ok) // 1,3,4번 로드셀 통신두절이 되면   
			{
				Display_Weight=(Two_Weight)*4;  //2번 출력으로만 무게계산 
//				Weight_flag = 1;
				adc_complete = 1;
			}
			// 1,2,4번 로드셀 통신두절이 되면   
			else if( !One_Weight_Ok && !Two_Weight_Ok && Three_Weight_Ok && !Four_Weight_Ok) // 1,2,4번 로드셀 통신두절이 되면   
			{
				Display_Weight=(Three_Weight)*4;  //3번 출력으로만 무게계산 
//				Weight_flag = 1;
				adc_complete = 1;
			}
			// 2,3,4번 로드셀 통신두절이 되면   
			else if( One_Weight_Ok && !Two_Weight_Ok && !Three_Weight_Ok && !Four_Weight_Ok) // 2,3,4번 로드셀 통신두절이 되면   
			{
				Display_Weight=(One_Weight)*4;  //1번 출력으로만 무게계산 
//				Weight_flag = 1;
				adc_complete = 1;
			} // 2,3,4 번이 통신 두절되면 
		}
//sprintf(sT0, "%ld:%ld, %ld, %ld, %ld %ld.", Weight_flag, One_Weight, Two_Weight, Three_Weight, Four_Weight, Display_Weight);
//USART1_puts(sT0); 

if(gnlDisplay)
{
//	sprintf(sT0, "%ld, %ld, %ld, %ld %ld.", One_Weight, Two_Weight, Three_Weight, Four_Weight, Display_Weight);
	sprintf(sT0, " (%u:%u) %ld, %d..%d,%d,%d,%d,%d,%d, %ld, %ld, %ld, %ld, %ld, %ld ", rotation, Weight_flag, Display_Weight, adc_complete,
	One_Weight_Ok, Two_Weight_Ok, Three_Weight_Ok, Four_Weight_Ok, Five_Weight_Ok, Six_Weight_Ok, One_Weight, Two_Weight, Three_Weight, Four_Weight, Five_Weight, Six_Weight
	);
	//lcd70_show_string_mode(300, 150+(5*30), (const unsigned char *)sT0, 20, 0);
//	lcd70_show_string_mode(10, 400, (const unsigned char *)sT0, 20, 0);
}
	} // if rotation==1 이면 4번 다하고나면 


	rotation++;
// 1.9 2017.6.26	if(rotation>4) rotation=1;
	if(rotation > Loadcell_Number) rotation = 1;
	
///USART1_puts("x");	
//2016.7.26
	if(adc_complete) 
	{
///USART1_puts("y");	
		Weight_flag = 1;
		ret_val = Display_Weight;
	}
	else
	{
///USART1_puts("z");	
		ret_val = mon_raw_adc;
	}
	
	return ret_val;
}



/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
#ifdef ____unused____
long Get_adc_from_module(void) //1116
{
	int	iz;
	long Display_Weight;
	char sT0[50];	//0116

	// 422 송신 프로토콜 
	//request adc data to junction box
	sprintf(sT0, "@-%cR", rotation+0x30); 
	USART2_puts(sT0);
//USART1_print(sT0);
  
	rotation++;
	if(rotation>4) rotation=1;
	
	//wait response from junction box
	for(iz=0; iz<100; iz++) //typical response time 45ms~47ms
	{  	
		//delay_ms(100);   // critical !! module로부터 응답을 기다리는 시간, 굉장히 중요함  
		delay_ms(1);
		if(Usart2_Interrupt_flag)
		{
			delay_ms(1);   // critical !! module로부터 응답을 기다리는 시간, 굉장히 중요함  
			break;
		}
	}
//sprintf(sT0, " iz=%d.", iz); 
//USART1_puts(sT0);

	
   	// 1,2,3,4 순서대로 ad 모듈을 스캐닝한다 
//==============================
	if(Usart2_Interrupt_flag) // a/d 모듈이  
	{
        Adc_Counter=OPTION_POWER_COUNTER; // 20 을 기본으로 무존건 넣은다. 
	    Usart2_Interrupt_flag=0;
		RxCounter2=0;	     
	
		switch(cmd_2[1]) //address
		{
			case '1':
		    	One_Weight=(cmd_2[2]-0x30)*100000+(cmd_2[3]-0x30)*10000+(cmd_2[4]-0x30)*1000+(cmd_2[5]-0x30)*100+(cmd_2[6]-0x30)*10+(cmd_2[7]-0x30);
		    	One_Weight_Ok=1;
				One_Weight_Error_Counter=0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
		   		break;
		  	case '2':
		    	Two_Weight=(cmd_2[2]-0x30)*100000+(cmd_2[3]-0x30)*10000+(cmd_2[4]-0x30)*1000+(cmd_2[5]-0x30)*100+(cmd_2[6]-0x30)*10+(cmd_2[7]-0x30);
		    	Two_Weight_Ok=1;
				Two_Weight_Error_Counter=0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
		   		break;
          	case '3':
				Three_Weight=(cmd_2[2]-0x30)*100000+(cmd_2[3]-0x30)*10000+(cmd_2[4]-0x30)*1000+(cmd_2[5]-0x30)*100+(cmd_2[6]-0x30)*10+(cmd_2[7]-0x30);
		    	Three_Weight_Ok=1;
				Three_Weight_Error_Counter=0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
		   		break;
          	case '4':
		  		Four_Weight=(cmd_2[2]-0x30)*100000+(cmd_2[3]-0x30)*10000+(cmd_2[4]-0x30)*1000+(cmd_2[5]-0x30)*100+(cmd_2[6]-0x30)*10+(cmd_2[7]-0x30);
		  		Four_Weight_Ok=1;
				Four_Weight_Error_Counter=0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
		   		break;
		   default:
		     	break;
        }
		
		cmd_2[10] = 0;
sprintf(sT0, "%s", cmd_2);
//lcd70_show_string_mode(400, 30+(cmd_2[1]-'0')*30, (const unsigned char *)sT0, 20, 0);
USART1_puts(sT0);
	 	 
			Initial_cmd_2_RxBuffer2();

	} //if(Usart2_Interrupt_flag) // a/d 모듈이	
	else  // return이 없으면 
	{
		if(Adc_Counter) Adc_Counter--;
	}

	if(rotation==1) // module전부를 한cycle scaning 이 끝나면 	 
	{


//sprintf(sT0, "%d, %d, %d, %d.", One_Weight_Ok, Two_Weight_Ok, Three_Weight_Ok, Four_Weight_Ok);
//lcd70_show_string_mode(400, 30+(5*30), (const unsigned char *)sT0, 20, 0);


		if(One_Weight_Ok&& Two_Weight_Ok && Three_Weight_Ok &&Four_Weight_Ok) // 무게 4개가 다알아오면 
	   	{
	  		Display_Weight=(One_Weight+Two_Weight+Three_Weight+Four_Weight);
        	One_Weight_Ok=0;
		  	Two_Weight_Ok=0;
		  	Three_Weight_Ok=0;
		  	Four_Weight_Ok=0;	

if(gnlDisplay)
{
	sprintf(sT0, "%ld, %ld, %ld, %ld %ld.", One_Weight, Two_Weight, Three_Weight, Four_Weight, Display_Weight);
	//lcd70_show_string_mode(300, 150+(5*30), (const unsigned char *)sT0, 20, 0);
//	lcd70_show_string_mode(10, 400, (const unsigned char *)sT0, 20, 0);
}

			Initial_cmd_2_RxBuffer2();

			Weight_flag=1;
			RxCounter2=0;
	    	return Display_Weight;
	   	} // 무게가 4개 다날아ㅇ오면 


	 	///////////////////// 통신에러가 발생시 ///////////////////////
		if(!One_Weight_Ok ||  !Two_Weight_Ok  ||  !Three_Weight_Ok  || !Four_Weight_Ok) // 4개중에 하나라도 통신에러가 발생하면 
	  	{
	  		if(!One_Weight_Ok&& Two_Weight_Ok && Three_Weight_Ok &&Four_Weight_Ok) // 1번 로드셀이 통신두절이면  
	   		{
         		// 에러갯수 조건 집어넣어야하고
		 		// cal시 비율 조건 계산해야하고 
		  		One_Weight_Error_Counter++;
		  		if(One_Weight_Error_Counter>=2)
		  		{
	  	   			Display_Weight=(Two_Weight*2+Three_Weight+Four_Weight);
		   			One_Weight_Error_Counter=0;
		  		}
          		One_Weight_Ok=0;
		  		Two_Weight_Ok=0;
		  		Three_Weight_Ok=0;
		  		Four_Weight_Ok=0;

			Initial_cmd_2_RxBuffer2();

	  			Weight_flag=1;
				RxCounter2=0;
	    		return Display_Weight;
	   		} // 1,2번이 통신 두절되면 
	
      		if(One_Weight_Ok&& !Two_Weight_Ok && Three_Weight_Ok &&Four_Weight_Ok) // 2번 로드셀이 통신두절이면  
	   		{
         		// 에러갯수 조건 집어넣어야하고	
		 		// cal시 비율 조건 계산해야하고 
		  		Two_Weight_Error_Counter++;
		  		if(Two_Weight_Error_Counter>=2)
		  		{
	  	   			Display_Weight=(One_Weight*2+Three_Weight+Four_Weight);
		   			Two_Weight_Error_Counter=0;
		  		}
          		One_Weight_Ok=0;
		  		Two_Weight_Ok=0;
		  		Three_Weight_Ok=0;
		  		Four_Weight_Ok=0;

			Initial_cmd_2_RxBuffer2();

	  			Weight_flag=1;
				RxCounter2=0;
	    		return Display_Weight;
	   		} // 2번이 통신 두절되면  
	
 	   		if(One_Weight_Ok&& Two_Weight_Ok && !Three_Weight_Ok &&Four_Weight_Ok) // 3번 로드셀이 통신두절이면  
	   		{
         		// 에러갯수 조건 집어넣어야하고
		 		// cal시 비율 조건 계산해야하고 
		  		Three_Weight_Error_Counter++;
		  		if(Three_Weight_Error_Counter>=2)
		  		{
	  	   			Display_Weight=(One_Weight+Two_Weight+Four_Weight*2);
		   			Three_Weight_Error_Counter=0;
		  		}
          		One_Weight_Ok=0;
		  		Two_Weight_Ok=0;
		  		Three_Weight_Ok=0;
		  		Four_Weight_Ok=0;

			Initial_cmd_2_RxBuffer2();

	  			Weight_flag=1;
				RxCounter2=0;
	    		return Display_Weight;
	   		} // 3번이 통신 두절되면 
	
	  		if(One_Weight_Ok&& Two_Weight_Ok && Three_Weight_Ok &&!Four_Weight_Ok) // 4번 로드셀이 통신두절이면  
	   		{
         		// 에러갯수 조건 집어넣어야하고
		 		// cal시 비율 조건 계산해야하고 
		  		Four_Weight_Error_Counter++;
		  		if(Four_Weight_Error_Counter>=2)
		  		{
	  	   			Display_Weight=(One_Weight+Two_Weight+Three_Weight*2);
		   			Four_Weight_Error_Counter=0;
		  		}
          		One_Weight_Ok=0;
		  		Two_Weight_Ok=0;
		  		Three_Weight_Ok=0;
		  		Four_Weight_Ok=0;

			Initial_cmd_2_RxBuffer2();

	  			Weight_flag=1;
				RxCounter2=0;
	    		return Display_Weight;
	   		} // 4번이 통신 두절되면  


	   		if(!One_Weight_Ok && !Two_Weight_Ok && Three_Weight_Ok &&Four_Weight_Ok) // 1,2번 로드셀 통신두절이 되면   
	   		{
         		// 에러갯수 조건 집어넣어야하고
		 		// cal시 비율 조건 계산해야하고 
		  		One_Weight_Error_Counter++;
		  		if(One_Weight_Error_Counter>=2)
		  		{
	  	   			Display_Weight=(Three_Weight+Four_Weight)*2;  //3번4번 출력으로만 무게계산 
		   			One_Weight_Error_Counter=0;
		  		}
          		One_Weight_Ok=0;
		  		Two_Weight_Ok=0;
		  		Three_Weight_Ok=0;
		  		Four_Weight_Ok=0;

			Initial_cmd_2_RxBuffer2();

	  			Weight_flag=1;
				RxCounter2=0;
	    		return Display_Weight;
	   		} // 1,2 번이 통신 두절되면 

	  		if(One_Weight_Ok&& Two_Weight_Ok && !Three_Weight_Ok &&!Four_Weight_Ok) // 1,2번 로드셀 통신두절이 되면   
	   		{
         		// 에러갯수 조건 집어넣어야하고
		 		// cal시 비율 조건 계산해야하고 
		 		Three_Weight_Error_Counter++;
		  		if(Three_Weight_Error_Counter>=2)
		  		{
	  	   			Display_Weight=(One_Weight+Two_Weight)*2;  //3번4번 출력으로만 무게계산 
		   			Three_Weight_Error_Counter=0;
		  		}
          		One_Weight_Ok=0;
		  		Two_Weight_Ok=0;
		  		Three_Weight_Ok=0;
		  		Four_Weight_Ok=0;

				Initial_cmd_2_RxBuffer2();
				
	  			Weight_flag=1;
				RxCounter2=0;
	    		return Display_Weight;
	   		} // 1,2 번이 통신 두절되면 

		} //4개중에서 하나라도 통신에러가 생기면 
		///////////////////// 통신에러가 발생시 ///////////////////////
	 
	} // if rotation==1 이면 4번 다하고나면 

	return -1;
}
#endif	//#ifdef ____unused____

/* end of file */
