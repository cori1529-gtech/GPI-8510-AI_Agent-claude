

#include <stdio.h>

#include <stm32f10x.h>
#include "key.h"

u16   KEYCODE;
u8	  qKEYIN;
u8	  nKEYIN;
u8	  rKEYIN;


/*
char key_disp[53] = {-1,
                       3, 2, 10, 1, -1, -1, -1, -1, -1, -1,  
					  -1, -1, -1, -1, -1, -1,  6, 5, 11,  4,
					  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					  -1, -1,  9,  8, 12,  7, -1, -1, -1, -1,
					  -1, -1, -1, -1, -1, -1, -1, -1, 15, 0,
					   13, 14
				     };
*/

char key_disp[70] = {-1,
                      16, 17, -1, 18, -1, -1, -1, -1, -1, -1,//10  
					  -1, -1, -1, -1, -1, -1, 15, 10,  0, 11, //20
					  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //30
					  -1, -1, 14,  7,  8,  9, -1, -1, 13, -1, //40
					  -1, -1, -1, -1, -1, -1, -1, -1, 13,  4, //50
					   5,  6, -1, -1, -1, -1, -1, -1, -1, -1, //60
					  -1, -1, -1, -1, 12,  1,  2,  3, -1,  //70
					   				     };


void bsp_key_gpio_init(void)  //0123
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// key 출력 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	// key 입력 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7|GPIO_Pin_8 |GPIO_Pin_9|GPIO_Pin_10 ;// gl-150c
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
 
  // relay 출력 
  GPIO_InitStructure.GPIO_Pin =      GPIO_Pin_5 | GPIO_Pin_8 |GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode =     GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed =    GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // relay 출력   CDMA 전원 ON/OFF 릴레이용 
  GPIO_InitStructure.GPIO_Pin =      GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode =     GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed =    GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  // relay 출력 졍션 보드  전원 ON/OFF 릴레이용 
  GPIO_InitStructure.GPIO_Pin =      GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode =     GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed =    GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

/*
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_8 |GPIO_Pin_9;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
*/

  // LED 출력
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

 // GPIO_InitTypeDef GPIO_InitStructure;
  /*  eeprom */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // 외부입력   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7| GPIO_Pin_8 |GPIO_Pin_12  ;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
   GPIO_Init(GPIOC, &GPIO_InitStructure);
  // 7 : 입력 1번 , 8: 입력2번  12: 진동센서 


 /*
  GPIO_InitTypeDef GPIO_InitStructure;
  
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_8 |GPIO_Pin_9  ;
   //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
  */


	/* Configure the GPIO ports */
   /*
	GPIO_InitStructure.GPIO_Pin =  KEY1_USER_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(KEY1_USER_PORT, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin =  KEY2_USER_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(KEY2_USER_PORT, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin =  KEY3_USER_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(KEY3_USER_PORT, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin =  KEY4_USER_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(KEY4_USER_PORT, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin =  KEY5_USER_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(KEY5_USER_PORT, &GPIO_InitStructure);	
	 */

	/* Configure EXTI  to generate an interrupt on falling edge */
	/*
	EXTI_InitStructure.EXTI_Line = KEY5_IRQ_EXTI_Line;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);	
	 */
	 
	GPIO_PinRemapConfig(GPIO_Remap_FSMC_NADV, ENABLE);			 
}


char keypush(void)
{
	static pre_KEYCODE = 0;
	
	KEYINENC();
	
	if(pre_KEYCODE == KEYCODE) return 0;
	pre_KEYCODE = KEYCODE;
	
	if (KEYCODE == 0) return 0;
	buzzer_on();
	Delay_ms(150);
	buzzer_off();
//	GPIO_ResetBits(GPIOC, GPIO_Pin_0);
//	Buzzer_On();
     //   beep(9000);
//	delay(2000);
	return 1;   //return -1;
} /* of keypush() */




