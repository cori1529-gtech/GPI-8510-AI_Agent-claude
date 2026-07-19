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
	 RS422_test();
	
	 
	 //RS232_test();
	 
//	cdma_test();
	//input_output_test();

 }

 
void RS422_test(void)
	  {
			
	//		#define	RS422ON		GPIO_SetBits(GPIOA, GPIO_Pin_4)
 //   #define	  RS422OFF	GPIO_ResetBits(GPIOA, GPIO_Pin_4)

     char sT0[100];
	   char ch;
	   char exit_flag=0;
 	
		lcd_cls();
	  lcd_clear();
		//GLCD_string (0,0 ,"    TEST MODE   ");
		//Delay_ms(1000);
		lcd_inverse();
		GLCD_string (0,0 ," RS_422 TEST  ");
		lcd_normal();
		GLCD_string (0,6 ,"ENTER : NEXT    ");
		
 
	 while(!exit_flag)
	  {
			
			
			if(Usart2_Interrupt_flag==1)
			{
       ////if((cmd_2[0]-0x30)==(gnlMyAddress/10) &&(cmd_2[1]-0x30)==(gnlMyAddress%10))
			 {				 
				RS422ON;Delay_ms(100);
				 sprintf (sT0, "number %2d interrupt is occured!", (long)gnlMyAddress);
				 USART2_puts(sT0);
				 Delay_ms(100);
				 //USART2_puts("RS_422 Interrupt is occured !");
				 RS422OFF;
				Usart2_Interrupt_flag=0;
			 }
			 
			 }	
			
	  if(keypush_8510())
		{
		// ch=keypush_8510();

		 switch(KeyCode)
   {
		 case UP:
		 case DOWN:
		 case LEFT:
		 case RIGHT:
			  RS422ON;//Delay_ms(100);
		 USART2_puts("This is RS_422 TEST !!!");
		  RS422OFF;//Delay_ms(100);
		 break;
		 case ENTER:
		   exit_flag=1;
		  break;
		 default:
			 break;
	 } // of switch 
		} // of keypush
	 }; // of while 
  }

void RS232_test(void)
	  {
			
	//		#define	RS422ON		GPIO_SetBits(GPIOA, GPIO_Pin_4)
 //   #define	  RS422OFF	GPIO_ResetBits(GPIOA, GPIO_Pin_4)

     char sT0[100];
	   char ch;
	   char exit_flag=0;
 	
		lcd_cls();
			lcd_clear();
		//GLCD_string (0,0 ,"    TEST MODE   ");
		//Delay_ms(1000);
		lcd_inverse();
		GLCD_string (0,0 ," RS_232 TEST  ");
		lcd_normal();
		GLCD_string (0,6 ,"ENTER : NEXT    ");
		
 
	 while(!exit_flag)
	  {
			
			
			if(Usart3_sms_received_flag==1)
			{
       ////if((cmd_2[0]-0x30)==(gnlMyAddress/10) &&(cmd_2[1]-0x30)==(gnlMyAddress%10))
			 {				 
				 //RS422ON;Delay_ms(100);
				 sprintf (sT0, "number %2d interrupt is occured!", (long)gnlMyAddress);
				 USART3_puts(sT0);
				 Delay_ms(100);
				 //USART2_puts("RS_422 Interrupt is occured !");
				 //RS422OFF;
				Usart3_sms_received_flag=0;
			 }
			 
			 }	
			
	  if(keypush_8510())
		{
		// ch=keypush_8510();

		 switch(KeyCode)
   {
		 case UP:
		 case DOWN:
		 case LEFT:
		 case RIGHT:
			  RS422ON;//Delay_ms(100);
		 USART3_puts("This is RS_232 TEST !!!");
		  RS422OFF;//Delay_ms(100);
		 break;
		 case ENTER:
		   exit_flag=1;
		  break;
		 default:
			 break;
	 } // of switch 
		} // of keypush
	 }; // of while 
  }




	
 
 
 
 
