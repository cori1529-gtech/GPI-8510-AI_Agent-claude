
#include "stm32f10x.h"

#include "stdio.h"
#include "string.h"
#include "stm32f10x_lib.h"

#include "def.h"
#include "global.h"
//#include "card_info.h"
#include "aria.h"
#include "i2c_ee.h"
#include "tcpip_send.h"
#include "sms.h"
#include "usart.h"	//add gpi-8310
#include "var.h"


#include "debug.h"


#define MAX_GTECH_SERVER_PORT_QTY	3
static volatile int	giGtechServerCurrentPortIndex = 1;	// 8392, 8393, 8391, 8392....
static volatile int	iStabeTwoWaitCount;	//2015.9.21
static volatile int	iStage40Count = 0;	//41과 42 합치기위하여 0,1,2,2015.9.22
static volatile int	iStage100Count = 0;	//100과 101 합치기위하여 0,1,2,2015.9.22
//static volatile int	giSendingStageRetry = 0; //2015.8.20


int 	Sending_Retry_Counter;

vu8 	RxCdmaBuffer[RX_CDMA_BUFFER_SIZE];
vu16	RxCdmaCounter;

vu8 	RxCdmaSmsBuffer[30];
vu16	RxCdmaSmsCounter;

vu8 	RxZigbeeBuffer[RXCARDBUFFER2SIZE];
vu16	RxCdmaCounter;

u8 		Keco_card_info_flag;

int	giCdmaStage = 0;
int	giCdmaStep = 0;

extern u8 Rx4Cmd[];
extern u32	Rx4CmdCounter;

extern u8   Once_flag;
int ret_val;


extern struct jangbi_info jangbi;


extern char str[];	//char str[50];

void Stage_Four_Data_Sending_send_msg(char *msg, int length);  // 배출데이타를 전송 
int Stage_Four_Data_Sending_wait_response(int init, int wait);  //배출데이타를 전송 후 리턴값 기다림
void Cdma_Force_Stop(void);  // 전화 강제로 끊기 
int	Stage_Idle(int init, u32 wait);  //리턴값 기다림

void websms_connection(void);	//web sms
int SocketOpen(char *ip, int port);


#define	cdma_putchar(ch)	USART3_putchar(ch)
/*
void cdma_putchar(char ch)
{
	USART3_putchar(ch);
}
*/

// add 2071.3.2  ver 1.8 
void cdma_detach(void)
{
	cdma_send_cmd("AT%PSMACTIVE=0", 14);
	Delay_ms(100);
	cdma_send_cmd("AT$LGTPWDN", 10);
	Delay_ms(900);
}


int cdma_send_msg(char *msg, int length)
{
	int i;

	//send to cdma
	for(i=0; i<length; i++) cdma_putchar(msg[i]);

	return i;
}

int cdma_send_msg_with_buffer_clear(char *msg, int length)
{
	Keco_card_info_flag = 1;	//cdma data 받아오을때 
	
	RxCdmaCounter = 0;		
	memset(RxCdmaBuffer, 0, RX_CDMA_BUFFER_SIZE);	//리턴할 배열 초기화

	cdma_send_msg( msg, length);
}

int cdma_send_cmd(char *cmd, int length)
{
	int i;

	i = cdma_send_msg(cmd, length);
	cdma_putchar(0x0d);
	cdma_putchar(0x0a);

	return i;
}

int cdma_send_cmd_with_buffer_clear(char *cmd, int length)
{
	Keco_card_info_flag = 1;	//cdma data 받아오을때 
	
	RxCdmaCounter = 0;		
	memset(RxCdmaBuffer, 0, RX_CDMA_BUFFER_SIZE);	//리턴할 배열 초기화
	
	cdma_send_cmd( cmd, length);
}

int get_cdma_telno()
{
	///return get_cdma_telno_NONSTOP();
	return get_cdma_info_WAIT(CDMA_GET_TELNO, "TELNO");
}

int get_cdma_time()
{
	///return get_cdma_time_NONSTOP();
	return get_cdma_info_WAIT(CDMA_GET_TIME, "TIME");
}

int get_cdma_rssi()
{
	///return get_cdma_rssi_NONSTOP();
	return get_cdma_info_WAIT(CDMA_GET_RSSI, "RSSI");
}


void tcpip_send_20140806(char *ip, int port)
{
	int	ret_val = 0;

	switch(giSendingType)
	{
		case 0:
			websms_connection();
			if(iSendStage == 300)
			{	
				giSendingType++;
				iSendStage = 0;
				//get_cdma_time();
			}
			break;		

		case 1:
			tcpip_send_form_zigbee_data(MONITORING_SERVER_IP, MONITORING_SERVER_PORT+giGtechServerCurrentPortIndex);
			if(iSendStage == 300)
			{	
				giSendingType++;
				iSendStage = 0;
				//get_cdma_time();
			}
			break;

		case 2:
			if(gbGetCdmaTime)
			{
				gbGetCdmaTime = 0;
				get_cdma_time();
			}
			//ret_val = get_cdma_stop_NONSTOP_RET( 1, "dummy");
///5			ret_val = get_cdma_info_NONSTOP_RET( CDMA_FORCE_HANGUP, "dummy");
///5			if((ret_val == -1) || (ret_val == 6400))
		
			{
				giSendingType++;
				iSendStage = 0;
			}
			break;

		default:
			giSendingType = 0;
			iSendStage = 0;
			break;
	}
}


int	Stage_Idle(int init, u32 wait)  //시간 보내기
{
	int  ret_val;

	IWDG_ReloadCounter();
	ret_val = 0;
	if(init == 0) tcpip_send_idle_timer_ms = wait;
	if(!tcpip_send_idle_timer_ms) ret_val = 1;

	return ret_val;
}


//http://itguru.tistory.com/101
int check_response(char *str1, char *str2, int buf_length, int substr_length)
{
	int iz;
	int ret_val = 0;
    char *cp = (char *) str1;
    char *s1, *s2;
	static unsigned int pre_cnt = 0;

// 2014.10.21 
//	for(iz=0; iz<RxCardCounter2; iz++) 	USART1_putchar(RxCardBuffer2[iz]);
// 2014.10.23 - 마지막 30개만 표시
//	iz = RxCardCounter2 - 30;
//	if(iz < 0) iz = 0;

///5	for(iz=0; iz<buf_length; iz++) 	USART1_putchar(str1[iz]);
	
	
	if(buf_length < pre_cnt) pre_cnt = 0;
		
	if(pre_cnt < buf_length)
	{
		for(iz=pre_cnt; iz<buf_length; iz++) 	USART1_putchar(str1[iz]);
		pre_cnt = buf_length;
	}

    if (!*str2) ret_val = 0;
	else
	{
		//because keco response included null character at c1 function
//	    while (*cp)
		for(iz=0; iz<buf_length; iz++)
    	{
			cp = &str1[iz];
        	s1 = cp;
	        s2 = (char *) str2;

    	    while (*s1 && *s2 && !(*s1 - *s2)) s1++, s2++;
    	   	if (!*s2)
			{
				//ret_val = 1;
				ret_val = iz + 1;	//2015.9.22
				break;
			}
	        cp++;
	    }
	}

    return ret_val;
}


void Cdma_Force_Stop(void)  // 전화 강제로 끊기 
{
	SocketClose_LU202F();
}


u16	crc_calc(char *s)
{
	u16 crc = 0;

	do { crc += *s++; } while (*s);

	return crc;
}

