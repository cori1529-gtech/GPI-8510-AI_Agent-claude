/*******************************************************************************
 * agent_weight_display.c
 *
 * Phase 2 extraction (see prj/CLAUDE.md section 6).
 * Moved verbatim from main.c (was Agent_WeightDisplay_Task()) - pure move,
 * no behavior change. Original Korean comments preserved byte-for-byte
 * (still CP949, per project encoding rule).
 *
 * NOTE: on the load-cell-overflow error path this function intentionally
 * never returns (infinite while(1){Delay_ms(10);} loop, matching the
 * original - a hard-fault-style halt, not a bug introduced by this move).
 *
 * Include order matters here: stm32f10x.h (pulls in stm32f10x_type.h,
 * which declares enum ErrorStatus { ERROR = 0, SUCCESS }) must come BEFORE
 * buzzer.h (which #define ERROR 3). If reversed, the buzzer.h macro would
 * textually corrupt the enum declaration itself when the preprocessor
 * reaches it. This matches the same order main.c already relies on.
 ******************************************************************************/

#include "stm32f10x.h"  /* s16/s32/u8/u16/u32 base types; ErrorStatus enum */
#include "stypes.h"
#include "type.h"       /* BOOL */
#include "var.h"        /* gxlWeight, gxlWeight_2 */
#include "i2c_ee.h"     /* gnlSilo1Selection, gnlLanguage */
#include "lcd.h"        /* lcd_clear() */
#include "hw_config.h"  /* Delay_ms() */
#include "buzzer.h"     /* Gpi8510_Relay_On(), ERROR macro - see note above */

/* still plain globals/functions in main.c - no header declares these yet */
extern char str[100];
extern u8   sign, sign_2;
extern BOOL Output_Low_Error, Output_Low_Error2, Output_High_Error;
extern void SendtoPc(void);
extern void Silo_1_Getweight(void);
extern void Silo_2_Getweight(void);

/* defined in lcd.c, not declared in lcd.h */
extern void GLCD_string(unsigned char x, unsigned char y, unsigned char *string);
extern void lcd_inverse(void);
extern void lcd_normal(void);

void Agent_WeightDisplay_Task(void)
{
    if(!Output_Low_Error && !Output_Low_Error2 && !Output_High_Error)
    {
        if(gnlSilo1Selection == 1)
        {
            SendtoPc();

            /* SILO 1 현재 무게 읽기 */
            Silo_1_Getweight();

            /* SILO 1 현재 무게 표시 */
            if(sign)
                sprintf(str, ":-%5ld kg ", gxlWeight);
            else
                sprintf(str, ": %5ld kg ", gxlWeight);

            GLCD_string(5, 0, str);

            /* 로드셀 이상값 검사 */
            if(gxlWeight > 99999)
            {
                lcd_clear();

                if(gnlLanguage == 1)
                {
                    GLCD_string(0, 0, "무게센서 에러!!");
                    GLCD_string(0, 2, "장비 전원을 끄고");
                    GLCD_string(0, 4, "무게센서를");
                    GLCD_string(0, 6, "점검해 주세요!");
                    Gpi8510_Relay_On(ERROR);
                }
                else
                {
                    GLCD_string(0, 0, "LOAD CELL ERROR!");
                    GLCD_string(0, 2, "POWER OFF !");
                    GLCD_string(0, 4, "CHECK");
                    GLCD_string(0, 6, "LOAD CELL !");
                    Gpi8510_Relay_On(ERROR);
                }

                while(1)
                {
                    Delay_ms(10);
                }
            }

            SendtoPc();
        }
        else if(gnlSilo1Selection == 2)
        {
            SendtoPc();

            /* SILO 1, SILO 2 현재 무게 읽기 */
            Silo_1_Getweight();
            SendtoPc();
            Silo_2_Getweight();
            SendtoPc();

            lcd_inverse();

            /*
             * 두 개의 SILO 무게를 한 줄에 표시한다.
             * sign/sign_2 값에 따라 음수 표시 형식을 다르게 처리한다.
             */
            if(sign && !sign_2)
            {
                if(gxlWeight > 9900)
                    gxlWeight = 9900;

                sprintf(str, "-%4ld/%5ld", (long)gxlWeight, (long)gxlWeight_2);
            }
            else if(!sign && sign_2)
            {
                if(gxlWeight_2 > 9900)
                    gxlWeight_2 = 9900;

                sprintf(str, "%5ld/-%4ld", (long)gxlWeight, (long)gxlWeight_2);
            }
            else if(sign && sign_2)
            {
                if(gxlWeight > 9900)
                    gxlWeight = 9900;

                if(gxlWeight_2 > 9900)
                    gxlWeight_2 = 9900;

                sprintf(str, "-%4ld/-%4ld", (long)gxlWeight, (long)gxlWeight_2);
            }
            else
            {
                sprintf(str, "%5ld/%5ld", gxlWeight, gxlWeight_2);
            }

            GLCD_string(5, 0, str);
            lcd_normal();

            /* 로드셀 이상값 검사 */
            if(gxlWeight > 99999 || gxlWeight_2 > 99999)
            {
                lcd_clear();

                if(gnlLanguage == 1)
                {
                    GLCD_string(0, 0, "무게센서 에러!!");
                    GLCD_string(0, 2, "장비 전원을 끄고");
                    GLCD_string(0, 4, "무게센서를");
                    GLCD_string(0, 6, "점검해 주세요!");
                    Gpi8510_Relay_On(ERROR);
                }
                else
                {
                    GLCD_string(0, 0, "LOAD CELL ERROR!");
                    GLCD_string(0, 2, "POWER OFF !");
                    GLCD_string(0, 4, "CHECK");
                    GLCD_string(0, 6, "LOAD CELL !");
                    Gpi8510_Relay_On(ERROR);
                }

                while(1)
                {
                    Delay_ms(10);
                }
            }
        }
    }
}

/* end of file */
