/*
	main.c

	optimization level 2

원격입력
span factor	현재완료	
 @#RFAC=12341@
 @#ZIGBEE=000195000000F4EA,$RFAC=12345@
현재무게	프로그램 수정
@#RWEIGHT=12345@
@#ZIGBEE=000195000000F4EA,$RWEIGHT=12345@
영점	프로그램 수정
@#RZERO=12345@
@#ZIGBEE=000195000000F4EA,$RZERO=12345@
	
	
배출무게계산	"음수나오는문제 
  배출무게계산 다시한번생각"	1. 핸드폰 어플에서 표현	06월 28일
로드셀고장시	보정계산	1.프로그램 수정	06월 30일

	ver 2.0 (2017.7.5)
	  1. test, set 모드 후 메뉴 다시 표시
	  2. set_mode() 에서 real weight 에서 죽는 거 수정 (str[50] -> str[150] 변경)
	  3. 영점키 길게 누르고 확인 (0, 5)
	  4. 원격 업데이트
	  

	1.9 (2017.6.26)
	  1. check l/c 6ea
  	  2. L/C 6 EA #no
	     #2 #4 #6
		 #1 #3 #5


	1.8 (2017.3.2)
	  1. gbHourEvent 추가
	  2. LCD_restart() 추가
	  

	1.7 (2016.7.26)
	 1. adc "-1" 나오는 거
		- mon_ 변수 추가
	 2. 날짜 넘어가는 문제 (시계 읽어오는거 확인할것. 212)	
	 3. "-" 무게 표시하기


1.6 (2016.7.4)
1.5b1
설정값 보낼때  보정무게를 unsigned -> signed로 
상태정보값 보낼때  보정무게를 unsigned -> signed로 
gnlTick 을 TIMER2에서 RTC로 옮김
음수일 때 한눈에 값에 맞누는 거 보완(음수에서도 동작하게)
get_weight() 함수 추가
현재무게	음수나오는 문제	  -값으로 표현


	- 모뎀 없을시 늦어지는 거 대책 만들것.
	- 자체경보 - 배출량이 많거나, 작거나
	- 지그비로 날아가는 것도 만들기

	2016.5.2
	 - 무게표시 적색을 검은색으로 변경

	2016.4.27
	 - IWDG_SetReload(0xFFFF);	//26214.4 ms

	2016.4.4
	 - 

#define	MONITORING_SERVER_IP	"112.217.212.250"
#define	MONITORING_SERVER_PORT	8391


//#RES_MODEMLONE01034431529@(26 BYTE)// CDMA RESET
//#REG_TELEPHONE01034431529@(26 BYTE)// 배출량 보내는 전화번호등록 
//#DEL_TELEPHONE01034431529@(26 BYTE)// 등록된 전화번호 삭제
//#AUT_SENDINGTIME_SMS_1234@(26 BYTE)// DATA 전송하는 시간 등록(자동전송)
//#MAN_SENDINGTI01034431529@(26 BYTE)// SMS 를 통한 DATA 전송 
//#REMOTE_PROGRAM_UPLOADING@(26 BYTE)// 원격 펌웨어 업데이터 
//#IP=123.123.123.123:12345@(26 BYTE)	// 펌웨어 업데이트 IP 저장 


Program Size: Code=46164 RO-data=48172 RW-data=1260 ZI-data=22844  
Program Size: Code=37784 RO-data=38552 RW-data=784 ZI-data=29384  

상태정보	상태정보를 1시간에 1번씩 전송한다.
사료 재고량	사료 재고량 정보를 1시간에 1번씩 상태정보와 함께 전송한다.
배출정보	배출 시작시 상태정보를 전송한다.
		배출중 중량을 10분에 1번씩 전송한다.  (WORKING_SEND_INTERVAL)
		배출 종료시 배출량과 현재고량, 배출시간을 전송한다.
카운터값	카운터에 수신한 데이터를 1분간격으로 전송한다.
알람값	알람을 수신하는 즉시 전송한다.
아날로그값	수신한 아날로그 데이터를 1분간격으로 전송한다.
통신값	수신한 통신 데이터를 1분간격으로 전송한다.


	2015.4.29
	 1. 16x16, 24x24 폰트 출력함수 추가	(폰트 데이터 약 10 kb)
	   1-1. void lcd70_show_string_mode_24x24(unsigned int x,unsigned int y,const unsigned char *p,unsigned int size,unsigned int mode)
	   1-2. void lcd70_show_char_mode_24x24(unsigned int x,unsigned int y,unsigned int num,unsigned int size,unsigned int mode)
	 2. 글꼴 만들기
       2-1. 세로로 한글자씩 BMP로 작성. 그림판 이용. 단색 비트맵으로 저장.
       2-2. TOTAL IMAGE CONVERTER 로 RAW NO HEADER 로 저장
       2-3. FED 로 읽어서 헤더없이 저장, DOSBOX 이용.
       2-4. PG4UW로 읽어서 Straigth HEX 형식으로 저장
       2-5. DESYEDIT 로 읽어서 0x00, 형식으로 변환 (매크로 이용)
       2-6. 배열로 만들어서 표시
       2-7. 끝

		USART  사용 
		COM1(CN3) : 정션보드 RS-232
		COM2(CN12,CN13) : 정션보드 RS-485
		COM3(CN18) : CDMA
		COM4(CN14) : jigbee

		Predefine 설정 
		100pin
		STM32F103VCT6(512KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_HD
		STM32F105VCT6(512KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_CL
		STM32F107VBT6(256KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_CL
		STM32F107VCT6(256KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_CL
		STM32F107VBT6(128KB, 32KB) :  USE_STDPERIPH_DRIVER, STM32F10X_CL
		
		144pin
		STM32F103ZET6(512KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_HD
*/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"	



#include "stm32f10x.h"
#include "board_gpi8510.h"
#include "stm32f10x_lib.h"
#include "stm32f10x_it.h"
#include "stm32f10x_iwdg.h"
#include "cortexm3_macro.h"
#include "hw_config.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_systick.h"


#include "stm32f10x_lib.h"
#include "stm32f10x_it.h"
#include "stm32f10x_rcc.h"


/*******************************agent**********************************************/
#include  "agent_input_filter.h"
#include "agent_weight.h"
/*******************************agent**********************************************/


#include "def.h"
#include "Sflash.h"
#include "adc.h"
#include "buzzer.h"
#include "cal.h"
#include "global.h"
#include "i2c_ee.h"
#include "key.h"
#include "lcd.h"
#include "lcd70.h"
#include "led.h"
#include "main.h"
#include "rs485.h"
#include "rtc.h"
#include "var.h"
#include "sms.h"
#include "spi.h"
#include "tcpip_send.h"
#include "timer.h"
#include "usart.h"
#include "type.h"

#include "debug.h"



#define  MAX_ERROR_COUNTER  7

#define NORMAL_DISPLAY  1
#define STATUS_DISPLAY  2

#define	INPUT_ONE	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) == 0x00)     
#define INPUT_TWO   if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) == 0x00)     
#define	INPUT_THREE if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12) == 0x00)   //진동센서   
	
/**********AI AGENT**************/

#define FIRST_INPUT_ON   if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) == 0x00)


#define SECOND_INPUT_ON  if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) == 0x00)

/********** AGENT RAW INPUT - ACTIVE LOW **********/

#ifdef FIRST_INPUT_RAW
#undef FIRST_INPUT_RAW
#endif

#ifdef SECOND_INPUT_RAW
#undef SECOND_INPUT_RAW
#endif

#define FIRST_INPUT_RAW()   ((GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) == 0x00) ? 1 : 0)
#define SECOND_INPUT_RAW()  ((GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) == 0x00) ? 1 : 0)


//#define	RS422ON		GPIO_SetBits(GPIOA, GPIO_Pin_4)
//#define	RS422OFF	GPIO_ResetBits(GPIOA, GPIO_Pin_4)


volatile int	giCdmaReturn;	//2015.8.17
char			gszCdmaReturn[100];	//2015.8.17

char str[100];



//<<<<<<<<<<<<<<<<<< local variable




u8 Once_flag=0;
u8 Once_flag_First=0;
u8 Once_flag_Second=0;
u8 Once_flag_Third=0;

u8 Error_Counter=0;

u8	baichul_start_flag = 0;
u8	baichul_end_flag = 0;		    /* determine done */


u8 Error_Counter2=0;

u8	baichul_start_flag2 = 0;
u8	baichul_end_flag2 = 0;		    /* determine done */




u8	  ek_flag1 = 0;	/* second external key flag */

u8	  ek_flag1_HighCount = 0,	/* second external key flag */
	    ek_flag1_LowCount  = 0;	/* second external key flag */

u8 byNewStart=0;
u8 stop_flag=0;
long	lPulseRef = 0;
u8	  ek_flag2 = 0;	/* second external key flag */
u8	  ek_flag2_HighCount = 0,	/* second external key flag */
	    ek_flag2_LowCount  = 0;	/* second external key flag */
u8 byNewStart2=0;
u8 stop_flag2=0;
long	lPulseRef2 = 0;





s32    Start_Weight;
s32    Stop_Weight;
s32    TodayTotalref;
s32    TodayTotalref_result;

s32    Start_Weight2;
s32    Stop_Weight2;
s32    TodayTotalref2;
s32    TodayTotalref_result2;





u8  sign_flag=0; // 1: minus   0: plus

long    lDay_Ref_Weight=0;
long	 lPulseMaxHold = 0;
u8     	lPulseMaxHoldCount = 0;

long    lRefWeigh = 0;
//long    lRefWeigh2 = 0;

long    lDay_Ref_Weight2=0;
long	 lPulseMaxHold2 = 0;
u8     	lPulseMaxHoldCount2 = 0;

long    lRefWeigh2 = 0;



char once_flag=0;

char gtech_tel[] ="01034431529";

#define ApplicationAddress 0x8000000  // normal program execution address
typedef  void (*pFunction)(void);
pFunction Jump_To_Application;
u32 JumpAddress;

/*
long    NoiseSeconds = 0;    //Ver1.10 noise time setting (2001.12.27)
int     NoiseCount = 0;      //Ver1.10 relay start (2001.12.27)
long	NoiseWeightBack = 0; //Ver1.10 back weight - weight (2001.12.27)
long	NoiseWeight = 0;     //Ver1.10 (2001.12.27)
int		NoiseError = 0;      //Ver1.10 not stop (8 sec) (2001.12.27)
int     NoiseFactor = 0;     //Ver1.10 8 sec (2001.12.27)
long	NoiseWeightGo =0;    //Ver1.10  (2001.12.27)
int     NoiseLast = 0;       //Ver1.10 gnlLossLimit/1.02 (2001.12.27)
*/

//char Nongjang_Sms_Hour;
//char Nongjang_Sms_Minute;
//char Nongjang_tel[] ="01034431529";

//EXTI_InitTypeDef EXTI_InitStructure;
//ErrorStatus HSEStartUpStatus;
//ErrorStatus HSEStartUpStatus;

char CdmaSendQueue_tmp[256];
int	 CdmaSendQueue_index = 0;

char RxCdmaTokenQ_tmp[256];
int	 RxCdmaTokenQ_index = 0;

float Real_Weight;
int	  poweron_flag = 1;


// local variable >>>>>>>>>>>>>>>>>>>




//volatile u8	ghPowerOnResetStatus = 'R';	//status sendd power on reset=R, else G

//extern const struct bin  gimp_image;
extern const struct lamp  start_lamp;
extern const struct lamp  stop_lamp;


//void __MSR_MSP(u32 TopOfMainStack);

void zigbee_setting(void);
int	 RTC_time_display(void);
void logo(void);
void mode_check(void);
void Adc_Power_Off_On(void);
void Data_Clear(void); // data clear
void test_set(void);
void Data_Send_Sms(char *tel_number);
void Get_Weight(void);
void SMS_PROCESS(void);
//void Initiaize_eeprom(void);
int	isCdmaSendQueueEmpty();
int CdmaSendQueue_puts(char *txt);
int CdmaSendQueue_gets();

void modem_power_reset(void);	//LTE MODEM reset and power on/off

void sms_process(char *msg);

void zero_func(void);
void zero_func2(void);//silo 2 zero function

//GPI-8510

void  Silo_1_Led_Operation();
void  Silo_2_Led_Operation();
void  Error_1_Led_Operation();
void  Error_2_Led_Operation();
void  Silo_1_Weight_Display();
void  Silo_2_Weight_Display();


//GPI-8510






volatile u8 INVERSE;
volatile unsigned char   HAN_CODE;
volatile unsigned char   FONT_SIZE;             
volatile unsigned char   FONT_STYLE;
                         
volatile u16 xcharacter;
volatile u16 ycharacter;
volatile u16 xgraphic;
volatile u16 ygraphic;
volatile u16 cur_x;
volatile u16 cur_y;
volatile u16 xch;		//character x
volatile u16 ych;		//character y
volatile u16 xgr;		//graphic x
volatile u16 ygr;		//graphic y
volatile u16 xcur;		//cursor x
volatile u16 ycur;		//cursor y

volatile int cx, cy; 	//2014.8.27
volatile u8 LCX, LCY;
volatile u8 LCDPAGE;
u8		net_sign     = 0;
//u8 cal_selection;

void cal_display(void);

//void Delay_us(vu32 nCount);
// #include "stm32f10x_systick.h"


void Silo_1_Getweight(void);
void Silo_2_Getweight(void);
void SendtoPc(void);  // 

	u8 sign =0;      // for silo1
	u8 sign_2 =0;   // for silo2

u8  min_rotate=0;
u8  min_buffer=0;
u8 Rtc_Seconds;
u8 HT1381_year,HT1381_month,HT1381_date,HT1381_hour,HT1381_minute,HT1381_second; 


void Date_Time_Display(void);
char Loadcell_Error1=0,Loadcell_Error2=0;
long lPulseWeigh=0;
long lPulseWeigh2=0;

BOOL	bPulse = 0;
BOOL	bPulse2 = 0;
int  Error_Sec_Counter=0;
BOOL Output_Low_Error = 0;
BOOL Output_High_Error = 0;

int  Error_Sec_Counter2=0;
BOOL Output_Low_Error2 = 0;
BOOL Output_High_Error2 = 0;
long lOutput=0;
//char start_sign_flag=0;
 char start_sign_flag=0; // 시작시 현재무게 값이 - 면 1, 
 char start_sign_flag_2=0; // 시작시 현재무게 값이 - 면 1, 
	



//long gxuPulseSetWeigh=20;

#define STX 0X02
#define ETX 0X03

void Show_ToalOutput(void); // 총누적배출량 표시하기 
void Initial_Display(void);
void set_ip(void);  // VER 2.6 
void set_port(void);


void Phone_Sms_Processing(void);
char szMsg[15];
char szResponse[15];
char sT0[256];

s32  Output_total=0; //ver 2.6
s32  Output_total2=0; //ver 2.6 ,silo 2번 

long UpperError_Weight=0; // 

char lte_restart_flag1=0,lte_restart_flag2=0,lte_restart_flag3=0,lte_restart_flag4=0;


	#define FIRST_INPUT_OFF_CONFIRM_COUNT       30
	#define SECOND_INPUT_OFF_CONFIRM_COUNT   30
	
	
	/*****************************************************************AI AGENT***************************************/
	
	//AGENT_INPUT_FILTER g_first_input;
//AGENT_INPUT_FILTER g_second_input;
	
AGENT_INPUT_FILTER g_first_input;
AGENT_INPUT_FILTER g_second_input;	
	
	/************************************************************
 * Agent Discharge Input Task Event
 ************************************************************/
#define AGENT_DISCHARGE_TASK_NONE          0
#define AGENT_DISCHARGE_TASK_FIRST_START   1
#define AGENT_DISCHARGE_TASK_SECOND_START  2

unsigned char Agent_DischargeInput_Task(void);  // 입 ,출력
void Agent_WeightDisplay_Task(void);  //  무게표시

AGENT_WEIGHT g_silo1_weight;
AGENT_WEIGHT g_silo2_weight;

#define AGENT_DISCHARGE_CALC_NONE   0
#define AGENT_DISCHARGE_CALC_STOP   1

unsigned char Agent_DischargeCalc_Task(void);  //배출량 계산

void Agent_DischargeStop_Task(void);  // 배출량 계산 stop
void Agent_BaichulSend_Task(void);    //배출량 서버전송
void Agent_MakeBaichulPacket(char *bc_code);  


#define AGENT_STOP_DELAY_NONE   0
#define AGENT_STOP_DELAY_DONE   1

unsigned char Agent_DischargeStopDelay_Task(void);
void Agent_DateChange_Task(void);  // 날짜 변경시 해야할 일


#define AGENT_KEY_EVENT_NONE      0
#define AGENT_KEY_EVENT_INITIAL   1

unsigned char Agent_KeyProcess_Task(void);
void Agent_CdmaTask(void); //전화기에서 온 데이터 처리
void Agent_MinuteEvent_Task();

//AGENT_WEIGHT_MONITOR g_silo1_weight_monitor;
//AGENT_WEIGHT_MONITOR g_silo2_weight_monitor;

/*****************************************************************AI AGENT***************************************/
		
	int	poweron_date_set_counter = 2; //(4-1)분

