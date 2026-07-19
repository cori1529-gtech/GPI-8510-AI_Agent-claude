/*
	buzzer.c
*/

#include "hw_config.h"
#include "buzzer.h"

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
	// TR is active low.
	GPIO_SetBits(BUZZER_OUT_PORT, BUZZER_OUT_PIN);

//GPIO_ResetBits(GPIOC, GPIO_Pin_0);	
}

void buzzer_off(void) 
{
	GPIO_ResetBits(BUZZER_OUT_PORT, BUZZER_OUT_PIN);
//	GPIO_SetBits(GPIOC, GPIO_Pin_0);	
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
	Relay_2_On();
}

// 에러  릴레이 off
void Error_Relay_Off(void) 
{
//	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	Relay_2_Off();
}

// 펄스 릴레이 on ,3번 
void  Pulse_Relay_On(void) 
{	
//	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	Relay_3_On();
}

 // 펄스  릴레이 off
void  Pulse_Relay_Off(void)
{	
//	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	Relay_3_Off();
}

//  cdma 전원   릴레이 on
void  Cdma_Power_On(void) 
{
//	GPIO_ResetBits(GPIOD, GPIO_Pin_2);
	Relay_4_On();
}

// cdma power off 
void  Cdma_Power_Off(void)
{
//	GPIO_SetBits(GPIOD, GPIO_Pin_2);
	Relay_4_Off();
}

//  junction 전원   릴레이 on
void  Junction_Power_On(void) 
{
//	GPIO_ResetBits(GPIOD, GPIO_Pin_3);
	Relay_5_On();
}

// junction power off 
void  Junction_Power_Off(void) 
{
//	GPIO_SetBits(GPIOD, GPIO_Pin_3);
	Relay_5_Off();
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
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);// Relay_3 on
}

void Relay_3_Off(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_9);// Relay_3 off
}

void Relay_4_On(void)
{
	GPIO_ResetBits(GPIOD, GPIO_Pin_2);
}

void Relay_4_Off(void)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_2);
}

void Relay_5_On(void)
{
	GPIO_ResetBits(GPIOD, GPIO_Pin_3);
}	

void Relay_5_Off(void)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_3);
}	

