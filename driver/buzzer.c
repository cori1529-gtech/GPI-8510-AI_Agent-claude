/*
	buzzer.c
*/

#include "hw_config.h"
#include "buzzer.h"
#include "board_gpi8510.h"

void Relay_1_On(void);
void Relay_1_Off(void);
void Relay_2_On(void);
void Relay_2_Off(void);
void Relay_3_On(void);
void Relay_3_Off(void);
void Relay_4_On(void);
void Relay_4_Off(void);
void Relay_5_On(void);
void Relay_5_Off(void);


//gpi-8510   20170818
/*
#define    FIRST_RELAY _ON                      GPIO_SetBits(GPIOD, GPIO_Pin_11);  
#define    SECOND_RELAY _ON                 GPIO_SetBits(GPIOD, GPIO_Pin_12);  
#define    THIRD_RELAY _ON                     GPIO_SetBits(GPIOD, GPIO_Pin_13);  
#define    FOURTH_RELAY _ON                 GPIO_SetBits(GPIOB, GPIO_Pin_4);  
*/

void  First_Relay_On(void)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_11);
}
void  Second_Relay_On(void)
	{
	GPIO_SetBits(GPIOD, GPIO_Pin_12);
}
void  Third_Relay_On(void)
	{
	GPIO_SetBits(GPIOD, GPIO_Pin_13);
}
void  Fourth_Relay_On(void)
	{
	GPIO_SetBits(GPIOB, GPIO_Pin_4);
}
void  First_Relay_Off(void)
	{
	GPIO_ResetBits(GPIOD, GPIO_Pin_11);
  }
void  Second_Relay_Off(void)
{
	GPIO_ResetBits(GPIOD, GPIO_Pin_12);
  }
	void  Third_Relay_Off(void)
{
	GPIO_ResetBits(GPIOD, GPIO_Pin_13);
  }
	void  Fourth_Relay_Off(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);
  }

	void  All_Relay_Off(void)
	{
		First_Relay_Off();
		Second_Relay_Off();
		Third_Relay_Off();
		Fourth_Relay_Off();
		
	}	
		
void Gpi8510_Relay_On(char number)
 {
	 switch(number)
	 {
		 case 1:First_Relay_On();
			    break;
		 case 2:Second_Relay_On();
			    break;
		 case 3:Third_Relay_On();
			    break;
		 case 4:Fourth_Relay_On();
			    break;
		 default: break;
	 } 
 } 
	
	
	void Gpi8510_Relay_Off(char number)

	{
	 switch(number)
	 {
		 case 1:First_Relay_Off();
			    break;
		 case 2:Second_Relay_Off();
			    break;
		 case 3:Third_Relay_Off();
			    break;
		 case 4:Fourth_Relay_Off();
			    break;
		 default: break;
	 } 
 }
	
	
	




void buzzer_init(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;						 					
	
  	GPIO_InitStructure.GPIO_Pin =  BUZZER_OUT_PIN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(BUZZER_OUT_PORT, &GPIO_InitStructure);

	buzzer_off();
}

void buzzer_on(void) 
{
	GPIO_SetBits(BUZZER_OUT_PORT, BUZZER_OUT_PIN);

}

void buzzer_off(void) 
{
	GPIO_ResetBits(BUZZER_OUT_PORT, BUZZER_OUT_PIN);
}

void beep(long dly)
{
	buzzer_on();
	Delay_ms(dly);
	buzzer_off();
}

void retry_message()
{
	int iz;

	for(iz=0; iz<7; iz++) { beep(20); Delay_ms(5); }
}

// 배출 릴레이 on  ,1번
void Output_Relay_On(void)
{	
//	GPIO_SetBits (GPIOB, GPIO_Pin_5);
	Relay_1_On();
}


void  Battery_Power_On(void) //  battery power on
{
	GPIO_SetBits(GPIOD, GPIO_Pin_1);
}

void  Battery_Power_Off(void) //  battery  power off 
{

GPIO_ResetBits(GPIOD, GPIO_Pin_1);
}

// 배출 릴레이 off
void Output_Relay_Off(void) 
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	Relay_1_Off();
}
 
// 에러  릴레이 on ,2번 
void Error_Relay_On(void) 
{	
//	GPIO_SetBits(GPIOB, GPIO_Pin_8);
//-	Relay_2_On();
	Relay_3_On();
}

// 에러  릴레이 off
void Error_Relay_Off(void) 
{
//	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
//-	Relay_2_Off();
	Relay_3_Off();
}

// 펄스 릴레이 on ,3번 
void  Pulse_Relay_On(void) 
{	
//	GPIO_SetBits(GPIOB, GPIO_Pin_9);
//-	Relay_3_On();
}

 // 펄스  릴레이 off
void  Pulse_Relay_Off(void)
{	
//	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
//-	Relay_3_Off();
}

//  cdma 전원   릴레이 on
void  Cdma_Power_On(void) 
{
  GPIO_ResetBits(MODEM_PWR_PORT, MODEM_PWR_PIN);
	//Relay_4_On();
}

// cdma power off 
void  Cdma_Power_Off(void)
{
 GPIO_SetBits(MODEM_PWR_PORT, MODEM_PWR_PIN);
	//Relay_4_Off();
}

//  junction 전원   릴레이 on
void  Junction_Power_On(void) 
{
	 GPIO_ResetBits(GPIOD, GPIO_Pin_3);
}

// junction power off 
void  Junction_Power_Off(void) 
{
 GPIO_SetBits(GPIOD, GPIO_Pin_3);
}

/* end of file */


/*
	GPIO_SetBits (GPIOB, GPIO_Pin_5);
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	GPIO_ResetBits(GPIOD, GPIO_Pin_2);
	GPIO_ResetBits(GPIOD, GPIO_Pin_3);
*/


void Adc_Power_Off(void)
{
	Relay_3_Off();
	//GPIO_SetBits(GPIOB, GPIO_Pin_9);// Relay_1 off
}

void Adc_Power_On(void)
{
	Relay_3_On();
	//GPIO_ResetBits(GPIOB, GPIO_Pin_9);// Relay_1 off
}

void Relay_1_On(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_5);// Relay_1 on
}

void Relay_1_Off(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);// Relay_1 off
}

void Relay_2_On(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_8);// Relay_2 on
}

void Relay_2_Off(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);// Relay_2 off
}

void Relay_3_On(void)
{
//-	GPIO_ResetBits(GPIOB, GPIO_Pin_9);// Relay_3 on
	GPIO_SetBits(GPIOB, GPIO_Pin_9);// Relay_3 on
}

void Relay_3_Off(void)
{
//-	GPIO_SetBits(GPIOB, GPIO_Pin_9);// Relay_3 off
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);// Relay_3 off
}

void Relay_4_On(void)
{
	GPIO_ResetBits(MODEM_PWR_PORT, MODEM_PWR_PIN);
}

void Relay_4_Off(void)
{
	GPIO_SetBits(MODEM_PWR_PORT, MODEM_PWR_PIN);
}

void Relay_5_On(void)
{
	GPIO_ResetBits(GPIOD, GPIO_Pin_3);
}	

void Relay_5_Off(void)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_3);
}	