int main(void)
{
//	long main_loop = 0;
//	long get_weight_oop = 0;
	
	int	test_sended_time = 100;
//	int	poweron_date_set_counter = 2; //(4-1)분

	char sT0[256];
	char ch;
	int  iz, jz, kz;
	u32 i;
	u32 Making_zero_timer=0;
	u8 Current_Display = NORMAL_DISPLAY;
	long temp_buff;
	long temp_buff_1;
	long temp_buff_final;
	signed long which_day_feeding=0; 
	long sum;
	//signed long  v_i_value;
//	u8 sign =0;      // for silo1
//	u8 sign_2 =0;   // for silo2
	
	u32   FLASH_ID = 0;
	
	int _2MinEvent;
	long key_pressed_counter;
	char keycode_8510;
	
__disable_irq();
    bsp_init_rcc();	     //hw.comfig_c , clock configuration   , OUTPUT i
	  bsp_init_interrupt();
__enable_irq();

   bsp_key_gpio_init(); //including relay ,lcd(128 *64) and so on	

//////	buzzer_init();
/////	register_timer_function(timer2ServiceFunction, timer2_event);
//	register_rtc_function(rtcServiceFunction, rtc_event);


	 USARTx_Initial(); // usart initialize
   USART1_puts("Cdma_Power_On();");
	 Cdma_Power_On();
	 Delay_ms(100);
	 cdma_send_cmd("AT$LGTRESET", 11);	//2016.6.10 when main softreset
	 Delay_ms(100);
 	 Cdma_Power_Off();
	 Delay_ms(1000);

 
   USART1_puts("I2C_EE_Init();");
	 I2C_EE_Init();	//EEPROM port 초기화
	 Delay_ms(100);
	 
//	 mode_check();
	 
	 //write_factors();

   USART1_puts("read_factors();");
	 Delay_ms(100);
	 FSMC_DISABLE;
	 read_factors();  // FSMC를 disable 시키지 않으면 eeprom read, write 가 되지 않음 
	 lcd_initial();
   lcd_clear();

	 
	//mode_check();	
		
		sprintf(str, "v_capacity : %d", v_capacity);	USART1_puts(str);
		sprintf(str, "v_division : %d", v_division);	USART1_puts(str);
		sprintf(str, "v_zero : %d", v_zero);	USART1_puts(str);
	  sprintf(str, "gnfFactor : %f", gnfFactor);	USART1_puts(str);
 
	 
	  sprintf(str, "v_capacity_2 : %d", v_capacity_2);	USART1_puts(str);
		sprintf(str, "v_division_2 : %d", v_division_2);	USART1_puts(str);
		sprintf(str, "v_zero_2 : %d", v_zero_2);	USART1_puts(str);
	  sprintf(str, "gnfFactor_2 : %f", gnfFactor_2);	USART1_puts(str);
    sprintf(str, "gnlMyAddress : %d", gnlMyAddress);	USART1_puts(str);
	
 //gnlMyAddress
 
	 //20171130
	 /*****************************RTC TEST*****************************************/
	

 Feeding_Counter1=0; //배출량 계산주기 타이머
 Feeding_Counter2=0; //배출량 계산주기 타이머



 USART1_puts("RTC TEST ");Delay_ms(2000);


	//#define  Data_Write      Rtc_Write()
	//#define  Data_Read      Rtc_Write()
	
	
	#define	 RTC_CLOCK_HIGH   GPIO_SetBits(RTC_SCLK_PORT, RTC_SCLK_PIN)
  #define	 RTC_CLOCK_LOW  	GPIO_ResetBits(RTC_SCLK_PORT, RTC_SCLK_PIN)
  #define	 RTC_RESET_HIGH   GPIO_SetBits(RTC_RST_PORT, RTC_RST_PIN)
  #define	 RTC_RESET_LOW  	GPIO_ResetBits(RTC_RST_PORT, RTC_RST_PIN)
  #define	 RTC_DATA_HIGH   GPIO_SetBits(RTC_IO_PORT, RTC_IO_PIN)
  #define	 RTC_DATA_LOW  	GPIO_ResetBits(RTC_IO_PORT, RTC_IO_PIN)
	
	
#define GetSecond 0x81   
#define GetMinute 0x83   
#define GetHour   0x85   
#define GetDate   0x87   
#define GetMonth  0x89   
#define GetDay    0x8b   
#define GetYear   0x8d   
#define GetTime   0x0bf   
#define WP        0x8e   
#define CH        0x80   
   
#define SetSecond 0x80   
#define SetMinute 0x82   
#define SetHour   0x84   
#define SetDate   0x86   
#define SetMonth  0x88   
#define SetDay    0x8a   
#define SetYear   0x8c   
#define SetTime   0x0be   

    Clock_Pin_Init();// RTC initialization
    Delay(500); 
  	bsp_init_timer2();
    RTC_Configuration();  /* RTC Configuration */
		key_init();
   
	 Cdma_Power_On();
   USART1_puts("RS422ON;");
	 
	 Gpi8510_Relay_On(ERROR); // 에러신호
	// Delay_ms(3000); 
	
	//// mode_check();
	
	
	 RS422ON;	
	 Delay_ms(500); // request from Mr park
	
  
	 USART2_puts("This is RS485 Serial test !!!"); // Delay_ms(1000);
	 USART2_puts("This is RS485 Serial test !!!"); // Delay_ms(1000);
	
	
	 USART3_puts("RS422ON;");
   RS422OFF;
   USART1_puts("bsp_lcd70_init();");
   lcd_initial();
   lcd_clear();

   //All_Relay_Off();




 GLCD_string (0,0 ,"   GPI-8510     ");
 GLCD_string (0,2 ,"MULTI -PURPOSE");
 GLCD_string (0,4 ,"FEED &FEEDING");
 GLCD_string (0,6 ,"  CONTROLLER  ");
 Delay_ms(3000);
 lcd_clear();
 GLCD_string (0,2 ,"   CLAUDE1   ");
 GLCD_string (0,4 ,"   VER 4.0 ");  //   2025년 6월 30일, 
 
 Delay_ms(3000);
 lcd_clear();

//if(!gnlCdmaUse) // 전화기 선택 안되었을때
{	
 for(i=0;i<2;i++)
 Date_Time_Display(); // RTC TIME display 
 
      HT1381_year=Gettimebuf[6];
			HT1381_month=Gettimebuf[4];
			HT1381_date=Gettimebuf[3];
			HT1381_hour=Gettimebuf[2];
			HT1381_minute=Gettimebuf[1];
			HT1381_second=Gettimebuf[0];
}		
 
 
 
 
 
 
 	/******************* systick configuration, this configu must be declared here 1!***********************************************/
	    
    SysTick_CLKSourceConfig(9000000);
    SysTick_SetReload(90);// 90/9MHZ =  100 khz > a/d clock (50 k hz) 
  	SysTick_SetReload(590);// 590/9MHZ =  15.24  khz > a/d clock (7.62  k hz) 
		SysTick_SetReload(60);// 90/9MHZ =  100 khz > a/d clock (50 k hz) 
  	
		SysTick_CounterCmd(SysTick_Counter_Enable);	//Enable the SysTick Counter
		SysTick_ITConfig(ENABLE);
  
  // ads 1251 sampling 
	//  speed                        clk   
	//  20                           7.68 khz
	//  50                           19.2 khz
	// 100                           38.4 khz

 /******************* systick configuration***********************************************/

  Ten_Mili_Counter=0;
  Silo1_led_Off();
  Silo2_led_Off();
  Silo1_Error_led_Off();
  Silo2_Error_led_Off();
	
	
	adc_initial();     // ADS1251
	INIT_CS5555(); // CS5555 초기화
	
	 temp_buff = RDDATA_ads1251(); // 어떤 ad 컨버터가 사용되었는지 판단하는 함수

	/*********** ADS 1251  인지 CS5555인지 ?******************/
	mode_check();
	lcd_initial();
  lcd_clear();
	
	/************************************************agent**********************************************/
	
	AGENT_InputFilter_Init(&g_first_input,3,100,0);
  AGENT_InputFilter_Init(&g_second_input,3,100,0);
//	AGENT_Weight_Init(&g_silo1_weight);
//AGENT_Weight_Init(&g_silo2_weight);
	
	/************************************************agent**********************************************/
	

	lcd_normal();

 v_adc_org  = (long)((float)v_zero * gnfFactor);  // calibration시 영점값 기억 
 v_ei_multiply_factor = 10;
 
 Usart3_SMS_flag = 0;
 Delay_ms(500);
 gnlDisplay=1;

 if(gnlDisplay)
	{
		sprintf(sT0, "Get_Weight() before");
	//	lcd70_show_string_mode(10, 280, (const unsigned char *)sT0, 20, 0);
	}	

	//CdmaSendQueue init
	CdmaSendQueue_qhead = CdmaSendQueue_qtail = 0;
	for(iz=0; iz<MAX_CDMA_QUEUE_BUFFER; iz++)
	{
		for(jz=0; jz<256; jz++)
		{
			CdmaSendQueue[iz][jz] = NULL;
		}
	}

	RxCdmaQ_qhead = RxCdmaQ_qtail = 0;
	for(iz=0; iz<RXCDMAQ_SIZE; iz++)
	{
		RxCdmaQ[iz] = NULL;
	}

	RxCdmaTokenQ_qhead = RxCdmaTokenQ_qtail = 0;

	bachul_send_timer_ms = 0;	//배출중 정보 전송 타이머
	cdma_response_waiting_flag = 0;	//cdma 응답을 기다리는 상태
	cdma_response_wait_timer_ms = 0;	//cdma 응답 기다리는 타이머
	giCdmaStopEnabled = 0;	//cdma 를 강제로 stop 실행을 해야 하는가 플래그
	
	
	
	
	

/**********************************************************************
sprintf(sT0, "tel=%s,rssi= %d",  jangbi.cdma.telno, jangbi.cdma.rssi);
send_sms("01091954072", sT0);
***********************************************************************/

DEBUG_puts("do  MAIN LOOP");

gnlUseLTE=1;


		for(iz=0;iz<3;iz++)
		{
			//add 2015.1.22
			DEBUG_puts("kwak won ho");
			cdma_send_cmd("AT$LGTDELRM=*", strlen("AT$LGTDELRM=*"));
			delay_ms(100);
		}
		
DEBUG_puts("get_cdma_info_WAIT(CDMA_GET_TELNO)");	
		
lcd_clear();
		
    if(gnlLanguage==1) // korean 
		{	
		   GLCD_string (0,0 ,"잠시만");
    GLCD_string (0,2 ,"기다려 주세요");
    GLCD_string (0,4 ,"장비초기화중...");
  	
    }
		
	  else
	{
    GLCD_string (0,0 ,"PLEASE WAIT ....");
    GLCD_string (0,2 ,"TELEPHONE CHECK.");
    GLCD_string (0,4 ,"SEVER TESTING..");
  }		
		
	//Delay_ms(3000);
	 for(i=0;i<30;i++)
	  {

			if(which_adc==1) 
			{
				temp_buff =read_filtered_adc_CS5555_1();
				temp_buff_1 =read_filtered_adc_CS5555_2();
			}
		 else
		 { 
		  temp_buff = read_filtered_adc();
      temp_buff_1 = read_filtered_adc_1();
	   }
		delay_ms(100);
		 sprintf(str, "adc_valew: %d", temp_buff);	USART1_puts(str);
		
	  }		
//0413		
		
		
	//	Silo_1_Getweight();//
	

if(which_adc==1) 
{		
if ((read_filtered_adc_CS5555_1()< 10L) || (read_filtered_adc_CS5555_1() > 380000L))
{	
	  lcd_clear();
		 if(gnlLanguage==1) 
		 {	 
		  GLCD_string (0,0 ,"무게센서 에러!!");
		  GLCD_string (0,4 ,"무게센서를");
		  GLCD_string (0,6 ,"점검해 주세요!");
		  Gpi8510_Relay_Off(ERROR);
		 }
		 else
     {	 
		  GLCD_string (0,0 ,"LOAD CELL ERROR!");
		  GLCD_string (0,4 ,"CHECK");
		  GLCD_string (0,6 ,"LOAD CELL !");
		  Gpi8510_Relay_Off(ERROR);
		 }
	  return;
	
	}
}
else
{		
if ((read_filtered_adc() < 10L) || (read_filtered_adc() > 380000L))
{	
	  lcd_clear();
		 if(gnlLanguage==1) 
		 {	 
		  GLCD_string (0,0 ,"무게센서 에러!!");
		  GLCD_string (0,4 ,"무게센서를");
		  GLCD_string (0,6 ,"점검해 주세요!");
		  Gpi8510_Relay_Off(ERROR);
		 }
		 else
     {	 
		  GLCD_string (0,0 ,"LOAD CELL ERROR!");
		  GLCD_string (0,4 ,"CHECK");
		  GLCD_string (0,6 ,"LOAD CELL !");
		  Gpi8510_Relay_Off(ERROR);
		 }
	  return;
	
	}
}	
	
	
		
	lcd_clear();

	if(gnlCdmaUse) // 전화기 선택 되었을때
 {
		for(i=0;i<100;i++)
		{	
			
    if(which_adc==1) 
			{
				temp_buff =read_filtered_adc_CS5555_1();
				temp_buff_1 =read_filtered_adc_CS5555_2();
			}
		 else
		 { 
		  temp_buff = read_filtered_adc();
      temp_buff_1 = read_filtered_adc_1();
	   }
			 
			
			
			
			//delay_ms(100);
			get_cdma_info_WAIT(CDMA_GET_TELNO, "TELNO");	//get_cdma_telno();
			get_cdma_info_WAIT(CDMA_GET_TIME, "TIME");	//get_cdma_time();
			get_cdma_info_WAIT(CDMA_GET_RSSI, "RSSI");	//get_cdma_rssi();
			//0728
			delay_ms(500);
			
			sprintf(sT0, "%2d,T=%s", i, jangbi.cdma.telno);
			GLCD_string(0,0, sT0);
			
			sprintf(sT0, "%2d,RSSI= -%d db", i, jangbi.cdma.rssi);
			GLCD_string(0,2, sT0);
			
      sprintf(sT0, "DATE=%04d-%02d-%02d", jangbi.cdma.tm.year, jangbi.cdma.tm.month, jangbi.cdma.tm.date);
		  GLCD_string(0,4, sT0);
 		
      sprintf(sT0, "TIME=%02d:%02d:%02d",  jangbi.cdma.tm.hour, jangbi.cdma.tm.min, jangbi.cdma.tm.sec);
		  GLCD_string(0,6, sT0);
			Delay_ms(150);
		if((jangbi.cdma.telno[0]=='0' )&&(jangbi.cdma.telno[1]=='1' )&&((jangbi.cdma.tm.year/100)==20)) // 20xx 년도이면
		  {	
			break;
		  }	
		} //of for 
		
			for(i=0;i<2;i++)
		{	
		//	get_cdma_telno();
		
			get_cdma_info_WAIT(CDMA_GET_TELNO, "TELNO");	//get_cdma_telno();
			get_cdma_info_WAIT(CDMA_GET_TIME, "TIME");	//get_cdma_time();
			get_cdma_info_WAIT(CDMA_GET_RSSI, "RSSI");	//get_cdma_rssi();

      delay_ms(500);			
			
			sprintf(sT0, "%2d,T=%s", i, jangbi.cdma.telno);
			GLCD_string(0,0, sT0);
			
			sprintf(sT0, "%2d,RSSI= -%d db", i, jangbi.cdma.rssi);
			GLCD_string(0,2, sT0);
			
      sprintf(sT0, "DATE=%04d-%02d-%02d", jangbi.cdma.tm.year, jangbi.cdma.tm.month, jangbi.cdma.tm.date);
		  GLCD_string(0,4, sT0);
 		
      sprintf(sT0, "TIME=%02d:%02d:%02d",  jangbi.cdma.tm.hour, jangbi.cdma.tm.min, jangbi.cdma.tm.sec);
		  GLCD_string(0,6, sT0);
			Delay_ms(100);

		}// for loop

		
		
		
		
	
 } // of if(gnlCdmaUse) // 전화기 선택 되었을때	
		if(gxhWeightBackup)
		{
		 v_adc_org=(long)(gnfFactor * v_zero);
     v_adc_org_2=(long)(gnfFactor_2 * v_zero_2);
		}
   else
		 {
		 v_adc_org=(long)(gnfFactor * temp_buff);
     v_adc_org_2=(long)(gnfFactor_2 * temp_buff_1);
		}

		  Silo_1_Getweight();
			Silo_1_Getweight();
			Silo_2_Getweight();
			Silo_2_Getweight();

		//AGENT_Weight_SetCurrent(&g_silo1_weight, gxlWeight, sign);
   //AGENT_Weight_SetCurrent(&g_silo2_weight, gxlWeight_2, sign_2);
		
		
		
		
		
		
		

if(gnlCdmaUse)//전화기 선택되었을때 .
{	
	for(i=0;i<2;i++)
		 {
		    Send_to_server(SEND_BAICHUL, "BC-B");	//배출 시작시 정보 전송
		  	test_routine();//회사서버로 보내기 
        Delay_ms(100);
		 }	
	//close websms socket
	MultiSocketClose_LU202F( 2, 200);
	MultiSocketClose_LU202F( 2, 200);
}
		 
	monitoring_timer_ms = 10;		//send status
	monitoring_set_timer_ms = 2000;	//send set value
	Ten_Mili_Counter=0;
	Sec_Mili_Counter=0;	
	Sending_Mili_Counter=0;
	giToday = -1;
  lcd_clear(); //Delay_ms(3000);
 
 
  sprintf(str, "v_adc_or!!!: %d", v_adc_org);	USART1_puts(str);
  Led_Mili_Counter=0;
  Error_Led_Mili_Counter=0;

	
   // lcd_initial();
    //lcd_clear();
			

//0413
 /**************  Initial display *****************************/
 if(gnlSilo1Selection==1)// silo  NUMBER ==1   0413
 {
	 SendtoPc();
   Silo_1_Weight_Display(); // silo 1  current weight, feed comsumption weight displaying 
	 SendtoPc();
	 lcd_inverse();
	 if(gnlLanguage==1) // korean 
	 sprintf(str, "오 늘: %5ld kg ", gnlDailyTotal); 
	 else
	 sprintf(str, "TODAY: %5ld kg ", gnlDailyTotal);
	 
	 GLCD_string (0,6 ,str);
	 lcd_normal();

 }
else // silo number is 2  20171019
 {
	  if(gnlLanguage==1)
	 { 
	 lcd_inverse(); 
   sprintf(str, "무게:%5ld-%5ld", gxlWeight,gxlWeight_2);
	 GLCD_string (0,0 ,str);
	 lcd_normal();
	  /*
	 sprintf(str, "급이량1:%5ld kg", gnlDailyTotal);
	 GLCD_string (0,2 ,str);
	 sprintf(str, "급이량2:%5ld kg", gnlDailyTotal2);
	 */
 	 //ver 2.8
		sprintf(str, "급이량1:%5ld kg", gnlFeeding1);
	 GLCD_string (0,2 ,str);
	 sprintf(str, "급이량2:%5ld kg", gnlFeeding2);
	 GLCD_string (0,4 ,str);
	
		 lcd_inverse();
	 sum=gnlDailyTotal+gnlDailyTotal2;
	 sprintf(str, "오 늘: %5ld kg",sum);
 
		 
		 
		 //sprintf(str, "오 늘: %5ld kg ", gnlDailyTotal); 
	 GLCD_string (0,6 ,str);
	 lcd_normal();
	 }
	 else
	 { 
	 lcd_inverse(); 
   sprintf(str, "SILO:%5ld-%5ld", gxlWeight,gxlWeight_2);
	 GLCD_string (0,0 ,str);
	 lcd_normal();
	 sprintf(str, "FEED1:%5ld kg", gnlDailyTotal);
	 GLCD_string (0,2 ,str);
	 sprintf(str, "FEED2:%5ld kg", gnlDailyTotal2);
	 GLCD_string (0,4 ,str);
	 lcd_inverse();
	 sum=gnlDailyTotal+gnlDailyTotal2;
	 sprintf(str, "TODAY: %5ld kg ",sum);
 //sprintf(str, "오 늘: %5ld kg ", gnlDailyTotal); 
	 GLCD_string (0,6 ,str);
	 lcd_normal();
	 }
	  
	
	}
 /**************  Initial display *****************************/
 
//	 if(gnlSilo1Selection==1)// silo  NUMBER ==1
//		  Silo_1_Weight_Display(); // silo 1  current weight, feed comsumption weight displaying 
	
	
	
 
 /************** 이더넷 관련   처음 전원 ON  ***************************/
	
	//	RS422ON;	
		//  if(gnlSendingInterval)
      {				
	      if(gnlSilo1Selection==1)
				{
	        sprintf( sT0, "%c|01212345678|M000|77777|M%07ld|M%03ld|BC-S|%s|%lx|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%c",
					 STX,gnlSerialNo, gnlMyAddress, VERSION, gnlTick, gxlWeight, gnlDailyTotal%100000,
					 (long)gnlPreDailyTotal[0]%100000, (long)gnlPreDailyTotal[1]%100000, (long)gnlPreDailyTotal[2]%100000, (long)gnlPreDailyTotal[3]%100000,
					 (long)gnlPreDailyTotal[4]%100000, (long)gnlPreDailyTotal[5]%100000, (long)gnlPreDailyTotal[6]%100000,
					 ((working_flag)?'W':'N')
					 , gxlGrossWeight_2, gnlDailyTotal2%100000,
					 (long)gnlPreDailyTotal2[0]%100000, (long)gnlPreDailyTotal2[1]%100000, (long)gnlPreDailyTotal2[2]%100000, (long)gnlPreDailyTotal2[3]%100000,
					 (long)gnlPreDailyTotal2[4]%100000, (long)gnlPreDailyTotal2[5]%100000, (long)gnlPreDailyTotal2[6]%100000,
					 ((working_flag_2)?'W':'N'),ETX 
					 
					 );
				 }
     else
       {
	        sprintf( sT0, "%c|01212345678|M000|77777|M%07ld|M%03ld|BC-S|%s|%lx|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%c",
					 STX,gnlSerialNo, gnlMyAddress, VERSION, gnlTick, (gxlWeight+gxlWeight_2), (gnlDailyTotal+gnlDailyTotal2)%100000,
					 (long)(gnlPreDailyTotal[0]+gnlPreDailyTotal2[0])%100000, (long)(gnlPreDailyTotal[1]+gnlPreDailyTotal2[01])%100000, (long)(gnlPreDailyTotal[2]+gnlPreDailyTotal2[2])%100000, 
						 (long)(gnlPreDailyTotal[3]+gnlPreDailyTotal2[3])%100000,
					 (long)(gnlPreDailyTotal[4]+gnlPreDailyTotal2[4])%100000, (long)(gnlPreDailyTotal[5]+gnlPreDailyTotal2[5])%100000, (long)(gnlPreDailyTotal[6]+gnlPreDailyTotal2[6])%100000,
					 ((working_flag)?'W':'N')
					 , gxlGrossWeight_2, gnlDailyTotal2%100000,
					 (long)gnlPreDailyTotal2[0]%100000, (long)gnlPreDailyTotal2[1]%100000, (long)gnlPreDailyTotal2[2]%100000, (long)gnlPreDailyTotal2[3]%100000,
					 (long)gnlPreDailyTotal2[4]%100000, (long)gnlPreDailyTotal2[5]%100000, (long)gnlPreDailyTotal2[6]%100000,
					 ((working_flag_2)?'W':'N'),ETX 
					 
					 );
				 }
			 }
				 
				if(gnlSendingInterval) //설정의 전송시간에 값이 들어가 있을때만 
		      USART2_puts(sT0);
		      Sending_Mili_Counter=0;
	
	/************** 이더넷 관련   처음 전원 ON  ***************************/
	
	
	
				
				
	
 
	do
	{
			SendtoPc(); // for pc program rs_422  
		  Phone_Sms_Processing();// lte 모듈자체에 있는  원격  sms 처리 
		  IWDG_ReloadCounter(); // Reload IWDG counter
		RTC_sec_to_time_conversion();
		if(gnlDisplay) 
		{
			RTC_time_display();	
			sprintf(sT0, "gnuDay=%d, working_flag=%d, gnhToday=%d, jangbi.cdma.tm.min=%d, mCnt=%ld, INTERVAL=%ld", gnuDay, working_flag, giToday, jangbi.cdma.tm.min, Ten_Mili_Counter, GET_TIME_INTERFAL_TIME);
		}
		
		Agent_CdmaTask(); //전화기에서 온 데이터 처리 
		
/************* 입출력 처리 ****************************/
switch(Agent_DischargeInput_Task())
{
    case AGENT_DISCHARGE_TASK_FIRST_START:
    case AGENT_DISCHARGE_TASK_SECOND_START:
        goto start_func;

    default:
        break;
}

/************* 입출력 처리 ****************************/
	
		SendtoPc();

if(Agent_DischargeStopDelay_Task() == AGENT_STOP_DELAY_DONE) // stop 신호후 enddelay 처리
{
    goto stop_func;
}
Agent_BaichulSend_Task(); //배출정보 전송
	
/**********************************agent**************************************************************/

/*
			15분마다
			1. CDMA 날짜 읽어오기
			2. SMS 3개 보내기 설정시간에
			3. 날이 바뀌면, 배출날짜 계산 (D-N), 모뎀 소프트리셋
*/	

Agent_DateChange_Task();

		
/************************agent***************************/		
		
	Agent_WeightDisplay_Task();  // 무게 표시부	
		
 if(Agent_DischargeCalc_Task() == AGENT_DISCHARGE_CALC_STOP) // 배출량 계산
{
    goto stop_func;
}
	
Agent_MinuteEvent_Task();//매분마다 처리하는 부분





/******************agent****************************************/
		
/*************zero fucntion ****************************/
  	LEFT_ARROW_ON	 //0918
		{
			Zero_Mili_Counter=0;
		  beep(50);
     while(1)
		 {
			 
			 LEFT_ARROW_ON	
			{
			 //  Making_zero_timer++;
			if(Zero_Mili_Counter>(DEF_1_SEC*3))
			{
        //Making_zero_timer=0;
			  Zero_Mili_Counter=0;
				beep(500);
				zero_func();
				break;
			}
		}
				else
				{
					Zero_Mili_Counter=0;
					break;
				}
			}; // of while 	 
    } // of LEFT_ARROW_ON	
/*************zero fucntion ****************************/
		
//RIGHT_ARROW_ON  1211

if(gnlSilo1Selection==2)
{	
	RIGHT_ARROW_ON	 //0918
		{
			Zero_Mili_Counter=0;
			beep(50);
		 while(1)
		 {
 RIGHT_ARROW_ON
			{
			 //  Making_zero_timer++;
			if(Zero_Mili_Counter>(DEF_1_SEC*3))
			{
        //Making_zero_timer=0;
			  Zero_Mili_Counter=0;
				beep(500);
				zero_func2();
				break;
			}
		}
				else
				{
					Zero_Mili_Counter=0;
					break;
				}
			}; // of while 	 
    }		
	}	
		
		
		
		if (keypush_8510()) // 0830
		{
			
			switch(KeyCode)
			{	case UP:
					
				 	which_day_feeding++;
				if(which_day_feeding>8) // 원래는 7, VER 2.5 : 8, 총누적 배출량 표시하기위하여
					{
						// retry_message();
						which_day_feeding=0;
					}	
			
      	
				if(gnlSilo1Selection==1)// silo 1 enable )// silo 1 enable
				{
					 if(gnlLanguage==1)// korean
           { 	
             switch(which_day_feeding)
						 {
					     case 0:
							 sprintf(str, "오 늘: %5ld kg ",(int)gnlDailyTotal);
               break;
						   case 1:
							 sprintf(str, "1일전: %5ld kg ",(int)gnlPreDailyTotal[0]);
               break;
							 case 2:
							 sprintf(str, "2일전: %5ld kg ",(int)gnlPreDailyTotal[1]);
               break;
						   case 3:
							 sprintf(str, "3일전: %5ld kg ",(int)gnlPreDailyTotal[2]);
               break;
						   case 4:
							 sprintf(str, "4일전: %5ld kg ",(int)gnlPreDailyTotal[3]);
               break;
						   case 5:
							 sprintf(str, "5일전: %5ld kg ",(int)gnlPreDailyTotal[4]);
               break;
						   case 6:
							 sprintf(str, "6일전: %5ld kg ",(int)gnlPreDailyTotal[5]);
               break;
						   case 7:
							 sprintf(str, "7일전: %5ld kg ",(int)gnlPreDailyTotal[6]);
               break;
	            case 8:
							 sprintf(str, "총누적: %5ld kg",(int)gxhTotalOutput);
               break;
													

							 default: break;
						 
						 } //of switch
           }


					 else// ENGLISH
					 { 
              switch(which_day_feeding)
						 {
					     case 0:
							 sprintf(str, "TODAY: %5ld kg ",(int)gnlDailyTotal);
               break;
						   case 1:
							 sprintf(str, "DAY-1: %5ld kg ",(int)gnlPreDailyTotal[0]);
               break;
							 case 2:
							 sprintf(str, "DAY-2: %5ld kg ",(int)gnlPreDailyTotal[1]);
               break;
						   case 3:
							 sprintf(str, "DAY-3: %5ld kg ",(int)gnlPreDailyTotal[2]);
               break;
						   case 4:
							 sprintf(str, "DAY-4: %5ld kg ",(int)gnlPreDailyTotal[3]);
               break;
						   case 5:
							 sprintf(str, "DAY-5: %5ld kg ",(int)gnlPreDailyTotal[4]);
               break;
						   case 6:
							 sprintf(str, "DAY-6: %5ld kg ",(int)gnlPreDailyTotal[5]);
               break;
						   case 7:
							 sprintf(str, "DAY-7: %5ld kg ",(int)gnlPreDailyTotal[6]);
               break;
							 case 8:
							 sprintf(str, "TOTAL: %5ld kg ",(int)gxhTotalOutput);
               break;
							 default: break;
						 } //of switch

					 }		 
						 
				 lcd_inverse(); 
				 GLCD_string (0,6 ,str);
				 lcd_normal();  
				}	// if(gnlSilo1Selection==1)// ONLY  SILO 
        else //silo 2 ea   //0331
					{
					 if(gnlLanguage==1)// korean
           { 	
             switch(which_day_feeding)
						 {
					     case 0:
								sum= gnlDailyTotal+gnlDailyTotal2;
							 sprintf(str, "오 늘:  %5ld kg",(int)sum);
               break;
						   case 1:
								sum= gnlPreDailyTotal[0]+gnlPreDailyTotal2[0];
							 sprintf(str, "1일전:  %5ld kg",(int)sum);
               break;
							 case 2:
								 sum= gnlPreDailyTotal[1]+gnlPreDailyTotal2[1];
							 sprintf(str, "2일전:  %5ld kg",(int)sum);
               break;
						   case 3:
								 sum= gnlPreDailyTotal[2]+gnlPreDailyTotal2[2];
							 sprintf(str, "3일전:  %5ld kg",(int)sum);
               break;
						   case 4:
								 sum= gnlPreDailyTotal[3]+gnlPreDailyTotal2[3];
							 sprintf(str, "4일전:  %5ld kg",(int)sum);
               break;
						   case 5:
								 sum= gnlPreDailyTotal[4]+gnlPreDailyTotal2[4];
							 sprintf(str, "5일전:  %5ld kg",(int)sum);
               break;
						   case 6:
								 sum= gnlPreDailyTotal[5]+gnlPreDailyTotal2[5];
							 sprintf(str, "6일전:  %5ld kg",(int)sum);
               break;
						   case 7:
								 sum= gnlPreDailyTotal[6]+gnlPreDailyTotal2[6];
							 sprintf(str, "7일전:  %5ld kg",(int)sum);
               break;
							 case 8:
							 sprintf(str, "총누적: %5ld kg",(int)gxhTotalOutput);
               break;
               default: break;
						 } //of switch
           }
					 else// ENGLISH
					 { 
              switch(which_day_feeding)
						 {
					     case 0:
								 sum= gnlDailyTotal+gnlDailyTotal2;
							 sprintf(str, "TODAY: %5ld kg ",(int)gnlDailyTotal);
               break;
						   case 1:
								 sum= gnlPreDailyTotal[0]+gnlPreDailyTotal2[0];
							 sprintf(str, "DAY-1: %5ld kg ",(int)gnlPreDailyTotal[0]);
               break;
							 case 2:
								 sum= gnlPreDailyTotal[1]+gnlPreDailyTotal2[1];
							 sprintf(str, "DAY-2: %5ld kg ",(int)gnlPreDailyTotal[1]);
               break;
						   case 3:
								 sum= gnlPreDailyTotal[2]+gnlPreDailyTotal2[2];
							 sprintf(str, "DAY-3: %5ld kg ",(int)gnlPreDailyTotal[2]);
               break;
						   case 4:
								 sum= gnlPreDailyTotal[3]+gnlPreDailyTotal2[3];
							 sprintf(str, "DAY-4: %5ld kg ",(int)gnlPreDailyTotal[3]);
               break;
						   case 5:
								 sum= gnlPreDailyTotal[4]+gnlPreDailyTotal2[4];
							 sprintf(str, "DAY-5: %5ld kg ",(int)gnlPreDailyTotal[4]);
               break;
						   case 6:
								 sum= gnlPreDailyTotal[5]+gnlPreDailyTotal2[5];
							 sprintf(str, "DAY-6: %5ld kg ",(int)gnlPreDailyTotal[5]);
               break;
						   case 7:
								 sum= gnlPreDailyTotal[6]+gnlPreDailyTotal2[6];
							 sprintf(str, "DAY-7: %5ld kg ",(int)gnlPreDailyTotal[6]);
               break;
							 case 8:
							 sprintf(str, "TOTAL: %5ld kg",(int)gxhTotalOutput);
               break;
							 default: break;
						 } //of switch

					 }//silo 2 ea		 
						 
				 lcd_inverse(); 
				 GLCD_string (0,6 ,str);
				 lcd_normal();  
				}
         
				/*
				which_day_feeding++;
				 if(which_day_feeding>7)
					{
						 retry_message();
						which_day_feeding=7;
					}	
				*/  
					break;
				
				case DOWN:
					
				 which_day_feeding--;
				if(which_day_feeding<0)
					{
						// retry_message();
						which_day_feeding=8;
						break;
					}
				
  	  		
				if(gnlSilo1Selection==1)// silo 1 enable )// silo 1 enable
 				{
				if(gnlLanguage==1)// korean	
				{
		       switch(which_day_feeding) //1226
						 {
					     case 0:
							 sprintf(str, "오 늘: %5ld kg ",(int)gnlDailyTotal);
               break;
						   case 1:
							 sprintf(str, "1일전: %5ld kg ",(int)gnlPreDailyTotal[0]);
               break;
							 case 2:
							 sprintf(str, "2일전: %5ld kg ",(int)gnlPreDailyTotal[1]);
               break;
						   case 3:
							 sprintf(str, "3일전: %5ld kg ",(int)gnlPreDailyTotal[2]);
               break;
						   case 4:
							 sprintf(str, "4일전: %5ld kg ",(int)gnlPreDailyTotal[3]);
               break;
						   case 5:
							 sprintf(str, "5일전: %5ld kg ",(int)gnlPreDailyTotal[4]);
               break;
						   case 6:
							 sprintf(str, "6일전: %5ld kg ",(int)gnlPreDailyTotal[5]);
               break;
						   case 7:
							 sprintf(str, "7일전: %5ld kg ",(int)gnlPreDailyTotal[6]);
               break;
							 case 8:
							 sprintf(str, "총누적: %5ld kg",(int)gxhTotalOutput);
               break;
							 default: break;
						 
						 
						 } //of switch

				
					 }
				
				else
        {	
         switch(which_day_feeding)
						 {
					     case 0:
							 sprintf(str, "TODAY: %5ld kg ",(int)gnlDailyTotal);
               break;
						   case 1:
							 sprintf(str, "DAY-1: %5ld kg ",(int)gnlPreDailyTotal[0]);
               break;
							 case 2:
							 sprintf(str, "DAY-2: %5ld kg ",(int)gnlPreDailyTotal[1]);
               break;
						   case 3:
							 sprintf(str, "DAY-3: %5ld kg ",(int)gnlPreDailyTotal[2]);
               break;
						   case 4:
							 sprintf(str, "DAY-4: %5ld kg ",(int)gnlPreDailyTotal[3]);
               break;
						   case 5:
							 sprintf(str, "DAY-5: %5ld kg ",(int)gnlPreDailyTotal[4]);
               break;
						   case 6:
							 sprintf(str, "DAY-6: %5ld kg ",(int)gnlPreDailyTotal[5]);
               break;
						   case 7:
							 sprintf(str, "DAY-7: %5ld kg ",(int)gnlPreDailyTotal[6]);
               break;
							 case 8:
							 sprintf(str, "TOTAL: %5ld kg",(int)gxhTotalOutput);
               break;
							 default: break;
						 } //of switch
				}		
	  		 lcd_inverse(); 
				 GLCD_string (0,6 ,str);
				 lcd_normal();
	  		}	//if(gnlSilo1Selection==1)// silo 1 enable )// silo 1 enable
 		  else// SILO SELECTION ==2
				{
				if(gnlLanguage==1)// korean	
				{
		       switch(which_day_feeding)
						 {
					     case 0:
								sum= gnlDailyTotal+gnlDailyTotal2;
							 sprintf(str, "오 늘:  %5ld kg",(int)sum);
               break;
						   case 1:
								sum= gnlPreDailyTotal[0]+gnlPreDailyTotal2[0];
							 sprintf(str, "1일전:  %5ld kg",(int)sum);
               break;
							 case 2:
								 sum= gnlPreDailyTotal[1]+gnlPreDailyTotal2[1];
							 sprintf(str, "2일전:  %5ld kg",(int)sum);
               break;
						   case 3:
								 sum= gnlPreDailyTotal[2]+gnlPreDailyTotal2[2];
							 sprintf(str, "3일전:  %5ld kg",(int)sum);
               break;
						   case 4:
								 sum= gnlPreDailyTotal[3]+gnlPreDailyTotal2[3];
							 sprintf(str, "4일전:  %5ld kg",(int)sum);
               break;
						   case 5:
								 sum= gnlPreDailyTotal[4]+gnlPreDailyTotal2[4];
							 sprintf(str, "5일전:  %5ld kg",(int)sum);
               break;
						   case 6:
								 sum= gnlPreDailyTotal[5]+gnlPreDailyTotal2[5];
							 sprintf(str, "6일전:  %5ld kg",(int)sum);
               break;
						   case 7:
								 sum= gnlPreDailyTotal[6]+gnlPreDailyTotal2[6];
							 sprintf(str, "7일전 : %5ld kg",(int)sum);
               break;
							 case 8:
							 sprintf(str, "총누적: %5ld kg",(int)gxhTotalOutput);
               break;
              
							 default: break;
						 
						 } //of switch
				
					 }
				
				else
        {	
         switch(which_day_feeding)
						 {
					    case 0:
								 sum= gnlDailyTotal+gnlDailyTotal2;
							 sprintf(str, "TODAY: %5ld kg ",(int)gnlDailyTotal);
               break;
						   case 1:
								 sum= gnlPreDailyTotal[0]+gnlPreDailyTotal2[0];
							 sprintf(str, "DAY-1: %5ld kg ",(int)gnlPreDailyTotal[0]);
               break;
							 case 2:
								 sum= gnlPreDailyTotal[1]+gnlPreDailyTotal2[1];
							 sprintf(str, "DAY-2: %5ld kg ",(int)gnlPreDailyTotal[1]);
               break;
						   case 3:
								 sum= gnlPreDailyTotal[2]+gnlPreDailyTotal2[2];
							 sprintf(str, "DAY-3: %5ld kg ",(int)gnlPreDailyTotal[2]);
               break;
						   case 4:
								 sum= gnlPreDailyTotal[3]+gnlPreDailyTotal2[3];
							 sprintf(str, "DAY-4: %5ld kg ",(int)gnlPreDailyTotal[3]);
               break;
						   case 5:
								 sum= gnlPreDailyTotal[4]+gnlPreDailyTotal2[4];
							 sprintf(str, "DAY-5: %5ld kg ",(int)gnlPreDailyTotal[4]);
               break;
						   case 6:
								 sum= gnlPreDailyTotal[5]+gnlPreDailyTotal2[5];
							 sprintf(str, "DAY-6: %5ld kg ",(int)gnlPreDailyTotal[5]);
               break;
						   case 7:
								 sum= gnlPreDailyTotal[6]+gnlPreDailyTotal2[6];
							 sprintf(str, "DAY-7: %5ld kg ",(int)gnlPreDailyTotal[6]);
               break;
							 case 8:
							 sprintf(str, "TOTAL: %5ld kg",(int)gxhTotalOutput);
               break;
							 default: break;
						 } //of switch
				  }		
	  		 lcd_inverse(); 
				 GLCD_string (0,6 ,str);
				 lcd_normal();
	  		}
					break;
  
				case KEY_WEIGHT: //현재무게키 
					
				
			
///LCD_restart();	// 2017.07.19 test only
				
					/*
					Initiaize_eeprom();
					FSMC_DISABLE;	 //1126
					kwak.dw=gnlDailyTotal;
					I2C_EE_BufferWrite(kwak.c, ADR_gnlDailyTotal, 4);
					I2C_EE_BufferRead( kwak.c,ADR_gnlDailyTotal , 4);
					FSMC_ENABLE;
					*/				
					//read_factors();
					//write_factors();
					
INITIAL:
				///	lcd70_clear_screen(WHITE);
				
				///	lcd70_draw_rectangle(450, 130, 700, 230,BLUE);//오늘배출량 INNER BOX 그리기  
			///		lcd70_show_string_mode(500, 135,"TODAY OUTPUT", 16, 0);
			///		lcd70_draw_rectangle(445, 125, 705, 235,BLUE);//오늘배출량 OUTER BOX 그리기 
			///		lcd70_draw_rectangle(95, 130, 350, 230,BLUE);//현재무게 INNNER BOX
			///		lcd70_draw_rectangle(90, 125, 355, 235,RED);  //현재무게  OUTER BOX  // 2015년 1월6일
			///		lcd70_show_string_mode(150, 135,"CURRENT WEIGHT", 16, 0);
					
			/*		if(working_flag )
						lcd70_draw_demo_bin(start_lamp.pixel_data,550,400,start_lamp.width+550,start_lamp.height+400);// RED 표시부 그리기 
					else
						lcd70_draw_demo_bin(stop_lamp.pixel_data,550,400,stop_lamp.width+550,stop_lamp.height+400);// RED 표시부 그리기 
					
					Current_Display=NORMAL_DISPLAY;
			*/
					break;

				case KEY_RECORD:		  //1126	   1127
//					lcd70_clear_screen(WHITE);
					//gnlPreDailyTotal[0]=gnlDailyTotal;
					/*
					lcd43_show_big_num(250,40,0);
					lcd43_show_big_num(280,40,':');
					
					
					lcd70_write_weight(300,40,gnlDailyTotal);
					*/
					gnlPreDailyTotal[0]=gnlDailyTotal;
					for(i=0;i<7;i++)
					{ 
					 Delay_ms(10);
					///	lcd43_show_big_num(250,40+(i*60),i);
					///	lcd43_show_big_num(280,40+(i*60),':');
					///	lcd70_write_weight(300,40+(i*60),gnlPreDailyTotal[i]);// 오늘의 배출량 보내기 
					}
					Delay_ms(5000);

					goto 	INITIAL;

//					break;

start_func:   //0908
					
					if(ek_flag1==1)  // silo 1
					stop_flag = 0; 
					else                 // silo2
						stop_flag2 = 0;
					
					if (!working_flag &&ek_flag1 )
					{
						if (!byNewStart)	//ver 1.34 if new start
						{
							Error_Sec_Counter=0; //20180104   output low error TIME Initialize
							baichul_start_flag = 1;
              /*
							lPulseRef = gxlGrossWeight;
							lRefWeigh = gxlGrossWeight; // 
							lDay_Ref_Weight=gxlGrossWeight; // 2011 12/24  ver 3.04 		critical
							Start_Weight=gxlGrossWeight; //  
							*/
							// ver 1.1
							lPulseRef = gxlWeight;
							lRefWeigh = gxlWeight;
							lDay_Ref_Weight=gxlWeight; // 2011 12/24  ver 3.04 		critical
							Start_Weight=gxlWeight; //  
							TodayTotalref= gnlDailyTotal; // 이제까지의 오늘 배출량을 대입하고  
							
							//20220121
							UpperError_Weight=0;
							
						//	gnlFeeding1=0;
							
							
							
						}
		       	gnlFeeding1=0; //ver 2.8
					  lcd_inverse();
						if(gnlSilo1Selection==1)
						{
						  sprintf(str, "급 이: %5ld kg ", gnlFeeding1);
						  GLCD_string (0,2 ,str);
						}
						else //  2개일때
						{	
							lcd_normal();
						  sprintf(str, "급이량1:%5ld kg", gnlFeeding1);
	            GLCD_string (0,2 ,str);
	           
						}
					//		sprintf(str, "급이량2:%5ld kg", gnlFeeding2);
	         //  GLCD_string (0,4 ,str);
	
						
						lcd_normal();
						/*
						lcd_inverse();
						
						sprintf(str, "급 이: %5ld kg ", gnlFeeding1);
						lcd_normal();
						GLCD_string (0,2 ,str);
						*/
						working_flag = 1;
						//bachul_send_timer_ms = 10;	//시작시 배출 정보 전송	2016.4.5
						bachul_send_timer_ms = WORKING_SEND_INTERVAL;	//배출중  정보 전송 시간 초기화
					//bErrorCheck = 1;
						//bStartOn = TRUE;
						//bReStart = FALSE;
					} // !working_flag
					
					if (!working_flag_2&& ek_flag2)
					{
						if (!byNewStart2)	//ver 1.34 if new start
						{
							baichul_start_flag2 = 1;
              /*
							lPulseRef2 = gxlGrossWeight_2;
							lRefWeigh2 = gxlGrossWeight_2;
							lDay_Ref_Weight2=gxlGrossWeight_2; // 2011 12/24  ver 3.04 		critical
							Start_Weight2=gxlGrossWeight_2; //  
							 */
							//ver 1.1  20180212
							lPulseRef2 = gxlWeight_2;
							lRefWeigh2 = gxlWeight_2;
							lDay_Ref_Weight2=gxlWeight_2; // 2011 12/24  ver 3.04 		critical
							Start_Weight2=gxlWeight_2; //  
					
							
							TodayTotalref2= gnlDailyTotal2; // 이제까지의 오늘 배출량을 대입하고  
							
							//gnlFeeding2=0;
							
						}
          //0713		       
						if(gnlSilo1Selection==2)
					 { 
					 
						 gnlFeeding2=0;
					   sprintf(str, "급이량2:%5ld kg", gnlFeeding2);
	           GLCD_string (0,4 ,str);
	         
						  sprintf(str, "start_gnlFeeding2=%5ld", gnlFeeding2); USART1_puts(str);
						  sprintf(str, "start_lPulseRef2=%5ld", lPulseRef2); USART1_puts(str);
						  sprintf(str, "start_gxlWeight_2=%5ld", gxlWeight_2); USART1_puts(str);
						 
						 
						 
						 
					 }
						
						working_flag_2 = 1;
						//bachul_send_timer_ms = 10;	//시작시 배출 정보 전송	2016.4.5
						bachul_send_timer_ms = WORKING_SEND_INTERVAL;	//배출중  정보 전송 시간 초기화
					//bErrorCheck = 1;
						//bStartOn = TRUE;
						//bReStart = FALSE;
					} // !working_flag
					
					
					
					break;
					
stop_func:   //20171020
					
					Agent_DischargeStop_Task();
			
 				
					break;

	



				case KEY_STATUS: //장비상태 
					sprintf(sT0, "DEVICE_TEL:%s", jangbi.cdma.telno);
					Reg_Tel[11] = 0;
					sprintf(sT0, "TEL=%s", Reg_Tel);
					sprintf(sT0, "SMS_TIME=%c%c%c%c", Sms_Time[0], Sms_Time[1], Sms_Time[2], Sms_Time[3] );
					
					Current_Display=STATUS_DISPLAY;
					Delay_ms(6000);
					goto INITIAL;					
				default:
					break;
			} //switch(ch)
		} //if (keypush())
	 
		
	#ifdef dpddp	
		
		//매분마다 실행하는 기능
		if(gbMinEvent) // 20171203  at rtc.c
		{
			
		 Get1381(); // 시간을 읽어오고 
			HT1381_year=Gettimebuf[6];
			HT1381_month=Gettimebuf[4];
			HT1381_date=Gettimebuf[3];
			HT1381_hour=Gettimebuf[2];
			HT1381_minute=Gettimebuf[1];
			HT1381_second=Gettimebuf[0];
				
			sprintf(str, " : giToday=%02x   1381_DATA=%02X",giToday, HT1381_date);	
		  USART1_puts(str);  
				
			
			
			
			
			if(gnlCdmaUse)//전화기 사용하면 
			{
			 	gbMinEvent = 0;
			//websms 서버 연결 확인 및 재연결
			gbWebSMSConnection = 1;
			//cdma 시간 읽어서 지그비로 동기시간 날리기
			gbGetCdmaTime = 1;
			if(gnlUseLTE)
			{
				sprintf( sT0,"AT+BC=$DATE=%04d%02d%02d%02d%02d%02d@", jangbi.cdma.tm.year, jangbi.cdma.tm.month, jangbi.cdma.tm.date, rtc_hour, rtc_min, rtc_sec);
				UART4_puts(sT0);
        USART1_puts(sT0);
			}
			
			//시간동기화 될동안 day-x 계산 날짜 업데이트 - 저녁 12시 근처이면 안될 수도 있음.
			if(poweron_date_set_counter)
			{
				poweron_date_set_counter--;
				///giToday = rtc_date;
				 giToday = HT1381_date;
			}
				
			}

      else // 전화기 사용 안하면 
      {				
			SendtoPc();
			gbMinEvent = 0;
			
			USART1_puts("One minute rotaion");
			if(poweron_date_set_counter) //1204
			{
				poweron_date_set_counter--;
				giToday = HT1381_date;
				
				sprintf(str, "One_minutes rotation : giToday=%02x   1381_DATA=%02X",giToday, HT1381_date);	
		    USART1_puts(str);
			}
			}
	

		} //매분마다 실행하는 기능
		
		#endif
   
	} while(1);
} /* main */




