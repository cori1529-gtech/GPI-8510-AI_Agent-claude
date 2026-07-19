/*
	buzzer.h
*/

#ifndef _BUZZER_H_
#define _BUZZER_H_

#define BUZZER_OUT_PORT  	GPIOC		//GPIOB
#define BUZZER_OUT_PIN	 	GPIO_Pin_0	//GPIO_Pin_5

void buzzer_init(void);
void buzzer_on(void);
void buzzer_off(void);
void beep(long dly);
void retry_message(void);

void Output_Relay_On(void); // 배출 릴레이 on  ,1번
void Output_Relay_Off(void); // 배출 릴레이 off

void Error_Relay_On(void); // 에러  릴레이 on ,2번 
void Error_Relay_Off(void); // 에러  릴레이 off

void  Pulse_Relay_On(void); // 펄스 릴레이 on ,3번 
void  Pulse_Relay_Off(void); // 펄스  릴레이 off

void  Cdma_Power_On(void); //  cdma 전원   릴레이 on
void  Cdma_Power_Off(void); // cdma power off 

void  Junction_Power_On(void); //  ad 졍션  전원   릴레이 on
void  Junction_Power_Off(void); // ad 졍션  power off 

void  test_mode(void); // test mode
void  key_test(void);

void  display_test(void);
void  adc_test(void);
long  Get_adc_value_test_mode(void);
void  input_output_test(void);
void  cdma_test(void);

#endif	//_BUZZER_H_

/* end of file */
