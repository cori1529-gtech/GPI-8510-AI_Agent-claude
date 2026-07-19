/*******************************************************************************
 * agent_discharge_calc.c
 *
 * Phase 2 extraction (see prj/CLAUDE.md section 6).
 * Moved verbatim from main.c (was Agent_DischargeCalc_Task()) - pure move,
 * no behavior change. Original Korean comments preserved byte-for-byte
 * (still CP949, per project encoding rule).
 *
 * gxuOutputHighErrorWeigh / gxuPulseSetWeigh / gxuOutputErrorWeigh are
 * declared in BOTH set.h and i2c_ee.h (i2c_ee.h has them commented out,
 * so no conflict there) - but set.h ALSO redeclares gnlDailyTotal and
 * gnlPreDailyTotal with a different signedness than var.h (a pre-existing
 * bug found while extracting agent_date_change.c). set.h is intentionally
 * NOT included here; the three gxu* variables are hand-declared extern
 * instead, matching set.h exact type (unsigned long).
 ******************************************************************************/

#include <stdio.h>

#include "stm32f10x.h"  /* s16/s32/u8/u16/u32 base types */
#include "stypes.h"
#include "type.h"       /* BOOL */
#include "var.h"        /* working_flag, working_flag_2, gxlWeight,
                           gxlWeight_2, gnlDailyTotal, gnlDailyTotal2 */
#include "i2c_ee.h"     /* gnlSilo1Selection, gnlLanguage, gnlFeeding1,
                           gnlFeeding2 */
#include "timer.h"      /* Feeding_Counter1, Feeding_Counter2, Pulse_timer */
#include "usart.h"      /* USART1_puts() */
#include "buzzer.h"     /* Gpi8510_Relay_On/Off(), SILO_ONE, PULSE, ERROR,
                           First_Relay_On/Off(), Second_Relay_On/Off() */
#include "lcd.h"        /* lcd_clear() */
#include "led.h"        /* Silo1_led_Off() */

#define AGENT_DISCHARGE_CALC_NONE   0
#define AGENT_DISCHARGE_CALC_STOP   1

/* declared in set.h too, but NOT included here - see file header note */
extern unsigned long gxuOutputHighErrorWeigh;
extern unsigned long gxuPulseSetWeigh;
extern unsigned long gxuOutputErrorWeigh;

/* still plain globals/functions in main.c - no header declares these yet */
extern char str[100];
extern u8   sign, ek_flag1;
extern BOOL Output_Low_Error, Output_High_Error;
extern int  Error_Sec_Counter;
extern long lPulseRef, lPulseRef2;
extern long lPulseWeigh, lPulseWeigh2;
extern long lOutput;
extern long UpperError_Weight;
extern long lPulseMaxHold, lPulseMaxHold2;
extern u8   lPulseMaxHoldCount, lPulseMaxHoldCount2;
extern long lRefWeigh;
extern BOOL bPulse, bPulse2;
extern void SendtoPc(void);
extern void Silo_1_Led_Operation(void);
extern void Silo_2_Led_Operation(void);
extern void Error_1_Led_Operation(void);
extern void Error_2_Led_Operation(void);

/* defined in lcd.c, not declared in lcd.h */
extern void GLCD_string(unsigned char x, unsigned char y, unsigned char *string);
extern void lcd_inverse(void);
extern void lcd_normal(void);