void SendtoPc(void)  // 20171108
{ 
	#define  STX 0X02
	#define  ETX 0X03
  #define  REQ 0X05
	char  str[50];
	long sum=0;
	
	RS422OFF;
 
 /******** Request from pc program *******************/
	// for test: @ ID CMD !
	// for real: REQ ID CMD ETX
  //	if((cmd_2[0]/0x30)==(gnlMyAddress/10) &&(cmd_2[1]/0x30)==(gnlMyAddress%10))
 if(Usart2_Interrupt_flag==1)//1204
 {	 
	//RS422ON; Delay_ms(30);
	 //#define	RS422ON		GPIO_SetBits(GPIOA, GPIO_Pin_4)
//#define	RS422OFF	GPIO_ResetBits(GPIOA, GPIO_Pin_4)

	if((cmd_2[0]-0x30)==(gnlMyAddress/10) &&(cmd_2[1]-0x30)==(gnlMyAddress%10))
	//	if((cmd_2[1]-0x30)==(gnlMyAddress%10))
	{
	 RS422ON;	
	 Delay_ms(50); // request from Mr park
		
	 switch (cmd_2[2] * 0x10000L + cmd_2[3] * 0x100L + cmd_2[4]) //1204
	{
	 case 'C'*0x10000L+'W'*0x100L+'T':	//Current WeighT//43   57  54
		 
	 USART1_puts("CWT");
      USART2_putchar(0x02);   //head
      USART2_putchar((gnlMyAddress/10)+0x30);
      USART2_putchar((gnlMyAddress%10)+0x30);	 
	    USART2_putchar(cmd_2[2]);
	    USART2_putchar(cmd_2[3]);
	    USART2_putchar(cmd_2[4]);
	   //1205 
	 if(gnlSilo1Selection==1)//SILO 갯수가 1개
	 {
	   if(sign)
			sprintf (str, "-%7lu", (long)gxlWeight);
			  else
			sprintf (str, " %7lu", (long)gxlWeight);
	 }   
	 else// silo 갯수가 2개
	 {
		 if(sign_2) gxlWeight_2=0;
		 if(sign)   gxlWeight=0;
		 sum=gxlWeight+gxlWeight_2;
		 sprintf (str, " %7lu", (long)sum);
	 } 
     //gxlWeight_2			
		  USART2_puts_rs422(str);
			USART2_putchar(0x03);
			USART2_putchar(0);
				
			//	USART2_puts_rs422
	 
	 
	    //sprintf(str, "CWT%5ld", (long)gxlWeight);
	    //USART2_puts(str);
	    //USART2_putchar(0x03);   //tail
  	    break;
	 
	 case 'D'*0x10000L+'-'*0x100L+'0':	//Current WeighT
		 USART1_puts("D-0");
		  USART2_putchar(0x02);   //head
      USART2_putchar((gnlMyAddress/10)+0x30);
      USART2_putchar((gnlMyAddress%10)+0x30);	 
	    USART2_putchar(cmd_2[2]);
	    USART2_putchar(cmd_2[3]);
	    USART2_putchar(cmd_2[4]);
			if(gnlSilo1Selection==1)//SILO 갯수가 1개
	     sprintf (str, "%8lu", (long)gnlDailyTotal);
		  else
			{
       sum=gnlDailyTotal+gnlDailyTotal2;
				sprintf (str, "%8lu", (long)sum);
      }				
	    USART2_puts_rs422(str);
			USART2_putchar(0x03);
			USART2_putchar(0);
	       break;
 		case 'D'*0x10000L+'A'*0x100L+'T':	//DATe
			USART1_puts("DAT");
		  USART2_putchar(0x02);   //head
      USART2_putchar((gnlMyAddress/10)+0x30);
      USART2_putchar((gnlMyAddress%10)+0x30);	 
	    USART2_putchar(cmd_2[2]);
	    USART2_putchar(cmd_2[3]);
	    USART2_putchar(cmd_2[4]);
		  sprintf (str, "%2X.%02X.%02X", HT1381_year, HT1381_month, HT1381_date);
			USART2_puts_rs422(str);
			USART2_putchar(0x03);
			USART2_putchar(0);
	  break;
	
    	case 'T'*0x10000L+'I'*0x100L+'M':	//DATe
				USART1_puts("TIM");
		  USART2_putchar(0x02);   //head
      USART2_putchar((gnlMyAddress/10)+0x30);
      USART2_putchar((gnlMyAddress%10)+0x30);	 
	    USART2_putchar(cmd_2[2]);
	    USART2_putchar(cmd_2[3]);
	    USART2_putchar(cmd_2[4]);
		  sprintf (str, "%2X.%02X.%02X", HT1381_hour, HT1381_minute, HT1381_second);
			USART2_puts_rs422(str);
			USART2_putchar(0x03);
			USART2_putchar(0);
	  break;
	

		
			/*
			
			 HT1381_year=Gettimebuf[6];
			HT1381_month=Gettimebuf[4];
			HT1381_date=Gettimebuf[3];
			HT1381_hour=Gettimebuf[2];
			HT1381_minute=Gettimebuf[1];
			HT1381_second=Gettimebuf[0];
		
			case 'D'*0x10000L+'A'*0x100L+'T':	//DATe
				SendDataEnable = 0xFF;
				sprintf (&SendBuffer[6], "%2d.%02d.%02d", (int)RTCYEAR, (int)RTCMONTH, (int)RTCDATE);
				SendBuffer[14] = ETX;
				SendBuffer[15] = 0;
				break;
			case 'T'*0x10000L+'I'*0x100L+'M':	//TIMe
				SendDataEnable = 0xFF;
				sprintf (&SendBuffer[6], "%2d.%02d.%02d", (int)RTCHOURS, (int)RTCMINUTES, (int)RTCSECONDS);
				SendBuffer[14] = ETX;
				SendBuffer[15] = 0;
				break;
			
			*/
			
			
			
			
			
			
	 	
	case 'D'*0x10000L+'-'*0x100L+'1':	//Current WeighT
		USART1_puts("D-1");
	    USART2_putchar(0x02);   //head
      USART2_putchar((gnlMyAddress/10)+0x30);
      USART2_putchar((gnlMyAddress%10)+0x30);	 
	    USART2_putchar(cmd_2[2]);
	    USART2_putchar(cmd_2[3]);
	    USART2_putchar(cmd_2[4]);
			if(gnlSilo1Selection==1)//SILO 갯수가 1개
			sprintf (str, "%8lu", (long)gnlPreDailyTotal[0]);
       else
			 {
        sum=gnlPreDailyTotal[0]+gnlPreDailyTotal2[0];
				 sprintf (str, "%8lu", (long)sum);
       }				 


			USART2_puts_rs422(str);
			USART2_putchar(0x03);
			USART2_putchar(0);
			 
	
	      break;
	 
	case 'D'*0x10000L+'-'*0x100L+'2':	//Current WeighT
		USART1_puts("D-2");
	      // USART2_puts("D-2 Result sending!!");
	       USART2_putchar(0x02);   //head
      USART2_putchar((gnlMyAddress/10)+0x30);
      USART2_putchar((gnlMyAddress%10)+0x30);	 
	    USART2_putchar(cmd_2[2]);
	    USART2_putchar(cmd_2[3]);
	    USART2_putchar(cmd_2[4]);
			//sprintf (str, "%8lu", (long)gnlPreDailyTotal[1]);
		if(gnlSilo1Selection==1)//SILO 갯수가 1개
			sprintf (str, "%8lu", (long)gnlPreDailyTotal[1]);
       else
			 {
        sum=gnlPreDailyTotal[1]+gnlPreDailyTotal2[1];
				 sprintf (str, "%8lu", (long)sum);
       }				 

		  USART2_puts_rs422(str);
			USART2_putchar(0x03);
			USART2_putchar(0);
			  
	
	      break;
	 
	case 'D'*0x10000L+'-'*0x100L+'3':	//Current WeighT
		USART1_puts("D-3");
	      //USART2_puts("D-3 Result sending!!");
	 USART2_putchar(0x02);   //head
      USART2_putchar((gnlMyAddress/10)+0x30);
      USART2_putchar((gnlMyAddress%10)+0x30);	 
	    USART2_putchar(cmd_2[2]);
	    USART2_putchar(cmd_2[3]);
	    USART2_putchar(cmd_2[4]);
			//sprintf (str, "%8lu", (long)gnlPreDailyTotal[2]);
		  if(gnlSilo1Selection==1)//SILO 갯수가 1개
			sprintf (str, "%8lu", (long)gnlPreDailyTotal[2]);
       else
			 {
        sum=gnlPreDailyTotal[2]+gnlPreDailyTotal2[2];
				 sprintf (str, "%8lu", (long)sum);
       }				 

		  USART2_puts_rs422(str);
			USART2_putchar(0x03);
			USART2_putchar(0);
			
	      break;
	case 'D'*0x10000L+'-'*0x100L+'4':	//Current WeighT
		USART1_puts("D-4");
		   USART2_putchar(0x02);   //head
      USART2_putchar((gnlMyAddress/10)+0x30);
      USART2_putchar((gnlMyAddress%10)+0x30);	 
	    USART2_putchar(cmd_2[2]);
	    USART2_putchar(cmd_2[3]);
	    USART2_putchar(cmd_2[4]);
			//sprintf (str, "%8lu", (long)gnlPreDailyTotal[3]);
	  if(gnlSilo1Selection==1)//SILO 갯수가 1개
			sprintf (str, "%8lu", (long)gnlPreDailyTotal[3]);
       else
			 {
        sum=gnlPreDailyTotal[3]+gnlPreDailyTotal2[3];
				 sprintf (str, "%8lu", (long)sum);
       }				 

		  USART2_puts_rs422(str);
			USART2_putchar(0x03);
			USART2_putchar(0);
			
       // USART2_puts("D-4 Result sending!!");
	       break;
	case 'D'*0x10000L+'-'*0x100L+'5':	//Current WeighT
		USART1_puts("D-5");
		     USART2_putchar(0x02);   //head
      USART2_putchar((gnlMyAddress/10)+0x30);
      USART2_putchar((gnlMyAddress%10)+0x30);	 
	    USART2_putchar(cmd_2[2]);
	    USART2_putchar(cmd_2[3]);
	    USART2_putchar(cmd_2[4]);
			//sprintf (str, "%8lu", (long)gnlPreDailyTotal[4]);
	  if(gnlSilo1Selection==1)//SILO 갯수가 1개
			sprintf (str, "%8lu", (long)gnlPreDailyTotal[4]);
       else
			 {
        sum=gnlPreDailyTotal[4]+gnlPreDailyTotal2[4];
				 sprintf (str, "%8lu", (long)sum);
       }				 

		  USART2_puts_rs422(str);
			USART2_putchar(0x03);
			USART2_putchar(0);
			
		     //USART2_puts("D-5 Result sending!!");
	      break;
	case 'D'*0x10000L+'-'*0x100L+'6':	//Current WeighT
		USART1_puts("D-6");
	      // USART2_puts("D-6 Result sending!!"); 
	 USART2_putchar(0x02);   //head
      USART2_putchar((gnlMyAddress/10)+0x30);
      USART2_putchar((gnlMyAddress%10)+0x30);	 
	    USART2_putchar(cmd_2[2]);
	    USART2_putchar(cmd_2[3]);
	    USART2_putchar(cmd_2[4]);
			//sprintf (str, "%8lu", (long)gnlPreDailyTotal[5]);
	  if(gnlSilo1Selection==1)//SILO 갯수가 1개
			sprintf (str, "%8lu", (long)gnlPreDailyTotal[5]);
       else
			 {
        sum=gnlPreDailyTotal[5]+gnlPreDailyTotal2[5];
				 sprintf (str, "%8lu", (long)sum);
       }				 

		  USART2_puts_rs422(str);
			USART2_putchar(0x03);
			USART2_putchar(0);
			
	       break;
	case 'D'*0x10000L+'-'*0x100L+'7':	//Current WeighT
		USART1_puts("D-7");
	      //USART2_puts("D-7 Result sending!!");
	     USART2_putchar(0x02);   //head
      USART2_putchar((gnlMyAddress/10)+0x30);
      USART2_putchar((gnlMyAddress%10)+0x30);	 
	    USART2_putchar(cmd_2[2]);
	    USART2_putchar(cmd_2[3]);
	    USART2_putchar(cmd_2[4]);
			//sprintf (str, "%8lu", (long)gnlPreDailyTotal[6]);
	  if(gnlSilo1Selection==1)//SILO 갯수가 1개
			sprintf (str, "%8lu", (long)gnlPreDailyTotal[6]);
       else
			 {
        sum=gnlPreDailyTotal[6]+gnlPreDailyTotal2[6];
				 sprintf (str, "%8lu", (long)sum);
       }				 

		  USART2_puts_rs422(str);
			USART2_putchar(0x03);
			USART2_putchar(0);
			
	      break;
	
	default:
   break;		
		
}
	/*
cmd_2[0]=0XFF;
cmd_2[1]=0XFF;
cmd_2[2]=0XFF;
cmd_2[3]=0XFF;
cmd_2[4]=0XFF;
cmd_2[5]=0XFF;
	
Usart2_Interrupt_flag=0;
	*/
// Delay_ms(50);
 //RS422OFF;

//	Usart2_Interrupt_flag=0;
  Delay_ms(50);
  RS422OFF;
	
	}
	
	
cmd_2[0]=0XFF;
cmd_2[1]=0XFF;
cmd_2[2]=0XFF;
cmd_2[3]=0XFF;
cmd_2[4]=0XFF;
cmd_2[5]=0XFF;
	
RxBuffer2[0]=0XFF;
RxBuffer2[1]=0XFF;
RxBuffer2[2]=0XFF;
RxBuffer2[3]=0XFF;
RxBuffer2[4]=0XFF;
RxBuffer2[5]=0XFF;
RxBuffer2[6]=0XFF;
RxBuffer2[7]=0XFF;

	
	
	
	
Usart2_Interrupt_flag=0;
	
// Delay_ms(10);
 //RS422OFF;
	
}	//if(Usart2_Interrupt_flag==1)//1204
	

}


