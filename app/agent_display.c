/*******************************************************************************
 * agent_display.c
 *
 * Moved verbatim from main.c (main.c slimming, see prj/CLAUDE.md 10.2).
 * Pure move, no behavior change. Korean comments preserved byte-for-byte
 * (CP949, per project encoding rule).
 *
 * Contents: Silo_1_Weight_Display(), Silo_2_Weight_Display(),
 * Date_Time_Display(), Initial_Display(), logo() - pure LCD text output,
 * no calibration variables (gnfFactor/v_zero/v_span) touched.
 *
 * NOT included in this batch (deliberately held back for separate review):
 * Silo_1_Getweight() reads gnfFactor directly; cal_display() and
 * Show_ToalOutput() are interactive menus with blocking key-read loops.
 ******************************************************************************/

#include <stdio.h>

#include "stm32f10x.h"
#include "stypes.h"
#include "type.h"
#include "def.h"        /* VERSION */
#include "var.h"        /* gxlWeight, gxlWeight_2, gnlDailyTotal,
                           gnlDailyTotal2 */
#include "i2c_ee.h"     /* gnlLanguage, gnlFeeding1, gnlFeeding2,
                           gnlSilo1Selection */
#include "timer.h"      /* Get1381(), Gettimebuf[] */
#include "usart.h"      /* USART1_puts() */
#include "hw_config.h"  /* Delay_ms() */
#include "lcd.h"        /* lcd_initial() */

/* still plain globals/functions in main.c - no header declares these yet */
extern char str[100];
extern char Loadcell_Error1;
extern void SendtoPc(void);

/* defined in lcd.c, not declared in lcd.h. NOTE: GLCD_string() is
   intentionally declared with an old-style empty-parens prototype (no
   argument checking) because one call site below (the Loadcell_Error1
   branch in Silo_1_Weight_Display) passes only 1 argument instead of the
   real 3 (unsigned char x, y, unsigned char *string) - a pre-existing
   quirk in the original main.c that relied on implicit declaration to
   avoid a compile error. A strict prototype here would newly break that
   call site - not this pure-move steps job to fix. */
extern void GLCD_string();
extern void lcd_inverse(void);
extern void lcd_normal(void);