/*
u16 KEYINENC(void)
{
	u16   loc;
	u16   scan;


     First_digit_On(); // Delay_ms(5);Third_digit_On();
	 loc=GPIO_ReadInputData(GPIOF);
     loc&=0x07c0;
     loc>>=6;
 
   if (loc)  scan = 0x01;    // 1 4 7 c

	else
	{
	  Second_digit_On();  //Delay_ms(5);Second_digit_On();
	  loc=GPIO_ReadInputData(GPIOF);
      loc&=0x07c0;
      loc>>=6;

	  if (loc)  scan = 0x02;  // 2 5 8 0
    	else
	    {
	    
	    //KEYIN = 0x04;KEYIN = 0x04; _NOP();    loc = PIND & 0xF0;
		  Third_digit_On();  //Delay_ms(5);Third_digit_On();
	      loc=GPIO_ReadInputData(GPIOF);
          loc&=0x07c0;
          loc>>=6;
		 //if (loc) scan = 0x03;   // 3 6 9 SET
		 if (loc) scan = 0x03;   // 3 6 9 SET
	
	      else
		   {
	        Fourth_digit_On();  //Delay_ms(5);Fourth_digit_On();
	        loc=GPIO_ReadInputData(GPIOF);
            loc&=0x07c0;
            loc>>=6; 
		    if (loc) scan = 0x04; //G3 G2    G1
		   else
		    loc = scan = 0;
		   
		   }
		
	   }
	}
	
   
	switch(loc)
	{
	  	
		case 0x01:	KEYCODE = 0x00; break;
		case 0x02:	KEYCODE = 0x10; break;
		case 0x04:	KEYCODE = 0x20; break;
		case 0x08:	KEYCODE = 0x30; break;
		case 0x10:	KEYCODE = 0x00; break;
		case 0x20:	KEYCODE = 0x10; break;
		case 0x40:	KEYCODE = 0x20; break;
		case 0x80:	KEYCODE = 0x30; break;
		
		default:	KEYCODE = loc; break;
	}
	KEYCODE += scan;
	return KEYCODE;
}

  */

u16 KEYINENC(void)
{
	u16   loc;
	u16   scan;


     First_digit_On(); // Delay_ms(5);Third_digit_On();
	 loc=GPIO_ReadInputData(GPIOF);
     loc&=0x07c0;
     loc>>=6;
 
   if (loc)  scan = 0x01;    // 1 4 7 c

	else
	{
	  Second_digit_On();  //Delay_ms(5);Second_digit_On();
	  loc=GPIO_ReadInputData(GPIOF);
      loc&=0x07c0;
      loc>>=6;

	  if (loc)  scan = 0x02;  // 2 5 8 0
    	else
	    {
	    
	    //KEYIN = 0x04;KEYIN = 0x04; _NOP();    loc = PIND & 0xF0;
		  Third_digit_On();  //Delay_ms(5);Third_digit_On();
	      loc=GPIO_ReadInputData(GPIOF);
          loc&=0x07c0;
          loc>>=6;
		 //if (loc) scan = 0x03;   // 3 6 9 SET
		 if (loc) scan = 0x03;   // 3 6 9 SET
	
	      else
		   {
	        Fourth_digit_On();  //Delay_ms(5);Fourth_digit_On();
	        loc=GPIO_ReadInputData(GPIOF);
            loc&=0x07c0;
            loc>>=6; 
		    if (loc) scan = 0x04; //G3 G2    G1
		   else
		    {
			 Fiveth_digit_On();  //Delay_ms(5);Fourth_digit_On();
	        loc=GPIO_ReadInputData(GPIOF);
            loc&=0x07c0;
            loc>>=6; 
		    if (loc) scan = 0x10; //G3 G2    G1 
			   else
		    loc = scan = 0;





			}
		    //loc = scan = 0;
		   
		   }
		
	   }
	}
	
   
	switch(loc)
	{
	  	/*
		case 0x01:	KEYCODE = 0x00; break;
		case 0x02:	KEYCODE = 0x10; break;
		case 0x04:	KEYCODE = 0x20; break;
		case 0x08:	KEYCODE = 0x30; break;
		case 0x10:	KEYCODE = 0x00; break;
		case 0x20:	KEYCODE = 0x10; break;
		case 0x40:	KEYCODE = 0x20; break;
		case 0x80:	KEYCODE = 0x30; break;
		*/
        case 0x01:	KEYCODE = 0x00; break;
		case 0x02:	KEYCODE = 0x10; break;
		case 0x04:	KEYCODE = 0x20; break;
		case 0x08:	KEYCODE = 0x30; break;
		case 0x10:	KEYCODE = 0x40; break;
		case 0x20:	KEYCODE = 0x00; break;
		case 0x40:	KEYCODE = 0x10; break;
		case 0x80:	KEYCODE = 0x20; break;



		default:	KEYCODE = loc; break;
	}
	KEYCODE += scan;
	return KEYCODE;
}




