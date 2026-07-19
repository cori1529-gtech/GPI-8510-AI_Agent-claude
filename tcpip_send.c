/**

LGUPLUS++

//#ifdef __________________LGUPLUS_______
//#endif	//ifdef __________________LGUPLUS_______

**/

#include "string.h"
#include "stm32f10x_lib.h"
#include "global.h"
//#include "card_info.h"
#include "aria.h"
#include "tcpip_send.h"
#include "sms.h"
#include "i2c_ee.h"

extern volatile u16  guWatchdogTimer;		// 초 test_routine  clear, TIM3 에서 검사해서 리셋


//#define	LGUPLUS_TEST_DEVICE		"W2S-001-01-1"	//test server
//#define	LGUPLUS_TEST_DEVICE		"WZZ-033-02-1"	//real server
volatile	char LGU_TEST_JANGBI_CODE[]="WZZ-033-02-1";

extern	volatile int	iStabeTwoWaitCount;	//2015.9.21


//2014.11.14 use monitoring only
extern volatile s32 Before_Weight_for_monitoring, Before_Adc_for_monitoring, diff1_for_monitoring;

extern volatile u32  gxlCostMinWeight;	//ver 1.32s3 2014.11.10

extern volatile char gszGtechMonitoringServerIp[]; 	//gtech monitoring server ip
extern volatile char gszGtechMonitoringServerPort[];//gtech monitoring server base port
extern volatile u16  gwGtechMonitoringServerPort;	//gtech monitoring server base port

extern volatile char gszGtechConfigServerPort[];//gtech config (setting) server base port
extern volatile u16  gwGtechConfigServerPort;	//gtech config (setting) server base port


//2015.4.14
//extern volatile char	gszLguMonitoringServerIp[]; //lgu+ web monitoring REAL server ip
extern volatile char	gszLguMonitoringServerIp[]; //lgu+ web monitoring TEST server ip
extern volatile char	gszLguMonitoringServerPort[];	//lgu+ web monitoring server base port
extern volatile u16		gwLguMonitoringServerPort;	//lgu+ web monitoring server base port
extern volatile u16		LGU_TEST_MODE_JANGBI_CODE;	//lguplus jangbi code 57788 -> WZZ...

//2015.4.23
extern volatile char 	gszIapServerIp[]; 	//gtech monitoring server ip
extern volatile char 	gszIapServerPort[];//gtech monitoring server base port
extern volatile u16  	gwIapServerPort;	//gtech monitoring server base port

extern volatile char 	gszKecoServerIp[]; 	//gtech monitoring server ip
extern volatile char 	gszKecoServerPort[];//gtech monitoring server base port
extern volatile u16  	gwKecoServerPort;	//gtech monitoring server base port

extern volatile char	gszCipServerIp[]; 	//one time reply server ip
extern volatile char	gszCipServerPort[]; //one time reply server port
extern volatile u16		gwCipServerPort; 	//one time reply server port


/* i2c-ee.c */
extern volatile u32  gxlCostPerOnekilo;  
extern volatile u16  Nor_Flash_Counter;

//main.c
extern	u8 Left_Moter_Status;
extern	u8 Left_Card_Status;
extern	u8 Left_BasketFull_Status;
extern	u8 Left_BasketBeing_Status;
extern	u8 Left_BasketDoor_Status;
extern	u8 Left_Scale_Status;
extern	u8 Left_CardReader_Status;

extern	s32 Before_Weight, After_Weight;

extern	u8	Usart2_Interrupt_flag;

////extern	vu16 RxCounter2_sms;
///extern 	u8	RxBuffer2_sms[];

extern	u32 RxCounter1;
extern 	u8	RxBuffer1[];


extern 	char	Send_Once_Flag; // 실제배출데이탈 한번만 보내고 기다릴때 체크하는 
extern 	int 	Sending_Retry_Counter;
extern 	int		iTcpipRecivedNo, iTcpipRecivedCloseNo;

extern	u8 		RxCardBuffer2[];
extern	vu16	RxCardCounter2;
extern	u8 		Keco_card_info_flag;

extern	volatile u16		Nor_Flash_Counter;

extern	u8   Stage_One_flag;
extern	u8   Stage_Two_flag;
extern	u8   Stage_Three_flag;
extern	u8   Stage_Four_flag;

extern u8  cdma_error_flag; //  시간 읽을때 cdma 가 없으면 아무것도 날아오지 않는다. 
extern u8  time_error_flag;
extern u8  cdma_error_check_flag;




// u32 gxlLocalCode_One ; // 지역코드  
// u32 gxlLocalCode_Two ; // 지역코드  
//extern	u32 gxlLocalCode_Three; // 지역코드 
// u16 Nor_Flash_Counter;

extern	u8   Once_flag;	//u8   Once_flag=0;
//char Send_Once_Flag=0; // 실제배출데이탈 한번만 보내고 기다릴때 체크하는 
extern	char Send_Ok_Flag; //char Send_Ok_Flag=0;	// 실제배출데이탈 한번만 보내고 기다릴때 체크하는 

extern	u8   New_baichul;  //u8   New_baichul=0;   새로운 배출이 있을대 


//    char szData[256];
//	char szCData[256];
//	char szDataLength[10];
//extern	int	 iDataLength;
extern	char szHeader[];	//char szHeader[100];
extern	int ret_val;		//int ret_val=0;
/*
	char szUID[16+1];
	char szWeight[6+1];
	char szCost[6+1];
	int  iWeight;
	int  iCost;
	int	i;
	char szDateTime[20];
	int jangbi_code_length;
*/
extern	char sRet[200];		//char sRet[200];

//전역변수 
struct timedate
{
	int	year;
	int	month;
	int date;
	int hour;
	int min;
	int sec;
	int msec;
};	// tm;

struct cdma_info
{
	char telno[13];
	unsigned int  rssi;
	struct timedate tm;
};	// cdma;

struct jangbi_info
{
	unsigned long uid;
	char code[17];	// = {"W2V-201-01-1    "};
	unsigned int  rate;	//원/kg
	char mode;	//real, test
	char speaker_vol[10];	//speaker volume  day, night ....

	unsigned long weight;
	unsigned long volume;	//부피

	//....

	int		send_c1_time;			//배출정보 전송할 시간 분. (TEST ONLY)
	int		send_c1_flag;
	int		send_c1_success_flag;	//배출정보 전송할 플래그

	int		send_c4_time;			//상태정보 전송할 시간 분.
	int		send_c4_flag;
	int		send_c4_success_flag;	//상태정보 전송할 플래그

	struct timedate tm;
	struct cdma_info cdma;
};
extern struct jangbi_info jangbi;

#define	MAX_RETRY	10 // ORIGIN 3
#define	MAX_CDMA_RECEIVED	1000

#define	MAX_RETRY_IAP_SERVER_CONNECT	30	//add 2014.10.20


#define NOR_MAX_COUNTER   	2
struct baichul_info_salsa	// 배출정보 
{
	char date[14]; 	        // 0~13                배출한시간                               // 0~13
    unsigned int  weight; 	// 14~15       배출한양   10000
	char card1_uid[16]; 	// 16~31      원래 카드는 16 인데 1그냥 더했음
	char sdong[4];			// 32~35	
	unsigned int  dong;		// 36~37
	char sho[4];			// 38~41
	int Status_keco_new;	//  42
	int Status_keco_send;	//  43
	int Status_gtech;		//  44
	int Status_lguplus;		//  45
	int CardOrPassword;		//  46
	int Whichcard; // 카드 	//  47 
};
extern	struct baichul_info_salsa baichul_info_nor[NOR_MAX_COUNTER];

extern  union { u32 dw; u16 u[2]; u8 c[4];} jisu;		//union { u32 dw; u16 u[2]; u8 c[4];} jisu;
extern	char str[];	//char str[50];
extern	char local_code[];	//char local_code[12];

//move global.h #define	RXCARDBUFFER2SIZE	8000
#define	NOR_BAICHUL_BASE	0x64780000	// ~7F0000	64 * 12000 = 768K
//2015.8.20 #define	STAGE4_WAIT_COUNTER	300
#define	STAGE4_WAIT_COUNTER	20

u32  Cdma_Counter=0;

//int Stage_One_DataLoading(char *jangbi_code,  char *cardno, unsigned int uWeight);
//int Stage_Two_IPSetting(char *ip);
int Stage_Two_IPSetting(char *ip, int port, int init);
//int Stage_Three_Keco_Connection(void);
void Stage_Three_Keco_Connection_send_cmd(void);	//send ATDT019
int  Stage_Three_Keco_Connection_wait_response(int init, int wait_limit);//wait tcp connection
void Stage_Four_Data_Sending_send_msg(char *msg, int length);  // 배출데이타를 전송 
int Stage_Four_Data_Sending_wait_response(int init, int wait);  //배출데이타를 전송 후 리턴값 기다림
//int Stage_Four_Data_Sending(char *msg, int length, char *ret);
void Cdma_Force_Stop(void);  // 전화 강제로 끊기 
int	Stage_Idle(int init, u32 wait);  //리턴값 기다림

void tcpip_send_c1_data_gtech_20141020(char *ip, int port);	//2014.10.20

void tcpip_send_c1_data_lguplus_20150415(char *ip, int port);
void tcpip_send_c4_status_lguplus_20150414(char *ip, int port);	//2015.4.14
void Send_to_Lguplus_Status(); //Status를 전송한다.
void lguplus_monitoring_status_data_make();
void tcpip_send_CIP_20140808(char *ip, int port);

void modem_reset();

extern void read_Gtech_Monitoring_IP();
extern void read_Lguplus_Monitoring_IP();
extern void read_Iap_Server_IP();

//2015.8.20
//int SocketOpen_LU202F(char *ip);
int SocketOpen_LU202F(char *ip, int port, int init);
int SocketClose_LU202F();
int LGTTCPSTATE_LU202F();
int get_cdma_with_rx2cardbuffer(char *cmd, char *response, int retry);

//2015.9.15
int	giWebSMSConnection = 4;
int	giWebSMSPreConnection = 44;
int MultiSocketOpen_LU202F(int socketno, char *ip, int port, int wait);

int	tcpip_send_case_20(char *ip, int port, int init);

void cdma_putchar(char ch)
{
/*
	USART_SendData(USART2, ch);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
*/
	USART2_putchar(ch);
}

int cdma_send_msg(char *msg, int length)
{
	int i;

	//MONITORING
	for(i=0; i<length; i++) USART1_putchar(msg[i]);
	//send to cdma
	for(i=0; i<length; i++) cdma_putchar(msg[i]);

	return i;
}

int cdma_send_cmd(char *cmd, int length)
{
	int i;

//	for(i=0; i<length; i++) cdma_putchar(cmd[i]);
	i = cdma_send_msg(cmd, length);
	cdma_putchar(0x0d);
	cdma_putchar(0x0a);

	USART1_putchar(0x0d);
	USART1_putchar(0x0a);

	return i;
}


void get_jangbi_code()
{
    gszJangbiCode[0] = local_code[3];
    gszJangbiCode[1] = local_code[2];
    gszJangbiCode[2] = local_code[1];
    gszJangbiCode[3] = local_code[0];
    gszJangbiCode[4] = local_code[7];
    gszJangbiCode[5] = local_code[6];
    gszJangbiCode[6] = local_code[5];
    gszJangbiCode[7] = local_code[4];
    gszJangbiCode[8] = local_code[11];
    gszJangbiCode[9] = local_code[10];
    gszJangbiCode[10] = local_code[9];
    gszJangbiCode[11] = local_code[8];

	gszJangbiCode[12] = 0;
}

//2015.4.23
//port 57788:WZZ- , else jangbi_code
void get_lguplus_jangbi_code()	//2015.4.23
{
/******* LGUPLUS++
	int iz = 0;

	if(LGU_TEST_MODE_JANGBI_CODE)
	{
		LGU_TEST_JANGBI_CODE[iz++] = 'W';
		LGU_TEST_JANGBI_CODE[iz++] = 'Z';
		LGU_TEST_JANGBI_CODE[iz++] = 'Z';
	}
	for( ; iz<=11; iz++) LGU_TEST_JANGBI_CODE[iz] = gszJangbiCode[iz]; 
******* LGUPLUS++ ***/
}

//add ver 1.32r2 2014.10.14
void get_cdma_information_now()
{
	if(gbGetCdmaTelno) { get_cdma_telno(); gbGetCdmaTelno = 0; }
	if(gbGetCdmaRssi)  { get_cdma_rssi();	gbGetCdmaRssi = 0; }
	if(gbGetCdmaTime)  { get_cdma_time(); 	gbGetCdmaTime = 0; }
	if(gbGetCdmaGigiguk)  { get_cdma_gigiguk();	gbGetCdmaGigiguk = 0; }
}

void get_cdma_information()
{
	switch(iSendStage)
	{
		case 0:  if(gbGetCdmaTelno) { get_cdma_telno(); gbGetCdmaTelno = 0; iSendStage = 10; break; }
		case 10: if(gbGetCdmaRssi)  { get_cdma_rssi();	gbGetCdmaRssi = 0; 	iSendStage = 20; break; }
		case 20: if(gbGetCdmaTime)  { get_cdma_time(); 	gbGetCdmaTime = 0; 	iSendStage = 30; break; }
		case 30: if(gbGetCdmaGigiguk)  { get_cdma_gigiguk();	gbGetCdmaGigiguk = 0; }
		default: iSendStage = 300;
//				 ++giSendingType;
//				 iSendStage = 0;
				 break;
	}
}

#ifdef ___inging___P
int get_ip(char *str, char *ip, u16 *port)
{
	char szip_port[30];

		Sms_Message_Number = 0;
		for(iz=0; iz<RxCounter2_sms; iz++)
		{
			//@MPO=033# 
			smsbuffer[4+Sms_Message_Number] = RxBuffer2_sms[iz];
			if(RxBuffer2_sms[iz]=='@') Sms_Message_Number = 0;
			else if(RxBuffer2_sms[iz]=='#')
			{	
				//"GMSIP=114.206.253.205,7701"
				if(    smsbuffer[4] == 'G'
					&& smsbuffer[5] == 'M'
					&& smsbuffer[6] == 'S'
					&& smsbuffer[7] == 'I'
					&& smsbuffer[8] == 'P'
					&& smsbuffer[9] == '='
				)
				{
					dot_counter = 0;
					port_counter = 0;
					ip_flag = 1;
					for(iz0=0; iz0<Sms_Message_Number; iz0++)
					{
						if(smsbuffer[iz0+10] == '#') break;
						else if(smsbuffer[iz0+10] == ',') { ip_flag = 0; szip[iz0] = 0; }
						else if(smsbuffer[iz0+10] == '.')
						{	
							dot_counter++;			 
							if((ip_flag) && (iz0<=20)) szip[iz0] = smsbuffer[iz0+10];
						}
						else if((ip_flag) && ('0'<=smsbuffer[iz0+10]) && (smsbuffer[iz0+10]<='9') && (iz0<=20)) szip[iz0] = smsbuffer[iz0+10];
						else if((!ip_flag) && ('0'<=smsbuffer[iz0+10]) && (smsbuffer[iz0+10]<='9') && (port_counter<4))
						{	
							szport[port_counter++] = smsbuffer[iz0+10];

							USART1_putchar(port_counter-1+'0');
							USART1_putchar(szport[port_counter-1]);
						}
						else
						{
							//fail - invalid data
							ip_flag = 1;
							dot_counter = 0;
							break;
						}
					}

					if((ip_flag==0) && (dot_counter==3))
					{
						szport[port_counter] = 0;
						iz0 = 0;
						do{ gszGtechMonitoringServerIp[iz0] = szip[iz0]; } while(szip[iz0++]);
						iz0 = 0;
						gwGtechMonitoringServerPort = 0;
						do{ gszGtechMonitoringServerPort[iz0] = szport[iz0]; 
							if(szport[iz0]) gwGtechMonitoringServerPort = (gwGtechMonitoringServerPort*10) + (gszGtechMonitoringServerPort[iz0]-'0');

							USART1_putchar(iz0+'0');
							USART1_putchar(szport[iz0]);
						} while(szport[iz0++]);

						USART1_puts(gszGtechMonitoringServerIp);
						USART1_puts(gszGtechMonitoringServerPort);
					}
				}
				else Usart3_SMS_flag=1;

				break;
			}
			else ++Sms_Message_Number;
}
}
#endif //#ifdef ___inging___P

//keco test server "211.112.135.142,13001"
//keco real server "61.108.48.26,13001"

#ifdef ___test ing ____
//2015.4.20 ver 1.33a1 add  0:none, 1:GTECH, 2:LGU+
int ip_setting(u8 ADR_Ip, u8 ADR_Port, char *ip, char *sport, u16 *port, int portcount) //2015.4.20 add
{
	int	iz0;
	char szip[23+1]; 	//ip
	u32  iport;
	char szport[5+1];	//port
	int  port_counter;

	union { u32 dw; u16 u[2]; u8 c[4];} temp;

	port_counter = portcount;
	I2C_EE_BufferWrite(temp.c, ADR_Ip, 4);	//I2C_EE_BufferWrite(temp.c, ADR_GtechMonitoringServerIp, 4);
	I2C_EE_BufferRead( temp.c, ADR_Ip, 4);	//I2C_EE_BufferRead( temp.c, ADR_GtechMonitoringServerIp, 4);

    temp.dw = iport;
	I2C_EE_BufferWrite(temp.c, ADR_Port, 4);	//I2C_EE_BufferWrite(temp.c, ADR_GtechMonitoringServerPort, 4);
	I2C_EE_BufferRead( temp.c, ADR_Port, 4);	//I2C_EE_BufferRead( temp.c, ADR_GtechMonitoringServerPort, 4);

	szport[port_counter] = 0;
	iz0 = 0;
	do{ ip[iz0] = szip[iz0]; } while(szip[iz0++]);
	//do{ gszGtechMonitoringServerIp[iz0] = szip[iz0]; } while(szip[iz0++]);
	iz0 = 0;
	*port = 0;	//gwGtechMonitoringServerPort = 0;
	do{ 
		sport[iz0] = szport[iz0]; 
		if(szport[iz0]) *port = (*port*10) + (sport[iz0]-'0');
					
		USART1_putchar(iz0+'0');
		USART1_putchar(szport[iz0]);
	} while(szport[iz0++]);
		
	USART1_puts(ip);
	USART1_puts(sport);
}
#endif 	//#ifdef ___test ing ____



void tcpip_send_20140806(char *ip, int port)  // 2014년8월13일 	  test
//void tcpip_send_20140806_original(char *ip, int port)  // 2014년8월13일 	  test
{
//<< 2014.10.16
	char szMsg[40];	//add 2015.9.15


//sprintf(szip, "STAGE::%d", giSendingType);
//USART1_puts(szip);

	switch(giSendingType)
	{
		case 0:
			switch(iSendStage)
			{
				case 0:
					get_cdma_information();
					iSendStage = 20;
					break;
				
				case 20:
					if(!gbWebSMSConnection) { iSendStage = 300; break; }
					gbWebSMSConnection = 0;
//					giWebSMSConnection = MultiSocketOpen_LU202F(2, "112.222.115.165", 7701, 300);
					giWebSMSConnection = MultiSocketOpen_LU202F(2, "112.217.212.250", 1529, 300);
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

			if(iSendStage == 300)
			{	++giSendingType;
				iSendStage = 0;
				giSendingStageRetry = 0;	//2015.8.20
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///testonly		giSendingType = 3;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//2015.4.24		

goto case_1;
			}
			break;

		case 1:	//배출정보 전송
case_1:	//배출정보 전송

/*
//Nor_Flash_Counter = 7000;
Nor_Flash_Counter = 0xffff;

USART1_puts("case 1111: start tcpip_send_20140806");
sprintf(szip, " -- %d --", Nor_Flash_Counter);
USART1_puts(szip);
*/
			//USART1_puts("case 1: tcpip_send_20140806");
			//tcpip_send_c1_data_20140806("211.112.135.142", 13001); //test server 
			//tcpip_send_c1_data_20140806("61.108.48.26", 13001);
/***
			if(gbKecoForceSend == 6099)
			{
				if(tcpip_send_c1_data_20140806(gszKecoServerIp, gwKecoServerPort, 6099) == 300)
				{
					//강제 전송 끝
					gbKecoForceSend = 0;	//2015.9.22
				}
			}
			else
****/
				tcpip_send_c1_data_20140806(gszKecoServerIp, gwKecoServerPort, 0xffff);
			//	baichul_info_nor[1].Status_keco_new	61.108.48.26,13001
			if(iSendStage == 300) // 배출완료가 되면 (실패를 했던 성공을 해던 )
			{	
/////				if(gbKecoForceSend != 6099) 
				++giSendingType;
				iSendStage = 0;
goto case_2;
//				goto case_default;	//TEST ONLY
			}
			break;

		case 2:	//공단 상태정보 전송
case_2:	//공단 상태정보 전송
			//USART1_puts("case 2: tcpip_send_20140806");
			//tcpip_send_c4_status_20140808("211.112.135.142", 13001);

		    //tcpip_send_c4_status_20140808("61.108.48.26", 13001);
		    tcpip_send_c4_status_20140808(gszKecoServerIp, gwKecoServerPort);
	  
			if(iSendStage == 300)
			{	
				++giSendingType;
				iSendStage = 0;
				giSendingStageRetry = 0;	//2015.8.20
//2015.4.24				
goto case_3;
			}
			break;

		case 3: //지테크 모니터링 서버 상태정보
case_3: //지테크 모니터링 서버 상태정보
//USART1_puts("case 3 : tcpip_send_c4_status_gtech_20140808");

//			tcpip_send_c4_status_gtech_20140808("116.123.106.53", 9900);
//			tcpip_send_c4_status_gtech_20140808("119.194.243.45", 9900); //sangmii status
//			tcpip_send_c4_status_gtech_20140808("116.123.106.53", 9901); //echo 
//			tcpip_send_c4_status_gtech_20140808("116.123.106.53", 9903+(MAX_giGtechSendingStatusRetry-giGtechSendingStatusRetry)-1); //9901 ~ 9903
//2014.10.24			tcpip_send_c4_status_gtech_20140808("116.123.106.53", MONITORING_SERVER_PORT+giGtechServerCurrentPortIndex); //9901 ~ +MAX_GTECH_SERVER_PORT_QTY //2014.10.8

			if(gwGtechMonitoringServerPort) tcpip_send_c4_status_gtech_20140808(gszGtechMonitoringServerIp, gwGtechMonitoringServerPort+giGtechServerCurrentPortIndex); //9901 ~ +MAX_GTECH_SERVER_PORT_QTY //2014.10.24
			else iSendStage = 300;

			if(iSendStage == 300)
			{	
				++giSendingType;
				iSendStage = 0;
				giSendingStageRetry = 0;	//2015.8.20
//2015.4.24				
goto case_4;
			}
			break;

		case 4: //지테크 모니터링 서버 이벤트 정보
case_4: //지테크 모니터링 서버 이벤트 정보
//USART1_puts("case 4 : gbGetCardUpdate");
//2014.10.24			tcpip_send_event_gtech_20141017("116.123.106.53", MONITORING_SERVER_PORT+giGtechServerCurrentPortIndex); //9901 ~ +MAX_GTECH_SERVER_PORT_QTY //2014.10.8

			if(gwGtechMonitoringServerPort) tcpip_send_event_gtech_20141017(gszGtechMonitoringServerIp, gwGtechMonitoringServerPort+giGtechServerCurrentPortIndex); //9901 ~ +MAX_GTECH_SERVER_PORT_QTY //2014.10.24
			else iSendStage = 300;

			if(iSendStage == 300)
			{	
				++giSendingType;
				iSendStage = 0;
				giSendingStageRetry = 0;	//2015.8.20
//2015.4.24				
goto case_5;
			}
			break;

		case 5: //지테크 모니터링 서버 배출 정보
case_5: //지테크 모니터링 서버 배출 정보
//USART1_puts("case 5 : gbGetCardUpdate");

//			tcpip_send_c1_data_gtech_20141020("116.123.106.53", MONITORING_SERVER_PORT+giGtechServerCurrentPortIndex); //9901 ~ +MAX_GTECH_SERVER_PORT_QTY //2014.10.20
//2014.10.30	tcpip_send_c1_data_gtech_20141020(gszGtechMonitoringServerIp, MONITORING_SERVER_PORT+giGtechServerCurrentPortIndex); //9901 ~ +MAX_GTECH_SERVER_PORT_QTY //2014.10.20

			if(gwGtechMonitoringServerPort) tcpip_send_c1_data_gtech_20141020(gszGtechMonitoringServerIp, gwGtechMonitoringServerPort+giGtechServerCurrentPortIndex); //9901 ~ +MAX_GTECH_SERVER_PORT_QTY //2014.10.20
			else iSendStage = 300;

			if(iSendStage == 300)
			{	
				++giSendingType;
				iSendStage = 0;
				giSendingStageRetry = 0;	//2015.8.20
//2015.4.24	
goto case_6;
			}
			break;
		case 6:
case_6:
			++giSendingType;
		case 7:
case_7:
			++giSendingType;


		case 8:	//기타 cdma 이용 관련 기능 처리
case_8:	//기타 cdma 이용 관련 기능 처리

//USART1_puts("case 8 : ");

			if(gbGetCardInfo) //공단으로 부터 카드정보 받아오기 
			{
				gbGetCardInfo = 0;
				Send_to_Gtech_Event("CARD_GET_FULL", "TRY"); //발생한 event를 전송한다.
				Get_card_Info();
			}
			if(gbGetCardUpdate)
			{
//USART1_puts("case 8 : gbGetCardUpdate");
				gbGetCardUpdate = 0;
				Send_to_Gtech_Event("CARD_UPDATE", "TRY"); //발생한 event를 전송한다.
   				Get_card_update_Info();
			}

//			tcpip_send_baichul_data_20140821("116.123.106.53", 9960);
//			if(iSendStage == 300)
			{	
				++giSendingType;
				iSendStage = 0;
				giSendingStageRetry = 0;	//2015.8.20
//2015.4.24				
goto case_9;
			}
			break;

		case 9:
case_9:
			modem_reset();

			if(iSendStage == 300)
			{	
				++giSendingType;
				iSendStage = 0;
				giSendingStageRetry = 0;	//2015.8.20
//2015.4.24				
goto case_10;
			}
			break;

		case 10:	//test rouinte only
case_10:	//test rouinte only
//USART1_puts("case 10 : Nor_data_Writhing");
///			Nor_data_Writhing(Nor_Flash_Counter,0);

//			if(iSendStage == 300)
			{	
				++giSendingType;
				iSendStage = 0;
				giSendingStageRetry = 0;	//2015.8.20
//2015.4.24				
goto case_11;
			}
			break;


		case 11:
case_11:	//cip rouinte
			tcpip_send_CIP_20140808(gszCipServerIp, gwCipServerPort);
			if(iSendStage == 300)
			{	
				++giSendingType;
				iSendStage = 0;
				giSendingStageRetry = 0;	//2015.8.20
goto case_default;
			}
			break;

		default:
case_default:
			giSendingType = 0; //2014.8.8
			iSendStage = 0;
			break;
	}
}


int tcpip_send_c1_data_20140806(char *ip, int port, int force)  ///salsa99
{
	int  iz, it_offset;
	long l, lTmp_Nor_Flash_Counter;	//because address is 32bit.
	static u32	dwSendingIndex;	
	static int	iDataLength;
	char server_ip[25];
	int	ret_val = 0;


	char 	szT[20];


	//암호화 
	Byte p[16], c[16], dc[16];
	Byte rk[16*17], mk[32];
	const Byte aria_key[5][32] =
	{	
	// test server
	{ 0xAB, 0x3C, 0x68, 0xC4, 0xB6, 0x09, 0x49, 0x14, 0x0D, 0x5D, 0x39, 0x93, 0x7C, 0x95, 0xC1, 0x22,
	  0x77, 0xA4, 0x9F, 0x64, 0xBB, 0x98, 0x62, 0xEE, 0x18, 0xB9, 0x96, 0x17, 0x1A, 0x54, 0x64, 0xDB },
	// 서귀포 선불, 제주시 선불, 의정부시 선불, 포항 선불
	// 부산강서 후불
	{ 0xF0, 0xB9, 0xC8, 0x47, 0x62, 0x21, 0xA3, 0x26, 0xC8, 0x84, 0xDE, 0xC8, 0xB9, 0xFE, 0xB0, 0x6F,
	  0x6F, 0x7E, 0xCD, 0x43, 0x65, 0x3A, 0x30, 0x8A, 0x03, 0x6C, 0x00, 0x4B, 0x70, 0x67, 0x0D, 0x71 },
	// 의정부시 후불, 의왕시 후불 aria key
	{ 0xC8, 0x43, 0x9A, 0x52, 0x8F, 0x84, 0x58, 0x60, 0x73, 0x0F, 0x68, 0x1F, 0x93, 0xD5, 0x7B, 0x47,
	  0x48, 0x3A, 0x47, 0xBD, 0x61, 0x3F, 0x28, 0xF3, 0xD6, 0xA7, 0x83, 0xB5, 0x2C, 0x87, 0xAA, 0x77 },

	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 },
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 }
	};



