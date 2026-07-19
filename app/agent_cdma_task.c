/*******************************************************************************
 * agent_cdma_task.c
 *
 * Phase 2 extraction (see prj/CLAUDE.md section 6).
 * Moved verbatim from main.c (was Agent_CdmaTask()) - pure move, no
 * behavior change. Original Korean comments preserved byte-for-byte
 * (still CP949, per project encoding rule).
 ******************************************************************************/

#include <stdio.h>
#include <string.h>

#include "stm32f10x.h"  /* s16/s32/u8/u16/u32 base types */
#include "stypes.h"
#include "type.h"
#include "global.h"     /* giGigbeeSendingFlag, gszSendingGigbee,
                           CdmaSendQueue, CdmaSendQueue_qhead/qtail,
                           giSendingGigbeeLength */
#include "var.h"        /* gnlUseLTE, jangbi, Usart3_sms_received_flag */
#include "i2c_ee.h"     /* gnlCdmaUse */
#include "debug.h"      /* DEBUG_puts (== USART1_puts) */
#include "usart.h"      /* USART1_puts() */

/* still plain globals/functions in main.c/usart.c - no header declares
   these yet */
extern char sT0[256];
extern char RxCdmaTokenQ_tmp[256];
extern int  RxCdmaTokenQ_index;
extern void SMS_PROCESS(void);
extern int  CdmaSendQueue_gets(void);

/* defined in usart.c, not declared in usart.h */
extern int  RxCdmaQ_get(void);
extern void USART1_putchar(char ch);
extern int  RxCdmaTokenQ_puts(char *txt);

void Agent_CdmaTask(void)
{
    int iz;
    int jz;
    int kz;

    if(gnlCdmaUse)
    {
        /**********************************************************************
         * CDMA 荐脚 单捞磐 贸府
         **********************************************************************/
        for(iz = 0; ; iz++)
        {
            iz = RxCdmaQ_get();

            if(iz == -1)
            {
                break;
            }
            else
            {
                USART1_putchar(iz);

                RxCdmaTokenQ_tmp[RxCdmaTokenQ_index++] = iz;

                if(iz == 0x0a)
                {
                    RxCdmaTokenQ_tmp[RxCdmaTokenQ_index] = NULL;
                    RxCdmaTokenQ_puts(RxCdmaTokenQ_tmp);
                    RxCdmaTokenQ_index = 0;
                    break;
                }
            }
        }

        /**********************************************************************
         * CDMA/LTE 价脚 钮 贸府
         **********************************************************************/
        jz = CdmaSendQueue_gets();

        if(jz != -1)
        {
            giGigbeeSendingFlag = 1;

            if(gnlUseLTE)
            {
                gszSendingGigbee[0] = 0x02;
                gszSendingGigbee[1] = '|';

                for(iz = 0; iz < 11; iz++)
                {
                    if(jangbi.cdma.telno[iz])
                    {
                        gszSendingGigbee[iz + 2] = jangbi.cdma.telno[iz];
                    }
                }

                gszSendingGigbee[iz + 2] = '|';
                kz = iz + 3;

                for(iz = 0; iz < strlen(CdmaSendQueue[jz]); iz++)
                {
                    gszSendingGigbee[iz + kz] = CdmaSendQueue[jz][iz];
                }

                sprintf(sT0,
                        "CdmaSendQueue_qhead=%d, CdmaSendQueue_qtail=%d, %s",
                        CdmaSendQueue_qhead,
                        CdmaSendQueue_qtail,
                        CdmaSendQueue[jz]);

                DEBUG_puts(sT0);
            }
            else
            {
                sprintf(gszSendingGigbee,
                        "AT+UNICAST=0000,%s",
                        &CdmaSendQueue[jz][2]);
            }

            giSendingGigbeeLength = strlen(gszSendingGigbee);

            USART1_puts("::::::::::::");
            DEBUG_puts(gszSendingGigbee);
        }

        /**********************************************************************
         * USART3 CDMA SMS 荐脚 贸府
         **********************************************************************/
        if(Usart3_sms_received_flag)
        {
            SMS_PROCESS();
        }
    }
} // Agent_CdmaTask

/* end of file */
