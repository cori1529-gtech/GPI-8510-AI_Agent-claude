/*******************************************************************************
 * agent_minute_event.c
 *
 * Phase 2 pilot extraction (see prj/CLAUDE.md section 6).
 * Moved verbatim from main.c (was Agent_MinuteEvent_Task(), originally near
 * the HT1381 RTC read block) - pure move, no behavior change. Original
 * Korean comments inside the function body are preserved byte-for-byte
 * (still CP949, per project encoding rule).
 *
 * The extern declarations below are needed because most of the globals this
 * function touches are still plain main.c globals with no header exposing
 * them (str, sT0, HT1381_*, poweron_date_set_counter, SendtoPc, UART4_puts).
 * They are listed here rather than silently duplicated, per the Phase 2
 * rule: "globalize now, encapsulate later" - do not remove/rename them.
 ******************************************************************************/

#include <stdio.h>

#include "stm32f10x.h"  /* provides s16/s32/u8/u16/u32 (var.h needs s16) */
#include "stypes.h"
#include "type.h"
#include "var.h"     /* gbMinEvent, gbWebSMSConnection, gbGetCdmaTime,
                          gnlUseLTE, giToday, jangbi, rtc_hour/min/sec */
#include "i2c_ee.h"  /* gnlCdmaUse */
#include "timer.h"   /* Get1381(), Gettimebuf[] */
#include "usart.h"   /* USART1_puts() */

/* still plain globals in main.c - no header declares these yet */
extern u8   HT1381_year, HT1381_month, HT1381_date;
extern u8   HT1381_hour, HT1381_minute, HT1381_second;
extern int  poweron_date_set_counter;
extern char str[100];
extern char sT0[256];
extern void SendtoPc(void);
extern void UART4_puts(char *str);

void Agent_MinuteEvent_Task(void)
{
    /*
     * gbMinEvent는 rtc.c 등에서 1분마다 SET 되는 플래그로 판단된다.
     * 플래그가 없으면 아무 처리도 하지 않는다.
     */
    if(gbMinEvent)
    {
        /**********************************************************************
         * HT1381 RTC 시간 읽기
         **********************************************************************/
        Get1381();

        HT1381_year   = Gettimebuf[6];
        HT1381_month  = Gettimebuf[4];
        HT1381_date   = Gettimebuf[3];
        HT1381_hour   = Gettimebuf[2];
        HT1381_minute = Gettimebuf[1];
        HT1381_second = Gettimebuf[0];

        sprintf(str,
                " : giToday=%02x   1381_DATA=%02X",
                giToday,
                HT1381_date);

        USART1_puts(str);

        /**********************************************************************
         * CDMA/LTE 사용 시 처리
         **********************************************************************/
        if(gnlCdmaUse)
        {
            /*
             * 1분 이벤트 처리 완료
             */
            gbMinEvent = 0;

            /*
             * WebSMS 서버 연결 확인 요청
             */
            gbWebSMSConnection = 1;

            /*
             * CDMA 시간 읽기 요청
             */
            gbGetCdmaTime = 1;

            /*
             * LTE 사용 시 현재 날짜/시간을 옵션보드 또는 외부 장치로 전송
             */
            if(gnlUseLTE)
            {
                sprintf(sT0,
                        "AT+BC=$DATE=%04d%02d%02d%02d%02d%02d@",
                        jangbi.cdma.tm.year,
                        jangbi.cdma.tm.month,
                        jangbi.cdma.tm.date,
                        rtc_hour,
                        rtc_min,
                        rtc_sec);

                UART4_puts(sT0);
                USART1_puts(sT0);
            }

            /*
             * 전원 투입 후 일정 시간 동안 날짜를 현재 RTC 날짜로 맞춘다.
             */
            if(poweron_date_set_counter)
            {
                poweron_date_set_counter--;
                giToday = HT1381_date;
            }
        }

        /**********************************************************************
         * CDMA/LTE 미사용 시 처리
         **********************************************************************/
        else
        {
            SendtoPc();

            gbMinEvent = 0;

            USART1_puts("One minute rotaion");

            if(poweron_date_set_counter)
            {
                poweron_date_set_counter--;
                giToday = HT1381_date;

                sprintf(str,
                        "One_minutes rotation : giToday=%02x   1381_DATA=%02X",
                        giToday,
                        HT1381_date);

                USART1_puts(str);
            }
        }
    }
} // Agent_MinuteEvent_Task

/* end of file */