void SMS_PROCESS(void)
{	
	int	iz;
	char telephone_number[20];
	union { u32 dw; u16 u[2]; u8 c[4];} temp;
	char sT0[100];

//	Send_to_server(SMS_RECEIVED, RxBuffer2_sms);

	//#RES_MODEMLONE01034431529@(26 BYTE)	//CDMA RESET
	//#REG_TELEPHONE01034431529@(26 BYTE)	//배출량 보내는 전화번호등록 
	//#DEL_TELEPHONE01034431529@(26 BYTE)	//등록된 전화번호 삭제
	//#AUT_SENDINGTIME_SMS_1234@(26 BYTE)	//DATA 전송하는 시간 등록(자동전송)
	//#MAN_SENDINGTI01034431529@(26 BYTE)	//SMS 를 통한 DATA 전송 
	//#REMOTE_PROGRAM_UPLOADING@(26 BYTE)	//원격 펌웨어 업데이터 
	//#IP=123.123.123.123:12345@(26 BYTE)	//펌웨어 업데이트 IP 저장 
	
/*
	sprintf(sT0, "sms=%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c  c=%2d",
                   RxBuffer2_sms[RxCounter2_sms-28],	   
                   RxBuffer2_sms[RxCounter2_sms-27],	   
                   RxBuffer2_sms[RxCounter2_sms-26],	   
				   RxBuffer2_sms[RxCounter2_sms-25],
				   RxBuffer2_sms[RxCounter2_sms-24],
				   RxBuffer2_sms[RxCounter2_sms-23],
				   
                   RxBuffer2_sms[RxCounter2_sms-22],	   
                   RxBuffer2_sms[RxCounter2_sms-21],	   
				   RxBuffer2_sms[RxCounter2_sms-20],
				   RxBuffer2_sms[RxCounter2_sms-19],
				   RxBuffer2_sms[RxCounter2_sms-18],
				   RxBuffer2_sms[RxCounter2_sms-17],
				   RxBuffer2_sms[RxCounter2_sms-16],
				   RxBuffer2_sms[RxCounter2_sms-15],
				   RxBuffer2_sms[RxCounter2_sms-14],
				   RxBuffer2_sms[RxCounter2_sms-13],
				   RxBuffer2_sms[RxCounter2_sms-12],
				   RxBuffer2_sms[RxCounter2_sms-11],	   
				   RxBuffer2_sms[RxCounter2_sms-10],
				   RxBuffer2_sms[RxCounter2_sms-9],
				   RxBuffer2_sms[RxCounter2_sms-8],
				   RxBuffer2_sms[RxCounter2_sms-7],
				  // RxBuffer2_sms[RxCounter2_sms-6],
				  // RxBuffer2_sms[RxCounter2_sms-5],
				  // RxBuffer2_sms[RxCounter2_sms-4],
				  // RxBuffer2_sms[RxCounter2_sms-3],
				  // RxBuffer2_sms[RxCounter2_sms-2],
				   RxCounter2_sms
	);
	lcd70_show_string_mode(200, 100, (const unsigned char *)sT0, 20, 0);
*/
//	sprintf(sT0, "sms=%s ::c=%2d", RxBuffer2_sms, RxCounter2_sms);
//	lcd70_show_string_mode(200, 100, (const unsigned char *)sT0, 20, 0);

		   //1127
		   //1222
		   
DEBUG_puts("SMS RECEIVED");
DEBUG_puts(RxBuffer2_sms);

//	if(RxBuffer2_sms[RxCounter2_sms-28]=='R' &&  RxBuffer2_sms[RxCounter2_sms-27]=='E' && RxBuffer2_sms[RxCounter2_sms-26]=='S')
	if(RxBuffer2_sms[1]=='R' &&  RxBuffer2_sms[2]=='E' && RxBuffer2_sms[3]=='S')
	{
		//lcd70_show_string_mode(300, 50,"MODEM RESET !!!", 20, 0);
		cdma_send_cmd("AT$LGTRESET", 11);	//2014.10.27
	}
/**	
	else if(RxBuffer2_sms[1]=='I' &&  RxBuffer2_sms[2]=='A' && RxBuffer2_sms[3]=='P')
	{
		lcd70_show_string_mode(300, 50,"FIRMWARE DOWNLOAD  !!!", 20, 0);
		goto_iap(TMS_SERVER_IP, TMS_SERVER_PORT); 
		//cdma_send_cmd("AT$LGTRESET", 11);	//2014.10.27
	}
***/	
	else if(RxBuffer2_sms[1]=='R' &&  RxBuffer2_sms[2]=='E' && RxBuffer2_sms[3]=='G')
	{
		//lcd70_show_string_mode(300, 50,"TELEPHONE_REGISTRATION", 20, 0);
		
		//long Tel_Number_1;//     01
		//long Tel_Number_2;//     3443 
		//long Tel_Number_3;//     1529 
		//Reg_Tel[20]
		
/* 2016.4.20
		Initiaize_eeprom();
		FSMC_DISABLE;
		
		Delay_ms(1000);
		
		//temp.dw=v_zero;
		temp.c[0]=RxBuffer2_sms[RxCounter2_sms-15];//0
		temp.c[1]=RxBuffer2_sms[RxCounter2_sms-14];//0
		temp.c[2]=RxBuffer2_sms[RxCounter2_sms-13];//0
		
		
		I2C_EE_BufferWrite(temp.c, ADR_Tel_Number_1, 4);
		I2C_EE_BufferRead( temp.c, ADR_Tel_Number_1, 4);
		
		Reg_Tel[0]=temp.c[0];
		Reg_Tel[1]=temp.c[1];
		Reg_Tel[2]=temp.c[2];
		


		     
             temp.c[0]=RxBuffer2_sms[RxCounter2_sms-12];//0
			 temp.c[1]=RxBuffer2_sms[RxCounter2_sms-11];//0
			 temp.c[2]=RxBuffer2_sms[RxCounter2_sms-10];//0
			 temp.c[3]=RxBuffer2_sms[RxCounter2_sms-9];//0
			 I2C_EE_BufferWrite(temp.c, ADR_Tel_Number_2, 4);
	         I2C_EE_BufferRead( temp.c, ADR_Tel_Number_2, 4);

			 Reg_Tel[3]=temp.c[0];
			 Reg_Tel[4]=temp.c[1];
             Reg_Tel[5]=temp.c[2];
			 Reg_Tel[6]=temp.c[3];

			 temp.c[0]=RxBuffer2_sms[RxCounter2_sms-8];//0
			 temp.c[1]=RxBuffer2_sms[RxCounter2_sms-7];//0
			 temp.c[2]=RxBuffer2_sms[RxCounter2_sms-6];//0
			 temp.c[3]=RxBuffer2_sms[RxCounter2_sms-5];//0
			 I2C_EE_BufferWrite(temp.c, ADR_Tel_Number_3, 4);
	         I2C_EE_BufferRead( temp.c, ADR_Tel_Number_3, 4);
		
			 Reg_Tel[7]=temp.c[0];
			 Reg_Tel[8]=temp.c[1];
             Reg_Tel[9]=temp.c[2];
			 Reg_Tel[10]=temp.c[3];
			 Reg_Tel[11]=0;
	          FSMC_ENABLE;
*/
/*
		Reg_Tel[0] = temp.c[0] = RxBuffer2_sms[RxCounter2_sms-15];//0
		Reg_Tel[1] = temp.c[0] = RxBuffer2_sms[RxCounter2_sms-14];//0
		Reg_Tel[2] = temp.c[0] = RxBuffer2_sms[RxCounter2_sms-13];//0
		EE_Write_val(ADR_Tel_Number_1, temp.dw, 4);

		Reg_Tel[3] = temp.c[0] = RxBuffer2_sms[RxCounter2_sms-12];//0
		Reg_Tel[4] = temp.c[1] = RxBuffer2_sms[RxCounter2_sms-11];//0
		Reg_Tel[5] = temp.c[2] = RxBuffer2_sms[RxCounter2_sms-10];//0
		Reg_Tel[6] = temp.c[3] = RxBuffer2_sms[RxCounter2_sms-9];//0
		EE_Write_val(ADR_Tel_Number_2, temp.dw, 4);

		Reg_Tel[7] = temp.c[0] = RxBuffer2_sms[RxCounter2_sms-8];//0
		Reg_Tel[8] = temp.c[1] = RxBuffer2_sms[RxCounter2_sms-7];//0
		Reg_Tel[9] = temp.c[2] = RxBuffer2_sms[RxCounter2_sms-6];//0
		Reg_Tel[10] = temp.c[3] = RxBuffer2_sms[RxCounter2_sms-5];//0
		Reg_Tel[11]=0;
		EE_Write_val(ADR_Tel_Number_3, temp.dw, 4);
*/
		sprintf(sT0, "TEL=%s", Reg_Tel);
		//lcd70_show_string_mode(300, 80, (const unsigned char *)sT0, 20, 0);
	}
    else if(RxBuffer2_sms[RxCounter2_sms-28]=='D' &&  RxBuffer2_sms[RxCounter2_sms-27]=='E' && RxBuffer2_sms[RxCounter2_sms-26]=='L')
	{
		//lcd70_show_string_mode(300, 50,"TELEPHONE_DELETION !!!", 20, 0);
	}
	else if(RxBuffer2_sms[RxCounter2_sms-28]=='A' &&  RxBuffer2_sms[RxCounter2_sms-27]=='U' && RxBuffer2_sms[RxCounter2_sms-26]=='T')
//	else if(RxBuffer2_sms[1]=='A' &&  RxBuffer2_sms[2]=='U' && RxBuffer2_sms[3]=='T')
	{
		//lcd70_show_string_mode(300, 50,"DATA_SENDING_TIME  !!!", 20, 0);
/* 2016.4.20		
		Initiaize_eeprom();
		FSMC_DISABLE;
		
		temp.c[0]=RxBuffer2_sms[RxCounter2_sms-8];//0
		temp.c[1]=RxBuffer2_sms[RxCounter2_sms-7];//0
		temp.c[2]=RxBuffer2_sms[RxCounter2_sms-6];//0
		temp.c[3]=RxBuffer2_sms[RxCounter2_sms-5];//0
		
		Delay_ms(1000);
		I2C_EE_BufferWrite(temp.c, ADR_gnlSms_Send_Time, 4);
		I2C_EE_BufferRead( temp.c, ADR_gnlSms_Send_Time, 4);
		FSMC_ENABLE; 
		
		Sms_Time[0]=temp.c[0];
		Sms_Time[1]=temp.c[1];
		Sms_Time[2]=temp.c[2];
		Sms_Time[3]=temp.c[3];
*/		
//		FSMC_DISABLE;
//		Delay_ms(1000);
/*
		Sms_Time[0] = temp.c[0] = RxBuffer2_sms[RxCounter2_sms-8];
		Sms_Time[1] = temp.c[1] = RxBuffer2_sms[RxCounter2_sms-7];
		Sms_Time[2] = temp.c[2] = RxBuffer2_sms[RxCounter2_sms-6];
		Sms_Time[3] = temp.c[3] = RxBuffer2_sms[RxCounter2_sms-5];
		
		EE_Write_val(ADR_gnlSms_Send_Time, temp.dw, 4);
*/
//		Delay_ms(1);
//  		FSMC_ENABLE; 

		sprintf(sT0, "SMS TIME=%lu.%lu.%c%c%c%c", temp.dw, (u32 *)Sms_Time, Sms_Time[0], Sms_Time[1], Sms_Time[2], Sms_Time[3]);
		//lcd70_show_string_mode(300, 20, (const unsigned char *)sT0, 20, 0);
	}
   	// 실제무게 FACTOR 값 :: #RFAC=12345@
   	//else if(RxBuffer2_sms[RxCounter2_sms-28]=='F' &&  RxBuffer2_sms[RxCounter2_sms-27]=='A' && RxBuffer2_sms[RxCounter2_sms-26]=='C')
	else if( !strncmp(RxBuffer2_sms, "#RFAC=", 6))
	{
		sms_process(RxBuffer2_sms);
/*		
		lcd70_show_string_mode(300, 50,"REAL_WEIGHT_FACTOR  !!!", 20, 0);
		
		RxBuffer2_sms[11] = 0;
		Real_Weight_Factor = EE_Write_val(ADR_Real_Weight_Factor, atol(&RxBuffer2_sms[6]), 4);
		
		sprintf(sT0, "FACTOR=%ld", (long)Real_Weight_Factor);
		lcd70_show_string_mode(300, 80, (const unsigned char *)sT0, 20, 0);
*/		
	}
   	// 실제무게 값 :: #RWEIGHT=12345@
	else if( !strncmp(RxBuffer2_sms, "#RWEIGHT=", 8))
	{
		sms_process(RxBuffer2_sms);
	}
   	// 영점설정 :: #RZERO=12345@
	else if( !strncmp(RxBuffer2_sms, "#RZERO=", 6))
	{
		sms_process(RxBuffer2_sms);
	}
   	// 상태정보 전송간격 설정(분) :: #RSTTIME=12345@
	else if( !strncmp(RxBuffer2_sms, "#RSTTIME=", 8))
	{
		sms_process(RxBuffer2_sms);
	}
	// 로드셀 죽었을 때 보상 ON/OFF/
	else if( !strncmp(RxBuffer2_sms, "#RLCCOMP=", 8))
	{
		sms_process(RxBuffer2_sms);
	}
	else if( !strncmp(RxBuffer2_sms, "#IAP=", 4))
	{
	//	lcd70_show_string_mode(300, 50,"FIRMWARE DOWNLOAD  !!!", 20, 0);
		goto_iap(TMS_SERVER_IP, TMS_SERVER_PORT); 
	}
	else if(RxBuffer2_sms[RxCounter2_sms-28]=='M' &&  RxBuffer2_sms[RxCounter2_sms-27]=='A' && RxBuffer2_sms[RxCounter2_sms-26]=='N')
	{
	//	lcd70_show_string_mode(300, 50,"MANUAL DATA SENDING  !!!", 20, 0);	//1127
		
		telephone_number[0]=RxBuffer2_sms[RxCounter2_sms-15];	  //0
		telephone_number[1]=RxBuffer2_sms[RxCounter2_sms-14];	  //1
		telephone_number[2]=RxBuffer2_sms[RxCounter2_sms-13];	  //0
		
		telephone_number[3]=RxBuffer2_sms[RxCounter2_sms-12];	  //3
		telephone_number[4]=RxBuffer2_sms[RxCounter2_sms-11];	  //4
		telephone_number[5]=RxBuffer2_sms[RxCounter2_sms-10];	  //4
		telephone_number[6]=RxBuffer2_sms[RxCounter2_sms-9];	  //3
		
		telephone_number[7]=RxBuffer2_sms[RxCounter2_sms-8];	 //1
		telephone_number[8]=RxBuffer2_sms[RxCounter2_sms-7];	 //5
		telephone_number[9]=RxBuffer2_sms[RxCounter2_sms-6];	 //2
		telephone_number[10]=RxBuffer2_sms[RxCounter2_sms-5];	 //9
		telephone_number[11]=0; //NULL

		/*
		telephone_number[0]='0';
		telephone_number[1]='1';
		telephone_number[2]='0';
		
		telephone_number[3]='3';
		telephone_number[4]='4';
		telephone_number[5]='4';
		telephone_number[6]='3';
		
		telephone_number[7]='1';
		telephone_number[8]='5';
		telephone_number[9]='2';
		telephone_number[10]='9';
		telephone_number[11]=0;
		*/

		Data_Send_Sms(telephone_number);
		cdma_send_cmd("AT$LGTACK", 9); 	//2014.10.21 받았다는 것을 보내서 문자가 한번만 날아오게 한다. 
	}
	
	//zigbee로 전송 @#ZIGBEE=ID,$MSG@
	else if( !strncmp(RxBuffer2_sms, "#ZIGBEE=", 8))
	{
		//lcd70_show_string_mode(300, 50,"ZIGBEE !!!", 20, 0);
		
		sprintf(sT0, "AT+UC=%s", &RxBuffer2_sms[8]);
		ZIGBEE_puts(sT0);
	}

//	RxBuffer2_sms[RxCounter2_sms-28]=0XFF;
//	RxBuffer2_sms[RxCounter2_sms-27]=0XFF;
//	RxBuffer2_sms[RxCounter2_sms-26]=0XFF;

	for(iz=0; iz<MAX_RXBUFFER2_SMS; iz++) RxBuffer2_sms[iz] = 0;
	
sprintf(sT0, "Usart3_sms_received_flag=%d, ", Usart3_sms_received_flag);
//lcd70_show_string_mode(0, 20,sT0, 16, 0);	//1127
	
	Usart3_sms_received_flag=0;
	RxCounter2_sms = 0;
	Delay_ms(1000);
sprintf(sT0, "Usart3_sms_received_flag=%d, ", Usart3_sms_received_flag);
//lcd70_show_string_mode(300, 20,sT0, 16, 0);	//1127
	
//	lcd70_show_string_mode(300, 50,"                         ", 16, 0);	//1127
}

//lte, zigbee common
void sms_process(char *msg)     // 0829
{
	signed long	l;
	unsigned long ul;
	char sT0[50];
	
   	// 실제무게 FACTOR 값 :: #RFAC=12345@, $RFAC=12345@
	if( !strncmp( &msg[1], "RFAC=", 5))	//stx 제외
	{
//		lcd70_show_string_mode(300, 50,"REAL_WEIGHT_FACTOR  !!!", 20, 0);
		
		msg[11] = 0;
		Real_Weight_Factor = EE_Write_val(ADR_Real_Weight_Factor, atol(&msg[6]), 4);
	
		sprintf(sT0, "FACTOR=%ld  ", (long)Real_Weight_Factor);
		GLCD_string (0, 4, sT0); Delay_ms(3000);
		
		
		
		
//		lcd70_show_string_mode(300, 80, (const unsigned char *)sT0, 20, 0);
		
		Send_to_server(SEND_SETVALUE, "set");
	}
   	// 실제무게 값 :: #RWEIGHT=12345@, $RWEIGHT=12345@
	else if( !strncmp( &msg[1], "RWEIGHT=", 8))	//stx 제외
	{
//		lcd70_show_string_mode(300, 50,"REAL_WEIGHT !!!", 20, 0);
		msg[14] = 0;
		gplCompensationWeight = calcCompensationWeight(atol(&msg[9]));
		gplCompensationWeight = EE_Write_val(ADR_gplCompensationWeight, gplCompensationWeight, 4);
	
		sprintf(sT0, "REAL_WEIGHT=%ld, %ld. ", (long)gplCompensationWeight, atol(&msg[9]));
//		lcd70_show_string_mode(300, 80, (const unsigned char *)sT0, 20, 0);
	
		Delay_ms(2000);
		
		Send_to_server(SEND_SETVALUE, "set");
		USART1_puts(sT0);
	}
   	// 영점설정 :: #RZERO=12345@
	else if( !strncmp( &msg[1], "RZERO=", 6))
	{
//		sms_process(RxBuffer2_sms);
		zero_func();
	}
   	// 상태정보 전송간격 설정(분) :: #RSTTIME=12345@
	else if( !strncmp( &msg[1], "RSTTIME=", 8))
	{
		l = atol(&msg[9]);
		if(( MIN_gxwStatusSendInterval <= gxwStatusSendInterval) && (gxwStatusSendInterval <= MAX_gxwStatusSendInterval))
		{
			gxwStatusSendInterval = EE_Write_val(ADR_gxwStatusSendInterval, gxwStatusSendInterval, 4);
		}
	}
	// 로드셀 죽었을 때 보상 ON/OFF/
	else if( !strncmp( &msg[1], "RLCCOMP=", 8))
	{
		l = atol(&msg[9]);
		if(( MIN_gnlLcCompensation <= l) && (l <= MAX_gnlLcCompensation))
		{
			gnlLcCompensation = l;
			gnlLcCompensation = EE_Write_val(ADR_gnlLcCompensation, gnlLcCompensation, 4);
		}
	}

	//날짜, 시간 동기화
	else if( !strncmp(&msg[1], "DATE=", 5) && msg[20] == '@')
	{
		RTC_date_to_sec( &msg[6]);
	}

USART1_puts(msg);	
}	

#ifdef __gpi8310

void zero_func(void)
{
	char sT0[256];
	
	sprintf(sT0, "v_zero1=%6lu  comp=%6lu",(long)v_zero,(long)gplCompensationWeight);
//	lcd70_show_string_mode(200, 30, (const unsigned char *)sT0, 20, 0);

	/*
	while(!Weight_flag)
	{
		raw_data=Get_adc_from_module_20170626(); 	// 
	};
*/
	
	get_weight_wait();
	get_weight_wait();
	get_weight_wait();
	if( get_weight_wait()) {
	/*		
		Weight_flag = 0;
		rotation = 1;
			
		raw_data=Get_adc_from_module_20170626(); 	// 
		raw_data=Get_adc_from_module_20170626(); 	// 
		raw_data=Get_adc_from_module_20170626(); 	// 
		raw_data=Get_adc_from_module_20170626(); 	// 
		raw_data=Get_adc_from_module_20170626(); 	// 
		raw_data=Get_adc_from_module_20170626(); 	// 
		raw_data=Get_adc_from_module_20170626(); 	// 
		raw_data=Get_adc_from_module_20170626(); 	// 
		raw_data=Get_adc_from_module_20170626(); 	// 
		raw_data=Get_adc_from_module_20170626(); 	// 
		raw_data=Get_adc_from_module_20170626(); 	// 
		raw_data=Get_adc_from_module_20170626(); 	// 
		raw_data=Get_adc_from_module_20170626(); 	// 
		raw_data=Get_adc_from_module_20170626(); 	// 
		raw_data=Get_adc_from_module_20170626(); 	// 
		raw_data=Get_adc_from_module_20170626(); 	// 
		raw_data=Get_adc_from_module_20170626(); 	// 
		raw_data=Get_adc_from_module_20170626(); 	// 
		raw_data=Get_adc_from_module_20170626(); 	// 
		raw_data=Get_adc_from_module_20170626(); 	// 

		sprintf(sT0, "v_zero2=%6lu  comp=%6lu",(long)v_zero,(long)gplCompensationWeight);
		lcd70_show_string_mode(200, 30, (const unsigned char *)sT0, 20, 0);

		v_zero=raw_data;

		v_adc_org  = (long)((float)raw_data * gnfFactor);  // calibration시 영점값 기억 

		gplCompensationWeight=0;
	*/
		sprintf(sT0, "v_zero2=%6lu  comp=%6lu",(long)v_zero,(long)gplCompensationWeight);
//		lcd70_show_string_mode(200, 30, (const unsigned char *)sT0, 20, 0);

		v_zero = gxlRawAdc;
		v_adc_org  = gxlFactoredAdc;	// factor 곱한 영점값 기억 
		gplCompensationWeight = 0;

		v_zero = EE_Write_val(ADR_v_zero, v_zero, 4);
		v_adc_org = EE_Write_val(ADR_v_adc_org, v_adc_org, 4);
		gplCompensationWeight = EE_Write_val(ADR_gplCompensationWeight, gplCompensationWeight, 4);

	/*
		v_zero = EE_Write_val(ADR_v_zero, v_zero, 4);
		v_zero = EE_Read_val(ADR_v_zero, 4);
		Delay_ms(300);
		//					gplCompensationWeight = EE_Write_val(ADR_gplCompensationWeight, gplCompensationWeight, 4);
		gplCompensationWeight = EE_Write_val(ADR_gplCompensationWeight, 0, 4);

		gplCompensationWeight = EE_Read_val(ADR_gplCompensationWeight, 4);
	**/	
		
		sprintf(sT0, "v_zero3=%6lu  comp=%6lu",(long)v_zero,(long)gplCompensationWeight);
//		lcd70_show_string_mode(200, 30, (const unsigned char *)sT0, 20, 0);
	}
}

#endif

void zero_func(void) // 0829
{
	char sT0[100];
	u8 i;
	long temp_buff;
	u8 exit_flag=0;
	long sum=0;
	
	sprintf(sT0, "v_zero1=%6lu  comp=%6lu",(long)v_zero,(long)gplCompensationWeight);
	
	//091801
	
	lcd_clear();
	if(gnlLanguage==1)// korean
	{	
	if(gnlSilo1Selection==2)// silo 2개 사용시	
	GLCD_string (0, 2, " 싸이로 1번");	
	GLCD_string (0, 4, " 영점 실행 ?");
	
	}
	else
		{
		if(gnlSilo1Selection==2)// silo 2개 사용시		
		GLCD_string (0, 2, " SILO 1");	
		GLCD_string (0, 4, " MAKE ZERO ?");
		}
	
	Delay_ms(2000);
	
	while(!exit_flag)
	{
		if(keypush_8510())
	{
		//ch=(keypush_8510());

		switch(KeyCode)
		{
			case  ENTER:
		   exit_flag=1;
		  break;
			
			case  UP: case  DOWN: case  LEFT: case  RIGHT:
			   retry_message();
         goto ZERO_FINISH;			
			
		  break;
			
			
      default:
       break;				
			
		}
		
		
	} // of keypuxh 
	
};	
	
	lcd_clear();
	
	/*
	get_weight_wait();
	get_weight_wait();
	get_weight_wait();
	*/
	 for(i=0;i<5;i++)
 {
   //temp_buff = read_filtered_adc();
 if(which_adc==1) 
			{
				temp_buff =read_filtered_adc_CS5555_1();
				//temp_buff_1 =read_filtered_adc_CS5555_2();
			}
		 else
		 { 
		  temp_buff = read_filtered_adc();
      //temp_buff_1 = read_filtered_adc_1();
	   }
 
 }
    v_adc_org=(long)(gnfFactor * temp_buff);
		sprintf(sT0, "v_zero2=%6lu  comp=%6lu",(long)v_zero,(long)gplCompensationWeight);

		v_zero = temp_buff;
		gplCompensationWeight = 0;
		v_zero = EE_Write_val(ADR_v_zero, v_zero, 4);                   Delay_ms(10);
		v_adc_org = EE_Write_val(ADR_v_adc_org, v_adc_org, 4);   Delay_ms(10);
		gplCompensationWeight = EE_Write_val(ADR_gplCompensationWeight, gplCompensationWeight, 4); Delay_ms(10);
		sprintf(sT0, "v_zero3=%6lu  comp=%6lu",(long)v_zero,(long)gplCompensationWeight);
	 GLCD_string (0, 4, " ZERO FINISHED..");
   sign=0;
   sign_2=0;

 ZERO_FINISH:
 
 //0919
lcd_clear(); 
 
  /**************  Initial display *****************************/
 //20210408
 if(gnlSilo1Selection==1)// silo 1 enable   20180627
 {
   Silo_1_Weight_Display(); // silo 1  current weight, feed comsumption weight displaying  0627
	 lcd_inverse();
	// sprintf(str, "오 늘 :%5ld kg",gnlDailyTotal);
	 if(gnlLanguage==1)  
	  sprintf(str, "오 늘: %5ld kg ", gnlDailyTotal); 
	 else
		sprintf(str, "TODAY: %5ld kg ", gnlDailyTotal); 
	  
	// sprintf(str, "DAY-0: %5ld kg ", gnlDailyTotal);
	
	 GLCD_string (0,6 ,str);
	 lcd_normal();

 }
 if(gnlSilo1Selection==2)//1205
 {
	 if(gnlLanguage==1)
	 { 
	  lcd_inverse(); 
    sprintf(str, "무게:%5ld-%5ld", gxlWeight,gxlWeight_2);
	  GLCD_string (0,0 ,str);
	  lcd_normal();
		/* 
	  sprintf(str, "급이량1:%5ld kg", gnlDailyTotal);
	  GLCD_string (0,2 ,str);
	  sprintf(str, "급이량2:%5ld kg", gnlDailyTotal2);
	  GLCD_string (0,4 ,str);
	  */
		sprintf(str, "급이량1:%5ld kg", gnlFeeding1);
	  GLCD_string (0,2 ,str);
	  sprintf(str, "급이량2:%5ld kg", gnlFeeding2);
	  GLCD_string (0,4 ,str);
	   
		lcd_inverse();
	  sum=gnlDailyTotal+gnlDailyTotal2;
	  sprintf(str, "오 늘 :%5ld kg",sum);
	  GLCD_string (0,6 ,str);
	  lcd_normal(); 
	 }
	 else
		 { 
	  lcd_inverse(); 
    sprintf(str, "SILO:%5ld-%5ld", gxlWeight,gxlWeight_2);
	  GLCD_string (0,0 ,str);
	  lcd_normal();
	  sprintf(str, "FEED1:%5ld kg", gnlDailyTotal);
	  GLCD_string (0,2 ,str);
	  sprintf(str, "FEED2:%5ld kg", gnlDailyTotal2);
	  GLCD_string (0,4 ,str);
	  lcd_inverse();
	  sum=gnlDailyTotal+gnlDailyTotal2;
	  sprintf(str, "TODAY :%5ld kg",sum);
	  GLCD_string (0,6 ,str);
	  lcd_normal(); 
	 }
	 
	}

}


