
#include  <stm32f10x.h>
/************ 128*64 *****************/
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
//#include "LCD.h"
#include "LCD.h"
#include "stm32f10x_lib.h"
//#include "platform_config.h"
#include "stm32f10x_it.h"
#include "english.h"
#include "graphic.h"
#include "hw_config.h"


 #define  LCD_DELAY  1


//--------------------------------------------------------------------------
//		Font Processing Table for Korean Character
// 초중성 테이블입니다
// --------------------------------------------------------------------------
const u8 table_cho[21]= 
	{0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
const u8 table_joong[30] = 
	{0,0,0,1,2,3,4,5,0,0,6,7,8,9,10,11,0,0,12,13,14,15,16,17,0,0,18,19,20,21};
const u8 table_jong[30] = 
	{0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,0,17,18,19,20,21,22,23,24,25,26,27};

const u8 bul_cho1[22]  = {0,0,0,0,0,0,0,0,0,1,3,3,3,1,2,4,4,4,2,1,3,0};
const u8 bul_cho2[22]  = {0,5,5,5,5,5,5,5,5,6,7,7,7,6,6,7,7,7,6,6,7,5};
const u8 bul_jong[22]  = {0,0,2,0,2,1,2,1,2,3,0,2,1,3,3,1,2,1,3,3,1,1};

#if 0
unsigned char table_cho[21] =
	{0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
unsigned char table_joong[30] =
	{0,0,0,1,2,3,4,5,0,0,6,7,8,9,10,11,0,0,12,13,14,15,16,17,0,0,18,19,20,21};
unsigned char table_jong[30] =
	{0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,0,17,18,19,20,21,22,23,24,25,26,27};

unsigned char bul_cho1[22] = {0,0,0,0,0,0,0,0,0,1,3,3,3,1,2,4,4,4,2,1,3,0};
unsigned char bul_cho2[22] = {0,5,5,5,5,5,5,5,5,6,7,7,7,6,6,7,7,7,6,6,7,5};
unsigned char bul_jong[22] = {0,0,2,0,2,1,2,1,2,3,0,2,1,3,3,1,2,1,3,3,1,1};
#endif
 


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* ------------ RCC registers bit address in the alias region ----------- */
 
/*
 음식물 종량제 처리기 1차 

  - GSE-1600
	LCD_E    : PE5            -----\___________/---\______
    LCD_RW   : PE6            -------\_______________/----
    LCD_RS   : PE7            ---------\____/-------------
    LCD_DATA : PF0..PF7       ---|||||||||||||||||||||||--
	
	  _ VENDING MACHINE  
	LCD_E    : PG9           -----\___________/---\______
    LCD_RW   : PG10            -------\_______________/----
    LCD_RS   : PG11           ---------\____/-------------
    LCD_DATA : PG0..PG7       ---|||||||||||||||||||||||--
*/

 /*

 음식물 종량제 처리기 2차 2012년 3월 28일  
	  
	  Filename  :  lcd.c
	  Date      :  1996. 10. 30
	  Model     :  VIP-97 (HG12605NY-LY)
	  Version   :  1.00
	  LCD MODULE : hyundai electronics


-	NO CONTROL BOARD
	RS  - L : INSTRUCTION OR STATUS
		- H : DATA READ / WRITE
	R/W - L : WRITE
		- H : READ
                                                              
	  Instruction write
	  E         --------\_______/-------\_______/---------
	  R/W       ----------\_________________/-------------
	  DI        ----------\_________________/-------------
	  CS1       ----------\_________________/-------------
	  CS2       ------------------------------------------
	  DB0-7     --------|||||||||||||||||||||||-----------
	  CS2 CS1  E  RW  DI
	   0   0   1   1   1
	   0   0   0   1   1
	   0   1   0   0   0
	   0   1   1   0   0
	   0   1   0   0   0
	   0   0   0   1   1
	   0   0   1   1   1

	  Data write
	  E         --------\_______/-------\_______/---------
	  R/W       ----------\_________________/-------------
	  DI        __________/-----------------\_____________
	  CS1       __________/-----------------\_____________
	  CS2       __________________________________________
	  DB0-7     --------|||||||||||||||||||||||-----------
	  CS2 CS1  E  RW  DI
	   0   0   1   1   0
	   0   0   0   1   0
	   0   1   0   0   1
	   0   1   1   0   1
	   0   1   0   0   1
	   0   0   0   1   0
	   0   0   1   1   0
            
	   CS2: page 1  CS1 : page 0

- graphic panel
   0,0
     +------------------------+   +------------------------+
	      PAGE 0 (CS1)                   PAGE 1 (CS2)
    D0 D1 D2 D3 D4 D5 ...... D63 D0 D1 D2 D3 D4 D5 ...... D63 
    b0 b0 b0 b0 b0 b0 
	b1 b1 b1 b1 b1 b1
	b2 b2
	b3 b3
	b4
	b5
	b6
	b7
    
	D64 D65 ......         
	b0
	b1

     +------------------------+   +------------------------+
														127,63
*/







//#define LCD_COM_PORT      PORTE  // LCD port 
//#define LCD_DATA_PORT     PORTA  // LCD data port 

#define LCD_DATA_PORT_IN  PINA //input data LCD 
/*
#define LCD_E 		4 // LCD enable line 
#define LCD_RW 		3 // LCD read/write selecting line 
#define LCD_DI 		2 // LCD Data/instruction selecting line 
#define LCD_CS1     6 // LCD left/right selecting line 
#define LCD_CS2     5 // LCD left/right selecting line 
#define LCD_RES     7 // LCD Reset tied vcc

*/
#define LCD_E 		LCD_E_PIN // LCD enable line 
#define LCD_RW 		LCD_RW_PIN // LCD read/write selecting line 
#define LCD_DI 		LCD_DI_PIN // LCD Data/instruction selecting line 
#define LCD_CS1     LCD_CS1_PIN // LCD left/right selecting line 
#define LCD_CS2     LCD_CS2_PIN // LCD left/right selecting line 
#define LCD_RES     LCD_RESET_PIN // LCD Reset tied vcc


//GPIO_Pin_9

 /****** for lcd start ***********/
 //unsigned char   INVERSE;     // 0x00, 0xFF
extern vu8 INVERSE;
extern unsigned char   HAN_CODE;
extern unsigned char   FONT_SIZE;             
extern unsigned char   FONT_STYLE;
                         
extern vu16 xcharacter;
extern vu16 ycharacter;
extern vu16 xgraphic;
extern vu16 ygraphic;
extern vu16 cur_x;
extern vu16 cur_y;
extern vu16 xch;		//character x
extern vu16 ych;		//character y
extern vu16 xgr;		//graphic x
extern vu16 ygr;		//graphic y
extern vu16 xcur;		//cursor x
extern vu16 ycur;		//cursor y

extern volatile  int cx, cy; 	//2014.8.27
extern vu8 LCX, LCY;
extern vu8 LCDPAGE;
//extern u8 INVERSE;

//void lcdu8(u8 control, u8 value) //_lcdu8:	; void lcdu8(u8 control, u8 data)
void lcdu8(u8 control, u8 value) //_lcdu8:	; void lcdu8(u8 control, u8 data)

{
////#define LCD_DATA      0x0800
////#define LCD_CONTROL   0x7F
////       PG15 PG14 PG13 PG12   PG11 PG10 PG9 PG8   PG7 PG6 PG5 PG4    PG3 PG2 PG1  PG0       
////                              RS   RW   E   X     D7  D6  D5  D4     D3 D2  D1   D0	 														 
	  /* 
	    PORTE &= 0xDF;      //E LOW
        
        PORTF = value;
        PORTE |= LCD_DATA;              //RS : DATA
        if (control == LCD_CONTROL) PORTE &= LCD_CONTROL;  //RS : COMMAND
        PORTE &= 0xBF;  //RESET LCD_RW
        _NOP();
        PORTE |= 0x20;      //E  HIGH
        delay_time1(100);
        PORTE &= 0xDF;      //E LOW
        _NOP();
        PORTE |= 0x40;      //RW : READ
        PORTE |= LCD_DATA;      //RS : DATA
       */

	  
	GPIO_ResetBits(GPIO_LCD_DATA, GPIO_Pin_9);  // E LOW
	//GPIO_Write(GPIO_LCD_DATA, value );  //     DATA LOADING  0x00value  output  so  E LOW REMAIN !!
      
	//value&=0xffff;
	GPIO_Write(GPIO_LCD_DATA, value );  //     DATA LOADING  0x00value  output  so  E LOW REMAIN !!
	if (control == LCD_CONTROL)  GPIO_ResetBits(GPIO_LCD_DATA, GPIO_Pin_11);  // RS low
	else GPIO_SetBits(GPIO_LCD_DATA, GPIO_Pin_11);  // RS HIGH

    GPIO_ResetBits(GPIO_LCD_DATA, GPIO_Pin_10);  // RW LOW
	Delay_us(1);	//5
	GPIO_SetBits(GPIO_LCD_DATA, GPIO_Pin_9);  // E HIGH

	Delay_us(1); //1  	 
	  
	GPIO_ResetBits(GPIO_LCD_DATA, GPIO_Pin_9);  // E LOW
	Delay_us(1);	 //5
	GPIO_SetBits(GPIO_LCD_DATA, GPIO_Pin_10);  // RW HIGH
    GPIO_ResetBits(GPIO_LCD_DATA, GPIO_Pin_11);  // RS LOW
	GPIO_SetBits(GPIO_LCD_DATA, GPIO_Pin_9);  // E HIGH
  	  
	  

	 /*
	  GPIO_Write(GPIO_LCD_DATA, LCD_DATA|value );  //  RS : HIGH  E: LOW    VALUE IS DATA
	  if (control == LCD_CONTROL)  GPIO_Write(GPIO_LCD_DATA, value );  //  RS : LOW  E: LOW    VALUE IS CONTROL
	  //   also  RW LOW
	  delay_us(100);
	  //  E HIGH
	  GPIO_SetBits(GPIO_LCD_DATA, GPIO_Pin_9);  // E HIGH
	  Delay_ms(100);
	  */ 
	 
 }
/*
void lcd_cls(void)
{
	 lcdu8 (LCD_CONTROL, 0x01); // Clear Display 
	 cx = 0;
	 cy = 0;
}
*/
 /*
void lcd_clear(void)
{
	GLCD_string (0,0 ,"                ");
  GLCD_string (0,2 ,"                ");
  GLCD_string (0,4 ,"                ");
  GLCD_string (0,6 ,"                ");
 
}
*/
void lcd_cls( void)
{
/**
	u8 x, y;
//	u8 i;

	for (y=0; y < 8; y++)
	{
		for (x=0; x < 22; x++)
		{
			lcd_putimage_xy(x, y, 0x00);
//			lcd_gotoxy(x, y);
//			for (i=0; i<6; i++) lcd_data_write(ascii_table[0][i]);
		}
	}
**/

	unsigned char pages,i; 


	for (pages=0;pages<8;pages++)
	{ 
    	LCDPAGE=0;
		LCDleft(); 
    	lcd_command(64); 
    	lcd_command(184+pages); 
    	for (i=0;i<64;i++)
		{ 
      		lcd_data_write(0); 
			    Delay_us(3);
    	}
		LCDPAGE=1; 
    	LCDright(); 
    	lcd_command(64); 
    	lcd_command(184+pages); 
    	for (i=0;i<64;i++)
		{ 
      		lcd_data_write(0);
           Delay_us(3);			
    	} 
  	} 



	LCDPAGE = 0;
	LCX = 0; LCY = 0;
	lcd_gotoxy (LCX, LCY);
	xcharacter = 0;
	ycharacter = 0;
	xgraphic = 0;
	ygraphic = 0;
	cur_x = 0;
	cur_y = 0;


	xch = 0;		//character x
	ych = 0;		//character y
	xgr = 0;		//graphic x
	ygr = 0;		//graphic y
	xcur = 0;		//cursor x
	ycur = 0;		//cursor y
} 

void cursor_on()
{
	lcdu8 (LCD_CONTROL, 0x0F); // blink cursor on 
}

void cursor_off()
{
	lcdu8 (LCD_CONTROL, 0x0C); // blink cursor off 
}

void goto_xy ( u8 x, u8 y)
{
	switch (y)
	{
		case 0:	lcdu8 (LCD_CONTROL, 0x80 + x); break;
		case 1:	lcdu8 (LCD_CONTROL, 0xC0 + x); break;
		case 2: lcdu8 (LCD_CONTROL, 0x80 + 20 + x); break;
		case 3:	lcdu8 (LCD_CONTROL, 0xC0 + 20 + x); break;
		default: break;
	}
	cx = x; cy = y;
}
//void lputs_xy(u8 c, u8 r, flash char *str)

// 음식물 종량제 1차 

/*
void lcd_initial()
{
	lcdu8 (LCD_CONTROL, 0x38); Delay_ms(10);	// 00111000B 
	lcdu8 (LCD_CONTROL, 0x0C); Delay_ms(10);
	lcdu8 (LCD_CONTROL, 0x06); Delay_ms(10);	// Entry Mode Set 
	lcdu8 (LCD_CONTROL, 0x01); Delay_ms(10);	// Clear Display 
	Delay_ms(1000);
} // of lcd_initial() 

*/



/************************* 종량제 2차 ********************/

void lcd_delay(void)
{
///	_delay_us(100);
 Delay_us(100);
}


u8 lcd_status(void)
{
	Delay_us(10);
}

void lcd_normal (void)
{
	INVERSE = 0x00;	//lputchar (0x1B);  lputchar ('C');
}

void lcd_inverse (void)
{
	INVERSE = 0xFF;	//lputchar (0x1B);  lputchar ('D');
}

 void lcd_gotoy(u8 y)
{
   LCY = y;
   lcd_command(LCY + 0xb8);
   lcd_delay();
}

void lcd_gotox(u8 x)
{
   LCX = x & 0x1f;
   if(LCX <11) 
   {	LCDPAGE=0;
	lcd_command(LCX*6+0x40);
   }
   else 
   {	LCDPAGE=1;
	lcd_command((LCX-11)*6+2+0x40);
   }
   lcd_delay();
}  


void lcd_gotoxy(u8 x, u8 y)
{
	lcd_gotox(x);
	lcd_gotoy(y);
}	


/** graphic **/
void lcd_goto_y(u8 y)
{
   LCY = y;
   lcd_command(LCY + 0xb8);
   lcd_delay();
}

void lcd_goto_x(u8 x)
{
   LCX = x;	// & 0x7f;
   if(LCX < 64) 
   {	LCDleft();
		lcd_command(LCX+0x40);
   }
   else 
   {	LCDright();
		lcd_command((LCX-64)+0x40);
   }
   lcd_delay();
}  


void lcd_goto_xy(u8 x, u8 y)
{
	lcd_goto_x(x);
	lcd_goto_y(y);
}	
                
void lcd_goto_x0()	//;goto cursor 0, y
{
	LCDPAGE = 1;
	lcd_gotoy(LCY);
//	lcd_delay();
	lcd_command(0x40);		;//x address 0
	lcd_delay();
}


void lcd_putimage_xy(u8 x, u8 y, u8 img)
{
	u8  i;

	lcd_gotoxy(x, y);
	for (i = 0; i < 6; i++) lcd_data_write(img);
}

                    
					
void lcd_set_pixel(unsigned char x, unsigned char y)
{  // set a pixel on LCD 
	unsigned char page; 
  	unsigned char oldValue; 

  	if(x < 64)
	{ 
    	LCDleft(); 
  	}
	else
	{ 
    	LCDright(); 
    	x = x - 64; 
  	} 

  	page = 184 + y/8; 
  	lcd_command(page);                 // select Page 
  	lcd_command(64+x);                 // select Y 
  	////oldValue = lcd_data_read();        // read old value 
  	////oldValue = lcd_data_read(); 
  	lcd_command(page);                 // select Page 
  	lcd_command(64+x);                 // select Y 
  	lcd_data_write(oldValue | (1 << (y % 8))); 
} 

void lcd_reset_pixel(unsigned char x, unsigned char y)
{  // Clear a pixel on LCD 

  	unsigned char page; 
  	unsigned char oldValue; 

  	if(x < 64)
  	{ 
    	LCDleft(); 
  	}
  	else
  	{ 
  		LCDright(); 
    	x = x - 64; 
  	} 

  	page = 184 + y/8; 
  	lcd_command(page);  //select Page 
  	lcd_command(64+x);   //select Y 
  ////	oldValue = lcd_data_read(); 
  ////	oldValue = lcd_data_read(); 
  	lcd_command(page);  //select Page 
  	lcd_command(64+x);   //select Y 
  	lcd_data_write(oldValue & ~(1 << (y % 8))); 
} 

/*
void GLCD_English_hg12605(unsigned char Ecode)		// display a 8x16 Dot English(ASCII)
{
    unsigned char i;
  
   for(i = 0; i < 8; i++)
    {                         
		lcd_goto_xy(cur_x, cur_y);
        lcd_data_write(E_font[Ecode][i]^INVERSE);
		Delay_ms(500);
		lcd_goto_xy(cur_x, cur_y+1);
	    lcd_data_write(E_font[Ecode][i+8]^INVERSE);
		Delay_ms(500);
        cur_x++;
    }
   
	++xcharacter;
}

 */
 
 
 /*
void GLCD_English_hg12605(unsigned char Ecode)		// 가로 쓰기  display a 8x16 Dot English(ASCII)
{
    unsigned char i;


  
   for(i = 0; i < 8; i++)
    {                         
		lcd_goto_xy(cur_x, cur_y);
        lcd_data_write(E_font[Ecode][i]^INVERSE);
	  
		lcd_goto_xy(cur_x, cur_y+1);
	    lcd_data_write(E_font[Ecode][i+8]^INVERSE);
		
        cur_x++;
    }
   
	++xcharacter;

}
*/
  


 /* 

  void GLCD_English_hg12605(unsigned char Ecode)// 세로 쓰기 
{
    unsigned char i;

	cur_x =  xcharacter * 8;
	cur_y =  ycharacter;
 
   for(i = 0; i < 16; i++)
    {                         
		lcd_goto_xy(cur_x, cur_y);
        lcd_data_write(E_font[Ecode][i]^INVERSE);
        cur_x++;
    }
   
	--ycharacter;
}

 */
 void GLCD_English_hg12605(unsigned char Ecode)		// display a 8x16 Dot English(ASCII)
{
    unsigned char i;

	 /*
    for(i = 0; i < 8; i++)
    {                         
		lcd_goto_xy(cur_x, cur_y);
        lcd_data_write(pgm_read_byte(&E_font[Ecode][i])^INVERSE);
		lcd_goto_xy(cur_x, cur_y+1);
	    lcd_data_write(pgm_read_byte(&E_font[Ecode][i+8])^INVERSE);

        cur_x++;
    }           
*/	 
//	  cur_x=3; cur_y=3;

	  for(i = 0; i < 8; i++)
    {                         
		lcd_goto_xy(cur_x, cur_y);
        lcd_data_write(E_font[Ecode][i]^INVERSE);
		lcd_goto_xy(cur_x, cur_y+1);
	    lcd_data_write(E_font[Ecode][i+8]^INVERSE);

        cur_x++;
    }
   
	++xcharacter;
}

 /*

void GLCD_Korean_hg12605(unsigned char *image32)		// display a 16x16 Dot Korean///
{   
	int i;

	cur_x =  xcharacter * 8;
	cur_y =  ycharacter;

    for(i = 0; i < 16; i++)               //LCD 출력 부분입니다.
    {
		lcd_goto_xy(cur_x, cur_y);
        lcd_data_write(image32[i]^INVERSE);
		lcd_goto_xy(cur_x,cur_y+1);
        lcd_data_write(image32[i+16]^INVERSE);

        cur_x++;
    }

    xcharacter++;
    xcharacter++;
}

void GLCD_Korean(unsigned int Kcode)		// display a 16x16 Dot Korean///
{
	unsigned char i;
    unsigned char cho_5bit, joong_5bit, jong_5bit;
    unsigned char cho_bul, joong_bul, jong_bul=0, jong_flag;
    unsigned int character;
    unsigned char Korean_buffer[32];		// 32 byte Korean font buffer
    
    
	// get cho, joong, jong 5 bit
    cho_5bit = pgm_read_byte(&table_cho[(Kcode >> 10) & 0x001F]);//Kcode
    joong_5bit = pgm_read_byte(&table_joong[(Kcode >> 5) & 0x001F]);
    jong_5bit = pgm_read_byte(&table_jong[Kcode & 0x001F]);

    if(jong_5bit == 0)				// don't have jongsung
    {   jong_flag = 0;
        cho_bul = pgm_read_byte(&bul_cho1[joong_5bit]);
        if((cho_5bit == 1) || (cho_5bit == 16)) joong_bul = 0;
        else                                    joong_bul = 1;
    }
    else						// have jongsung
    {   jong_flag = 1;
        cho_bul = pgm_read_byte(&bul_cho2[joong_5bit]);
        if((cho_5bit == 1) || (cho_5bit == 16)) joong_bul = 2;
        else                                    joong_bul = 3;
        jong_bul = pgm_read_byte(&bul_jong[joong_5bit]);
    }

    character = (unsigned int)cho_bul*20 + cho_5bit;		// copy chosung font
    for(i = 0; i <= 31; i++)
        Korean_buffer[i] = pgm_read_byte(&K_font[character][i]);

    character = 8*20 + (unsigned int)joong_bul*22 + joong_5bit;	// OR joongsung font
    for(i = 0; i <= 31; i++)
        Korean_buffer[i] |= pgm_read_byte(&K_font[character][i]);

    if(jong_flag == 1)				// if jongsung, OR jongsung font
    {   character = 8*20 + 4*22 +(unsigned int)jong_bul*28 + jong_5bit;
        for(i = 0; i <= 31; i++)
            Korean_buffer[i] |= pgm_read_byte(&K_font[character][i]);
    }


//	LCD_Korean_hy_12232(Korean_buffer);		// display a 16x16 Dot Korean///
	GLCD_Korean_hg12605(Korean_buffer);			// display a 16x16 Dot Korean///
}

  */
 
// 		  쓰기 
 
 /*
 void GLCD_Korean_hg12605(unsigned char *image32)		// display a 16x16 Dot Korean///
{   
	int i;

	cur_x =  xcharacter * 8;
	cur_y =  ycharacter;


    for(i = 0; i < 16; i++)               //LCD 출력 부분입니다.
    {
		lcd_goto_xy(cur_x, cur_y);
        lcd_data_write(image32[i]^INVERSE);
		lcd_goto_xy(cur_x,cur_y+1);
        lcd_data_write(image32[i+16]^INVERSE);

        cur_x++;
    }

    xcharacter++;
    xcharacter++;
}

 */


 /*

 // 세로쓰기 
 void GLCD_Korean_hg12605(unsigned char *image32)		// display a 16x16 Dot Korean///
{   
	int i;

	cur_x =  xcharacter * 8;
	cur_y =  ycharacter;


    for(i = 0; i < 16; i++)               //LCD 출력 부분입니다.
    {
		lcd_goto_xy(cur_x, cur_y);
        lcd_data_write(image32[i]^INVERSE);
		lcd_goto_xy(cur_x,cur_y+1);
        lcd_data_write(image32[i+16]^INVERSE);

        cur_x++;
    }

    ycharacter--;
	ycharacter--;
}
  */



  void GLCD_Korean_hg12605(unsigned char *image32)		// display a 16x16 Dot Korean///
{   
	int i;

	cur_x =  xcharacter * 8;
	cur_y =  ycharacter;

//	cur_x =  xcharacter ;
//	cur_y =  ycharacter*8;


    for(i = 0; i < 16; i++)               //LCD 출력 부분입니다.
    {
	   
		lcd_goto_xy(cur_x, cur_y);
        lcd_data_write(image32[i]^INVERSE);
		lcd_goto_xy(cur_x,cur_y+1);
        lcd_data_write(image32[i+16]^INVERSE);
		
        cur_x++;
    }

    xcharacter++;
    xcharacter++;
}

void GLCD_Korean(unsigned int Kcode)		// display a 16x16 Dot Korean///
{
	unsigned char i;
    unsigned char cho_5bit, joong_5bit, jong_5bit;
    unsigned char cho_bul, joong_bul, jong_bul=0, jong_flag;
    unsigned int character;
    unsigned char Korean_buffer[32];		// 32 byte Korean font buffer
    
	// get cho, joong, jong 5 bit
    cho_5bit = (table_cho[(Kcode >> 10) & 0x001F]);//Kcode
    
	joong_5bit =(table_joong[(Kcode >> 5) & 0x001F]);
    jong_5bit = (table_jong[Kcode & 0x001F]);

    if(jong_5bit == 0)				// don't have jongsung
    {   jong_flag = 0;
        cho_bul = (bul_cho1[joong_5bit]);
        if((cho_5bit == 1) || (cho_5bit == 16)) joong_bul = 0;
        else                                    joong_bul = 1;
    }
    else						// have jongsung
    {   jong_flag = 1;
        cho_bul = (bul_cho2[joong_5bit]);
        if((cho_5bit == 1) || (cho_5bit == 16)) joong_bul = 2;
        else                                    joong_bul = 3;
        jong_bul = (bul_jong[joong_5bit]);
    }

    character = (unsigned int)cho_bul*20 + cho_5bit;		// copy chosung font
    for(i = 0; i <= 31; i++)
        Korean_buffer[i] = (K_font[character][i]);

    character = 8*20 + (unsigned int)joong_bul*22 + joong_5bit;	// OR joongsung font
    for(i = 0; i <= 31; i++)
        Korean_buffer[i] |= (K_font[character][i]);

    if(jong_flag == 1)				// if jongsung, OR jongsung font
    {   character = 8*20 + 4*22 +(unsigned int)jong_bul*28 + jong_5bit;
        for(i = 0; i <= 31; i++)
            Korean_buffer[i] |= (K_font[character][i]);
    }


//	LCD_Korean_hy_12232(Korean_buffer);		// display a 16x16 Dot Korean///
	GLCD_Korean_hg12605(Korean_buffer);			// display a 16x16 Dot Korean///
}





//------------------------------------------------------------------------------
// 8*16 한글 영문 출력 부분 입니다.
//------------------------------------------------------------------------------
// 2014.10.21 add
void GLCD_string_center(unsigned char y, unsigned char *string)	// display a string
{
    unsigned char character1;
    unsigned int character2;
	char 	length;
	char	x;

	length = strlen(string);
	x = 7 - (length-1)/2;	//16 ch/line, start 0.			
    xcharacter = x;
    ycharacter = y;
    cur_x = x*8;
    cur_y = y;		//cur_y = y*16;
    
    while(*string != '\0')
    {   character1 = *string;
        string++;
        if(character1 < 0x80)           //영문일경우...
        {
           // GLCD_English(character1);	// 8x16 display English character
		   GLCD_English_hg12605(character1);
        }
        else
        {
            character2 = character1*256 + (*string & 0xFF);
            string++;
            character2 = kssm_convert(character2);//완성형을 조합형으로 변환

            GLCD_Korean(character2);		// display Korean character
        }
    }
}




//void GLCD_string(unsigned char x,unsigned char y,flash unsigned char *string)	// display a string
void GLCD_string(unsigned char x,unsigned char y, unsigned char *string)	// display a string

{
    unsigned char character1;
    unsigned int character2;

//	GPIO_SetBits(GPIOG, GPIO_Pin_8);
    xcharacter = x;
    ycharacter = y;
    cur_x = x*8;
    cur_y = y;		//cur_y = y*16;

    
    while(*string != '\0')
    {   character1 = *string;
        string++;
        if(character1 < 0x80)           //영문일경우...
        {
           // GLCD_English(character1);	// 8x16 display English character
		   GLCD_English_hg12605(character1);
        }
	   
        else
        {
            character2 = character1*256 + (*string & 0xFF);
            string++;
            character2 = kssm_convert(character2);//완성형을 조합형으로 변환

            GLCD_Korean(character2);		// display Korean character
        }
       

    }
}
 /*
 void GLCD_Korean_hg12605(unsigned char *image32)		// 1213display a 16x16 Dot Korean///
{   
	int i;

	cur_x =  xcharacter * 8;
	cur_y =  ycharacter;

    for(i = 0; i < 16; i++)               //LCD 출력 부분입니다.
    {
		lcd_goto_xy(cur_x, cur_y);
        lcd_data_write(image32[i]^INVERSE);
		lcd_goto_xy(cur_x,cur_y+1);
        lcd_data_write(image32[i+16]^INVERSE);

        cur_x++;
    }

    xcharacter++;
    xcharacter++;
}
  */
/*
void lcd_line_checker(unsigned char x1, unsigned char y1,unsigned char x2, unsigned char y2)
{  // Draw a line on LCD
	unsigned int x, y;

	if(y1 != y2)				
	{
		if(y1 < y2)			
			for(y = y1; y <= y2; y++)
		  	{	x = x1 + (y - y1)*(x2 - x1)/(y2 - y1);
				lcd_set_pixel(x,y);	//Dot(x,y);
		  	}
	  	else
			for(y = y1; y >= y2; y--)
		  	{	x = x1 + (y - y1)*(x2 - x1)/(y2 - y1);
				lcd_set_pixel(x,y);	//Dot(x,y);
		  	}
	} 
	else if(x1 != x2)			
	{ 	if(x1 < x2)				
			for(x = x1; x <= x2; x++)
		  	{	y = y1 + (x - x1)*(y2 - y1)/(x2 - x1);
				lcd_set_pixel(x,y);	//Dot(x,y);
		  	}
	  	else
			for(x = x1; x >= x2; x--)
		  	{	y = y1 + (x - x1)*(y2 - y1)/(x2 - x1);
				lcd_set_pixel(x,y);	//Dot(x,y);
		  	}
	}
  	else						
		lcd_set_pixel(x,y);	//Dot(x,y);
}

*/					  
 // 15 14 13 12 11 10 9 8   7 6 5 4 3 2 1 0 
 // 9: reset

 #define lcd_delay
void lcd_command(u16 cmd)
{  
  u16 data,left_data,right_data;

   data=cmd; //  0000 0000 1234 5678  -> to be 0000 0001 2034 5678
   data<<=1;                  // 	           0000 0001 2345 6780
   left_data= data & 0x0180; // 			   0000 0001 2000 0000	////
   data=cmd;				 //				   0000 0000 1234 5678
   right_data=data & 0x003f; //				   0000 0000 0034 5678
   data=left_data|right_data;      

   if (!LCDPAGE)  LCDleft();    else LCDright();  
	
	
	GPIO_ResetBits(GPIO_LCD_DATA, LCD_DI); //Delay_ms(1); // high: data   low: instruction
	GPIO_ResetBits(GPIO_LCD_DATA, LCD_RW);//Delay_ms(1);  // high: read   low: write
	//di rw  e c2 c1	r
	//15 14 13 12 11 10 9 8    7 6 5 4 3 2 1 0
  //GPIO_Write(GPIO_LCD_DATA, cmd|0x0200 );Delay_ms(1);  //     DATA LOADING
   //GPIO_Write(GPIO_LCD_DATA, cmd|0x1a00 );//Delay_ms(1);  //     DATA LOADING
    
	/*
	// 서귀포     
	if (!LCDPAGE)
	  GPIO_Write(GPIO_LCD_DATA, cmd|0x0b00 );//Delay_ms(1);	0000  1010   Original :0X0A00   
	  else 
	  GPIO_Write(GPIO_LCD_DATA, cmd|0x1300 );//Delay_ms(1);	0001  0010	 original :0X1200
	*/
	//저가형 
	if (!LCDPAGE)
	  GPIO_Write(GPIO_LCD_DATA, data|0x0800 );//Delay_ms(1);	0000  1010   Original :0X0A00   
	  else 
	  GPIO_Write(GPIO_LCD_DATA, data|0x1000 );//Delay_ms(1);	0001  0010	 original :0X1200
	




	GPIO_SetBits(GPIO_LCD_DATA, LCD_E); //Delay_ms(1); // data loading 
	Delay_us(1);// original 1
    //lcd_delay_command();
	GPIO_ResetBits(GPIO_LCD_DATA, LCD_E);  // 
	
	
	
	//LCD_COM_PORT &= ~(1<<LCD_DI);	//cbi(LCD_COM_PORT, LCD_DI); 
  	//cbi(LCD_COM_PORT, LCD_RW); 
	//LCD_DATA_PORT = cmd;
  	//LCD_COM_PORT |= (1<<LCD_E);		//sbi(LCD_COM_PORT, LCD_E); 
  	//lcd_delay_command();	//_delay_us(1);	//_delay_us(10);		//delay_10us(); 
  	//cbi(LCD_COM_PORT, LCD_E); 

} 

void lcd_delay_command(void)
{
 	unsigned int i;

//	for (i=0; i<1; ++i) ;
 	for (i=0; i<2; ++i) ;
}


void lcd_data_write(u16 ch)		 //
{
   u16 data,left_data,right_data;

   data=ch; //  0000 0000 1234 5678  -> to be 0000 0001 2034 5678
   data<<=1;                  // 	          0000 0001 2345 6780
   left_data= data & 0x0180; // 			  0000 0001 2000 0000	////
   data=ch;				 //				      0000 0000 1234 5678
   right_data=data & 0x003f; //				  0000 0000 0034 5678
   data=left_data |right_data;      




	if (!LCDPAGE)  LCDleft();    else LCDright();  
	GPIO_SetBits(GPIO_LCD_DATA, LCD_DI);   // Delay_ms(1);    // high: data   low: instruction
	GPIO_ResetBits(GPIO_LCD_DATA, LCD_RW);// Delay_ms(1); // high: read   low: write

	//di rw	e  c2 c1	r
	//15 14 13 12 11 10 9 8    7 6 5 4 3 2 1 0
    //GPIO_Write(GPIO_LCD_DATA, ch|0x8200 );        Delay_ms(1);//     DATA LOADING
    // GPIO_Write(GPIO_LCD_DATA, ch|0x9a00 );   //     Delay_ms(1);//     DATA LOADING
    
	
	 /* // 서귀포 
	   if(!LCDPAGE)
		GPIO_Write(GPIO_LCD_DATA, ch|0x8b00 );  // 1000 1010     
		else
		GPIO_Write(GPIO_LCD_DATA, ch|0x9300 );	// 1001 0011 (back light 포함)
	 */

     // 저가형 종량제 	  
	   if(!LCDPAGE)
		GPIO_Write(GPIO_LCD_DATA, data|0x8800 );  // 1000   1000     
		else
		GPIO_Write(GPIO_LCD_DATA, data|0x9000 );	// 1001 0000
	 	


 //0413

	GPIO_SetBits(GPIO_LCD_DATA, LCD_E);   // Delay_ms(1);// data loading 
	Delay_us(1);  // original1

    // lcd_delay_command();
	GPIO_ResetBits(GPIO_LCD_DATA, LCD_E);  // 

	/*
	sbi(LCD_COM_PORT,LCD_DI); 
  	cbi(LCD_COM_PORT,LCD_RW); 
  	LCD_DATA_PORT = ch;
  	sbi(LCD_COM_PORT,LCD_E); 
  	lcd_delay_command();	//_delay_us(1);	//_delay_us(10);		//delay_10us(); 
  	cbi(LCD_COM_PORT,LCD_E); 
    */

}
void display_on(void)
{
	 lcd_command(0x3f);
}



void LCDleft(void)
{                   // select left part of the LCD 
	LCDPAGE = 0;
	GPIO_ResetBits(GPIO_LCD_DATA, LCD_CS2);//Delay_ms(1);  // ACTIVE HIGH
	 Delay_us(3);
    //Delay_ms(20);	//Delay_ms(20);
 	GPIO_SetBits(GPIO_LCD_DATA, LCD_CS1);/// Delay_ms(1); // 

////	sbi(LCD_COM_PORT, LCD_CS1); 
 //// 	cbi(LCD_COM_PORT, LCD_CS2); 
//  	_delay_us(10);		//  delay_10us(); 
} 

void LCDright(void)
{                 // select right part of the LCD active high
	LCDPAGE = 1;

   	GPIO_ResetBits(GPIO_LCD_DATA, LCD_CS1); //Delay_ms(1); // ACTIVE HIGH
    Delay_us(3);
    
	//Delay_ms(20);	//Delay_ms(20);
 	GPIO_SetBits(GPIO_LCD_DATA, LCD_CS2);//Delay_ms(1);  // 

    	
  
  
  ////	cbi(LCD_COM_PORT, LCD_CS1); 
  ////	sbi(LCD_COM_PORT, LCD_CS2); 
//  	_delay_us(10);		//  delay_10us(); 
}


void lcd_initial(void)
{
  //// RESET : active low 	
	 GPIO_SetBits(LCD_RESET_PORT, LCD_RES);  // high
     Delay_ms(10);	//Delay_ms(20);
 	 GPIO_ResetBits(LCD_RESET_PORT, LCD_RES);  // low
	 Delay_ms(10);	//Delay_ms(20);
 	 GPIO_SetBits(LCD_RESET_PORT, LCD_RES);  // high
	
	 //for(;;);
     
	
	INVERSE = 0;    //  unsigned char 로 선언 되어 있음 
	LCDPAGE = 0;

	LCDleft(); 
 
	lcd_command(0xC0);		//the top of the screen 0
	Delay_ms(20);	//Delay_ms(20);
	lcd_command(0xB8);		//x address 0
	Delay_ms(20);	//	Delay_ms(20);
	lcd_command(0x40);		//y address

  

//	delay(10);
//	display_on();

	LCDPAGE = 1;
	LCDright(); 
	lcd_command(0xC0);		//the top of the screen 0 	//LCDwriteInstruction(64); 
	Delay_ms(20);	//	Delay_ms(20);
	lcd_command(0xB8);		//x address 0				//LCDwriteInstruction(63); 
	Delay_ms(20);	//	Delay_ms(20);
	lcd_command(0x40);		//y address					//LCDwriteInstruction(192);    

	lcd_cls();			//	lcls();

	LCDPAGE = 1;
	LCDright(); 
	display_on();
//	delay(10);
	LCDPAGE = 0;
	LCDleft(); 
	display_on();

	LCDPAGE = 0;
	LCX = 0; LCY = 0;
	lcd_goto_xy (LCX, LCY);
	xcharacter = 0;
	ycharacter = 0;
	xgraphic = 0;
	ygraphic = 0;
	cur_x = 0;
	cur_y = 0;


	xch = 0;		//character x
	ych = 0;		//character y
	xgr = 0;		//graphic x
	ygr = 0;		//graphic y
	xcur = 0;		//cursor x
	ycur = 0;		//cursor y

	
} /* of lcd_initial() */



/************************* 종량제 2차 ********************/



void lputs_xy(u8 c, u8 r,  char *str)

{
	unsigned  char i;

	lcdu8 (LCD_CONTROL, 0x06); // Entry Mode Set : not shift 
	Delay_ms(LCD_DELAY);
	//  Delay_us(100);
        i = c;
	switch(r)
	{
		case 0:
			lcdu8 (LCD_CONTROL, 0x80); // Set DD RAM Address 
			for(i=0; i<20; i++) lcdu8 (LCD_DATA, str[i]);
			break;
		case 1:
			lcdu8 (LCD_CONTROL, 0xC0); // Set DD RAM Address  2ND LINE 
			for(i=0; i<20; i++)	lcdu8 (LCD_DATA, str[i]);
			break;
		case 2:
			lcdu8 (LCD_CONTROL, 0x94); // Set DD RAM Address 
			for(i=0; i<20; i++) lcdu8 (LCD_DATA, str[i]);
			c = i;
			break;
		case 3:
			lcdu8 (LCD_CONTROL, 0xD4); // Set DD RAM Address  2ND LINE 
			for(i=0; i<20; i++)	lcdu8 (LCD_DATA, str[i]);
			break;
		default:
			break;
	}
} // of lcd_display() 

void lputstr_xy(u8 c, u8 r, char *str)
{
	unsigned  char i;

	lcdu8 (LCD_CONTROL, 0x06); // Entry Mode Set : not shift 
	Delay_ms(LCD_DELAY);
//	Delay_us(100);

        i = c;
	switch(r)
	{
		case 0:
			lcdu8 (LCD_CONTROL, 0x80); // Set DD RAM Address 
			for(i=0; i<20; i++) lcdu8 (LCD_DATA, str[i]);
			break;
		case 1:
			lcdu8 (LCD_CONTROL, 0xC0); // Set DD RAM Address  2ND LINE 
			for(i=0; i<20; i++)	lcdu8 (LCD_DATA, str[i]);
			break;
		case 2:
			lcdu8 (LCD_CONTROL, 0x94); // Set DD RAM Address 
			for(i=0; i<20; i++) lcdu8 (LCD_DATA, str[i]);
			c = i;
			break;
		case 3:
			lcdu8 (LCD_CONTROL, 0xD4); // Set DD RAM Address  2ND LINE 
			for(i=0; i<20; i++)	lcdu8 (LCD_DATA, str[i]);
			break;
		default:
			break;
	}
} // of lcd_display() 




/*********** graphic ******************/
/*
void splash()
{
	byte bmp;
	unsigned int u = 0;
	unsigned char pages,i; 

  	for (pages=0;pages<8;pages++)
	{ 
    	LCDleft(); 
    	lcd_command(64); 
    	lcd_command(184+pages); 
    	for (i=0;i<64;i++)
		{ 
        	bmp = pgm_read_byte(&good[u++]); // 플래시롬을 1바이트 읽음
      		lcd_data_write(bmp); 
    	} 

    	LCDright(); 
    	lcd_command(64); 
    	lcd_command(184+pages); 
    	for (i=0;i<64;i++)
		{ 
        	bmp = pgm_read_byte(&good[u++]); // 플래시롬을 1바이트 읽음
      		lcd_data_write(bmp); 
    	} 
  	} 
}

void splash_paps_200()
{
	byte bmp;
	unsigned int u = 0;
	unsigned char pages,i; 

  	for (pages=0;pages<8;pages++)
	{ 
    	LCDleft(); 
    	lcd_command(64); 
    	lcd_command(184+pages); 
    	for (i=0;i<64;i++)
		{ 
        	bmp = pgm_read_byte(&paps_200[u++]); // 플래시롬을 1바이트 읽음
      		lcd_data_write(bmp); 
    	} 

    	LCDright(); 
    	lcd_command(64); 
    	lcd_command(184+pages); 
    	for (i=0;i<64;i++)
		{ 
        	bmp = pgm_read_byte(&paps_200[u++]); // 플래시롬을 1바이트 읽음
      		lcd_data_write(bmp); 
    	} 
  	} 
}

void splash_paps_200_g()
{
	byte bmp;
	unsigned int u = 0;
	unsigned char pages,i; 

  	for (pages=0;pages<8;pages++)
	{ 
    	LCDleft(); 
    	lcd_command(64); 
    	lcd_command(184+pages); 
    	for (i=0;i<64;i++)
		{ 
        	bmp = pgm_read_byte(&paps_200_g[u++]); // 플래시롬을 1바이트 읽음
      		lcd_data_write(bmp); 
    	} 

    	LCDright(); 
    	lcd_command(64); 
    	lcd_command(184+pages); 
    	for (i=0;i<64;i++)
		{ 
        	bmp = pgm_read_byte(&paps_200_g[u++]); // 플래시롬을 1바이트 읽음
      		lcd_data_write(bmp); 
    	} 
  	} 
}
*/
/*
void splash()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 

  	for (pages=0;pages<8;pages++)
	{ 
    	LCDleft(); 
    	lcd_command(64); 
    	lcd_command(184+pages); 
    	for (i=0;i<64;i++)
		{ 
        	//bmp = pgm_read_byte(&good[u++]); // 플래시롬을 1바이트 읽음
      		//lcd_data_write(bmp);
			lcd_data_write(good[u++]); 
    	} 

    	LCDright(); 
    	lcd_command(64); 
    	lcd_command(184+pages); 
    	for (i=0;i<64;i++)
		{ 
        	//bmp = pgm_read_byte(&good[u++]); // 플래시롬을 1바이트 읽음
      		//lcd_data_write(bmp); 
			lcd_data_write(good[u++]);
    	} 
  	} 
}
*/
/*
void splash_paps_200()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 

  	for (pages=0;pages<8;pages++)
	{ 
    	LCDleft(); 
    	lcd_command(64); 
    	lcd_command(184+pages); 
    	for (i=0;i<64;i++)
		{ 
        	//bmp = pgm_read_byte(&paps_200[u++]); // 플래시롬을 1바이트 읽음
      		//lcd_data_write(bmp);
			lcd_data_write(paps_200[u++]); 
    	} 

    	LCDright(); 
    	lcd_command(64); 
    	lcd_command(184+pages); 
    	for (i=0;i<64;i++)
		{ 
        	//bmp = pgm_read_byte(&paps_200[u++]); // 플래시롬을 1바이트 읽음
      		//lcd_data_write(bmp);
	      	lcd_data_write(paps_200[u++]); 
    	} 
  	} 
}

void splash_paps_200_g()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 

  	for (pages=0;pages<8;pages++)
	{ 
    	LCDleft(); 
    	lcd_command(64); 
    	lcd_command(184+pages); 
    	for (i=0;i<64;i++)
		{ 
        	//bmp = pgm_read_byte(&paps_200_g[u++]); // 플래시롬을 1바이트 읽음
      		//lcd_data_write(bmp);
			lcd_data_write(paps_200_g[u++]); 
    	} 

    	LCDright(); 
    	lcd_command(64); 
    	lcd_command(184+pages); 
    	for (i=0;i<64;i++)
		{ 
        	//bmp = pgm_read_byte(&paps_200_g[u++]); // 플래시롬을 1바이트 읽음
      		//lcd_data_write(bmp);
			lcd_data_write(paps_200_g[u++]); 
    	} 
  	} 
}
   */


 /*
void Right_full()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;

   
  	for (pages=0;pages<8;pages++)
	{ 

	
    	LCDright(); 
    	lcd_command(64); 
    	lcd_command(184+pages); //184
    	for (i=0;i<64;i++)
		{ 
   
			lcd_data_write(0xff);
    	} 
  	} 
}

void Right_Real_full()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;
  
  	for (pages=0;pages<8;pages++)
	{ 

	 
    	LCDright(); 
    	lcd_command(64); 
    	lcd_command(184+pages); //184
    	for (i=0;i<64;i++)
		{ 
       
			lcd_data_write(full[u++]);
			lcd_normal(); 

    	} 
  	} 
}
  */
/* 
대치됨 2015.4.23 ver 1.33a1 #define	Right_Bad()	display_Image_Right(empty)
 void Right_Bad()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;

  	for (pages=0;pages<8;pages++)
	{ 

	 
    	LCDright(); 
    	lcd_command(64); 
    	lcd_command(184+pages); //184
    	for (i=0;i<64;i++)
		{ 
        
			lcd_data_write(empty[u++]);
	
    	} 
  	} 
}
*/
 // Right_Bad()
 //Right_Circle()
 //Right_Standby()
 /*
 void Right_Circle()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;
  
  
  	for (pages=0;pages<8;pages++)
	{ 

	 
    	LCDright(); 
    	lcd_command(64); 
    	lcd_command(184+pages); //184
    	for (i=0;i<64;i++)
		{ 
        
			lcd_data_write(Circle[u++]);
	
    	} 
  	} 
}

  void Right_Standby()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;
  
  
  	for (pages=0;pages<8;pages++)
	{ 

	 
    	LCDright(); 
    	lcd_command(64); 
    	lcd_command(184+pages); //184
    	for (i=0;i<64;i++)
		{ 
        
			lcd_data_write(Standby[u++]);
	
    	} 
  	} 
}




  void Left_full()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;

   
  	for (pages=0;pages<8;pages++)
	{ 

	  
    	LCDleft(); 
    	lcd_command(64); 
    	lcd_command(184+pages); 
    	for (i=0;i<64;i++)
		{ 
        	//bmp = pgm_read_byte(&paps_200_g[u++]); // 플래시롬을 1바이트 읽음
      		//lcd_data_write(bmp);
			lcd_data_write(0xff); 
		 //	lcd_data_write(empty[u++]);
    	} 
	
  	} 
}
 
 
   void Left_Bad()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;

   
  	for (pages=0;pages<8;pages++)
	{ 

	  
    	LCDleft(); 
    	lcd_command(64); 
    	lcd_command(184+pages); 
    	for (i=0;i<64;i++)
		{ 
        	//bmp = pgm_read_byte(&paps_200_g[u++]); // 플래시롬을 1바이트 읽음
      		//lcd_data_write(bmp);
			//lcd_data_write(0xff);
			lcd_data_write(empty[u++]); 
    	} 
	
  	} 
}

   void Left_Circle()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;

   
  	for (pages=0;pages<8;pages++)
	{ 

	  
    	LCDleft(); 
    	lcd_command(64); 
    	lcd_command(184+pages); 
    	for (i=0;i<64;i++)
		{ 
        	//bmp = pgm_read_byte(&paps_200_g[u++]); // 플래시롬을 1바이트 읽음
      		//lcd_data_write(bmp);
			//lcd_data_write(0xff);
			lcd_data_write(Circle[u++]); 
    	} 
	
  	} 
}


  void Left_Standby()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;

   
  	for (pages=0;pages<8;pages++)
	{ 

	  
    	LCDleft(); 
    	lcd_command(64); 
    	lcd_command(184+pages); 
    	for (i=0;i<64;i++)
		{ 
        	//bmp = pgm_read_byte(&paps_200_g[u++]); // 플래시롬을 1바이트 읽음
      		//lcd_data_write(bmp);
			//lcd_data_write(0xff);
			lcd_data_write(Standby[u++]); 
    	} 
	
  	} 
}
 	  */
/*
대치됨 2015.4.23 ver 1.33a1 #define	Level_One()	display_Image_Right(Level_1)
..
void Level_One()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;
  
  	for (pages=0;pages<8;pages++)
	{ 

	   
    	LCDright(); 
    	lcd_command(64); 
    
		lcd_command(184+pages); //184
    	for (i=0;i<64;i++)
		{ 
			lcd_data_write(Level_1[u++]);
    	} 
	  
		
  	} 
}

void Level_Two()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;

  	for (pages=0;pages<8;pages++)
	{ 
  	    LCDright(); 
    	lcd_command(64); 
 		lcd_command(184+pages); //184
    	for (i=0;i<64;i++)
		{ 
			lcd_data_write(Level_2[u++]);
    	} 
 	} 
}


void Level_Three()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;

  	for (pages=0;pages<8;pages++)
	{ 
  	    LCDright(); 
    	lcd_command(64); 
 		lcd_command(184+pages); //184
    	for (i=0;i<64;i++)
		{ 
			lcd_data_write(Level_3[u++]);
    	} 
 	} 
}

void Level_Four()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;

  	for (pages=0;pages<8;pages++)
	{ 
  	    LCDright(); 
    	lcd_command(64); 
 		lcd_command(184+pages); //184
    	for (i=0;i<64;i++)
		{ 
			lcd_data_write(Level_4[u++]);
    	} 
 	} 
}


 void Level_Five()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;

  	for (pages=0;pages<8;pages++)
	{ 
  	    LCDright(); 
    	lcd_command(64); 
 		lcd_command(184+pages); //184
    	for (i=0;i<64;i++)
		{ 
			lcd_data_write(Level_5[u++]);
    	} 
 	} 
}
*/
										  
 /*
 void Level_Error()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;
  	for (pages=0;pages<8;pages++)
	{ 
  	    LCDright(); 
    	lcd_command(64); 
 		lcd_command(184+pages); //184
    	for (i=0;i<64;i++)
		{ 
			lcd_data_write(No_Use[u++]);
    	} 
 	} 
}

  */

/*
 void Right_Two_Five()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;
  
  
  	for (pages=0;pages<8;pages++)
	{ 

	 
    	LCDright(); 
    	lcd_command(64); 
    	lcd_command(184+pages); //184
    	for (i=0;i<64;i++)
		{ 
        
			lcd_data_write(Two_Five[u++]);
	
    	} 
  	} 
}
 */
 /*
 void Right_Five_Zero()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;
  
  
  	for (pages=0;pages<8;pages++)
	{ 

	 
    	LCDright(); 
    	lcd_command(64); 
    	lcd_command(184+pages); //184
    	for (i=0;i<64;i++)
		{ 
        
			lcd_data_write(Five_Zero[u++]);
	
    	} 
  	} 
}

 void Right_Seven_Five()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;
  
  
  	for (pages=0;pages<8;pages++)
	{ 

	 
    	LCDright(); 
    	lcd_command(64); 
    	lcd_command(184+pages); //184
    	for (i=0;i<64;i++)
		{ 
        
			lcd_data_write(Seven_Five[u++]);
	
    	} 
  	} 
}

  void Right_Nine_Five()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;
  
  
  	for (pages=0;pages<8;pages++)
	{ 

	 
    	LCDright(); 
    	lcd_command(64); 
    	lcd_command(184+pages); //184
    	for (i=0;i<64;i++)
		{ 
        
			lcd_data_write(Nine_Five[u++]);
	
    	} 
  	} 
}
 */

 /*
 void Right_Bad()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;
  
  
  	for (pages=0;pages<8;pages++)
	{ 

	 
    	LCDright(); 
    	lcd_command(64); 
    	lcd_command(184+pages); //184
    	for (i=0;i<64;i++)
		{ 
        
			lcd_data_write(empty[u++]);
	
    	} 
  	} 
}

*/
 // Right_Bad()
 //Right_Circle()
 //Right_Standby()

 /*
 void Right_Circle()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;
  
  
  	for (pages=0;pages<8;pages++)
	{ 

	 
    	LCDright(); 
    	lcd_command(64); 
    	lcd_command(184+pages); //184
    	for (i=0;i<64;i++)
		{ 
        
			lcd_data_write(Circle[u++]);
	
    	} 
  	} 
}
*/
 /*
  void Right_Standby()
{
	u8 bmp;
	unsigned int u = 0;
	unsigned char pages,i; 
	u16 x,y;
  
  
  	for (pages=0;pages<8;pages++)
	{ 

	 
    	LCDright(); 
    	lcd_command(64); 
    	lcd_command(184+pages); //184
    	for (i=0;i<64;i++)
		{ 
        
			lcd_data_write(Standby[u++]);
	
    	} 
  	} 
}

  */

//2015.4.23  ver 1.33a1
void display_TONG_EXIST(int status)
{
	lcd_cls();

	lcd_inverse();
	GLCD_string(0,2, "수거통이");
	if(status == 0) GLCD_string(0,4, "없습니다");
	else 			GLCD_string(0,4, "찼습니다");
	lcd_normal();
}

//2015.4.23  ver 1.33a1
void display_Image_Right(u8 *image)
{
	unsigned int u = 0;
	unsigned char pages, iz; 

  	for (pages=0;pages<8;pages++)
	{ 
  	    LCDright(); 
    	lcd_command(64); 
 		lcd_command(184+pages); //184
    	for (iz=0;iz<64;iz++)
		{ 
			lcd_data_write(image[u++]);
    	} 
 	} 
}
  
void lcd_clear(void)
{
	GLCD_string (0,0 ,"                ");
  GLCD_string (0,2 ,"                ");
  GLCD_string (0,4 ,"                ");
  GLCD_string (0,6 ,"                ");
 
}




/* end of file */