void Send_to_server(int msg_type, char *msg)
{
	int length;
	int	crc;
	char sz[200];
	char szT[200];

	gnlTick %= 0x1000000;
	
	switch(msg_type)
	{
		case SEND_BAICHUL:	//배출 정보 전송
			/*
			STX			HEAD, 0X02					1 |			구분자				1
			63			보낼데이타 객수(M1603007~W)		2 |			구분자				1
			M1603007 	2016년 3월 7번째로 생산된 메인장비	7 |			구분자				1
			M001		메인 ID :1, 계사번호: 1			4 |			구분자				1
			123456		호파현재무게: 123456 kg			6 |			구분자				1
			10000		오늘 배출무게					5 |			구분자				1
			10001		1일전배출무게					5 |			구분자				1
			10002		2일전배출무게					5 |			구분자				1
			10003		3일전배출무게					5 |			구분자				1
			10004		4일전배출무게					5 |			구분자				1
			10005		5일전배출무게					5 |			구분자				1
			10006		6일전배출무게					5 |			구분자				1
			W			현재배출중('W':배출중,'N':배출중 아님)	1 |			구분자				1
			CRC			53~W 까지  체크섬값				2 |			구분자				1
			ETX			TAIL, 0X03					1	
									TOTAL	72
			*/
		//	sprintf( sz, "M%07ld|M%03ld|%s|%s|%010lu|%05ld|%05ld|%05ld|%05ld|%05ld|%05ld|%05ld|%05ld|%05ld|%c",
			sprintf( sz, "M%07ld|M%03ld|%s|%s|%lx|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c",
					 gnlSerialNo, gnlMyAddress, msg, VERSION, gnlTick, gxlGrossWeight, gnlDailyTotal%100000,
					 (long)gnlPreDailyTotal[0]%100000, (long)gnlPreDailyTotal[1]%100000, (long)gnlPreDailyTotal[2]%100000, (long)gnlPreDailyTotal[3]%100000,
					 (long)gnlPreDailyTotal[4]%100000, (long)gnlPreDailyTotal[5]%100000, (long)gnlPreDailyTotal[6]%100000,
					 ((working_flag)?'W':'N')
			);		
			break;
			 
		case SEND_SETVALUE:	//설정값 전송
			/*
			sprintf( sz, "M%07ld|M%03ld|STATUS|%s|%lu|%lu|%lu|%lu|%lu|%lu|%lu|%lu|%f|%lu|%lu|%lu|%lu|%lu|%ld|%ld|%ld|%ld|%ld|%ld|%c%c%c%c%c%c",
						gnlSerialNo, gnlMyAddress, VERSION, gnlTick,
						gxhWeightBackup, gxhEndDelay, gplCompensationWeight,
						gnlSms_Send_Time_First,	gnlSms_Send_Time_Second, gnlSms_Send_Time_Third,
						Loadcell_Number, Real_Weight_Factor, gnlDisplay, gnlSerialNo, gnlMyAddress, gnlHenhouseNo, gnlUseLTE,
						One_Weight, Two_Weight, Three_Weight, Four_Weight, Five_Weight, Six_Weight,
						((One_Weight_Ok)?'G':'N'), ((Two_Weight)?'G':'N'), ((Three_Weight)?'G':'N'), ((Four_Weight)?'G':'N'), ((Five_Weight)?'G':'N'), ((Six_Weight)?'G':'N')
			);
			*/
			sprintf( sz, "M%07ld|M%03ld|SET|%s|%lx|%lu|%lu|%lu|%lu|%ld|%lu|%lu|%lu|%lu|%lu|%lu|%ld",
						gnlSerialNo, gnlMyAddress, VERSION, gnlTick, gnlHenhouseNo, gnlUseLTE,
						gxhWeightBackup, gxhEndDelay, gplCompensationWeight,
						gnlSms_Send_Time_First,	gnlSms_Send_Time_Second, gnlSms_Send_Time_Third,
						Loadcell_Number, (long)Real_Weight_Factor, gnlDisplay, gxwStatusSendInterval
			);
			break;

		case SMS_RECEIVED:
			sprintf( sz, "M%07ld|M%03ld|SMS-R|%s|%lx|%lu|%d|%ld|%s",
						gnlSerialNo%10000000, gnlMyAddress%1000, VERSION, gnlTick, gnlHenhouseNo%100, gnuDay, gxlGrossWeight%100000, msg);
			break;

		case SEND_STATUS:	//상태정보 전송
		default:
			/*
			STX			HEAD, 0X02					1 |	구분자		1
			61			보낼데이타 객수(M1603007~GGGGDD)	2 |	구분자		1
			M1603007	2016년 3월 7번째로 생산된 메인장비	7 |	구분자		1
			M001		메인 ID :1, 계사번호: 1			4 |	구분자		1
			
			123456		1번로드셀 A/D 값				6 |	구분자		1
			123456		2번로드셀 A/D 값				6 |	구분자		1
			123456		3번로드셀 A/D 값				6 |	구분자		1
			123456		4번로드셀 A/D 값				6 |	구분자		1
			999999		5번로드셀 설치안됨					6 |	구분자		1
			999999		6번로드셀 설치안됨					6 |	구분자		1
			GGGGDD		1~4번 로드셀정상, 5,6번 설치안됨		6 |	구분자		1
			CRC			61~GGGGDD  까지 체크섬값			2 |	구분자		1
			ETX			TAIL, 0X03					1
									TOTAL	70
			*/
			/*
			sprintf( sz, "M%07ld|M%03ld|STATUS|%s|%lu|%lu|%lu|%lu|%lu|%lu|%lu|%lu|%f|%lu|%lu|%lu|%lu|%lu|%ld|%ld|%ld|%ld|%ld|%ld|%c%c%c%c%c%c",
						gnlSerialNo, gnlMyAddress, VERSION, gnlTick,
						gxhWeightBackup, gxhEndDelay, gplCompensationWeight,
						gnlSms_Send_Time_First,	gnlSms_Send_Time_Second, gnlSms_Send_Time_Third,
						Loadcell_Number, Real_Weight_Factor, gnlDisplay, gnlSerialNo, gnlMyAddress, gnlHenhouseNo, gnlUseLTE,
						One_Weight, Two_Weight, Three_Weight, Four_Weight, Five_Weight, Six_Weight,
						((One_Weight_Ok)?'G':'N'), ((Two_Weight)?'G':'N'), ((Three_Weight)?'G':'N'), ((Four_Weight)?'G':'N'), ((Five_Weight)?'G':'N'), ((Six_Weight)?'G':'N')
			);
			*/
			/*** 2016.7.25
			//	sprintf( sz, "M%07ld|M%03ld|ST|%s|%lu|%2lu|%5lu|%1lu|%6lu|%6lu|%6lu|%6lu|%6lu|%6lu|%c%c%c%c%c%c",
			sprintf( sz, "M%07ld|M%03ld|ST|%s|%lx|%lu|%d|%ld|%lu|%lu|%lu|%lu|%lu|%lu|%lu|%c%c%c%c%c%c",
						gnlSerialNo%10000000, gnlMyAddress%1000, VERSION, gnlTick, gnlHenhouseNo%100, gnuDay, gxlGrossWeight%100000,
						Loadcell_Number%10, One_Weight%1000000, Two_Weight%1000000, Three_Weight%1000000, Four_Weight%1000000, Five_Weight%1000000, Six_Weight%1000000,
						((One_Weight_Ok)?'G':'N'), ((Two_Weight)?'G':'N'), ((Three_Weight)?'G':'N'), ((Four_Weight)?'G':'N'), ((Five_Weight)?'G':'N'), ((Six_Weight)?'G':'N')
			);
			***/

			/***
			sprintf( sz, "M%07ld|M%03ld|ST|%s|%lx|%lu|%d|%ld|%lu|%lu|%lu|%lu|%lu|%lu|%lu|%c%c%c%c%c%c",
						gnlSerialNo%10000000, gnlMyAddress%1000, VERSION, gnlTick, gnlHenhouseNo%100, gnuDay, mon_gxlGrossWeight%100000,
						Loadcell_Number%10, mon_One_Weight%1000000, mon_Two_Weight%1000000, mon_Three_Weight%1000000, mon_Four_Weight%1000000, mon_Five_Weight%1000000, mon_Six_Weight%1000000,
						((mon_One_Weight_Ok)?'G':'N'), ((mon_Two_Weight_Ok)?'G':'N'), ((mon_Three_Weight_Ok)?'G':'N'), ((mon_Four_Weight_Ok)?'G':'N'), ((mon_Five_Weight_Ok)?'G':'N'), ((mon_Six_Weight_Ok)?'G':'N')
			);
			***/
			sprintf( sz, "M%07ld|M%03ld|ST|%s|%lx|%lu|%d|%ld|%lu|%lu|%lu|%lu|%lu|%ld|%lu|%c%c%c%c%c%c",
						gnlSerialNo%10000000, gnlMyAddress%1000, VERSION, gnlTick, gnlHenhouseNo%100, gnuDay, mon_gxlGrossWeight%100000,
						Loadcell_Number%10, mon_One_Weight%1000000, mon_Two_Weight%1000000, mon_Three_Weight%1000000, mon_Four_Weight%1000000, mon_raw_adc, mon_Six_Weight%1000000,
						((mon_One_Weight_Ok)?'G':'N'), ((mon_Two_Weight_Ok)?'G':'N'), ((mon_Three_Weight_Ok)?'G':'N'), ((mon_Four_Weight_Ok)?'G':'N'), ((mon_Five_Weight_Ok)?'G':'N'), ((mon_Six_Weight_Ok)?'G':'N')
			);

			break;
	}

	length = strlen(sz);
	sz[82] = 0;	//zigbee send max length
	
	crc = crc_calc(sz);
	//sprintf( szT,"%c|%s|%d|%s|%02X|%c", 0x02, jangbi.cdma.telno, length, sz, crc&0x00ff, 0x03);
	sprintf( szT,"%c|%d|%s|%02X|%c", 0x02, length, sz, crc&0x00ff, 0x03);

	//cdma send queue
	CdmaSendQueue_puts(szT);	
}

