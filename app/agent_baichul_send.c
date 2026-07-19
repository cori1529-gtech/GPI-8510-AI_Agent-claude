/*******************************************************************************
 * agent_baichul_send.c
 *
 * Phase 2 extraction (see prj/CLAUDE.md section 6).
 * Moved verbatim from main.c (was Agent_BaichulSend_Task()) - pure move,
 * no behavior change. Original Korean comments preserved byte-for-byte
 * (still CP949, per project encoding rule).
 *
 * main.c still has a dead duplicate of this function wrapped in
 * "#ifdef dpdpd" (dpdpd is never defined anywhere - permanently disabled).
 * Left untouched, per the rule not to move/delete commented-out code
 * without explicit approval.
 ******************************************************************************/

#include "stm32f10x.h"  /* s16/s32/u8/u16/u32 base types */
#include "stypes.h"
#include "type.h"
#include "global.h"     /* bachul_send_timer_ms, WORKING_SEND_INTERVAL, DEF_1_MIN */
#include "var.h"        /* working_flag, SEND_BAICHUL */
#include "i2c_ee.h"     /* gnlSendingInterval */
#include "timer.h"      /* Sending_Mili_Counter */

/* still plain globals/functions in main.c - no header declares these yet */
extern u8   baichul_start_flag, baichul_end_flag;
extern char sT0[256];
extern void Agent_MakeBaichulPacket(char *bc_code);

/* defined in usart.c, not declared in usart.h */
extern void USART2_puts(char *str);

/* defined in tcpip_send.c, not declared in tcpip_send.h; test_routine()
   itself IS already declared via global.h above */
extern void Send_to_server(int msg_type, char *msg);

void Agent_BaichulSend_Task(void)
{
    /**************************************************************************
     * 배출 시작 정보 전송
     **************************************************************************/
    if(baichul_start_flag)
    {
        USART2_puts("Baichul start !!");

        baichul_start_flag = 0;

        Send_to_server(SEND_BAICHUL, "BC-B");

        Agent_MakeBaichulPacket("BC-B");

        if(gnlSendingInterval > 0)
        {
            USART2_puts(sT0);
        }
    }

    /**************************************************************************
     * 배출 중 정보 전송
     **************************************************************************/
    else if(working_flag)
    {
        if(!bachul_send_timer_ms)
        {
            USART2_puts("Baichul jung sending !!");

            bachul_send_timer_ms = WORKING_SEND_INTERVAL;

            Send_to_server(SEND_BAICHUL, "BC-J");

            Agent_MakeBaichulPacket("BC-J");

            if(gnlSendingInterval > 0)
            {
                USART2_puts(sT0);
            }
        }
    }

    /**************************************************************************
     * 배출 완료 정보 전송
     **************************************************************************/
    else if(baichul_end_flag)
    {
        USART2_puts("Baichul end !!");

        baichul_end_flag = 0;

        Agent_MakeBaichulPacket("BC-E");

        if(gnlSendingInterval > 0)
        {
            USART2_puts(sT0);
        }

        Send_to_server(SEND_BAICHUL, "BC-E");
    }

    /**************************************************************************
     * 정기 상태 정보 전송
     *
     * 주의:
     *   gnlSendingInterval == 0이면 주기 전송을 하지 않는다.
     *   기존 구조화 과정에서 gnlSendingInterval이 0일 경우
     *   조건식이 항상 참이 되어 BC-N이 반복 송신될 수 있었다.
     **************************************************************************/
    if((gnlSendingInterval > 0) &&
       (Sending_Mili_Counter >= (DEF_1_MIN * gnlSendingInterval)))
    {
        Agent_MakeBaichulPacket("BC-N");

        Send_to_server(SEND_BAICHUL, "BC-N");

        test_routine();

        USART2_puts(sT0);

        Sending_Mili_Counter = 0;
    }

    /* Unconditional service call for the CDMA/LTE send state machine
     * (tcpip_send_20140806() -> tcpip_send_form_zigbee_data()). Restored
     * to match main.c's dead #ifdef dpdpd duplicate of this function,
     * which called test_routine() unconditionally every invocation.
     * Without this, BC-B/BC-J/BC-E events only got serviced once every
     * gnlSendingInterval minutes (via the BC-N block above), delaying
     * their actual transmission over the modem. */
    test_routine();
} // Agent_BaichulSend_Task

/* end of file */
