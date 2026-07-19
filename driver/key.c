

#include <stdio.h>

#include <stm32f10x.h>
#include "key.h"
#include "board_gpi8510.h"

u16   KEYCODE;
u8	  qKEYIN;
u8	  nKEYIN;
u8	  rKEYIN;
#define	KEYCHECKTIME	3

volatile char key_flag;
volatile int key_count;


volatile char NewKeyIn;
//volatile char ReleaseKeyIn;
volatile char PressKeyIn;
volatile char PreKeyIn;

volatile u16	KeyCode;
volatile u16	KeyCodePre;

volatile u16	KeyCheckTimer = KEYCHECKTIME;



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

/*********   128 * 64 LCD  **************************/

GPIO_InitStructure.GPIO_Pin =    LCD_D0_PIN | LCD_D1_PIN | LCD_D2_PIN | LCD_D3_PIN | LCD_D4_PIN | LCD_D5_PIN | 
                                 LCD_D6_PIN | LCD_D7_PIN | LCD_CS1_PIN | LCD_CS2_PIN | LCD_E_PIN |
								 LCD_RW_PIN | LCD_DI_PIN;

GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(LCD_DATA_PORT, &GPIO_InitStructure);


GPIO_InitStructure.GPIO_Pin = LCD_RESET_PIN;//��   
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(LCD_RESET_PORT, &GPIO_InitStructure);




/**************128 * 64 LCD*********************/


 /*  eeprom */
  GPIO_InitStructure.GPIO_Pin =  EEPROM_SCL_PIN | EEPROM_SDA_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(EEPROM_SCL_PORT, &GPIO_InitStructure);


 /*  eeprom */


  /* weight scale -> CN6  */
 GPIO_InitStructure.GPIO_Pin = LC3_DOUT_PIN | LC3_2_DOUT_PIN;		   // C.2 : DOUT
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	   //GPIO_Mode_IPU	GPIO_Mode_IN_FLOATING
 GPIO_Init(LC3_DOUT_PORT, &GPIO_InitStructure);
 
 
  
  // GPIOC Configuration:Pin  left loadcell 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | LC3_SCLK_PIN ;	  // C.1 : CLK (unused, no macro) C0 : SCLK
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LC3_SCLK_PORT, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = LC3_2_SCLK_PIN ;	  // B1 : SCLK
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LC3_2_SCLK_PORT, &GPIO_InitStructure);
	
	
	
	
	
//2022�� 3��, ���� ads1251	
	
	/***************CS5555*************************/
  // LC1
  GPIO_InitStructure.GPIO_Pin =LC1_DOUT_PIN;  //  DOUT 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(LC1_DOUT_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =LC1_SCLK_PIN  ;  // sclk
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(LC1_SCLK_PORT, &GPIO_InitStructure);

  // LC2
  GPIO_InitStructure.GPIO_Pin =LC2_DOUT_PIN;  //  DOUT 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(LC2_DOUT_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =LC2_SCLK_PIN  ;  // sclk
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(LC2_SCLK_PORT, &GPIO_InitStructure);
/***************CS5555*************************/

	
	
	
	
	
	
	
	/* weight scale -> CN6  */


// STATUS LED  silo ����,  siol���� 

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9 |GPIO_Pin_10 ;//��   
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOF, &GPIO_InitStructure);
// STATUS LED

// KEY INPUT
 GPIO_InitStructure.GPIO_Pin = KEY5_PIN | KEY6_PIN | KEY7_PIN | KEY8_PIN | KEY9_PIN;		   // C.2 : DOUT
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	   //INTERNAL PULL UP
 GPIO_Init(KEY5_PORT, &GPIO_InitStructure);
// KEY INPUT


// BUZZER  ���  
GPIO_InitStructure.GPIO_Pin = BUZZER_PIN ;   
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(BUZZER_PORT, &GPIO_InitStructure);
// BUZZER  ��� 

 
// JUNCTION POWER ON/OFF  ���  
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;   
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOD, &GPIO_InitStructure);
// JUNCTION POWER ON/OFF  ��� 

// CDMA POWER ON/OFF  ���  
GPIO_InitStructure.GPIO_Pin = MODEM_PWR_PIN ;   
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(MODEM_PWR_PORT, &GPIO_InitStructure);
// CDMA POWER ON/OFF  ��� 


// BAT POWER ON/OFF  ���  
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;   
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOD, &GPIO_InitStructure);
// BAT POWER ON/OFF  ��� 


// POWER  üũ  �Է�  
 GPIO_InitStructure.GPIO_Pin = BAT_CHECK_PIN;		   
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	   
 GPIO_Init(BAT_CHECK_PORT, &GPIO_InitStructure);
// POWER  üũ  �Է�  



//input
 GPIO_InitStructure.GPIO_Pin = IN1_PIN | IN2_PIN | IN3_PIN;		   
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	   
 GPIO_Init(IN1_PORT, &GPIO_InitStructure);

 GPIO_InitStructure.GPIO_Pin = IN4_PIN;		   
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	   
 GPIO_Init(IN4_PORT, &GPIO_InitStructure);
//input

//output

GPIO_InitStructure.GPIO_Pin = OUT1_PIN |  OUT2_PIN | OUT3_PIN;   
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(OUT1_PORT, &GPIO_InitStructure);

GPIO_InitStructure.GPIO_Pin = OUT4_PIN;   
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(OUT4_PORT, &GPIO_InitStructure);

//output






 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);	 // JTAG ������� �ʴ� Remapping
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	 // JTAG ������� �ʴ� Remapping






  /****************GPI -8510 (2017��)*******************************/