void zero_func2(void) // 0829
{
	char sT0[100];
	u8 i;
	long temp_buff;
	u8 exit_flag=0;
	long sum=0;
	
	sprintf(sT0, "v_zero1=%6lu  comp=%6lu",(long)v_zero,(long)gplCompensationWeight);
	
	//091801
	
	lcd_clear();
	if(gnlLanguage==1)// korean
	{	
	GLCD_string (0, 2, " 싸이로 2");	
	GLCD_string (0, 4, " 영점 실행    ?");
	}
		else
		{
    GLCD_string (0, 2, "SILO 2");			
	  GLCD_string (0, 4, " MAKE ZERO    ?");
	  }
	
	Delay_ms(2000);
	
	while(!exit_flag)
	{
		if(keypush_8510())
	{
		switch(KeyCode)
		{
			case  ENTER:
		   exit_flag=1;
		  break;
			case  UP: case  DOWN: case  LEFT: case  RIGHT:
			   retry_message();
         goto ZERO_FINISH;			
		  break;
      default:
       break;				
		}
	} // of keypuxh 
};	
// of silo2	

/*
    raw_data = read_filtered_adc_1();		// ad 값으 읽어온다ㅣ
		v_adc1_buf = (s32)(gnfFactor_2 * raw_data);	// count 값 입력(무게에 대한)
			v_i_value  =  v_adc1_buf - v_adc_org_2;	//현재 카운트 값에서 zero 에서의 카운트갓을 뺀다. 
		
*/

	lcd_clear();
	 for(i=0;i<5;i++)
 {
   //temp_buff = read_filtered_adc_1();
	 if(which_adc==1) 
			{
				//temp_buff =read_filtered_adc_CS5555_1();
				temp_buff =read_filtered_adc_CS5555_2();
			}
		 else
		 { 
		  temp_buff = read_filtered_adc_1();
      //temp_buff_1 = read_filtered_adc_1();
	   }
 }
    v_adc_org_2=(long)(gnfFactor_2 * temp_buff);
		sprintf(sT0, "v_zero2=%6lu  comp=%6lu",(long)v_zero,(long)gplCompensationWeight);
		v_zero_2 = temp_buff;
		gplCompensationWeight2 = 0;
		v_zero_2 = EE_Write_val(ADR_v_zero_2, v_zero_2, 4);                   Delay_ms(10);
		v_adc_org_2 = EE_Write_val(ADR_v_adc_org_2, v_adc_org_2, 4);   Delay_ms(10);
		gplCompensationWeight2 = EE_Write_val(ADR_gplCompensationWeight2, gplCompensationWeight2, 4); Delay_ms(10);
		sprintf(sT0, "v_zero2=%6lu  comp=%6lu",(long)v_zero_2,(long)gplCompensationWeight2);
	 GLCD_string (0, 4, " ZERO FINISHED..");
   sign=0;
   sign_2=0;

 ZERO_FINISH:
 
 //0919
lcd_clear(); 
 
  /**************  Initial display *****************************/
 if(gnlSilo1Selection==1)// silo 1 enable  //0627
 {
   Silo_1_Weight_Display(); // silo 1  current weight, feed comsumption weight displaying  0627
	 lcd_inverse();
	  if(gnlLanguage==1)
	   sprintf(str, "오 늘: %5ld kg ", gnlDailyTotal);
    else		
		sprintf(str, "TODAY: %5ld kg ", gnlDailyTotal);	
	 GLCD_string (0,6 ,str);
	 lcd_normal();

 }
 if(gnlSilo1Selection==2)//1205
 {
	 if(gnlLanguage==1)
	 { 
	 lcd_inverse(); 
   sprintf(str, "무게:%5ld-%5ld", gxlWeight,gxlWeight_2);
	 GLCD_string (0,0 ,str);
	 lcd_normal();
	 sprintf(str, "급이량1:%5ld kg", gnlDailyTotal);
	 GLCD_string (0,2 ,str);
	 sprintf(str, "급이량2:%5ld kg", gnlDailyTotal2);
	 GLCD_string (0,4 ,str);
	 lcd_inverse();
	 sum=gnlDailyTotal+gnlDailyTotal2;
	 sprintf(str, "오 늘: %5ld kg",sum);
	 GLCD_string (0,6 ,str);
	 lcd_normal(); 
	 }
	else
	 { 
	 lcd_inverse(); 
   sprintf(str, "SILO:%5ld-%5ld", gxlWeight,gxlWeight_2);
	 GLCD_string (0,0 ,str);
	 lcd_normal();
	 sprintf(str, "FEED1:%5ld kg", gnlDailyTotal);
	 GLCD_string (0,2 ,str);
	 sprintf(str, "FEED2:%5ld kg", gnlDailyTotal2);
	 GLCD_string (0,4 ,str);
	 lcd_inverse();
	 sum=gnlDailyTotal+gnlDailyTotal2;
	 sprintf(str, "TODAY: %5ld kg",sum);
	 GLCD_string (0,6 ,str);
	 lcd_normal(); 
	 }	
 
 }

}





 void Data_Send_Sms(char *tel_number)
 {
	char sT0[100];

	 //Data_Send_Sms(gtech_tel);
   // sprintf(sT0, "07-7-무게:%5lu kg 오늘:%5lu kg 어제:%5lu kg ", 	 //천안이례농장 
    sprintf(sT0, "07-1-무게:%5lu kg 오늘:%5lu kg 어제:%5lu kg ",  //회사장비 

		                                    (long)gxlGrossWeight,
											(long)gnlDailyTotal,
											(long)gnlPreDailyTotal[1]
											);
    send_sms(tel_number ,sT0); Delay_ms(1000);  //지테크한테  문자보내기 
  }

void test_set(void)	  // 0409
{
	char ch;
	char exit_flag=0;
	int	menu_display = 1;
	
	#define  GAP   30
	#define  START 100
	Delay_ms(1000);

	do {
		if ( menu_display) {
			menu_display = 0;

		}
		
		if (keypush())
		{
			menu_display = 1;

			ch=key_disp[KEYCODE];

			switch(ch)
			{
				case  KEY_3:
					cal_mode();
					break;
				case  KEY_1:
					test_mode();
					break;
				case  KEY_CLEAR:
					exit_flag = 1;
					break;
				case  KEY_PRINT:
					Data_Clear(); // 배출량 기록 "0" 으로 초기화 
					break;
				case  KEY_7:
					set_mode();
					break;
				case KEY_SET:
					set_mode_simple();
					break;
				default:
					menu_display = 0;
					retry_message();
					break;
			} // of switch
		} // of keypush()
	} while( !exit_flag);
}


void Data_Clear(void)//0908
{
	long password;
	int i;
	u8 exit_flag=0;
	union { u32 dw; u16 u[2]; u8 c[4];} temp;
		 
	lcd_clear(); // data clear
  /*	
	sprintf(str, "TODAY=%6lu ",(long)gnlDailyTotal);
  
	
	GLCD_string (0, 2, str); Delay_ms(500);
	for(i=0;i<7;i++)
	{
		sprintf(str, "%d  gnlPre=%6lu ",(int)i,(long)gnlPreDailyTotal[i]);
	}
	*/
 
 if(gnlLanguage==1) // korean
 { 
	lcd_inverse();
	GLCD_string (0, 0,"데이타 지우기"); //Delay_ms(5000);
  lcd_normal();
	sprintf(str, "현재일령=%5lu",(long)gnuDay);
  GLCD_string (0, 2, str); //Delay_ms(5000);
	lcd_inverse();
	GLCD_string (0, 6, "확인키:지우기");
	lcd_normal();
	// GLCD_string(0, 6, "ALL DAT");
 }
 else
  { 
	lcd_inverse();
	GLCD_string (0, 0,"DATA CLEAR"); //Delay_ms(5000);
  lcd_normal();
	sprintf(str, "DAYCOUNT=%5lu",(long)gnuDay);
  GLCD_string (0, 2, str); //Delay_ms(5000);
	GLCD_string (0, 4, "PRESS ENTER");
	 GLCD_string(0, 6, "ALL DATA !!");
 }
 
 
	while(!exit_flag)
	{
		 	if (keypush_8510()) // 0830
		{
			if(KeyCode==ENTER)
				exit_flag=1;
			}
	};

	
	lcd_clear();
 // GLCD_string (0, 0, "DATA CLEAR!");

	password=4070;
	if(password==4070)
	{
		gnuDay = 0;
		gnlDailyTotal = 0;
		gnlPreDailyTotal[0] = 0;
		gnlPreDailyTotal[1] = 0;
		gnlPreDailyTotal[2] = 0;
		gnlPreDailyTotal[3] = 0;
		gnlPreDailyTotal[4] = 0;
		gnlPreDailyTotal[5] = 0;
		gnlPreDailyTotal[6] = 0;

    gnuDay2 = 0;
		gnlDailyTotal2 = 0;
		gnlPreDailyTotal2[0] = 0;
		gnlPreDailyTotal2[1] = 0;
		gnlPreDailyTotal2[2] = 0;
		gnlPreDailyTotal2[3] = 0;
		gnlPreDailyTotal2[4] = 0;
		gnlPreDailyTotal2[5] = 0;
		gnlPreDailyTotal2[6] = 0;

		save_DailyTotals();

		Delay_ms(200);	


		FSMC_ENABLE;
		if(gnlLanguage==1) // korean
 { 
	lcd_inverse();
	GLCD_string (0, 0,"데이타 지우기"); //Delay_ms(5000);
  lcd_normal();
	sprintf(str, "현재일령=%5lu",(long)gnuDay);
  GLCD_string (0, 2, str); //Delay_ms(5000);
	lcd_inverse();
	GLCD_string (0, 6, "지우기 완료");
	lcd_normal();
	// GLCD_string(0, 6, "ALL DAT");
 }
 else
  { 
	lcd_inverse();
	GLCD_string (0, 0,"DATA CLEAR"); //Delay_ms(5000);
  lcd_normal();
	sprintf(str, "DAYCOUNT=%5lu",(long)gnuDay);
  GLCD_string (0, 2, str); //Delay_ms(5000);
	//GLCD_string (0, 4, "");
	 GLCD_string(0, 6, "END ..");
 }

    /*
		sprintf(str, "TODAY=%6lu ",(long)gnlDailyTotal);
		 GLCD_string (0, 2, str); //Delay_ms(5000);
		for(i=0;i<7;i++)
		{
			sprintf(str, "%d  gnlPre=%6lu ",(int)i,(long)gnlPreDailyTotal[i]);
		}

		sprintf(str, "day=%6lu ",(long)gnuDay);
		 GLCD_string (0, 4, str);  Delay_ms(5000);
	*/
	
	
	}
}


void Adc_Power_Off_On(void)
{
	if( !Adc_Counter)  // 옵션보드로 부터 일정시간 응답이 없으면 옵션부 전원을 강제로  ON/OFF 한다 
	{
		Adc_Power_Off();
		Delay_ms(1000);
		Adc_Power_On();
		Adc_Counter = OPTION_POWER_COUNTER;
	}
}


void logo(void)
{
	#define Y_AXIS_SPACE   50
	char str[20];

//POINT_COLOR = BLACK;
//BACK_COLOR = WHITE;

	//lcd70_show_string_mode_24x24(200, 100+(Y_AXIS_SPACE*1),"MODEL:GPI-8310", 16, 0);
	sprintf(str, "VERSION:%s", VERSION);
	//lcd70_show_string_mode_24x24(200, 100+(Y_AXIS_SPACE*2), str, 16, 0);
	//lcd70_show_string_mode_24x24(200, 100+(Y_AXIS_SPACE*3),"G-TECH INTERNATIONAL CO., ", 16, 0);
}

void mode_check(void)//0727
{
	char ch;
	
//	if (keypush())
	if(keypush_8510())
	{
		//ch=(keypush_8510());

		switch(KeyCode)
		{
			case  UP:
				test_mode();
				break;
			
			case  DOWN:
				//test_mode();
   			 
    	if(gnlSilo1Selection==2)// if 2 hopper selected
			{				
			 cal_display();
			 if(cal_selection==1)	
			   cal_mode();
			  else
					cal_mode_2();
		  }		
			else if(gnlSilo1Selection==1)// one hopper
			{
				cal_selection=1; 
				cal_mode();
			}	
				break;
		
     case  ENTER:
			 if(gnlSilo1Selection==1)//silo가 한개이면 
				   set_mode();
			   else //silo 가 2개이면 
				  set_mode2();
		 
		     break;
		 
		case  RIGHT: // 배출량 기록 "0" 으로 초기화 
				Data_Clear();
				break;
		case  LEFT:
       Show_ToalOutput();
		     Initial_Display();			

			break;
		
   case  DOUBLE:
		  	 set_ip();
			   set_port();
		
	
    break;




		
			/*
			case  RIGHT: // 배출량 기록 "0" 으로 초기화 
				Data_Clear();
				break;
			case KEY_SET: test_set();
     */			

			break;
		} // of switch 
	}
}
   

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#ifdef _____unused___________

void Get_Weight(void)
{
	
/////STST20160608
/**
DEBUG_puts("Get_Weight(void) -- TEST MODE");
Weight_flag = 1;
Real_Weight = 5300;
gxlGrossWeight = Real_Weight;
gxlWeight = gxlGrossWeight;

return;	
**/	

	
	//raw_data=Get_adc_from_module();  // 모듈로부터 a/d 값을 읽어온다.
	
	//if(Weight_flag)// 모듈로 부터 무게를 정상적으로 받았으면  현재무게를 표시한다. 
	while( !Weight_flag)
	{
		raw_data = Get_adc_from_module_20170626();  // 모듈로부터 a/d 값을 읽어온다.
		v_adc1_buf = (long)(gnfFactor * raw_data);
		
		//0116		   v_adc1_buf=1000;	 /////
		//0116		   v_adc_org=3000;	 ///// 0115
		
		diff1 = v_adc1_buf - v_adc_org;
		if(diff1<0)
		{
			diff1 = diff1*-1;
			sign_flag = 1;
		}
		else sign_flag=0;
		
		diff2 = v_adc1_buf - prev_adc1;	// for stable 
		// gxlWeight = ((diff1 + 10) / v_ei_multiply_factor) * v_division;
		gxlWeight = ((diff1) / v_ei_multiply_factor) * v_division;
		
		if(gnlDisplay)
		{
			sprintf(str, "diff1_1=%6lu ",(long)diff1);
	//		lcd70_show_string_mode(100, 300,str, 16, 0);
			
			sprintf(str, "v_adc_org_1=%6lu ",(long)v_adc_org);
	//		lcd70_show_string_mode(100, 330,str, 16, 0);

			sprintf(str, "gxlWeight_1=%6lu ",(long)gxlWeight);
	//		lcd70_show_string_mode(100, 360,str, 16, 0);
		}
		
		// 음수이면 
	//	if(gxlGrossWeight==0)
	//		sign_flag=0;
		if(sign_flag) gxlGrossWeight = gplCompensationWeight - gxlWeight;
		else 		  gxlGrossWeight = gplCompensationWeight + gxlWeight;
		
	//	gxlGrossWeight =  gxlWeight;
		
		
		if(Real_Weight_Factor != 10000)// 무게보정 factor가 가동되어을때 
		{
			Real_Weight = (float)(((Real_Weight_Factor)/10000.0) * gxlGrossWeight);
			gxlGrossWeight = Real_Weight;
			//lcd70_write_weight(100,220,Real_Weight);// total weight displayin
		}
		
		if(gxlGrossWeight < (v_capacity+2000)) // 현재무게가 v_capacity 보다 작을때 현재무게를 표시한다.
		{
			if(0 <= gxlGrossWeight) //2016.7.5
			{
	//			lcd70_write_weight(100,150,gxlGrossWeight);//  Big font total weight displayin
			}
		}
		
		
		/* 
		if(Real_Weight_Factor!=10000)// 무게보정 factor가 가동되어을때 
		{
			(float)Real_Weight=(float)(((Real_Weight_Factor)/10000)*gxlGrossWeight);
			lcd70_write_weight(100,220,Real_Weight);// total weight displayin
		}
		*/
		
		//  Weight_flag=0;
	} //while( !Weight_flag) //  무게를 읽어오면 
}
#endif	//#ifdef _____unused___________



void goto_iap(char *ip, u16 port)																 
{
	int ret_val;
	int retry = 0;
//	int iz;
	char szIP[20];
	
//	return;

//	NVIC_InitTypeDef NVIC_InitStructure;

	
	USART1_puts(" IN IAP ");

   	//2014.10.21
	////Send_to_Gtech_Event("FIRMWARE_UPDATE", "FAIL"); //발생한 event를 전송한다.

	
	ret_val = SocketOpen(TMS_SERVER_IP, TMS_SERVER_PORT);
	if(ret_val != 1) return;
	
	
//2014.10.30	__disable_irq();

	TIM_Cmd(TIM1, DISABLE);
	TIM_Cmd(TIM2, DISABLE);
	TIM_Cmd(TIM3, DISABLE);

	//Disable USART1 Receive and Transmit interrupts 
  	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

	//Disable USART2 Receive and Transmit interrupts 
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);

	//Disable USART3 Receive and Transmit interrupts 
	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
	//USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);

	//Disable the USART1 
	USART_Cmd(USART1, DISABLE);
	//Disable the USART2 
	USART_Cmd(USART2, DISABLE);
	//Disable the USART2 
	USART_Cmd(USART3, DISABLE);
	USART_Cmd(UART4, DISABLE);

	RTC_ITConfig(RTC_IT_SEC, DISABLE); // 2017.7.6 Disable the RTC Second

	//lcd70_clear_screen(WHITE);

//	lcd70_show_string_mode_24x24(0, 200,"장비를 업그레이드 합니다.", 16, 0);
//	lcd70_show_string_mode_24x24(0, 250,"잠시만 기다려 주십시요.", 16, 0);

///	lcd70_show_string_mode(250, 200,"장비를 업그레이드 합니다.", 16, 0);
///	lcd70_show_string_mode(250, 250,"잠시만 기다려 주십시요.", 16, 0);

//	lcd70_show_string_mode_24x24( 350, 150, "Upgrading.... ", 16, 0);	
//	lcd70_show_string_mode_24x24( 200, 250, "Wait a minute, and I'll be back soon.", 16, 0);	

//	Delay_ms(1000);

//	system_reset('T', 12345);

  	/* Set the Vector Table base location at 0x08000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);  


	//iap flag
	USART1->BRR = 12345;

	JumpAddress = *(vu32*) (ApplicationAddress + 4);

    /* Jump to user application */
    Jump_To_Application = (pFunction) JumpAddress;
    /* Initialize user application's Stack Pointer */
	__MSR_MSP(*(vu32*) ApplicationAddress); 
    Jump_To_Application();
}


int test_get_cdma_time()
{
	char sT0[30];

	get_cdma_time();
	sprintf(sT0, "%04d-%02d-%02d %02d:%02d:%02d", jangbi.cdma.tm.year, jangbi.cdma.tm.month, jangbi.cdma.tm.date, jangbi.cdma.tm.hour, jangbi.cdma.tm.min, jangbi.cdma.tm.sec);
//	lcd70_show_string_mode(500, 10, (const u8 *)sT0, strlen(sT0), 0);

	return 0;
}		


#ifdef  USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif


void ZIGBEE_puts(char *str)
{
	UART4_puts(str);
}


//2000 ~ 2099 년까지
#define	SECONDS_OF_HOUR	3600
int RTC_sec_to_time_conversion()
{	
	int ret_val = 0;
    u32 rtc_days;
    u32 THH, TMM, TSS;
	u32 TimeVar;
	// the beginning of each month
	u32 DaysToMonth[13] = { 0,31,59,90,120,151,181,212,243,273,304,334,365 };	
		  
	if(RTC_TimeUpdated)
	{
		ret_val = -1;
		RTC_TimeUpdated = 0;
		TimeVar = RTC_GetCounter();

/*		
RTC_BinaryToDate(RTC_GetCounter(),&system_datetime);

// 구조체의 내용으로 초를 계산하는 경우

Sec = RTC_DateToBinary(&system_datetime);		
*/
		
		rtc_days = TimeVar / (SECONDS_OF_HOUR*24);
		
		rtc_year = rtc_days / 365;
		rtc_month = (rtc_days - (rtc_year*365)) / 30;
		rtc_date = (rtc_days - (rtc_year*365 + rtc_year/4 + 1)) - DaysToMonth[rtc_month];
		
		
		TimeVar = TimeVar % (24*SECONDS_OF_HOUR);
		rtc_hour = TimeVar / SECONDS_OF_HOUR;  /* Compute  hours */
		rtc_min = (TimeVar % SECONDS_OF_HOUR) / 60;  /* Compute minutes */
		rtc_sec = TimeVar % 60;  /* Compute seconds */
		
/////////STST		
//		rtc_date = rtc_hour/2;
		
		
		
//		sprintf(str, "Date:%04lu-%02lu-%02lu, Time:%02d:%02d:%02d, %lu", rtc_year, rtc_month, rtc_date, rtc_hour, rtc_min, rtc_sec, rtc_days);
///		sprintf(str, "Date:20%02lu-%02lu-%02lu, Time:%02d:%02d:%02d, %lu", rtc_year, rtc_month, rtc_date, rtc_hour, rtc_min, rtc_sec, rtc_days);
///		USART1_puts(str);
//		sprintf(sT0, "tel=%s,rssi= %d,Date=%04d-%02d-%02d Time=%02d:%02d:%02d",  jangbi.cdma.telno, jangbi.cdma.rssi,
//		jangbi.cdma.tm.year, jangbi.cdma.tm.month, jangbi.cdma.tm.date, jangbi.cdma.tm.hour, jangbi.cdma.tm.min, jangbi.cdma.tm.sec);
//lcd70_show_string_mode(420, 30, (const unsigned char *)str, 20, 0);
///lcd70_show_string_mode(170, 460, (const unsigned char *)str, 20, 0);
	}

	return ret_val;
}