void test_routine()
{
	//배출정보, 상태정보, 모니터링 등 전송
//	if( !cdma_interval_timer_ms)
	{
		cdma_interval_timer_ms = 50;
		if(gnlUseLTE) tcpip_send_20140806("192.168.1.1", 100);
		else zigbee_send();
	}

	if(!monitoring_timer_ms)
	{
///		monitoring_timer_ms = SEND_INTERVAL_MONITORING;	//60 min  나중에는 60분 
		monitoring_timer_ms = DEF_1_MIN * gxwStatusSendInterval;
//DEBUG_puts("STATUS INTERVAL TEST ----");
//		monitoring_timer_ms = 10000;
//		Send_status();
//		Send_setting();
		Send_to_server(SEND_STATUS, "status");
		Send_to_server(SEND_BAICHUL, "BC-S");	//배출 상태 정보 전송
	}
	if( !monitoring_set_timer_ms)
	{
		monitoring_set_timer_ms = SEND_SET_INTERVAL_MONITORING;	//360 min
		Send_to_server(SEND_SETVALUE, "set");
	}
	

	//websms disconned or 전원켠 후 120초 동안 재시도, 이후에는 1분단위 재시도
	if(giWebSMSConnection == 4 &&  gnlTick<12000) gbWebSMSConnection = 1;
	
}