unsigned char Agent_DischargeCalc_Task(void)
{
    long diff;
	 int Pulse_timer2=0;

    /**************************************************************************
     * SILO 1 배출량 계산
     **************************************************************************/
    if(working_flag)
    {
        SendtoPc();

        Silo_1_Led_Operation();

        if(250 < Feeding_Counter1)
        {
            Feeding_Counter1 = 0;
            USART1_puts("interval !");
            lPulseMaxHoldCount = 0;

            if(!sign)
            {
                if(gxlWeight < lPulseRef)
                {
                    diff = lPulseRef - gxlWeight;

                    if(diff <= 100)
                    {
                        gnlDailyTotal += diff;
                        gnlFeeding1   += diff;
                        lPulseWeigh   += diff;
                        lOutput       += diff;

                        UpperError_Weight += diff;

                        lPulseRef = gxlWeight;
                        lPulseMaxHold = gxlWeight;
                        lPulseMaxHoldCount = 0;

                        if((gxuOutputHighErrorWeigh <= UpperError_Weight) &&
                           (gxuOutputHighErrorWeigh != 0) &&
                           !Output_High_Error)
                        {
													if(gnlSilo1Selection==1)
                            Gpi8510_Relay_Off(SILO_ONE);
                            Gpi8510_Relay_Off(ERROR);

                            Output_High_Error = 1;
                            Error_2_Led_Operation();

                            lcd_clear();

                            if(gnlLanguage == 1)
                            {
                                GLCD_string(0, 0, "상한배출 에러!");
                                GLCD_string(0, 2, "사료,배출모터");
                                GLCD_string(0, 4, "점검후 전원을");
                                GLCD_string(0, 6, "다시 켜주세요!");
                            }
                            else
                            {
                                GLCD_string(0, 0, " HIGH RELEASE");
                                GLCD_string(0, 2, "ERROR !!");
                                GLCD_string(0, 4, "CHECK   RELEASE");
                                GLCD_string(0, 6, "MOTOR !!");
                            }

                            Silo1_led_Off();
                            working_flag = 0;
                            ek_flag1 = 0;

                            return AGENT_DISCHARGE_CALC_STOP;
                        }

                        if(gnlSilo1Selection == 1)
                        {
                            lcd_inverse();

                            sprintf(str, " %5ld kg ", gnlFeeding1);
                            GLCD_string(6, 2, str);

                            sprintf(str, "오 늘: %5ld kg ", gnlDailyTotal);
                            GLCD_string(0, 6, str);

                            lcd_normal();
                        }
                        else if(gnlSilo1Selection == 2)
                        {
                            sprintf(str, "%5ld", gnlFeeding1);
                            GLCD_string(8, 2, str);

                            lcd_inverse();

                            sprintf(str, "오 늘: %5ld kg ", (gnlDailyTotal + gnlDailyTotal2));
                            GLCD_string(0, 6, str);

                            lcd_normal();
                        }
                    }
                }
            }
        }
    }

    /**************************************************************************
     * SILO 1 펄스 출력 처리
     **************************************************************************/
    if(gxuPulseSetWeigh)
    {
        if(!bPulse)
        {
            if((long)gxuPulseSetWeigh <= lPulseWeigh)
            {
                Pulse_timer = 0;
                lPulseWeigh -= (long)gxuPulseSetWeigh;
                bPulse = 1;
							  if(gnlSilo1Selection==1)
                Gpi8510_Relay_On(PULSE);
								else
								 First_Relay_On();	
            }
        }

        if(Pulse_timer >= 50)
        {
					
					 if(gnlSilo1Selection==1)
            Gpi8510_Relay_Off(PULSE);
					  else
						First_Relay_Off();	
        }

        if(Pulse_timer >= 100)
        {
            bPulse = 0;
        }
    }

    /**************************************************************************
     * SILO 1 하한 배출 에러 검사
     **************************************************************************/
    if(working_flag && ek_flag1 && !Output_Low_Error && gnlSilo1Selection == 1)
    {
        if(Error_Sec_Counter >= 60 && Error_Sec_Counter <= 63)
        {
            if((lRefWeigh - gxlWeight < (long)gxuOutputErrorWeigh) &&
               ((long)gxuOutputErrorWeigh != 0))
            {
							 if(gnlSilo1Selection==1)
                Gpi8510_Relay_Off(SILO_ONE);
                Gpi8510_Relay_Off(ERROR);

                Output_Low_Error = 1;
                Error_1_Led_Operation();

                lcd_clear();

                if(gnlLanguage == 1)
                {
                    GLCD_string(0, 0, "하한배출 에러!");
                    GLCD_string(0, 2, "사료,배출모터");
                    GLCD_string(0, 4, "점검후 전원을");
                    GLCD_string(0, 6, "다시 켜주세요!");
                }
                else
                {
                    GLCD_string(0, 0, " LOW RELEASE");
                    GLCD_string(0, 2, "ERROR !!");
                    GLCD_string(0, 4, "CHECK   RELEASE");
                    GLCD_string(0, 6, "MOTOR !!");
                }

                Silo1_led_Off();
                working_flag = 0;
                ek_flag1 = 0;

                return AGENT_DISCHARGE_CALC_STOP;
            }
        }
    }

    if(Output_Low_Error == 1)
    {
        Error_1_Led_Operation();
    }

    if(Output_High_Error == 1)
    {
        Error_2_Led_Operation();
    }

    /**************************************************************************
     * SILO 2 배출량 계산
     **************************************************************************/
    if(working_flag_2 && gnlSilo1Selection == 2)
    {
        SendtoPc();

        Silo_2_Led_Operation();

        lPulseMaxHoldCount2++;

        if(250 < Feeding_Counter2)
        {
            Feeding_Counter2 = 0;

            if(gxlWeight_2 < lPulseRef2)
            {
                diff = lPulseRef2 - gxlWeight_2;

                if(diff <= 100)
                {
                    gnlDailyTotal2 += diff;
                    lPulseWeigh2   += diff;
                    gnlFeeding2    += diff;

                    lPulseRef2 = gxlWeight_2;
                    lPulseMaxHold2 = gxlWeight_2;
                    lPulseMaxHoldCount = 0;
                }
            }

            sprintf(str, "%5ld", gnlFeeding2);
            GLCD_string(8, 4, str);

            lcd_inverse();

            sprintf(str, "오 늘: %5ld kg ", (gnlDailyTotal + gnlDailyTotal2));
            GLCD_string(0, 6, str);

            lcd_normal();
						/**************************************************************************
 * SILO 2 펄스 출력 처리
 *
 * 주의:
 *   Relay ON 후 같은 루프에서 바로 OFF 하면 실제 릴레이가 동작하지 않는다.
 *   SILO1과 동일하게 Pulse_timer를 이용해 일정 시간 유지 후 OFF 한다.
 **************************************************************************/

         
        } // of   
    
		if(gxuPulseSetWeigh)
   {
    if(!bPulse2)
    {
        if((long)gxuPulseSetWeigh <= lPulseWeigh2)
        {
            Pulse_timer = 0;
            lPulseWeigh2 -= (long)gxuPulseSetWeigh;
            bPulse2 = 1;

            if(gnlSilo1Selection == 1)
            {
                Gpi8510_Relay_On(PULSE);
            }
            else
            {
                Second_Relay_On();
            }
        }
    }

    if(Pulse_timer >= 50)
    {
        if(gnlSilo1Selection == 1)
        {
            Gpi8510_Relay_Off(PULSE);
        }
        else
        {
            Second_Relay_Off();
        }
    }

    if(Pulse_timer >= 100)
    {
        bPulse2 = 0;
    }
    }
		
		
		
		}

    return AGENT_DISCHARGE_CALC_NONE;
}

/* end of file */