unsigned char CHKEYIN(void)
{
  unsigned int   loc, loc1;
  
  loc=GPIO_ReadInputData(GPIOF);
  loc&=0x07c0;
  loc>>=6;
  Delay_ms(10);

   loc1=GPIO_ReadInputData(GPIOF);
  loc1&=0x07c0;
  loc1>>=6;
   /*
   DDRD |= 0xF0;      //pd7, 6, 5, 4 output

    PORTD |= 0xF0;    //1111 QQQQ output

	loc = KEYIN;
	delay(400);
	loc1 = KEYIN;

	*/
	if (loc != loc1) return 0;

	if (loc == 0)
	{	rKEYIN = 1;
		qKEYIN = 1;
		nKEYIN = 0;
	}
	else
	{
		if (rKEYIN == 1)
		{
			rKEYIN = 0;
			nKEYIN = 1;
		}
		else
		{
			nKEYIN = 0;
		}
		qKEYIN = 0;
	}
	
	return loc;
}




 void First_digit_On(void)
 {
  GPIO_SetBits(GPIOG, GPIO_Pin_10);
  GPIO_ResetBits(GPIOG, GPIO_Pin_11);
  GPIO_ResetBits(GPIOG, GPIO_Pin_12);
  GPIO_ResetBits(GPIOG, GPIO_Pin_13);
  GPIO_ResetBits(GPIOG, GPIO_Pin_14);
 
 }
void Second_digit_On(void)
{
  GPIO_ResetBits(GPIOG, GPIO_Pin_10);
  GPIO_SetBits(GPIOG, GPIO_Pin_11);
  GPIO_ResetBits(GPIOG, GPIO_Pin_12);
  GPIO_ResetBits(GPIOG, GPIO_Pin_13);
  GPIO_ResetBits(GPIOG, GPIO_Pin_14);
 
 }



void Third_digit_On(void)
{
 GPIO_ResetBits(GPIOG, GPIO_Pin_10);
  GPIO_ResetBits(GPIOG, GPIO_Pin_11);
  GPIO_SetBits(GPIOG, GPIO_Pin_12);
  GPIO_ResetBits(GPIOG, GPIO_Pin_13);
  GPIO_ResetBits(GPIOG, GPIO_Pin_14);
 
 }
void Fourth_digit_On(void)
{
  GPIO_ResetBits(GPIOG, GPIO_Pin_10);
  GPIO_ResetBits(GPIOG, GPIO_Pin_11);
  GPIO_ResetBits(GPIOG, GPIO_Pin_12);
  GPIO_SetBits(GPIOG, GPIO_Pin_13);
  GPIO_ResetBits(GPIOG, GPIO_Pin_14);
 
 }

void Fiveth_digit_On(void)
{
  GPIO_ResetBits(GPIOG, GPIO_Pin_10);
  GPIO_ResetBits(GPIOG, GPIO_Pin_11);
  GPIO_ResetBits(GPIOG, GPIO_Pin_12);
  GPIO_ResetBits(GPIOG, GPIO_Pin_13);
  GPIO_SetBits(GPIOG, GPIO_Pin_14);
 
 }


