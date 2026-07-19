/*******************************************************************************
 * agent_discharge_stop.c
 *
 * Phase 2 extraction (see prj/CLAUDE.md section 6).
 * Moved verbatim from main.c (was Agent_DischargeStop_Task()) - pure move,
 * no behavior change. Original Korean comments preserved byte-for-byte
 * (still CP949, per project encoding rule).
 *
 * Writes to EEPROM via EE_Write_val() and shows totals on the LCD - this
 * touches daily-total/output state (same family as agent_date_change.c
 * and agent_weight_display.c), not the CAL.C calibration factors
 * themselves.
 ******************************************************************************/

#include <stdio.h>

#include "stm32f10x.h"  /* s16/s32/u8/u16/u32 base types */
#include "stypes.h"
#include "type.h"       /* BOOL, FALSE */
#include "var.h"        /* working_flag, working_flag_2, gxlWeight,
                           gxlWeight_2, gxlGrossWeight, gnlDailyTotal,
                           gnlDailyTotal2 */
#include "i2c_ee.h"     /* gnlSilo1Selection, gnlLanguage, gxhTotalOutput,
                           ADR_gxhTotalOutput, ADR_gnlFeeding1/2,
                           ADR_gnlDailyTotal/2, gnlFeeding1, gnlFeeding2 */
#include "led.h"        /* Silo1_led_Off(), Silo2_led_Off(), Silo2_led_On() */
#include "hw_config.h"  /* Delay_ms() */
#include "usart.h"      /* USART1_puts() */

/* still plain globals/functions in main.c/i2c_ee.c - no header declares
   these yet */
extern char str[100];
extern u8  stop_flag, stop_flag2;
extern u8  baichul_start_flag, baichul_start_flag2;
extern u8  baichul_end_flag, baichul_end_flag2;
extern s32 TodayTotalref, TodayTotalref_result, TodayTotalref_result2;
extern s32 Start_Weight, Stop_Weight, Start_Weight2, Stop_Weight2;
extern s32 Output_total, Output_total2;

/* defined in i2c_ee.c, not declared in i2c_ee.h */
extern u32 EE_Write_val(int adr, u32 val, u8 bytes);

/* defined in lcd.c, not declared in lcd.h */
extern void GLCD_string(unsigned char x, unsigned char y, unsigned char *string);
extern void lcd_inverse(void);
extern void lcd_normal(void);

void Agent_DischargeStop_Task(void)
{
	 long sum=0;
	
    /**************************************************************************
     * SILO 1 배출 종료 처리
     **************************************************************************/
    if(stop_flag)
    {
        working_flag = FALSE;
        Silo1_led_Off();

        stop_flag = 0;
        baichul_start_flag = 0;

        TodayTotalref_result = gnlDailyTotal;

        /*
         * 배출 시작 무게와 종료 무게 차이로 총누적 배출량 계산
         */
        Stop_Weight = gxlWeight;
        Output_total = Start_Weight - Stop_Weight;

        if(Output_total > 0)
        {
            gxhTotalOutput += Output_total;
            gxhTotalOutput = EE_Write_val(ADR_gxhTotalOutput, gxhTotalOutput, 4);
        }

        /*
         * 최근 급이량 EEPROM 저장
         */
        gnlFeeding1 = EE_Write_val(ADR_gnlFeeding1, gnlFeeding1, 4);
        Delay_ms(100);

        gnlFeeding2 = EE_Write_val(ADR_gnlFeeding2, gnlFeeding2, 4);
        Delay_ms(100);

        /*
         * 오늘 배출량이 유효 범위이면 EEPROM 저장
         * 기존 코드 기준 : 15000 미만 저장
         */
        if(TodayTotalref_result < 15000)
        {
            USART1_puts("silo1 current feeding quantity eeprom save start !");

            gnlDailyTotal = EE_Write_val(ADR_gnlDailyTotal, TodayTotalref_result, 4);
            Delay_ms(100);

            if(gnlSilo1Selection == 1)
            {
                if(gnlLanguage == 1)
                    sprintf(str, "오 늘: %5ld kg ", gnlDailyTotal);
                else
                    sprintf(str, "TODAY: %5ld kg", gnlDailyTotal);

                lcd_inverse();
                GLCD_string(0, 6, str);
                Delay_ms(3000);
                lcd_normal();
            }
            else if(gnlSilo1Selection == 2)
            {
                sum = 0;
                lcd_inverse();

                sum = gnlDailyTotal + gnlDailyTotal2;
                sprintf(str, "오 늘: %5ld kg", sum);

                GLCD_string(0, 6, str);
                lcd_normal();
            }

            USART1_puts("silo1 current feeding quantity eeprom save end !");
        }

        baichul_end_flag = 1;

        sprintf(str, "TodayTotalref=%6lu ", (long)TodayTotalref_result);
        USART1_puts(str);

        sprintf(str, "Stop_Weight : %d", Stop_Weight);
        USART1_puts(str);

        sprintf(str, "Start_Weight : %d", Start_Weight);
        USART1_puts(str);

        sprintf(str, "gxlGrossWeight : %d", gxlGrossWeight);
        USART1_puts(str);

        sprintf(str, "TodayTotalref : %d", TodayTotalref);
        USART1_puts(str);

        sprintf(str, "gnlDailyTotal : %d", gnlDailyTotal);
        USART1_puts(str);
    }

    /**************************************************************************
     * SILO 2 배출 종료 처리
     **************************************************************************/
    else if(stop_flag2)
    {
        working_flag_2 = FALSE;
        Silo2_led_Off();

        stop_flag2 = 0;
        baichul_start_flag2 = 0;

        Stop_Weight2 = gxlWeight_2;
        Output_total2 = Start_Weight2 - Stop_Weight2;

        if(Output_total2 > 0)
        {
            gxhTotalOutput += Output_total2;
            gxhTotalOutput = EE_Write_val(ADR_gxhTotalOutput, gxhTotalOutput, 4);
        }

        TodayTotalref_result2 = gnlDailyTotal2;

        /*
         * SILO2 오늘 배출량 EEPROM 저장
         */
        if(TodayTotalref_result2 >= 0 && TodayTotalref_result2 < 9000)
        {
            gnlDailyTotal2 = EE_Write_val(ADR_gnlDailyTotal2, TodayTotalref_result2, 4);
            Delay_ms(200);

            lcd_inverse();

            sum = gnlDailyTotal + gnlDailyTotal2;
            sprintf(str, "오 늘: %5ld kg", sum);

            GLCD_string(0, 6, str);
            lcd_normal();
        }

        sprintf(str, "TodayTotalref=%6lu ", (long)TodayTotalref_result2);
        USART1_puts(str);

        baichul_end_flag2 = 1;

        sprintf(str, "%ld", Stop_Weight2);
        USART1_puts(str);

        sprintf(str, "%ld", gnlDailyTotal2);
        USART1_puts(str);

        Delay_ms(100);

        /*
         * SILO2 종료 표시 LED 점멸
         */
        Silo2_led_Off();
        Delay_ms(200);

        Silo2_led_On();
        Delay_ms(200);

        Silo2_led_Off();
    }
} // Agent_DischargeStop_Task

/* end of file */
