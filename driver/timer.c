
#include <stdio.h>

#include "stm32f10x.h"
#include "hw_config.h"
#include "timer.h"
#include "global.h"
#include "var.h"
#include "key.h"



unsigned long Zero_Mili_Counter;
unsigned long Feeding_Counter1; //¹èÃâ·® °è»êÁÖ±â Å¸ÀÌ¸Ó
unsigned long Feeding_Counter2; //¹èÃâ·® °è»êÁÖ±â Å¸ÀÌ¸Ó


unsigned long  Ten_Mili_Counter;
unsigned long Sec_Mili_Counter;
unsigned long Sending_Mili_Counter;
unsigned long Led_Mili_Counter;
unsigned long Error_Led_Mili_Counter;

unsigned long Led_Mili_Counter_2;
unsigned long Error_Led_Mili_Counter_2;
unsigned long  Pulse_timer=0;



/*********** RTC***************************/
u8 Settimebuf[7],Gettimebuf[7];


void Delay(unsigned int nCount) 
{ 
  while (nCount != 0) 
  { 
    nCount--; 
  } 
} 



void Clock_Pin_Init(void) 
{ 
   // GPIO_Init_TypeDef GPIO_InitStructure; 
	  EXTI_InitTypeDef EXTI_InitStructure;
  	GPIO_InitTypeDef GPIO_InitStructure;

	
	
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //_LOW_SLOW; 
    GPIO_InitStructure.GPIO_Pin = DS7_SCLK; 
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PBPORT, &GPIO_InitStructure); 
    
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//_LOW_FAST;//GPIO_MODE_OUT_OD_HIZ_FAST ; 
    GPIO_InitStructure.GPIO_Pin = DS6_IO; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(PBPORT, &GPIO_InitStructure); 
     
    
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//_LOW_SLOW; 
    GPIO_InitStructure.GPIO_Pin = DS5_RST; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(PBPORT, &GPIO_InitStructure); 
	
	


}


#define DATA_DELAY 10



void W8bit_HT1381(u8 wrdata) 
{ 
    u8 i; 
	Rtc_Write(); // 1201
  for(i=0; i<8; i++) 
  { 
        if(wrdata&0x01){GPIO_SetBits(PBPORT,DS6_IO );} 
        else{GPIO_ResetBits(PBPORT,DS6_IO );} 
        Delay(DATA_DELAY); 
        GPIO_SetBits(PBPORT,DS7_SCLK);         
      Delay(DATA_DELAY); 
        GPIO_ResetBits(PBPORT,DS7_SCLK); 
        Delay(DATA_DELAY); 
        wrdata >>= 1; 
  } 
} 
u8 R8bit_HT1381(void) 
{ 
    u8 i,rddata=0; 
    GPIO_SetBits(PBPORT,DS6_IO );
   Rtc_Read();	
  for(i=0; i<8; i++) 
  { 
        rddata >>=1; 
        Delay(DATA_DELAY); 
       // if ((GPIO_ReadInputData(PBPORT) & DS6_IO) != (u8)0x00) 
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) != 0x00)
        { rddata |=0x80; } 
        GPIO_SetBits(PBPORT,DS7_SCLK);         
      Delay(DATA_DELAY); 
        GPIO_ResetBits(PBPORT,DS7_SCLK); 
        Delay(DATA_DELAY); 
         
  } 
  return(rddata); 
}

	void Write1381(u8 Addr, u8 Wdata) 
{ 
    GPIO_ResetBits(PBPORT,DS5_RST); 
    Delay(DATA_DELAY); 
    GPIO_ResetBits(PBPORT,DS7_SCLK); 
    Delay(DATA_DELAY); 
    
	  GPIO_SetBits(PBPORT,DS5_RST); 
    W8bit_HT1381(Addr); 
    W8bit_HT1381(Wdata); 
    GPIO_SetBits(PBPORT,DS7_SCLK); 
    Delay(DATA_DELAY); 
    GPIO_ResetBits(PBPORT,DS7_SCLK); 
    Delay(DATA_DELAY); 
    GPIO_ResetBits(PBPORT,DS5_RST); 
} 
u8 Read1381(u8 Addr) 
{ 
    u8 rddata; 
    GPIO_ResetBits(PBPORT,DS5_RST); 
    Delay(DATA_DELAY); 
    GPIO_ResetBits(PBPORT,DS7_SCLK); 
    Delay(DATA_DELAY); 
   
	 GPIO_SetBits(PBPORT,DS5_RST); 
    Delay(DATA_DELAY); 
    W8bit_HT1381(Addr); 
    rddata = R8bit_HT1381(); 
    Delay(DATA_DELAY); 
    GPIO_SetBits(PBPORT,DS7_SCLK);         
    Delay(DATA_DELAY); 
    GPIO_ResetBits(PBPORT,DS7_SCLK); 
    Delay(DATA_DELAY); 
    GPIO_ResetBits(PBPORT,DS5_RST); 
  return(rddata); 
} 

