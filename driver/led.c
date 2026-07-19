

#include  <stm32f10x.h>
#include "led.h"
#include "timer.h"      /* Led_Mili_Counter, Led_Mili_Counter_2,
                           Error_Led_Mili_Counter, Error_Led_Mili_Counter_2 */


const bsp_led_core_group_type led_core_group[ledCoreMax]=
{
	{LED_USER_CORE_PORT, LED_USER_CORE_PIN }
};

const bsp_led_bottom_group_type led_bottom_group[ledBottomMax]=
{
	{LED_USER_BOTTOM1_PORT, LED_USER_BOTTOM1_PIN },
	{LED_USER_BOTTOM2_PORT, LED_USER_BOTTOM2_PIN },
	{LED_USER_BOTTOM3_PORT, LED_USER_BOTTOM3_PIN }
};


void bsp_led_core_on(BSP_LED_CORE_Def led)
{
	if( led >= ledCoreMax )
		return;

	GPIO_SetBits(led_core_group[led].gpio_reg, led_core_group[led].pin);
}

void bsp_led_core_off(BSP_LED_CORE_Def led)
{
	if( led >= ledCoreMax )
		return;

	GPIO_ResetBits(led_core_group[led].gpio_reg, led_core_group[led].pin);
}

void bsp_led_core_toggle(BSP_LED_CORE_Def led)
{
	s32 pins;
	if( led >= ledCoreMax )
		return;

	pins = GPIO_ReadOutputData(led_core_group[ led ].gpio_reg);
	if ((pins & led_core_group[led].pin) == 0)
	{
		GPIO_SetBits(led_core_group[led].gpio_reg, led_core_group[led].pin);
	}
	else
	{
		GPIO_ResetBits(led_core_group[led].gpio_reg, led_core_group[led].pin);
	}
}

void bsp_led_bottom_on(BSP_LED_BOTTOM_Def led)
{
	if( led >= ledBottomMax )
		return;

	GPIO_SetBits(led_bottom_group[led].gpio_reg, led_bottom_group[led].pin);
}

void bsp_led_bottom_off(BSP_LED_BOTTOM_Def led)
{
	if( led >= ledBottomMax )
		return;

	GPIO_ResetBits(led_bottom_group[led].gpio_reg, led_bottom_group[led].pin);
}

void bsp_led_bottom_toggle(BSP_LED_BOTTOM_Def led)
{
	s32 pins;
	if( led >= ledBottomMax )
		return;

	pins = GPIO_ReadOutputData(led_bottom_group[ led ].gpio_reg);
	if ((pins & led_bottom_group[led].pin) == 0)
	{
		GPIO_SetBits(led_bottom_group[led].gpio_reg, led_bottom_group[led].pin);
	}
	else
	{
		GPIO_ResetBits(led_bottom_group[led].gpio_reg, led_bottom_group[led].pin);
	}
}
/*
 void  Lc1_led_On(void)
  {
   GPIO_SetBits(GPIOE, GPIO_Pin_2);
  }

 void Lc2_led_On(void)
  {
   GPIO_SetBits(GPIOE, GPIO_Pin_0);
  }

 void Lc3_led_On(void)
  {
   GPIO_SetBits(GPIOA, GPIO_Pin_8);
  }

 void Lc4_led_On(void)
  {
   GPIO_SetBits(GPIOC, GPIO_Pin_6);
  }

 void Lc5_led_On(void)
  {
   GPIO_SetBits(GPIOB, GPIO_Pin_15);
  }

 void Lc6_led_On(void)
  {
   GPIO_SetBits(GPIOB, GPIO_Pin_14);
  }

 void Er1_led_On(void)
  {
   GPIO_SetBits(GPIOB, GPIO_Pin_13);
  }
 void Er2_led_On(void)
  {
   GPIO_SetBits(GPIOB, GPIO_Pin_12);
  }


 void  Lc1_led_Off(void)
  {
   GPIO_ResetBits(GPIOE, GPIO_Pin_2);
  }

 void Lc2_led_Off(void)
  {
   GPIO_ResetBits(GPIOE, GPIO_Pin_0);
  }

 void Lc3_led_Off(void)
  {
   GPIO_ResetBits(GPIOA, GPIO_Pin_8);
  }

 void Lc4_led_Off(void)
  {
   GPIO_ResetBits(GPIOC, GPIO_Pin_6);
  }

 void Lc5_led_Off(void)
  {
   GPIO_ResetBits(GPIOB, GPIO_Pin_15);
  }

 void Lc6_led_Off(void)
  {
   GPIO_ResetBits(GPIOB, GPIO_Pin_14);
  }

 void Er1_led_Off(void)
  {
   GPIO_ResetBits(GPIOB, GPIO_Pin_13);
  }
 void Er2_led_Off(void)
  {
   GPIO_ResetBits(GPIOB, GPIO_Pin_12);
  }
*/

 void Silo1_led_On(void)
  {
   GPIO_ResetBits(GPIOF, GPIO_Pin_7);
  }

 void Silo2_led_On(void)
   {
   GPIO_ResetBits(GPIOF, GPIO_Pin_9);
  }

 void Silo1_Error_led_On(void)
   {
   GPIO_ResetBits(GPIOF, GPIO_Pin_8);
  }

 void Silo2_Error_led_On(void)
   {
   GPIO_ResetBits(GPIOF, GPIO_Pin_10);
  }

  void All_led_On(void)
	{
		Silo1_led_On();
		Silo2_led_On();
		Silo1_Error_led_On();
		Silo2_Error_led_On();
	}

  void All_led_Off(void)
	{
		Silo1_led_Off();
		Silo2_led_Off();
		Silo1_Error_led_Off();
		Silo2_Error_led_Off();
	}


 void Silo1_led_Off(void)
  {
   GPIO_SetBits(GPIOF, GPIO_Pin_7);
  }

 void Silo2_led_Off(void)
  {
   GPIO_SetBits(GPIOF, GPIO_Pin_9);
  }

 void Silo1_Error_led_Off(void)
  {
   GPIO_SetBits(GPIOF, GPIO_Pin_8);
  }

 void Silo2_Error_led_Off(void)

  {
   GPIO_SetBits(GPIOF, GPIO_Pin_10);
  }


/*******************************************************************************
 * Below: moved from main.c (Day 2 driver extraction, see prj/CLAUDE.md 10.2).
 * These wrap the raw on/off calls above with the blink-timing logic that used
 * to live inline in main.c. Pure move, no behavior change.
 ******************************************************************************/

void  Silo_1_Led_Operation()
{
	 if(Led_Mili_Counter>100)
	    Silo1_led_On();
	 if(Led_Mili_Counter>200)
	 {
		 Silo1_led_Off();
		 Led_Mili_Counter=0;
	 }


}

void  Silo_2_Led_Operation()
{
	 if(Led_Mili_Counter_2>100)
	    Silo2_led_On();
	 if(Led_Mili_Counter_2>200)
	 {
		 Silo2_led_Off();
		 Led_Mili_Counter_2=0;
	 }
}

void Error_1_Led_Operation()
{
	 if(Error_Led_Mili_Counter>50)
	     Silo1_Error_led_On();
	 if(Error_Led_Mili_Counter>100)
	 {
		  Silo1_Error_led_Off();
		 Error_Led_Mili_Counter=0;
	 }
}

void Error_2_Led_Operation()
{
	 if(Error_Led_Mili_Counter_2>50)
	     Silo2_Error_led_On();
	 if(Error_Led_Mili_Counter_2>100)
	 {
		  Silo2_Error_led_Off();
		 Error_Led_Mili_Counter_2=0;
	 }
}
