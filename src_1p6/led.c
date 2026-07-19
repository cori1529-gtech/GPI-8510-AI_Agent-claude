

#include  <stm32f10x.h>
#include "led.h"


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