//	char szT[30];	//test only 2015.4.27


	///////////////////////////////
//	USART1_putchar(iSendStage+'0');
//	USART1_putchar(Nor_Flash_Counter+'A');

	++giSendingStageRetry;	//2015.8.20
								
	switch(iSendStage)
	{
		case 0:
			giSendingStageRetry = 0;	//2015.8.20
			Fsmc_Enable();

//USART1_puts("c1_case 0 start ");
			//2015.4.27 test only
			if(7000 < Nor_Flash_Counter) lTmp_Nor_Flash_Counter = 7000; //add 2015.4.27
			else lTmp_Nor_Flash_Counter	= Nor_Flash_Counter;

//2015.4.27	for(l=0; l<Nor_Flash_Counter; l++) // Nor_Flash_Counter
////2015.9.30			for(l=0; l<lTmp_Nor_Flash_Counter; l++) // Nor_Flash_Counter
			for(l=0; l<7000; l++) // Nor_Flash_Counter
			{
//				USART1_putchar(l+'a');

				Nor_data_reading(l); // nor flash 
        		if(baichul_info_nor[1].Status_keco_new==7777) // 새로운 데이타가  들어왓냐  
				{

//sprintf(szT, "b:%d,F:%d,S:%d.", gbKecoForceSend, force,baichul_info_nor[1].Status_keco_send);
//USART1_puts(szT);

					if(baichul_info_nor[1].Status_keco_send==0xffff)// 데이터를 공단으로 보내지 않았으면 
					{
						dwSendingIndex = l;	 // nor 에서 몇번째 데이타냐 ?
						break;
					} //if(baichul_info_nor[1].Status_keco_send==0xffff)// 데이터를 공단으로 보내지 않았으면 	
/*
					else if(force == 6099)	//수동전송
					{
						if(baichul_info_nor[1].Status_keco_send==6099)// 데이터를 공단으로 보내지 않았으면 
						{
							dwSendingIndex = l;	 // nor 에서 몇번째 데이타냐 ?
							break;
						} //if(baichul_info_nor[1].Status_keco_send==0xffff)// 데이터를 공단으로 보내지 않았으면 	
					}
*/
				} //if(baichul_info_nor[1].Status_keco_new==7777) // 새로운 데이타가  들어왓냐  
			} //for(i=0;i<Nor_Flash_Counter;i++) // Nor_Flash_Counter
			Fsmc_Disable();

			if(l < Nor_Flash_Counter) iSendStage = 10;// 보내야할 테이타가 있다. 
			else
			{	iSendStage = 300;	//보낼 데이터 없을 때 딜레이 없이 바로 복귀 2014.8.21  change 100 to 300;	//end
				ret_val = 300;	//다 보냈다.
			}

			gwBaichulCount = Nor_Flash_Counter;	//for monitoring information

			//<< 2015.12.9  배출한 횟수가 7000을 넘었을 때  미전송건수가  없을시 미전송값  틀린것 때문에 
			if(l < 7000) gwSendCount = l;	////for monitoring information
			else gwSendCount = gwBaichulCount;
			// 2015.12.9 >>
			
			//<< 2015.12.9  배출한 횟수가 7000을 넘었을 때  미전송건수가  없을시 미전송값  틀린것 때문에 
			//add 2014.10.20
			// 2015.12.9 if(gwBaichulCount < gwSendCount) gwBaichulCount += 7000;
			// 2015.12.9 >>

//			if(giKecoSendingC1BaichulRetry) --giKecoSendingC1BaichulRetry;
//			if(!giKecoSendingC1BaichulRetry) giKecoSendingC1BaichulFlag = 0;

/**/
//iSendStage = 10;
//Nor_Flash_Counter = 1;
//gwBaichulCount = 1;
//sprintf(szT, "%d,%d,%d,%d", l, iSendStage, Nor_Flash_Counter, gwBaichulCount);	
//USART1_puts(szT);
/*
USART1_puts("c1_case 0 end");
strcpy(baichul_info_nor[1].card1_uid, "FFA89E78");
baichul_info_nor[1].weight = 0;
strcpy(baichul_info_nor[1].date, "20150818000000");
***/

			break;

		case 10:

//USART1_puts("c1_case 10 start");

//			USART1_putchar(iSendStage+'s');
			get_jangbi_code();
			iz = Stage_One_DataLoading_New(gszJangbiCode, baichul_info_nor[1].card1_uid, (unsigned int)(baichul_info_nor[1].weight),baichul_info_nor[1].date);
			if(iz <= 0)	//data error, 장비코드 error
			{
//				USART1_putchar(10+iz+'0');
				iSendStage = 100;

		   		Fsmc_Enable();
				Delay_ms(1);
		   		FSMC_NOR_WriteHalfWord(NOR_BAICHUL_BASE+(dwSendingIndex*64)+44,5555); //장비코드 에러
		   		Fsmc_Disable();
			}
			else
			{
				iStabeTwoWaitCount = 0;
				iSendStage = 20;
			}
			break;

		case 20:
			iSendStage = tcpip_send_case_20(ip, port, iStabeTwoWaitCount);
			break;
/**
		case 30:
       		Stage_Three_Keco_Connection_send_cmd();	// tcp 접속  
			iSendStage = 31;
			break;

		case 31:
       		switch(Stage_Three_Keco_Connection_wait_response(0, STAGE_THREE_RETRY))	//initialize waiting tcp 접속
			{
			 	case -1: //time out
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					Cdma_Force_Stop(); // 강제로 전화를 끊는다. 
					break;
			 	case -2: //or no carrier
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					break;
				case 1:
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 40;	//connection ok
					break;
				default:
					iSendStage = 32;	//dummy stage - waiting stage
					break;
			}
			break;

		case 32:
       		switch(Stage_Three_Keco_Connection_wait_response(1, STAGE_THREE_RETRY))	//initialize waiting tcp 접속
			{
			 	case -1: //time out
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					Cdma_Force_Stop(); // 강제로 전화를 끊는다. 
					break;
			 	case -2: //or no carrier
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					break;
				case 1:
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 40;	//connection ok
					break;
				default:
					break;
			}
			break;
*/

		case 40:
			//monitoring
//			for(iz=0; iz<giSendingDataLength; iz++) USART1_putchar(gszSendingData[iz]);
			Stage_Four_Data_Sending_send_msg(gszSendingData, giSendingDataLength);
//			iSendStage = 41;	//wait response
			iStage40Count = 0;	//2015.9,22 41,42 통합
			iSendStage = 42;	//wait response 2015.9.22
			break;
		case 42:
//       		switch(Stage_Four_Data_Sending_wait_response(1, STAGE4_WAIT_COUNTER))	//waiting server response
       		switch(Stage_Four_Data_Sending_wait_response(iStage40Count++, STAGE4_WAIT_COUNTER))	//waiting server response
			{
			 	case -1: //time out
					iSendStage = 100;
					iStage100Count = 0;

//					USART1_puts("time out");
#ifdef ___LGUPLUS_AM200____
					Keco_card_info_flag = 0; 
					break;
#endif
				case 100:
				case 1: //send ok
//					Keco_card_info_flag = 0;
					iSendStage = 100;
					iStage100Count = 0;
/*
USART1_puts("::");
USART1_puts(gszCdmaReturn);
USART1_puts("::");
*/
					giKecoSendingC1BaichulFlag = 0;
					giKecoSendingC1BaichulRetry = 0;

					//keco response de
					//아리아키 선택
					for(iz=0; iz<32; iz++) mk[iz] = aria_key[giAriaKeyIndex][iz];
	
					DecKeySetup(mk, rk, 256);

					//for(iz=0; iz<16; iz++) c[iz] = RxCardBuffer2[iz+32];
					it_offset = 0;
					for(iz=0; iz<20-3; iz++)
					{
						if(gszCdmaReturn[iz] == 'P' && gszCdmaReturn[iz+1] == 'R' && gszCdmaReturn[iz+2] == 'D' && gszCdmaReturn[iz+3] == ':') 
						{
							it_offset = iz + 7;	//TCPRD:99,
							break;
						}
					}

					for(iz=0; iz<16; iz++) c[iz] = gszCdmaReturn[iz+32 +it_offset];
					Crypt(c, 16, rk, dc);	//c:source, dc:decy
USART1_putchar(0x0d);
USART1_putchar(0x0a);
					for(iz=0; iz<16; iz++) USART1_putchar(dc[iz]);
					/**
					for(iz=0; iz<16; iz++) c[iz] = RxCardBuffer2[iz+32+16];
					Crypt(c, 16, rk, dc);
					for(iz=0; iz<16; iz++) USART1_putchar(dc[iz]);
					**/
//					for(iz=0; iz<16; iz++) USART1_putchar(dc[iz]);
USART1_putchar(0x0d);
USART1_putchar(0x0a);


					if(check_response(dc, "R10020", 16, 6)) //send ok
					{
				   		Fsmc_Enable();
						if(baichul_info_nor[1].Status_keco_send==0xffff)	//2015.9.22
				   			FSMC_NOR_WriteHalfWord(NOR_BAICHUL_BASE+(dwSendingIndex*64)+44,9999); // 정상적으로 보냈으면 9999
						else if(baichul_info_nor[1].Status_keco_send==6099)	//기타에러
				   			FSMC_NOR_WriteHalfWord(NOR_BAICHUL_BASE+(dwSendingIndex*64)+44,5888); // 정상적으로 보냈으면 6099에서 전송했으면
				   		Fsmc_Disable();
					}
					else if(check_response(dc, "R10120", 16, 6)) //배출원에 등록이 안된 카드
					{
				   		Fsmc_Enable();
				   		FSMC_NOR_WriteHalfWord(NOR_BAICHUL_BASE+(dwSendingIndex*64)+44,6001);
				   		Fsmc_Disable();
					}
					else if(check_response(dc, "R10220", 16, 6)) //미등록 카드 send ok
					{
				   		Fsmc_Enable();
				   		FSMC_NOR_WriteHalfWord(NOR_BAICHUL_BASE+(dwSendingIndex*64)+44,6002); //미등록 카드 (카드 정보 부재)
				   		Fsmc_Disable();
					}
					else
					{
				   		Fsmc_Enable();
				   		FSMC_NOR_WriteHalfWord(NOR_BAICHUL_BASE+(dwSendingIndex*64)+44,6099); //기타 에러
				   		Fsmc_Disable();
					}
					break;

				default:
					break;
			}
			break;

		case 100:
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
			iSendStage = 300;	//end
			break;
	} //switch(iSendStage)

	return ret_val;
}					  


void tcpip_send_c4_status_20140808(char *ip, int port)
{

 //giSendingStatusGtechLength 
	int  iz;
	char server_ip[25];

//	USART1_putchar(iSendStage+'0');
//	USART1_putchar(Nor_Flash_Counter+'A');

	++giSendingStageRetry;	//2015.8.20
						
	switch(iSendStage)
	{
		case 0:
			giSendingStageRetry = 0;	//2015.8.20

			if(giKecoSendingStatusFlag)
			{
				iStabeTwoWaitCount = 0;
				iSendStage = 20;	
			}
			else iSendStage = 300;	//보낼 데이터 없을 때 딜레이 없이 바로 복귀 2014.8.21  change 100 to 300;	//end

			if(giKecoSendingStatusRetry) --giKecoSendingStatusRetry;
			if(!giKecoSendingStatusRetry) giKecoSendingStatusFlag = 0;
			break;

		case 20:
			iSendStage = tcpip_send_case_20(ip, port, iStabeTwoWaitCount);
			break;

#ifdef ___LGUPLUS_AM200____
		case 30:
       		Stage_Three_Keco_Connection_send_cmd();	// tcp 접속  
			iSendStage = 31;
			break;

		case 31:
       		switch(Stage_Three_Keco_Connection_wait_response(0, STAGE_THREE_RETRY))	//initialize waiting tcp 접속
			{
			 	case -1: //time out
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					Cdma_Force_Stop(); // 강제로 전화를 끊는다. 
					break;
			 	case -2: //or no carrier
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					break;
				case 1:
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 40;	//connection ok
					break;
				default:
					iSendStage = 32;	//dummy stage - waiting stage
					break;
			}
			break;

		case 32:
       		switch(Stage_Three_Keco_Connection_wait_response(1, STAGE_THREE_RETRY))	//initialize waiting tcp 접속
			{
			 	case -1: //time out
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					Cdma_Force_Stop(); // 강제로 전화를 끊는다. 
					break;
			 	case -2: //or no carrier
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					break;
				case 1:
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 40;	//connection ok
					break;
				default:
					break;
			}
			break;
#endif	//#ifdef ___LGUPLUS_AM200____

		case 40:
			//monitoring
//			for(iz=0; iz<giSendingDataLength; iz++) USART1_putchar(gszSendingData[iz]);
//			Stage_Four_Data_Sending_send_msg(gszSendingData, giSendingDataLength);
			Stage_Four_Data_Sending_send_msg(gszSendingStatus, giSendingStatusLength);

			iStage40Count = 0;	//2015.9,22 41,42 통합
//			iSendStage = 41;	//wait response
			iSendStage = 42;	//wait response
///2015.8/20	break;
		case 42:
//       		switch(Stage_Four_Data_Sending_wait_response(1, STAGE4_WAIT_COUNTER))	//waiting server response
       		switch(Stage_Four_Data_Sending_wait_response(iStage40Count++, STAGE4_WAIT_COUNTER))	//waiting server response
			{
			 	case -1: //time out
					iSendStage = 100;
					iStage100Count = 0;
//					USART1_puts("time out");
#ifdef ___LGUPLUS_AM200____
					Keco_card_info_flag = 0; 
					break;
#endif
				case 1: //send ok
					Keco_card_info_flag = 0;
					iSendStage = 100;
					iStage100Count = 0;
/*
USART1_puts("2-R400 BEFORE");
USART1_puts(giCdmaReturn);
*/
					//keco response de
					for(iz=0; iz<16; iz++) USART1_putchar(gszCdmaReturn[iz]);
					if(check_response(RxCardBuffer2, "34R400", RxCardCounter2, 6)) //send ok
///					if(check_response(gszCdmaReturn, "34R400", giCdmaReturn, 6)) //send ok
//					for(iz=0; iz<16; iz++) USART1_putchar(RxCardBuffer2[iz+32]);
//					if(check_response(RxCardBuffer2, "34R400", RxCardCounter2, 6)) //send ok
//					if(check_response(RxCardBuffer2, "R40120", RxCardCounter2, 7)) //send ok test
					{
						giSendingStatusLength = 0;	
						giKecoSendingStatusRetry = 0;
						giKecoSendingStatusFlag = 0;
/*
USART1_puts("2-R400 OK");
USART1_puts(giCdmaReturn);
*/
					}
					break;

				case 4002:
//					Keco_card_info_flag = 0;
					iSendStage = 100;
					iStage100Count = 0;

					giSendingStatusLength = 0;	
					giKecoSendingStatusRetry = 0;
					giKecoSendingStatusFlag = 0;
					break;

				default:
					break;
			}
			break;

		case 100:
       		switch(Stage_Idle(iStage100Count++, SEND_INTERVAL))	//waiting send idle
			{
				case 1:
					iStage100Count = 0;
					iSendStage = 300;	//end
					break;
				default:
					break;
			}
			break;

		default:
			Keco_card_info_flag = 0;
			iStage100Count = 0;
			iSendStage = 300;
			break;
	} //switch(iSendStage)
}		
			  
void tcpip_send_c1_data_gtech_20141020(char *ip, int port)
{
	int  iz;
	long laddr;	//because address is 32bit.
	static u32	dwSendingIndex;	
	static int	iDataLength;
	char server_ip[25];

	++giSendingStageRetry;	//2015.8.20

	switch(iSendStage)
	{
		case 0:
			giSendingStageRetry = 0;	//2015.8.20
			Fsmc_Enable();
//USART1_puts("case 0: tcpip_send_c1_data_gtech_20141020");
			for(laddr=0; laddr<7000; laddr++) // Nor_Flash_Counter
			{
//				USART1_putchar(l+'a');

				Nor_data_reading(laddr); // nor flash 
        		if(baichul_info_nor[1].Status_keco_new == 7777) // 새로운 데이타가  들어왓냐  
				{
					if(baichul_info_nor[1].Status_gtech == 0x7777)// 데이터를 모니터링으로 보내지 않았으면 
					{
						dwSendingIndex = laddr;	 // nor 에서 몇번째 데이타냐 ?
						break;
					}
				} //if(baichul_info_nor[1].Status_keco_new==7777) // 새로운 데이타가  들어왓냐  
			} //for(i=0;i<Nor_Flash_Counter;i++) // Nor_Flash_Counter
			Fsmc_Disable();

			if(laddr < 7000)
			{	iSendStage = 10;// 보내야 할 테이타가 있다. 
				gwSendGtechCount = laddr;	////for monitoring information
			}
			else iSendStage = 300;	//보낼 데이터 없을 때 딜레이 없이 바로 복귀 2014.8.21  change 100 to 300;	//end

			gwBaichulCount = Nor_Flash_Counter;	//for monitoring information

			//add 2014.10.20
			if(gwBaichulCount < gwSendGtechCount) gwBaichulCount += 7000;

//			if(giKecoSendingC1BaichulRetry) --giKecoSendingC1BaichulRetry;
//			if(!giKecoSendingC1BaichulRetry) giKecoSendingC1BaichulFlag = 0;

			break;

		case 10:
//USART1_puts("case 10: tcpip_send_c1_data_gtech_20141020");
//			USART1_putchar(iSendStage+'s');
			get_jangbi_code();
			iz = Stage_One_DataLoading_New_Gtech(gszJangbiCode, baichul_info_nor[1].card1_uid, (unsigned int)(baichul_info_nor[1].weight),baichul_info_nor[1].date);
			if(iz <= 0)	//data error, 장비코드 error
			{
//				USART1_putchar(10+iz+'0');
				iSendStage = 100;

		   		Fsmc_Enable();
				Delay_ms(1);
		   		FSMC_NOR_WriteHalfWord(NOR_BAICHUL_BASE+(dwSendingIndex*64)+46,0x5555); //장비코드 에러
		   		Fsmc_Disable();
			}
			else
			{
				iStabeTwoWaitCount = 0;
				iSendStage = 20;
			}
			break;

		case 20:
			iSendStage = tcpip_send_case_20(ip, port, iStabeTwoWaitCount);
			break;
    	     
#ifdef ___LGUPLUS_AM200____
		case 30:
//USART1_puts("case 30: tcpip_send_c1_data_gtech_20141020");
       		Stage_Three_Keco_Connection_send_cmd();	// tcp 접속  
			iSendStage = 31;
			break;

		case 31:
       		switch(Stage_Three_Keco_Connection_wait_response(0, STAGE_THREE_RETRY))	//initialize waiting tcp 접속
			{
			 	case -1: //time out
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					Cdma_Force_Stop(); // 강제로 전화를 끊는다. 
					break;
			 	case -2: //or no carrier
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					break;
				case 1:
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 40;	//connection ok
					break;
				default:
					iSendStage = 32;	//dummy stage - waiting stage
					break;
			}
			break;

		case 32:
       		switch(Stage_Three_Keco_Connection_wait_response(1, STAGE_THREE_RETRY))	//initialize waiting tcp 접속
			{
			 	case -1: //time out
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					Cdma_Force_Stop(); // 강제로 전화를 끊는다. 
					break;
			 	case -2: //or no carrier
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					break;
				case 1:
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 40;	//connection ok
					break;
				default:
					break;
			}
			break;
#endif	//#ifdef ___LGUPLUS_AM200____

		case 40:
			//monitoring
//			for(iz=0; iz<giSendingDataLength; iz++) USART1_putchar(gszSendingData[iz]);
			Stage_Four_Data_Sending_send_msg(gszSendingData, giSendingDataLength);
//			iSendStage = 41;	//wait response
			iStage40Count = 0;	//2015.9,22 41,42 통합
			iSendStage = 42;	//wait response
			break;

		case 42:
       		switch(Stage_Four_Data_Sending_wait_response(iStage40Count++, STAGE4_WAIT_COUNTER))	//waiting server response
			{
			 	case -1: //time out
					iSendStage = 100;
					iStage100Count = 0;

//					USART1_puts("time out");
#ifdef ___LGUPLUS_AM200____
					Keco_card_info_flag = 0; 
					break;
#endif
				case 1: //send ok
					Keco_card_info_flag = 0;
					iSendStage = 100;
					iStage100Count = 0;

					giKecoSendingC1BaichulFlag = 0;
					giKecoSendingC1BaichulRetry = 0;

					//keco response de
					if(check_response(RxCardBuffer2, "R40020", RxCardCounter2, 7)) //send ok
					{
				   		Fsmc_Enable();
				   		FSMC_NOR_WriteHalfWord(NOR_BAICHUL_BASE+(dwSendingIndex*64)+46,0x3333); // 정상적으로 보냈으면 9999
				   		Fsmc_Disable();
					}
					else
					{
				   		Fsmc_Enable();
				   		FSMC_NOR_WriteHalfWord(NOR_BAICHUL_BASE+(dwSendingIndex*64)+46,0x6066); //기타 에러
				   		Fsmc_Disable();
					}
					break;

				case 400:
//					Keco_card_info_flag = 0;
					iSendStage = 100;
					iStage100Count = 0;

					giKecoSendingC1BaichulFlag = 0;
					giKecoSendingC1BaichulRetry = 0;

			   		Fsmc_Enable();
			   		FSMC_NOR_WriteHalfWord(NOR_BAICHUL_BASE+(dwSendingIndex*64)+46,0x3333); // 정상적으로 보냈으면 9999
			   		Fsmc_Disable();
					break;

				default:
					break;
			}
			break;

		case 100:
			Cdma_Force_Stop(); // 강제로 전화를 끊는다. 
			Keco_card_info_flag = 0;

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
			iSendStage = 300;	//end
			break;
	} //switch(iSendStage)
}					  


