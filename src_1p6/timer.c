
#include <stdio.h>

#include "stm32f10x.h"
#include "hw_config.h"
#include "timer.h"
#include "global.h"
#include "var.h"



unsigned long  Ten_Mili_Counter;
unsigned long Sec_Mili_Counter;
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
	
//	gnlTick++;
/*
	if( gbl_ar_timer_service[timer2ServiceFunction].run != NULL )
	{
		gbl_ar_timer_service[timer2ServiceFunction].run();
	}
*/	
	Ten_Mili_Counter++;
	Sec_Mili_Counter++;
	
	if(monitoring_timer_ms) --monitoring_timer_ms;
	if(monitoring_set_timer_ms) --monitoring_set_timer_ms;
	if(tcpip_send_idle_timer_ms) --tcpip_send_idle_timer_ms;
	if(bachul_send_timer_ms) --bachul_send_timer_ms;	//배출중 데이터 전송
	if(cdma_response_wait_timer_ms) --cdma_response_wait_timer_ms;	//cdma 응답 기다리는 타이머
	if(cdma_interval_timer_ms) --cdma_interval_timer_ms; //cdma 간격

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

	/* 10msc 마다 인터럽트 발생 하도록 설정 */
	// 10KHz = 1usec
	// 1Hz = 1sec, 10Hz = 100msec, 100Hz = 10msec, 1KHz = 1msec, 10KHz = 1usec, 100Khz = 0.1usec, 1MHz = 0.01usec

	// TIM2CLK = 72 MHz( APB1은 원래 36MHz 인데 분주를 해서 사용하므로 36*2 = 72MHz 가 된다. )
	// 시간의  기본단위 :S(초)-->nS.uS.mS.S.
	// 72000000/59999+1=1200 즉 1초에 1200번 클럭이 발생하므로 1199+1 번에 한번 인터럽트가 
	// 발생하도록 설정하면 1초에 한번 인터럽트가 발생 하도록 한다.


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