//2000 ~ 2099 년까지
int RTC_time_display()
{	
	sprintf(str, "Date:20%02lu-%02lu-%02lu, Time:%02d:%02d:%02d.", rtc_year, rtc_month, rtc_date, rtc_hour, rtc_min, rtc_sec);
//	lcd70_show_string_mode(170, 460, (const unsigned char *)str, 20, 0);
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

int get_weight()
{
	int ret_val = 0;
	long raw_adc;
	
// v1.9 2017.6.26	raw_adc = Get_adc_from_module_20160629();
	raw_adc = Get_adc_from_module_20170626();

	if(Weight_flag)// 모듈로 부터 무게를 정상적으로 받았으면  현재무게를 표시한다. 
	{
		ret_val = -1;
		Weight_flag = 0;
		
		v_adc1_buf = (long)(gnfFactor * raw_adc);
		diff1 = v_adc1_buf - v_adc_org;
//		diff2 = v_adc1_buf - prev_adc1;	// for stable 
		gxlWeight = ((diff1) / v_ei_multiply_factor) * v_division;
		
		if(gnlDisplay)
		{
			sprintf(str, "gxlWeight=%6ld ",(long)gxlWeight);
//			lcd70_show_string_mode(10, 360,str, 16, 0);
			sprintf(str, "diff1=%6ld ",(long)diff1);//
//			lcd70_show_string_mode(10, 300,str, 16, 0);
			sprintf(str, "v_adc_org=%6ld ",(long)v_adc_org);
//			lcd70_show_string_mode(10, 330,str, 16, 0);
		}

		gxlGrossWeight = gxlWeight + gplCompensationWeight;
		if(Real_Weight_Factor != 10000)// 무게보정 factor가 가동되어을때 
		{
			gxlGrossWeight = (float)(((Real_Weight_Factor)/10000.0) * gxlGrossWeight);
		}

		//한눈의 값으로 맞추기
		if(gxlGrossWeight < 0) gxlGrossWeight -= (long)(-gxlGrossWeight % v_division);
		else gxlGrossWeight -= ((long)gxlGrossWeight % v_division);
		
// 2016.7.25
		mon_raw_adc = raw_adc;
		
		mon_gxlGrossWeight = gxlGrossWeight;
		
		mon_One_Weight = One_Weight;
		mon_Two_Weight = Two_Weight;
		mon_Three_Weight = Three_Weight;
		mon_Four_Weight = Four_Weight;
		mon_Five_Weight = Five_Weight;
		mon_Six_Weight = Six_Weight;
		
		mon_One_Weight_Ok = One_Weight_Ok;
		mon_Two_Weight_Ok = Two_Weight_Ok;
		mon_Three_Weight_Ok = Three_Weight_Ok;
		mon_Four_Weight_Ok = Four_Weight_Ok;	
		mon_Five_Weight_Ok = Five_Weight_Ok;	
		mon_Six_Weight_Ok = Six_Weight_Ok;	


///STST			
//Send_to_server(SEND_STATUS, "status");
	}
	
	return ret_val;
}		


int get_weight_nowait()
{
	int	 ret_val = 0;
	int	 exit_flag = 0;
	long raw_adc, factored_adc;
	long diff, raw_weight, gross_weight;

//	rotation = 1;

//	do {
		raw_adc = Get_adc_from_module_20170626();

		if( Weight_flag)// 모듈로부터 무게를 정상적으로 받았으면 현재무게를 표시한다. 
		{
			Weight_flag = 0;
			ret_val = -1;

			factored_adc = (long)(gnfFactor * raw_adc);
			diff = factored_adc - v_adc_org;
//			diff2 = adc1_buf - prev_adc1;	// for stable 
			raw_weight = ((diff + (v_ei_multiply_factor/2)) / v_ei_multiply_factor) * v_division;

			// 보정무게 반영
			gross_weight = raw_weight + gplCompensationWeight;

			// 무게보정 factor가 가동되어을때 
			gross_weight = (float)(((Real_Weight_Factor)/10000.0) * gross_weight);

			//한눈의 값으로 맞추기
			if(gross_weight < 0) gross_weight -= (long)(-gross_weight % v_division);
			else gross_weight -= ((long)gross_weight % v_division);

			if(gnlDisplay)
			{
				sprintf(str, "diff1=%6ld ",(long)diff1);
//				lcd70_show_string_mode(10, 330,str, 16, 0);
				sprintf(str, "v_adc_org=%6ld ",(long)v_adc_org);
//				lcd70_show_string_mode(10, 350,str, 16, 0);
				sprintf(str, "gxlWeight=%6ld, gxlGrossWeight=%ld ",(long)raw_weight, gross_weight);
//				lcd70_show_string_mode(10, 370,str, 16, 0);
			}

			
			gxiAdcFlag = 1;	// ad conversion end flag
			gxlRawAdc = raw_adc;	// raw adc value
			gxlSpanAdc = gxlRawAdc - v_zero; // gxlRawAdc - v_zero
			gxlPreAdc = gxlFactoredAdc;	// previous gxlFactoredAdc  : prev_adc1
			gxlFactoredAdc = factored_adc;	// gxlRawAdc * gnfFactor
			gxlCountAdc = diff;	// internal weight counter
			gxlRawWeight = raw_weight; // raw_span weight - not compensation
			gxlWeight = raw_weight;	// net weight
			gxlGrossWeight = gross_weight; // gross weight			

			
			mon_raw_adc = raw_adc;
			mon_gxlGrossWeight = gxlGrossWeight;
			
			mon_One_Weight = One_Weight;
			mon_Two_Weight = Two_Weight;
			mon_Three_Weight = Three_Weight;
			mon_Four_Weight = Four_Weight;
			mon_Five_Weight = Five_Weight;
			mon_Six_Weight = Six_Weight;
			
			mon_One_Weight_Ok = One_Weight_Ok;
			mon_Two_Weight_Ok = Two_Weight_Ok;
			mon_Three_Weight_Ok = Three_Weight_Ok;
			mon_Four_Weight_Ok = Four_Weight_Ok;	
			mon_Five_Weight_Ok = Five_Weight_Ok;	
			mon_Six_Weight_Ok = Six_Weight_Ok;	

	///STST			
	//Send_to_server(SEND_STATUS, "status");
		}
			
//	} while( !exit_flag);
	
	return ret_val;
}		





/******************************************************************************/


int get_weight_wait()
{
	int ret_val = 0;

	rotation = 1;	// scan start

	while( !get_weight_nowait());
	
	ret_val = -1;

	return ret_val;
}



/************** function of gpi-8510 ***********************/



/* Silo_1_Led_Operation()/Silo_2_Led_Operation()/Error_1_Led_Operation()/
   Error_2_Led_Operation() moved to driver/led.c (Day 2 driver extraction). */
void Silo_1_Led_Operation(void);
void Silo_2_Led_Operation(void);
void Error_1_Led_Operation(void);
void Error_2_Led_Operation(void);


void  Silo_1_Weight_Display() //0908  // 0627
{
	
	//lcd_initial();
 
 if(gnlLanguage==1) // korean
 { 

	if(Loadcell_Error1)
   	 GLCD_string ("로드셀 에러");
	
  else//정상일때	
  {  
	 sprintf(str, "무 게: %5ld kg ", gxlWeight);
	 //  sprintf(str, ": %5ld kg ", gxlWeight);	
   	
		GLCD_string (0,0 ,str);
	} 
	lcd_inverse();	
  

	//sprintf(str, "급 이: %5ld kg ", gnlDailyTotal);
	// if(!working_flag)
   {		 
	   sprintf(str, "급 이: %5ld kg ", gnlFeeding1);
  	 GLCD_string (0,2 ,str);
	 } 
	//sprintf(str, "오 늘: %5ld kg ", gnlDailyTotal);
	//GLCD_string (0,6 ,str);

	 lcd_normal();	
 
	
	}//of if(gnlLanguage==1)
 
  else
 {
  if(Loadcell_Error1)
   	 GLCD_string ("LOAD_CELL ERROR!");
	 else
   {		 
   	sprintf(str, "SILO : %5ld kg ", gxlWeight);
  	GLCD_string (0,0 ,str);
   }	

	lcd_inverse();	
	//sprintf(str, "FEED : %5ld kg ", gnlDailyTotal);
	 sprintf(str, "FEED : %5ld kg ", gnlFeeding1);
	GLCD_string (0,2 ,str);
	 lcd_normal();	
 }
 
 
 

}	

void  Silo_2_Weight_Display()
{
	lcd_initial();
	sprintf(str, "SILO2: %5ld kg ", gxlWeight_2);
	GLCD_string (0,4 ,str);
	 lcd_inverse();	
	sprintf(str, "FEED2: %5ld kg ", gnlDailyTotal2);
	GLCD_string (0,6 ,str);
	 lcd_normal();	
}	
	


void cal_display(void)  //0907
{
	 u8 exit_flag=0;
//	 u8 select_flag;
	lcd_clear(); 
	GLCD_string (0,0 ,"CAL SELECT");
  lcd_inverse();
	GLCD_string (0,2 ,"SILO_1 CAL");
	lcd_normal();
  GLCD_string (0,4 ,"SILO_2 CAL");
  GLCD_string (0,6 ,"ENTER:NEXT");
	cal_selection=1; 

	do
	{
		
		if (keypush_8510()) // 0830
		{
			 //ch=key_disp[KEYCODE];   //0829
			
			switch(KeyCode)
			{
           case UP:	//1127
           cal_selection=1; 
					 GLCD_string (0,0 ,"CAL SELECT");
          lcd_inverse();
	       GLCD_string (0,2 ,"SILO_1 CAL");
	       lcd_normal();
         GLCD_string (0,4 ,"SILO_2 CAL");
	          break;
					 
					 case DOWN:
				cal_selection=2; 
			   GLCD_string (0,2 ,"SILO_1 CAL");
         	lcd_inverse();
	    		GLCD_string (0,4 ,"SILO_2 CAL");
	       lcd_normal();
         
						 break;
           case ENTER:
						  exit_flag=1;
						 break;
					 default: 
						 break;
        	
			}//of switch
		
	
	//cal_mode();
//	cal_mode_2();
		} //of if 	
	} while( !exit_flag);
	
}






void Silo_1_Getweight(void) //1211
{
/**************** 무게읽어서 표시 silo_1 ******************************/
		//raw_data = read_filtered_adc();		// ad 값으 읽어온다.
	 if(which_adc==1) 
			{
				raw_data =read_filtered_adc_CS5555_1();
				//temp_buff_1 =read_filtered_adc_CS5555_2();
			}
		 else
		 { 
		  raw_data = read_filtered_adc();
      //temp_buff_1 = read_filtered_adc_1();
	   }
	  if(raw_data>400000)
			Loadcell_Error1=1;
		else
			Loadcell_Error1=0;
	
		v_adc1_buf = (s32)(gnfFactor * raw_data);	// count 값 입력(무게에 대한)
			v_i_value  =  v_adc1_buf - v_adc_org;	//현재 카운트 값에서 zero 에서의 카운트갓을 뺀다. 
		
			if(Real_Weight_Factor != 10000)// 무게보정 factor가 가동되어을때 
		{
			Real_Weight = (float)(((Real_Weight_Factor)/10000.0) * v_i_value);
			v_i_value = Real_Weight;
			//lcd70_write_weight(100,220,Real_Weight);// total weight displayin
		}
		if (0 <= v_i_value)   //0828
		{
			gxlGrossWeight = (v_i_value + (v_ei_multiply_factor/2));  // 10000+(10/2)=10005
			gxlGrossWeight=(gxlGrossWeight / v_ei_multiply_factor);	//10005/10=1000
			gxlGrossWeight=gxlGrossWeight * v_division;					//1000*5=5000
  		sign=0;
		}
		else  
		{
 			sign=1;
			v_i_value=v_i_value*-1;
			gxlGrossWeight = (v_i_value + (v_ei_multiply_factor/2));
			gxlGrossWeight=(gxlGrossWeight / v_ei_multiply_factor);
			gxlGrossWeight=gxlGrossWeight * v_division;
		  if(gxlGrossWeight==0)
			sign=0;	
		
		}

	//	 if(gxlGrossWeight>=0  &&gxlGrossWeight<=v_capacity)
	
   if (!gplCompensationWeight) gxlWeight = gxlGrossWeight;      // 보상무게 처리 
    else 
		{
     if(!sign)			
		 gxlWeight = gxlGrossWeight + gplCompensationWeight;
		 else
		  gxlWeight = gplCompensationWeight-gxlGrossWeight;
		
   	 if(gxlWeight>=0)
		  sign=0;
		 else
		 {
			 gxlWeight=gxlWeight*-1; 
			 sign=1;
		
     }
		}	
			
		
	}
		
	void Silo_2_Getweight(void)
	{
	/**************** 무게읽어서 표시 silo_2 ******************************/
		//raw_data = read_filtered_adc_1();		// ad 값으 읽어온다ㅣ
    if(which_adc==1) 
			{
				//temp_buff =read_filtered_adc_CS5555_1();
				raw_data =read_filtered_adc_CS5555_2();
			}
		 else
		 { 
		  //temp_buff = read_filtered_adc();
      raw_data = read_filtered_adc_1();
	   }	



		if(raw_data>400000)
			Loadcell_Error2=1;
		else
			Loadcell_Error2=0;
	
		
		
		
		v_adc1_buf = (s32)(gnfFactor_2 * raw_data);	// count 값 입력(무게에 대한)
			v_i_value  =  v_adc1_buf - v_adc_org_2;	//현재 카운트 값에서 zero 에서의 카운트갓을 뺀다. 
		
			if(Real_Weight_Factor2 != 10000)// 무게보정 factor가 가동되어을때 
		{
			Real_Weight = (float)(((Real_Weight_Factor)/10000.0) * v_i_value);
			v_i_value = Real_Weight;
			//lcd70_write_weight(100,220,Real_Weight);// total weight displayin
		}
		if (0 <= v_i_value)   //0828
		{
			gxlGrossWeight_2 = (v_i_value + (v_ei_multiply_factor/2));  // 10000+(10/2)=10005
			gxlGrossWeight_2=(gxlGrossWeight_2 / v_ei_multiply_factor);	//10005/10=1000
			gxlGrossWeight_2=gxlGrossWeight_2 * v_division_2;					//1000*5=5000
  		sign_2=0;
		}
		else  
		{
 			sign_2=1;
			v_i_value=v_i_value*-1;
			gxlGrossWeight_2 = (v_i_value + (v_ei_multiply_factor/2));
			gxlGrossWeight_2=(gxlGrossWeight_2 / v_ei_multiply_factor);
			gxlGrossWeight_2=gxlGrossWeight_2 * v_division_2;
			if(gxlGrossWeight_2==0)
			sign_2=0;	
		}

	//	 if(gxlGrossWeight>=0  &&gxlGrossWeight<=v_capacity)
	
   if (!gplCompensationWeight2) gxlWeight_2 = gxlGrossWeight_2;      // 보상무게 처리 
   else
		{
     if(!sign_2)			
		 gxlWeight_2 = gxlGrossWeight_2 + gplCompensationWeight2;
		 else
		  gxlWeight_2 = gplCompensationWeight2-gxlGrossWeight_2;
		
		if(gxlWeight_2>=0)
		  sign_2=0;
		 else
		 { 
			 sign_2=1;
		   gxlWeight_2=gxlWeight_2*-1;
		 
		 }
		}	


		
		
	}
/*
		if(Real_Weight_Factor != 10000)// 무게보정 factor가 가동되어을때 
		{
			Real_Weight = (float)(((Real_Weight_Factor)/10000.0) * gxlWeight);
			gxlWeight = Real_Weight;
			//lcd70_write_weight(100,220,Real_Weight);// total weight displayin
		}
*/
	
/*
	
	#define	 RTC_CLOCK_HIGH   GPIO_SetBits(GPIOB, GPIO_Pin_5)
  #define	 RTC_CLOCK_LOW  	GPIO_ResetBits(GPIOB, GPIO_Pin_5)
  #define	 RTC_RESET_HIGH   GPIO_SetBits(GPIOB, GPIO_Pin_9)
  #define	 RTC_RESET_LOW  	GPIO_ResetBits(GPIOB, GPIO_Pin_9)
  #define	 RTC_DATA_HIGH   GPIO_SetBits(GPIOB, GPIO_Pin_8)
  #define	 RTC_DATA_LOW  	GPIO_ResetBits(GPIOB, GPIO_Pin_8)
	
	*/
	
#define  TIME_DLEAY 5
	
	void WriteByte(unsigned char Data)   
{   
  unsigned i; 
  Rtc_Write();// data portstatus : output  
  for(i = 0; i < 8; i++)   
  {   
  // SCLK_0;
   RTC_CLOCK_LOW; // clock high		
   if(Data & 0x01)   
   RTC_DATA_HIGH;   
   else  
   RTC_DATA_LOW;	

   Delay(TIME_DLEAY);
	 
	 
	 RTC_CLOCK_HIGH; Delay(TIME_DLEAY);
   Data <<= 1;   
  }
   RTC_CLOCK_LOW; Delay(TIME_DLEAY);		 
   
}   

unsigned char ReadByte()   
{   
  unsigned i, Data = 0;   
  Rtc_Read();	// data port status : input
  RTC_CLOCK_LOW;	
 
	for(i = 0; i < 8; i++)   
  {   
   RTC_CLOCK_HIGH;	Delay(TIME_DLEAY);	
  	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0x01)
           Data += 0x80;   
   else   
           Data += 0x00;   
  RTC_CLOCK_LOW;	Delay(TIME_DLEAY);
   Data >>= 1; Delay(TIME_DLEAY);  
  }   
  return Data;   
}   


unsigned char Get_Second()   
{   
 unsigned Second;   
 RTC_RESET_LOW; Delay(TIME_DLEAY);  
 RTC_RESET_HIGH;  Delay(TIME_DLEAY) ;
 WriteByte(GetSecond);   
 Second = ReadByte();   
 RTC_RESET_LOW;  Delay(TIME_DLEAY);
 return Second;   
}   

/*
#define PCPORT         GPIOB 
#define DS7_SCLK   GPIO_PIN_5 //DS1302?? 
#define DS6_IO    GPIO_PIN_8 // DS1302 ?据 
#define DS5_RST   GPIO_PIN_9 //DS1302片?/?位 
*/

#ifdef dpfpd

void Clock_Pin_Init(void) 
{ 
   // GPIO_Init_TypeDef GPIO_InitStructure; 
	  EXTI_InitTypeDef EXTI_InitStructure;
  	GPIO_InitTypeDef GPIO_InitStructure;

	
	
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //_LOW_SLOW; 
    GPIO_InitStructure.GPIO_Pin = DS7_SCLK; 
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PBPORT, &GPIO_InitStructure); 
    
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//_LOW_FAST;//GPIO_MODE_OUT_OD_HIZ_FAST ; 
    GPIO_InitStructure.GPIO_Pin = DS6_IO; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(PBPORT, &GPIO_InitStructure); 
     
    
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//_LOW_SLOW; 
    GPIO_InitStructure.GPIO_Pin = DS5_RST; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(PBPORT, &GPIO_InitStructure); 
	
	


}


#define DATA_DELAY 10



void W8bit_HT1381(u8 wrdata) 
{ 
    u8 i; 
	Rtc_Write(); // 1201
  for(i=0; i<8; i++) 
  { 
        if(wrdata&0x01){GPIO_SetBits(PBPORT,DS6_IO );} 
        else{GPIO_ResetBits(PBPORT,DS6_IO );} 
        Delay(DATA_DELAY); 
        GPIO_SetBits(PBPORT,DS7_SCLK);         
      Delay(DATA_DELAY); 
        GPIO_ResetBits(PBPORT,DS7_SCLK); 
        Delay(DATA_DELAY); 
        wrdata >>= 1; 
  } 
} 
u8 R8bit_HT1381(void) 
{ 
    u8 i,rddata=0; 
    GPIO_SetBits(PBPORT,DS6_IO );
   Rtc_Read();	
  for(i=0; i<8; i++) 
  { 
        rddata >>=1; 
        Delay(DATA_DELAY); 
       // if ((GPIO_ReadInputData(PBPORT) & DS6_IO) != (u8)0x00) 
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) != 0x00)
        { rddata |=0x80; } 
        GPIO_SetBits(PBPORT,DS7_SCLK);         
      Delay(DATA_DELAY); 
        GPIO_ResetBits(PBPORT,DS7_SCLK); 
        Delay(DATA_DELAY); 
         
  } 
  return(rddata); 
}

	void Write1381(u8 Addr, u8 Wdata) 
{ 
    GPIO_ResetBits(PBPORT,DS5_RST); 
    Delay(DATA_DELAY); 
    GPIO_ResetBits(PBPORT,DS7_SCLK); 
    Delay(DATA_DELAY); 
    
	  GPIO_SetBits(PBPORT,DS5_RST); 
    W8bit_HT1381(Addr); 
    W8bit_HT1381(Wdata); 
    GPIO_SetBits(PBPORT,DS7_SCLK); 
    Delay(DATA_DELAY); 
    GPIO_ResetBits(PBPORT,DS7_SCLK); 
    Delay(DATA_DELAY); 
    GPIO_ResetBits(PBPORT,DS5_RST); 
} 
u8 Read1381(u8 Addr) 
{ 
    u8 rddata; 
    GPIO_ResetBits(PBPORT,DS5_RST); 
    Delay(DATA_DELAY); 
    GPIO_ResetBits(PBPORT,DS7_SCLK); 
    Delay(DATA_DELAY); 
   
	 GPIO_SetBits(PBPORT,DS5_RST); 
    Delay(DATA_DELAY); 
    W8bit_HT1381(Addr); 
    rddata = R8bit_HT1381(); 
    Delay(DATA_DELAY); 
    GPIO_SetBits(PBPORT,DS7_SCLK);         
    Delay(DATA_DELAY); 
    GPIO_ResetBits(PBPORT,DS7_SCLK); 
    Delay(DATA_DELAY); 
    GPIO_ResetBits(PBPORT,DS5_RST); 
  return(rddata); 
} 

void Set1381(u8 *pSecDa) 
{ 
    u8 i; 
    u8 Addr = 0x80; 
    Write1381(0x8e,0x00); /* 控制命令,WP=0,?操作?*/ 
    for(i =7;i>0;i--) 
    { 
        Write1381(Addr,*pSecDa); /* 秒 分 ? 日 月 星期 年 */ 

        pSecDa++; 
        Addr +=2; 
    } 
    Write1381(0x8e,0x80); /* 控制命令,WP=1,?保??*/ 
} 
/******************************************************************** 
* 
* 名?: v_Get1381 
* ?明: 
* 功能: ?取HT1381?前?? 
* ?用: uc_R1381() 
* ?入: ucCurtime: 保存?前??地址。?前??格式?: 秒 分 ? 日 月 星期 年 
* 7Byte (BCD?) 1B 1B 1B 1B 1B 1B 1B 
* 返回?: 无 
***********************************************************************/ 
void Get1381(void) 
{ 
    u8 i; 
    u8 Addr = 0x81; 
    for (i=0;i<7;i++) 
    { 
        Gettimebuf[i] = Read1381(Addr);/*格式?: 秒 分 ? 日 月 
        星期 年 */ 
        Addr += 2; 
    } 
} 

	#endif
	
	


void Date_Time_Display(void)
{
  
	Get1381(); 
	sprintf(str, "date:20%02x-%02X-%02X  time:%02x-%02X-%02X ", Gettimebuf[6],Gettimebuf[4],Gettimebuf[3],Gettimebuf[2],Gettimebuf[1],Gettimebuf[0]);	USART1_puts(str);
  if(gnlLanguage==1)
	{	
	 GLCD_string (0,0 ,"현재날짜");
   sprintf(str, "20%02x년%02X월%02X일", Gettimebuf[6],Gettimebuf[4],Gettimebuf[3]);	
	 lcd_inverse();
 	 GLCD_string (0,2 ,str);
   lcd_normal();	 
   GLCD_string (0,4 ,"현재시간");
   lcd_inverse();
	 sprintf(str, "%02x시%02X분%02X초", Gettimebuf[2],Gettimebuf[1],Gettimebuf[0]);	
   GLCD_string (0,6 ,str);
	 lcd_normal();
	}
else
		
	{	
	GLCD_string (0,0 ,"    DATE        ");
  sprintf(str, "  20%02x-%02X-%02X    ", Gettimebuf[6],Gettimebuf[4],Gettimebuf[3]);	
	lcd_inverse();
	GLCD_string (0,2 ,str);
  lcd_normal();	 
  GLCD_string (0,4 ,"    TIME        ");
  lcd_inverse();
	sprintf(str, "  %02x-%02X-%02X      ", Gettimebuf[2],Gettimebuf[1],Gettimebuf[0]);	
  GLCD_string (0,6 ,str);
	 lcd_normal();
	}
		Delay_ms(1000);
}



void Show_ToalOutput(void)//누적배출량
{
	
	
	long password;
	int i;
	u8 exit_flag=0;
	union { u32 dw; u16 u[2]; u8 c[4];} temp;
	u8 clear_counter=0;
		 
	lcd_clear(); // data clear
  
 if(gnlLanguage==1) // korean
 { 
	lcd_inverse();
	GLCD_string (0, 0," 총누적 배출량"); //Delay_ms(5000);
  lcd_normal();
	sprintf(str, "총=%9lu kg",(long)gxhTotalOutput); // 총누적배출량 표시
  GLCD_string (0, 2, str); //Delay_ms(5000);
	
	 
	 lcd_inverse();
	GLCD_string (0, 4, "지우기:아래키3번");
	lcd_normal();
	 GLCD_string (0, 6, "나가기:확인키");
	// GLCD_string(0, 6, "ALL DAT");
 }
 //1202
 
	while(!exit_flag)
	{
		 	if (keypush_8510()) // 0830
		{
		       switch(KeyCode)
			 {
						 case UP:case LEFT: case RIGHT:
							  clear_counter=0;
						     break;
						 case DOWN:
               clear_counter++;
						   if(clear_counter==3)
								  exit_flag=1;
               break;						 
	           case ENTER:
            		  exit_flag=1;
               break;						 
	 		 } // of switch
		 }// of keypush 
	};

  if(clear_counter==3) //1202
	{
		lcd_inverse();
	GLCD_string (0, 0," 총누적 지우기"); //Delay_ms(5000);
  lcd_normal();
		
	gxhTotalOutput=0; 	
		gxhTotalOutput = EE_Write_val(ADR_gxhTotalOutput, gxhTotalOutput, 4); Delay_ms(100);
		
	sprintf(str, "총=%9lu kg",(long)gxhTotalOutput); // 총누적배출량 표시
  GLCD_string (0, 2, str); Delay_ms(2000);
		
	}	



	
	lcd_clear();

}

void Initial_Display(void)
{
	long  sum;
 /**************  Initial display *****************************/
 if(gnlSilo1Selection==1)// silo  NUMBER ==1
 {
	 SendtoPc();
   Silo_1_Weight_Display(); // silo 1  current weight, feed comsumption weight displaying 
	 SendtoPc();
	 lcd_inverse();
	 if(gnlLanguage==1) // korean 
	 sprintf(str, "오 늘: %5ld kg ", gnlDailyTotal); 
	 else
	 sprintf(str, "TODAY: %5ld kg ", gnlDailyTotal);
	 
	 GLCD_string (0,6 ,str);
	 lcd_normal();

 }
else // silo number is 2  20171019
 {
	if(gnlLanguage==1)
  {		
	 lcd_inverse(); 
   sprintf(str, "무게:%5ld-%5ld", gxlWeight,gxlWeight_2);
	 GLCD_string (0,0 ,str);
	 lcd_normal();
    /*	
		sprintf(str, "급이량1:%5ld kg", gnlDailyTotal);
	 GLCD_string (0,2 ,str);
	 sprintf(str, "급이량2:%5ld kg", gnlDailyTotal2);
	 GLCD_string (0,4 ,str);
		*/
   //ver 2.8
		sprintf(str, "급이량1:%5ld kg", gnlFeeding1);
	 GLCD_string (0,2 ,str);
	 sprintf(str, "급이량2:%5ld kg", gnlFeeding2);
	 GLCD_string (0,4 ,str);



		lcd_inverse();
	 sum=gnlDailyTotal+gnlDailyTotal2;
	 sprintf(str, "오 늘: %5ld kg",sum);
 //sprintf(str, "오 늘: %5ld kg ", gnlDailyTotal); 
	 GLCD_string (0,6 ,str);
	 lcd_normal();
	}
	else
   {		
	 lcd_inverse(); 
   sprintf(str, "SILO:%5ld-%5ld", gxlWeight,gxlWeight_2);
	 GLCD_string (0,0 ,str);
	 lcd_normal();
	 sprintf(str, "FEED1:%5ld kg", gnlDailyTotal);
	 GLCD_string (0,2 ,str);
	 sprintf(str, "FEED2:%5ld kg", gnlDailyTotal2);
	 GLCD_string (0,4 ,str);
	 lcd_inverse();
	 sum=gnlDailyTotal+gnlDailyTotal2;
	 sprintf(str, "TODAY: %5ld kg",sum);
 //sprintf(str, "오 늘: %5ld kg ", gnlDailyTotal); 
	 GLCD_string (0,6 ,str);
	 lcd_normal();
	}
 }
}
 /**************  Initial display *****************************/

