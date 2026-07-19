/*******************************************************************************
 * agent_make_baichul_packet.c
 *
 * Phase 2 extraction (see prj/CLAUDE.md section 6).
 * Moved verbatim from main.c (was Agent_MakeBaichulPacket()) - pure move,
 * no behavior change. Original Korean comments preserved byte-for-byte
 * (still CP949, per project encoding rule).
 *
 * Called from app/agent_baichul_send.c (extern-declared there).
 ******************************************************************************/

#include <stdio.h>
#include <string.h>

#include "stm32f10x.h"  /* s16/s32/u8/u16/u32 base types */
#include "stypes.h"
#include "type.h"
#include "def.h"        /* VERSION */
#include "var.h"        /* gnlSerialNo, gnlMyAddress, gnlTick, gxlWeight,
                           gxlWeight_2, gxlGrossWeight_2, gnlDailyTotal,
                           gnlDailyTotal2, gnlPreDailyTotal, gnlPreDailyTotal2,
                           working_flag, working_flag_2 */
#include "i2c_ee.h"     /* gnlSilo1Selection */

/* main.c-local macros (#define, not extern - safe to duplicate) */
#define STX 0X02
#define ETX 0X03

/* still plain main.c global - no header declares this yet */
extern char sT0[256];

void Agent_MakeBaichulPacket(char *bc_code)
{
    char send_code[8];

    /*
     * 안전 처리:
     * bc_code가 NULL이면 기본값으로 정기 상태 코드 사용
     */
    if(bc_code == 0)
    {
        strcpy(send_code, "BC-N");
    }
    else
    {
        strcpy(send_code, bc_code);
    }

    /**************************************************************************
     * SILO 1 단독 사용 패킷
     **************************************************************************/
    if(gnlSilo1Selection == 1)
    {
        sprintf(sT0,
                "%c|01212345678|M000|77777|M%07ld|M%03ld|%s|%s|%lx|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%c",
                STX,
                gnlSerialNo,
                gnlMyAddress,
                send_code,
                VERSION,
                gnlTick,
                gxlWeight,
                gnlDailyTotal % 100000,
                (long)gnlPreDailyTotal[0] % 100000,
                (long)gnlPreDailyTotal[1] % 100000,
                (long)gnlPreDailyTotal[2] % 100000,
                (long)gnlPreDailyTotal[3] % 100000,
                (long)gnlPreDailyTotal[4] % 100000,
                (long)gnlPreDailyTotal[5] % 100000,
                (long)gnlPreDailyTotal[6] % 100000,
                ((working_flag) ? 'W' : 'N'),
                gxlGrossWeight_2,
                gnlDailyTotal2 % 100000,
                (long)gnlPreDailyTotal2[0] % 100000,
                (long)gnlPreDailyTotal2[1] % 100000,
                (long)gnlPreDailyTotal2[2] % 100000,
                (long)gnlPreDailyTotal2[3] % 100000,
                (long)gnlPreDailyTotal2[4] % 100000,
                (long)gnlPreDailyTotal2[5] % 100000,
                (long)gnlPreDailyTotal2[6] % 100000,
                ((working_flag_2) ? 'W' : 'N'),
                ETX);
    }

    /**************************************************************************
     * SILO 2개 사용 패킷
     **************************************************************************/
    else
    {
        /*
         * 기존 코드에서는 일부 상황에서 BC-S를 강제로 사용했다.
         * 그러나 구조화 이후에는 호출자가 넘긴 bc_code를 그대로 사용한다.
         *
         * 필요하면 호출부에서 Agent_MakeBaichulPacket("BC-S")로 호출하면 된다.
         */
        sprintf(sT0,
                "%c|01212345678|M000|77777|M%07ld|M%03ld|%s|%s|%lx|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%c",
                STX,
                gnlSerialNo,
                gnlMyAddress,
                send_code,
                VERSION,
                gnlTick,
                (gxlWeight + gxlWeight_2),
                (gnlDailyTotal + gnlDailyTotal2) % 100000,
                (long)(gnlPreDailyTotal[0] + gnlPreDailyTotal2[0]) % 100000,
                (long)(gnlPreDailyTotal[1] + gnlPreDailyTotal2[1]) % 100000,
                (long)(gnlPreDailyTotal[2] + gnlPreDailyTotal2[2]) % 100000,
                (long)(gnlPreDailyTotal[3] + gnlPreDailyTotal2[3]) % 100000,
                (long)(gnlPreDailyTotal[4] + gnlPreDailyTotal2[4]) % 100000,
                (long)(gnlPreDailyTotal[5] + gnlPreDailyTotal2[5]) % 100000,
                (long)(gnlPreDailyTotal[6] + gnlPreDailyTotal2[6]) % 100000,
                ((working_flag) ? 'W' : 'N'),
                gxlGrossWeight_2,
                gnlDailyTotal2 % 100000,
                (long)gnlPreDailyTotal2[0] % 100000,
                (long)gnlPreDailyTotal2[1] % 100000,
                (long)gnlPreDailyTotal2[2] % 100000,
                (long)gnlPreDailyTotal2[3] % 100000,
                (long)gnlPreDailyTotal2[4] % 100000,
                (long)gnlPreDailyTotal2[5] % 100000,
                (long)gnlPreDailyTotal2[6] % 100000,
                ((working_flag_2) ? 'W' : 'N'),
                ETX);
    }
} // Agent_MakeBaichulPacket

/* end of file */