void  Silo_1_Weight_Display() //0908  // 0627
{
	
	//lcd_initial();
 
 if(gnlLanguage==1) // korean
 { 

	if(Loadcell_Error1)
   	 GLCD_string ("로드셀 에러");
	
  else//정상일때	
  {  
	 sprintf(str, "무 게: %5ld kg ", gxlWeight);
	 //  sprintf(str, ": %5ld kg ", gxlWeight);	
   	
		GLCD_string (0,0 ,str);
	} 
	lcd_inverse();	
  

	//sprintf(str, "급 이: %5ld kg ", gnlDailyTotal);
	// if(!working_flag)
   {		 
	   sprintf(str, "급 이: %5ld kg ", gnlFeeding1);
  	 GLCD_string (0,2 ,str);
	 } 
	//sprintf(str, "오 늘: %5ld kg ", gnlDailyTotal);
	//GLCD_string (0,6 ,str);

	 lcd_normal();	
 
	
	}//of if(gnlLanguage==1)
 
  else
 {
  if(Loadcell_Error1)
   	 GLCD_string ("LOAD_CELL ERROR!");
	 else
   {		 
   	sprintf(str, "SILO : %5ld kg ", gxlWeight);
  	GLCD_string (0,0 ,str);
   }	

	lcd_inverse();	
	//sprintf(str, "FEED : %5ld kg ", gnlDailyTotal);
	 sprintf(str, "FEED : %5ld kg ", gnlFeeding1);
	GLCD_string (0,2 ,str);
	 lcd_normal();	
 }
 
 
 

}void  Silo_2_Weight_Display()
{
	lcd_initial();
	sprintf(str, "SILO2: %5ld kg ", gxlWeight_2);
	GLCD_string (0,4 ,str);
	 lcd_inverse();	
	sprintf(str, "FEED2: %5ld kg ", gnlDailyTotal2);
	GLCD_string (0,6 ,str);
	 lcd_normal();	
}void Date_Time_Display(void)
{
  
	Get1381(); 
	sprintf(str, "date:20%02x-%02X-%02X  time:%02x-%02X-%02X ", Gettimebuf[6],Gettimebuf[4],Gettimebuf[3],Gettimebuf[2],Gettimebuf[1],Gettimebuf[0]);	USART1_puts(str);
  if(gnlLanguage==1)
	{	
	 GLCD_string (0,0 ,"현재날짜");
   sprintf(str, "20%02x년%02X월%02X일", Gettimebuf[6],Gettimebuf[4],Gettimebuf[3]);	
	 lcd_inverse();
 	 GLCD_string (0,2 ,str);
   lcd_normal();	 
   GLCD_string (0,4 ,"현재시간");
   lcd_inverse();
	 sprintf(str, "%02x시%02X분%02X초", Gettimebuf[2],Gettimebuf[1],Gettimebuf[0]);	
   GLCD_string (0,6 ,str);
	 lcd_normal();
	}
else
		
	{	
	GLCD_string (0,0 ,"    DATE        ");
  sprintf(str, "  20%02x-%02X-%02X    ", Gettimebuf[6],Gettimebuf[4],Gettimebuf[3]);	
	lcd_inverse();
	GLCD_string (0,2 ,str);
  lcd_normal();	 
  GLCD_string (0,4 ,"    TIME        ");
  lcd_inverse();
	sprintf(str, "  %02x-%02X-%02X      ", Gettimebuf[2],Gettimebuf[1],Gettimebuf[0]);	
  GLCD_string (0,6 ,str);
	 lcd_normal();
	}
		Delay_ms(1000);
}void Initial_Display(void)
{
	long  sum;
 /**************  Initial display *****************************/
 if(gnlSilo1Selection==1)// silo  NUMBER ==1
 {
	 SendtoPc();
   Silo_1_Weight_Display(); // silo 1  current weight, feed comsumption weight displaying 
	 SendtoPc();
	 lcd_inverse();
	 if(gnlLanguage==1) // korean 
	 sprintf(str, "오 늘: %5ld kg ", gnlDailyTotal); 
	 else
	 sprintf(str, "TODAY: %5ld kg ", gnlDailyTotal);
	 
	 GLCD_string (0,6 ,str);
	 lcd_normal();

 }
else // silo number is 2  20171019
 {
	if(gnlLanguage==1)
  {		
	 lcd_inverse(); 
   sprintf(str, "무게:%5ld-%5ld", gxlWeight,gxlWeight_2);
	 GLCD_string (0,0 ,str);
	 lcd_normal();
    /*	
		sprintf(str, "급이량1:%5ld kg", gnlDailyTotal);
	 GLCD_string (0,2 ,str);
	 sprintf(str, "급이량2:%5ld kg", gnlDailyTotal2);
	 GLCD_string (0,4 ,str);
		*/
   //ver 2.8
		sprintf(str, "급이량1:%5ld kg", gnlFeeding1);
	 GLCD_string (0,2 ,str);
	 sprintf(str, "급이량2:%5ld kg", gnlFeeding2);
	 GLCD_string (0,4 ,str);



		lcd_inverse();
	 sum=gnlDailyTotal+gnlDailyTotal2;
	 sprintf(str, "오 늘: %5ld kg",sum);
 //sprintf(str, "오 늘: %5ld kg ", gnlDailyTotal); 
	 GLCD_string (0,6 ,str);
	 lcd_normal();
	}
	else
   {		
	 lcd_inverse(); 
   sprintf(str, "SILO:%5ld-%5ld", gxlWeight,gxlWeight_2);
	 GLCD_string (0,0 ,str);
	 lcd_normal();
	 sprintf(str, "FEED1:%5ld kg", gnlDailyTotal);
	 GLCD_string (0,2 ,str);
	 sprintf(str, "FEED2:%5ld kg", gnlDailyTotal2);
	 GLCD_string (0,4 ,str);
	 lcd_inverse();
	 sum=gnlDailyTotal+gnlDailyTotal2;
	 sprintf(str, "TODAY: %5ld kg",sum);
 //sprintf(str, "오 늘: %5ld kg ", gnlDailyTotal); 
	 GLCD_string (0,6 ,str);
	 lcd_normal();
	}
 }
}void logo(void)
{
	#define Y_AXIS_SPACE   50
	char str[20];

//POINT_COLOR = BLACK;
//BACK_COLOR = WHITE;

	//lcd70_show_string_mode_24x24(200, 100+(Y_AXIS_SPACE*1),"MODEL:GPI-8310", 16, 0);
	sprintf(str, "VERSION:%s", VERSION);
	//lcd70_show_string_mode_24x24(200, 100+(Y_AXIS_SPACE*2), str, 16, 0);
	//lcd70_show_string_mode_24x24(200, 100+(Y_AXIS_SPACE*3),"G-TECH INTERNATIONAL CO., ", 16, 0);
}

/* end of file */