// input_output_test();
void  key_test(void)
  {
     char sT0[100];
	   char ch;
	   char exit_flag=0;
 	
		lcd_cls();
		lcd_clear();
		GLCD_string (0,0 ,"    TEST MODE   ");
		Delay_ms(1000);
		lcd_inverse();
		GLCD_string (0,0 ,"    KEY TEST    ");
		lcd_normal();
		GLCD_string (0,6 ,"ENTER : NEXT    ");
		
 
	 while(!exit_flag)
	  {
	  if(keypush_8510())
		{
		// ch=keypush_8510();

		 switch(KeyCode)
   {
		 case UP:
		  GLCD_string (0,4 ,"UP              ");
		  break;
		 case DOWN:
		  GLCD_string (0,4 ,"DOWN            ");
		  break;
		 case LEFT:
		  GLCD_string (0,4 ,"LEFT            ");
		  break;
		 case RIGHT:
		  GLCD_string (0,4 ,"RIGHT           ");
		  break;
		 case ENTER:
		   exit_flag=1;
		  break;
		 default:
			 break;
	 } // of switch 
		} // of keypush
	 }; // of while 
  }

	
	void input_output_test(void)
	
  {
     char sT0[100];
	   char ch;
	   char exit_flag=0;
 	
		lcd_cls();
		lcd_clear();
		All_Relay_Off();
	//	GLCD_string (0,0 ,"    TEST MODE   ");
		//Delay_ms(1000);
		lcd_inverse();
		GLCD_string (0,0 ,"INPUT-OUTPUT  ");
		lcd_normal();
		
		GLCD_string (0,4 ,"KEY : OUTPUT    ");
		GLCD_string (0,6 ,"ENTER : NEXT    ");
		
 
	 while(!exit_flag)
	  {
			
			 FIRST_INPUT_ON // first input
			{
			  GLCD_string (0,2 ,"FIRST INPUT ON ");
				GLCD_string (0,4 ,"OUTPUT  1 ON");
				All_Relay_Off();
				Gpi8510_Relay_On(SILO_ONE);
				/*
				First_Relay_On();
				Second_Relay_Off();
				Third_Relay_Off();
				Fourth_Relay_Off();
				*/
			}

    	SECOND_INPUT_ON // first input
			{
			  GLCD_string (0,2 ,"SECOND INPUT ON ");
				GLCD_string (0,4 ,"OUTPUT  2 ON");
				All_Relay_Off();
				Gpi8510_Relay_On(SILO_TWO);
				
				/*
				Second_Relay_On();
				First_Relay_Off();
				Third_Relay_Off();
				Fourth_Relay_Off();
			  */	
			}

 		THIRD_INPUT_ON // first input
			{
			  GLCD_string (0,2 ,"THIRD INPUT ON ");
				GLCD_string (0,4 ,"OUTPUT  3 ON");
				/*
				Second_Relay_Off();
				First_Relay_Off();
				Fourth_Relay_Off();
				Third_Relay_On();
         */
        All_Relay_Off();
				Gpi8510_Relay_On(ERROR);
								

				}
	  
			FOURTH_INPUT_ON // first input
			{
			  GLCD_string (0,2 ,"FOURTH INPUT ON ");
				GLCD_string (0,4 ,"OUTPUT  4 ON");
				/*
				Fourth_Relay_On();
				Second_Relay_Off();
				Third_Relay_Off();
				First_Relay_Off();
			  */
        All_Relay_Off();
				Gpi8510_Relay_On(PULSE);
								
			}

	  if(keypush_8510())
		{
			
		 switch(KeyCode)
   {
		 case UP:
		    GLCD_string (0,4 ,"OUTPUT  1 ON");
		  	First_Relay_On();
				Second_Relay_Off();
				Third_Relay_Off();
				Fourth_Relay_Off();
			
		  break;
		 case DOWN:
			 GLCD_string (0,4 ,"OUTPUT  2 ON");
		 Second_Relay_On();
				First_Relay_Off();
				Third_Relay_Off();
				Fourth_Relay_Off();
		  break;
		 case LEFT:
			 GLCD_string (0,4 ,"OUTPUT  3 ON");
		 			Second_Relay_Off();
				First_Relay_Off();
				Fourth_Relay_Off();
				Third_Relay_On();
	
		  break;
		 case RIGHT:
			 GLCD_string (0,4 ,"OUTPUT  4 ON");
		 		Fourth_Relay_On();
				Second_Relay_Off();
				Third_Relay_Off();
				First_Relay_Off();
		
		  break;
		 case ENTER:
		   exit_flag=1;
		  break;
		 default:
			 break;
	 } // of switch 
		} // of keypush
	 }; // of while 
  }
	
	
	
	
	

