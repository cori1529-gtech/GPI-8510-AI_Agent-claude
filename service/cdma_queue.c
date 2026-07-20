/*******************************************************************************
 * cdma_queue.c  (service layer)
 *
 * Moved verbatim from main.c (main.c slimming step, see prj/CLAUDE.md 10.2).
 * Pure move, no behavior change. Korean comments preserved byte-for-byte
 * (CP949, per project encoding rule).
 *
 * Contents:
 *   ZIGBEE_puts()          - thin wrapper over UART4 (zigbee/BT channel)
 *   CdmaSendQueue_puts()   - push one message into the modem send queue
 *   CdmaSendQueue_gets()   - pop index of next queued message (-1 = empty)
 *   modem_power_reset()    - AT$LGTRESET + power cycle via MODEM_PWR pin
 ******************************************************************************/

#include <stdio.h>
#include <string.h>

#include "stm32f10x.h"
#include "stypes.h"
#include "type.h"
#include "global.h"      /* CdmaSendQueue, CdmaSendQueue_qhead/qtail,
                            MAX_CDMA_QUEUE_BUFFER */
#include "usart.h"       /* USART1_puts() */
#include "hw_config.h"   /* Delay_ms() */
#include "buzzer.h"      /* Cdma_Power_On(), Cdma_Power_Off() */
#include "tcpip_send.h"  /* cdma_send_cmd() */

/* defined in usart.c, not declared in usart.h */
extern void UART4_puts(char *str);

void ZIGBEE_puts(char *str)
{
	UART4_puts(str);
}


int CdmaSendQueue_puts(char *txt)
{
	int	ret_val = -1;

	CdmaSendQueue_qhead = ++CdmaSendQueue_qhead % MAX_CDMA_QUEUE_BUFFER;
	strncpy(CdmaSendQueue[CdmaSendQueue_qhead], txt, strlen(txt)+1);

USART1_puts(CdmaSendQueue[CdmaSendQueue_qhead]);

	return ret_val;
}

int CdmaSendQueue_gets()
{
	if (CdmaSendQueue_qhead == CdmaSendQueue_qtail)	return -1;	//queue empty
	
	CdmaSendQueue_qtail = ++CdmaSendQueue_qtail % MAX_CDMA_QUEUE_BUFFER;

//USART1_puts(CdmaSendQueue_queue[CdmaSendQueue_qtail]);

	return CdmaSendQueue_qtail;
}

//LTE MODEM reset and power on/off
void modem_power_reset(void)
{
	cdma_send_cmd("AT$LGTRESET", 11);	//2014.12.23   날이바뀌면 modem reset 실행 
	Delay_ms(1000);
	Cdma_Power_Off();
	Delay_ms(1000);
	Cdma_Power_On();
}

/* end of file */
