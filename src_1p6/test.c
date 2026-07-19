/*
	test.c
*/
											   
#include "hw_config.h"
#include "stm32f10x_it.h"									 
#include "stm32f10x.h"

#include "buzzer.h"
#include "sms.h"
#include "main.h"
#include "led.h"
#include "timer.h"
#include "usart.h"
//#include "rtc.h"
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


#include <string.h>


#define    FIRST_INPUT          if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) == 0x00)     
#define    SECOND_INPUT         if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) == 0x00)     
#define    VIBRATION_INPUT      if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12) == 0x00)   //진동센서   


void  test_mode(void)
 {

	key_test();
	display_test();
	adc_test();
	input_output_test();
	cdma_test();
	
	

 }

// input_output_test();
void  key_test(void)
  {
     char sT0[100];
	 char ch;
	 char exit_flag=0;
	 lcd70_clear_screen(WHITE);
	 lcd70_show_string_mode(300, 100,"TEST 1  : KEY TEST", 16, 0);
	 lcd70_show_string_mode(300, 165,"PRESS ANY   KEY !!", 16, 0);
 	 lcd70_show_string_mode(300, 195,"ENTER KEY: NEXT TEST", 16, 0);
 	
 
	 while(!exit_flag)
	  {
	  if (keypush())
		{
		 ch=key_disp[KEYCODE];   //1127
	     sprintf(sT0, "KEY=%2d  CODE=%2d",(int)ch,(int)KEYCODE);
	     lcd70_show_string_mode(300, 225, (const unsigned char *)sT0, 20, 0);
		 if(ch==KEY_ENTER) exit_flag=1;
   	   }
	 };
  }


void  display_test(void)
  {
     char sT0[100];
	 char ch;
	 int i;
	 char exit_flag=0;
	 start:
	 lcd70_clear_screen(WHITE);
	 lcd70_show_string_mode(300, 100,"TEST 2  : DISPLAY TEST", 16, 0);
	 lcd70_show_string_mode(300, 165,"PRESS ANY   KEY !!", 16, 0);
 	 lcd70_show_string_mode(300, 195,"ENTER KEY: NEXT TEST", 16, 0);
 	
 
	 while(!exit_flag)
	  {
	  if (keypush())
		{
		 ch=key_disp[KEYCODE];
		 if(ch==KEY_ENTER) exit_flag=1;
	
	     for(i=0;i<320;i+=30)
		 {
	
	     sprintf(sT0, "ABCDEFGHIGJLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789~!@#$%^&*()_+ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	     lcd70_show_string_mode(0, i, (const unsigned char *)sT0, 16, 0);
		 }
		 goto start;
	   }
	 };
  }

void  adc_test(void)
  {
     char sT0[100];
	 char ch;
	 char exit_flag=0;
	 lcd70_clear_screen(WHITE);
	 lcd70_show_string_mode(300, 100,"TEST 3  : ADC_TEST", 16, 0);
 	 lcd70_show_string_mode(300, 195,"ENTER KEY: NEXT TEST", 16, 0);
 	
	Get_adc_value_test_mode();
 	 /*
	 while(!exit_flag)
	  {
	  if (keypush())
		{
		 ch=key_disp[KEYCODE];   //1127
	    // sprintf(sT0, "KEY=%2d  CODE=%2d",(int)ch,(int)KEYCODE);
	    // lcd70_show_string_mode(300, 225, (const unsigned char *)sT0, 20, 0);
		 if(ch==KEY_ENTER) exit_flag=1;
		 Get_adc_value_test_mode();
   	   }
	 };
	 */
  }
 long Get_adc_value_test_mode(void)
 {
   char temp;
   char str_1[50];
   char ch;
    int i_1;
	long zero_1,zero_2,zero_3,zero_4;
	long zero_sum;
	char One_Weight_Ok_1=0,Two_Weight_Ok_1=0,Three_Weight_Ok_1=0,Four_Weight_Ok_1=0;
    char rotation_1=1;; // ㅇ
//	char ch;
   
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
	   
	       sprintf(str_1, "L/C_1 ADC=%6lu ",(long)zero_1);
	       lcd70_show_string_mode(300, 250,str_1, 16, 0);
		 
		  sprintf(str_1, "L/C_2 ADC=%6lu ",(long)zero_2);
	      lcd70_show_string_mode(300, 270,str_1, 16, 0);
	 	 
		  sprintf(str_1, "L/C_3 ADC=%6lu ",(long)zero_3);
	      lcd70_show_string_mode(300, 290,str_1, 16, 0);
	 	 
		  sprintf(str_1, "L/C_4 ADC=%6lu ",(long)zero_4);
	      lcd70_show_string_mode(300, 310,str_1, 16, 0);
	 	 	
		 

		  zero_sum=(zero_1+zero_2+zero_3+zero_4);
		    sprintf(str_1, "SUM VALUE=%6lu ",(long)zero_sum);
	       lcd70_show_string_mode(300, 350,str_1, 16, 0);
	 	 
		  
		

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
		 ch=key_disp[KEYCODE];   //1127
		 if(ch==KEY_ENTER)
		 break;
	  /*
	  //return 
	  if(KEYCODE==KEY)// 확인키 
	   {
	    // return zero_sum;
		  break;
       }
		*/
	 }
	 	
	};
  }


