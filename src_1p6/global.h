/*
	global.h
*/

#ifndef	__GLOBAL_H__
#define	__GLOBAL_H__

#include "stm32f10x.h"

#include "type.h"

//#define	TMS_SERVER_IP	"116.123.106.53"
//#define	TMS_SERVER_PORT	9997
#define	TMS_SERVER_IP	"112.217.212.250"
#define	TMS_SERVER_PORT	8397

#define	MONITORING_SERVER_IP	"112.217.212.250"
#define	MONITORING_SERVER_PORT	8391

#define	DEF_1_SEC	100
#define	DEF_1_MIN	(DEF_1_SEC * 60)

#define	SEND_INTERVAL_MONITORING			(DEF_1_MIN*60)	//1 hour
#define	SEND_SET_INTERVAL_MONITORING		(DEF_1_MIN*360)	//6 hour

//#define	GET_TIME_INTERFAL_TIME		    (DEF_1_MIN*15)	//1 min  몇분마다 현재시간 및 날짜를 읽어올 것인가 
#define	GET_TIME_INTERFAL_TIME		    	(DEF_1_MIN*3)	//1 min  몇분마다 현재시간 및 날짜를 읽어올 것인가 

#define	WORKING_SEND_INTERVAL			10 * (DEF_1_MIN)	//배출중에는 10분 간격으로 전송

#define	SEND_INTERVAL		(DEF_1_SEC * 1)		//5 sec


//<<<<<<<<<<< cdma 동작 루틴
#define	CDMA_STAGE_NONE		0
#define	STAGE_FORCE_NONE	0
#define	STAGE_FORCE_HANGUP	1

#define	CDMA_FORCE_HANGUP	101
#define	CDMA_GET_TELNO		102
#define	CDMA_GET_TIME		103
#define	CDMA_GET_RSSI		104

#define	CDMA_IP_SETTING		111

#define	STEP_SEND_INIT	1010
#define	STEP_SEND_CMD	1020


extern	volatile int	giCdmaReturn;	//2015.8.17
extern	char	gszCdmaReturn[];	//2015.8.17




extern  vu8		version[];

//extern	vu8		ghPowerOnResetStatus;	//status sendd power on reset=1, else 0

extern	vu32	timer_ms;
extern	vu32	monitoring_timer_ms;
extern	vu32	monitoring_set_timer_ms;
extern	vu32	tcpip_send_idle_timer_ms;
extern	u32		bachul_send_timer_ms;	//배출중 정보 전송 간격

extern	int	cdma_response_waiting_flag;	//cdma 응답을 기다리는 상태
extern 	u32	cdma_response_wait_timer_ms;	//cdma 응답 기다리는 타이머

//for cdma data sending
extern	volatile int	giSendingType; //2014.8.7
extern	volatile int	iSendStage;	//0:none, 1:data loading, 2:, 3:, 4
extern	volatile int	iZigbeeSendStage;

extern	int	giCdmaStopEnabled;	//cdma 를 강제로 stop 실행을 해야 하는가 플래그


#define	MAX_CDMA_QUEUE_BUFFER	10
extern	char	gszSendingGigbee[256];
extern	int		giSendingGigbeeLength;
extern	char	CdmaSendQueue[MAX_CDMA_QUEUE_BUFFER][256];
extern	int		CdmaSendQueue_qhead;
extern	int		CdmaSendQueue_qtail;

//cdma rx buffer queue
#define	RXCDMAQ_SIZE	8000
extern	vu8 	RxCdmaQ[RXCDMAQ_SIZE];
extern	vu16	RxCdmaQ_qhead;
extern	vu16	RxCdmaQ_qtail;

//cdma rx token buffer queue
#define	RXCDMATOKENQ_SIZE	50
extern	vu8 	RxCdmaTokenQ[RXCDMATOKENQ_SIZE][100];
extern	vu16	RxCdmaTokenQ_qhead;
extern	vu16	RxCdmaTokenQ_qtail;

#define MAX_giGigbeeSendingRetry	3
extern	volatile int	giGigbeeSendingRetry;	//2016.3.28
extern	volatile int	giGigbeeSendingFlag;	//2016.3.28




void test_routine(void);

int get_cdma_telno(void);
int get_cdma_rssi(void);
int get_cdma_time(void);



#endif	//#ifdef __GLOBAL_H__

/* end of file */