void  display_test(void)
  {
     //char sT0[100];
	 char ch;
	 int i;
	 char exit_flag=0;
	
		restart:
    All_led_Off();		
			lcd_cls();
		lcd_clear();
		lcd_inverse();
		GLCD_string (0,0 ,"  DISPLAY TEST  ");
		lcd_normal();
		GLCD_string (0,2 ,"ANY KEY : START ");
		GLCD_string (0,6 ,"ENTER : NEXT    ");
		
		
		
   while(!exit_flag)
	  {
	  if(keypush_8510())
		{
		// ch=keypush_8510();

		 switch(KeyCode)
   {
		 case UP:case DOWN:case LEFT:case RIGHT:
		 lcd_cls();
		 GLCD_string (0,0 ,"SILO_1 FEED LED ");
		 Silo1_led_On();
		 Delay_ms(500);
		
		 GLCD_string (0,2 ,"SILO_2 FEED LED ");
		 Silo2_led_On();
		 Delay_ms(500);
		
		 GLCD_string (0,4 ,"SILO_1 ERROR LED");
		 //Silo1_led_On();
		 Silo1_Error_led_On();
		 Delay_ms(500);
		 GLCD_string (0,6 ,"SILO_2 ERROR LED");
		 Silo2_Error_led_On();
		 Delay_ms(500);
		 
		 
		 goto restart;
		 break;
		 
		 
		 case ENTER:
		   exit_flag=1;
		  break;
		 default:
			 break;
	 } // of switch 
		} // of keypush
	 }; // of while 




  } // of function

	
	
	