void input_output_test(void)
  {
     char sT0[100];
	 char ch;
	 char exit_flag=0;
	 lcd70_clear_screen(WHITE);
	 lcd70_show_string_mode(100, 100,"TEST 4:INPUT OUTPUT TEST", 16, 0);
	 lcd70_show_string_mode(100, 165,"KEY_1:RELAY1_ON   KEY_2: RELAY2_ON  KEY_3:RELAY3_ON   KEY_4:RELAY1,2,3 OFF"   , 16, 0);
     lcd70_show_string_mode(100, 195,"KEY_5:JUNCTION ON   KEY_6:JUNCTION OFF  KEY_7:CDMA_ON   KEY_8:CDMA OFF"   , 16, 0);
	 lcd70_show_string_mode(100, 255,"ENTER KEY: NEXT TEST", 16, 0);
 
 	 while(!exit_flag)
	  {

	  
      FIRST_INPUT
	  {
	    lcd70_show_string_mode(100, 290,"FIRST_INPUT ON", 16, 0);
      }
      
	 SECOND_INPUT
	  {
	    lcd70_show_string_mode(100, 290,"SECOND_INPUT ON", 16, 0);
      }
	  
//	 VIBARTION_INPUT
     VIBRATION_INPUT
	  {
	    lcd70_show_string_mode(100, 290,"VIBRATION_INPUT ON", 16, 0);
      }

	  if (keypush())
		{
		 ch=key_disp[KEYCODE];   //1127
		 switch(ch)
		  {
		   case KEY_1:
		      Output_Relay_On();
               break;
		   case KEY_2:
		       Error_Relay_On();
               break;
		   case KEY_3:
		      Pulse_Relay_On();
               break;
		   case KEY_4:
		      Output_Relay_Off();
			  Error_Relay_Off();
		      Pulse_Relay_Off();
               break;
		  case KEY_5:
		      Junction_Power_On();
               break;
		  case KEY_6:
		      Junction_Power_Off();
               break;
		   
		  case KEY_7:
		      Cdma_Power_On();
               break;
		  case KEY_8:
		      Cdma_Power_Off();
               break;
	       default: break;
         }
	 	 if(ch==KEY_ENTER) exit_flag=1;
  	   }
	 };
  }

//cdma_test();
void cdma_test(void)
  {
     char sT0[100];
	 char ch;
	 char exit_flag=0;
	 lcd70_clear_screen(WHITE);
	 lcd70_show_string_mode(100, 100,"TEST 4:CDMA TEST", 16, 0);
	 lcd70_show_string_mode(100, 255,"ENTER KEY: NEXT TEST", 16, 0);
 
 	 while(!exit_flag)
	  {
	  if (keypush())
		{
		 ch=key_disp[KEYCODE];   //1127
		 switch(ch)
		  {
		   case KEY_1:
		      Output_Relay_On();
               break;
		   case KEY_2:
		       Error_Relay_On();
               break;
		   case KEY_3:
		      Pulse_Relay_On();
               break;
		   case KEY_4:
		      Output_Relay_Off();
			  Error_Relay_Off();
		      Pulse_Relay_Off();
               break;
		  case KEY_5:
		      Junction_Power_On();
               break;
		  case KEY_6:
		      Junction_Power_Off();
               break;
		   
		  case KEY_7:
		      Cdma_Power_On();
               break;
		  case KEY_8:
		      Cdma_Power_Off();
               break;
	       default: break;
         }
	  
	   //  sprintf(sT0, "KEY=%2d  CODE=%2d",(int)ch,(int)KEYCODE);
	   //  lcd70_show_string_mode(300, 225, (const unsigned char *)sT0, 20, 0);
		 if(ch==KEY_ENTER) exit_flag=1;
  	   }
	 };
  }


/* end of file */