void Set1381(u8 *pSecDa) 
{ 
    u8 i; 
    u8 Addr = 0x80; 
    Write1381(0x8e,0x00); /* Íóð¤Ù¤Öµ,WP=0,?ðÃíÂ?*/ 
    for(i =7;i>0;i--) 
    { 
        Write1381(Addr,*pSecDa); /* õ© ÝÂ ? ìí êÅ àøÑ¢ Ò´ */ 

        pSecDa++; 
        Addr +=2; 
    } 
    Write1381(0x8e,0x80); /* Íóð¤Ù¤Öµ,WP=1,?ÜÁ??*/ 
} 
/******************************************************************** 
* 
* Ù£?: v_Get1381 
* ?Ù¥: 
* ÍíÒö: ?ö¢HT1381?îñ?? 
* ?éÄ: uc_R1381() 
* ?ìý: ucCurtime: ÜÁðí?îñ??ò¢ò£¡£?îñ??Ì«ãÒ?: õ© ÝÂ ? ìí êÅ àøÑ¢ Ò´ 
* 7Byte (BCD?) 1B 1B 1B 1B 1B 1B 1B 
* Ú÷üÞ?: Ùé 
***********************************************************************/ 
void Get1381(void) 
{ 
    u8 i; 
    u8 Addr = 0x81; 
    for (i=0;i<7;i++) 
    { 
        Gettimebuf[i] = Read1381(Addr);/*Ì«ãÒ?: õ© ÝÂ ? ìí êÅ 
        àøÑ¢ Ò´ */ 
        Addr += 2; 
    } 
} 







/*********** RTC***************************/




//unsigned int  Minute;


timer_service_function_type gbl_ar_timer_service[timerServiceFunctionMAX] = 
{
	{timer1ServiceFunction, NULL},
	{timer2ServiceFunction, NULL}
};
 
/* ------------------------------------------------------------------------------------------------- */
/* BSP Timer */
/* ------------------------------------------------------------------------------------------------- */

void register_timer_function(timer_register_function_type timer_fn_type, timer_register_function fn)
{
	gbl_ar_timer_service[timer_fn_type].run = fn;
}

//10ms timer




void TIM2_IRQHandler(void) //1121
{
	// Also cleared the wrong interrupt flag in the ISR
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	// TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // Clear the interrupt flag
	
   /// GPIO_WriteBit(GPIOC, GPIO_Pin_0, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_0)));
///	GPIO_WriteBit(GPIOC, GPIO_Pin_1, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_1)));
	Zero_Mili_Counter++;
	Ten_Mili_Counter++;
	Sec_Mili_Counter++;
	Sending_Mili_Counter++;
	Led_Mili_Counter++;
	Error_Led_Mili_Counter++;
	Led_Mili_Counter_2++;
 Error_Led_Mili_Counter_2++;
	
 Pulse_timer++;
	
	
 Feeding_Counter1++; //¹èÃâ·® °è»êÁÖ±â Å¸ÀÌ¸Ó
 Feeding_Counter2++; //¹èÃâ·® °è»êÁÖ±â Å¸ÀÌ¸Ó



	
	
	
	
	if(KeyCheckTimer) --KeyCheckTimer;
	
	if(monitoring_timer_ms) --monitoring_timer_ms;
	if(monitoring_set_timer_ms) --monitoring_set_timer_ms;
	if(tcpip_send_idle_timer_ms) --tcpip_send_idle_timer_ms;
	if(bachul_send_timer_ms) --bachul_send_timer_ms;	//¹èÃâÁß µ¥ÀÌÅÍ Àü¼Û
	if(cdma_response_wait_timer_ms) --cdma_response_wait_timer_ms;	//cdma ÀÀ´ä ±â´Ù¸®´Â Å¸ÀÌ¸Ó
	if(cdma_interval_timer_ms) --cdma_interval_timer_ms; //cdma °£°Ý

}
   
  


void bsp_init_timer2(void/*isr_function timer2_isr*/)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;


	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);		

	/* 10msc ¸¶´Ù ÀÎÅÍ·´Æ® ¹ß»ý ÇÏµµ·Ï ¼³Á¤ */
	// 10KHz = 1usec
	// 1Hz = 1sec, 10Hz = 100msec, 100Hz = 10msec, 1KHz = 1msec, 10KHz = 1usec, 100Khz = 0.1usec, 1MHz = 0.01usec

	// TIM2CLK = 72 MHz( APB1Àº ¿ø·¡ 36MHz ÀÎµ¥ ºÐÁÖ¸¦ ÇØ¼­ »ç¿ëÇÏ¹Ç·Î 36*2 = 72MHz °¡ µÈ´Ù. )
	// ½Ã°£ÀÇ  ±âº»´ÜÀ§ :S(ÃÊ)-->nS.uS.mS.S.
	// 72000000/59999+1=1200 Áï 1ÃÊ¿¡ 1200¹ø Å¬·°ÀÌ ¹ß»ýÇÏ¹Ç·Î 1199+1 ¹ø¿¡ ÇÑ¹ø ÀÎÅÍ·´Æ®°¡ 
	// ¹ß»ýÇÏµµ·Ï ¼³Á¤ÇÏ¸é 1ÃÊ¿¡ ÇÑ¹ø ÀÎÅÍ·´Æ®°¡ ¹ß»ý ÇÏµµ·Ï ÇÑ´Ù.


	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 12-1;	// ARR(Auto reload register)
	TIM_TimeBaseStructure.TIM_Prescaler = 59999;	// 1MHz = 0.01usec
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 





	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);	

	/* TIM IT enable */
	TIM_ITConfig(TIM2, TIM_IT_Update , ENABLE);	
}


u16 bsp_get_timer2_cnt(void)
{
	return TIM2->CNT;
}

void bsp_set_timer2_cnt( u16 cnt)
{
	TIM2->CNT = cnt;
}