void tcpip_send_c4_status_gtech_20140808(char *ip, int port)
{
	int  iz;
	char server_ip[25];

	char s[300];

	++giSendingStageRetry;	//2015.8.20
	switch(iSendStage)
	{
		case 0:
			giSendingStageRetry = 0;	//2015.8.20
//USART1_puts("tcpip_send_c4_status_gtech_20140808  case 0");

			if(!giGtechSendingStatusFlag) iSendStage = 300;	//보낼 데이터 없을 때 딜레이 없이 바로 복귀 2014.8.21  change 100 to 300;	//end
			else
			{
/**
				//2014.10.8 1.32r1
				if(MAX_giGtechSendingStatusRetry <= giGtechSendingStatusRetry)	//data initialize
				{
					//make sending data
					gtech_monitoring_data_make();
				}
				else
				{
					// next port calc
					++giGtechServerCurrentPortIndex;	//2014.10.8
					if(MAX_GTECH_SERVER_PORT_QTY <= giGtechServerCurrentPortIndex) giGtechServerCurrentPortIndex = 0;
				}

				iSendStage = 20;	
				if(giGtechSendingStatusRetry) --giGtechSendingStatusRetry;
				if(!giGtechSendingStatusRetry) giGtechSendingStatusFlag = 0;
**/
				//2015.4.24 1.33a1
				if(giGtechSendingStatusRetry < MAX_giGtechSendingStatusRetry)
				{
					// next port calc
					++giGtechServerCurrentPortIndex;	//2014.10.8
					if(MAX_GTECH_SERVER_PORT_QTY <= giGtechServerCurrentPortIndex) giGtechServerCurrentPortIndex = 0;
				}

				//make sending data
				gtech_monitoring_data_make();

				iStabeTwoWaitCount = 0;
				iSendStage = 20;	

				if(giGtechSendingStatusRetry) --giGtechSendingStatusRetry;
				if(!giGtechSendingStatusRetry) giGtechSendingStatusFlag = 0;
			}
			break;

		case 20:
			iSendStage = tcpip_send_case_20(ip, port, iStabeTwoWaitCount);
			break;
    	     
#ifdef ___LGUPLUS_AM200____
		case 30:

//USART1_puts("tcpip_send_c4_status_gtech_20140808  case 30");

       		Stage_Three_Keco_Connection_send_cmd();	// tcp 접속  
			iSendStage = 31;
			break;

		case 31:
       		switch(Stage_Three_Keco_Connection_wait_response(0, STAGE_THREE_RETRY))	//initialize waiting tcp 접속
			{
			 	case -1: //time out
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					Cdma_Force_Stop(); // 강제로 전화를 끊는다. 
					break;
			 	case -2: //or no carrier
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					break;
				case 1:
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 40;	//connection ok
					break;
				default:
					iSendStage = 32;	//dummy stage - waiting stage
					break;
			}
			break;

		case 32:
       		switch(Stage_Three_Keco_Connection_wait_response(1, STAGE_THREE_RETRY))	//initialize waiting tcp 접속
			{
			 	case -1: //time out
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					Cdma_Force_Stop(); // 강제로 전화를 끊는다. 
					break;
			 	case -2: //or no carrier
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					break;
				case 1:
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 40;	//connection ok
					break;
				default:
				//	USART1_putchar('.');
					break;
			}
			break;
#endif	//#ifdef ___LGUPLUS_AM200____

		case 40:

//USART1_puts("tcpip_send_c4_status_gtech_20140808  case 40");

			//monitoring
//			for(iz=0; iz<giSendingDataLength; iz++) USART1_putchar(gszSendingData[iz]);
//			Stage_Four_Data_Sending_send_msg(gszSendingData, giSendingDataLength);
//2015.4.24	Stage_Four_Data_Sending_send_msg(gszSendingStatusGtech, giSendingStatusGtechLength);
			Stage_Four_Data_Sending_send_msg(gszSending, giSendingLength);
//			iSendStage = 41;	//wait response
			iStage40Count = 0;	//2015.9,22 41,42 통합
			iSendStage = 42;	//wait response
			break;
		case 42:
       		switch(Stage_Four_Data_Sending_wait_response(iStage40Count++, 20))	//waiting server response
			{
			 	case -1: //time out
					iSendStage = 100;
					iStage100Count = 0;
//					USART1_puts("time out");
#ifdef ___LGUPLUS_AM200____
					Keco_card_info_flag = 0; 
					break;
#endif
				case 1: //send ok
					Keco_card_info_flag = 0;

					iSendStage = 100;
					iStage100Count = 0;

//					//keco response de
					for(iz=0; iz<16; iz++) USART1_putchar(RxCardBuffer2[iz]);
//					if(check_response(RxCardBuffer2, "R40020", RxCardCounter2, 7)) //send ok
//					if(check_response(RxCardBuffer2, "R400", RxCardCounter2, 4)) //send ok test
					{
						giSendingLength = 0;	
						giGtechSendingStatusRetry = MAX_giGtechSendingStatusRetry;	//2014.8.25
						giGtechSendingStatusFlag = 0;	//2014.8.25
					}
					break;

				case 400:
					iSendStage = 100;
					iStage100Count = 0;
					giSendingLength = 0;	
					giGtechSendingStatusRetry = MAX_giGtechSendingStatusRetry;	//2014.8.25
					giGtechSendingStatusFlag = 0;	//2014.8.25
					break;

				default:
					break;
			}
			break;

		case 100:
			Keco_card_info_flag = 0;
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

// void Stage_One_DataLoading(void)		  // 0409_1
//int keco_send_c1(char *jangbi_code, char *card_no, unsigned int uWeight)
int Stage_One_DataLoading_New(char *jangbi_code,  char *card_no, unsigned int uWeight,char *data_time)
{
//	int ret_val = 0;
	int	 i, iz;
	int  iWeight;
	int  iCost;

	char szCost[6+1];
	char szWeight[6+1];
	char szDataLength[10];
	char szUID[16+1];
	char szHeader[35];
	char szData[256];
	char szCData[256];

	char szCRC16Buf[10];
	unsigned short wCRC16;

	int	 jangbi_code_length;
	int	 iDataLength;


	//암호화 
//	int	 iAriaKeyIndex = TEST_ARIA_INDEX;
	Byte p[16], c[16], dc[16];
	Byte rk[16*17], mk[32];

	const Byte aria_key[5][32] =
	{
		// test server
		{ 0xAB, 0x3C, 0x68, 0xC4, 0xB6, 0x09, 0x49, 0x14, 0x0D, 0x5D, 0x39, 0x93, 0x7C, 0x95, 0xC1, 0x22,
		  0x77, 0xA4, 0x9F, 0x64, 0xBB, 0x98, 0x62, 0xEE, 0x18, 0xB9, 0x96, 0x17, 0x1A, 0x54, 0x64, 0xDB },
		// 서귀포 선불, 제주시 선불, 의정부시 선불, 포항 선불
		// 부산강서 후불
		{ 0xF0, 0xB9, 0xC8, 0x47, 0x62, 0x21, 0xA3, 0x26, 0xC8, 0x84, 0xDE, 0xC8, 0xB9, 0xFE, 0xB0, 0x6F,
		  0x6F, 0x7E, 0xCD, 0x43, 0x65, 0x3A, 0x30, 0x8A, 0x03, 0x6C, 0x00, 0x4B, 0x70, 0x67, 0x0D, 0x71 },
		// 의정부시 후불, 의왕시 후불 aria key
		{ 0xC8, 0x43, 0x9A, 0x52, 0x8F, 0x84, 0x58, 0x60, 0x73, 0x0F, 0x68, 0x1F, 0x93, 0xD5, 0x7B, 0x47,
		  0x48, 0x3A, 0x47, 0xBD, 0x61, 0x3F, 0x28, 0xF3, 0xD6, 0xA7, 0x83, 0xB5, 0x2C, 0x87, 0xAA, 0x77 },

		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 },
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 }
	};


	//////////// test only
	//////////// 2014.8.7
//	strcpy(szData, "WZZ-007-02-1");
/*
	strcpy(szData, "WZZ-33A-01-1");
	for(iz=0; iz<12; iz++) jangbi_code[iz] = szData[iz];
	strcpy(szData, "6583B621");
	for(iz=0; iz<12; iz++) baichul_info_nor[1].card1_uid[iz] = szData[iz];
  */

	//장비코드 정합성 검사
	iz = GetJangbiAreaCode();
	if(iz < 0) return iz;
	/***
	//구분자 검사																	 
	if((jangbi_code[3] != '-') || (jangbi_code[7] != '-') || (jangbi_code[10] != '-')) return -1;

	//지역코드 검사
	if((jangbi_code[0] == 'W') && (jangbi_code[1] == 'Z') && (jangbi_code[2] == 'Z')) giAriaKeyIndex = TEST_ARIA_INDEX;
	else if((jangbi_code[0] == 'W') && (jangbi_code[1] == '1') && (jangbi_code[2] == '1')) giAriaKeyIndex = BSGS_ARIA_INDEX;
	else if((jangbi_code[0] == 'W') && (jangbi_code[1] == '2') && (jangbi_code[2] == 'A')) giAriaKeyIndex = UJBSI_INDEX;
	else if((jangbi_code[0] == 'W') && (jangbi_code[1] == '2') && (jangbi_code[2] == 'V')) giAriaKeyIndex = UWSI_INDEX;
	else if((jangbi_code[0] == 'W') && (jangbi_code[1] == '6') && (jangbi_code[2] == 'X')) giAriaKeyIndex = SGPOSI_ARIA_INDEX;
	else if((jangbi_code[0] == 'W') && (jangbi_code[1] == '6') && (jangbi_code[2] == 'Y')) giAriaKeyIndex = JEJUSI_ARIA_INDEX;
	else if((jangbi_code[0] == 'W') && (jangbi_code[1] == '0') && (jangbi_code[2] == '8')) giAriaKeyIndex = SEONGBUKGU_ARIA_INDEX;
	else
	{
//		ShowMessage("미등록 장비코드입니다");
		return -2;
    }

	//000검사
//	for(iz=4; iz<=6; iz++) if((jangbi_code[iz] < '0') || ('9' < jangbi_code[iz])) return -3;
	for(iz=4; iz<=6; iz++) 
	{	
		if((jangbi_code[iz] < '0') || ('F' < jangbi_code[iz])) return -3;
	 	if(('9'<jangbi_code[iz])&&(jangbi_code[iz]<'A')) return -3;
	}

	for(iz=8; iz<=9; iz++)
	{
		if((jangbi_code[iz] < '0') || ('F' < jangbi_code[iz])) return -4;
	 	if(('9'<jangbi_code[iz])&&(jangbi_code[iz]<'A')) return -4;
	}

	for(iz=11; iz<=11; iz++)
	{	
		if((jangbi_code[iz] < '0') || ('F' < jangbi_code[iz])) return -5;
	 	if(('9'<jangbi_code[iz])&&(jangbi_code[iz]<'A')) return -5;
	}

	***/

	//데이터 생성	
	//CARD 번호 
	jangbi_code_length = strlen(baichul_info_nor[1].card1_uid);
	for(i=0; i<jangbi_code_length; i++) gszCard_no[i] = baichul_info_nor[1].card1_uid[i];
	gszCard_no[i] = 0;
	gszCard_no[8] = 0;

	//카드번호 정합성 검사	2014.11.10 ver 1.32s3
	iz = CheckCardNo(gszCard_no, 8);
	if(iz < 0) return iz;

	if((jangbi_code[0] == 'W') && (jangbi_code[1] == 'Z') && (jangbi_code[2] == 'Z'))
	{
		szUID[0] = 'K';
		szUID[1] = 'E';
		szUID[2] = 'C';
		szUID[3] = 'O';	
		szUID[4] = 'F';
		szUID[5] = 'W';
		szUID[6] = 'C';
		szUID[7] = '0';
		szUID[8] = '0';
		szUID[9] = '0';
		szUID[10] = '7';
		szUID[11] = '0';
		szUID[12] = '0';
		szUID[13] = '0';

		//KECOFWC000700001
		if((gszCard_no[0]=='8')&&(gszCard_no[1]=='2')&&(gszCard_no[2]=='3')&&(gszCard_no[3]=='1'))
		{
			szUID[14] = '0';
			szUID[15] = '1';
		}
		else if((gszCard_no[0]=='2')&&(gszCard_no[1]=='2')&&(gszCard_no[2]=='3')&&(gszCard_no[3]=='0'))
		{
			szUID[14] = '0';
			szUID[15] = '2';
		}
		else
		{
			szUID[14] = '0';
			szUID[15] = '7';
		}
	}
	else
	{
		jangbi_code_length = GetCardId();
		for(i=0; i<jangbi_code_length; i++) szUID[i] = gszCard_no[i];
//		jangbi_code_length = strlen(baichul_info_nor[1].card1_uid);
//		for(i=0; i<jangbi_code_length; i++) szUID[i] = baichul_info_nor[1].card1_uid[i];
		for( ;i<16; i++) szUID[i] = ' ';
	}
	//배출량 
	iWeight = baichul_info_nor[1].weight;
	sprintf(szWeight, "%6d", iWeight);

	//금액 항상 "0"
	iCost = 0;
	sprintf(szCost, "%06d", iCost);


	//전송할 DATA 초기화
	strcpy(szData, "C100000120130923232527459a9565        123456000000AFCRC1          ");
	iDataLength = strlen(szData);
	//배출일시 yyyymmddhhmmss
	for(i=0; i<14; i++) szData[8+i] = baichul_info_nor[1].date[i];
	//CARD 번호 
	for(i=0; i<16; i++) szData[22+i] = szUID[i];
	//배출량 
	for(i=0; i<6; i++) szData[38+i] = szWeight[i];
	//금액 항상 "0"
	for(i=0; i<6; i++) szData[44+i] = szCost[i];

	//CRC 계산
	wCRC16 = CalcCRC16(szData, 0, (iDataLength-14)/2, szCRC16Buf);
	//CRC - space 10
	szData[iDataLength-14] = szCRC16Buf[2];
	szData[iDataLength-13] = szCRC16Buf[3];
	szData[iDataLength-12] = szCRC16Buf[0];
	szData[iDataLength-11] = szCRC16Buf[1];
	szData[iDataLength] = 0;

	//전송할 데이터 길이 - 헤더에서 사용함.
	sprintf(szDataLength, "%04d", strlen(szData));				   

	//아리아키 선택
	for(i=0; i<32; i++) mk[i] = aria_key[giAriaKeyIndex][i];
 /***/
  
	EncKeySetup(mk, rk, 256);

	for(i=0; i<16; i++) p[i] = szData[i];
	Crypt(p, 16, rk, c);
	for(i=0; i<16; i++) szCData[i] = c[i];

	for(i=16; i<32; i++) p[i-16] = szData[i];
	Crypt(p, 16, rk, c);
	for(i=16; i<32; i++) szCData[i] = c[i-16];

	for(i=32; i<48; i++) p[i-32] = szData[i];
	Crypt(p, 16, rk, c);
	for(i=32; i<48; i++) szCData[i] = c[i-32];

	for(i=48; i<64; i++) p[i-48] = szData[i];
	Crypt(p, 16, rk, c);
	for(i=48; i<64; i++) szCData[i] = c[i-48];


/***/

	//////test
////	for(i=0; i<64; i++) szCData[i] = szData[i];
	/////////////test

	//make header
	//strcpy(szHeader, "12C1W2V-201-01-1    10EN1234CR34");
	strcpy(szHeader, "12C1WZZ-007-02-2    10EN1234CR34");

	jangbi_code_length = strlen(jangbi_code);
	for(i=0; i<jangbi_code_length; i++) szHeader[4+i] = *jangbi_code++;
	for( ;i<16; i++) szHeader[4+i] = ' ';

	szHeader[0x18] = iDataLength % 0x100;
	szHeader[0x19] = (iDataLength / 0x100) % 0x100;
	szHeader[0x1A] = (iDataLength / 0x10000) % 0x100;
	szHeader[0x1B] = (iDataLength / 0x1000000) % 0x100;

//	wCRC16 = CalcCRC16(szHeader, 2, (strlen(szHeader)-6)/2, szCRC16Buf);
	wCRC16 = CalcCRC16(szHeader, 2, (32-6)/2, szCRC16Buf);

	szHeader[0x1C] = wCRC16 % 0x100;
	szHeader[0x1D] = wCRC16 / 0x100;

	//make sending data
	for(iz=0; iz<32; iz++) gszSendingData[iz] = szHeader[iz];
	for(iz=0; iz<iDataLength; iz++) gszSendingData[iz+32] = szCData[iz];
	//for(i=0; i<iDataLength; i++) szHeader[32+i] = szCData[i];
	giSendingDataLength = 32 + iDataLength; //2014.8.7

	//MoNiToRiNg
	for(iz=0; iz<giSendingDataLength; iz++) USART1_putchar(gszSendingData[iz]);

	return iDataLength;
}

int Stage_One_DataLoading_New_Gtech(char *jangbi_code,  char *card_no, unsigned int uWeight,char *data_time)
{
	int	 i, iz;
	int  iWeight;
	int  iCost;

	char szCost[6+1];
	char szWeight[6+1];
	char szDataLength[10];
	char szUID[16+1];
	char szBaichulTime[14+1];

	char szData[256];
	char szCData[256];

	char szCRC16Buf[10];
	unsigned short wCRC16;

	int	 jangbi_code_length;
	int	 iDataLength;

	//장비코드 정합성 검사
	iz = GetJangbiAreaCode();
	if(iz < 0) return iz;

	//데이터 생성	
	//CARD 번호 
	jangbi_code_length = strlen(baichul_info_nor[1].card1_uid);
	for(i=0; i<jangbi_code_length; i++) gszCard_no[i] = baichul_info_nor[1].card1_uid[i];
	gszCard_no[i] = 0;
	gszCard_no[8] = 0;

	//카드번호 정합성 검사	2014.11.10 ver 1.32s3
	iz = CheckCardNo(gszCard_no, 8);
	jangbi_code_length = GetCardId();
	for(i=0; i<jangbi_code_length; i++) szUID[i] = gszCard_no[i];
	if(iz < 0)
	{
		szUID[i++] = 'E';
		szUID[i++] = 'R';
		szUID[i++] = 'R';
	}
	for( ;i<16; i++) szUID[i] = ' ';
	szUID[i] = 0;

	//배출량 
	iWeight = baichul_info_nor[1].weight;
//	sprintf(szWeight, "%d", iWeight);

	//금액 항상
	iCost = iWeight * gxlCostPerOnekilo;
//	sprintf(szCost, "%d", iCost);

	//배출일시 yyyymmddhhmmss
	for(i=0; i<14; i++) szBaichulTime[i] = baichul_info_nor[1].date[i];
	szBaichulTime[i] = 0;

/*
	sprintf( gszSendingStatusGtech,"STATUS|%s|0000|%s|%s|%s|%u%03u|%s| |%u| | | |%d|REAL|%d|%c%c%c%c%c%c%c%c| |%d|%d|%u|%d|%u|%u|%u|%02d%02d%02d%02d%02d%02d|%s|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%04X|%c", \
	 		gszMonitoringProtocolVersion
	 		,gszModel, gszJangbiCode, jangbi.cdma.telno, tick_sec, clock_ms, version

			,SmellMinute

			, gxlCostPerOnekilo, jangbi.cdma.rssi, \
			//리셋, 왼쪽 모터 상태, 카드끼임 상태, 왼쪽수거통 차임상태(부피센서), 왼쪽수거통 있냐 없냐?, 왼쪽 수거통 윗문 상태, 왼쪽 저울상태, 왼쪽 카드리더기 상태 
			ghPowerOnResetStatus, Left_Moter_Status, Left_Card_Status, Left_BasketFull_Status, Left_BasketBeing_Status, Left_BasketDoor_Status, Left_Scale_Status, Left_CardReader_Status, \
			(gwBaichulCount-gwSendCount), gwBaichulCount, After_Weight \
			//현재 ad, 총무게
			,gxlGrossWeight, v_zero, Left_Adc, raw_data	\

			Before_Weight= 

*/	



	//전송할 DATA
//	sprintf(szData, "BAICHUL|%s|0000|", gszMonitoringProtocolVersion);
	sprintf(szData, "BAICHUL|%s1|0000|", gszMonitoringProtocolVersion);
	sprintf(szCData, "%s|%s|%s|%s|%d|%d|%u|REAL|%u|%u|%u|%u|%s|%u|%u|%d|%u|%u|%u|%u|%d|%d|%02d%02d%02d%02d%02d%02d|%04X|%c" \
			,gszModel, gszJangbiCode, jangbi.cdma.telno, szUID, 1, 1 	\
			,gxlCostPerOnekilo	\
								\
			,(gwBaichulCount-gwSendGtechCount), gwBaichulCount \
			,iWeight, iCost	\
			,szBaichulTime \
//			,After_Weight, Before_Weight, gxlGrossWeight, Left_Adc, Left_Adc_After, v_zero, raw_data	
			,After_Weight, Before_Weight_for_monitoring, gxlGrossWeight, Left_Adc, Left_Adc_After, v_zero, raw_data	\
			,Before_Adc_for_monitoring, diff1_for_monitoring \

			,jangbi.tm.year, jangbi.tm.month, jangbi.tm.date, jangbi.tm.hour, jangbi.tm.min, jangbi.tm.sec \
			,wCRC16	  \
			,0x04	\
	);

	//전송할 데이터 길이
	iDataLength = strlen(szData);
	strcat(szData, szCData);
	giSendingDataLength = strlen(szData);
	sprintf(szDataLength, "%04d", giSendingDataLength);
	szData[iDataLength-5] = szDataLength[0];
	szData[iDataLength-4] = szDataLength[1];
	szData[iDataLength-3] = szDataLength[2];
	szData[iDataLength-2] = szDataLength[3];

	//CRC 계산
	wCRC16 = CalcCRC16(szData, 0, (giSendingDataLength-6)/2, szCRC16Buf);
	//CRC - space 10
	szData[giSendingDataLength-6] = szCRC16Buf[2];
	szData[giSendingDataLength-5] = szCRC16Buf[3];
	szData[giSendingDataLength-4] = szCRC16Buf[0];
	szData[giSendingDataLength-3] = szCRC16Buf[1];
	szData[giSendingDataLength] = 0;

	for(iz=0; iz<giSendingDataLength; iz++) gszSendingData[iz] = szData[iz];

	//MoNiToRiNg
	for(iz=0; iz<giSendingDataLength; iz++) USART1_putchar(gszSendingData[iz]);

	return giSendingDataLength;
}


int Stage_Two_IPSetting(char *ip, int port, int init)
{
//	return SocketOpen_LU202F(ip, port, 200);
	return SocketOpen_LU202F(ip, port, init);
}


