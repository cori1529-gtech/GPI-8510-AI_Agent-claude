/*******************************************************************************
 * agent_date_change.c
 *
 * Phase 2 extraction (see prj/CLAUDE.md section 6).
 * Moved verbatim from main.c (was Agent_DateChange_Task()) - pure move,
 * no behavior change. Original Korean comments preserved byte-for-byte
 * (still CP949, per project encoding rule).
 *
 * This one has a much larger dependency surface than the two earlier
 * pilots (agent_minute_event.c, agent_discharge_stop_delay.c) - it touches
 * RTC, CDMA/modem, LCD display, and daily-total rollover state. Every
 * extern below was individually traced back to its definition in main.c,
 * i2c_ee.c, tcpip_send.c, or lcd.c before being listed here.
 ******************************************************************************/

#include "stm32f10x.h"  /* s16/s32/u8/u16/u32 base types */
#include "stypes.h"
#include "type.h"       /* BOOL */
#include "global.h"     /* DEF_1_MIN, GET_TIME_INTERFAL_TIME, CDMA_GET_TIME */
#include "var.h"        /* gnlPreDailyTotal, gnlPreDailyTotal2, gnlDailyTotal,
                           gnlDailyTotal2, gnuDay, gnuDay2, gxlWeight,
                           gxlWeight_2, jangbi, giToday */
#include "i2c_ee.h"     /* gnlCdmaUse, gnlSilo1Selection, gnlLanguage */
#include "timer.h"      /* Ten_Mili_Counter, Get1381(), Gettimebuf[] */
#include "usart.h"      /* USART1_puts() */
#include "hw_config.h"  /* delay_ms() */
#include "buzzer.h"     /* beep() */

/* still plain globals/functions in main.c - no header declares these yet */
extern u8   HT1381_year, HT1381_month, HT1381_date;
extern u8   HT1381_hour, HT1381_minute, HT1381_second;
extern char str[100];
extern char lte_restart_flag1, lte_restart_flag2;
extern u8   Once_flag, Once_flag_First, Once_flag_Second, Once_flag_Third;
extern BOOL Output_Low_Error, Output_Low_Error2, Output_High_Error;
extern void modem_power_reset(void);
extern void Silo_1_Weight_Display(void);

/* defined in i2c_ee.c, not declared in i2c_ee.h */
extern void save_DailyTotals(void);

/* defined in tcpip_send.c, not declared in tcpip_send.h */
extern int get_cdma_info_WAIT(int which, char *par);

/* defined in lcd.c, not declared in lcd.h */
extern void GLCD_string(unsigned char x, unsigned char y, unsigned char *string);
extern void lcd_inverse(void);
extern void lcd_normal(void);