int Cdma_Force_Stop_no_delay(int init, char *par)  // 전화 강제로 끊기 
{
	int  i; 
	int ret_val = 0;
 	char szMsg[15];
	char szResponse[15];

	static int iRetry;
	static int iHangupStage = 0;

	if(init == 0)
	{
//		iRetry = 3;
		iHangupStage = 10;
	}
	  
	switch(iHangupStage)
	{
		case 10:
			/*		               
			3 * (10MS + 300MS) = 930MS
			
			//"+++" 보내고 1초내에 , 응답 확인 "OK" 받을때까지 3회 반복, "CLOSED", "ERROR"
			//"AT$LGTTCPCL" 보내고 110M 내에  "OK" 받을때까지 3회 반복, "ERROR"
			//"AT$LGTPREL" 보내고 110MS 내에 "CARRIER"   받을때까지 3회 반복, "ERROR"
			*/
			//cdma 를 강제로 stop 실행을 해야 하는가 플래그
			if( !giCdmaStopEnabled) { ret_val = -1; break; }
			{
				iHangupStage = 12;
				iRetry = 3;
				ret_val = iHangupStage;
USART1_puts("CALL1_head");
			}
//			break;

		case 12:
//USART1_puts("CALL1_head_12");
			if( !iRetry) 
			{
//USART1_puts("CALL1_head_12_0");
				ret_val = -1; 	
			}
			else 
			{
USART1_puts("CALL1_head_12_+++");
				iHangupStage = 14;	//response wait
				cdma_response_waiting_flag = 1;	//cdma 응답을 기다리는 상태
				cdma_response_wait_timer_ms = DEF_1_SEC/3;	//cdma 응답 기다리는 타이머

				RxCdmaCounter = 0;		
				Keco_card_info_flag = 1;	//cdma data 받아오을때 
				memset(RxCdmaBuffer, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화
	
				strcpy(szMsg, "+++");
				cdma_send_msg(szMsg, strlen(szMsg));

				ret_val = iHangupStage;
			}
			break;

		case 14:
//sprintf(str, "CALL1_head_14__%d::%ld.", iRetry, cdma_response_wait_timer_ms);
//USART1_puts(str);
			if( !cdma_response_wait_timer_ms && iRetry)
			{
//USART1_puts("CALL1_head_14_0");
				iRetry--;
				if( !iRetry) iHangupStage = 12;	//retry
			}
			else
			{
//USART1_puts("CALL1_head_14_1");
				sprintf(szResponse, "OK%c%c%c", 0x0d, 0x0a, 0);
				//if(RxCardBuffer2[RxCardCounter2-4] == 'O' && RxCardBuffer2[RxCardCounter2-3] == 'K' && RxCardBuffer2[RxCardCounter2-2] == 0x0d && RxCardBuffer2[RxCardCounter2-1] == 0x0a)
				if(check_response(RxCdmaBuffer, szResponse, RxCdmaCounter, strlen(szResponse)))
				{	
					iHangupStage = 20;
					ret_val = 6400;
//USART1_puts("CALL_FORCE_STOP_1_res_ok");
		            break;
				}
				else if(check_response(RxCdmaBuffer, "CLOSED", RxCdmaCounter, strlen(szResponse)))
				{	
					ret_val = 6400;
//USART1_puts("CALL_FORCE_STOP_1_Res_closed");
		            break;
				}
				else if(check_response(RxCdmaBuffer, "ERROR", RxCdmaCounter, strlen(szResponse)))
				{	
					ret_val = -1;
//USART1_puts("CALL_FORCE_STOP_1_Res_error");
		            break;
				}
			}
			break;

		case 20:
			iHangupStage = 22;
			iRetry = 3;
			ret_val = iHangupStage;
USART1_puts("CALL2_head");
//			break;

		case 22:
			if( !iRetry) 
			{
				ret_val = -1; 	
			}
			else 
			{
				iHangupStage = 24;	//response wait
				cdma_response_waiting_flag = 1;	//cdma 응답을 기다리는 상태
				cdma_response_wait_timer_ms = DEF_1_SEC/10;	//cdma 응답 기다리는 타이머 100ms

				RxCdmaCounter = 0;		
				Keco_card_info_flag = 1;	//cdma data 받아오을때 
				memset(RxCdmaBuffer, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화
	
				strcpy(szMsg, "AT$LGTTCPCL");
				cdma_send_msg(szMsg, strlen(szMsg));

				ret_val = iHangupStage;
			}
			break;

		case 24:
			if( !cdma_response_wait_timer_ms && iRetry)
			{
				iRetry--;
				if( !iRetry) iHangupStage = 22;	//retry
			}
			else
			{
				sprintf(szResponse, "CARRIER%c", 0);
				//if(RxCardBuffer2[RxCardCounter2-4] == 'O' && RxCardBuffer2[RxCardCounter2-3] == 'K' && RxCardBuffer2[RxCardCounter2-2] == 0x0d && RxCardBuffer2[RxCardCounter2-1] == 0x0a)
				if(check_response(RxCdmaBuffer, szResponse, RxCdmaCounter, strlen(szResponse)))
				{	
					iHangupStage = 30;
					ret_val = 6400;
//USART1_puts("CALL_FORCE_STOP_1_res_ok");
		            break;
				}
				else if(check_response(RxCdmaBuffer, "ERROR", RxCdmaCounter, strlen(szResponse)))
				{	
					ret_val = -1;
//USART1_puts("CALL_FORCE_STOP_1_Res_error");
		            break;
				}
			}
			break;

		case 30:
			iHangupStage = 32;
			iRetry = 3;
			ret_val = iHangupStage;
USART1_puts("CALL3_head");
//			break;
		
		case 32:
			if( !iRetry) 
			{
				ret_val = -1; 	
			}
			else 
			{
				iHangupStage = 34;	//response wait
				cdma_response_waiting_flag = 1;	//cdma 응답을 기다리는 상태
				cdma_response_wait_timer_ms = DEF_1_SEC/10;	//cdma 응답 기다리는 타이머 100ms

				RxCdmaCounter = 0;		
				Keco_card_info_flag = 1;	//cdma data 받아오을때 
				memset(RxCdmaBuffer, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화
	
				strcpy(szMsg, "AT$LGTPREL");
				cdma_send_msg(szMsg, strlen(szMsg));

				ret_val = iHangupStage;
			}
			break;

		case 34:
			if( !cdma_response_wait_timer_ms && iRetry)
			{
				iRetry--;
				if( !iRetry) iHangupStage = 32;	//retry
			}
			else
			{
				sprintf(szResponse, "OK%c%c%c", 0x0d, 0x0a, 0);
				//if(RxCardBuffer2[RxCardCounter2-4] == 'O' && RxCardBuffer2[RxCardCounter2-3] == 'K' && RxCardBuffer2[RxCardCounter2-2] == 0x0d && RxCardBuffer2[RxCardCounter2-1] == 0x0a)
				if(check_response(RxCdmaBuffer, szResponse, RxCdmaCounter, strlen(szResponse)))
				{	
					ret_val = 6400;
					iHangupStage = 10;
//USART1_puts("CALL_FORCE_STOP_1_res_ok");
		            break;
				}
				else if(check_response(RxCdmaBuffer, "ERROR", RxCdmaCounter, strlen(szResponse)))
				{	
					ret_val = -1;
//USART1_puts("CALL_FORCE_STOP_1_Res_error");
		            break;
				}
			}
			break;
	}
	
	Keco_card_info_flag = 0;	//cdma data 받아오을때 

//sprintf(str, "Cdma_Force_Stop_no_delay_RET = %d, STAGE=%d. iRetry=%d. timer=%ld.", ret_val, iHangupStage, iRetry, cdma_response_wait_timer_ms);
//USART1_puts(str);

	return ret_val;
}



int get_cdma_stop_NONSTOP_RET(int which, char *par)
{
	int	ret_val = 0;
	int	qidx;
		
//	giCdmaStage = which;

	if( !giCdmaStopEnabled) return -1;

	switch(giCdmaStep)
	{
		case STEP_SEND_INIT:
///USART1_puts("cdma_exec_INIT");
			giCdmaStep = STEP_SEND_CMD;
			Cdma_Force_Stop_no_delay(0, par);
			break;

		case STEP_SEND_CMD:
//USART1_puts("cdma_exec_SEND_CMD");
			ret_val = Cdma_Force_Stop_no_delay(1, par);
			break;
		default:
			giCdmaStage = CDMA_STAGE_NONE;
			giCdmaStep = STEP_SEND_INIT;
			break;
	}

	if((ret_val == -1) || (ret_val == 6400))
	{	
		giCdmaStage = CDMA_STAGE_NONE;
		giCdmaStep = STEP_SEND_INIT;
	}

/**
do
{
	qidx = RxCdmaQ_get();
	if(qidx != -1) USART1_putchar(RxCdmaQ[qidx]);
} while (qidx != -1);
**/


sprintf(str, "get_cdma_stop_NONSTOP_RET:::%d., STEP=%d.", ret_val, giCdmaStep);
USART1_puts(str);

	return ret_val;
}

//cdma 동작 루틴
int	cdma_exec()
{
	int  i; 
	int	 ret_val;
 	char szMsg[15];
	char szResponse[15];

	static char iRetry;
	static int iHangupStage = 0;

	switch(giCdmaStage)
	{
		case CDMA_FORCE_HANGUP:
			switch(giCdmaStep)
			{
				case STEP_SEND_INIT:
USART1_puts("cdma_exec_INIT");
					giCdmaStep = STEP_SEND_CMD;
					Cdma_Force_Stop_no_delay(0, "1");
					break;

				case STEP_SEND_CMD:
//USART1_puts("cdma_exec_SEND_CMD");
					ret_val = Cdma_Force_Stop_no_delay(1, "1");
					if((ret_val == -1) || (ret_val == 6400))
					{
						giCdmaStage = CDMA_STAGE_NONE;
						giCdmaStep = STEP_SEND_INIT;

sprintf(str, "Cdma_Force_Stop_no_delay_RET::%d.", ret_val);
USART1_puts(str);

					}
					break;
			}
			break;

		default:
			break;
	}

	return ret_val;
}



//int get_cdma_time_with_rx2cardbuffer()
int	get_cdma_time_no_delay(int init, char *par)
{
	int  iz; 
	int ret_val = 0;
 	char szMsg[15];
	char szResponse[15];

	static int iRetry;
	static int iGetTimeStage = 0;
//	int year, month, date, hour, min, sec;
//	u32	rtc_seconds, rtc_days, rtc_year_since_2000;
	// the beginning of each month
//	u32 DaysToMonth[13] = { 0,31,59,90,120,151,181,212,243,273,304,334,365 };	

	if(init == 0)
	{
		iRetry = 3;
		iGetTimeStage = 10;
	}
	  
	switch(iGetTimeStage)
	{
		case 10:
			/*		               
			3 * (10MS + 300MS) = 930MS
			
			//"+++" 보내고 1초내에 , 응답 확인 "OK" 받을때까지 3회 반복, "CLOSED", "ERROR"
			//"AT$LGTTCPCL" 보내고 110M 내에  "OK" 받을때까지 3회 반복, "ERROR"
			//"AT$LGTPREL" 보내고 110MS 내에 "CARRIER"   받을때까지 3회 반복, "ERROR"
			*/
			iGetTimeStage = 12;
			iRetry = 10;
			ret_val = iGetTimeStage;
///			USART1_puts("GetTime_head");
//			break;

		case 12:
//USART1_puts("GetTime_head_12");
			if( !iRetry) 
			{
//USART1_puts("GetTime_head_12_0");
				ret_val = -1; 	
			}
			else 
			{
//USART1_puts("GetTime_head_12_+++");
				iGetTimeStage = 14;	//response wait
				cdma_response_waiting_flag = 1;	//cdma 응답을 기다리는 상태
				cdma_response_wait_timer_ms = DEF_1_SEC/10;	//cdma 응답 기다리는 타이머	100ms

///				RxCdmaCounter = 0;		
///				Keco_card_info_flag = 1;	//cdma data 받아오을때 
///				memset(RxCdmaBuffer, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화
	
				strcpy(szMsg, "AT$LGTTIME?");
//				strcpy(szMsg, "AT$LGTMDN?");
//				strcpy(szMsg, "AT$LGTRSSI?");
				cdma_send_cmd_with_buffer_clear(szMsg, strlen(szMsg));

				RxCdmaCounter = 0;		
				Keco_card_info_flag = 1;	//cdma data 받아오을때 
				memset(RxCdmaBuffer, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화

				ret_val = iGetTimeStage;
			}
			break;

		case 14:
//sprintf(str, "CALL1_head_14__%d::%ld.", iRetry, cdma_response_wait_timer_ms);
//USART1_puts(str);
			if( !cdma_response_wait_timer_ms && iRetry)
			{
//USART1_puts("CALL1_head_14_0");
				if( !iRetry) iRetry--;
				iGetTimeStage = 12;	//retry

				Keco_card_info_flag = 1;	//cdma 시간등 받아올때
			}
			else
			{
//USART1_puts("CALL1_head_14_1");

				sprintf(szResponse, "OK%c%c%c", 0x0d, 0x0a, 0);

				//if(RxCardBuffer2[RxCardCounter2-4] == 'O' && RxCardBuffer2[RxCardCounter2-3] == 'K' && RxCardBuffer2[RxCardCounter2-2] == 0x0d && RxCardBuffer2[RxCardCounter2-1] == 0x0a)
				if(check_response(RxCdmaBuffer, szResponse, RxCdmaCounter, strlen(szResponse)))
				{	
					iGetTimeStage = 20;
					ret_val = -1;
//USART1_puts("GetTime_res_ok");
//USART1_puts(&RxCdmaBuffer[22]);

					Keco_card_info_flag = 0;	//cdma data 받아오을때 
					RTC_date_to_sec( &RxCdmaBuffer[22]);
				}
			}
			break;
	}


	return ret_val;
}


int	get_cdma_telno_no_delay(int init, char *par)
{
	int  iz; 
	int  ret_val = 0;
 	char szMsg[15];
	char szResponse[15];

	static int iRetry;
	static int iGetTelnoStage = 0;
	char telno[13];


	if(init == 0)
	{
		iRetry = 10;
		iGetTelnoStage = 10;
	}
	  
	switch(iGetTelnoStage)
	{
		case 10:
			iGetTelnoStage = 12;
			iRetry = 10;
			ret_val = iGetTelnoStage;
///			USART1_puts("GetTime_head");
//			break;

		case 12:
//USART1_puts("GetTime_head_12");
			if( !iRetry) 
			{
//USART1_puts("GetTime_head_12_0");
				ret_val = -1; 	
			}
			else 
			{
//USART1_puts("GetTime_head_12_+++");
				iGetTelnoStage = 14;	//response wait
				cdma_response_waiting_flag = 1;	//cdma 응답을 기다리는 상태
				cdma_response_wait_timer_ms = DEF_1_SEC/10;	//cdma 응답 기다리는 타이머	100ms

///				RxCdmaCounter = 0;		
///				Keco_card_info_flag = 1;	//cdma data 받아오을때 
///				memset(RxCdmaBuffer, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화
	
				strcpy(szMsg, "AT$LGTMDN?");
//				strcpy(szMsg, "AT$LGTMDN?");
//				strcpy(szMsg, "AT$LGTRSSI?");
				cdma_send_cmd_with_buffer_clear(szMsg, strlen(szMsg));
/*
				RxCdmaCounter = 0;		
				Keco_card_info_flag = 1;	//cdma data 받아오을때 
				memset(RxCdmaBuffer, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화
*/
				ret_val = iGetTelnoStage;
			}
			break;

		case 14:
//sprintf(str, "CALL1_head_14__%d::%ld.", iRetry, cdma_response_wait_timer_ms);
//USART1_puts(str);
			if( !cdma_response_wait_timer_ms && iRetry)
			{
//USART1_puts("CALL1_head_14_0");
				if( !iRetry) iRetry--;
				iGetTelnoStage = 12;	//retry

				Keco_card_info_flag = 1;	//cdma 시간등 받아올때
			}
			else
			{
//USART1_puts("CALL1_head_14_1");

				sprintf(szResponse, "OK%c%c%c", 0x0d, 0x0a, 0);

				//if(RxCardBuffer2[RxCardCounter2-4] == 'O' && RxCardBuffer2[RxCardCounter2-3] == 'K' && RxCardBuffer2[RxCardCounter2-2] == 0x0d && RxCardBuffer2[RxCardCounter2-1] == 0x0a)
				if(check_response(RxCdmaBuffer, szResponse, RxCdmaCounter, strlen(szResponse)))
				{	
					iGetTelnoStage = 20;
					ret_val = -1;
//USART1_puts("GetTime_res_ok");

					Keco_card_info_flag = 0;	//cdma data 받아오을때 

					for(iz=0; iz<11; iz++)
					{
						telno[iz] = RxCdmaBuffer[iz+20];
						if(('0'<=telno[iz]) && (telno[iz]<='9')) continue;
					}
					if(11 == iz)
					{
						ret_val = 6400;
	
						for(iz=0; iz<11; iz++) jangbi.cdma.telno[iz] = telno[iz];
						jangbi.cdma.telno[iz] = 0;
					}
				}
			}
	
			break;
	}

	return ret_val;
}


int	get_cdma_rssi_no_delay(int init, char *par)
{
	int  iz; 
	int  ret_val = 0;
 	char szMsg[15];
	char szResponse[15];

	static int iRetry;
	static int iGetRssiStage = 0;
	char telno[13];


	if(init == 0)
	{
		iRetry = 10;
		iGetRssiStage = 10;
	}
	  
	switch(iGetRssiStage)
	{
		case 10:
			iGetRssiStage = 12;
			iRetry = 10;
			ret_val = iGetRssiStage;
///			USART1_puts("GetTime_head");
//			break;

		case 12:
//USART1_puts("GetTime_head_12");
			if( !iRetry) 
			{
//USART1_puts("GetTime_head_12_0");
				ret_val = -1; 	
			}
			else 
			{
//USART1_puts("GetTime_head_12_+++");
				iGetRssiStage = 14;	//response wait
				cdma_response_waiting_flag = 1;	//cdma 응답을 기다리는 상태
				cdma_response_wait_timer_ms = DEF_1_SEC/10;	//cdma 응답 기다리는 타이머	100ms

///				RxCdmaCounter = 0;		
///				Keco_card_info_flag = 1;	//cdma data 받아오을때 
///				memset(RxCdmaBuffer, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화
	
				strcpy(szMsg, "AT$LGTRSSI?");
				cdma_send_cmd_with_buffer_clear(szMsg, strlen(szMsg));

				RxCdmaCounter = 0;		
				Keco_card_info_flag = 1;	//cdma data 받아오을때 
				memset(RxCdmaBuffer, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화

				ret_val = iGetRssiStage;
			}
			break;

		case 14:
//sprintf(str, "CALL1_head_14__%d::%ld.", iRetry, cdma_response_wait_timer_ms);
//USART1_puts(str);
			if( !cdma_response_wait_timer_ms && iRetry)
			{
//USART1_puts("CALL1_head_14_0");
				if( !iRetry) iRetry--;
				iGetRssiStage = 12;	//retry

				Keco_card_info_flag = 1;	//cdma 시간등 받아올때
			}
			else
			{
//USART1_puts("CALL1_head_14_1");

				sprintf(szResponse, "OK%c%c%c", 0x0d, 0x0a, 0);

				//if(RxCardBuffer2[RxCardCounter2-4] == 'O' && RxCardBuffer2[RxCardCounter2-3] == 'K' && RxCardBuffer2[RxCardCounter2-2] == 0x0d && RxCardBuffer2[RxCardCounter2-1] == 0x0a)
				if(check_response(RxCdmaBuffer, szResponse, RxCdmaCounter, strlen(szResponse)))
				{	
					iGetRssiStage = 20;
					ret_val = -1;
//USART1_puts("GetTime_res_ok");

					Keco_card_info_flag = 0;	//cdma data 받아오을때 

					for(iz=23; iz<25; iz++)
					{
						if(('0'<=RxCdmaBuffer[iz]) && (RxCdmaBuffer[iz]<='9')) continue;
						break;
					}
					if(24 == iz)
					{
						ret_val = 6400;
						jangbi.cdma.rssi = (RxCdmaBuffer[23] - '0');
					}
					else if(25 == iz)
					{
						ret_val = 6400;
						jangbi.cdma.rssi = (RxCdmaBuffer[23] - '0')*10 + (RxCdmaBuffer[24] - '0');
					}
				}
			}
	
			break;
	}

	return ret_val;
}


int get_cdma_info_exec(int which, char *par)
{
	int	 ret_val = 0;

	if( !gnlUseLTE) return -1;

	switch(giCdmaStep)
	{
		case STEP_SEND_INIT:
///USART1_puts("cdma_exec_INIT");
			giCdmaStep = STEP_SEND_CMD;

			switch(giCdmaStage)
			{
				case CDMA_GET_TELNO: get_cdma_telno_no_delay(0, par); break;
				case CDMA_GET_TIME:  get_cdma_time_no_delay(0, par); break;
				case CDMA_GET_RSSI:  get_cdma_rssi_no_delay(0, par); break;

				case CDMA_FORCE_HANGUP:  Cdma_Force_Stop_no_delay(0, par); break;
			}
			break;

		case STEP_SEND_CMD:
//USART1_puts("cdma_exec_SEND_CMD");
			switch(giCdmaStage)
			{
				case CDMA_GET_TELNO: ret_val = get_cdma_telno_no_delay(1, par); break;
				case CDMA_GET_TIME:  ret_val = get_cdma_time_no_delay(1, par); break;
				case CDMA_GET_RSSI:  ret_val = get_cdma_rssi_no_delay(1, par); break;

				case CDMA_FORCE_HANGUP:  ret_val = Cdma_Force_Stop_no_delay(1, par); break;
			}

			break;
	}

	return ret_val;
}


int get_cdma_info_WAIT(int which, char *par)
{
	int	ret_val = 0;
	int	exit_flag = 0;
	int	qidx;

	cdma_response_wait_timer_ms = DEF_1_SEC/2; 	//500ms
		
	giCdmaStage = which;
	giCdmaStep = STEP_SEND_INIT;

	do
	{
		ret_val = get_cdma_info_exec(which, par);

		if((ret_val == -1) || (ret_val == 6400))
		{	
			exit_flag = 1;
			giCdmaStage = CDMA_STAGE_NONE;
			giCdmaStep = STEP_SEND_INIT;
		}

/**
do
{
	qidx = RxCdmaQ_get();
	if(qidx != -1) USART1_putchar(RxCdmaQ[qidx]);
} while (qidx != -1);
**/

	} while( !exit_flag && cdma_response_wait_timer_ms);

	return ret_val;
}



int get_cdma_info_NONSTOP_RET(int which, char *par)
{
	int	ret_val = 0;
	int	qidx;
		
	giCdmaStage = which;
//	giCdmaStep = STEP_SEND_INIT;

	ret_val = get_cdma_info_exec(which, par);

	if((ret_val == -1) || (ret_val == 6400))
	{	
		giCdmaStage = CDMA_STAGE_NONE;
		giCdmaStep = STEP_SEND_INIT;
	}

/**
do
{
	qidx = RxCdmaQ_get();
	if(qidx != -1) USART1_putchar(RxCdmaQ[qidx]);
} while (qidx != -1);
**/


//sprintf(str, "get_cdma_info_NONSTOP_RET:::%d.", ret_val);
//USART1_puts(str);

	return ret_val;
}

#define	MAX_USART4_BUFFER 	100
int zigbee_send()
{
	int  iz;
	static int	ret_val = 0;	//add 2016.4.8
	char szResponse[15];
	char str[100];

//sprintf(str, "zigbee_send::%d.", iZigbeeSendStage);
//USART1_puts(str);
								
	switch(iZigbeeSendStage)
	{
		case 0:
			ret_val = 0; //초기화 시킨다.
			if(giGigbeeSendingFlag) iZigbeeSendStage = 40;
			else iZigbeeSendStage = 300; //보낼 데이터 없을 때 딜레이 없이 바로 복귀 2014.8.21  change 100 to 300;	//end
			break;

		case 40:
			iZigbeeSendStage = 42;	//wait response
			for(iz=0; iz<MAX_USART4_BUFFER; iz++) Rx4Cmd[iz] = 0;
			UART4_puts(gszSendingGigbee);
USART1_puts(gszSendingGigbee);
			cdma_response_wait_timer_ms = 2 * DEF_1_SEC;
			break;

		case 42: //waiting server response

///for(iz=0; iz<strlen(Rx4Cmd); iz++) 	USART1_putchar(Rx4Cmd[iz]);

///sprintf(str, "Rx4Cmd::%d::%d::%s::", iZigbeeSendStage, strlen(Rx4Cmd), Rx4Cmd);
///USART1_puts(str);


			sprintf(szResponse, "OK%c%c", 0x0d, 0);
			if(check_response_zigbee(Rx4Cmd, szResponse, strlen(Rx4Cmd), strlen(szResponse))) // 전송이 정상적으로 끝났다. 
			{	
				iZigbeeSendStage = 100;

//				//response de
				for(iz=0; iz<16; iz++) USART1_putchar(Rx4Cmd[iz]);
//				if(check_response(RxCardBuffer2, "R40020", RxCardCounter2, 7)) //send ok
//				if(check_response(RxCardBuffer2, "R40120", RxCardCounter2, 7)) //send ok test
				{
					ret_val = 6400;
//					giGigbeeSendingFlag = 0;	//2014.8.25
					giSendingGigbeeLength = 0;	
//					giGigbeeSendingRetry = MAX_giGigbeeSendingRetry;	//2014.8.25
				}

		       	USART1_puts("GOOD PROCESS!!");
			}
			else if(check_response_zigbee(Rx4Cmd, "ERROR", strlen(Rx4Cmd), strlen("ERROR"))) 
			{	
				ret_val = 1;
				iZigbeeSendStage = 100;
		       	USART1_puts("BAD PROCESS!!");
			}
			else if( !cdma_response_wait_timer_ms) //time out
			{
				ret_val = -1;
				iZigbeeSendStage = 100;
				USART1_puts("TIMEOUT PROCESS!!");
			}
			else
			{
//sprintf(str, "ELSE ::%s.", Rx4Cmd);
//USART1_puts(str);
			}

/*
sprintf(str, "zigbee_send::42::ret_val=%d.", ret_val);
USART1_puts(str);
*/
			break;

		case 100:
			iZigbeeSendStage = 101;
			if(ret_val == 6400) giGigbeeSendingFlag = 0;	//2016.4.8 전송완료
/*
sprintf(str, "zigbee_send:100:ret_val=%d.", ret_val);
USART1_puts(str);
*/
			break;

		case 101:
       		switch(Stage_Idle(1, SEND_INTERVAL))	//waiting send idle
			{
				case 1:
					iZigbeeSendStage = 300;	//end
					break;
				default:
					break;
			}
			break;

		case 300:
			iZigbeeSendStage = 0;
			break;

		default:
			iZigbeeSendStage = 300;
			break;
	} //switch(iZigbeeSendStage)	

	return ret_val;
}


int check_response_zigbee(char *str1, char *str2, int buf_length, int substr_length)
{
	int iz;
	int ret_val = 0;
//    char *cp = (char *) str1;
    char *s1, *s2;

	char strz[50];

    if (!*str2) ret_val = 0;
	else
	{
		//because keco response included null character at c1 function
//	    while (*cp)
///		for(iz=0; iz<RxCdmaCounter; iz++)
		for(iz=0; iz<buf_length-substr_length; iz++)
    	{
///			cp = &str1[iz];
///        	s1 = cp;
			s1 = &str1[iz];
	        s2 = (char *) str2;

    	    while (*s1 && *s2 && !(*s1 - *s2)) { USART1_putchar(*s1); s1++, s2++; }
    	   	if (!*s2)
			{
				ret_val = 1;
				break;
			}
//	        cp++;
	    }
	}

	if(ret_val) giCdmaStopEnabled = 0;	//cdma 를 강제로 stop 실행을 해야 하는가 플래그

    return ret_val;
}


////////////////////////////////// LTE ///////////////////////////////////////////
////////////////////////////////// LTE ///////////////////////////////////////////
////////////////////////////////// LTE ///////////////////////////////////////////
////////////////////////////////// LTE ///////////////////////////////////////////
////////////////////////////////// LTE ///////////////////////////////////////////
////////////////////////////////// LTE ///////////////////////////////////////////

int SocketOpen_LU202F(char *ip, int port, int init)
{
	int iz;
	int	ret_val;
	int	iRetry;
	char szMsg[40];

	ret_val = 0;
	
	
//DEBUG_puts("SocketOpen_LU202F()");
	

	if(init == 0)
	{
		sprintf(szMsg, "AT$LGTTCPOP=%s,%d", ip, port);	//cdma_send_cmd("AT$LGTTCPOP=61.108.48.26,13001", 30);
		RxCdmaCounter = 0;		
		memset(RxCdmaBuffer, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화
		cdma_send_cmd_with_buffer_clear(szMsg, strlen(szMsg));
DEBUG_puts(szMsg);
	}
	else
	{
		if(check_response(RxCdmaBuffer, "$LGTTCPOP:0", RxCdmaCounter, 11)) ret_val = 1;	//0: 성공
		else if(check_response(RxCdmaBuffer, "$LGTTCPOP:4", RxCdmaCounter, 11)) ret_val = 4;	//4: 실패
/*
		ret_val = LGTTCPSTATE_LU202F();
*/
	}

	return ret_val;
}


int	tcpip_send_case_20(char *ip, int port, int init)
{
	int iz;
char sz[50];
	
	
	iz = SocketOpen_LU202F(ip, port,  iStabeTwoWaitCount++);
	
sprintf(sz, "SocketOpen_LU202F()::%d, %d", iz, iStabeTwoWaitCount);
DEBUG_puts(sz);

	if(iStabeTwoWaitCount <= 50) //wait response
	{
		if(iz == 1) iSendStage = 40;	//0: 성공
		else if(iz == 4)	//4: 실패
		{
			iStage100Count = 0;
			iSendStage = 100;
		}
	}
	else
	{
		iStage100Count = 0;
		iSendStage = 100;
	}

	return iSendStage;
}

void Stage_Four_Data_Sending_send_msg(char *msg, int length)  // 배출데이타를 전송 
{
	int  iz;
	char szMsg[15];

USART1_puts("Stage_4_send_message!");

//	for(iz=0; iz<length; iz++) { USART1_putchar(gszSendingData[iz]); }
//	for(iz=0; iz<length; iz++) USART1_putchar(msg[iz]);

	cdma_send_msg_with_buffer_clear(msg, length);	 // 측정 데이타 보내는 하수 
}


int Stage_Four_Data_Sending_wait_response(int init, int wait)  //배출데이타를 전송 후 리턴값 기다림
{
	int iz, iz0;
	int  ret_val;
	char szResponse[15];
	char Send_Ok = 0;
    static int Sending_Retry_Counter = 0;   //0528
	char str[100];

	IWDG_ReloadCounter();

	ret_val = 0;

	if(init == 0) Sending_Retry_Counter = 0;

sprintf(str, "stage_four_1 %d ", Sending_Retry_Counter);
USART1_puts(str);

//sprintf(str, "(%d)", RxCdmaCounter);
//USART1_print(str);
for(iz=0; iz<RxCdmaCounter; iz++) DEBUG_putchar(RxCdmaBuffer[iz]);
	
	
	if(check_response(RxCdmaBuffer, "G400", RxCdmaCounter, 4)) //gtech monitoring status 전송이 정상적으로 끝났다.  2014.10.16
	{	
		ret_val = 400;
       	USART1_puts("::G400 GOOD::");
	}
	else if(check_response(RxCdmaBuffer, "$LGTTCPCL:0", RxCdmaCounter, 11)) //gtech monitoring status 전송이 정상적으로 끝났다.  2014.10.16
	{	
		ret_val = 1;
       	USART1_puts("::$LGTTCPCL:0::");
		iz0 = check_response(RxCdmaBuffer, "$LGTTCPRD:", RxCdmaCounter, 10);
		if(iz0)
		{
			ret_val = 100;

			for(iz=0; iz<RxCdmaCounter; iz++)
			{
				gszCdmaReturn[iz] = RxCdmaBuffer[iz0+iz];
				if(99<iz) break;
			}
		}
		else
			for(iz=0;iz<99;iz++) gszCdmaReturn[iz] = RxCdmaBuffer[iz];

		giCdmaReturn = iz;
	}
	else if(wait < ++Sending_Retry_Counter) //time out
	{
		ret_val = -1;
		USART1_puts("::BAD PROCESS!!::");
	}

	return ret_val;
}




//LTE MODEM
void tcpip_send_form_zigbee_data(char *ip, int port)
{
	int  iz;
	char server_ip[25];

	char s[300];

	switch(iSendStage)
	{
		case 0:
			
//DEBUG_print("tcpip_send_form_zigbee_data() STAGE - 0");
//DEBUG_putchar('-');
//DEBUG_putchar(giGigbeeSendingFlag+'0');
//DEBUG_puts("");
		
			if( !giGigbeeSendingFlag) iSendStage = 300;	//보낼 데이터 없을 때 딜레이 없이 바로 복귀 2014.8.21  change 100 to 300;	//end
			else 
			{
				iSendStage = 20;
				
				if(giGigbeeSendingRetry) --giGigbeeSendingRetry;

				if( !giGigbeeSendingRetry)
				{
					//못보내면 버리네 다시 확인 할 것.
					//giGigbeeSendingFlag = 0;

					giGigbeeSendingRetry = MAX_giGigbeeSendingRetry;	//2014.8.25

					// next port calc
					++giGtechServerCurrentPortIndex;
					giGtechServerCurrentPortIndex = giGtechServerCurrentPortIndex % MAX_GTECH_SERVER_PORT_QTY;
				}
			}

///sprintf(str, "R=%d, P=%d, O=%d", giGigbeeSendingRetry, MONITORING_SERVER_PORT, giGtechServerCurrentPortIndex);
///USART1_puts(str);

			break;
			
			
		case 20:
//DEBUG_print("tcpip_send_form_zigbee_data() STAGE - 20");
//DEBUG_puts("");		
			SocketClose_LU202F();
			iSendStage = 22;
			iStabeTwoWaitCount = 0;
			break;

		case 22:
//DEBUG_print("tcpip_send_form_zigbee_data() STAGE - 22");
//DEBUG_puts("");		
			iSendStage = tcpip_send_case_20(ip, port, iStabeTwoWaitCount);
			break;
		

		case 40:
//DEBUG_print("tcpip_send_form_zigbee_data() STAGE - 40 -- ");
DEBUG_print("gszSendingGigbee::");
DEBUG_puts(gszSendingGigbee);
			sprintf(s, "AT$LGTTCPWR=%d,%s", giSendingGigbeeLength, gszSendingGigbee);
//			Stage_Four_Data_Sending_send_msg(gszSendingGigbee, giSendingGigbeeLength);
			Stage_Four_Data_Sending_send_msg(s, strlen(s));
			iStage40Count = 0;	//2015.9,22 41,42 통합
			iSendStage = 42;	//wait response
///			break;
		
		case 42:
//DEBUG_print("tcpip_send_form_zigbee_data() STAGE - 42");
//DEBUG_puts("");		
			LGTTCPSTATE_LU202F();
		
//       		switch(Stage_Four_Data_Sending_wait_response(iStage40Count++, 20))	//waiting server response
       		switch(Stage_Four_Data_Sending_wait_response(iStage40Count++, 20))	//waiting server response
			{
			 	case -1: //time out
					iSendStage = 100;
					iStage100Count = 0;
				case 1: //send ok
					Keco_card_info_flag = 0;

					iSendStage = 100;
					iStage100Count = 0;

//					//keco response de
for(iz=0; iz<16; iz++) USART1_putchar(RxCdmaBuffer[iz]);
//					if(check_response(RxCardBuffer2, "R40020", RxCardCounter2, 7)) //send ok
//					if(check_response(RxCardBuffer2, "R400", RxCardCounter2, 4)) //send ok test
					{
						giSendingGigbeeLength = 0;	
						giGigbeeSendingRetry = MAX_giGigbeeSendingRetry;	//2014.8.25
						giGigbeeSendingFlag = 0;	//2014.8.25
					}
					break;

				case 400:
					iSendStage = 100;
					iStage100Count = 0;
					giSendingGigbeeLength = 0;	
					giGigbeeSendingRetry = MAX_giGigbeeSendingRetry;	//2014.8.25
					giGigbeeSendingFlag = 0;	//2014.8.25
					break;

				default:
					break;
			}
			break;

		case 100:
//DEBUG_print("tcpip_send_form_zigbee_data() STAGE - 100");
//DEBUG_puts("");	
			SocketClose_LU202F();
			iSendStage = 102;
			Keco_card_info_flag = 0;
			break;
		
		case 102:
       		switch(Stage_Idle(iStage100Count++, SEND_INTERVAL))	//waiting send idle
			{
				case 1:
					iSendStage = 300;	//end
					break;
				default:
					break;
			}
			break;

		default:
			Keco_card_info_flag = 0;
			iSendStage = 300;
			break;
	} //switch(iSendStage)
}					  



//void Cdma_Force_Stop_step2_tcpcl_20140806(void)  // 전화 강제로 끊기 
int SocketClose_LU202F()
{
	int  iz; 
	int	 iRetry;
	int	 ret_val;

	ret_val = 0;
	iRetry = 3;

//DEBUG_puts("SocketClose_LU202F()----------------");
	if(LGTTCPSTATE_LU202F() == 6) return 1;	//already closed
//DEBUG_puts("SocketClose_LU202F()------POINT-----");

	do
	{
		RxCdmaCounter = 0;		
		memset(RxCdmaBuffer, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화

		cdma_send_cmd_with_buffer_clear("AT$LGTTCPCL", 11);

		Delay_ms(10);	//2014.04.02  add because fail

		for(iz=0; iz<200; iz++)
		{
			Delay_ms(1);

			if(check_response(RxCdmaBuffer, "$LGTTCPCL:0", RxCdmaCounter, 11))
			{	
				ret_val=1;
//			    USART1_puts("SocketClose_LU202F OK!");
				break;
			}
			else if(check_response(RxCdmaBuffer, "$LGTTCPCL:4", RxCdmaCounter, 11))
			{	
				ret_val=4;
//			    USART1_puts("SocketClose_LU202F FAIL!");
	            break;
			}
		}

////		Keco_card_info_flag = 0;	//cdma data 받아오을때 

	} while(--iRetry && !ret_val);

	return ret_val;
}


int LGTTCPSTATE_LU202F()
{
	int  iz; 
	int	 ret_val;

	ret_val = 0;

//DEBUG_puts("LGTTCPSTATE_LU202F()==================");
	
////		Keco_card_info_flag = 1;	//cdma data 받아오을때 

		cdma_send_cmd("AT$LGTTCPSTATE?", 15);
//		memset(RxCdmaBuffer, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화
//		RxCdmaCounter = 0;		

		Delay_ms(20);	//2014.04.02  add because fail
	
		for(iz=0; iz<200; iz++)
		{
			Delay_ms(1);

			if(check_response(RxCdmaBuffer, "$LGTTCPSTATE:3", RxCdmaCounter, 14))	//TCP 접속 대기 상태
			{	
				ret_val = 3;
			    USART1_puts("Socket wating...");
				break;
			}
			else if(check_response(RxCdmaBuffer, "$LGTTCPSTATE:4", RxCdmaCounter, 14))	//TCP 접속 성공 상태
			{	
				ret_val = 4;
			    USART1_puts("Socket opened!");
				break;
			}
			else if(check_response(RxCdmaBuffer, "$LGTTCPSTATE:5", RxCdmaCounter, 14)) //TCP Close 대기 중인 상태
			{	
				ret_val = 5;
			    USART1_puts("Socket Close waiting...");
	            break;
			}			
			else if(check_response(RxCdmaBuffer, "$LGTTCPSTATE:6", RxCdmaCounter, 14)) //TCP Close 성공 상태
			{	
				ret_val = 6;
			    USART1_puts("Socket Closed!");
	            break;
			}
		}

////		Keco_card_info_flag = 0;	//cdma data 받아오을때 

//DEBUG_puts("LGTTCPSTATE_LU202F()========  END  ==========");
	
	
	return ret_val;
}


void websms_connection()
{
	char szMsg[50];
	
	switch(iSendStage)
	{
		case 0:
//			get_cdma_information();
			iSendStage = 20;
//			break;
		
		case 20:
			if(!gbWebSMSConnection) { iSendStage = 300; break; }
			gbWebSMSConnection = 0;
//					giWebSMSConnection = MultiSocketOpen_LU202F(2, "112.222.115.165", 7701, 300);
//			giWebSMSConnection = MultiSocketOpen_LU202F(2, "112.217.212.250", 1529, 300);
//			giWebSMSConnection = MultiSocketOpen_LU202F(2, "112.217.212.250", 1530, 300);
			giWebSMSConnection = MultiSocketOpen_LU202F(2, "112.217.212.250", 8330, 300);
			
			
//					giWebSMSConnection = MultiSocketOpen_LU202F(2, "112.217.212.250", 1528, 300);
//					giWebSMSConnection = MultiSocketOpen_LU202F(2, "112.222.115.165", 1529, 300);
			if((giWebSMSConnection == 100) || (giWebSMSConnection == 4)) cdma_send_cmd("AT$LGTMTCPCL=2", 14);
			iSendStage = 40;

			sprintf(szMsg, "open state %d.%d.", giWebSMSConnection, giWebSMSPreConnection);
			USART1_puts(szMsg);
			break;
		case 40:
			if((giWebSMSConnection == 0) || ((giWebSMSConnection == 5) && (giWebSMSConnection != giWebSMSPreConnection)))
			{
				//send tel no
				sprintf(szMsg, "AT$LGTMTCPWR=%d,%d,%s", 2, strlen(jangbi.cdma.telno), jangbi.cdma.telno);
				cdma_send_cmd( szMsg, strlen(szMsg));
				giWebSMSPreConnection = giWebSMSConnection;
			}
			iSendStage = 100;
			iStage100Count = 0;
			break;
		case 100:
			switch(Stage_Idle(iStage100Count++, 1))	//waiting send idle
			{
				case 1:
					iSendStage = 300;	//end
					break;
				default:
					break;
			}
			break;
		default:
			iSendStage = 300;	//end
			break;
	}
}	
		

//2015.9.15
#define	MAX_RETRY	10
int MultiSocketOpen_LU202F(int socketno, char *ip, int port, int wait)
{
	int iz;
	int	ret_val;
	int	iRetry;
	char szMsg[40];

	ret_val = 4; //closed
	iRetry = MAX_RETRY;	 

	sprintf(szMsg, "AT$LGTMTCPOP=%d,%s,%d", socketno, ip, port);	//cdma_send_cmd("AT$LGTTCPOP=61.108.48.26,13001", 30);

	Keco_card_info_flag = 1;	//cdma data 받아오을때 

	do
	{
		RxCdmaCounter = 0;		
		memset(RxCdmaBuffer, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화

		cdma_send_cmd(szMsg, strlen(szMsg));

		Delay_ms(10);	//2015.8.20

//		for(iz=0; iz<200; iz++)
		for(iz=0; iz<wait; iz++)
		{
			Delay_ms(1);
			if(check_response(RxCdmaBuffer, "$LGTMTCPOP:2,0", RxCdmaCounter, 14))	//success
			{
				ret_val = 0;
				break;
			}
			if(check_response(RxCdmaBuffer, "$LGTMTCPOP:2,5", RxCdmaCounter, 14))	//already opened
			{
				ret_val = 5;
				break;
			}
			else if(check_response(RxCdmaBuffer, "$LGTMTCPOP:2,4", RxCdmaCounter, 14)) //FAIL
			{
				giWebSMSPreConnection = 4;
				ret_val = 4;
				break;
			}
		}

	} while(--iRetry && !ret_val);

	Keco_card_info_flag = 0;	//cdma data 받아오을때 

	return ret_val;
}


int MultiSocketClose_LU202F(int socketno, int wait)
{
	int iz;
	int	ret_val;
	int	iRetry;
	char szMsg[40];

	ret_val = 4; //closed
	iRetry = MAX_RETRY;	 

	sprintf(szMsg, "AT$LGTMTCPOP=%d ", socketno);

	Keco_card_info_flag = 1;	//cdma data 받아오을때 

	do
	{
		RxCdmaCounter = 0;		
		memset(RxCdmaBuffer, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화

		cdma_send_cmd(szMsg, strlen(szMsg));

		Delay_ms(10);	//2015.8.20

//		for(iz=0; iz<200; iz++)
		for(iz=0; iz<wait; iz++)
		{
			Delay_ms(1);
			if(check_response(RxCdmaBuffer, "$LGTMTCPCL:2,0", RxCdmaCounter, 14))	//success
			{
				ret_val = 0;
				break;
			}
			if(check_response(RxCdmaBuffer, "$LGTMTCPOP:2,5", RxCdmaCounter, 14))	//already opened
			{
				ret_val = 5;
				break;
			}
			else if(check_response(RxCdmaBuffer, "$LGTMTCPOP:2,4", RxCdmaCounter, 14)) //FAIL
			{
				giWebSMSPreConnection = 4;
				ret_val = 4;
				break;
			}
		}

	} while(--iRetry && ret_val==4);

	Keco_card_info_flag = 0;	//cdma data 받아오을때 

	return ret_val;
}



int SocketOpen(char *ip, int port)
{
	int iz;	
	int	ret_val = 0;
	int retry_counter, wait_counter;
	int stage = 20;
	char sz[50];	
	
	for(retry_counter=0; retry_counter<50; retry_counter++)
	{
		delay_ms(100);
		switch(stage)
		{
			case 20:
DEBUG_puts("SocketOpen::SocketClose_LU202F() STAGE - 20");		
				SocketClose_LU202F();
				wait_counter = 0;
				stage = 22;
				break;

			case 22:
				iz = SocketOpen_LU202F(ip, port, wait_counter++);
sprintf(sz, "SocketOpen::SocketOpen_LU202F()::%d, %d", iz, wait_counter);
DEBUG_puts(sz);
				break;
		}
		if(iz == 1) { ret_val = 1; break; }	//0: 성공
		else if(iz == 4) break;		//4: 실패
	}
	
	return ret_val;
}

/* end of file */