void Stage_Three_Keco_Connection_send_cmd(void)
{
#ifdef ___LGUPLUS_AM200____ //2015.8.13 
	USART1_puts("stage_three_1");

	RxCardCounter2 = 0;		
	Keco_card_info_flag = 1;	//cdma data 받아오을때 
	memset(RxCardBuffer2, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화

	cdma_send_cmd("ATDT019", 7);
#endif	//#ifdef ___LGUPLUS_AM200____ //2015.8.13 
}


int Stage_Three_Keco_Connection_wait_response(int init, int wait_limit)  // 실제이부분이 응답시간이 많이 걸린다.
{
	int i;
	int ret_val;
	static int Stage_Three_Retry = 0;
	char szResponse[15];
	char str[100];


	if(init == 0)	//					  
	{
		Cdma_Counter=0;
///		Delay_ms(100);
		Stage_Three_Retry=0;
		USART1_puts("stage_three_init");	//2014.8.7
	}

	USART1_puts("stage_three_2");	//2014.8.7
	ret_val = 0;

	//check timeout
//	if(STAGE_THREE_RETRY < ++Stage_Three_Retry)
	if(wait_limit < ++Stage_Three_Retry)	//2014.8.21
	{	
		ret_val = -1;	//fail
	}

#ifdef ___LGUPLUS_AM200____ //2015.8.13 
	sprintf(szResponse, "OPEN%c", 0);
	if(check_response(RxCardBuffer2, szResponse, RxCardCounter2, strlen(szResponse)))
	{	
		ret_val = 1;
		USART1_puts("stage_three_ok");
	}

	// NO CARRIER	// 0429
	sprintf(szResponse, "CARRIER%c", 0);
	if(check_response(RxCardBuffer2, szResponse, RxCardCounter2, strlen(szResponse)))
	{
		ret_val = -2;	//fail
		USART1_puts("stage_three_fail");
	}

#else
	sprintf(szResponse, "0%c", 0);
	if(check_response(RxCardBuffer2, szResponse, RxCardCounter2, strlen(szResponse)))
	{	
		ret_val = 1;
		USART1_puts("stage_three_ok");
	}

	// NO CARRIER	// 0429
	sprintf(szResponse, "4%c", 0);
	if(check_response(RxCardBuffer2, szResponse, RxCardCounter2, strlen(szResponse)))
	{
		ret_val = -2;	//fail
		USART1_puts("stage_three_fail");
	}

#endif	//#ifdef ___LGUPLUS_AM200____ //2015.8.13 

//	sprintf(str, "Stage_Three_Retry = %d, %d, %d", Stage_Three_Retry, STAGE_THREE_RETRY, ret_val);
//	USART1_puts(str);
	return ret_val;
}


void Stage_Four_Data_Sending_send_msg(char *msg, int length)  // 배출데이타를 전송 
{
	int  iz;
	int	 header_length = 0;
	char szMsg[25];

    USART1_puts("Stage_4_send_message!");

#ifdef	___LGUPLUS_LGINNOTEK_PEGASUS____
	sprintf(szMsg, "AT$LGTTCPWR=%d,", length);
	header_length = strlen(szMsg);
#endif

	RxCardCounter2 = 0;		
////	Keco_card_info_flag = 1;	//cdma data 받아오을때 
	memset(RxCardBuffer2, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화

//	for(iz=0; iz<length; iz++) { USART1_putchar(gszSendingData[iz]); }
//	for(iz=0; iz<length; iz++) USART1_putchar(msg[iz]);

	//MONITORING
	for(iz=0; iz<header_length; iz++) USART1_putchar(szMsg[iz]);

	//send to cdma
	for(iz=0; iz<header_length; iz++) cdma_putchar(szMsg[iz]);	//AT$LGTTCPWR=999,
	cdma_send_cmd(msg, length);

#ifdef ___LGUPLUS_LGINNOTEK_PEGASUS____

//		sprintf(szMsg, "OK%c%c%c", 0x0d, 0x0a, 0);
//2015.8.20		sprintf(szMsg, "$LGTTCPWR:0%c%c%c%cOK%c%c%c", 0x0d, 0x0a, 0x0d, 0x0a, 0x0d, 0x0a, 0);
		Delay_ms(10);	//2014.04.02  add because fail

		for(iz=0; iz<100; iz++)
		{
			Delay_ms(2);

			if(check_response(RxCardBuffer2, "$LGTTCPWR:0", RxCardCounter2, 11))
			{	
//2015.8.20		RxCardCounter2 = 0;		
//2015.8.20		memset(RxCardBuffer2, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화
				break;
			}
		}
//2015.8.20		Keco_card_info_flag = 0;	//cdma data 받아오을때 

#endif
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

#ifdef	___LGUPLUS_AM200____
	sprintf(szResponse, "CLOSED%c", 0);
///	Delay_ms(1);
	if(check_response(RxCardBuffer2, szResponse, RxCardCounter2, strlen(szResponse))) // 전송이 정상적으로 끝났다. 
	{	
		ret_val = 1;
       	USART1_puts("GOOD PROCESS!!");
		Cdma_Force_Stop();	//2014.10.16  강제로 전화
		Keco_card_info_flag = 0;	//cdma data 받아오을때 
	}
	else 
#endif	//#ifdef	___LGUPLUS_AM200____

	if(check_response(RxCardBuffer2, "G400", RxCardCounter2, 4)) //gtech monitoring status 전송이 정상적으로 끝났다.  2014.10.16
	{	
		ret_val = 400;
       	USART1_puts("::G400 GOOD::");
		Cdma_Force_Stop();	//2014.10.16  강제로 전화
		Keco_card_info_flag = 0;	//cdma data 받아오을때 
	}
	else if(check_response(RxCardBuffer2, "34R4002", RxCardCounter2, 7)) //gtech monitoring status 전송이 정상적으로 끝났다.  2014.10.16
	{	
		ret_val = 4002;
USART1_puts("::R4002::");
////		giCdmaReturn = RxCardCounter2;
////		if(99 < giCdmaReturn) giCdmaReturn = 99;
////		for(iz=0;iz<99;iz++) gszCdmaReturn[iz] = RxCardBuffer2[iz];

/* 2015.8.20
		RxCardCounter2 = 0;		
		memset(RxCardBuffer2, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화
*/
		Cdma_Force_Stop();	//2014.10.16  강제로 전화
//		Keco_card_info_flag = 0;	//cdma data 받아오을때 
	}
#ifdef	_____new_new_new__
	else if(check_response(RxCardBuffer2, "TCPRD:", RxCardCounter2, 6)) //gtech monitoring status 전송이 정상적으로 끝났다.  2014.10.16
	{	
		ret_val = 1;
       	USART1_puts("TCPRD:");
		giCdmaReturn = RxCardCounter2;
		if(99 < giCdmaReturn) giCdmaReturn = 99;
		for(iz=0;iz<99;iz++) gszCdmaReturn[iz] = RxCardBuffer2[iz];

/* 2015.8.20
		RxCardCounter2 = 0;		
		memset(RxCardBuffer2, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화
*/
		Cdma_Force_Stop();	//2014.10.16  강제로 전화
		Keco_card_info_flag = 0;	//cdma data 받아오을때 
	}
#endif	//___new_new_new__
	else if(check_response(RxCardBuffer2, "$LGTTCPCL:0", RxCardCounter2, 11)) //gtech monitoring status 전송이 정상적으로 끝났다.  2014.10.16
	{	
		ret_val = 1;
       	USART1_puts("::$LGTTCPCL:0::");
		iz0 = check_response(RxCardBuffer2, "$LGTTCPRD:", RxCardCounter2, 10);
		if(iz0)
		{
			ret_val = 100;

			for(iz=0; iz<RxCardCounter2; iz++)
			{
				gszCdmaReturn[iz] = RxCardBuffer2[iz0+iz];
				if(99<iz) break;
			}
		}
		else
			for(iz=0;iz<99;iz++) gszCdmaReturn[iz] = RxCardBuffer2[iz];

		giCdmaReturn = iz;

		Cdma_Force_Stop();	//2014.10.16  강제로 전화
//		Keco_card_info_flag = 0;	//cdma data 받아오을때 
	}
	else if(wait < ++Sending_Retry_Counter) //time out
	{
		ret_val = -1;
		USART1_puts("::BAD PROCESS!!::");
		Cdma_Force_Stop();	//강제로 전화
//		Keco_card_info_flag = 0;	//cdma data 받아오을때 
	}
	else
	{}

	return ret_val;
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


// void Stage_One_DataLoading(void)		  // 0409_1
//int keco_send_c1(char *jangbi_code, char *card_no, unsigned int uWeight)
void Stage_One_DataLoading(char *jangbi_code,  char *card_no, unsigned int uWeight)
{
	int	i, iDataLength;
//	int ret_val = 0;
	char szCRC16Buf[10];
	unsigned short wCRC16;

	int  iWeight;
	int  iCost;
	int jangbi_code_length;

	char szUID[16+1];
	char szWeight[6+1];
	char szCost[6+1];
	char szDataLength[10];
	char szData[256];
	char szCData[256];

  /*
//	TByteDynArray bdaData;
//	String sData;
	int	 iDataLength;
	char szHeader[100];

	char szDateTime[20];
	char sRet[200];
   */
//암호화 
	Byte p[16], c[16];
	Byte rk[16*17], mk[32];

   /*
	jisu.dw=gxlLocalCode_Three;
    sprintf(str, "W2V-201-%c%c%c%c",jisu.c[3],jisu.c[2],jisu.c[1],jisu.c[0]);

  */
///////////////////////////////////////////////////////////////////////////////////
////////////  	protocol 	///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

	//DATA 초기화
	strcpy(szData, "C100000120130923232527459a9565        123456000000AFCRC1          ");
	iDataLength = strlen(szData);

	//현재 날짜, 시간
/*
	String sDateTime;
	DateTimeToString( sDateTime, "yyyymmddhhnnss", Now());
	for(int i=0; i<14; i++) szData[8+i] = sDateTime[i+1];
*/
	//sprintf(szDateTime, "%04d%02d%02d%02d%02d%02d", jangbi.tm.year, jangbi.tm.month, jangbi.tm.date, jangbi.tm.hour, jangbi.tm.min, jangbi.tm.sec);
	
	
//	for(i=0; i<14; i++) szData[8+i] = szDateTime[i+1];
   for(i=0; i<14; i++) szData[8+i] = baichul_info_nor[1].date[i];
//	baichul_info_nor[1].date[0]

//	iWeight = (int)(szData[18]-0x30)*1000 + (int)(szData[19]-0x30)*100 + (int)(szData[20]-0x30)*10 + (int)(szData[21]-0x30);
//	iWeight = (int)(szData[16]-0x30)*1000 + (int)(szData[17]-0x30)*100 + (int)(szData[18]-0x30)*10 + (int)(szData[19]-0x30);
	iWeight = (int)(szData[17]-0x30)*1000 + (int)(szData[18]-0x30)*100 + (int)(szData[19]-0x30)*10 + (int)(szData[20]-0x30);
	iWeight = jangbi.weight;

	//iWeight = uWeight;
	 iWeight = baichul_info_nor[1].weight;
	
	
	
	iCost = iWeight * jangbi.rate;

	//CARD 번호 
//	strcpy(szUID, "459a9565        ");
	//jangbi_code_length = strlen(card_no);

   jangbi_code_length = strlen(baichul_info_nor[1].card1_uid);



	//for(i=0; i<jangbi_code_length; i++) szUID[i] = *card_no++;
	for(i=0; i<jangbi_code_length; i++) szUID[i] = baichul_info_nor[1].card1_uid[i];


	for( ;i<16; i++) szUID[i] = ' ';

	for(i=0; i<16; i++) szData[22+i] = szUID[i];

	sprintf(szWeight, "%6d", iWeight);
	for(i=0; i<6; i++) szData[38+i] = szWeight[i];

	//금액 항상 "0"
	sprintf(szCost, "%06d", 0);
	for(i=0; i<6; i++) szData[44+i] = szCost[i];

	//CRC 계산
	wCRC16 = CalcCRC16(szData, 0, (iDataLength-14)/2, szCRC16Buf);
	//CRC - space 10
	szData[iDataLength-14] = szCRC16Buf[2];
	szData[iDataLength-13] = szCRC16Buf[3];
	szData[iDataLength-12] = szCRC16Buf[0];
	szData[iDataLength-11] = szCRC16Buf[1];
	szData[iDataLength] = 0;

	sprintf(szDataLength, "%04d", strlen(szData));				   


	
///	의왕시   ,의정부 후불 aria key		공단에서 받아온 마스타 키  
	mk[0] = 0xC8;
	mk[1] = 0x43;
	mk[2] = 0x9A;
	mk[3] = 0x52;
	mk[4] = 0x8F;
	mk[5] = 0x84;
	mk[6] = 0x58;
	mk[7] = 0x60;
	mk[8] = 0x73;
	mk[9] = 0x0F;
	mk[10] = 0x68;
	mk[11] = 0x1F;
	mk[12] = 0x93;
	mk[13] = 0xD5;
	mk[14] = 0x7B;
	mk[15] = 0x47;
	mk[16] = 0x48;
	mk[17] = 0x3A;
	mk[18] = 0x47;
	mk[19] = 0xBD;
	mk[20] = 0x61;
	mk[21] = 0x3F;
	mk[22] = 0x28;
	mk[23] = 0xF3;
	mk[24] = 0xD6;
	mk[25] = 0xA7;
	mk[26] = 0x83;
	mk[27] = 0xB5;
	mk[28] = 0x2C;
	mk[29] = 0x87;
	mk[30] = 0xAA;
	mk[31] = 0x77;
	
/*   
   // 서귀포 선불, 제주시 선불, 의정부시 선불  ,부산후불 
	mk[0] = 0xF0;
	mk[1] = 0xB9;
	mk[2] = 0xC8;
	mk[3] = 0x47;
	mk[4] = 0x62;
	mk[5] = 0x21;
	mk[6] = 0xA3;
	mk[7] = 0x26;
	mk[8] = 0xC8;
	mk[9] = 0x84;
	mk[10] = 0xDE;
	mk[11] = 0xC8;
	mk[12] = 0xB9;
	mk[13] = 0xFE;
	mk[14] = 0xB0;
	mk[15] = 0x6F;
	mk[16] = 0x6F;
	mk[17] = 0x7E;
	mk[18] = 0xCD;
	mk[19] = 0x43;
	mk[20] = 0x65;
	mk[21] = 0x3A;
	mk[22] = 0x30;
	mk[23] = 0x8A;
	mk[24] = 0x03;
	mk[25] = 0x6C;
	mk[26] = 0x00;
	mk[27] = 0x4B;
	mk[28] = 0x70;
	mk[29] = 0x67;
	mk[30] = 0x0D;
	mk[31] = 0x71;
	 */

	EncKeySetup(mk, rk, 256);

	for(i=0; i<16; i++) p[i] = szData[i];
	Crypt(p, 16, rk, c);
	for(i=0; i<16; i++) szCData[i] = c[i];

	for(i=16; i<32; i++) p[i-16] = szData[i];
	Crypt(p, 16, rk, c);
	for(i=16; i<32; i++) szCData[i] = c[i-16];

	for(i=32; i<48; i++) p[i-32] = szData[i];
	Crypt(p, 16, rk, c);
	for(i=32; i<48; i++) szCData[i] = c[i-32];

	for(i=48; i<64; i++) p[i-48] = szData[i];
	Crypt(p, 16, rk, c);
	for(i=48; i<64; i++) szCData[i] = c[i-48];

///////////////////////////////////////////////////////////////////////////////////
////////////  	header 	///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

	strcpy(szHeader, "12C1W2V-201-01-1    10EN1234CR34");
//	strcpy(szHeader, "12C1WZZ-007-02-2    10EN1234CR34");

	jangbi_code_length = strlen(jangbi_code);
	for(i=0; i<jangbi_code_length; i++) szHeader[4+i] = *jangbi_code++;
	for( ;i<16; i++) szHeader[4+i] = ' ';

	szHeader[0x18] = iDataLength % 0x100;
	szHeader[0x19] = (iDataLength / 0x100) % 0x100;
	szHeader[0x1A] = (iDataLength / 0x10000) % 0x100;
	szHeader[0x1B] = (iDataLength / 0x1000000) % 0x100;

//	wCRC16 = CalcCRC16(szHeader, 2, (strlen(szHeader)-6)/2, szCRC16Buf);
	wCRC16 = CalcCRC16(szHeader, 2, (32-6)/2, szCRC16Buf);

	szHeader[0x1C] = wCRC16 % 0x100;
	szHeader[0x1D] = wCRC16 / 0x100;

	for(i=0; i<iDataLength; i++) szHeader[32+i] = szCData[i];

  /*
//  공단으로 전송 
	{
		for(i=0; i<iDataLength; i++) szHeader[32+i] = szCData[i];

		if(keco_send(szHeader, 32+iDataLength, sRet))
		{
			if((sRet[0] == '1') && (sRet[1] == '2') && (sRet[2] == 'R') && (sRet[3] == '1'))
			{
				ret_val = 1;			
			}
		}
	}
   
	return ret_val;
	 */
}

void Send_to_Gtech(void) // sms 문자에 의해서 gtech 서버에 상태정보를 전송한다. 093
{
	giGtechSendingStatusRetry = MAX_giGtechSendingStatusRetry;	//2014.8.25
	giGtechSendingStatusFlag = 1;	//2014.8.25
}

//2015.4.24
void cip_monitoring_data_make()
{
	char szT[100]; //25 * 4ea  123.456.789.012:3456
	int  iz;

	switch(giCipSendingFlag)
	{
		case 5:
			sprintf(szT, "%s:%d, %s:%d, %s:%d, %s:%d, %s:%d%d",
					gszKecoServerIp, gwKecoServerPort,	//gtech monitoring server base port
					gszGtechMonitoringServerIp, gwGtechMonitoringServerPort,	//gtech monitoring server base port
					gszIapServerIp, gwIapServerPort,	//gtech monitoring server base port
					gszCipServerIp, gwCipServerPort, 	//one time reply server port
					gszLguMonitoringServerIp, LGU_TEST_MODE_JANGBI_CODE, gwLguMonitoringServerPort	//lgu+ web monitoring server base port
			);

			//test printer
////			USART1_puts(szT);
			for(iz=0; szT[iz]; iz++) if(szT[iz] != '.') szT[iz] += 1;

			Send_to_Gtech_Event("IPS", szT); //설정된 IP를 전송한다.
			break;

		default:
			gtech_monitoring_data_make();
		break;
	}
}

void gtech_monitoring_data_make()
{
	int iz;
	unsigned long CRC16 = 0;
	unsigned short wCRC16;
	char szCRC16Buf[10];


//USART1_puts("gtech_monitoring_data_make            ");

	for(iz=0; iz<11; iz++)
	{
		if(('0'<=jangbi.cdma.telno[iz]) && (jangbi.cdma.telno[iz]<='9')) continue;	//전화번호가 숫자로만 이루어졌는지 확인
		get_cdma_telno();
	}

    get_cdma_rssi_with_rx2cardbuffer();
	get_jangbi_code();
	gszGigiguk[19] = 0;
/*
	//           0         1         2         3         4         5         6         7         8         9         0         1         2         3         4         5         6         7         8         9         0         1          
	//           0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789 
//	strcpy(str, "GB=CFG IDX=334 TEL=01081064792 TIME=20140407192003 VER=20130502 S/N=ZB11100288 MODE=REAL TID=W5P-02F-06-0 UID=W5P02F0000 STA=GGGGGGGGGGGGGGGG CAP=34 FID=6000010288 SAMID=0720080020371061 UNSEND=10 ACCESS=20140407192233");
	strcpy(str, "GB=CFG IDX=334 TEL=01081064792 TIME=20140407192003 VER=20130502 S/N=0011100288 MODE=REAL TID=W0V-001-01-1 STA=GGGGGGGG CAP=34 UNSEND=10 ACCESS=20140407192233");
*/
//			strcpy(gszSendingStatusGtech, "2014-08-11 16:30:34|012-3456-7890|WZZ-007-02-1|KKRTEST000123|TEST|30 |card123|2-14/08/11|6111111|444444");
//			strcpy(gszSendingStatusGtech, "01234567890|serial no 123|WZZ-007-02-1|6000060001|TEST|5068300025|2014/08/11|KKRTEST000123|30");
//			strcpy(gszSendingStatusGtech, "01039624730|ZB13090064|W6X-113-01-1|6000060026|REAL|5068300025|2013/01/03|KKR6WX1C20000113|22");
//	sprintf( gszSendingStatusGtech,"%s|14080064|%s|6000060026|REAL|5068300025|%s|KKR6WZ1C20000113|%d|%d|%s|%d|%d", jangbi.cdma.telno, jangbi.code, version, jangbi.rate, jangbi.cdma.rssi, "GGGGGGGG", 0, After_Weight);
	//부피센서, 리셋, 기타 상태들

/* 버전 0
//	sprintf( gszSendingStatusGtech,"STATUS|V1|0000|%s|14080064|%s|6000060026|REAL|5068300025|%s|KKR6WZ1C20000113|%d|%d|%c%c%c%c%c%c%c%c|%d|%d|%u|%u| %u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u| %u|%u|%u|%u|%02d%02d%02d%02d%02d%02d|%s|%04X|", \
//	 		jangbi.cdma.telno, gszJangbiCode, version, gxlCostPerOnekilo, jangbi.cdma.rssi, \
*/

	// 버전 1.1  add gxlCostMinWeight
//	sprintf( gszSendingStatusGtech,"STATUS|%s1|0000|%s|%s|%s|%u%03u|%s| |%u| | | |%d|REAL|%d|%c%c%c%c%c%c%c%c| |%d|%d|%u|%d|%u|%u|%u|%02d%02d%02d%02d%02d%02d|%s|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%04X|%c", 
	sprintf( gszSending,"STATUS|%s1|0000|%s|%s|%s|%u%03u|%s| |%u| | | |%d|REAL|%d|%c%c%c%c%c%c%c%c| |%d|%d|%u|%d|%u|%u|%u|%02d%02d%02d%02d%02d%02d|%s|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u|%04X|%c", \
	 		gszMonitoringProtocolVersion
	 		,gszModel, gszJangbiCode, jangbi.cdma.telno, tick_sec, clock_ms, version

			,SmellMinute

			, gxlCostPerOnekilo, jangbi.cdma.rssi, \
			//리셋, 왼쪽 모터 상태, 카드끼임 상태, 왼쪽수거통 차임상태(부피센서), 왼쪽수거통 있냐 없냐?, 왼쪽 수거통 윗문 상태, 왼쪽 저울상태, 왼쪽 카드리더기 상태 
			ghPowerOnResetStatus, Left_Moter_Status, Left_Card_Status, Left_BasketFull_Status, Left_BasketBeing_Status, Left_BasketDoor_Status, Left_Scale_Status, Left_CardReader_Status, \
			(gwBaichulCount-gwSendCount), gwBaichulCount, After_Weight \
			//현재 ad, 총무게
			,gxlGrossWeight, v_zero, Left_Adc, raw_data	\
	
			,jangbi.tm.year, jangbi.tm.month, jangbi.tm.date, jangbi.tm.hour, jangbi.tm.min, jangbi.tm.sec \
			,gszGigiguk \
			,gwVolumeDistance, \
			//setting value total 23
			gxlAutoCloseTime, \
			gxlNextCardTime, \
			gxlAutoVoice, gxlSoundMagnitude, \
			gxlLedOnTime, gxlLedOnTime_Stop, \
			gxlDisplay, \
			gxlCostPerOnekilo, gxlMinimumMoney, \
			gxlUpperWeight, gxlBasketWeight, \
			gxlBulkEnable, gxlBulkValue, gxlBulkWeight, gxlBulkEnable_Right, \
			gxlHandDetectEnable, gxlHandDetectValue, gxlHandDetectNumber, \
			gxlMoterOpenLoadEnable, gxlMoterOpenValue, gxlDoorCloseDetect, Operation_Fast, \
			gxlSmellMoterOperation, gxlSmellMoterPeriod \
			//배출최소무게설정 1.32s2(2014.11.10) 문자로 설정가능
			,gxlCostMinWeight \

/*
			gxlSmellMoterOperation, gxlSmellMoterPeriod, gxlUpperWeight, gxlBulkValue, gxlSoundMagnitude, gxlNextCardTime, gxlBulkWeight, \
			gxlBulkEnable, gxlBulkEnable_Right, gxlBasketWeight, gxlHandDetectEnable, gxlDoorCloseDetect, gxlDisplay, \
			gxlAutoCloseTime, gxlAutoVoice, gxlCostPerOnekilo, gxlMoterOpenLoadEnable, gxlHandDetectNumber, gxlMinimumMoney, gxlLedOnTime, gxlLedOnTime_Stop, \
			gxlHandDetectValue, gxlMoterOpenValue \
*/
			,CRC16
			,0x04	//EOT
	);


/*
gxlSmellMoterOperation>=0 && gxlSmellMoterOperation<=10) //탈취모터 동작 시간 0~10초까지 
gxlSmellMoterPeriod>=0 &&gxlSmellMoterPeriod<=60)	// 탈취모터 동작주기섥정  0~60분 
gxlUpperWeight=	((smsbuffer[8]-0x30)*100 + (smsbuffer[9]-0x30)*10 +(smsbuffer[10]-0x30))*100;// 최대무게 설정 
gxlBulkValue=	((smsbuffer[8]-0x30)*100 + (smsbuffer[9]-0x30)*10 +(smsbuffer[10]-0x30)); // 부피센서범위설정  
gxlSoundMagnitude=	((smsbuffer[8]-0x30)*100 + (smsbuffer[9]-0x30)*10 +(smsbuffer[10]-0x30));// 볼륨크기설정  
gxlNextCardTime=	((smsbuffer[8]-0x30)*100 + (smsbuffer[9]-0x30)*10 +(smsbuffer[10]-0x30));// 다음카드 대기시간  
gxlBulkWeight=	((smsbuffer[8]-0x30)*100 + (smsbuffer[9]-0x30)*10 +(smsbuffer[10]-0x30))*100;// 부피센서 연동무게 
gxlBulkWeight=	((smsbuffer[8]-0x30)*100 + (smsbuffer[9]-0x30)*10 +(smsbuffer[10]-0x30))*100;// 부피센서 동작설정 ENABLE/DISAABLE
gxlBulkEnable=1;
gxlBulkEnable_Right=1;// 부피센서 동작설정 오른쪽통  ENABLE/DISAABLE
gxlBulkEnable_Right=0;
gxlBasketWeight=	((smsbuffer[8]-0x30)*100 + (smsbuffer[9]-0x30)*10 +(smsbuffer[10]-0x30))*100;//빈통무게설정
gxlHandDetectEnable=1;//손끼임감지 동작설정 ENABLE/DISAABLE
gxlHandDetectEnable=0;
gxlDoorCloseDetect=1;//강제 문닫힘  ENABLE/DISAABLE
gxlDoorCloseDetect=0;
gxlDisplay=0;//에러코드 화면표시  ENABLE/DISAABLE
gxlDisplay=1;
gxlAutoCloseTime=	((smsbuffer[8]-0x30)*100 + (smsbuffer[9]-0x30)*10 +(smsbuffer[10]-0x30));// 자동닫힘설정  
gxlAutoVoice=1;//음량설정 ENABLE/DISAABLE
gxlAutoVoice=0;
gxlCostPerOnekilo=	((smsbuffer[8]-0x30)*100 + (smsbuffer[9]-0x30)*10 +(smsbuffer[10]-0x30));// 요율설정   
gxlMoterOpenLoadEnable=1;//문열때 MAX A/D 체크  ENABLE/DISAABLE
gxlMoterOpenLoadEnable=0;
gxlHandDetectNumber=	((smsbuffer[8]-0x30)*100 + (smsbuffer[9]-0x30)*10 +(smsbuffer[10]-0x30));// 손끼임감지횟수   
gxlMinimumMoney=	((smsbuffer[8]-0x30)*1000 + (smsbuffer[9]-0x30)*100 +(smsbuffer[10]-0x30)*10 +(smsbuffer[11]-0x30) );// 최소잔액 설정    
gxlLedOnTime=	((smsbuffer[8]-0x30)*1000 + (smsbuffer[9]-0x30)*100 +(smsbuffer[10]-0x30)*10 +(smsbuffer[11]-0x30) );//외부램프 동작시작설정     
gxlLedOnTime_Stop=gxlLedOnTime%100; // 꺼지는 시간	   // 8시일때 
gxlHandDetectValue=((smsbuffer[8]-0x30)*1000 + (smsbuffer[9]-0x30)*100 +(smsbuffer[10]-0x30)*10 +(smsbuffer[11]-0x30) );//손끼임 A/D 설정    
gxlMoterOpenValue=((smsbuffer[8]-0x30)*1000 + (smsbuffer[9]-0x30)*100 +(smsbuffer[10]-0x30)*10 +(smsbuffer[11]-0x30) );//문열때 부하체크값    
*/

	//데이터 길이 계산
	giSendingLength = strlen((const char *)gszSending); //2014.8.21
	gszSending[11] = giSendingLength / 1000 + '0';
	gszSending[12] = (giSendingLength % 1000) / 100 + '0';
	gszSending[13] = (giSendingLength % 100) / 10 + '0';
	gszSending[14] = giSendingLength % 10 + '0';
	
	//CRC 계산
	wCRC16 = CalcCRC16(gszSending, 0, (giSendingLength-5)/2, szCRC16Buf);
	//CRC
	gszSending[giSendingLength-6] = szCRC16Buf[2];
	gszSending[giSendingLength-5] = szCRC16Buf[3];
	gszSending[giSendingLength-4] = szCRC16Buf[0];
	gszSending[giSendingLength-3] = szCRC16Buf[1];
	gszSending[giSendingLength] = 0;

	//test printer
////	USART1_puts(gszSending);


/**
	//데이터 길이 계산
	giSendingStatusGtechLength = strlen((const char *)gszSendingStatusGtech); //2014.8.21
	gszSendingStatusGtech[11] = giSendingStatusGtechLength / 1000 + '0';
	gszSendingStatusGtech[12] = (giSendingStatusGtechLength % 1000) / 100 + '0';
	gszSendingStatusGtech[13] = (giSendingStatusGtechLength % 100) / 10 + '0';
	gszSendingStatusGtech[14] = giSendingStatusGtechLength % 10 + '0';
	
	//CRC 계산
	wCRC16 = CalcCRC16(gszSendingStatusGtech, 0, (giSendingStatusGtechLength-5)/2, szCRC16Buf);
	//CRC
	gszSendingStatusGtech[giSendingStatusGtechLength-6] = szCRC16Buf[2];
	gszSendingStatusGtech[giSendingStatusGtechLength-5] = szCRC16Buf[3];
	gszSendingStatusGtech[giSendingStatusGtechLength-4] = szCRC16Buf[0];
	gszSendingStatusGtech[giSendingStatusGtechLength-3] = szCRC16Buf[1];
	gszSendingStatusGtech[giSendingStatusGtechLength] = 0;

	//test printer
	USART1_puts(gszSendingStatusGtech);
**/

//	giGtechSendingStatusRetry = MAX_giGtechSendingStatusRetry;	//2014.8.25
//	giGtechSendingStatusFlag = 1;	//2014.8.25

	ghPowerOnResetStatus = 'G';	//reset power on reset status
}

void test_routine()
{
	int i, iz;
	char szDateTime[20];
	char card_str[16];
	s32 Cost_Weight=0; //가격을 책정할 weight
    char szData[256];
	char szMsg[50];
	u16  state_flag;	

/*
for(;;)
{
	SocketOpen_LU202F("61.108.48.26",13001, 10000);
	for(iz=0; iz<5000; iz++)
	{
		Delay_ms(1);
		check_response(RxCardBuffer2, "KFJDSL", RxCardCounter2, 6);
	}
	SocketClose_LU202F();
	for(iz=0; iz<5000; iz++)
	{
		Delay_ms(1);
		check_response(RxCardBuffer2, "KFJDSL", RxCardCounter2, 6);
	}
}
*/

/*
	if(Usart3_SMS_flag == 1)
	{
		Usart3_SMS_flag = 0;
		sms_queue_put(&smsbuffer[4]);
	}
*/					  

	guWatchdogTimer = WATCHDOG_TIME;	//2015.9.29

	if((iSendStage == 0) && ghSMS_VIP)
	{
		ghSMS_VIP = 0;
		giSendingType = 8;	//sms VIP 우선 처리
	}

	//배출정보, 상태정보, 모니터링 등 전송
	tcpip_send_20140806("192.168.1.1", 100);

	if(giWebSMSConnection == 4 &&  tick_sec<180) gbWebSMSConnection = 1;	//2015.9.22 disconned or 전원켠 후 120초 동안 재시도, 이후에는 1분단위 재시도

	//매분마다 실행하는 기능
	if(gbMinute)
	{
		gbMinute = 0;
		gbLampOperation = 1;  
		gbWebSMSConnection = 1;	//2015.9.22
	}

	if(SmellMinute==5)
   {

	SmellMinute=0;
	Cdma_RePower_Process();// 새벽 2시가 되면 무조건 CDMA 전원을 껏다 켠다. 


   }





//<< ver 1.32t1 2015.2.4
	if(gb10Minute)
	{
		gb10Minute = 0;
		//새벽3:10, 4:10 에 두 번 카드 정보 받기
/////// TEST ONLY 2015.10.1 EVERY HOUR		
		if((jangbi.tm.hour == 3) || (jangbi.tm.hour == 4)) 
		{
			//공단으로 부터 카드정보 받아오기 
			gbGetCardInfo = 1; 
		}
	}
	else if(gb30Minute)
	{
		gb30Minute = 0;
	}
//>> ver 1.32t1 2015.2.4 >>

	//매시간마다 실행하는 기능
	if(gbHour)
	{
		gbHour = 0;
		get_cdma_time();	//add 2014.10.24

		//새벽 2시 모뎀 리셋
		if(jangbi.tm.hour == 2) gbModemReset = 1; 

		//새벽3시나 4시네 
/////// TEST ONLY 2015.10.1 EVERY HOUR		
		if(jangbi.tm.hour==3 || jangbi.tm.hour==4) 
		{
			//공단으로 부터 카드 업데이트 정보 받아오기 
			gbGetCardUpdate = 1; 
		}

	  if(jangbi.tm.hour == 5)// 새벽 5시로 바꾸어야함  
		{
          Cdma_RePower_Process();// 새벽 2시가 되면 무조건 CDMA 전원을 껏다 켠다. 
		}

	}

	//2014.10.22
	//상태정보가 변했는지 검사해서 변했으면 서버로 전송
	{
		// Left_BasketDoor_Status, Left_BasketBeing_Status, ---, Left_CardReader_Status, Left_Scale_Status, Left_Moter_Status, ------, Left_BasketFull_Status
		// 왼쪽 수거통 윗문 상태, 왼쪽수거통 있냐 없냐?, 카드끼임 상태, 왼쪽 카드리더기 상태, 왼쪽 저울상태, 왼쪽 모터 상태, ------, 왼쪽수거통 차임상태(부피센서)
		// 02h 1	'0'	뚜껑상태 :정상 '0' , 오류 '1', '2',...
		// 03h 1	'0'	수거통상태: 정상 '0' , 오류 '1', '2',...(full 여부 등)
		// 04h 1	'0'	카드끼임상태: 정상 '0' , 오류 '1', '2',...
		// 05h 1	'0'	리더의 동작 상태: 정상 '0' , 오류 '1', '2',...
		// 06h 1	'0'	전자저울 동작상태: 정상 '0' , 오류 '1', '2',...
		// 07h 1	'0'	모터부상태: 정상 '0' , 오류 '1', '2',...
		// 08h 1	'0'	모뎀상태:정상 ‘0’, 오류 ‘1’, ‘2’,….
		// 09h 1	'0'	장비만통상태:비만통 ‘0’, 만통 ‘1’

		state_flag = 0x0000;
		if(Left_BasketDoor_Status == 'N') 	state_flag += 0x0001;
		if(Left_BasketBeing_Status == 'N') 	state_flag += 0x0002;
		//if( == 'N') 						state_flag += 0x0004;
		if(Left_CardReader_Status == 'N') 	state_flag += 0x0008;
		if(Left_Scale_Status == 'N') 		state_flag += 0x0010;
		if(Left_Moter_Status == 'N') 		state_flag += 0x0020;
		//if( == 'N') 						state_flag += 0x0040;
		if(Left_BasketFull_Status == 'N') 	state_flag += 0x0080;

		if(gwState_flag != state_flag)
		{
			gbSendStatusToMonitoringServer = 1;
			gbSendStatusToKECO = 1;

			gbSendStatusToLGU = 1;


			gwState_flag = state_flag;
		}
	}

	//시간되면 서버로 상태정보 전송
	if(SEND_INTERVAL_MONITORING < monitoring_timer_ms) monitoring_timer_ms = 0;
	if(!monitoring_timer_ms)
	{
		monitoring_timer_ms = SEND_INTERVAL_MONITORING;	//60 min  나중에는 60분 

		gbSendStatusToMonitoringServer = 1;
		gbSendStatusToKECO = 1;
		gbSendStatusToLGU = 1;
	}

	//action 플래그에 따라 기능 동작
	if(gbSendStatusToMonitoringServer)
	{
		gbSendStatusToMonitoringServer = 0;
		Send_to_Gtech();
	}

/****************** test inging 
	if(gbSendStatusToKECO)
	{
		gbSendStatusToKECO = 0;
		SendStatusToKeco();//1.32o
	}
	if(gbSendStatusToLGU)
	{
		gbSendStatusToLGU = 0;
		Send_to_Lguplus_Status();
	}
	if(gbLampOperation)
	{
		gbLampOperation = 0;  

		//LED LAMP ON/OFF, 소독 램프 ON/OFF
		Current_Hour = jangbi.tm.hour;
		Lamp_Operation();
	}
 ****************** test inging ***/


	if(gbSendStatusToKECO)
	{
		gbSendStatusToKECO = 0;
		SendStatusToKeco();//1.32o
	}
	if(gbLampOperation)
	{
		gbLampOperation = 0;  

		//LED LAMP ON/OFF, 소독 램프 ON/OFF
		Current_Hour = jangbi.tm.hour;
		Lamp_Operation();
	}





//<<2015.8.12
//	GLCD_string (0,0 ,"투입이 끝나면  ");	    
//	GLCD_string (0,2 ,"카드를 다시한번");	  
	//현재 날짜, 시간
/*
	sprintf(szMsg, "%04d%02d%02d", clock_year, clock_month, clock_date);
	GLCD_string (0,2 ,szMsg);
	sprintf(szMsg, "%02d%02d%02d", clock_hour, clock_min, clock_sec);
	GLCD_string (0,4 ,szMsg);
*/
/*
	sprintf(szMsg, "%04d%02d%02d.%02d%02d%02d", clock_year, clock_month, clock_date, clock_hour, clock_min, clock_sec);
	GLCD_string (0,4 ,szMsg);
*/
//>>2015.8.12


#ifdef ____20150423___because_code_size_too_big___

	if(RxCounter1 == 1)		   // from pc
	{
		USART1_puts("S");

		RxCounter1 = 0;
		if(RxBuffer1[0] == 'S')	// pc 상에서 '4'누르면 
		{
			get_cdma_time();
//		 	sprintf(szMsg, "현재시각은 %04d-%02d-%02d %02d:%02d:%02d 입니다.", jangbi.tm.year, jangbi.tm.month, jangbi.tm.date, jangbi.tm.hour, jangbi.tm.min, jangbi.tm.sec);
//		 	sprintf(szMsg, "현재시각은 %04d-%02d-%02d%  02d:%02d:%02d c%c%%c%c입니다.", jangbi.tm.year, jangbi.tm.month, jangbi.tm.date, jangbi.tm.hour, jangbi.tm.min, jangbi.tm.sec, jangbi.tm.month, jangbi.tm.date,jangbi.tm.min,jangbi.tm.sec);
		 	sprintf(szMsg, "HEX TEST |%c%c%c%c%c|%c%c%c%c%c|%c%c%c%c%c|...", 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);

			RxCardCounter2 = 0;		
			Keco_card_info_flag = 1;	// 공단으로부터 배출원 정보를 받아오을때 
			memset(RxCardBuffer2, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화

			send_sms("01091954072", szMsg);
//			send_sms("01034431529", szMsg);

			Delay_ms(1000);
			for(iz=0; iz<RxCardCounter2; iz++) USART1_putchar(RxCardBuffer2[iz]);
			Keco_card_info_flag = 0;	// 공단으로부터 배출원 정보를 받아오을때 
		}
		else if(RxBuffer1[0] == '4')	// pc 상에서 '4'누르면 
		{
/*
		   	sprintf(szData, "%c%c%c%c%c%c%c%c%c%c%c%c",local_code[3],local_code[2],local_code[1],local_code[0],
	                                        local_code[7],local_code[6],local_code[5],local_code[4],
											local_code[11],local_code[10],local_code[9],local_code[8]);
			keco_send_c4_20140808(szData, 123);	
*/
			get_jangbi_code();
			keco_send_c4_20140808(gszJangbiCode, 23);	
		}

		else if(0 && RxBuffer1[0] == '1') // pc 상에서 '1' 누르먼  //disable 2014.11.10  ver1.32s3
//		else if(RxBuffer1[0] == '1') // pc 상에서 '1' 누르먼  //disable 2014.11.10  ver1.32s3
		{
			USART1_puts("D");

			if(get_cdma_time()) {}	
			IWDG_ReloadCounter();  // about 20 sec
    
			// 시간 저장 	 
		 	sprintf(szDateTime, "%04d%02d%02d%02d%02d%02d", jangbi.tm.year, jangbi.tm.month, jangbi.tm.date, jangbi.tm.hour, jangbi.tm.min, jangbi.tm.sec);
	     	for(i=0; i<14; i++) //szData[8+i] = szDateTime[i+1];
         			baichul_info_nor[0].date[i]=szDateTime[i];
      
			//카드번호저장 	 
//			strcpy(card_str, "3514BA21");
//			strcpy(card_str, "19035db0");	//0419650992 -  성북 
//			strcpy(card_str, "abfb9014");	//AD2885390356      KKRW081C100013494(ABFB9014) 109 306   3157  
//			strcpy(card_str, "c43d3db6");	//3057466820        KKR2081C100020651
			strcpy(card_str, "01234567");	//3057466820        KKR2081C100020651

		 	for(i=0; i<8; i++)
					baichul_info_nor[0].card1_uid[i] = card_str[i];
	        for(i=8; i<16; i++)
					baichul_info_nor[0].card1_uid[i] = 0;
			// 배출무게저장 	 
		   	baichul_info_nor[0].weight = jangbi.tm.sec*100+jangbi.tm.sec;	//Cost_Weight;
//		   	baichul_info_nor[0].weight = 0;

			// 새로운 데이타가 들어있다 
	
		 	baichul_info_nor[0].Status_keco_new=7777;//0529
		 
			// 공단으로 보냈느냐  
	
			//// baichul_info_nor[0].Status_keco_send=0;//0529

         	// nor counter sending
         	sprintf(szData, "nor_clunter =%7ld",Nor_Flash_Counter);
		 	USART1_puts(szData);

			//배출카운터를 기록한다. 
		 	Nor_data_Writhing(Nor_Flash_Counter,0);


		 	/*
		 				sprintf(szData, "date =%c%c%c%c%c%c%c%c%c%c%c%c%c%c",baichul_info_nor[0].date[0],
		 													  baichul_info_nor[0].date[1],
															  baichul_info_nor[0].date[2],
															  baichul_info_nor[0].date[3],
															  baichul_info_nor[0].date[4],
															  baichul_info_nor[0].date[5],
															  baichul_info_nor[0].date[6],
															  baichul_info_nor[0].date[7],
															  baichul_info_nor[0].date[8],
															  baichul_info_nor[0].date[9],
															  baichul_info_nor[0].date[10],
															  baichul_info_nor[0].date[11],
															  baichul_info_nor[0].date[12],baichul_info_nor[1].date[13]);
	     				USART1_puts(szData);





		 				sprintf(szData, "weight =%4ld",baichul_info_nor[0].weight);
		 				USART1_puts(szData);
	    				sprintf(szData, "card_no =%c%c%c%c%c%c%c%c",         baichul_info_nor[0].card1_uid[0],
		 													  baichul_info_nor[0].card1_uid[1],
															  baichul_info_nor[0].card1_uid[2],
															  baichul_info_nor[0].card1_uid[3],
															  baichul_info_nor[0].card1_uid[4],
															  baichul_info_nor[0].card1_uid[5],
															  baichul_info_nor[0].card1_uid[6],
															  baichul_info_nor[0].card1_uid[7]);
		 				USART1_puts(szData);
         
		 
		
		
		 				Nor_data_reading(Nor_Flash_Counter); // 한번 읽어봐 

						//읽은 데이타 보내보기 
         				sprintf(szData, "date =%c%c%c%c%c%c%c%c%c%c%c%c%c%c",baichul_info_nor[1].date[0],
		 													  baichul_info_nor[1].date[1],
															  baichul_info_nor[1].date[2],
															  baichul_info_nor[1].date[3],
															  baichul_info_nor[1].date[4],
															  baichul_info_nor[1].date[5],
															  baichul_info_nor[1].date[6],
															  baichul_info_nor[1].date[7],
															  baichul_info_nor[1].date[8],
															  baichul_info_nor[1].date[9],
															  baichul_info_nor[1].date[10],
															  baichul_info_nor[1].date[11],
															  baichul_info_nor[1].date[12],baichul_info_nor[1].date[13]);
	     				USART1_puts(szData);


		 				sprintf(szData, "weight =%4ld",baichul_info_nor[1].weight);
		 				USART1_puts(szData);


		 				sprintf(szData, "card_no =%c%c%c%c%c%c%c%c",         baichul_info_nor[1].card1_uid[0],
		 													  baichul_info_nor[1].card1_uid[1],
															  baichul_info_nor[1].card1_uid[2],
															  baichul_info_nor[1].card1_uid[3],
															  baichul_info_nor[1].card1_uid[4],
															  baichul_info_nor[1].card1_uid[5],
															  baichul_info_nor[1].card1_uid[6],
															  baichul_info_nor[1].card1_uid[7]);
						USART1_puts(szData);													  
	 					// 5월30일 salsa

		 				*/


			//5월31일shark
		 
		 	Nor_Flash_Counter++;
		 	Fsmc_Enable();

		 	FSMC_NOR_EraseBlock(NOR_BAICHUL_BASE+0x70000);
		 	FSMC_NOR_WriteHalfWord(NOR_BAICHUL_BASE+0x70000,Nor_Flash_Counter); // 
 
		 
		 	Fsmc_Disable();
		}		
	}
#endif	//#ifdef ____20150423___because_code_size_too_big___
}



int keco_send_c4_20140808(char *jangbi_code, unsigned int uWeight)
{
	int ret_val = 0;
	char szCRC16Buf[10];
	unsigned short wCRC16;

	char szHeader[50];
	char szData[100];
	char szDataLength[10];
	int	 iDataLength;
	char szDateTime[20];
	char szWeight[10];
	int	i, iz;	  
	int jangbi_code_length;

///////////////////////////////////////////
/////////////	data protocol	///////////
///////////////////////////////////////////
	//DATA 초기화
	//              01234567890123456789012345678901234567890123456789012345678901234567890123456789
//	strcpy(szData, "C400000000008000640070201309231835030000000000000000000018123456789012345678CRC1");
	strcpy(szData, "C400000000008000640000201404011635030000000000000000000018 N/A              CRC1");

	if(150 < uWeight) uWeight = 150;
	sprintf(szWeight, "%04ud", uWeight);
	for(i=0; i<4; i++) szData[14+i] = szWeight[i]; //may be protocol error
//	for(i=0; i<4; i++) szData[18+i] = szWeight[i];

	//현재 날짜, 시간
	sprintf(szDateTime, "%04d%02d%02d%02d%02d%02d", jangbi.tm.year, jangbi.tm.month, jangbi.tm.date, jangbi.tm.hour, jangbi.tm.min, jangbi.tm.sec);
	for(i=0; i<14; i++) szData[22+i] = szDateTime[i];

	iDataLength = strlen(szData);
	//CRC 계산
	wCRC16 = CalcCRC16(szData, 0, (iDataLength-4)/2, szCRC16Buf);
	//CRC
	szData[iDataLength-4] = szCRC16Buf[2];
	szData[iDataLength-3] = szCRC16Buf[3];
	szData[iDataLength-2] = szCRC16Buf[0];
	szData[iDataLength-1] = szCRC16Buf[1];
	szData[iDataLength] = 0;

	sprintf(szDataLength, "%04d", strlen(szData));


///////////////////////////////////////////
/////////////	header	///////////////////
///////////////////////////////////////////

	strcpy(szHeader, "12C4JANGBI_CODE     10PL1234CR34");
//	strcpy(szHeader, "12C4W2V-201-01-1    10PL1234CR34");
//	strcpy(szHeader, "12C4WZZ-007-02-2    10PL1234CR34");

/* 2014.8.22  unknown issue
	jangbi_code_length = strlen(jangbi_code);
	for(i=0; i<jangbi_code_length ; i++)
	{
		szHeader[4+i] = *jangbi_code++;
	}
*/
	get_jangbi_code();
	jangbi_code_length = strlen(gszJangbiCode);
	for(i=0; i<jangbi_code_length; i++) szHeader[4+i] = gszJangbiCode[i];
	for( ;i<16; i++) szHeader[4+i] = ' ';

	szHeader[0x18] = iDataLength % 0x100;
	szHeader[0x19] = (iDataLength / 0x100) % 0x100;
	szHeader[0x1A] = (iDataLength / 0x10000) % 0x100;
	szHeader[0x1B] = (iDataLength / 0x1000000) % 0x100;

//	wCRC16 = CalcCRC16(szHeader, 2, (strlen(szHeader)-6)/2, szCRC16Buf);
	wCRC16 = CalcCRC16(szHeader, 2, (32-6)/2, szCRC16Buf);

/*
	szHeader[0] = 1;
	szHeader[1] = 2;
	szHeader[2] = 3;
	szHeader[3] = 4;
	szHeader[4] = 5;
	szHeader[5] = 6;
	szHeader[6] = 0;

	wCRC16 = CalcCRC16(szHeader, 2, 1, szCRC16Buf);
*/
	szHeader[0x1C] = wCRC16 % 0x100;
	szHeader[0x1D] = wCRC16 / 0x100;

	//make sending data
	for(iz=0; iz<32; iz++) gszSendingStatus[iz] = szHeader[iz];
	for(iz=0; iz<iDataLength; iz++) gszSendingStatus[iz+32] = szData[iz];
	//for(i=0; i<iDataLength; i++) szHeader[32+i] = szCData[i];
	giSendingStatusLength = 32 + iDataLength; //2014.8.7

	giKecoSendingStatusRetry = MAX_giKecoSendingStatusRetry;	//2014.8.22
	giKecoSendingStatusFlag = 1;	//2014.8.22

/*
	for(iz=0; iz<iDataLength; iz++)
	{
		USART1_putchar(szData[iz]);
	}
	for(iz=0; iz<giSendingStatusLength; iz++)
	{
		USART1_putchar(gszSendingStatus[iz]);
	}
*/

/***
HEADER

Address	00	01	02	03	04	05	06	07	08	09	0A	0B	0C	0D	0E	0F
Value(Hex)	31h	32h													20h	20h
Comment	STX
'1'    '2'	명령코드	장비 ID 16 바이트 ( 16바이트가 안될 때 남은 부분은 스페이스로 채움)
수거차량경우 차량번호(utf-8 엔코딩))
Address	10	11	12	13	14	15	16	17	18	19	1A	1B	1C	1D	1E	1F
Value(Hex)	20h	20h	20h	20h	31h	30h	'E'	'N'							33h	34h
Comment	 	Version
'1'    '0'	암호여부
EN: 암호
PL: 평문	LSB          MSB
헤더를 제외한 데이터 크기
(정수, Little endian)
암호화 되어 있다면 16바이트 단위로 할당된 값이 되어야 한다.
	CRC
02h~1Bh 영역을 A5A5h를 초기값으로 하여16Bits
XOR 한다.	ETX
'3'    '4'
Function 프로토콜
  시작	각 Function 프로토콜의 규약에 따른다.
암호화 되어 있거나, 또는 평문일 수 있다. 이는 헤더의 정보에 의존한다.
암호화 되어 있을 경우 배포된 ARIA키를 이용하여 Decoding 한 후에 해석을 시작해야 한다.

unsigned short CalcCRC16(unsigned char *pData,int nOffset,int nWords,char *szCRC16Buf)



Offset	Size(bytes)	Data	 Comment
00h - 01h	2	'C4'	개별계량장비 상태정보를 보낸다는 Function Code ('C4')

02h 1	'0'	뚜껑상태 :정상 '0' , 오류 '1', '2',...
03h	1	'0'	수거통상태: 정상 '0' , 오류 '1', '2',...(full 여부 등)
04h	1	'0'	카드끼임상태: 정상 '0' , 오류 '1', '2',...
05h	1	'0'	리더의 동작 상태: 정상 '0' , 오류 '1', '2',...
06h	1	'0'	전자저울 동작상태: 정상 '0' , 오류 '1', '2',...
07h	1	'0'	모터부상태: 정상 '0' , 오류 '1', '2',...
08h	1	‘0’	모뎀상태:정상 ‘0’, 오류 ‘1’, ‘2’,….
09h	1	‘0’	장비만통상태:비만통 ‘0’, 만통 ‘1’

0Ah-0Dh	4	‘0080‘	장비가용 최대 부피(%)
0Eh-11h	4	'0064'	수거함 현재중량(%)  예시값(64 KG), 최대 999 KG
12h-15h	4	'0070'	수거함 현재부피(%)  예시값(70 %)
16h-23h	14	'yyyyMMddHHmmss' 	최근업데이트시간,
24h-29h	6	'000256'	장비가 수신한 배출원정보 건수(256건)
2Ah-2Fh	6	'000256'	장비가 수신해야 할 총 건수 (256건)
30h-35h	6	'000256'	현재 장비에 저장되어있는 배출원수
36h-39h 	4	'0012'	Etc 정보 길이 (예시값)  12 bytes
(반드시 짝수 길이로 할것)
3Ah-45h	[36-39] 값
12	'123456789012'	Etc 정보 (위의 구성요소 외 발생장애상황)
(가변 길이 필드,짝수 길이여야 함)

46h - 49h	4	'XXXX'	CRC16값 프로토콜 OFFSET 00h ~ 45h까지의 값을 A5A5h를 초기값으로 하여 16Bits
XOR한 값을 4자리 16진수 문자열로 표기.
ETC 정보의 길이가 가변이므로 CRC검사 영역은 계산되어야 함 .

****/

	return ret_val;
}


int get_cdma_time_with_rx2cardbuffer_old()
{
	int ret_val;
	int i, iz;
	int	 iRetry;
	char szMsg[50];
	char szResponse[15];
	int	 year, month, date, hour, min, sec;

	ret_val = 0;
	iRetry = 10;	//MAX_RETRY;

	do
	{
		strcpy(szMsg, "AT$LGTTIME?");
		RxCardCounter2 = 0;		
		Keco_card_info_flag = 1;	//cdma data 받아오을때 
		memset(RxCardBuffer2, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화

		cdma_send_cmd(szMsg, strlen(szMsg));

		sprintf(szResponse, "OK%c%c%c", 0x0d, 0x0a, 0);
		Delay_ms(10);	//2014.04.02  add because fail

		for(i=0; i<100; i++)
		{
			Delay_ms(11);

//for(iz=0; iz<RxCardCounter2; iz++) 	USART1_putchar(RxCardBuffer2[iz]);
//USART1_putchar('C');
//for(iz=23; iz<30; iz++) 	USART1_putchar(RxCardBuffer2[iz]);

			if(check_response(RxCardBuffer2, szResponse, RxCardCounter2, strlen(szResponse)))
			{	
				Keco_card_info_flag = 0;	//cdma data 받아오을때 
#ifdef	___LGUPLUS_AM200____
	 			year = (RxCardBuffer2[23]-'0')*1000 + (RxCardBuffer2[24]-'0')*100 + (RxCardBuffer2[25]-'0')*10 + (RxCardBuffer2[26]-'0');
				month = (RxCardBuffer2[27]-'0')*10 + (RxCardBuffer2[28]-'0');
				date = (RxCardBuffer2[29]-'0')*10 + (RxCardBuffer2[30]-'0');
				hour = (RxCardBuffer2[31]-'0')*10 + (RxCardBuffer2[32]-'0');
				min = (RxCardBuffer2[33]-'0')*10 + (RxCardBuffer2[34]-'0');
				sec = (RxCardBuffer2[35]-'0')*10 + (RxCardBuffer2[36]-'0');
#else
	 			year = (RxCardBuffer2[22]-'0')*1000 + (RxCardBuffer2[23]-'0')*100 + (RxCardBuffer2[24]-'0')*10 + (RxCardBuffer2[25]-'0');
				month = (RxCardBuffer2[26]-'0')*10 + (RxCardBuffer2[27]-'0');
				date = (RxCardBuffer2[28]-'0')*10 + (RxCardBuffer2[29]-'0');
				hour = (RxCardBuffer2[30]-'0')*10 + (RxCardBuffer2[31]-'0');
				min = (RxCardBuffer2[32]-'0')*10 + (RxCardBuffer2[33]-'0');
				sec = (RxCardBuffer2[34]-'0')*10 + (RxCardBuffer2[35]-'0');
#endif

				if((2015<=year) && (year<=2050))
				{
					if((1<=month) && (month<=12))
					{
						if((1<=date) && (date<=31))
						{
							if((0<=hour) && (hour<=23))
							{
								if((0<=min) && (min<=59))
								{
									if((0<=sec) && (sec<=59))
									{
										ret_val = 1;

							 			jangbi.tm.year = year;
										jangbi.tm.month = month;
										jangbi.tm.date = date;
										jangbi.tm.hour = hour;
										jangbi.tm.min = min;
										jangbi.tm.sec = sec;
									}
								}
							}
						}
					}
				}
//2014.10.16	ret_val = 1;
				break;
			}

			//break;
		}

		Keco_card_info_flag = 0;	//cdma data 받아오을때 
	} while(--iRetry && !ret_val);

//	USART1_putchar('R');
//	USART1_putchar(iRetry+'0');

	return ret_val;
}


#ifdef dpdpf 
int get_cdma_time_with_rx2cardbuffer()	 //0916
{
	int ret_val;
	int	 year, month, date, hour, min, sec;
	
	if(ret_val = get_cdma_with_rx2cardbuffer("AT$LGTTIME?",  "OK", 2))
	{
#ifdef	___LGUPLUS_AM200____
	 			year = (RxCardBuffer2[23]-'0')*1000 + (RxCardBuffer2[24]-'0')*100 + (RxCardBuffer2[25]-'0')*10 + (RxCardBuffer2[26]-'0');
				month = (RxCardBuffer2[27]-'0')*10 + (RxCardBuffer2[28]-'0');
				date = (RxCardBuffer2[29]-'0')*10 + (RxCardBuffer2[30]-'0');
				hour = (RxCardBuffer2[31]-'0')*10 + (RxCardBuffer2[32]-'0');
				min = (RxCardBuffer2[33]-'0')*10 + (RxCardBuffer2[34]-'0');
				sec = (RxCardBuffer2[35]-'0')*10 + (RxCardBuffer2[36]-'0');
#else
	 			year = (RxCardBuffer2[22]-'0')*1000 + (RxCardBuffer2[23]-'0')*100 + (RxCardBuffer2[24]-'0')*10 + (RxCardBuffer2[25]-'0');
				month = (RxCardBuffer2[26]-'0')*10 + (RxCardBuffer2[27]-'0');
				date = (RxCardBuffer2[28]-'0')*10 + (RxCardBuffer2[29]-'0');
				hour = (RxCardBuffer2[30]-'0')*10 + (RxCardBuffer2[31]-'0');
				min = (RxCardBuffer2[32]-'0')*10 + (RxCardBuffer2[33]-'0');
				sec = (RxCardBuffer2[34]-'0')*10 + (RxCardBuffer2[35]-'0');
#endif
			

              //0916_1  

		//	 sprintf(str, "year=%4d-month=%2d-date=%2d",year,month,date);
        //     USART1_puts(str);
			 cdma_error_flag=0; // cdma로 부터 뭐라도 날아왔으면 
			 time_error_flag=1; // 엉뚱한 시간이 날아오면 
			 
			
				ret_val = 0;

				if((2015<=year) && (year<=2050))
				{
					if((1<=month) && (month<=12))
					{
						if((1<=date) && (date<=31))
						{
							if((0<=hour) && (hour<=23))
							{
								if((0<=min) && (min<=59))
								{
									if((0<=sec) && (sec<=59))
									{
										ret_val = 1;
										time_error_flag=0; // 정상적인 시간이 날아오면 

							 			jangbi.tm.year = year;
										jangbi.tm.month = month;
										jangbi.tm.date = date;
										jangbi.tm.hour = hour;
										jangbi.tm.min = min;
										jangbi.tm.sec = sec;
									}
								}
							}
						}
					}
				}
			}

		else
		{
		 if(cdma_error_check_flag)//체크 할때만 
		    cdma_error_flag++;// cdma로 부터 아무것도 날아오지 않으면 cdma 모듈이 없는것으로 판단 
		}
			Keco_card_info_flag = 0;	//cdma data 받아오을때 
	
			return ret_val;
}
 #endif 

int get_cdma_time_with_rx2cardbuffer()	 //0916
{
	int ret_val;
	int	 year, month, date, hour, min, sec;
	
	if(ret_val = get_cdma_with_rx2cardbuffer("AT$LGTTIME?",  "OK", 2))
	{
#ifdef	___LGUPLUS_AM200____
	 			year = (RxCardBuffer2[23]-'0')*1000 + (RxCardBuffer2[24]-'0')*100 + (RxCardBuffer2[25]-'0')*10 + (RxCardBuffer2[26]-'0');
				month = (RxCardBuffer2[27]-'0')*10 + (RxCardBuffer2[28]-'0');
				date = (RxCardBuffer2[29]-'0')*10 + (RxCardBuffer2[30]-'0');
				hour = (RxCardBuffer2[31]-'0')*10 + (RxCardBuffer2[32]-'0');
				min = (RxCardBuffer2[33]-'0')*10 + (RxCardBuffer2[34]-'0');
				sec = (RxCardBuffer2[35]-'0')*10 + (RxCardBuffer2[36]-'0');
#else
	 			year = (RxCardBuffer2[22]-'0')*1000 + (RxCardBuffer2[23]-'0')*100 + (RxCardBuffer2[24]-'0')*10 + (RxCardBuffer2[25]-'0');
				month = (RxCardBuffer2[26]-'0')*10 + (RxCardBuffer2[27]-'0');
				date = (RxCardBuffer2[28]-'0')*10 + (RxCardBuffer2[29]-'0');
				hour = (RxCardBuffer2[30]-'0')*10 + (RxCardBuffer2[31]-'0');
				min = (RxCardBuffer2[32]-'0')*10 + (RxCardBuffer2[33]-'0');
				sec = (RxCardBuffer2[34]-'0')*10 + (RxCardBuffer2[35]-'0');
#endif
			

              //0916_1  

		//	 sprintf(str, "year=%4d-month=%2d-date=%2d",year,month,date);
        //     USART1_puts(str);
			 cdma_error_flag=0; // cdma로 부터 뭐라도 날아왔으면 
			// time_error_flag=1; // 엉뚱한 시간이 날아오면 
			  time_error_flag++;
			  
			  if(time_error_flag>1) // 연속 2번 이상한 시간 읽어오면 
			  {
			   Cdma_RePower_Process();
			   time_error_flag=0;
			 //   cdma_error_flag=0;
			
			  }
				ret_val = 0;

				if((2015<=year) && (year<=2050))
				{
					if((1<=month) && (month<=12))
					{
						if((1<=date) && (date<=31))
						{
							if((0<=hour) && (hour<=23))
							{
								if((0<=min) && (min<=59))
								{
									if((0<=sec) && (sec<=59))
									{
										ret_val = 1;
										time_error_flag=0; // 정상적인 시간이 날아오면 

							 			jangbi.tm.year = year;
										jangbi.tm.month = month;
										jangbi.tm.date = date;
										jangbi.tm.hour = hour;
										jangbi.tm.min = min;
										jangbi.tm.sec = sec;
									}
								}
							}
						}
					}
				}
			}

		else
		{
		// if(cdma_error_check_flag)//체크 할때만 
		    cdma_error_flag++;// cdma로 부터 아무것도 날아오지 않으면 cdma 모듈이 없는것으로 판단 
			if(cdma_error_flag>1)
			 {
              Cdma_RePower_Process();
			  cdma_error_flag=0;
			 }

		}
			Keco_card_info_flag = 0;	//cdma data 받아오을때 
	
			return ret_val;
}






int get_cdma_telno_with_rx2cardbuffer_old()
{
	int ret_val;
	int i, iz;
	int	 iRetry;
	char szMsg[50];
	char szResponse[15];
	char telno[13];

	ret_val = 0;
	iRetry = 10;	//MAX_RETRY;

	do
	{
		strcpy(szMsg, "AT$LGTMDN?");
		RxCardCounter2 = 0;		
		Keco_card_info_flag = 1;	//cdma data 받아오을때 
		memset(RxCardBuffer2, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화

		cdma_send_cmd(szMsg, strlen(szMsg));  //----------------------------

		sprintf(szResponse, "OK%c%c%c", 0x0d, 0x0a, 0);
		Delay_ms(10);	//2014.04.02  add because fail

		for(i=0; i<100; i++)
		{
			Delay_ms(1);

//for(iz=0; iz<RxCardCounter2; iz++) 	USART1_putchar(RxCardBuffer2[iz]);
//USART1_putchar('C');
//for(iz=21; iz<30; iz++) 	USART1_putchar(RxCardBuffer2[iz]);

			if(check_response(RxCardBuffer2, szResponse, RxCardCounter2, strlen(szResponse)))
			{	
				Keco_card_info_flag = 0;	//cdma data 받아오을때 

				for(iz=0; iz<11; iz++)
				{
#ifdef	___LGUPLUS_AM200____
					telno[iz] = RxCardBuffer2[iz+21];
#else
					telno[iz] = RxCardBuffer2[iz+20];
#endif

					if(('0'<=telno[iz]) && (telno[iz]<='9')) continue;
				}
				if(11 == iz)
				{
					ret_val = 1;

					for(iz=0; iz<11; iz++) jangbi.cdma.telno[iz] = telno[iz];
					jangbi.cdma.telno[iz] = 0;
				}
				ret_val = 1;
				break;
			}

		//	break;
		}

		Keco_card_info_flag = 0;	//cdma data 받아오을때 

	} while(--iRetry && !ret_val);


	return ret_val;
}

int get_cdma_telno_with_rx2cardbuffer()
{
	int iz;
	int ret_val;
	char telno[13];
	
	if(ret_val = get_cdma_with_rx2cardbuffer("AT$LGTMDN?",  "OK", 2))
	{				
		ret_val = 0;
			
		for(iz=0; iz<11; iz++)
		{
#ifdef	___LGUPLUS_AM200____
			telno[iz] = RxCardBuffer2[iz+21];
#else
			telno[iz] = RxCardBuffer2[iz+20];
#endif
			if(('0'<=telno[iz]) && (telno[iz]<='9')) continue;
		}
		if(11 == iz)
		{
			ret_val = 1;
			for(iz=0; iz<11; iz++) jangbi.cdma.telno[iz] = telno[iz];
			jangbi.cdma.telno[iz] = 0;
		}
	}
	Keco_card_info_flag = 0;	//cdma data 받아오을때

	return ret_val;
}	


int get_cdma_rssi_with_rx2cardbuffer_old()
{
	int ret_val;
	int i, iz;
	int	 iRetry;
	char szMsg[50];
	char szResponse[15];
	int	 year, month, date, hour, min, sec;

	ret_val = 0;
	iRetry = 10;	//MAX_RETRY;

		do
	{
		strcpy(szMsg, "AT$LGTRSSI?");
		RxCardCounter2 = 0;		
		Keco_card_info_flag = 1;	//cdma data 받아오을때 
		memset(RxCardBuffer2, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화

		cdma_send_cmd(szMsg, strlen(szMsg));

		sprintf(szResponse, "OK%c%c%c", 0x0d, 0x0a, 0);
		Delay_ms(10);	//2014.04.02  add because fail

		for(i=0; i<100; i++)
		{
			Delay_ms(1);

//for(iz=0; iz<RxCardCounter2; iz++) 	USART1_putchar(RxCardBuffer2[iz]);
//USART1_putchar('C');
//for(iz=23; iz<30; iz++) 	USART1_putchar(RxCardBuffer2[iz]);

			if(check_response(RxCardBuffer2, szResponse, RxCardCounter2, strlen(szResponse)))
			{	
				Keco_card_info_flag = 0;	//cdma data 받아오을때 

				for(iz=23; iz<25; iz++)
				{
					if(('0'<=RxCardBuffer2[iz]) && (RxCardBuffer2[iz]<='9')) continue;
					break;
				}
				if(24 == iz)
				{
					ret_val = 1;
					jangbi.cdma.rssi = (RxCardBuffer2[23] - '0');
				}
				else if(25 == iz)
				{
					ret_val = 1;
					jangbi.cdma.rssi = (RxCardBuffer2[23] - '0')*10 + (RxCardBuffer2[24] - '0');
				}

				ret_val = 1;
				break;
			}
		//	break;
		}
		
		Keco_card_info_flag = 0;	//cdma data 받아오을때 

	} while(--iRetry && !ret_val);

	return ret_val;
}


int get_cdma_rssi_with_rx2cardbuffer()
{
	int iz;
	int ret_val;
	
	if(ret_val = get_cdma_with_rx2cardbuffer("AT$LGTRSSI?",  "OK", 2))
	{				
		ret_val = 0;
			
		for(iz=23; iz<25; iz++)
		{
			if(('0'<=RxCardBuffer2[iz]) && (RxCardBuffer2[iz]<='9')) continue;
			break;
		}
		if(24 == iz)
		{
			ret_val = 1;
			jangbi.cdma.rssi = (RxCardBuffer2[23] - '0');
		}
		else if(25 == iz)
		{
			ret_val = 1;
			jangbi.cdma.rssi = (RxCardBuffer2[23] - '0')*10 + (RxCardBuffer2[24] - '0');
		}
	}
		
	Keco_card_info_flag = 0;	//cdma data 받아오을때 

	return ret_val;
}

int get_cdma_gigiguk_old()	//_with_rx2cardbuffer()
{
	int ret_val;
	int i, iz;
	int	 iRetry;
	char szMsg[50];
	char szResponse[15];

	ret_val = 0;
	iRetry = 10;	//MAX_RETRY;

	do
	{
#ifdef	___LGUPLUS_AM200____	
		strcpy(szMsg, "AT$LGTLOC?");
#else
		strcpy(szMsg, "AT$LGTLVLOC?");	//lte
#endif
		RxCardCounter2 = 0;		
		Keco_card_info_flag = 1;	//cdma data 받아오을때 
		memset(RxCardBuffer2, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화

		cdma_send_cmd(szMsg, strlen(szMsg));

		sprintf(szResponse, "OK%c%c%c", 0x0d, 0x0a, 0);
		Delay_ms(10);	//2014.04.02  add because fail

		for(i=0; i<100; i++)
		{
			Delay_ms(2);

//for(iz=0; iz<RxCardCounter2; iz++) 	USART1_putchar(RxCardBuffer2[iz]);
//USART1_putchar('C');

			if(check_response(RxCardBuffer2, szResponse, RxCardCounter2, strlen(szResponse)))
			{	
				Keco_card_info_flag = 0;	//cdma data 받아오을때 

#ifdef	___LGUPLUS_AM200____	
				for(iz=21, i=0; (iz<RxCardCounter2-4) && (i<18); iz++, i++)
				{
					if(RxCardBuffer2[iz] < ' ') break;
					gszGigiguk[iz-21] = RxCardBuffer2[iz];
				}
				gszGigiguk[iz-21] = 0;
#else
				for(iz=24, i=0; (iz<RxCardCounter2-4) && (i<56); iz++, i++)
				{
					if(RxCardBuffer2[iz] < ' ') break;
					gszGigiguk[iz-24] = RxCardBuffer2[iz];
				}
				gszGigiguk[iz-24] = 0;
#endif
				ret_val = 1;

				break;
			}
		}
		Keco_card_info_flag = 0;	//cdma data 받아오을때 

	} while(--iRetry && !ret_val);

	return ret_val;
}


int get_cdma_gigiguk()	//_with_rx2cardbuffer()
{
	int i, iz;
	int ret_val;
	
#ifdef	___LGUPLUS_LGINNOTEK_PEGASUS____
	if(ret_val = get_cdma_with_rx2cardbuffer("AT$LGTLVLOC?",  "OK", 2))
#else
	if(ret_val = get_cdma_with_rx2cardbuffer("AT$LGTLOC?",  "OK", 2))
#endif
	{								
#ifdef	___LGUPLUS_LGINNOTEK_PEGASUS____				
		for(iz=24, i=0; (iz<RxCardCounter2-4) && (i<56); iz++, i++)
		{
			if(RxCardBuffer2[iz] < ' ') break;
			gszGigiguk[iz-24] = RxCardBuffer2[iz];
		}
		gszGigiguk[iz-24] = 0;
#else	//#ifdef	___LGUPLUS_AM200____	
		for(iz=21, i=0; (iz<RxCardCounter2-4) && (i<18); iz++, i++)
		{
			if(RxCardBuffer2[iz] < ' ') break;
			gszGigiguk[iz-21] = RxCardBuffer2[iz];
		}
		gszGigiguk[iz-21] = 0;
#endif
	}
		
	Keco_card_info_flag = 0;	//cdma data 받아오을때 

	return ret_val;
}


//116.123.106.53, 9940 - baichul data upload
void tcpip_send_baichul_data_gtech_20141020(char *ip, int port)
{
	int  iz;
	long l;	//because address is 32bit.
	static u32	dwSendingIndex;	
	static int	iDataLength;
	char server_ip[25];
	char filename[100];
	char szDateTime[20];

	++giSendingStageRetry;	//2015.8.20

	switch(iSendStage)
	{
		case 0:
			giSendingStageRetry = 0;	//2015.8.20
			if(giBaichulDataUploadFlag)
			{
				iStabeTwoWaitCount = 0;
				iSendStage = 20;	
			}
			else iSendStage = 300;	//보낼 데이터 없을 때 딜레이 없이 바로 복귀 2014.8.21  change 100 to 300;	//end

			if(giBaichulDataUploadRetry) --giBaichulDataUploadRetry;
			if(!giBaichulDataUploadRetry) giBaichulDataUploadFlag = 0;
			break;

		case 20:
			iSendStage = tcpip_send_case_20(ip, port, iStabeTwoWaitCount);
			break;
    	     
#ifdef ___LGUPLUS_AM200____
		case 30:
       		Stage_Three_Keco_Connection_send_cmd();	// tcp 접속  
			iSendStage = 31;
			break;

		case 31:
       		switch(Stage_Three_Keco_Connection_wait_response(0, 50))	//initialize waiting tcp 접속
			{
			 	case -1: //time out
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					Cdma_Force_Stop(); // 강제로 전화를 끊는다. 
					break;
			 	case -2: //or no carrier
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					break;
				case 1:
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 40;	//connection ok
					break;
				default:
					iSendStage = 32;	//dummy stage - waiting stage
					break;
			}
			break;

		case 32:
       		switch(Stage_Three_Keco_Connection_wait_response(1, 50))	//initialize waiting tcp 접속
			{
			 	case -1: //time out
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					Cdma_Force_Stop(); // 강제로 전화를 끊는다. 
					break;
			 	case -2: //or no carrier
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					break;
				case 1:
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 40;	//connection ok
					break;
				default:
					break;
			}
			break;
#endif	//#ifdef ___LGUPLUS_AM200____

		case 40:
			//monitoring
			get_jangbi_code();
			sprintf( filename,"[BCJB=000000],%s,%s,%04d%02d%02d%02d%02d%02d.dat", gszJangbiCode, jangbi.cdma.telno, jangbi.tm.year, jangbi.tm.month, jangbi.tm.date, jangbi.tm.hour, jangbi.tm.min, jangbi.tm.sec);
			Stage_Four_Data_Sending_send_msg(filename, strlen(filename));
/*
			sprintf( filename,"[BCJB=000001],%s,%s,11111111111111111111111111111", gszJangbiCode, jangbi.cdma.telno);
			Stage_Four_Data_Sending_send_msg(filename, strlen(filename));

			sprintf( filename,"[BCJB=999999],%s,%s,12345667,334325", gszJangbiCode, jangbi.cdma.telno);
			Stage_Four_Data_Sending_send_msg(filename, strlen(filename));
*/
			//iSendStage = 41;	//wait response
			iStage40Count = 0;	//2015.9,22 41,42 통합
			iSendStage = 42;	//wait response
			break;

		case 42:
       		switch(Stage_Four_Data_Sending_wait_response(iStage40Count++, 20))	//waiting server response
			{
			 	case -1: //time out
					iSendStage = 100;
					iStage100Count = 0;
//					USART1_puts("time out");
#ifdef ___LGUPLUS_AM200____
					break;
#endif
				case 1: //send ok
					iSendStage = 100;
					iStage100Count = 0;

					giBaichulDataUploadFlag = 0;
					giBaichulDataUploadRetry = 0;

					break;

				default:
					break;
			}
			break;

		case 100:
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
			iSendStage = 300;	//end
			break;
	} //switch(iSendStage)
}


void tcpip_send_event_gtech_20141017(char *ip, int port)
{
	int  iz;
	char server_ip[25];

	++giSendingStageRetry;	//2015.8.20
	
	switch(iSendStage)
	{
		case 0:
			giSendingStageRetry = 0;	//2015.8.20
			if(!giGtechSendingEventFlag) iSendStage = 300;	//보낼 데이터 없을 때 딜레이 없이 바로 복귀 2014.8.21  change 100 to 300;	//end
			else
			{
				//2014.10.8 1.32r1
				if(MAX_giGtechSendingEventRetry <= giGtechSendingEventRetry)	//data initialize
				{
					//make sending data
					gtech_monitoring_event_data_make();
				}
				else
				{
					// next port calc
					++giGtechServerCurrentPortIndex;	//2014.10.8
					if(MAX_GTECH_SERVER_PORT_QTY <= giGtechServerCurrentPortIndex) giGtechServerCurrentPortIndex = 0;
				}

				iStabeTwoWaitCount = 0;
				iSendStage = 20;	
				if(giGtechSendingEventRetry) --giGtechSendingEventRetry;
				if(!giGtechSendingEventRetry) giGtechSendingEventFlag = 0;
			}
			break;

		case 20:
			iSendStage = tcpip_send_case_20(ip, port, iStabeTwoWaitCount);
			break;

#ifdef ___LGUPLUS_AM200____
		case 30:
       		Stage_Three_Keco_Connection_send_cmd();	// tcp 접속  
			iSendStage = 31;
			break;

		case 31:
       		switch(Stage_Three_Keco_Connection_wait_response(0, STAGE_THREE_RETRY))	//initialize waiting tcp 접속
			{
			 	case -1: //time out
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					Cdma_Force_Stop(); // 강제로 전화를 끊는다. 
					break;
			 	case -2: //or no carrier
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					break;
				case 1:
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 40;	//connection ok
					break;
				default:
					iSendStage = 32;	//dummy stage - waiting stage
					break;
			}
			break;

		case 32:
       		switch(Stage_Three_Keco_Connection_wait_response(1, STAGE_THREE_RETRY))	//initialize waiting tcp 접속
			{
			 	case -1: //time out
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					Cdma_Force_Stop(); // 강제로 전화를 끊는다. 
					break;
			 	case -2: //or no carrier
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					break;
				case 1:
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 40;	//connection ok
					break;
				default:
				//	USART1_putchar('.');
					break;
			}
			break;
#endif	//#ifdef ___LGUPLUS_AM200____

		case 40:
			//monitoring
//			for(iz=0; iz<giSendingDataLength; iz++) USART1_putchar(gszSendingData[iz]);
//			Stage_Four_Data_Sending_send_msg(gszSendingData, giSendingDataLength);
			Stage_Four_Data_Sending_send_msg(gszSendingEventGtech, giSendingEventGtechLength);
			//iSendStage = 41;	//wait response
			iStage40Count = 0;	//2015.9,22 41,42 통합
			iSendStage = 42;	//wait response
			break;
		case 42:
       		switch(Stage_Four_Data_Sending_wait_response(iStage40Count++, 20))	//waiting server response
			{
			 	case -1: //time out
					iSendStage = 100;
					iStage100Count = 0;
//					USART1_puts("time out");
#ifdef ___LGUPLUS_AM200____
					Keco_card_info_flag = 0; 
					break;
#endif
				case 1: //send ok
					Keco_card_info_flag = 0;

					iSendStage = 100;
					iStage100Count = 0;

//					//keco response de
					for(iz=0; iz<16; iz++) USART1_putchar(RxCardBuffer2[iz]);
//					if(check_response(RxCardBuffer2, "R40020", RxCardCounter2, 7)) //send ok
//					if(check_response(RxCardBuffer2, "R400", RxCardCounter2, 4)) //send ok test
					{
						giSendingEventGtechLength = 0;	
						giGtechSendingEventRetry = MAX_giGtechSendingEventRetry;	//2014.8.25
						giGtechSendingEventFlag = 0;	//2014.8.25
					}
					break;
				default:
					break;
			}
			break;

		case 100:
			Keco_card_info_flag = 0;
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

void tcpip_send_c1_data_lguplus_20150415(char *ip, int port)
{
#ifdef __________________LGUPLUS_______

	int  iz;
	long laddr;	//because address is 32bit.
	static u32	dwSendingIndex;	
	static int	iDataLength;
	char server_ip[25];
								
	++giSendingStageRetry;	//2015.8.20
	
	switch(iSendStage)
	{
		case 0:
			giSendingStageRetry = 0;	//2015.8.20
			Fsmc_Enable();
//USART1_puts("case 0: tcpip_send_c1_data_gtech_20141020");
			for(laddr=0; laddr<7000; laddr++) // Nor_Flash_Counter
			{
//				USART1_putchar(l+'a');

				Nor_data_reading(laddr); // nor flash 
//				if(baichul_info_nor[1].Status_keco_new == 7777) // 새로운 데이타가  들어왓냐  
				{

//USART1_puts("case 6 :0  ");

					if(baichul_info_nor[1].Status_lguplus == 0x7777)// 데이터를 모니터링으로 보내지 않았으면 
					{

//USART1_puts("case 6 :01 ");

						dwSendingIndex = laddr;	 // nor 에서 몇번째 데이타냐 ?
						break;
					}
				} //if(baichul_info_nor[1].Status_keco_new==7777) // 새로운 데이타가  들어왓냐  
			} //for(i=0;i<Nor_Flash_Counter;i++) // Nor_Flash_Counter
			Fsmc_Disable();

			if(laddr < 7000)
			{	iSendStage = 10;// 보내야 할 테이타가 있다. 
				gwSendLguplusCount = laddr;	////for monitoring information
			}
			else iSendStage = 300;	//보낼 데이터 없을 때 딜레이 없이 바로 복귀 2014.8.21  change 100 to 300;	//end

			gwBaichulCount = Nor_Flash_Counter;	//for monitoring information

			//add 2014.10.20
			if(gwBaichulCount < gwSendLguplusCount) gwBaichulCount += 7000;

//			if(giKecoSendingC1BaichulRetry) --giKecoSendingC1BaichulRetry;
//			if(!giKecoSendingC1BaichulRetry) giKecoSendingC1BaichulFlag = 0;

			break;

		case 10:
//USART1_puts("case 10: tcpip_send_c1_data_gtech_20141020");
//			USART1_putchar(iSendStage+'s');
			get_jangbi_code();
			get_lguplus_jangbi_code();	//2015.4.23
//			iz = Stage_One_DataLoading_New_Lguplus("W2S-001-01-1", baichul_info_nor[1].card1_uid, (unsigned int)(baichul_info_nor[1].weight),baichul_info_nor[1].date);
			iz = Stage_One_DataLoading_New_Lguplus(LGU_TEST_JANGBI_CODE, baichul_info_nor[1].card1_uid, (unsigned int)(baichul_info_nor[1].weight),baichul_info_nor[1].date);
			if(iz <= 0)	//data error, 장비코드 error
			{
//				USART1_putchar(10+iz+'0');
				iSendStage = 100;

		   		Fsmc_Enable();
				Delay_ms(1);
		   		FSMC_NOR_WriteHalfWord(NOR_BAICHUL_BASE+(dwSendingIndex*64)+48,0x5555); //장비코드 에러
		   		Fsmc_Disable();
			}
			else
			{
				iStabeTwoWaitCount = 0;
				iSendStage = 20;
			}
			break;

		case 20:
			iSendStage = tcpip_send_case_20(ip, port, iStabeTwoWaitCount);
			break;
    	     
#ifdef ___LGUPLUS_AM200____
		case 30:
//USART1_puts("case 30: tcpip_send_c1_data_gtech_20141020");
       		Stage_Three_Keco_Connection_send_cmd();	// tcp 접속  
			iSendStage = 31;
			break;

		case 31:
       		switch(Stage_Three_Keco_Connection_wait_response(0, STAGE_THREE_RETRY))	//initialize waiting tcp 접속
			{
			 	case -1: //time out
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					Cdma_Force_Stop(); // 강제로 전화를 끊는다. 
					break;
			 	case -2: //or no carrier
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					break;
				case 1:
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 40;	//connection ok
					break;
				default:
					iSendStage = 32;	//dummy stage - waiting stage
					break;
			}
			break;

		case 32:
       		switch(Stage_Three_Keco_Connection_wait_response(1, STAGE_THREE_RETRY))	//initialize waiting tcp 접속
			{
			 	case -1: //time out
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					Cdma_Force_Stop(); // 강제로 전화를 끊는다. 
					break;
			 	case -2: //or no carrier
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					break;
				case 1:
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 40;	//connection ok
					break;
				default:
					break;
			}
			break;
#endif	//#ifdef ___LGUPLUS_AM200____

		case 40:
			//monitoring
//			for(iz=0; iz<giSendingDataLength; iz++) USART1_putchar(gszSendingData[iz]);
			Stage_Four_Data_Sending_send_msg(gszSendingData, giSendingDataLength);
			//iSendStage = 41;	//wait response
			iStage40Count = 0;	//2015.9,22 41,42 통합
			iSendStage = 42;	//wait response
			break;
		case 42:
       		switch(Stage_Four_Data_Sending_wait_response(iStage40Count++, STAGE4_WAIT_COUNTER))	//waiting server response
			{
			 	case -1: //time out
					iSendStage = 100;
					iStage100Count = 0;

//					USART1_puts("time out");
#ifdef ___LGUPLUS_AM200____
					Keco_card_info_flag = 0; 
					break;
#endif	  
				case 1: //send ok
					Keco_card_info_flag = 0;
					iSendStage = 100;
					iStage100Count = 0;

					giKecoSendingC1BaichulFlag = 0;
					giKecoSendingC1BaichulRetry = 0;

					//keco response de
					if(check_response(RxCardBuffer2, "A200000", RxCardCounter2, 7)) //send ok
					{
				   		Fsmc_Enable();
				   		FSMC_NOR_WriteHalfWord(NOR_BAICHUL_BASE+(dwSendingIndex*64)+48,0x3333); // 정상적으로 보냈으면 9999
				   		Fsmc_Disable();
					}
					else
					{
				   		Fsmc_Enable();
				   		FSMC_NOR_WriteHalfWord(NOR_BAICHUL_BASE+(dwSendingIndex*64)+48,0x6066); //기타 에러
				   		Fsmc_Disable();
					}
					break;

				default:
					break;
			}
			break;

		case 100:
			Cdma_Force_Stop(); // 강제로 전화를 끊는다. 

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
			iSendStage = 300;	//end
			break;
	} //switch(iSendStage)
#endif	//ifdef __________________LGUPLUS_______

}					  

void tcpip_send_c4_status_lguplus_20150414(char *ip, int port)
{
#ifdef __________________LGUPLUS_______
	int  iz;
	char server_ip[25];

	++giSendingStageRetry;	//2015.8.20
	
	switch(iSendStage)
	{
		case 0:
			giSendingStageRetry = 0;	//2015.8.20
			if(!giLguplusSendingStatusFlag) iSendStage = 300;	//보낼 데이터 없을 때 딜레이 없이 바로 복귀 2014.8.21  change 100 to 300;	//end
			else
			{
				//2014.10.8 1.32r1
				if(MAX_giLguplusSendingStatusRetry <= giLguplusSendingStatusRetry)	//data initialize
				{
					//make sending data
					lguplus_monitoring_status_data_make();
				}

				iStabeTwoWaitCount = 0;
				iSendStage = 20;	
				if(giLguplusSendingStatusRetry) --giLguplusSendingStatusRetry;
				if(!giLguplusSendingStatusRetry) giLguplusSendingStatusFlag = 0;
			}
			break;

		case 20:
			iSendStage = tcpip_send_case_20(ip, port, iStabeTwoWaitCount);
			break;
    	     
#ifdef ___LGUPLUS_AM200____
		case 30:
       		Stage_Three_Keco_Connection_send_cmd();	// tcp 접속  
			iSendStage = 31;
			break;

		case 31:
       		switch(Stage_Three_Keco_Connection_wait_response(0, STAGE_THREE_RETRY))	//initialize waiting tcp 접속
			{
			 	case -1: //time out
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					Cdma_Force_Stop(); // 강제로 전화를 끊는다. 
					break;
			 	case -2: //or no carrier
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					break;
				case 1:
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 40;	//connection ok
					break;
				default:
					iSendStage = 32;	//dummy stage - waiting stage
					break;
			}
			break;

		case 32:
       		switch(Stage_Three_Keco_Connection_wait_response(1, STAGE_THREE_RETRY))	//initialize waiting tcp 접속
			{
			 	case -1: //time out
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					Cdma_Force_Stop(); // 강제로 전화를 끊는다. 
					break;
			 	case -2: //or no carrier
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					break;
				case 1:
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 40;	//connection ok
					break;
				default:
				//	USART1_putchar('.');
					break;
			}
			break;
#endif	//#ifdef ___LGUPLUS_AM200____

		case 40:
			//monitoring
//			for(iz=0; iz<giSendingDataLength; iz++) USART1_putchar(gszSendingData[iz]);
//			Stage_Four_Data_Sending_send_msg(gszSendingData, giSendingDataLength);
			Stage_Four_Data_Sending_send_msg(gszSendingStatusLguplus, giSendingStatusLguplusLength);
			//iSendStage = 41;	//wait response
			iStage40Count = 0;	//2015.9,22 41,42 통합
			iSendStage = 42;	//wait response
			break;
		case 42:
       		switch(Stage_Four_Data_Sending_wait_response(iStage40Count++, 20))	//waiting server response
			{
			 	case -1: //time out
					iSendStage = 100;
					iStage100Count = 0;
//					USART1_puts("time out");
#ifdef ___LGUPLUS_AM200____
					Keco_card_info_flag = 0; 
					break;
#endif
				case 1: //send ok
					Keco_card_info_flag = 0;

					iSendStage = 100;
					iStage100Count = 0;

//					//keco response de
					for(iz=0; iz<16; iz++) USART1_putchar(RxCardBuffer2[iz]);
//					if(check_response(RxCardBuffer2, "R40020", RxCardCounter2, 7)) //send ok
//					if(check_response(RxCardBuffer2, "R400", RxCardCounter2, 4)) //send ok test
					{
						giSendingStatusLguplusLength = 0;	
						giLguplusSendingStatusRetry = MAX_giLguplusSendingStatusRetry;	//2014.8.25
						giLguplusSendingStatusFlag = 0;	//2014.8.25
					}
					break;
				default:
					break;
			}
			break;

		case 100:
			Keco_card_info_flag = 0;
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
#endif	//ifdef __________________LGUPLUS_______
}					  

	  

/* struct
// key, funciton
struct
{
	char *key;
		 *func;
}	
*/


/*
	event log monitoring
*/
//birth 2014.10.17
void Send_to_Gtech_Event(char *content, char *result) //발생한 event를 전송한다.
{
	strcpy(gszEventContent, content);
	strcpy(gszEventResult, result);
	giGtechSendingEventRetry = MAX_giGtechSendingEventRetry;
	giGtechSendingEventFlag = 1;
}

//birth 2014.10.17
void gtech_monitoring_event_data_make()
{
	int iz;
	unsigned long CRC16 = 0;
	unsigned short wCRC16;
	char szCRC16Buf[10];

	for(iz=0; iz<11; iz++)
	{
		if(('0'<=jangbi.cdma.telno[iz]) && (jangbi.cdma.telno[iz]<='9')) continue;	//전화번호가 숫자로만 이루어졌는지 확인
		get_cdma_telno();
	}
	get_jangbi_code();

	// 버전 1
	sprintf( gszSendingEventGtech,"EVENT|%s|0000|%s|%s|%s|%s|%s|%02d%02d%02d%02d%02d%02d|%04X|%c", \
	 		gszMonitoringProtocolVersion
			,gszModel, gszJangbiCode, jangbi.cdma.telno
			,gszEventContent, gszEventResult
			,jangbi.tm.year, jangbi.tm.month, jangbi.tm.date, jangbi.tm.hour, jangbi.tm.min, jangbi.tm.sec \
			,CRC16
			,0x04	//EOT
	);

	//데이터 길이 계산
	giSendingEventGtechLength = strlen(gszSendingEventGtech); //2014.8.21
	gszSendingEventGtech[9] = giSendingEventGtechLength / 1000 + '0';
	gszSendingEventGtech[10] = (giSendingEventGtechLength % 1000) / 100 + '0';
	gszSendingEventGtech[11] = (giSendingEventGtechLength % 100) / 10 + '0';
	gszSendingEventGtech[12] = giSendingEventGtechLength % 10 + '0';
	
	//CRC 계산
	wCRC16 = CalcCRC16(gszSendingEventGtech, 0, (giSendingEventGtechLength-5)/2, szCRC16Buf);
	//CRC
	gszSendingEventGtech[giSendingEventGtechLength-6] = szCRC16Buf[2];
	gszSendingEventGtech[giSendingEventGtechLength-5] = szCRC16Buf[3];
	gszSendingEventGtech[giSendingEventGtechLength-4] = szCRC16Buf[0];
	gszSendingEventGtech[giSendingEventGtechLength-3] = szCRC16Buf[1];
	gszSendingEventGtech[giSendingEventGtechLength] = 0;

	//test printer
////	USART1_puts(gszSendingEventGtech);
} //end of void gtech_monitoring_Event_data_make()


/*
	lguplus web monitoring send baichul data
*/
//birth 2015.4.15
int Stage_One_DataLoading_New_Lguplus(char *jangbi_code,  char *card_no, unsigned int uWeight,char *data_time)
{
	int	 i, iz;
	int  iWeight;
	int  iCost;

	char szCost[6+1];
	char szWeight[6+1];
	char szDataLength[10];
	char szUID[16+1];
	char szBaichulTime[14+1];

	char szData[256];
	char szCData[256];

	char szCRC16Buf[10];
	unsigned short wCRC16;

	int	 jangbi_code_length;
	int	 iDataLength;

	//장비코드 정합성 검사
	iz = GetJangbiAreaCode();
	if(iz < 0) return iz;

	//데이터 생성	
	//CARD 번호 
	jangbi_code_length = strlen(baichul_info_nor[1].card1_uid);
	for(i=0; i<jangbi_code_length; i++) gszCard_no[i] = baichul_info_nor[1].card1_uid[i];
	gszCard_no[i] = 0;
	gszCard_no[8] = 0;

	//카드번호 정합성 검사	2014.11.10 ver 1.32s3
	iz = CheckCardNo(gszCard_no, 8);
	jangbi_code_length = GetCardId();
	for(i=0; i<jangbi_code_length; i++) szUID[i] = gszCard_no[i];
	if(iz < 0)
	{
		szUID[i++] = 'E';
		szUID[i++] = 'R';
		szUID[i++] = 'R';
	}
	for( ;i<16; i++) szUID[i] = ' ';
	szUID[i] = 0;

	//배출량 
	iWeight = baichul_info_nor[1].weight;
//	sprintf(szWeight, "%d", iWeight);

	//금액 항상
	iCost = (iWeight * gxlCostPerOnekilo)/1000;
//	sprintf(szCost, "%d", iCost);

	//배출일시 yyyymmddhhmmss
	for(i=0; i<14; i++) szBaichulTime[i] = baichul_info_nor[1].date[i];
	szBaichulTime[i] = 0;


	for(iz=0; iz<11; iz++)
	{
		if(('0'<=jangbi.cdma.telno[iz]) && (jangbi.cdma.telno[iz]<='9')) continue;	//전화번호가 숫자로만 이루어졌는지 확인
		get_cdma_telno();
	}

	get_jangbi_code();
	get_lguplus_jangbi_code();	//2015.4.23
	//012000A1GB=REC TID=000-000-00-3 TEL=01212341234 TIME=20150402173701 UID=1234567890123456 RESULT=9000 WEIGHT=10 AMOUNT=10
	sprintf( szData,"000000A1GB=REC TID=%s TEL=%s TIME=%s UID=1234567890123456 RESULT=9000 WEIGHT=%d AMOUNT=%d", \
			LGU_TEST_JANGBI_CODE //"W2S-001-01-1"	//gszJangbiCode
			,jangbi.cdma.telno
			,szBaichulTime
			,iWeight, iCost	
//			,jangbi.tm.year, jangbi.tm.month, jangbi.tm.date, jangbi.tm.hour, jangbi.tm.min, jangbi.tm.sec
//			,jangbi.tm.min*60+jangbi.tm.sec, ((jangbi.tm.min*60+jangbi.tm.sec))/1000*gxlCostPerOnekilo
	);
	//szUID, gxlCostPerOnekilo

	//전송할 데이터 길이
	iDataLength = strlen(szData);
	strcat(szData, szCData);
	giSendingBaichulLguplusLength = strlen(szData);
	sprintf(szDataLength, "%04d", giSendingBaichulLguplusLength);
	szData[0] = szDataLength[0];
	szData[1] = szDataLength[1];
	szData[2] = szDataLength[2];
	szData[3] = szDataLength[3];

	for(iz=0; iz<giSendingBaichulLguplusLength; iz++) gszSendingBaichulLguplus[iz] = szData[iz];

	//MoNiToRiNg
	for(iz=0; iz<giSendingBaichulLguplusLength; iz++) USART1_putchar(gszSendingBaichulLguplus[iz]);

	return giSendingBaichulLguplusLength;
}


//birth 2015.4.13
void Send_to_Lguplus_Status() //Status를 전송한다.
{
/*
	giLguplusSendingStatusRetry = MAX_giLguplusSendingStatusRetry;
	giLguplusSendingStatusFlag = 1;
*/
}

//birth 2015.4.13
void lguplus_monitoring_status_data_make()
{
#ifdef __________________LGUPLUS_______
	int iz;

	for(iz=0; iz<11; iz++)
	{
		if(('0'<=jangbi.cdma.telno[iz]) && (jangbi.cdma.telno[iz]<='9')) continue;	//전화번호가 숫자로만 이루어졌는지 확인
		get_cdma_telno();
	}
	get_jangbi_code();
	get_lguplus_jangbi_code();	//2015.4.23
	//053300A1GB=CFG IDX=627 TEL=01212341234 TIME=20150401184501 VER=20130423 S/N=123456789 MODE=TEST TID=W2S-001-01-1 UID= STA=GNGGGGGGGGGGGGGG CAP=0 FREE=100913152 ADMIN= VIN=12.18 TEMP=26 RSSI=60 STM=TEST BAL1=100 BAL5=500 RATE=0 RESEND=3 SILENT=20,6,70,40 CDMAID=0 CLIENT=0 OVF=300,150,200,10 IGNW=0 WMENT=k IRCHKW=0 FULL=85000 EMPTY=4800 DROPT=30 IRCHK=y RECLOSE=3 RFLEDT=18,7 TOPLEDT=18,7,0 PMENT=n DONGHO=y FRCHK=y SPRAYT=3,11,0,24 SOLI=1800 SOLT=10 SOLO=20 OPTVER=V1.0.0 8051VER=19 KECOT=1 KECOI=24 KECON=20 KECOR=1 KECDB=0 RECDB=0
	//012000A1GB=REC TID=000-000-00-3 TEL=01212341234 TIME=20150402173701 UID=1234567890123456 RESULT=9000 WEIGHT=10 AMOUNT=10
/**
	sprintf( gszLguplusStatusContent,"000000A1GB=CFG TEL=%s TIME=%02d%02d%02d%02d%02d%02d VER=20150000 S/N=123456789 MODE=TEST TID=%s UID= STA=GGGGGGGGGGGGGGGG CAP=0 FREE=100913152 ADMIN= VIN=12.18 TEMP=26 RSSI=60 STM=TEST BAL1=100 BAL5=500 RATE=0 RESEND=3 SILENT=20,6,70,40 CDMAID=0 CLIENT=0 OVF=300,150,200,10 IGNW=0 WMENT=k IRCHKW=0 FULL=85000 EMPTY=4800 DROPT=30 IRCHK=y RECLOSE=3 RFLEDT=18,7 TOPLEDT=18,7,0 PMENT=n DONGHO=y FRCHK=y SPRAYT=3,11,0,24 SOLI=1800 SOLT=10 SOLO=20 OPTVER=V1.0.0 8051VER=19 KECOT=1 KECOI=24 KECON=20 KECOR=1 KECDB=0 RECDB=0", \
			jangbi.cdma.telno
			,jangbi.tm.year, jangbi.tm.month, jangbi.tm.date, jangbi.tm.hour, jangbi.tm.min, jangbi.tm.sec \
			,gszJangbiCode 
	);
**/
//장비뚜껑열림	수거통유무	카드끼임	  카드리더기이상	전자저울이상	수거통교체요망	모터이상	통신이상
//LGUPLUS_TEST_DEVICE	"W2S-001-01-1"
//	sprintf( gszSendingStatusLguplus,"000000A1GB=CFG IDX=627 TEL=%s TIME=%02d%02d%02d%02d%02d%02d VER=20150000 S/N=123456789 MODE=TEST TID=W2S-001-01-1 UID= STA=GGGGGGGGGGGGGGGG CAP=0 FREE=100913152 ADMIN= VIN=12.18 TEMP=26 RSSI=60 STM=TEST BAL1=100 BAL5=500 RATE=0 RESEND=3 SILENT=20,6,70,40 CDMAID=0 CLIENT=0 OVF=300,150,200,10 IGNW=0 WMENT=k IRCHKW=0 FULL=85000 EMPTY=4800 DROPT=30 IRCHK=y RECLOSE=3 RFLEDT=18,7 TOPLEDT=18,7,0 PMENT=n DONGHO=y FRCHK=y SPRAYT=3,11,0,24 SOLI=1800 SOLT=10 SOLO=20 OPTVER=V1.0.0 8051VER=19 KECOT=1 KECOI=24 KECON=20 KECOR=1 KECDB=0 RECDB=0", 
//	sprintf( gszSendingStatusLguplus,"000000A1GB=CFG TID=W2S-001-01-1 TEL=01222039999 TIME=%02d%02d%02d%02d%02d%02d VER=20150000 S/N=123456789 MODE=TEST UID= STA=GGGGGGGGGGGGGGGG CAP=12 FREE=100913152 ADMIN= VIN=12.18 TEMP=26 RSSI=60 STM=TEST BAL1=100 BAL5=500 RATE=0 RESEND=3 SILENT=20,6,70,40 CDMAID=0 CLIENT=0 OVF=300,150,200,10 IGNW=0 WMENT=k IRCHKW=0 FULL=85000 EMPTY=4800 DROPT=30 IRCHK=y RECLOSE=3 RFLEDT=18,7 TOPLEDT=18,7,0 PMENT=n DONGHO=y FRCHK=y SPRAYT=3,11,0,24 SOLI=1800 SOLT=10 SOLO=20 OPTVER=V1.0.0 8051VER=19 KECOT=1 KECOI=24 KECON=20 KECOR=1 KECDB=0 RECDB=0 IDX=1", 
//ok	sprintf( gszSendingStatusLguplus,"000000A1GB=CFG TID=%s TEL=%s TIME=%02d%02d%02d%02d%02d%02d VER=20150000 S/N=123456789 MODE=TEST UID= STA=GGGGGGGNGGGGGGGG CAP=12 RATE=%d KECOUNSEND=21 UNSEND=0 FREE=100913152 ADMIN= VIN=12.18 TEMP=26 RSSI=60 STM=TEST BAL1=100 BAL5=500 RATE=41 RESEND=3 SILENT=20,6,70,40 CDMAID=0 CLIENT=0 OVF=300,150,200,10 IGNW=0 WMENT=k IRCHKW=0 FULL=85000 EMPTY=4800 DROPT=30 IRCHK=y RECLOSE=3 RFLEDT=18,7 TOPLEDT=18,7,0 PMENT=n DONGHO=y FRCHK=y SPRAYT=3,11,0,24 SOLI=1800 SOLT=10 SOLO=20 OPTVER=V1.0.0 8051VER=19 KECOT=1 KECOI=24 KECON=20 KECOR=1 KECDB=0 RECDB=0 IDX=1", 
	sprintf( gszSendingStatusLguplus,"000000A1GB=CFG TID=%s TEL=%s TIME=%02d%02d%02d%02d%02d%02d VER=20150000 S/N=123456789 MODE=TEST UID= STA=%c%c%c%c%c%c%cGGGGGGGGG CAP=%d RATE=%d KECOUNSEND=%d UNSEND=0 RSSI=%d", \
			LGU_TEST_JANGBI_CODE
			,jangbi.cdma.telno
			,jangbi.tm.year, jangbi.tm.month, jangbi.tm.date, jangbi.tm.hour, jangbi.tm.min, jangbi.tm.sec
			,Left_BasketDoor_Status, Left_BasketBeing_Status, Left_Card_Status, Left_CardReader_Status, Left_Scale_Status, Left_BasketFull_Status, Left_Moter_Status
			,gxlGrossWeight
			,gxlCostPerOnekilo
			,(gwBaichulCount-gwSendCount)
			,jangbi.cdma.rssi
	);

	//데이터 길이 계산
	giSendingStatusLguplusLength = strlen(gszSendingStatusLguplus); //2014.8.21
	gszSendingStatusLguplus[0] =  giSendingStatusLguplusLength / 1000 + '0';
	gszSendingStatusLguplus[1] = (giSendingStatusLguplusLength % 1000) / 100 + '0';
	gszSendingStatusLguplus[2] = (giSendingStatusLguplusLength % 100) / 10 + '0';
	gszSendingStatusLguplus[3] =  giSendingStatusLguplusLength % 10 + '0';
	
	//test printer
////	USART1_puts(gszSendingStatusLguplus);
#endif	//ifdef __________________LGUPLUS_______
} //end of void gtech_monitoring_Event_data_make()



void modem_reset()
{
	switch(iSendStage)
	{
		case 0:
			if(gbModemReset) //modem reset
			{
				gbModemReset = 0;
				cdma_send_cmd("AT$LGTRESET", 11);	//2014.10.27
				iSendStage = 100;	//end
				iStage100Count = 0;
			}
			else iSendStage = 300;	//end
			break;

		case 100:

   			switch(Stage_Idle(iStage100Count++, 10000))	//waiting send idle
			{
				case 1:
					iSendStage = 300;	//end
					Send_to_Gtech_Event("MODEM_RESET", "DONE"); //발생한 event를 전송한다.
					break;
				default:
					break;
			}
			break;

		default:
			iSendStage = 300;
			break;
	}
}


void tcpip_send_CIP_20140808(char *ip, int port)
{
	int  iz;
	char server_ip[25];

	char s[300];

	++giSendingStageRetry;	//2015.8.20
	switch(iSendStage)
	{
		case 0:		//giCipSendingRetry
			giSendingStageRetry = 0;	//2015.8.20
			if(!giCipSendingFlag) iSendStage = 300;	//보낼 데이터 없을 때 딜레이 없이 바로 복귀 2014.8.21  change 100 to 300;	//end
			else
			{
				//make sending data
				cip_monitoring_data_make();

//				sprintf(s, "1:%d,%d", giCipSendingFlag, iSendStage);
//				USART1_puts(s);

				if(giCipSendingFlag == 5)
				{
					iSendStage = 300;
					giCipSendingFlag = 0;
					break;
				}

//				sprintf(s, "2:%d,%d", giCipSendingFlag, iSendStage);
//				USART1_puts(s);

				iStabeTwoWaitCount = 0;
				iSendStage = 20;	
				if(giCipSendingRetry) --giCipSendingRetry;
				else giCipSendingFlag = 0;
			}
			break;

		case 20:
			iSendStage = tcpip_send_case_20(ip, port, iStabeTwoWaitCount);
			break;

#ifdef	___LGUPLUS_AM200____
		case 30:
//USART1_puts("tcpip_send_c4_status_gtech_20140808  case 30");

       		Stage_Three_Keco_Connection_send_cmd();	// tcp 접속  
			iSendStage = 31;
			break;

		case 31:
       		switch(Stage_Three_Keco_Connection_wait_response(0, STAGE_THREE_RETRY))	//initialize waiting tcp 접속
			{
			 	case -1: //time out
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					Cdma_Force_Stop(); // 강제로 전화를 끊는다. 
					break;
			 	case -2: //or no carrier
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					break;
				case 1:
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 40;	//connection ok
					break;
				default:
					iSendStage = 32;	//dummy stage - waiting stage
					break;
			}
			break;

		case 32:
       		switch(Stage_Three_Keco_Connection_wait_response(1, STAGE_THREE_RETRY))	//initialize waiting tcp 접속
			{
			 	case -1: //time out
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					Cdma_Force_Stop(); // 강제로 전화를 끊는다. 
					break;
			 	case -2: //or no carrier
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 100;	//20;
					break;
				case 1:
					Keco_card_info_flag = 0; //cdma data 받아오을때 
					iSendStage = 40;	//connection ok
					break;
				default:
				//	USART1_putchar('.');
					break;
			}
			break;
#endif

		case 40:				//	gszSendingStatusCip

//USART1_puts("tcpip_send_c4_status_gtech_20140808  case 40");

			//monitoring
//			for(iz=0; iz<giSendingDataLength; iz++) USART1_putchar(gszSendingData[iz]);
//			Stage_Four_Data_Sending_send_msg(gszSendingData, giSendingDataLength);
//			Stage_Four_Data_Sending_send_msg(gszSendingStatusCip, giSendingStatusCipLength);
			Stage_Four_Data_Sending_send_msg(gszSending, giSendingLength);
			//iSendStage = 41;	//wait response
			iStage40Count = 0;	//2015.9,22 41,42 통합
			iSendStage = 42;	//wait response
			break;
		case 42:	//giCipSendingStatusRetry
       		switch(Stage_Four_Data_Sending_wait_response(iStage40Count++, 20))	//waiting server response
			{
			 	case -1: //time out
					iSendStage = 100;
					iStage100Count = 0;
//					USART1_puts("time out");
#ifdef ___LGUPLUS_AM200____
					Keco_card_info_flag = 0; 
					break;
#endif
				case 1: //send ok
					Keco_card_info_flag = 0;

					iSendStage = 100;
					iStage100Count = 0;

//					//keco response de
					for(iz=0; iz<16; iz++) USART1_putchar(RxCardBuffer2[iz]);
//					if(check_response(RxCardBuffer2, "R40020", RxCardCounter2, 7)) //send ok
//					if(check_response(RxCardBuffer2, "R400", RxCardCounter2, 4)) //send ok test
					{
						giSendingLength = 0;	
						giCipSendingRetry = MAX_giCipSendingRetry;	//2014.8.25
						giCipSendingFlag = 0;	//2014.8.25
					}
					break;
				default:
					break;
			}
			break;

		case 100:
			Keco_card_info_flag = 0;
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



int echo_with_rx2cardbuffer()
{
	int ret_val;
	int i, iz;

	do
	{
		RxCardCounter2 = 0;		
		memset(RxCardBuffer2, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화

        cdma_send_cmd("ATZ", 3); 	//2014.10.21

		Delay_ms(10);	//2014.04.02  add because fail

		for(i=0; i<RxCardCounter2; i++)
		{
				USART1_putchar(RxCardBuffer2[i]);
		}
	} while(1);


	return ret_val;
}


int SocketClose(int init)
{
	int ret_val;
	int iRetry;
	char szMsg[40];
	char szRes[20];
	int	i;


	ret_val = init;
	iRetry = MAX_RETRY;

	{
	strcpy(szMsg, "+++");
	sprintf(szRes, "OK%c%c", 0x0d, 0x0a);
	do
	{
///			RxCardCounter2 = 0;
///			memset( RxCardBuffer2, 0, RXCARDBUFFER2SIZE);
			cdma_send_msg(szMsg, strlen(szMsg));

//			Delay_ms(10);	//2014.04.02  add because fail

			for(i=0; i<100; i++)
			{
				Delay_ms(11);

//				if(RxBuffer2[RxCounter2-4] == 'O' && RxBuffer2[RxCounter2-3] == 'K')
				if(check_response(RxCardBuffer2, szRes, RxCardCounter2, strlen(szRes))) //send ok test
				{	
///					ret_val = 1;
///					break;
				}
			}
		} while(--iRetry && !ret_val);
	}

#ifdef ___TCPIP_TEST_VIEW___TIMESTAMP
	Delay_ms(100);
	get_cdma_time();
	sprintf(szMsg, "+++_%1d%1d%1d%02d ", iRetry, ret_val, send_msg_flag, jangbi.tm.sec-start_sec);
	USART1_print(szMsg);
#endif	//#ifdef ___TCPIP_TEST_VIEW___


	/*
		AT$LGTTCPCL

		OK
	*/

//	if(ret_val && send_msg_flag != 1)	//if still connected
	{

//		USART1_puts("0: AT$LGTTCPCL");

//		ret_val = 0;
		iRetry = MAX_RETRY;
		strcpy(szMsg, "AT$LGTTCPCL");
		sprintf(szRes, "OK%c%c", 0x0d, 0x0a);
		do
		{
///			RxCardCounter2 = 0;
///			memset( RxCardBuffer2, 0, RXCARDBUFFER2SIZE);
//			for(i=0; i<MAX_CDMA_RECEIVED; i++) szCdmaReceived[i] = 0;
			cdma_send_cmd(szMsg, strlen(szMsg));
#ifdef ___TEST_VIEW___
	    	GLCD_string(4,7, &szMsg[0]);
#endif	//#ifdef ___TEST_VIEW___

//			Delay_ms(10);	//2014.04.02  add because fail

			for(i=0; i<10; i++)
			{
				Delay_ms(11);

//				if(RxBuffer2[RxCounter2-4] == 'O' && RxBuffer2[RxCounter2-3] == 'K')
///				if(check_response(RxCardBuffer2, szRes, RxCardCounter2, strlen(szRes))) //send ok test
				{	
///					ret_val = 1;
///					break;
				}
			}
		} while(--iRetry && !ret_val);
	} 

#ifdef ___TCPIP_TEST_VIEW___TIMESTAMP_NO
	Delay_ms(100);
	get_cdma_time();
	sprintf(szMsg, "AT$LGTTCPCL_%1d%1d%1d%02d ", iRetry, ret_val, send_msg_flag, jangbi.tm.sec-start_sec);
	USART1_print(szMsg);
#endif	//#ifdef ___TCPIP_TEST_VIEW___


	/*
		AT$LGTPREL

		NO CARRIER
	*/
//	if(ret_val)
	{

//		USART1_puts("0: AT$LGTPREL");

//		ret_val = 0;
		iRetry = MAX_RETRY;
		strcpy(szMsg, "AT$LGTPREL");
		sprintf(szRes, "CARRIER");
		do
		{
///			RxCardCounter2 = 0;
///			memset( RxCardBuffer2, 0, RXCARDBUFFER2SIZE);
			cdma_send_cmd(szMsg, strlen(szMsg));
#ifdef ___TEST_VIEW___
	    	GLCD_string(4,7, &szMsg[0]);
#endif	//#ifdef ___TEST_VIEW___

//			Delay_ms(10);	//2014.04.02  add because fail

			for(i=0; i<10; i++)
			{
				Delay_ms(11);

//				if(RxBuffer2[RxCounter2-8] == 'C' && RxBuffer2[RxCounter2-8] == 'A' && RxBuffer2[RxCounter2-7] == 'R' && RxBuffer2[RxCounter2-6] == 'R'
//				   && RxBuffer2[RxCounter2-5] == 'I' && RxBuffer2[RxCounter2-4] == 'E' && RxBuffer2[RxCounter2-3] == 'R')
///				if(check_response(RxCardBuffer2, szRes, RxCardCounter2, strlen(szRes))) //send ok test
				{	
///					ret_val = 1;
///					break;
				}
			}
		} while(--iRetry && !ret_val);
	} 

#ifdef ___TCPIP_TEST_VIEW___TIMESTAMP_NO
	Delay_ms(100);
	get_cdma_time();
	sprintf(szMsg, "AT$LGTPREL_%1d%1d%02d ", iRetry, ret_val, jangbi.tm.sec-start_sec);
	USART1_print(szMsg);
#endif	//#ifdef ___TCPIP_TEST_VIEW___




#ifdef ___TCPIP_TEST_VIEW___TIMESTAMP
	Delay_ms(100);
	get_cdma_time();
	sprintf(szMsg, "E_TM_%02d:%02d:%02d(%02d) ", jangbi.tm.hour, jangbi.tm.min, jangbi.tm.sec, (jangbi.tm.sec-start_sec));
	USART1_print(szMsg);
#endif	//#ifdef ___TCPIP_TEST_VIEW___

/**
AT+CRM=130

OK
AT$LGTPDIAL=0

OK
AT$LGTPDIAL=0

OK
AT$LGTTCPOP=61.108.48.26,13001

OK
ATDT019

OK

CONNECT

$TCPOPEN
12R4W2V-015-01-1    10PL ?34R40020140327175155F792          
$TCPCLOSED
AT$LGTTCPCL

OK
AT$LGTPREL
AT$LGTPREL

NO CARRIER
***/

//	USART1_puts("0: tcpip_send_predial_0  end");

	return ret_val;
}




void test_lte_modem()
{
	char szPort[10];
	int	 iz;

	return;

	for(;;)
	{
/*
		get_cdma_telno();
		get_cdma_rssi();
		get_cdma_time();
		get_cdma_gigiguk();

		lcd_cls();	  // 0330_2

		sprintf(str, "%c%c%c-%c%c%c%c-%c%c%c%c",jangbi.cdma.telno[0],jangbi.cdma.telno[1],jangbi.cdma.telno[2],
		                                        jangbi.cdma.telno[3],jangbi.cdma.telno[4],jangbi.cdma.telno[5],jangbi.cdma.telno[6],
												jangbi.cdma.telno[7],jangbi.cdma.telno[8],jangbi.cdma.telno[9],jangbi.cdma.telno[10]);
        GLCD_string(0,0 , str);

		sprintf(str, "%04d/%02d/%02d %02d/%02d/%02d",jangbi.tm.year,jangbi.tm.month, jangbi.tm.date,jangbi.tm.hour,jangbi.tm.min, jangbi.tm.sec);
		GLCD_string(0,2 , str);

		sprintf(str, "rssi=%02d ", jangbi.cdma.rssi);
        GLCD_string(0,4 , str);

		sprintf(str, "%15s",gszGigiguk);
        GLCD_string(0,6 , str);

		Delay_ms(1000);
*/

/*
//case_11:	//cip rouinte
		iSendStage = 0;
		giCipSendingFlag = 1;

		sprintf(gszCipServerIp, "%d.%d.%d.%d", 116,121,216,37);	//cip server ip
		gwCipServerPort = 9900;
		sprintf(gszCipServerPort, "%4d", gwCipServerPort);
		giCipSendingRetry = MAX_giCipSendingRetry;	//2015.4.24

		USART1_puts(gszCipServerIp);
		USART1_puts(gszCipServerPort);

		for(;;)
		{
			tcpip_send_CIP_20140808(gszCipServerIp, gwCipServerPort);
			if(iSendStage == 300)
			{	
				++giSendingType;
				iSendStage = 0;
				break;
			}
			Delay_ms(100);
		}

		Delay_ms(10000);
*/

/*
//	void tcpip_send_c4_status_20140808(char *ip, int port)
		strcpy(gszJangbiCode, "WZZ-33A-01-1");
		strcpy(local_code, "-ZZW-A331-10");

		iSendStage = 0;
		giKecoSendingStatusFlag = 1;
		SendStatusToKeco();
		USART1_puts(gszJangbiCode);
		keco_send_c4_20140808(gszJangbiCode, 23);	
		strcpy(gszKecoServerIp,"211.112.135.142");
		sprintf(szPort, "%5d", gwKecoServerPort);
		USART1_puts(gszKecoServerIp);
		USART1_puts(szPort);

		for(;;)
		{
		    tcpip_send_c4_status_20140808(gszKecoServerIp, gwKecoServerPort);
			if(iSendStage == 300)
			{	
				++giSendingType;
				iSendStage = 0;
				break;
			}
			Delay_ms(100);
		}

		Delay_ms(10000);
*/

/***************************
//	tcpip_send_c1_data_20140806(gszKecoServerIp, gwKecoServerPort);
			

			//	baichul_info_nor[1].Status_keco_new	61.108.48.26,13001
			if(iSendStage == 300) // 배출완료가 되면 (실패를 했던 성공을 해던 )
			{	
				++giSendingType;
				iSendStage = 0;
//				goto case_2;
//				goto case_default;	//TEST ONLY
			}

		strcpy(gszJangbiCode, "WZZ-33A-01-1");
		strcpy(local_code, "-ZZW-A331-10");

		iSendStage = 0;
		giKecoSendingStatusFlag = 1;
		SendStatusToKeco();
		USART1_puts(gszJangbiCode);
		keco_send_c4_20140808(gszJangbiCode, 23);	
		strcpy(gszKecoServerIp,"211.112.135.142");
		sprintf(szPort, "%5d", gwKecoServerPort);
		USART1_puts(gszKecoServerIp);
		USART1_puts(szPort);

		for(;;)
		{
			tcpip_send_c1_data_20140806(gszKecoServerIp, gwKecoServerPort);
			
			if(iSendStage == 300) // 배출완료가 되면 (실패를 했던 성공을 해던 )
			{	
				++giSendingType;
				iSendStage = 0;
				break;
			}

			Delay_ms(100);
		}
********************************/

/*********************
//		strcpy(gszJangbiCode, "WZZ-33A-01-1");
//		strcpy(local_code, "-ZZW-A331-10");
		strcpy(gszJangbiCode, "W2V-201-01-1");
		strcpy(local_code, "-V2W-1021-10");



		gbGetCardUpdate = 1;
		if(gbGetCardUpdate)
		{
USART1_puts("case 8 : gbGetCardUpdate");
			gbGetCardUpdate = 0;
//			Send_to_Gtech_Event("CARD_UPDATE", "TRY"); //발생한 event를 전송한다.

			Get_card_Info(); // 공단으로 부터 카드정보 받아오기 
// 			Get_card_update_Info();
		}
********************************/

/*************************************************
		strcpy(gszKecoServerIp,"211.112.135.142");
		sprintf(szPort, "%5d", gwKecoServerPort);
//		strcpy(gszJangbiCode, "WZZ-33A-01-1");
//		strcpy(local_code, "-ZZW-A331-10");
		strcpy(gszJangbiCode, "W2V-201-01-1");
		strcpy(local_code, "-V2W-1021-10");

		gbGetCardUpdate = 1;
		if(gbGetCardUpdate)
		{
USART1_puts("case 8 : gbGetCardUpdate");
			gbGetCardUpdate = 0;
//			Send_to_Gtech_Event("CARD_UPDATE", "TRY"); //발생한 event를 전송한다.

 			Get_card_update_Info();
		}
**************************/


/******************

		strcpy(gszJangbiCode, "WZZ-33A-01-1");
		strcpy(local_code, "-ZZW-A331-10");

		iSendStage = 0;
		giKecoSendingStatusFlag = 1;
		SendStatusToKeco();
		USART1_puts(gszJangbiCode);
		keco_send_c4_20140808(gszJangbiCode, 23);	
		strcpy(gszKecoServerIp,"211.112.135.142");
		sprintf(szPort, "%5d", gwKecoServerPort);
		USART1_puts(gszKecoServerIp);
		USART1_puts(szPort);

		for(;;)
		{
			tcpip_send_c1_data_20140806(gszKecoServerIp, gwKecoServerPort);
			
			//	baichul_info_nor[1].Status_keco_new	61.108.48.26,13001
			if(iSendStage == 300) // 배출완료가 되면 (실패를 했던 성공을 해던 )
			{	
				++giSendingType;
				iSendStage = 0;
				break;
			}
			Delay_ms(100);
		}
****************/

//		if(SocketOpen_LU202F("116.121.216.37,9900")) SocketClose_LU202F();
//		if(SocketOpen_LU202F("116.121.216.37",9900)) iz = SocketClose_LU202F();
//		if(SocketOpen_LU202F("211.112.135.142",13001)) iz = SocketClose_LU202F();	//test server
//		if(SocketOpen_LU202F("61.108.48.26",13001, 500)) iz = SocketClose_LU202F();	//real server
		
		sprintf(szPort, ".iz=%d.", iz);
		USART1_puts(szPort);
		
		Delay_ms(10000);
	}
}



int LGTTCPSTATE_LU202F()
{
	int  iz; 
//	int	 iRetry;
	int	 ret_val;

	ret_val = 0;
//	iRetry = 3;

//	do
	{
		RxCardCounter2 = 0;		
////		Keco_card_info_flag = 1;	//cdma data 받아오을때 
		memset(RxCardBuffer2, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화

		cdma_send_cmd("AT$LGTTCPSTATE?", 15);

		Delay_ms(10);	//2014.04.02  add because fail

		for(iz=0; iz<200; iz++)
		{
			Delay_ms(1);

			if(check_response(RxCardBuffer2, "$LGTTCPSTATE:4", RxCardCounter2, 14))
			{	
				ret_val=4;
			    USART1_puts("Socket opened!");
				break;
			}
			else if(check_response(RxCardBuffer2, "$LGTTCPCL:6", RxCardCounter2, 14))
			{	
				ret_val = 6;
			    USART1_puts("Socket Closed!");
	            break;
			}
		}

////		Keco_card_info_flag = 0;	//cdma data 받아오을때 

	}// while(--iRetry && !ret_val);

	return ret_val;
}

//int SocketOpen_LU202F(char *ip)
int SocketOpen_LU202F_onestop(char *ip, int port, int wait)
{
	int iz;
	int	ret_val;
	int	iRetry;
	char szMsg[40];

	ret_val = 0;
	iRetry = MAX_SOCKET_OPEN_RETRY;	 

//USART1_puts("stage_two_3");
//USART1_puts(ip);

	if(LGTTCPSTATE_LU202F() == 4) return 1;	//already opend

/**	
	strcpy(szMsg, "AT$LGTTCPOP=");	//cdma_send_cmd("AT$LGTTCPOP=61.108.48.26,13001", 30);
	strcat(szMsg, ip);
**/	
	sprintf(szMsg, "AT$LGTTCPOP=%s,%d", ip, port);	//cdma_send_cmd("AT$LGTTCPOP=61.108.48.26,13001", 30);

////	Keco_card_info_flag = 1;	//cdma data 받아오을때 

	do
	{
		RxCardCounter2 = 0;		
		memset(RxCardBuffer2, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화

		cdma_send_cmd(szMsg, strlen(szMsg));

		Delay_ms(10);	//2015.8.20

//		for(iz=0; iz<200; iz++)
		for(iz=0; iz<wait; iz++)	//공단 배출원 다운로드시 대기 시간때문에 
		{
			Delay_ms(1);
			if(check_response(RxCardBuffer2, "$LGTTCPOP:0", RxCardCounter2, 11))
			{
				ret_val = 1;
				break;
			}
			else if(check_response(RxCardBuffer2, "$LGTTCPOP:4", RxCardCounter2, 11)) //2015.8.20
			{
				ret_val = 4;
				break;
			}
		}

	} while(--iRetry && !ret_val);

	if(ret_val != 1) SocketClose_LU202F();

////	Keco_card_info_flag = 0;	//cdma data 받아오을때 

	return ret_val;
}

//int SocketOpen_LU202F(char *ip)
int SocketOpen_LU202F(char *ip, int port, int init)
{
	int iz;
	int	ret_val;
	int	iRetry;
	char szMsg[40];

	ret_val = 0;

	if(init == 0)
	{
		sprintf(szMsg, "AT$LGTTCPOP=%s,%d", ip, port);	//cdma_send_cmd("AT$LGTTCPOP=61.108.48.26,13001", 30);
		RxCardCounter2 = 0;		
		memset(RxCardBuffer2, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화
		cdma_send_cmd(szMsg, strlen(szMsg));
	}
	else
	{
		if(check_response(RxCardBuffer2, "$LGTTCPOP:0", RxCardCounter2, 11)) ret_val = 1;
		else if(check_response(RxCardBuffer2, "$LGTTCPOP:4", RxCardCounter2, 11)) ret_val = 4;
	}

	return ret_val;
}


//void Cdma_Force_Stop_step2_tcpcl_20140806(void)  // 전화 강제로 끊기 
int SocketClose_LU202F()
{
	int  iz; 
	int	 iRetry;
	int	 ret_val;

	ret_val = 0;
	iRetry = 3;

	if(LGTTCPSTATE_LU202F() == 6) return 1;	//already closed

	do
	{
		RxCardCounter2 = 0;		
////		Keco_card_info_flag = 1;	//cdma data 받아오을때 
		memset(RxCardBuffer2, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화

		cdma_send_cmd("AT$LGTTCPCL", 11);

		Delay_ms(10);	//2014.04.02  add because fail

		for(iz=0; iz<200; iz++)
		{
			Delay_ms(1);

			if(check_response(RxCardBuffer2, "$LGTTCPCL:0", RxCardCounter2, 11))
			{	
				ret_val=1;
//			    USART1_puts("SocketClose_LU202F OK!");
				break;
			}
			else if(check_response(RxCardBuffer2, "$LGTTCPCL:4", RxCardCounter2, 11))
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

	if(buf_length < pre_cnt) pre_cnt = 0;
		
	if(pre_cnt < buf_length)
	{
		for(iz=pre_cnt; iz<buf_length; iz++) 	USART1_putchar(str1[iz]);
		pre_cnt = buf_length;
	}
//	for(iz=0; iz<buf_length; iz++) 	USART1_putchar(str1[iz]);

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



int get_cdma_with_rx2cardbuffer(char *cmd, char *response, int retry)
{
	int	ret_val;
	int	iz;
	int	iRetry;

	ret_val = 0;
	iRetry = retry;

	do
	{
		RxCardCounter2 = 0;		
		Keco_card_info_flag = 1;	//cdma data 받아오을때 
		memset(RxCardBuffer2, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화

		cdma_send_cmd(cmd, strlen(cmd));

		Delay_ms(10);	//2014.04.02  add because fail

		for(iz=0; iz<100; iz++)
		{
			Delay_ms(1);

			if(check_response(RxCardBuffer2, response, RxCardCounter2, strlen(response)))
			{	
//				Keco_card_info_flag = 0;	//cdma data 받아오을때 
				ret_val = 1;
				break;
			}
		}

//		Keco_card_info_flag = 0;	//cdma data 받아오을때 
	} while(--iRetry && !ret_val);

	return ret_val;
}


//2015.9.15
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
		RxCardCounter2 = 0;		
		memset(RxCardBuffer2, 0, RXCARDBUFFER2SIZE);	//리턴할 배열 초기화

		cdma_send_cmd(szMsg, strlen(szMsg));

		Delay_ms(10);	//2015.8.20

//		for(iz=0; iz<200; iz++)
		for(iz=0; iz<wait; iz++)
		{
			Delay_ms(1);
			if(check_response(RxCardBuffer2, "$LGTMTCPOP:2,0", RxCardCounter2, 14))	//success
			{
				ret_val = 0;
				break;
			}
			if(check_response(RxCardBuffer2, "$LGTMTCPOP:2,5", RxCardCounter2, 14))	//already opened
			{
				ret_val = 5;
				break;
			}
			else if(check_response(RxCardBuffer2, "$LGTMTCPOP:2,4", RxCardCounter2, 14)) //FAIL
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


int	tcpip_send_case_20(char *ip, int port, int init)
{
	int iz;

	iz = Stage_Two_IPSetting(ip, port, iStabeTwoWaitCount++); //send command

	if(iStabeTwoWaitCount <= 50) //wait response
	{	if(iz == 1) iSendStage = 40;
		else if(iz == 4)
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


/* end of file */