void set_ip()
{
	int i;
	char loc=0;
	int  count;
	int  blink;
	unsigned char	digit_max[6] = { 9, 9, 1, 9, 3, 9 };
	unsigned char	exit_flag =0;
	unsigned char	curr_num = 0;
	char			tbuf[6];
	char  serverip[15];
	char			year, month, date;
	char  str[50];
	lcd_normal();
	lcd_clear();
	lcd_normal();
	if(gnlLanguage==1)//국문
	GLCD_string (0,0 ," IP 설정 ");
	else
	GLCD_string (0,0 ," IP SETTING");
	Delay_ms(1500);
	
	
	/*
	
	tbuf[0] = Gettimebuf[6] / 16;
	tbuf[1] = Gettimebuf[6] % 16;
	tbuf[2] = Gettimebuf[4] / 16;
	tbuf[3] = Gettimebuf[4] % 16;
	tbuf[4] = Gettimebuf[3] / 16;
	tbuf[5] = Gettimebuf[3] % 16;

  sprintf(str, "20%1x%1x-%1x%1x-%1x%1x",tbuf[0],tbuf[1],tbuf[2],tbuf[3],tbuf[4],tbuf[5] );	
	GLCD_string (0,4 ,str);
 */
	
	serverip[0]=IpAdresss_1/100;
	serverip[1]=(IpAdresss_1%100)/10;
	serverip[2]=(IpAdresss_1%100)%10;
	
	serverip[3]=IpAdresss_2/100;
	serverip[4]=(IpAdresss_2%100)/10;
	serverip[5]=(IpAdresss_2%100)%10;
	
	serverip[6]=IpAdresss_3/100;
	serverip[7]=(IpAdresss_3%100)/10;
	serverip[8]=(IpAdresss_3%100)%10;
	
	serverip[9]=IpAdresss_4/100;
	serverip[10]=(IpAdresss_4%100)/10;
	serverip[11]=(IpAdresss_4%100)%10;
	
  //sprintf(str, "%c%c%c.%c%c%c.%c%c%c.%c%c%c",serverip[0],serverip[1],serverip[2],serverip[3],serverip[4],serverip[5],serverip[6],serverip[7],serverip[8],serverip[9],serverip[10],serverip[11] );	
	sprintf(str, "%d%d%d.%d%d%d.%d%d%d.%d%d%d",serverip[0],serverip[1],serverip[2],serverip[3],serverip[4],serverip[5],serverip[6],serverip[7],serverip[8],serverip[9],serverip[10],serverip[11] );	
	
	GLCD_string (0,4 ,str);
	
	
	loc = 0;
	blink=2000;
	do
	{
		blink++;
		if(blink==3500)
		{lcd_normal();
		 switch(loc)
		 {
			 case 0:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (0,4 ,str); 			 
       break;			 
	     case 1:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (1,4 ,str); 			 
       break;			 
			 case 2:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (2,4 ,str); 			 
       break;			 
			 case 3:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (4,4 ,str); 			 
       break;			 
			 case 4:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (5,4 ,str); 			 
       break;			 
			 case 5:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (6,4 ,str); 			 
        break;			 
			 
			 case 6:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (8,4 ,str); 			 
        break;			 
			 case 7:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (9,4 ,str); 			 
        break;			 
			 case 8:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (10,4 ,str); 			 
        break;			 
			 
			 case 9:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (12,4 ,str); 			 
        break;			 
			 case 10:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (13,4 ,str); 			 
        break;			 
			 case 11:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (14,4 ,str); 			 
        break;			 
			 
			 default: break; 
		 }	//of switch 
			
		}

    else if(blink>7000)
		{
    blink=0;
		lcd_inverse();
			switch(loc)
		 {
		   case 0:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (0,4 ,str); 			 
       break;			 
	     case 1:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (1,4 ,str); 			 
       break;			 
			 case 2:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (2,4 ,str); 			 
       break;			 
			 case 3:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (4,4 ,str); 			 
       break;			 
			 case 4:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (5,4 ,str); 			 
       break;			 
			 case 5:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (6,4 ,str); 			 
        break;			 
			 
			 case 6:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (8,4 ,str); 			 
        break;			 
			 case 7:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (9,4 ,str); 			 
        break;			 
			 case 8:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (10,4 ,str); 			 
        break;			 
			 
			 case 9:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (12,4 ,str); 			 
        break;			 
			 case 10:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (13,4 ,str); 			 
        break;			 
			 case 11:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (14,4 ,str); 			 
        break;			 
			 
			 default: break; 
			 
			 
		 }	//of switch 
     lcd_normal();			


    }			
		
		if (keypush_8510()) // 0830
		{
			
			switch(KeyCode) //99년 12월 31일 
			{
				case UP:
					serverip[loc]++;
				 if(serverip[loc]>9)
				   {
				   serverip[loc]=9;
						retry_message(); 
			     }
					/*
					tbuf[loc]++;
				 if(loc==0 |loc==1|loc==5 )
         {					 
				 if(tbuf[loc]>9)
				   {
				   tbuf[loc]=9;
						retry_message(); 
			     }
				 }
         if(loc==2)
				 {
			    	if(tbuf[loc]>1)
						{
						retry_message();
						 tbuf[loc]=1;
						}
				 }	 
				 if(loc==3 )
				 {
			    	if(tbuf[loc]>2)
						{
						retry_message();
						 tbuf[loc]=2;
						}
				 }
				 if(loc==4 )
				 {
			    	if(tbuf[loc]>3)
						{
						retry_message();
						 tbuf[loc]=3;
						}
				 }
				 */
					break;
				
        case DOWN:
					if(serverip[loc]!=0) 
					serverip[loc]--;
					else
						retry_message();
				  break;
				
				case LEFT:
					Delay_ms(1);
				  break;
				case RIGHT:
					 loc++;
				   if(loc>=12)
						  loc=0;
				  break;
				case ENTER:
					exit_flag=1;
				  break;
				default: break;
			} // of switch 
			
			
			//sprintf(str, "20%1x%1x-%1x%1x-%1x%1x",tbuf[0],tbuf[1],tbuf[2],tbuf[3],tbuf[4],tbuf[5] );	
			sprintf(str, "%d%d%d.%d%d%d.%d%d%d.%d%d%d",serverip[0],serverip[1],serverip[2],serverip[3],serverip[4],serverip[5],serverip[6],serverip[7],serverip[8],serverip[9],serverip[10],serverip[11] );	
	    GLCD_string (0,4 ,str);

		}// of keypush	
	} while (!exit_flag);
	
	
	/*
	IpAdresss_1=((serverip[0]-0x30)*100)+((serverip[1]-0x30)*10)+(serverip[2]-0x30);
	IpAdresss_2=((serverip[3]-0x30)*100)+((serverip[4]-0x30)*10)+(serverip[5]-0x30);
	IpAdresss_3=((serverip[6]-0x30)*100)+((serverip[7]-0x30)*10)+(serverip[8]-0x30);
	IpAdresss_4=((serverip[9]-0x30)*100)+((serverip[10]-0x30)*10)+(serverip[11]-0x30);
	*/

  IpAdresss_1=((serverip[0])*100)+((serverip[1])*10)+(serverip[2]);
	IpAdresss_2=((serverip[3])*100)+((serverip[4])*10)+(serverip[5]);
	IpAdresss_3=((serverip[6])*100)+((serverip[7])*10)+(serverip[8]);
	IpAdresss_4=((serverip[9])*100)+((serverip[10])*10)+(serverip[11]);
	

	IpAdresss_1 = EE_Write_val(ADR_IpAdresss_1, IpAdresss_1, 4); Delay_ms(10);
 IpAdresss_2 = EE_Write_val(ADR_IpAdresss_2, IpAdresss_2, 4); Delay_ms(10);
 IpAdresss_3 = EE_Write_val(ADR_IpAdresss_3, IpAdresss_3, 4); Delay_ms(10);
 IpAdresss_4 = EE_Write_val(ADR_IpAdresss_4, IpAdresss_4, 4); Delay_ms(10);

	
	
	
	
	
	 /* 
   	Settimebuf[6] =tbuf[0]*16+tbuf[1]; //年 
    Settimebuf[4] =tbuf[2]*16+tbuf[3]; 0x12; //月 
 	  Settimebuf[3] =tbuf[4]*16+tbuf[5]; 0x01; //日 
    Settimebuf[2] =Gettimebuf[2]; //? 
    Settimebuf[1] =Gettimebuf[1]; //分 
    Settimebuf[0] =Gettimebuf[0];//秒 
		Set1381(Settimebuf);//setting current data & time  
	*/


	
} /* of set_date */

void set_port()
{
	int i;
	char loc=0;
	int  count;
	int  blink;
	unsigned char	digit_max[6] = { 9, 9, 1, 9, 3, 9 };
	unsigned char	exit_flag =0;
	unsigned char	curr_num = 0;
	char			tbuf[6];
	char  serverip[15];
	char			year, month, date;
	char  str[50];
	lcd_normal();
	lcd_clear();
	lcd_normal();
	if(gnlLanguage==1)//국문
	GLCD_string (0,0 ," 포트 설정 ");
	else
	GLCD_string (0,0 ," PORT SETTING");
	
	//IpPort=1491;
	//12345
	serverip[0]=IpPort/10000;
	serverip[1]=(IpPort%10000)/1000;
	serverip[2]=((IpPort%10000)%1000)/100;
	serverip[3]=(((IpPort%10000)%1000)%100)/10;
	serverip[4]=(((IpPort%10000)%1000)%100)%10;
	
	
	sprintf(str, "%d%d%d%d%d",serverip[0],serverip[1],serverip[2],serverip[3],serverip[4] );	
	
	GLCD_string (0,4 ,str);
	
	
	loc = 0;
	blink=2000;
	do
	{
		blink++;
		if(blink==3500)
		{lcd_normal();
		 switch(loc)
		 {
			 case 0:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (0,4 ,str); 			 
       break;			 
	     case 1:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (1,4 ,str); 			 
       break;			 
			 case 2:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (2,4 ,str); 			 
       break;			 
			 case 3:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (3,4 ,str); 			 
       break;			 
			 case 4:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (4,4 ,str); 			 
       break;			 
			 
			 default: break; 
		 }	//of switch 
			
		}

    else if(blink>7000)
		{
    blink=0;
		lcd_inverse();
			switch(loc)
		 {
		   case 0:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (0,4 ,str); 			 
       break;			 
	     case 1:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (1,4 ,str); 			 
       break;			 
			 case 2:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (2,4 ,str); 			 
       break;			 
			 case 3:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (3,4 ,str); 			 
       break;			 
			 case 4:
				 sprintf(str, "%1d",serverip[loc]); // A/D값
         GLCD_string (4,4 ,str); 			 
       break;			 
			 default: break; 
			 
			 
		 }	//of switch 
     lcd_normal();			


    }			
		
		if (keypush_8510()) // 0830
		{
			
			switch(KeyCode) //99년 12월 31일 
			{
				case UP:
					serverip[loc]++;
				 if(serverip[loc]>9)
				   {
				   serverip[loc]=9;
						retry_message(); 
			     }
					break;
				
        case DOWN:
					if(serverip[loc]!=0) 
					serverip[loc]--;
					else
						retry_message();
				  break;
				
				case LEFT:
					Delay_ms(1);
				  break;
				case RIGHT:
					 loc++;
				   if(loc>=5)
						  loc=0;
				  break;
				case ENTER:
					exit_flag=1;
				  break;
				default: break;
			} // of switch 
			
			
			//sprintf(str, "20%1x%1x-%1x%1x-%1x%1x",tbuf[0],tbuf[1],tbuf[2],tbuf[3],tbuf[4],tbuf[5] );	
			sprintf(str, "%d%d%d%d%d",serverip[0],serverip[1],serverip[2],serverip[3],serverip[4] );	
	    GLCD_string (0,4 ,str);

		}// of keypush	
	} while (!exit_flag);
	
	
	
	//IpPort=((serverip[0]-0x30)*10000)+((serverip[1]-0x30)*1000)+((serverip[2]-0x30)*100)+((serverip[3]-0x30)*10)+((serverip[4]-0x30));
	IpPort=((serverip[0])*10000)+((serverip[1])*1000)+((serverip[2])*100)+((serverip[3])*10)+((serverip[4]));
	
 IpPort = EE_Write_val(ADR_IpPort, IpPort, 4); Delay_ms(10);
 
	
	
	
	
	 /* 
   	Settimebuf[6] =tbuf[0]*16+tbuf[1]; //年 
    Settimebuf[4] =tbuf[2]*16+tbuf[3]; 0x12; //月 
 	  Settimebuf[3] =tbuf[4]*16+tbuf[5]; 0x01; //日 
    Settimebuf[2] =Gettimebuf[2]; //? 
    Settimebuf[1] =Gettimebuf[1]; //分 
    Settimebuf[0] =Gettimebuf[0];//秒 
		Set1381(Settimebuf);//setting current data & time  
	*/


	
} /* of set_date */



void Phone_Sms_Processing(void)
   {
		 int i;
		 int temp[7];
		 int tel_number[11];
	
 /*
		 if(keypush_8510())
	{
		//ch=(keypush_8510());

		switch(KeyCode)
		{
		 	
			case  UP:
				strcpy(szMsg, "AT$LGTCTRM?"); // SMS 메세지 갯수
		 cdma_send_cmd_with_buffer_clear(szMsg, strlen(szMsg));
				break;
			
			case  DOWN:
				
			
			strcpy(szMsg, "AT$LGTDELRM=*"); // SMS 메세지 전체지우기 
		 cdma_send_cmd_with_buffer_clear(szMsg, strlen(szMsg));
				break;
		
     case  ENTER:
		     break;
		 
		 case  RIGHT: // 배출량 기록 "0" 으로 초기화 
				break;
		
			break;
		} // of switch 
	}
	 
	 */
	 
	 //핸드폰 어플로부터  "START"라는 문자메세지를 받았을때 
	 if(Usart3_sms_received_flag==1) // 외부전화로 부터 sms 가 수신되면 //처음에 #으로 시작하는 문자가 오면
	{
			if(RxBuffer2_sms[1]=='L'&& RxBuffer2_sms[2]=='G'&&RxBuffer2_sms[3]=='T'&&RxBuffer2_sms[4]=='R'&&RxBuffer2_sms[5]=='M'&&
				
			RxBuffer2_sms[47]=='5'&&RxBuffer2_sms[48]=='3'&&RxBuffer2_sms[51]=='5'&&RxBuffer2_sms[52]=='4'
			)//문자메세지를 처음에 정상적으로 받았을때
			{
			
				
				
				//cdma reset 명령
			//	cdma_send_cmd("AT$LGTRESET", 11);	//2014.10.27
				
				
				
				
		/*********  문자보낸번호로 응답문자 보내기**********************************/
				USART3_putchar('A');
				USART3_putchar('T');
				USART3_putchar('$');
				USART3_putchar('L');
				USART3_putchar('G');
				USART3_putchar('T');
				USART3_putchar('S');
				USART3_putchar('N');
				USART3_putchar('D');
				USART3_putchar('S');
				USART3_putchar('M');
			  USART3_putchar('=');
			
      // 수신받을 사람 전화번호			
				USART3_putchar(RxBuffer2_sms[7]);
				USART3_putchar(RxBuffer2_sms[8]);
				USART3_putchar(RxBuffer2_sms[9]);
				USART3_putchar(RxBuffer2_sms[10]);
				USART3_putchar(RxBuffer2_sms[11]);
				USART3_putchar(RxBuffer2_sms[12]);
				USART3_putchar(RxBuffer2_sms[13]);
				USART3_putchar(RxBuffer2_sms[14]);
				USART3_putchar(RxBuffer2_sms[15]);
				USART3_putchar(RxBuffer2_sms[16]);
				USART3_putchar(RxBuffer2_sms[17]);
				
				
				USART3_putchar(',');
				USART3_putchar('0'); // short message
				USART3_putchar(',');
				USART3_putchar('0'); // 0: 아스키코드표  1: utf-8
				USART3_putchar(',');
				
				/******** 송신메세지*****************/
				USART3_putchar('"');
				
				sprintf(sT0,"House Number=%2d ,               ",gnlMyAddress);
				USART3_print(sT0);
				sprintf(sT0,"GPI-8530 ID=M%u  Remote Control Start!",gnlSerialNo);
				USART3_print(sT0);
				
				
				
				USART3_putchar('"');
				/******** 송신메세지*****************/
			  USART3_putchar(0x0d);
				USART3_putchar(0x0a);
		/**************문자보낸번호로 응답문자 보내기*****************************/
		
		
				 // strcpy(szMsg, "AT$LGTACK"); // SMS 수신했으면 잘받았다고  모듈에 전송을 한다. // 그래야 안읽은 메세지로 남지않느다.
			  //	cdma_send_cmd_with_buffer_clear(szMsg, strlen(szMsg));
			
				 beep(100);
   			

//Delay_ms(500);
				 //beep(100);
				 
				
				
				
			  Usart3_sms_received_flag=0;
			  RxCounter2_sms=0;
					
				//GLCD_string(0,2, "  처음문자");	
			 sprintf(sT0, "TEL=%c%c%c%c%c%c%c%c%c%c%c",RxBuffer2_sms[7],RxBuffer2_sms[8],RxBuffer2_sms[9],RxBuffer2_sms[10],RxBuffer2_sms[11],RxBuffer2_sms[12],RxBuffer2_sms[13],RxBuffer2_sms[14],RxBuffer2_sms[15],RxBuffer2_sms[16],RxBuffer2_sms[17]);
       //GLCD_string(0,2, sT0);	 
			 USART1_puts(sT0);
   
			sprintf(sT0, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c",RxBuffer2_sms[19],RxBuffer2_sms[20],RxBuffer2_sms[21],RxBuffer2_sms[22],RxBuffer2_sms[23],RxBuffer2_sms[24],RxBuffer2_sms[25],
                                                                                    RxBuffer2_sms[26],RxBuffer2_sms[27],RxBuffer2_sms[28],RxBuffer2_sms[29],RxBuffer2_sms[30],RxBuffer2_sms[31],RxBuffer2_sms[32]);
			 //GLCD_string(0,4, sT0); 
			   USART1_puts(sT0);
				  
			sprintf(sT0, "%c%c%c%c%c%c%c%c%c%c",RxBuffer2_sms[47],RxBuffer2_sms[48],RxBuffer2_sms[51],RxBuffer2_sms[52],RxBuffer2_sms[55],RxBuffer2_sms[56],RxBuffer2_sms[59],
                                                                                    RxBuffer2_sms[60],RxBuffer2_sms[63],RxBuffer2_sms[64]);
			 //GLCD_string(0,6, sT0); 
			  USART1_puts(sT0);
					
				
				  strcpy(szMsg, "AT$LGTACK"); // SMS 수신했으면 잘받았다고  모듈에 전송을 한다. // 그래야 안읽은 메세지로 남지않느다.
			  	cdma_send_cmd_with_buffer_clear(szMsg, strlen(szMsg));
					
					//	strcpy(szMsg, "AT$LGTDELRM=*"); // SMS 메세지 전체지우기 
		      //  cdma_send_cmd_with_buffer_clear(szMsg, strlen(szMsg));
			    //  Delay_ms(2000);
					
       /*				
				//자동출력신호  on 
					     Gpi8510_Relay_On(SILO_TWO);
									  beep(1500);
									  Gpi8510_Relay_Off(SILO_TWO);
				*/	
					
          for(i=0;i<100;i++)
	        RxBuffer2_sms[i]=0xff; //초기환

        // goto END_FUNCTION;
				
				
				 strcpy(szMsg, "AT$LGTDELSM=*"); // SMS 송신 메세지 전체지우기 
	   cdma_send_cmd_with_buffer_clear(szMsg, strlen(szMsg));
		

				}
			
				#ifdef dpfdpfdpf
     if(RxBuffer2_sms[1]=='L'&& RxBuffer2_sms[2]=='G'&&RxBuffer2_sms[3]=='T'&&RxBuffer2_sms[4]=='R'&&RxBuffer2_sms[5]=='S')//문자메세지를 처음에 2,3 번째 받았을때  정상적으로 받았을때
			{
				//$LGTRSM:01034431529,20201123122847,4098,20,2,"00530054004100520054"
   
				
				
				/*********  문자보낸번호로 응답문자 보내기**********************************/
				USART3_putchar('A');
				USART3_putchar('T');
				USART3_putchar('$');
				USART3_putchar('L');
				USART3_putchar('G');
				USART3_putchar('T');
				USART3_putchar('S');
				USART3_putchar('N');
				USART3_putchar('D');
				USART3_putchar('S');
				USART3_putchar('M');
			  USART3_putchar('=');
			
      // 수신받을 사람 전화번호			
				USART3_putchar(RxBuffer2_sms[8]);
				USART3_putchar(RxBuffer2_sms[9]);
				USART3_putchar(RxBuffer2_sms[10]);
				USART3_putchar(RxBuffer2_sms[11]);
				USART3_putchar(RxBuffer2_sms[12]);
				USART3_putchar(RxBuffer2_sms[13]);
				USART3_putchar(RxBuffer2_sms[14]);
				USART3_putchar(RxBuffer2_sms[15]);
				USART3_putchar(RxBuffer2_sms[16]);
				USART3_putchar(RxBuffer2_sms[17]);
				USART3_putchar(RxBuffer2_sms[18]);
				
				
				USART3_putchar(',');
				USART3_putchar('0'); // short message
				USART3_putchar(',');
				USART3_putchar('0'); // 0: 아스키코드표
				USART3_putchar(',');
				
				/******** 송신메세지*****************/
				USART3_putchar('"');
				sprintf(sT0,"ID=N%u Start!",gnlSerialNo);
				USART3_print(sT0);
				USART3_putchar('"');
				/******** 송신메세지*****************/
			  USART3_putchar(0x0d);
				USART3_putchar(0x0a);
		/**************문자보낸번호로 응답문자 보내기*****************************/
			
				 beep(100);
				 //Delay_ms(500);
				 //beep(100);
				 
				 strcpy(szMsg, "AT$LGTACK"); // SMS 수신했으면 잘받았다고  모듈에 전송을 한다. // 그래야 안읽은 메세지로 남지않느다.
			  	cdma_send_cmd_with_buffer_clear(szMsg, strlen(szMsg));
				
				
			  Usart3_sms_received_flag=0;
			  RxCounter2_sms=0;
					
				GLCD_string(0,2, "  두번문자");	
			 sprintf(sT0, "TEL=%c%c%c%c%c%c%c%c%c%c%c",RxBuffer2_sms[8],RxBuffer2_sms[9],RxBuffer2_sms[10],RxBuffer2_sms[11],RxBuffer2_sms[12],RxBuffer2_sms[13],RxBuffer2_sms[14],RxBuffer2_sms[15],RxBuffer2_sms[16],RxBuffer2_sms[17],RxBuffer2_sms[18]);
       //GLCD_string(0,2, sT0);	 
			 USART1_puts(sT0);
   
			sprintf(sT0, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c",RxBuffer2_sms[20],RxBuffer2_sms[21],RxBuffer2_sms[22],RxBuffer2_sms[23],RxBuffer2_sms[24],RxBuffer2_sms[25],RxBuffer2_sms[26],
                                                                                    RxBuffer2_sms[27],RxBuffer2_sms[28],RxBuffer2_sms[29],RxBuffer2_sms[30],RxBuffer2_sms[31],RxBuffer2_sms[32],RxBuffer2_sms[33]);
			 //GLCD_string(0,4, sT0); 
			   USART1_puts(sT0);
				  
			sprintf(sT0, "%c%c%c%c%c%c%c%c%c%c",RxBuffer2_sms[48],RxBuffer2_sms[49],RxBuffer2_sms[52],RxBuffer2_sms[53],RxBuffer2_sms[56],RxBuffer2_sms[57],RxBuffer2_sms[60],
                                                                                    RxBuffer2_sms[61],RxBuffer2_sms[64],RxBuffer2_sms[65]);
			 //GLCD_string(0,6, sT0); 
			  USART1_puts(sT0);
					
				
				  strcpy(szMsg, "AT$LGTACK"); // SMS 수신했으면 잘받았다고  모듈에 전송을 한다. // 그래야 안읽은 메세지로 남지않느다.
			  	cdma_send_cmd_with_buffer_clear(szMsg, strlen(szMsg));
				//strcpy(szMsg, "AT$LGTDELRM=*"); // SMS 메세지 전체지우기 
		     //cdma_send_cmd_with_buffer_clear(szMsg, strlen(szMsg));
				 //Delay_ms(2000);
			 
          for(i=0;i<100;i++)
	        RxBuffer2_sms[i]=0xff; //초기환

				
				
       /*			
				for(i=0;i<RxCounter2_sms;i++)
			    {
						USART1_putchar(RxBuffer2_sms[i]);
					}	
			   USART1_putchar(0x0d);
				 USART1_putchar(0x0a);
					
			  Usart3_sms_received_flag=0;
			  RxCounter2_sms=0;
		    	GLCD_string(0,2, "  두번문자");	
			 sprintf(sT0, "TEL=%c%c%c%c%c%c%c%c%c%c%c",RxBuffer2_sms[8],RxBuffer2_sms[9],RxBuffer2_sms[10],RxBuffer2_sms[11],RxBuffer2_sms[12],RxBuffer2_sms[13],RxBuffer2_sms[14],RxBuffer2_sms[15],RxBuffer2_sms[16],RxBuffer2_sms[17],RxBuffer2_sms[18]);
       //GLCD_string(0,2, sT0);	 
					USART1_puts(sT0);
   
			sprintf(sT0, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c",RxBuffer2_sms[20],RxBuffer2_sms[21],RxBuffer2_sms[22],RxBuffer2_sms[23],RxBuffer2_sms[24],RxBuffer2_sms[25],RxBuffer2_sms[26],
                                                                                    RxBuffer2_sms[27],RxBuffer2_sms[28],RxBuffer2_sms[29],RxBuffer2_sms[30],RxBuffer2_sms[31],RxBuffer2_sms[32],RxBuffer2_sms[33]);
			 //GLCD_string(0,4, sT0); 
					USART1_puts(sT0);
				  
			sprintf(sT0, "%c%c%c%c%c%c%c%c%c%c",RxBuffer2_sms[48],RxBuffer2_sms[49],RxBuffer2_sms[52],RxBuffer2_sms[53],RxBuffer2_sms[56],RxBuffer2_sms[57],RxBuffer2_sms[60],
                                                                                    RxBuffer2_sms[61],RxBuffer2_sms[64],RxBuffer2_sms[65]);
			 //GLCD_string(0,6, sT0); 
					USART1_puts(sT0);
			



					
      	  strcpy(szMsg, "AT$LGTACK"); // SMS 수신했으면 잘받았다고  모듈에 전송을 한다. // 그래야 안읽은 메세지로 남지않느다.
			  	cdma_send_cmd_with_buffer_clear(szMsg, strlen(szMsg));
			 
          for(i=0;i<100;i++)
	        RxBuffer2_sms[i]=0xff; //초기환
         */
				
				
				}
			
				#endif
			/*	
			 else	
			 {	
				  for(i=0;i<RxCounter2_sms;i++)
			    {
						USART1_putchar(RxBuffer2_sms[i]);
						
					}	
			   USART1_putchar(0x0d);
				 USART1_putchar(0x0a);
				 
					 Usart3_sms_received_flag=0;
			    RxCounter2_sms=0;
					for(i=0;i<100;i++)
	        RxBuffer2_sms[i]=0xff; //초기환
			 }
			*/	
				END_FUNCTION:			
				
					
				  for(i=0;i<RxCounter2_sms;i++)
			    {
						USART1_putchar(RxBuffer2_sms[i]);
						
					}	
			   USART1_putchar(0x0d);
				 USART1_putchar(0x0a);
				 
					 Usart3_sms_received_flag=0;
			    RxCounter2_sms=0;
					for(i=0;i<100;i++)
	        RxBuffer2_sms[i]=0xff; //초기환
					
					
		// strcpy(szMsg, "AT$LGTDELSM=*"); // SMS 송신 메세지 전체지우기 
	  // cdma_send_cmd_with_buffer_clear(szMsg, strlen(szMsg));
					
					
				
		}	// of if(Usart3_sms_received_flag==1) // 외부전화로 부터 sms 가 수신되면 //처음에 #으로 시작하는 문자가 오면

	  //for(i=0;i<100;i++)
	 //RxBuffer2_sms[i]=0xff; //초기환
   } // of funciton

