/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#//include "stm32f10x_nvic.h"

#include "hw_config.h"
#include "platform_config.h"

#include "led.h"
//#include "mmc_sd.h"
#include "key.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;
// EXTI_InitTypeDef EXTI_InitStructure;

/* Extern variables ----------------------------------------------------------*/
//static u8  fac_us=0;
//static u16 fac_ms=0;


/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power.
* Input          : None.
* Return         : None.
*******************************************************************************/

#define	___CLOCK__HSE___	0

void bsp_init_rcc(void)
{   
//#ifdef _____NOT_USED_IAP___
	/* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/ 
	
/*	
	 
  assert_param(IS_SYSTICK_CLK_SOURCE(SysTick_CLKSource));

  if (SysTick_CLKSource == SysTick_CLKSource_HCLK)
  {
    SysTick->CTRL |= SysTick_CLKSource_HCLK;
  }
  else
  {
    SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;
  }  
  */
	/* RCC system reset(for debug purpose) */
///	RCC_DeInit();


#ifdef ___CLOCK__HSE___
	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);
	
	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	
	if(HSEStartUpStatus == SUCCESS)
	{
#else
	/* Enable HSI */
	RCC_HSIConfig(RCC_HSI_ON);
	
	/* Wait till HSI is ready */
	HSIStartUpStatus = RCC_WaitForHSIStartUp();
	
	if(HSIStartUpStatus == SUCCESS)
	{
#endif

#ifdef ___fjdlfj__
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		
		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);
#endif /#ifdef ___fjdlfj__
		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 
		
		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1); 
		
		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);
		
		/* PLLCLK = 8MHz * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		
		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);
		
		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}
		
		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}
//#endif	//#ifdef _____NOT_USED_IAP___


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
  	/* AFIO Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
	/* TIM2 clock enable  for timer base  */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* TIM3 clock enable  for timer base  */	// ÃÊÀ½ÆÄ 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* TIM8 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	
	/* Enable USART1 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	/* Enable USART2 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	/* Enable USART3 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	/* Enable DMA1, DMA2 clock ENABLE */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, ENABLE);
	
	/* Enable ADC1 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC3, ENABLE);
	
	/* I2C1 Periph clock enable   EEPROM */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	/* DAC Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	//USART_USER_Initialization(); // at stm32f10x_usart.c
}



void bsp_init_gpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* AT24C16 EEPROM Configure I2C1 pins: SCL and SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void bsp_init_interrupt(void)
{

	NVIC_InitTypeDef NVIC_InitStructure;

	/* Set the Vector Table base address at 0x08000000 */
	//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);

	/* 1 bit for pre-emption priority, 3 bits for subpriority */
	/* 101111110100000000000000000 & */
	/* 				   11000000000	 */	
	/* SCB->AIRCR = 101111110100000011000000000 */


//#ifdef  VECT_TAB_RAM  
	/* Set the Vector Table base location at 0x20000000 */ 
//	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
//#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */ 
	////NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000);   
//#endif

	/* 2 bit for pre-emption priority, 2 bits for subpriority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  



#ifdef STM32F10X_CL
	/* Enable the USB Interrupts */
	NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the USB Wake-up interrupt */
	// NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_WKUP_IRQn;
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	// NVIC_Init(&NVIC_InitStructure);  

#else
	/* Enable the USB interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the USB Wake-up interrupt */
	// NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_IRQn;
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	// NVIC_Init(&NVIC_InitStructure);   
#endif /* STM32F10X_CL */

	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}

// JTAG mode, the mode used to set the JTAG
void JTAG_Set (u8 mode)
{
	u32 temp;
	temp = mode;
	temp <<= 25;
	RCC->APB2ENR |= 1 <<0; // Open the secondary clock
	AFIO->MAPR &= 0XF8FFFFFF; // clear the MAPR's [26:24]
	AFIO->MAPR |= temp; // set the jtag mode
}
/*
void Delay_us (const uint32_t usec)
{
	RCC_ClocksTypeDef  RCC_Clocks;  


	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

	RCC_GetClocksFreq(&RCC_Clocks);

	SysTick_Config(usec * (RCC_Clocks.HCLK_Frequency / 1000000));  

	SysTick->CTRL  &= ~SysTick_CTRL_TICKINT_Msk ;

	while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
}
*/

void delay_ms (const uint32_t usec)
{
	Delay_us(1000 * usec);
}

  
void Delay_ms (const uint32_t usec)
{
  u32 timer;
  u32 temp;
  temp=usec;
   
  //timer=
   while(temp--)
    {
		timer=10000;		
	   	while(timer--);
    };

}

void Delay_us (const uint32_t usec)
{
  u32 timer;
  u32 temp;
  temp=usec;
   
  //timer=
   while(temp--)
    {
		timer=10;		
	   	while(timer--);
    };

}



/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
