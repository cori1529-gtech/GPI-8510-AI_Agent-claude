/*******************************************************************************
 * agent_discharge_input.c
 *
 * Phase 2 extraction (see prj/CLAUDE.md section 6).
 * Moved verbatim from main.c (was Agent_DischargeInput_Task()) - pure move,
 * no behavior change. Original Korean comments preserved byte-for-byte
 * (still CP949, per project encoding rule).
 *
 * main.c still has TWO dead duplicates of this function right where this
 * one used to live: one wrapped in "#ifdef dpddpp" (dpddpp is never
 * defined - permanently disabled) and one wrapped in a block comment.
 * Both use a
 * different, AGENT_DischargeInput_Process()-based implementation. Left
 * untouched, per the rule not to move/delete commented-out code without
 * explicit approval.
 *
 * GPIOC Pin_7/Pin_8 are the same physical signals documented as
 * IN1_PIN/IN2_PIN in board_gpi8510.h, but this move keeps the original
 * raw GPIO_Pin_7/GPIO_Pin_8 literals unchanged (pure move first, BSP-macro
 * cleanup would be a separate follow-up step).
 ******************************************************************************/

#include "stm32f10x.h"  /* s16/s32/u8/u16/u32 base types; GPIO_ReadInputDataBit */
#include "stypes.h"
#include "type.h"       /* BOOL */
#include "var.h"        /* gxhEndDelay */
#include "i2c_ee.h"     /* gnlSilo1Selection */
#include "usart.h"      /* USART1_puts() */
#include "buzzer.h"     /* First_Relay_On(), First_Relay_Off() */

#define AGENT_DISCHARGE_TASK_NONE          0
#define AGENT_DISCHARGE_TASK_FIRST_START   1
#define AGENT_DISCHARGE_TASK_SECOND_START  2

/* still plain globals in main.c - no header declares these yet */
extern u8   ek_flag1, ek_flag2;
extern char start_sign_flag, start_sign_flag_2;
extern u8   sign, sign_2;
extern u8   stop_flag, stop_flag2;
extern u8   byNewStart, byNewStart2;
extern BOOL Output_Low_Error, Output_High_Error;
extern BOOL Output_Low_Error2, Output_High_Error2;

unsigned char Agent_DischargeInput_Task(void)
{
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) == 0x00)
    {
        if(!ek_flag1)
        {
            if(!Output_Low_Error && !Output_High_Error)
            {
                if(sign)
                    start_sign_flag = 1;
                else
                    start_sign_flag = 0;

                ek_flag1 = 1;
                stop_flag = 0;
               if(gnlSilo1Selection==1) // silo 선택 한개 일때만
                First_Relay_On();

                USART1_puts("FIRST DIRECT START\r\n");

                return AGENT_DISCHARGE_TASK_FIRST_START;
            }
        }
    }
    else
    {
        if(ek_flag1)
        {
					if(gnlSilo1Selection==1) // silo 선택 한개 일때만
            First_Relay_Off();

            byNewStart = gxhEndDelay;
            ek_flag1 = 0;
            stop_flag = 1;

            USART1_puts("FIRST DIRECT STOP\r\n");
        }
    }

    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) == 0x00)
    {
        if(!ek_flag2)
        {
            if(!Output_Low_Error2 && !Output_High_Error2)
            {
                if(sign_2)
                    start_sign_flag_2 = 1;
                else
                    start_sign_flag_2 = 0;

                ek_flag2 = 1;
                stop_flag2 = 0;

                //Second_Relay_On();

                USART1_puts("SECOND DIRECT START\r\n");

                return AGENT_DISCHARGE_TASK_SECOND_START;
            }
        }
    }
    else
    {
        if(ek_flag2)
        {
            //Second_Relay_Off();

            byNewStart2 = gxhEndDelay;
            ek_flag2 = 0;
            stop_flag2 = 1;

            USART1_puts("SECOND DIRECT STOP\r\n");
        }
    }

    return AGENT_DISCHARGE_TASK_NONE;
} // Agent_DischargeInput_Task

/* end of file */
