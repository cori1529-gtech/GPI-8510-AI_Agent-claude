/*
	global.c
*/

#include "stm32f10x_type.h"

//for cdma data sending
int		giSendingType = 0; //2014.8.7
int		iSendStage = 0;	//0:none, 1:data loading, 2:, 3:, 4

int		iZigbeeSendStage = 0;	//지그비 전송 스텝

//cdma tx buffer queue
#define	MAX_CDMA_QUEUE_BUFFER	20
char	gszSendingGigbee[256];
int		giSendingGigbeeLength = 0;
char	CdmaSendQueue[MAX_CDMA_QUEUE_BUFFER][256];
int		CdmaSendQueue_qhead;
int		CdmaSendQueue_qtail;

//cdma rx buffer queue
#define	RXCDMAQ_SIZE	8000
vu8 	RxCdmaQ[RXCDMAQ_SIZE];
vu16	RxCdmaQ_qhead;
vu16	RxCdmaQ_qtail;

//cdma rx token buffer queue
#define	RXCDMATOKENQ_SIZE	50
vu8 	RxCdmaTokenQ[RXCDMATOKENQ_SIZE][100];
vu16	RxCdmaTokenQ_qhead;
vu16	RxCdmaTokenQ_qtail;

#define MAX_giGigbeeSendingRetry	3
int		giGigbeeSendingRetry;	//2016.3.28
int		giGigbeeSendingFlag = 0;	//2016.3.28

u32	timer_ms;
u32	monitoring_timer_ms;		//status, 배출 모니터링 시간.  (1시간 간격)
u32	monitoring_set_timer_ms;	//set 값 모니터링 시간 (6시간 간격)
u32	tcpip_send_idle_timer_ms;
u32	bachul_send_timer_ms;	//배출중 정보 전송 간격

int	cdma_response_waiting_flag = 0;	//cdma 응답을 기다리는 상태
u32	cdma_response_wait_timer_ms;	//cdma 응답 기다리는 타이머

int	giCdmaStopEnabled = 0;	//cdma 를 강제로 stop 실행을 해야 하는가 플래그

/* end of file */