void  adc_test(void)
  {
     char sT0[100];
	 char ch;
	 char exit_flag=0;
	long temp_buff,temp_buff1;
	int iz, jz;
		
		lcd_cls();
		lcd_clear();
		lcd_inverse();
		GLCD_string (0,0 ,"  ADC TEST  ");
		lcd_normal();
		GLCD_string (0,6 ,"ENTER : NEXT    ");
		
	/*	
		
			//CdmaSendQueue init
	CdmaSendQueue_qhead = CdmaSendQueue_qtail = 0;
	for(iz=0; iz<MAX_CDMA_QUEUE_BUFFER; iz++)
	{
		for(jz=0; jz<256; jz++)
		{
			CdmaSendQueue[iz][jz] = NULL;
		}
	}

	RxCdmaQ_qhead = RxCdmaQ_qtail = 0;
	for(iz=0; iz<RXCDMAQ_SIZE; iz++)
	{
		RxCdmaQ[iz] = NULL;
	}

	RxCdmaTokenQ_qhead = RxCdmaTokenQ_qtail = 0;

	bachul_send_timer_ms = 0;	//배출중 정보 전송 타이머
	cdma_response_waiting_flag = 0;	//cdma 응답을 기다리는 상태
	cdma_response_wait_timer_ms = 0;	//cdma 응답 기다리는 타이머
	giCdmaStopEnabled = 0;	//cdma 를 강제로 stop 실행을 해야 하는가 플래그

		*/
		
	
		
		
	//	which_adc=1;
		
		
		  while(!exit_flag)
	  {
		

    if(which_adc==1) 
		{
			temp_buff1 = read_filtered_adc_CS5555_1(); // 처음에 값을 가져온다 .
			 if(temp_buff1>400000)
			sprintf(sT0, "SILO_1:NO L/C!"); // A/D값 
	    else
			 sprintf(sT0, "SILO_1:%7ld",(long)temp_buff1); // A/D값 
      GLCD_string(0,2, sT0);
		 temp_buff = read_filtered_adc_CS5555_2(); // 처음에 값을 가져온다 .
	  if(temp_buff>400000)
			sprintf(sT0, "SILO_2:NO L/C!"); // A/D값 
 	  else	
		sprintf(sT0, "SILO_2:%7ld",(long)temp_buff); // A/D값 
	 }  
	else
	 {
			temp_buff1 = read_filtered_adc(); // 처음에 값을 가져온다 .
			 if(temp_buff1>400000)
			sprintf(sT0, "SILO_1:NO L/C!"); // A/D값 
	    else
			 sprintf(sT0, "SILO_1:%7ld",(long)temp_buff1); // A/D값 
      GLCD_string(0,2, sT0);
		 temp_buff = read_filtered_adc_1(); // 처음에 값을 가져온다 .
	  if(temp_buff>400000)
			sprintf(sT0, "SILO_2:NO L/C!"); // A/D값 
 	  else	
		sprintf(sT0, "SILO_2:%7ld",(long)temp_buff); // A/D값 
	 } 
	
	
	
	
			GLCD_string(0,4, sT0);
			
	  if(keypush_8510())
		{
		// ch=keypush_8510();

		 switch(KeyCode)
   {
		 case ENTER:
		   exit_flag=1;
		  break;
		 default:
			 break;
	 } // of switch 
		} // of keypush
	
		
			
			
		};
  }
	
	void cdma_test(void)
	
	 {
     char sT0[50];
	 char ch;
	 char exit_flag=0;
	long temp_buff;
		 int iz,jz;
		 int i;
		
		lcd_cls();
		lcd_inverse();
		GLCD_string (0,0 ,"  LTE      TEST  ");
		lcd_normal();
		GLCD_string (0,6 ,"ENTER : NEXT    ");
		 
		 
	 	 USARTx_Initial(); // usart initialize
   USART1_puts("Cdma_Power_On();");
	 Cdma_Power_On();
	 Delay_ms(100);
	 cdma_send_cmd("AT$LGTRESET", 11);	//2016.6.10 when main softreset
	 Delay_ms(100);
 	 Cdma_Power_Off();
	 Delay_ms(1000);
	  Cdma_Power_On();
	 Delay_ms(100);
		 
		 
		 
		 
	/***************CdmaSendQueue init *************************************/
	CdmaSendQueue_qhead = CdmaSendQueue_qtail = 0;
	for(iz=0; iz<MAX_CDMA_QUEUE_BUFFER; iz++)
	{
		for(jz=0; jz<256; jz++)
		{
			CdmaSendQueue[iz][jz] = NULL;
		}
	}

	RxCdmaQ_qhead = RxCdmaQ_qtail = 0;
	for(iz=0; iz<RXCDMAQ_SIZE; iz++)
	{
		RxCdmaQ[iz] = NULL;
	}

	RxCdmaTokenQ_qhead = RxCdmaTokenQ_qtail = 0;

	bachul_send_timer_ms = 0;	//배출중 정보 전송 타이머
	cdma_response_waiting_flag = 0;	//cdma 응답을 기다리는 상태
	cdma_response_wait_timer_ms = 0;	//cdma 응답 기다리는 타이머
	giCdmaStopEnabled = 0;	//cdma 를 강제로 stop 실행을 해야 하는가 플래그
/***************CdmaSendQueue init ************************************/
		 
		for(iz=0;iz<3;iz++)
		{
			//add 2015.1.22
			cdma_send_cmd("AT$LGTDELRM=*", strlen("AT$LGTDELRM=*"));
			delay_ms(100);
		}
		LTE_TEST:	

		for(i=0;i<300;i++)
		{	
			
			delay_ms(100);
			get_cdma_info_WAIT(CDMA_GET_TELNO, "TELNO");	//get_cdma_telno();
			get_cdma_info_WAIT(CDMA_GET_TIME, "TIME");	//get_cdma_time();
			get_cdma_info_WAIT(CDMA_GET_RSSI, "RSSI");	//get_cdma_rssi();
			//0728
			
			
			sprintf(sT0, "%2d,t=%s", i, jangbi.cdma.telno);
			GLCD_string(0,0, sT0);
			
			sprintf(sT0, "%2d,rssi= %d", i, jangbi.cdma.rssi);
			GLCD_string(0,2, sT0);
			
      sprintf(sT0, "Date=%04d-%02d-%02d", jangbi.cdma.tm.year, jangbi.cdma.tm.month, jangbi.cdma.tm.date);
		  GLCD_string(0,4, sT0);
 		
      sprintf(sT0, "Time=%02d:%02d:%02d",  jangbi.cdma.tm.hour, jangbi.cdma.tm.min, jangbi.cdma.tm.sec);
		  GLCD_string(0,6, sT0);
			Delay_ms(100);

		}

		while(!exit_flag)
	  {
	  		
  		/*	
		 case UP:case DOWN:case LEFT:case RIGHT:
				  goto LTE_TEST;
			   break;

			*/
/*			
			#define    LEFT_ARROW_ON	       if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8) == 0x00)	 //
#define    RIGHT_ARROW_ON	     if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6) == 0x00)	 //
#define    UP_ARROW_ON	         if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9) == 0x00)	 //
#define    DOWN_ARROW_ON	     if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5) == 0x00)	//
#define    ENTER_ARROW_ON	   if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7) == 0x00)	//
*/
		 
		 LEFT_ARROW_ON	
			goto LTE_TEST;
		
		RIGHT_ARROW_ON
			goto LTE_TEST;
			
			UP_ARROW_ON
			goto LTE_TEST;




		 ENTER_ARROW_ON // if enter key is pressed
			 exit_flag=1;
		
			
			
		};
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
//	       lcd70_show_string_mode(300, 250,str_1, 16, 0);
		 
		  sprintf(str_1, "L/C_2 ADC=%6lu ",(long)zero_2);
//	      lcd70_show_string_mode(300, 270,str_1, 16, 0);
	 	 
		  sprintf(str_1, "L/C_3 ADC=%6lu ",(long)zero_3);
//	      lcd70_show_string_mode(300, 290,str_1, 16, 0);
	 	 
		  sprintf(str_1, "L/C_4 ADC=%6lu ",(long)zero_4);
//	      lcd70_show_string_mode(300, 310,str_1, 16, 0);
	 	 	
		 

		  zero_sum=(zero_1+zero_2+zero_3+zero_4);
		    sprintf(str_1, "SUM VALUE=%6lu ",(long)zero_sum);
//	       lcd70_show_string_mode(300, 350,str_1, 16, 0);
	 	 
		  
		

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

#ifdef dpfdpf 
void input_output_test(void)
  {
     char sT0[100];
	 char ch;
	 char exit_flag=0;
//	 lcd70_clear_screen(WHITE);
//	 lcd70_show_string_mode(100, 100,"TEST 4:INPUT OUTPUT TEST", 16, 0);
//	 lcd70_show_string_mode(100, 165,"KEY_1:RELAY1_ON   KEY_2: RELAY2_ON  KEY_3:RELAY3_ON   KEY_4:RELAY1,2,3 OFF"   , 16, 0);
  //   lcd70_show_string_mode(100, 195,"KEY_5:JUNCTION ON   KEY_6:JUNCTION OFF  KEY_7:CDMA_ON   KEY_8:CDMA OFF"   , 16, 0);
	// lcd70_show_string_mode(100, 255,"ENTER KEY: NEXT TEST", 16, 0);
 
 	 while(!exit_flag)
	  {

	  
      FIRST_INPUT
	  {
	  //  lcd70_show_string_mode(100, 290,"FIRST_INPUT ON", 16, 0);
      }
      
	 SECOND_INPUT
	  {
	   // lcd70_show_string_mode(100, 290,"SECOND_INPUT ON", 16, 0);
      }
	  
//	 VIBARTION_INPUT
     VIBRATION_INPUT
	  {
	   // lcd70_show_string_mode(100, 290,"VIBRATION_INPUT ON", 16, 0);
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

	#endif
	
//cdma_test();

	#ifdef dpfdpf 
	
	void cdma_test(void)
  {
     char sT0[100];
	 char ch;
	 char exit_flag=0;
	 //lcd70_clear_screen(WHITE);
	 //lcd70_show_string_mode(100, 100,"TEST 4:CDMA TEST", 16, 0);
	 //lcd70_show_string_mode(100, 255,"ENTER KEY: NEXT TEST", 16, 0);
 
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
#endif

/* end of file */