/* Agent_DischargeInput_Task() moved to app/agent_discharge_input.c (Phase 2).
   The dead #ifdef dpddpp / commented-out variants right below are left
   untouched (never compiled - dpddpp is undefined everywhere). */
unsigned char Agent_DischargeInput_Task(void);

#define AGENT_DISCHARGE_TASK_NONE          0
#define AGENT_DISCHARGE_TASK_FIRST_START   1
#define AGENT_DISCHARGE_TASK_SECOND_START  2
	 #ifdef dpddpp
	 unsigned char Agent_DischargeInput_Task(void)
{
    unsigned char first_raw;
    unsigned char second_raw;

    first_raw  = FIRST_INPUT_RAW();
    second_raw = SECOND_INPUT_RAW();

    /*
     * 입력 확인용 디버그
     * 입력 들어오면 RAW1 또는 RAW2가 1로 찍혀야 정상
     */
    sprintf(str, "RAW1=%d RAW2=%d ek1=%d ek2=%d\r\n",
            first_raw, second_raw, ek_flag1, ek_flag2);
    USART1_puts(str);

    switch(AGENT_DischargeInput_Process(&g_first_input,
                                        first_raw,
                                        &ek_flag1,
                                        Output_Low_Error,
                                        Output_High_Error,
                                        sign,
                                        &start_sign_flag))
    {
        case AGENT_INPUT_EVENT_START:
            ek_flag1 = 1;
            stop_flag = 0;
            First_Relay_On();
            USART1_puts("FIRST START EVENT\r\n");
            return AGENT_DISCHARGE_TASK_FIRST_START;

        case AGENT_INPUT_EVENT_STOP:
            First_Relay_Off();
            byNewStart = gxhEndDelay;
            ek_flag1 = 0;
            stop_flag = 1;
            USART1_puts("FIRST STOP EVENT\r\n");
            break;

        default:
            break;
    }

    switch(AGENT_DischargeInput_Process(&g_second_input,
                                        second_raw,
                                        &ek_flag2,
                                        Output_Low_Error2,
                                        Output_High_Error2,
                                        sign_2,
                                        &start_sign_flag_2))
    {
        case AGENT_INPUT_EVENT_START:
            ek_flag2 = 1;
            stop_flag2 = 0;
            Second_Relay_On();
            USART1_puts("SECOND START EVENT\r\n");
            return AGENT_DISCHARGE_TASK_SECOND_START;

        case AGENT_INPUT_EVENT_STOP:
            Second_Relay_Off();
            byNewStart2 = gxhEndDelay;
            ek_flag2 = 0;
            stop_flag2 = 1;
            USART1_puts("SECOND STOP EVENT\r\n");
            break;

        default:
            break;
    }

    return AGENT_DISCHARGE_TASK_NONE;
} // Agent_DischargeInput_Task
#endif	


/*
unsigned char Agent_DischargeInput_Task(void)
{
    switch(AGENT_DischargeInput_Process(&g_first_input,
                                        FIRST_INPUT_RAW(),
                                        &ek_flag1,
                                        Output_Low_Error,
                                        Output_High_Error,
                                        sign,
                                        &start_sign_flag))
    {
        case AGENT_INPUT_EVENT_START:
            First_Relay_On();
            return AGENT_DISCHARGE_TASK_FIRST_START;

        case AGENT_INPUT_EVENT_STOP:
            First_Relay_Off();
            byNewStart = gxhEndDelay;
            stop_flag = 1;
            break;

        default:
            break;
    }

    switch(AGENT_DischargeInput_Process(&g_second_input,
                                        SECOND_INPUT_RAW(),
                                        &ek_flag2,
                                        Output_Low_Error2,
                                        Output_High_Error2,
                                        sign_2,
                                        &start_sign_flag_2))
    {
        case AGENT_INPUT_EVENT_START:
            Second_Relay_On();
            return AGENT_DISCHARGE_TASK_SECOND_START;

        case AGENT_INPUT_EVENT_STOP:
            Second_Relay_Off();
            byNewStart2 = gxhEndDelay;
            stop_flag2 = 1;
            USART1_puts("Second input real off!\r\n");
            break;

        default:
            break;
    }

    return AGENT_DISCHARGE_TASK_NONE;
}
	 */
	 
	 /* Agent_WeightDisplay_Task() moved to app/agent_weight_display.c (Phase 2). */
void Agent_WeightDisplay_Task(void);
	 
	 
/* Agent_DischargeCalc_Task() moved to app/agent_discharge_calc.c (Phase 2). */
unsigned char Agent_DischargeCalc_Task(void);
	 

/* Agent_DischargeStop_Task() moved to app/agent_discharge_stop.c (Phase 2). */
void Agent_DischargeStop_Task(void);

/* Agent_BaichulSend_Task() moved to app/agent_baichul_send.c (Phase 2). */
void Agent_BaichulSend_Task(void);
#ifdef dpdpd
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

        if(gnlSilo1Selection == 1)
        {
					Agent_MakeBaichulPacket("BC-B"); 
					 /*
            sprintf(sT0,
                    "%c|01212345678|M000|77777|M%07ld|M%03ld|BC-B|%s|%lx|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%c",
                    STX,
                    gnlSerialNo,
                    gnlMyAddress,
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
                    ETX); */
        }
        else
        {
					Agent_MakeBaichulPacket("BC-S");
					 /*
            sprintf(sT0,
                    "%c|01212345678|M000|77777|M%07ld|M%03ld|BC-S|%s|%lx|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%c",
                    STX,
                    gnlSerialNo,
                    gnlMyAddress,
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
										*/
										
        }

        if(gnlSendingInterval)
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

            if(gnlSilo1Selection == 1)
            {
							Agent_MakeBaichulPacket("BC-J");
							 /*
                sprintf(sT0,
                        "%c|01212345678|M000|77777|M%07ld|M%03ld|BC-J|%s|%lx|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%c",
                        STX,
                        gnlSerialNo,
                        gnlMyAddress,
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
												*/
												
            }
            else
            {
							Agent_MakeBaichulPacket("BC-S");
							 /*
                sprintf(sT0,
                        "%c|01212345678|M000|77777|M%07ld|M%03ld|BC-S|%s|%lx|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%c",
                        STX,
                        gnlSerialNo,
                        gnlMyAddress,
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
												*/
												
            }

            if(gnlSendingInterval)
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

        if(gnlSilo1Selection == 1)
        {
					Agent_MakeBaichulPacket("BC-E");
					 /*
            sprintf(sT0,
                    "%c|01212345678|M000|77777|M%07ld|M%03ld|BC-E|%s|%lx|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%c",
                    STX,
                    gnlSerialNo,
                    gnlMyAddress,
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
                    ETX); */
        }
        else
        {
					Agent_MakeBaichulPacket("BC-S");
					 /*
            sprintf(sT0,
                    "%c|01212345678|M000|77777|M%07ld|M%03ld|BC-S|%s|%lx|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%c",
                    STX,
                    gnlSerialNo,
                    gnlMyAddress,
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
										*/
        }

        if(gnlSendingInterval)
        {
            USART2_puts(sT0);
        }

        Send_to_server(SEND_BAICHUL, "BC-E");
    }

    /**************************************************************************
     * 정기 상태 정보 전송
     **************************************************************************/
    if(Sending_Mili_Counter >= (DEF_1_MIN * gnlSendingInterval))
    {
        if(gnlSilo1Selection == 1)
        {
            sprintf(sT0,
                    "%c|01212345678|M000|77777|M%07ld|M%03ld|BC-N|%s|%lx|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%c",
                    STX,
                    gnlSerialNo,
                    gnlMyAddress,
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
        else
        {
            sprintf(sT0,
                    "%c|01212345678|M000|77777|M%07ld|M%03ld|BC-N|%s|%lx|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%c|%c",
                    STX,
                    gnlSerialNo,
                    gnlMyAddress,
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

        Send_to_server(SEND_BAICHUL, "BC-N");

        test_routine();

        if(gnlSendingInterval)
        {
            USART2_puts(sT0);
        }

        Sending_Mili_Counter = 0;
    }
		
		test_routine();//회사서버로 보내기 
} // Agent_BaichulSend_Task

#endif




/* Agent_MakeBaichulPacket() moved to app/agent_make_baichul_packet.c (Phase 2). */
void Agent_MakeBaichulPacket(char *bc_code);


/* Agent_DischargeStopDelay_Task() moved to app/agent_discharge_stop_delay.c (Phase 2). */
unsigned char Agent_DischargeStopDelay_Task(void);

/* Agent_DateChange_Task() moved to app/agent_date_change.c (Phase 2). */
void Agent_DateChange_Task(void);

/******************************************************************************
 * Function : Agent_KeyProcess_Task
 *
 * Description :
 *   키 입력 처리를 담당한다.
 *
 * 처리 내용 :
 *   1. LEFT_ARROW  3초 입력  : SILO1 영점
 *   2. RIGHT_ARROW 3초 입력  : SILO2 영점
 *   3. UP/DOWN              : 오늘/이전일/총누적 배출량 표시
 *   4. KEY_WEIGHT           : 현재무게 화면 복귀
 *   5. KEY_RECORD           : 기록 관련 처리
 *   6. KEY_STATUS           : 장비상태 화면 표시
 *
 * Return :
 *   AGENT_KEY_EVENT_NONE    : 특별 이벤트 없음
 *   AGENT_KEY_EVENT_INITIAL : 기존 goto INITIAL 수행 필요
 *
 * 주의 :
 *   기존 key 처리 코드에 goto INITIAL이 있었기 때문에
 *   함수 내부에서 직접 goto를 사용하지 않고 이벤트를 반환한다.
 ******************************************************************************/
unsigned char Agent_KeyProcess_Task(void)
{
    long key_sum = 0;
    int i;
    int which_day_feeding;
	int Current_Display;
	 
	
    /**************************************************************************
     * LEFT_ARROW 3초 입력 시 SILO1 영점 설정
     **************************************************************************/
    LEFT_ARROW_ON
    {
        Zero_Mili_Counter = 0;
        beep(50);

        while(1)
        {
            LEFT_ARROW_ON
            {
                if(Zero_Mili_Counter > (DEF_1_SEC * 3))
                {
                    Zero_Mili_Counter = 0;
                    beep(500);
                    zero_func();
                    break;
                }
            }
            else
            {
                Zero_Mili_Counter = 0;
                break;
            }
        }
    }

    /**************************************************************************
     * RIGHT_ARROW 3초 입력 시 SILO2 영점 설정
     **************************************************************************/
    if(gnlSilo1Selection == 2)
    {
        RIGHT_ARROW_ON
        {
            Zero_Mili_Counter = 0;
            beep(50);

            while(1)
            {
                RIGHT_ARROW_ON
                {
                    if(Zero_Mili_Counter > (DEF_1_SEC * 3))
                    {
                        Zero_Mili_Counter = 0;
                        beep(500);
                        zero_func2();
                        break;
                    }
                }
                else
                {
                    Zero_Mili_Counter = 0;
                    break;
                }
            }
        }
    }

    /**************************************************************************
     * 일반 키 입력 처리
     **************************************************************************/
    if(keypush_8510())
    {
        switch(KeyCode)
        {
            /******************************************************************
             * UP KEY
             * 오늘 → 1일전 → ... → 7일전 → 총누적 순서로 표시
             ******************************************************************/
            case UP:
                which_day_feeding++;

                if(which_day_feeding > 8)
                {
                    which_day_feeding = 0;
                }

                if(gnlSilo1Selection == 1)
                {
                    if(gnlLanguage == 1)
                    {
                        switch(which_day_feeding)
                        {
                            case 0:
                                sprintf(str, "오 늘: %5ld kg ", (long)gnlDailyTotal);
                                break;

                            case 1:
                                sprintf(str, "1일전: %5ld kg ", (long)gnlPreDailyTotal[0]);
                                break;

                            case 2:
                                sprintf(str, "2일전: %5ld kg ", (long)gnlPreDailyTotal[1]);
                                break;

                            case 3:
                                sprintf(str, "3일전: %5ld kg ", (long)gnlPreDailyTotal[2]);
                                break;

                            case 4:
                                sprintf(str, "4일전: %5ld kg ", (long)gnlPreDailyTotal[3]);
                                break;

                            case 5:
                                sprintf(str, "5일전: %5ld kg ", (long)gnlPreDailyTotal[4]);
                                break;

                            case 6:
                                sprintf(str, "6일전: %5ld kg ", (long)gnlPreDailyTotal[5]);
                                break;

                            case 7:
                                sprintf(str, "7일전: %5ld kg ", (long)gnlPreDailyTotal[6]);
                                break;

                            case 8:
                                sprintf(str, "총누적: %5ld kg", (long)gxhTotalOutput);
                                break;

                            default:
                                break;
                        }
                    }
                    else
                    {
                        switch(which_day_feeding)
                        {
                            case 0:
                                sprintf(str, "TODAY: %5ld kg ", (long)gnlDailyTotal);
                                break;

                            case 1:
                                sprintf(str, "DAY-1: %5ld kg ", (long)gnlPreDailyTotal[0]);
                                break;

                            case 2:
                                sprintf(str, "DAY-2: %5ld kg ", (long)gnlPreDailyTotal[1]);
                                break;

                            case 3:
                                sprintf(str, "DAY-3: %5ld kg ", (long)gnlPreDailyTotal[2]);
                                break;

                            case 4:
                                sprintf(str, "DAY-4: %5ld kg ", (long)gnlPreDailyTotal[3]);
                                break;

                            case 5:
                                sprintf(str, "DAY-5: %5ld kg ", (long)gnlPreDailyTotal[4]);
                                break;

                            case 6:
                                sprintf(str, "DAY-6: %5ld kg ", (long)gnlPreDailyTotal[5]);
                                break;

                            case 7:
                                sprintf(str, "DAY-7: %5ld kg ", (long)gnlPreDailyTotal[6]);
                                break;

                            case 8:
                                sprintf(str, "TOTAL: %5ld kg ", (long)gxhTotalOutput);
                                break;

                            default:
                                break;
                        }
                    }

                    lcd_inverse();
                    GLCD_string(0, 6, str);
                    lcd_normal();
                }
                else
                {
                    if(gnlLanguage == 1)
                    {
                        switch(which_day_feeding)
                        {
                            case 0:
                                key_sum = gnlDailyTotal + gnlDailyTotal2;
                                sprintf(str, "오 늘:  %5ld kg", key_sum);
                                break;

                            case 1:
                                key_sum = gnlPreDailyTotal[0] + gnlPreDailyTotal2[0];
                                sprintf(str, "1일전:  %5ld kg", key_sum);
                                break;

                            case 2:
                                key_sum = gnlPreDailyTotal[1] + gnlPreDailyTotal2[1];
                                sprintf(str, "2일전:  %5ld kg", key_sum);
                                break;

                            case 3:
                                key_sum = gnlPreDailyTotal[2] + gnlPreDailyTotal2[2];
                                sprintf(str, "3일전:  %5ld kg", key_sum);
                                break;

                            case 4:
                                key_sum = gnlPreDailyTotal[3] + gnlPreDailyTotal2[3];
                                sprintf(str, "4일전:  %5ld kg", key_sum);
                                break;

                            case 5:
                                key_sum = gnlPreDailyTotal[4] + gnlPreDailyTotal2[4];
                                sprintf(str, "5일전:  %5ld kg", key_sum);
                                break;

                            case 6:
                                key_sum = gnlPreDailyTotal[5] + gnlPreDailyTotal2[5];
                                sprintf(str, "6일전:  %5ld kg", key_sum);
                                break;

                            case 7:
                                key_sum = gnlPreDailyTotal[6] + gnlPreDailyTotal2[6];
                                sprintf(str, "7일전:  %5ld kg", key_sum);
                                break;

                            case 8:
                                sprintf(str, "총누적: %5ld kg", (long)gxhTotalOutput);
                                break;

                            default:
                                break;
                        }
                    }
                    else
                    {
                        switch(which_day_feeding)
                        {
                            case 0:
                                key_sum = gnlDailyTotal + gnlDailyTotal2;
                                sprintf(str, "TODAY: %5ld kg ", key_sum);
                                break;

                            case 1:
                                key_sum = gnlPreDailyTotal[0] + gnlPreDailyTotal2[0];
                                sprintf(str, "DAY-1: %5ld kg ", key_sum);
                                break;

                            case 2:
                                key_sum = gnlPreDailyTotal[1] + gnlPreDailyTotal2[1];
                                sprintf(str, "DAY-2: %5ld kg ", key_sum);
                                break;

                            case 3:
                                key_sum = gnlPreDailyTotal[2] + gnlPreDailyTotal2[2];
                                sprintf(str, "DAY-3: %5ld kg ", key_sum);
                                break;

                            case 4:
                                key_sum = gnlPreDailyTotal[3] + gnlPreDailyTotal2[3];
                                sprintf(str, "DAY-4: %5ld kg ", key_sum);
                                break;

                            case 5:
                                key_sum = gnlPreDailyTotal[4] + gnlPreDailyTotal2[4];
                                sprintf(str, "DAY-5: %5ld kg ", key_sum);
                                break;

                            case 6:
                                key_sum = gnlPreDailyTotal[5] + gnlPreDailyTotal2[5];
                                sprintf(str, "DAY-6: %5ld kg ", key_sum);
                                break;

                            case 7:
                                key_sum = gnlPreDailyTotal[6] + gnlPreDailyTotal2[6];
                                sprintf(str, "DAY-7: %5ld kg ", key_sum);
                                break;

                            case 8:
                                sprintf(str, "TOTAL: %5ld kg", (long)gxhTotalOutput);
                                break;

                            default:
                                break;
                        }
                    }

                    lcd_inverse();
                    GLCD_string(0, 6, str);
                    lcd_normal();
                }
                break;

            /******************************************************************
             * DOWN KEY
             * UP과 반대 방향으로 날짜별 배출량 표시
             ******************************************************************/
            case DOWN:
                which_day_feeding--;

                if(which_day_feeding < 0)
                {
                    which_day_feeding = 8;
                    break;
                }

                /*
                 * DOWN 처리도 UP과 동일한 표시 규칙을 사용해야 한다.
                 * 기존 동작 유지 목적상 현재는 UP과 유사한 형태로 구성했다.
                 */
                if(gnlSilo1Selection == 1)
                {
                    if(gnlLanguage == 1)
                    {
                        switch(which_day_feeding)
                        {
                            case 0:
                                sprintf(str, "오 늘: %5ld kg ", (long)gnlDailyTotal);
                                break;

                            case 1:
                                sprintf(str, "1일전: %5ld kg ", (long)gnlPreDailyTotal[0]);
                                break;

                            case 2:
                                sprintf(str, "2일전: %5ld kg ", (long)gnlPreDailyTotal[1]);
                                break;

                            case 3:
                                sprintf(str, "3일전: %5ld kg ", (long)gnlPreDailyTotal[2]);
                                break;

                            case 4:
                                sprintf(str, "4일전: %5ld kg ", (long)gnlPreDailyTotal[3]);
                                break;

                            case 5:
                                sprintf(str, "5일전: %5ld kg ", (long)gnlPreDailyTotal[4]);
                                break;

                            case 6:
                                sprintf(str, "6일전: %5ld kg ", (long)gnlPreDailyTotal[5]);
                                break;

                            case 7:
                                sprintf(str, "7일전: %5ld kg ", (long)gnlPreDailyTotal[6]);
                                break;

                            case 8:
                                sprintf(str, "총누적: %5ld kg", (long)gxhTotalOutput);
                                break;

                            default:
                                break;
                        }
                    }
                    else
                    {
                        switch(which_day_feeding)
                        {
                            case 0:
                                sprintf(str, "TODAY: %5ld kg ", (long)gnlDailyTotal);
                                break;

                            case 1:
                                sprintf(str, "DAY-1: %5ld kg ", (long)gnlPreDailyTotal[0]);
                                break;

                            case 2:
                                sprintf(str, "DAY-2: %5ld kg ", (long)gnlPreDailyTotal[1]);
                                break;

                            case 3:
                                sprintf(str, "DAY-3: %5ld kg ", (long)gnlPreDailyTotal[2]);
                                break;

                            case 4:
                                sprintf(str, "DAY-4: %5ld kg ", (long)gnlPreDailyTotal[3]);
                                break;

                            case 5:
                                sprintf(str, "DAY-5: %5ld kg ", (long)gnlPreDailyTotal[4]);
                                break;

                            case 6:
                                sprintf(str, "DAY-6: %5ld kg ", (long)gnlPreDailyTotal[5]);
                                break;

                            case 7:
                                sprintf(str, "DAY-7: %5ld kg ", (long)gnlPreDailyTotal[6]);
                                break;

                            case 8:
                                sprintf(str, "TOTAL: %5ld kg", (long)gxhTotalOutput);
                                break;

                            default:
                                break;
                        }
                    }

                    lcd_inverse();
                    GLCD_string(0, 6, str);
                    lcd_normal();
                }
                else
                {
                    if(gnlLanguage == 1)
                    {
                        switch(which_day_feeding)
                        {
                            case 0:
                                key_sum = gnlDailyTotal + gnlDailyTotal2;
                                sprintf(str, "오 늘:  %5ld kg", key_sum);
                                break;

                            case 1:
                                key_sum = gnlPreDailyTotal[0] + gnlPreDailyTotal2[0];
                                sprintf(str, "1일전:  %5ld kg", key_sum);
                                break;

                            case 2:
                                key_sum = gnlPreDailyTotal[1] + gnlPreDailyTotal2[1];
                                sprintf(str, "2일전:  %5ld kg", key_sum);
                                break;

                            case 3:
                                key_sum = gnlPreDailyTotal[2] + gnlPreDailyTotal2[2];
                                sprintf(str, "3일전:  %5ld kg", key_sum);
                                break;

                            case 4:
                                key_sum = gnlPreDailyTotal[3] + gnlPreDailyTotal2[3];
                                sprintf(str, "4일전:  %5ld kg", key_sum);
                                break;

                            case 5:
                                key_sum = gnlPreDailyTotal[4] + gnlPreDailyTotal2[4];
                                sprintf(str, "5일전:  %5ld kg", key_sum);
                                break;

                            case 6:
                                key_sum = gnlPreDailyTotal[5] + gnlPreDailyTotal2[5];
                                sprintf(str, "6일전:  %5ld kg", key_sum);
                                break;

                            case 7:
                                key_sum = gnlPreDailyTotal[6] + gnlPreDailyTotal2[6];
                                sprintf(str, "7일전 : %5ld kg", key_sum);
                                break;

                            case 8:
                                sprintf(str, "총누적: %5ld kg", (long)gxhTotalOutput);
                                break;

                            default:
                                break;
                        }
                    }
                    else
                    {
                        switch(which_day_feeding)
                        {
                            case 0:
                                key_sum = gnlDailyTotal + gnlDailyTotal2;
                                sprintf(str, "TODAY: %5ld kg ", key_sum);
                                break;

                            case 1:
                                key_sum = gnlPreDailyTotal[0] + gnlPreDailyTotal2[0];
                                sprintf(str, "DAY-1: %5ld kg ", key_sum);
                                break;

                            case 2:
                                key_sum = gnlPreDailyTotal[1] + gnlPreDailyTotal2[1];
                                sprintf(str, "DAY-2: %5ld kg ", key_sum);
                                break;

                            case 3:
                                key_sum = gnlPreDailyTotal[2] + gnlPreDailyTotal2[2];
                                sprintf(str, "DAY-3: %5ld kg ", key_sum);
                                break;

                            case 4:
                                key_sum = gnlPreDailyTotal[3] + gnlPreDailyTotal2[3];
                                sprintf(str, "DAY-4: %5ld kg ", key_sum);
                                break;

                            case 5:
                                key_sum = gnlPreDailyTotal[4] + gnlPreDailyTotal2[4];
                                sprintf(str, "DAY-5: %5ld kg ", key_sum);
                                break;

                            case 6:
                                key_sum = gnlPreDailyTotal[5] + gnlPreDailyTotal2[5];
                                sprintf(str, "DAY-6: %5ld kg ", key_sum);
                                break;

                            case 7:
                                key_sum = gnlPreDailyTotal[6] + gnlPreDailyTotal2[6];
                                sprintf(str, "DAY-7: %5ld kg ", key_sum);
                                break;

                            case 8:
                                sprintf(str, "TOTAL: %5ld kg", (long)gxhTotalOutput);
                                break;

                            default:
                                break;
                        }
                    }

                    lcd_inverse();
                    GLCD_string(0, 6, str);
                    lcd_normal();
                }
                break;

            /******************************************************************
             * KEY_WEIGHT
             * 현재무게 기본 화면으로 복귀
             ******************************************************************/
            case KEY_WEIGHT:
                return AGENT_KEY_EVENT_INITIAL;

            /******************************************************************
             * KEY_RECORD
             * 기존 기록 처리 후 INITIAL 화면 복귀
             ******************************************************************/
            case KEY_RECORD:
                gnlPreDailyTotal[0] = gnlDailyTotal;

                for(i = 0; i < 7; i++)
                {
                    Delay_ms(10);
                }

                Delay_ms(5000);

                return AGENT_KEY_EVENT_INITIAL;

            /******************************************************************
             * KEY_STATUS
             * 장비 상태 화면 표시 후 INITIAL 복귀
             ******************************************************************/
            case KEY_STATUS:
                sprintf(sT0, "DEVICE_TEL:%s", jangbi.cdma.telno);

                Reg_Tel[11] = 0;
                sprintf(sT0, "TEL=%s", Reg_Tel);

                sprintf(sT0,
                        "SMS_TIME=%c%c%c%c",
                        Sms_Time[0],
                        Sms_Time[1],
                        Sms_Time[2],
                        Sms_Time[3]);

                Current_Display = STATUS_DISPLAY;
                Delay_ms(6000);

                return AGENT_KEY_EVENT_INITIAL;

            default:
                break;
        }
    }

    return AGENT_KEY_EVENT_NONE;
} // Agent_KeyProcess_Task

/* Agent_CdmaTask() moved to app/agent_cdma_task.c (Phase 2). */
void Agent_CdmaTask(void);

/* Agent_MinuteEvent_Task() moved to app/agent_minute_event.c (Phase 2 pilot). */
void Agent_MinuteEvent_Task(void);

/* end of file */
