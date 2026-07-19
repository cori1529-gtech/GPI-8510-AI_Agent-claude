/*
	buzzer.h
*/

#ifndef _BUZZER_H_
#define _BUZZER_H_

#define BUZZER_OUT_PORT  	GPIOC		//GPIOB
#define BUZZER_OUT_PIN	 	GPIO_Pin_12	//GPIO_Pin_5

#define   FIRST_INPUT_ON           if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) == 0x00)   
#define   SECOND_INPUT_ON      if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) == 0x00)   
#define   THIRD_INPUT_ON          if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9) == 0x00)   
#define   FOURTH_INPUT_ON      if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 0x00)   
 

//AI AGENT

#define FIRST_INPUT_RAW()    (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) == 0x00)
#define SECOND_INPUT_RAW()   (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) == 0x00)


#define   FIRST_INPUT_OFF           if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) == 0x01)   
#define   SECOND_INPUT_OFF      if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) == 0x01)   
#define   THIRD_INPUT_OFF          if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9) == 0x01)   
#define   FOURTH_INPUT_OFF      if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 0x01)   


#define  SILO_ONE    1
#define  SILO_TWO    2
#define  ERROR       3
#define  PULSE       4




#define    FIRST_RELAY _ON                      GPIO_SetBits(GPIOD, GPIO_Pin_11);  
#define    SECOND_RELAY _ON                     GPIO_SetBits(GPIOD, GPIO_Pin_12);  
#define    THIRD_RELAY _ON                      GPIO_SetBits(GPIOD, GPIO_Pin_13);  
#define    FOURTH_RELAY _ON                     GPIO_SetBits(GPIOB, GPIO_Pin_4);  


#define    FIRST_RELAY _OFF                     GPIO_ResetBits(GPIOD, GPIO_Pin_11);  
#define    SECOND_RELAY _OFF                    GPIO_ResetBits(GPIOD, GPIO_Pin_12);  
#define    THIRD_RELAY _OFF                     GPIO_ResetBits(GPIOD, GPIO_Pin_13);  
#define    FOURTH_RELAY _OFF                     GPIO_ResetBits(GPIOB, GPIO_Pin_4);  








 
 /*
if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) == 0x00)
   GPIO_SetBits(GPIOD, GPIO_Pin_11);
  else
		GPIO_ResetBits(GPIOD, GPIO_Pin_11);

 if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) == 0x00)
   GPIO_SetBits(GPIOD, GPIO_Pin_12);
  else
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);

if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9) == 0x00)
   GPIO_SetBits(GPIOD, GPIO_Pin_13);
  else
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);

if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 0x00)
    GPIO_SetBits(GPIOB, GPIO_Pin_4);
 else
		GPIO_ResetBits(GPIOB, GPIO_Pin_4);
	*/














void Gpi8510_Relay_On(char number);
void Gpi8510_Relay_Off(char number);


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

void  Battery_Power_On(void); //  battery power on
void  Battery_Power_Off(void); //  battery  power off 





void  Junction_Power_On(void); //  ad 졍션  전원   릴레이 on
void  Junction_Power_Off(void); // ad 졍션  power off 

void  test_mode(void); // test mode
void  key_test(void);

void  display_test(void);
void  adc_test(void);
long  Get_adc_value_test_mode(void);
void  input_output_test(void);
void  cdma_test(void);
void  RS422_test(void);
void  RS232_test(void);




void  First_Relay_On(void);
void  Second_Relay_On(void);
void  Third_Relay_On(void);
void  Fourth_Relay_On(void);
void  First_Relay_Off(void);
void  Second_Relay_Off(void);
void  Third_Relay_Off(void);
void  Fourth_Relay_Off(void);
void  All_Relay_Off(void);




#endif	//_BUZZER_H_

/* end of file */