/*
	// key ��� 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	// key �Է� 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7|GPIO_Pin_8 |GPIO_Pin_9|GPIO_Pin_10 ;// gl-150c
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
 
  // relay ��� 
  GPIO_InitStructure.GPIO_Pin =      GPIO_Pin_5 | GPIO_Pin_8 |GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode =     GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed =    GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // relay ���   CDMA ���� ON/OFF �����̿� 
  GPIO_InitStructure.GPIO_Pin =      GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode =     GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed =    GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  // relay ��� ���� ����  ���� ON/OFF �����̿� 
  GPIO_InitStructure.GPIO_Pin =      GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode =     GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed =    GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);



  // LED ���
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
	 
	///GPIO_PinRemapConfig(GPIO_Remap_FSMC_NADV, ENABLE);			 
}



void Rtc_Write(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;   
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Rtc_Read(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;   
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
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



char keypressed(void)
{
	KEYINENC();

	if (KEYCODE == 0) return 0;
	else return 1;   //return -1;
} /* of keypressed() */


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
// 0000 0011 1110 0000 
// 

/*
void CHKEYIN()  // 0808
{
	NewKeyIn = 0;

	KeyCode = GPIO_ReadInputData(GPIOE) & 0x03E0;
	if(KeyCode == KeyCodePre)
	{
		if(!KeyCheckTimer)
		{
			if(KeyCode == 0x03E0) 
			{
				PressKeyIn = 0;         //not key
			}
			else 
			{
				//ReleaseKeyIn = 0;
				if(!PressKeyIn) NewKeyIn = 1;  	// new key pressed
				PressKeyIn = 1;
			}
		}
	}
	else KeyCheckTimer = KEYCHECKTIME;
	
	KeyCodePre = KeyCode;
	//PreKeyIn = PressKeyIn;
}

*/


//if (!PressKeyIn || !NewKeyIn) return 0;  /* not key in  */ /* not new key */

void CHKEYIN()  // 0829
{
	 u8 sT0[10];
	NewKeyIn = 0;

//KeyCode = GPIO_ReadInputData(GPIOE) & 0x03E0;   //      0000  0011 1110 0000   -> 0000 0001 1111 0000       0x01f0 
  
	KeyCode = GPIO_ReadInputData(GPIOE) & 0x01f0;   //   0000  0001 1111 0000       0x01f0 
 //  	KeyCode = GPIO_ReadInputData(GPIOE) & 0xffff;   //   0000  0001 1111 0000       0x01f0 

	KeyCode<<=1;
	
	sprintf(sT0, " %X",KeyCode); //	DebugString(msg);
	// GLCD_string(0,4, sT0); Delay_ms(1000);
	
	
	
		if(KeyCode == 0x03E0) 
			{
				PressKeyIn = 0;         //not key
			}
	 else 
	 {
		 PressKeyIn = 1;
	 }
	 
	 if(PressKeyIn)
	 {
		 if(KeyCode == KeyCodePre)
      NewKeyIn = 0;
		 else
			 NewKeyIn = 1;
		 
	 }
	
	
/*	
	//if(KeyCode == KeyCodePre)
	{
			if(KeyCode == 0x03E0) 
			{
				PressKeyIn = 0;         //not key
			}
	
			else 
			{
				//ReleaseKeyIn = 0;
				if(!PressKeyIn) NewKeyIn = 1;  	// new key pressed
				PressKeyIn = 1;
			}
	} // of  if(KeyCode == KeyCodePre)
	*/
	KeyCodePre = KeyCode;
	
	//PreKeyIn = PressKeyIn;
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

 
 
 void key_init()
{
	PressKeyIn = 0;
	PreKeyIn = 0;
	
	KeyCode = 0;
	KeyCodePre = 0;
	
	KeyCheckTimer = KEYCHECKTIME;
}
 
 
 
 
 
 
 
 
 
void Fiveth_digit_On(void)
{
  GPIO_ResetBits(GPIOG, GPIO_Pin_10);
  GPIO_ResetBits(GPIOG, GPIO_Pin_11);
  GPIO_ResetBits(GPIOG, GPIO_Pin_12);
  GPIO_ResetBits(GPIOG, GPIO_Pin_13);
  GPIO_SetBits(GPIOG, GPIO_Pin_14);
 
 }

 //              0000 0001 1111 0000 
//  UP         0000 0000 1111 0000
 // DOWN    0000 0001  1110 000
 // LEFT      0000 0001  0111 0000
 // RIGHT
 // ENTER

 
 unsigned char    keypush_8510(void) //0808
{
		CHKEYIN();	 //check key in

	if (!PressKeyIn || !NewKeyIn) return 0;  /* not key in  */ /* not new key */
//	if (!PressKeyIn) return 0;  /* not key in  */ /* not new key */
// 	if (!NewKeyIn) return 0;  /* not key in  */ /* not new key */
/*
	if( KeyCode==0x01E0 )	 KeyCode = UP;
	else if(KeyCode==0x03C0) KeyCode = DOWN;
	else if(KeyCode==0x0360) KeyCode = ENTER;
	else if(KeyCode==0x02E0) KeyCode = LEFT;
	else if(KeyCode==0x03A0) KeyCode = RIGHT;
	
	0x03E0
	0000 0011 1110 0000
  0000 0011 0110 0000	  0000 000
*/
	
	if( KeyCode==0x02E0 )	 KeyCode = UP;
	else if(KeyCode==0x03A0) KeyCode = DOWN;
	else if(KeyCode==0x01E0) KeyCode = ENTER;
	else if(KeyCode==0x0360) KeyCode = LEFT;
	else if(KeyCode==0x03c0) KeyCode = RIGHT;
	else if(KeyCode==0x00E0) KeyCode=DOUBLE; // UP +ENTER
	
	
	
	beep(10); //delay(20);
	return 1;
} // of  keypush_8510(void)

 
 
 
 
