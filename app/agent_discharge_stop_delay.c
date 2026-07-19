/*******************************************************************************
 * agent_discharge_stop_delay.c
 *
 * Phase 2 extraction (see prj/CLAUDE.md section 6).
 * Moved verbatim from main.c (was Agent_DischargeStopDelay_Task()) - pure
 * move, no behavior change. Original Korean comments preserved byte-for-byte
 * (still CP949, per project encoding rule).
 *
 * AGENT_STOP_DELAY_NONE/DONE are return-code macros. main.c keeps its own
 * copy (used at the call site, ~line 1208) - duplicate #define across
 * translation units is fine in C, unlike duplicate globals.
 ******************************************************************************/

#include "stm32f10x.h"  /* s16/s32/u8/u16/u32 base types */
#include "stypes.h"
#include "type.h"
#include "global.h"     /* DEF_1_SEC */
#include "var.h"        /* working_flag, working_flag_2 */
#include "timer.h"      /* Sec_Mili_Counter */

#define AGENT_STOP_DELAY_NONE   0
#define AGENT_STOP_DELAY_DONE   1

/* still plain globals in main.c - no header declares these yet */
extern int Error_Sec_Counter;
extern int Error_Sec_Counter2;
extern u8  stop_flag;
extern u8  stop_flag2;
extern u8  byNewStart;
extern u8  byNewStart2;

unsigned char Agent_DischargeStopDelay_Task(void)
{
    /**************************************************************************
     * 1초 주기 처리
     **************************************************************************/
    if(Sec_Mili_Counter >= DEF_1_SEC)
    {
        Sec_Mili_Counter = 0;

        /*
         * 배출 시작 후 하한 배출 에러 판단에 사용되는 시간 카운터
         */
        Error_Sec_Counter++;
        Error_Sec_Counter2++;

        /*
         * SILO1 종료 지연 카운트 감소
         */
        if(working_flag && stop_flag)
        {
            if(byNewStart)
            {
                byNewStart--;
            }
        }

        /*
         * SILO2 종료 지연 카운트 감소
         */
        if(working_flag_2 && stop_flag2)
        {
            if(byNewStart2)
            {
                byNewStart2--;
            }
        }
    }

    /**************************************************************************
     * SILO1 종료 조건 확인
     **************************************************************************/
    if(working_flag && stop_flag)
    {
        if(!byNewStart)
        {
            return AGENT_STOP_DELAY_DONE;
        }
    }

    /**************************************************************************
     * SILO2 종료 조건 확인
     **************************************************************************/
    if(working_flag_2 && stop_flag2)
    {
        if(!byNewStart2)
        {
            return AGENT_STOP_DELAY_DONE;
        }
    }

    return AGENT_STOP_DELAY_NONE;
} // Agent_DischargeStopDelay_Task

/* end of file */
