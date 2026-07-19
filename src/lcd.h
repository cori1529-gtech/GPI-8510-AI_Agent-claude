#ifndef __LCD_H
#define __LCD_H

#include "stypes.h"
#include "board_gpi8510.h"

/* Includes ------------------------------------------------------------------*/



/******** 128*64**************************/
 
//#include "stm32f10x_map.h"
#define	ONLY_1st	1
#define ONLY_2nd	2

//#define	LCD_CONTROL	0x01
//#define	LCD_DATA	0x05
#define LCD_DATA      0x0800
#define LCD_CONTROL   0x007F

#define GPIO_LCD_DATA                    GPIOG    
/*
volatile u8 INVERSE;
volatile unsigned char   HAN_CODE;
volatile unsigned char   FONT_SIZE;             
volatile unsigned char   FONT_STYLE;
                         
volatile u16 xcharacter;
volatile u16 ycharacter;
volatile u16 xgraphic;
volatile u16 ygraphic;
volatile u16 cur_x;
volatile u16 cur_y;
volatile u16 xch;		//character x
volatile u16 ych;		//character y
volatile u16 xgr;		//graphic x
volatile u16 ygr;		//graphic y
volatile u16 xcur;		//cursor x
volatile u16 ycur;		//cursor y

volatile int cx, cy; 	//2014.8.27
volatile u8 LCX, LCY;
volatile u8 LCDPAGE;
  */

void    lcdbyte(u8 control, u8 value);
void	lcd_cls(void);
void  lcd_clear(void);
void 	cursor_on(void);
void 	cursor_off(void);
void	goto_xy (u8 x, u8 y);
void	lcd_initial (void);
//void 	lputs_xy(u8 r, u8 c, flash char *str);
void 	lputs_xy(u8 r, u8 c,  char *str);
void 	lputstr_xy(u8 r, u8 c, char *str);
void LCDright(void);
void LCDleft(void);
void lcd_data_write(u16 ch);
void lcd_command(u16 cmd);
void lcd_goto_y(u8 y);
void lcd_gotoxy(u8 x, u8 y);
/*********128*64*************************/

 



			 
#endif  
	 