void Agent_DateChange_Task(void)
{
	unsigned long sum = 0;
    unsigned char which_day_feeding = 0;

	
    if(Ten_Mili_Counter >= GET_TIME_INTERFAL_TIME)
    {
        Ten_Mili_Counter = 0;

        Get1381();

        HT1381_year   = Gettimebuf[6];
        HT1381_month  = Gettimebuf[4];
        HT1381_date   = Gettimebuf[3];
        HT1381_hour   = Gettimebuf[2];
        HT1381_minute = Gettimebuf[1];
        HT1381_second = Gettimebuf[0];

        sprintf(str,
                "date:20%02x-%02X-%02X  time:%02x-%02X-%02X ",
                Gettimebuf[6],
                Gettimebuf[4],
                Gettimebuf[3],
                Gettimebuf[2],
                Gettimebuf[1],
                Gettimebuf[0]);

        USART1_puts(str);

        if(gnlCdmaUse)
        {
            get_cdma_info_WAIT(CDMA_GET_TIME, "TIME");
            delay_ms(500);

            sprintf(str,
                    "kwakwonho=%02d:%02d:%02d",
                    jangbi.cdma.tm.hour,
                    jangbi.cdma.tm.min,
                    jangbi.cdma.tm.sec);

            /*
             * 오전 9시 모뎀 강제 리셋
             */
            if(!lte_restart_flag1 && jangbi.cdma.tm.hour == 9)
            {
                modem_power_reset();
                USART1_puts("modem restart !!!");
                lte_restart_flag1 = 1;
            }

            if(jangbi.cdma.tm.hour != 9)
            {
                lte_restart_flag1 = 0;
            }

            /*
             * 오후 3시 모뎀 강제 리셋
             */
            if(!lte_restart_flag2 && jangbi.cdma.tm.hour == 15)
            {
                modem_power_reset();
                USART1_puts("modem restart !!!");
                lte_restart_flag2 = 1;
            }

            if(jangbi.cdma.tm.hour != 15)
            {
                lte_restart_flag2 = 0;
            }
        }

        /**********************************************************************
         * 날짜 변경 검사
         **********************************************************************/
        if(giToday != HT1381_date)
        {
            beep(300);
            USART1_puts("Date is changed");

            giToday = HT1381_date;

            Once_flag        = 0;
            Once_flag_First  = 0;
            Once_flag_Second = 0;
            Once_flag_Third  = 0;

            /*
             * SILO1 7일 이력 이동
             */
            gnlPreDailyTotal[6] = gnlPreDailyTotal[5];
            gnlPreDailyTotal[5] = gnlPreDailyTotal[4];
            gnlPreDailyTotal[4] = gnlPreDailyTotal[3];
            gnlPreDailyTotal[3] = gnlPreDailyTotal[2];
            gnlPreDailyTotal[2] = gnlPreDailyTotal[1];
            gnlPreDailyTotal[1] = gnlPreDailyTotal[0];
            gnlPreDailyTotal[0] = gnlDailyTotal;

            /*
             * SILO2 7일 이력 이동
             */
            gnlPreDailyTotal2[6] = gnlPreDailyTotal2[5];
            gnlPreDailyTotal2[5] = gnlPreDailyTotal2[4];
            gnlPreDailyTotal2[4] = gnlPreDailyTotal2[3];
            gnlPreDailyTotal2[3] = gnlPreDailyTotal2[2];
            gnlPreDailyTotal2[2] = gnlPreDailyTotal2[1];
            gnlPreDailyTotal2[1] = gnlPreDailyTotal2[0];
            gnlPreDailyTotal2[0] = gnlDailyTotal2;

            gnuDay++;
            gnuDay2++;

            gnlDailyTotal  = 0;
            gnlDailyTotal2 = 0;

            save_DailyTotals();

            if(!Output_Low_Error && !Output_Low_Error2 && !Output_High_Error)
            {
                if(gnlSilo1Selection == 1)
                {
                    lcd_inverse();

                    if(gnlLanguage == 1)
                    {
                        sprintf(str, "오 늘: %5ld  kg", gnlDailyTotal);
                    }
                    else
                    {
                        sprintf(str, "TODAY: %5ld  kg", gnlDailyTotal);
                    }

                    GLCD_string(0, 6, str);
                    lcd_normal();

                    which_day_feeding = 0;
                    Silo_1_Weight_Display();
                }
                else
                {
                    if(gnlLanguage == 1)
                    {
                        lcd_inverse();

                        sprintf(str, "무게:%5ld-%5ld", gxlWeight, gxlWeight_2);
                        GLCD_string(0, 0, str);

                        lcd_normal();

                        sprintf(str, "급이량1:%5ld kg", gnlDailyTotal);
                        GLCD_string(0, 2, str);

                        sprintf(str, "급이량2:%5ld kg", gnlDailyTotal2);
                        GLCD_string(0, 4, str);

                        lcd_inverse();

                        sum = gnlDailyTotal + gnlDailyTotal2;
                        sprintf(str, "오 늘: %5ld kg", sum);
                        GLCD_string(0, 6, str);

                        lcd_normal();
                    }
                    else
                    {
                        lcd_inverse();

                        sprintf(str, "SILO:%5ld-%5ld", gxlWeight, gxlWeight_2);
                        GLCD_string(0, 0, str);

                        lcd_normal();

                        sprintf(str, "FEED1:%5ld kg", gnlDailyTotal);
                        GLCD_string(0, 2, str);

                        sprintf(str, "FEED2:%5ld kg", gnlDailyTotal2);
                        GLCD_string(0, 4, str);

                        lcd_inverse();

                        sum = gnlDailyTotal + gnlDailyTotal2;
                        sprintf(str, "TODAY: %5ld kg", sum);
                        GLCD_string(0, 6, str);

                        lcd_normal();
                    }
                }
            }

            modem_power_reset();
        }
    }
} // Agent_DateChange_Task

/* end of file */
