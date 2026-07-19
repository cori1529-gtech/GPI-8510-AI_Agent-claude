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
#include "stm32f10x_lib.h"
#include "stm32f10x_it.h"									 
#include "stm32f10x_iwdg.h"
#include "cortexm3_macro.h"
#include "hw_config.h"

#include "def.h"
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

#define	RS422ON		GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define	RS422OFF	GPIO_ResetBits(GPIOA, GPIO_Pin_4)


volatile int	giCdmaReturn;	//2015.8.17
char			gszCdmaReturn[100];	//2015.8.17

char str[100];



//<<<<<<<<<<<<<<<<<< local variable

signed long prev_adc1;
signed long gxlWeight;


u8 Once_flag=0;
u8 Once_flag_First=0;
u8 Once_flag_Second=0;
u8 Once_flag_Third=0;

u8 Error_Counter=0;

u8	baichul_start_flag = 0;
u8	baichul_end_flag = 0;		    /* determine done */

u8	ek_flag1 = 0;	/* second external key flag */

// Ver 1.30
u8	ek_flag1_HighCount = 0,	/* second external key flag */
	ek_flag1_LowCount  = 0;	/* second external key flag */

u8 byNewStart=0;
u8 stop_flag=0;

long	lPulseRef = 0;

s32    Start_Weight;
s32    Stop_Weight;
s32    TodayTotalref;
s32    TodayTotalref_result;

u8  sign_flag=0; // 1: minus   0: plus

long    lDay_Ref_Weight=0;
long	lPulseMaxHold = 0;
u8	lPulseMaxHoldCount = 0;

long    lRefWeigh = 0;
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

void welcome(void)
{
	usart1_transmit_string("Hello, World! GPI-8310\r\n");

	// STM32F105, 107 에서는 PLL설정이 약간 틀리다. 
	// 이후에 PLL설정 스트링을 수정해야 한다.
//	usart1_transmit_string("HSE(High Speed External clock signal) Enable\r\n");
//	usart1_transmit_string("HCLK(SYSCLK) = 8MHz, PCLK2 = HCLK, PCLK1 = HCLK/2, ADCCLK = PCLK2/2\r\n");
//	usart1_transmit_string("PLLCLK = 8MHz / 1 * 9 = 72MHz, USBCLK = PLLCLK / 2 = 48MHz\r\n\r\n");
}


int main(void)
{
	int	test_sended_time = 100;
	int	poweron_date_set_counter = 4; //(4-1)분

	char sT0[256];
	char ch;
	int i, iz, jz, kz;
	u8 Current_Display = NORMAL_DISPLAY;
	
	float	Real_Weight_Factor_temp;
	
__disable_irq();
    bsp_init_rcc();	     //hw.comfig_c , clock configuration
	bsp_init_interrupt();
__enable_irq();

	bsp_key_gpio_init(); //including relay 	
	buzzer_init();
//	register_timer_function(timer2ServiceFunction, timer2_event);
//	register_rtc_function(rtcServiceFunction, rtc_event);

	USARTx_Initial();

///USART1_puts("Cdma_Power_On();");
	Cdma_Power_On();
	Delay_ms(100);
	cdma_send_cmd("AT$LGTRESET", 11);	//2016.6.10 when main softreset
	Delay_ms(100);
	Cdma_Power_Off();
	Delay_ms(1000);

///USART1_puts("I2C_EE_Init();");
	I2C_EE_Init();	//EEPROM port 초기화

//	Realy_3_Off();//옵션보드  전원 off 
//	Realy_1_On();// 실제 relay 는 off
//	Realy_2_On();// 실제 relay 는 off 
//	Realy_3_On();// 실제 relay 는 off 
	
///USART1_puts("read_factors();");
	read_factors();  // FSMC를 disable 시키지 않으면 eeprom read, write 가 되지 않음 

	
	bsp_init_timer2();
    RTC_Configuration();  /* RTC Configuration */


	//	lcd70_draw_demo_bin(start_lamp.pixel_data,550,400,start_lamp.width+550,start_lamp.height+400);	// 호파그리기 
	
	//Realy_3_Off();
	//Adc_Power_On();

	//bsp_init_dma_usart1();
	
	// 10msec
	// wait_10ms(1);
	
	// User LED ON
	//	bsp_led_core_off(ledUserCore);
	//	bsp_led_core_on(ledUserCore);

	//<<<<<<<<<<<<<<<<<< watch dog
	// IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency dispersion)
	// Enable write access to IWDG_PR and IWDG_RLR registers
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_256);	//6.8ms // IWDG counter clock
	//IWDG_SetReload(1000);
	IWDG_SetReload(0xFFFF);	//26214.4 ms
	IWDG_ReloadCounter(); // Reload IWDG counter
//	IWDG_Enable(); // Enable IWDG (the LSI oscillator will be enabled by hardware)
	//<<<<<<<<<<<<<<<<<< watch dog >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	welcome();

	Cdma_Power_On();
	

/* 	
	usart1_transmit_string("\r\n*****************************************************************************\r\n");	
	usart1_transmit_string("Start USART1 service on mode interrupt.\r\n");
	usart1_transmit_string_format("BaudRate = %d, Databit = %dbit, StopBits = %d, Parity = no, FlowControl = none\r\n", 115200, 8, 1);	
	usart1_transmit_string("\r\n*****************************************************************************\r\n");		
//	UART4_puts("\r\n*****************************************************************************\r\n");		
*/

///USART1_puts("RS422ON;");
	RS422ON;
//	usart1_tx_proc();
	
	//Old_Pcb_Power_On(); // 구형 pcb power on

//USART1_puts("LCD INIT  begin");

	//USART1_puts("bsp_lcd70_init();");
	bsp_lcd70_init();
	
//USART1_puts("LCD INIT  end");

//USART1_puts("lcd70_clear_screen(WHITE);");
	lcd70_clear_screen(WHITE);

//USART1_puts("logo();");
	lcd_wr_reg(0x0028); //display off
	Delay_ms(30);
	logo();
	lcd_wr_reg(0x0029); //display on

//USART1_puts("Delay_ms(5000);");
	Delay_ms(2000);

//USART1_puts("RTC_Configuration();");
//	zigbee_setting();

	/*
	for(i=0;i<10;i++)
	{
		Relay_On();
		lcd70_show_string_mode(500, 135,"RELAY ON", 16, 0);
		
		Delay_ms(2000);
		Relay_Off();
		lcd70_show_string_mode(500, 135,"RELAY OFF", 16, 0);
		
		Delay_ms(2000);
	}
	*/

	/*
	for(i=0;i<1;i++)
	{
		Realy_1_Off();//옵션보드  전원 off \ 
		Lc1_led_On();
		Delay_ms(200);
		Realy_1_On();//옵션보드  전원 on
		Lc1_led_Off();
		Delay_ms(200);
		
		Realy_2_Off();//옵션보드  전원 off \ 
		Lc2_led_On();
		Delay_ms(200);
		Realy_2_On();//옵션보드  전원 on \ 
		Lc2_led_Off();
		Delay_ms(200);
		
		Realy_3_Off();//옵션보드  전원 off \ 
		Lc3_led_On();Lc4_led_On();
		Delay_ms(200);
		Realy_3_On();//옵션보드  전원 on \ 
		Lc3_led_Off();Lc4_led_Off();
		Delay_ms(200);
	}
	*/
	
//	USART1_puts("LCD ON  end");
	
	Adc_Counter=OPTION_POWER_COUNTER; // 20 을 기본으로 무존건 넣은다.=OPTION_POWER_COUNTER;// AD 모듈에서 답문이 없을때 체크하는 시간 -> 강제로 모듈 전원 ㅐㄹㄹ

	v_adc_org  = (long)((float)v_zero * gnfFactor);  // calibration시 영점값 기억 
	v_ei_multiply_factor = 10;
	Usart3_SMS_flag=0;
	
	mode_check();
  
lcd_wr_reg(0x0028); //display off

	lcd70_clear_screen(WHITE);
	// lcd70_draw_demo_bin(gimp_image.pixel_data,50,50,gimp_image.width+50,gimp_image.height+50);	// 호파그리기 
	lcd70_draw_demo_bin(stop_lamp.pixel_data,550,400,stop_lamp.width+550,stop_lamp.height+400);	// stop lamp 그리기
	/*
	lcd70_draw_rectangle(450, 130, 700, 230,BLUE);//오늘배출량 INNER BOX 그리기  
	lcd70_show_string_mode(500, 135,"TODAY OUTPUT", 16, 0);
	lcd70_draw_rectangle(450, 130, 700, 230,BLUE);//오늘배출량 OUTER BOX 그리기 
	*/
	lcd70_draw_rectangle(450, 130, 700, 230,BLUE);//오늘배출량 INNER BOX 그리기  
	lcd70_show_string_mode(500, 135,"TODAY OUTPUT", 16, 0);
	lcd70_draw_rectangle(445, 125, 705, 235,BLUE);//오늘배출량 OUTER BOX 그리기 
	
	lcd70_write_weight(500,150,gnlDailyTotal);
	
	lcd70_draw_rectangle(95, 130, 350, 230,BLUE);//현재무게 INNNER BOX
	lcd70_draw_rectangle(90, 125, 355, 235,RED);  //현재무게  OUTER BOX  // 2015년 1월6일
	lcd70_show_string_mode(150, 135,"CURRENT WEIGHT", 16, 0);
	
lcd_wr_reg(0x0029); //display on


//////////////////////// test here
//////////test here
//	gnhDay = EE_Write_val(ADR_gnhDay, gnhDay, 4);
/*
	for(iz=0; ; iz++)
	{
		sprintf(sT0, "%d", iz);
		lcd70_show_string_mode(10, 280, (const unsigned char *)sT0, 20, 0);
		
		Cdma_Power_Off();
//Relay_1_Off();
//		Relay_2_Off();
//		Relay_3_Off();
		Relay_4_Off();
//		Relay_5_Off();
		Delay_ms(1000);
		Cdma_Power_On();
//		Relay_1_On();
//		Relay_2_On();
//		Relay_3_On();
		Relay_4_On();
//		Relay_5_On();
		Delay_ms(1000);
	}
*/

/*****	
	lcd70_show_string_mode_24x24(0, 300,"0123456789-ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz", 16, 0);	//2015.4.28 test__only
	Delay_ms(5000);
	lcd70_show_string_mode_24x24(0, 350,"0123456789-ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz", 24, 0);	//2015.4.28 test__only
	Delay_ms(3000);
*****/
	
	/*            
	RS422ON;
	Get_Weight();
	Delay_ms(1000);
	Get_Weight();
	Delay_ms(1000);
	Get_Weight();
	Delay_ms(1000);
	*/
	
	/*
	Output_Relay_On();Delay_ms(2000); // 배출 릴레이 on  ,1번  Delay_ms(3000);	
	
	Output_Relay_Off(); Delay_ms(2000);// 배출 릴레이 off
	
	Error_Relay_On();Delay_ms(2000); // 에러  릴레이 on ,2번 
	Error_Relay_Off();Delay_ms(2000); // 에러  릴레이 off
	
	Pulse_Relay_On();Delay_ms(2000); // 펄스 릴레이 on ,3번 
	Pulse_Relay_Off();Delay_ms(2000); // 펄스  릴레이 off
	
	Cdma_Power_On(); Delay_ms(2000);//  cdma 전원   릴레이 on
	Cdma_Power_Off();Delay_ms(2000); // cdma power off 
	
	Junction_Power_On();Delay_ms(2000); //  ad 졍션  전원   릴레이 on
	Junction_Power_Off(); Delay_ms(2000);// ad 졍션  power off 
	Delay_ms(2000);
	
	Cdma_Power_On(); Delay_ms(2000);//  cdma 전원   릴레이 on
	Junction_Power_On();Delay_ms(2000); //  ad 졍션  전원   릴레이 on
	*/
	//  Real_Weight_Factor=9980; // 표시값이 실제값보다 많이 나오기 때문에  적은 fator 값이 
	
	
	/* 
	//1219
	while(1)
	{
		raw_data=Get_adc_from_module_20160629();  // 모듈로부터 a/d 값을 읽어온다.
	};
	*/
	/*	
	for(;;)
	{
	USART1_puts("adc 0");	
		Weight_flag = 0;
		rotation = 1;
		while(!Weight_flag) Get_adc_from_module_20160629();	
	}	
	*/
	Delay_ms(3000);

	if(gnlDisplay)
	{
		sprintf(sT0, "Get_Weight() before");
		lcd70_show_string_mode(10, 280, (const unsigned char *)sT0, 20, 0);
	}	

	
DEBUG_puts("Get_Weight()");

	RS422ON;
/*	
	Get_Weight();	Delay_ms(1000);
	Get_Weight();	Delay_ms(1000);
	Get_Weight();	Delay_ms(1000);
*/	
	Reg_Tel[11] = 0;
	if(gnlDisplay)
	{
		sprintf(sT0, "TEL=%s", Reg_Tel);
		//lcd70_show_string_mode(470, 230, (const unsigned char *)sT0, 20, 0);
		//lcd70_show_string_mode(200, 100,"Data sendig to user_First!!", 16, 0);
		
		lcd70_show_string_mode(300, 330, (const unsigned char *)sT0, 20, 0);
		lcd70_show_string_mode(300, 300, "Data sendig to user_First!!", 16, 0);
	}	

/****
	Data_Send_Sms(Reg_Tel); //등록된 번호로 배출 데이터 보낸다 
	Delay_ms(1000);
	Data_Send_Sms(gtech_tel);// 곽원호한테 보내기 
***/


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

	ZIGBEE_puts("ATE0");	//echo off


Error_Relay_On();


/**********************************************************************
sprintf(sT0, "tel=%s,rssi= %d",  jangbi.cdma.telno, jangbi.cdma.rssi);
send_sms("01091954072", sT0);
***********************************************************************/

DEBUG_puts("do  MAIN LOOP");

	if(gnlUseLTE)
	{
		for(iz=0;iz<3;iz++)
		{
			//add 2015.1.22
			cdma_send_cmd("AT$LGTDELRM=*", strlen("AT$LGTDELRM=*"));
			delay_ms(100);
		}
		
DEBUG_puts("get_cdma_info_WAIT(CDMA_GET_TELNO)");	

		for(i=0;i<100;i++)
		{	
			lcd70_write_weight(100,150, 100-i);//  Big font total weight displayin
			
			delay_ms(100);
			get_cdma_info_WAIT(CDMA_GET_TELNO, "TELNO");	//get_cdma_telno();
			get_cdma_info_WAIT(CDMA_GET_TIME, "TIME");	//get_cdma_time();
			get_cdma_info_WAIT(CDMA_GET_RSSI, "RSSI");	//get_cdma_rssi();
			

	//		if(gnlDisplay)
			{
				sprintf(sT0, "%d, tel=%s,rssi= %d", i, jangbi.cdma.telno, jangbi.cdma.rssi);
				lcd70_show_string_mode(10, 430, (const unsigned char *)sT0, 20, 0);
				
				sprintf(sT0, "Date=%04d-%02d-%02d Time=%02d:%02d:%02d", jangbi.cdma.tm.year, jangbi.cdma.tm.month, jangbi.cdma.tm.date, jangbi.cdma.tm.hour, jangbi.cdma.tm.min, jangbi.cdma.tm.sec);
				lcd70_show_string_mode(10, 460, (const unsigned char *)sT0, 20, 0);
			}
	/////	Delay_ms(3000);
		}
/***		
		if(gnlDisplay)
		{
			sprintf(sT0, "time_adjust() before");
			lcd70_show_string_mode(10, 280, (const unsigned char *)sT0, 20, 0);
		}
DEBUG_puts("time_adjust()");
		time_adjust(jangbi.cdma.tm.hour, jangbi.cdma.tm.min, jangbi.cdma.tm.sec);
		gnhToday=jangbi.cdma.tm.date; //전원을 켤때 현재 날짜를 입력하고 

		if(gnlDisplay)
		{
			sprintf(sT0, "time_adjust() after");
			lcd70_show_string_mode(10, 280, (const unsigned char *)sT0, 20, 0);
		}
****/		

		if(!gnlDisplay)
		{
			sprintf(sT0, "                                             ");
			lcd70_show_string_mode(10, 430, (const unsigned char *)sT0, 20, 0);
			sprintf(sT0, "                                             ");
			lcd70_show_string_mode(10, 460, (const unsigned char *)sT0, 20, 0);
		}
	}

	//close websms socket
	MultiSocketClose_LU202F( 2, 200);
	MultiSocketClose_LU202F( 2, 200);

	monitoring_timer_ms = 10;		//send status
	monitoring_set_timer_ms = 2000;	//send set value
	Ten_Mili_Counter=0;
	Sec_Mili_Counter=0;	
	giToday = -1;

	//초기 무게를 읽어 온다.
	for(iz=0; iz<20; iz++) get_weight(0);
	
	do
	{
		if(gnlDisplay)
		{
			sprintf(sT0, "FACTOR=%ld, giToday=%d, rtc_date=%d  ", (long)Real_Weight_Factor, giToday, rtc_date);
			lcd70_show_string_mode(300, 80, (const unsigned char *)sT0, 20, 0);
		
			//gi8310-0122	
			sprintf(sT0, "%08ld", gnlTick);
			lcd70_show_string_mode(10, 280, (const unsigned char *)sT0, 20, 0);
		}


		IWDG_ReloadCounter(); // Reload IWDG counter

		if((test_sended_time != jangbi.cdma.tm.hour) && poweron_flag)
		{
			poweron_flag = 0;
			sprintf(sT0, "%lu:tel=%s,rssi=%d,%04d-%02d-%02d %02d:%02d:%02d",
							gnlTick, jangbi.cdma.telno, jangbi.cdma.rssi,
							jangbi.cdma.tm.year, jangbi.cdma.tm.month, jangbi.cdma.tm.date,
							jangbi.cdma.tm.hour, jangbi.cdma.tm.min, jangbi.cdma.tm.sec);
			send_sms("01091954072", sT0);

			test_sended_time = jangbi.cdma.tm.hour;
		}

		RTC_sec_to_time_conversion();
		if(gnlDisplay) 
		{
			RTC_time_display();	
			sprintf(sT0, "gnuDay=%d, working_flag=%d, gnhToday=%d, jangbi.cdma.tm.min=%d, mCnt=%ld, INTERVAL=%ld", gnuDay, working_flag, giToday, jangbi.cdma.tm.min, Ten_Mili_Counter, GET_TIME_INTERFAL_TIME);
			lcd70_show_string_mode(10, 280, (const unsigned char *)sT0, 20, 0);
		}

		//Zigbee 입력
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
/*
		if(Usart4_Interrupt_flag)
		{
			Usart4_Interrupt_flag = 0;

			for(i=0;;i++)
			{
				iz = USART4_Rx_queue_get();
				if(iz == -1) break; //empty
				else
				{
					USART1_putchar(iz);
//					sT0[i] = iz;
//					gszSendingGigbee[i] = iz;
				}
			}		
*/
/**
			sT0[i] = NULL;
			CdmaSendQueue_puts(sT0);
*/
/*
			gszSendingGigbee[i] = NULL;
			giGigbeeSendingFlag = 1;
			giSendingGigbeeLength = strlen(gszSendingGigbee);
*/
//		}

	   	//지그비에서 온 데이터 저장
		for(i=0;;i++)
		{
			iz = USART4_Rx_queue_get();
			if(iz == -1) break; //empty
			else
			{
USART1_putchar(iz);
				CdmaSendQueue_tmp[CdmaSendQueue_index++] = iz;
				if(iz == 0x0a)
				{
					CdmaSendQueue_tmp[CdmaSendQueue_index] = NULL;
					if(gnlUseLTE) //lte 사용하는 메인이면 지그비로 받아서 서버로 전송
					{
						if( !strncmp( CdmaSendQueue_tmp, "OK", 2) && strlen(CdmaSendQueue_tmp)<5)
						{
						}
						if( !strncmp( &CdmaSendQueue_tmp[1], "DATE=", 5))
						{
						}
						else CdmaSendQueue_puts(CdmaSendQueue_tmp);
					}
					else
					{
						for(jz=0; jz<strlen(CdmaSendQueue_tmp); jz++)
						{
							if(CdmaSendQueue_tmp[jz] == '$')
							{
								sms_process(&CdmaSendQueue_tmp[jz]);
							}
						}
					}
				 	CdmaSendQueue_index = 0;

					break;
				}
			}		
		}

	   	//CDMA에서 온 데이터 저장
		for(i=0;;i++)
		{
			iz = RxCdmaQ_get();
			if(iz == -1) break; //empty
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



/**
		sT0[i] = NULL;
		CdmaSendQueue_puts(sT0);
*/
		if( !giGigbeeSendingFlag)
		{
			jz = CdmaSendQueue_gets();
			if(jz != -1)	//보낼 데이터가 있으면
			{
				giGigbeeSendingFlag = 1;
//				giGigbeeSendingRetry = MAX_giGigbeeSendingRetry;	//2014.8.25

///				strcpy(gszSendingGigbee, CdmaSendQueue[jz]);
				if(gnlUseLTE)
				{
///					sprintf( gszSendingGigbee,"%c|%s|%s", 0x02, jangbi.cdma.telno, CdmaSendQueue[jz]);
					gszSendingGigbee[0] = 0x02;
					gszSendingGigbee[1] = '|';
					for(iz=0; iz<11; iz++) if(jangbi.cdma.telno[iz]) gszSendingGigbee[iz+2] = jangbi.cdma.telno[iz];
					gszSendingGigbee[iz+2] = '|';
					kz = iz+3;
//					for(iz=0; iz<strlen(CdmaSendQueue[jz]); iz++) gszSendingGigbee[iz+kz] = CdmaSendQueue[jz][iz+2]; //stx 제외
					for(iz=0; iz<strlen(CdmaSendQueue[jz]); iz++) gszSendingGigbee[iz+kz] = CdmaSendQueue[jz][iz]; 
///					sprintf( gszSendingGigbee,"|012345678901|%s", CdmaSendQueue[jz]);
///				sprintf( gszSendingGigbee,"AT+UNICAST=0000,%s|%s", jangbi.cdma.telno, &CdmaSendQueue[jz][2]);

//sprintf(sT0, "==(%d)|%s|==", strlen(jangbi.cdma.telno), jangbi.cdma.telno);
//DEBUG_puts(sT0);
//DEBUG_puts(CdmaSendQueue[jz]);
//DEBUG_puts(gszSendingGigbee);
					
sprintf(sT0, "CdmaSendQueue_qhead=%d, CdmaSendQueue_qtail=%d, %s", CdmaSendQueue_qhead, CdmaSendQueue_qtail, CdmaSendQueue[jz]);
DEBUG_puts(sT0);
					
				}
				else
				{
					sprintf( gszSendingGigbee,"AT+UNICAST=0000,%s", &CdmaSendQueue[jz][2]);
				}
				giSendingGigbeeLength = strlen(gszSendingGigbee);

USART1_puts("::::::::::::");
//USART1_puts(&CdmaSendQueue[jz][1]);
DEBUG_puts(gszSendingGigbee);

			}
		}

		/////////////////////////////////////////////////////////
		//USART3 CDMA SMS 받았을 때
		if(Usart3_sms_received_flag)
		{
//Send_to_server(SMS_RECEIVED, RxBuffer2_sms);
			
			SMS_PROCESS();
		}

		INPUT_ONE   // 시작입력이 ON
		{
			ek_flag1_HighCount = (ek_flag1_HighCount +1) % 50;
			ek_flag1_LowCount  = 0;
			if(ek_flag1_HighCount>2)
			{
				ek_flag1=1;
//				Realy_1_On();
				Output_Relay_On();
				goto start_func;
			}
		}
		else 
		{
			ek_flag1_LowCount = (ek_flag1_LowCount +1) % 50;
			ek_flag1_HighCount  = 0;
			if( ek_flag1_LowCount>2)
			{
				//ek_flag1=0;
//				Realy_1_Off();
				Output_Relay_Off();
				if(ek_flag1)
				{
				byNewStart = gxhEndDelay;
				ek_flag1=0;
				stop_flag=1;	  //ver3.02
				}
			}
		}
 
		if(Sec_Mili_Counter>=DEF_1_SEC) // 1초에 한번씩 
		{
			Sec_Mili_Counter=0;	
			if(working_flag && stop_flag)	 //
			{
				if (byNewStart) --byNewStart;
			}	
		}

		if(working_flag && stop_flag) // 시작중에   정지신호가 들어왔으면  
		{
			if (!byNewStart)	//ver 1.34 if new start	  ver 3.02
			{
				//gnlDailyTotal += (lDay_Ref_Weight - gxlGrossWeight);//VER1.32	   critical 
				goto stop_func;
			}
		}

		//배출 정보 전송
		if(baichul_start_flag)	//배출시작 정보 전송
		{
			baichul_start_flag = 0;
			Send_to_server(SEND_BAICHUL, "BC-B");	//배출 시작시 정보 전송
		}
		else if(working_flag)	//배출중 정보 전송
		{
			if( !bachul_send_timer_ms)	//배출중 정보 전송 간격
			{
				bachul_send_timer_ms = WORKING_SEND_INTERVAL;
				Send_to_server(SEND_BAICHUL, "BC-J");	//배출중 정보 전송
			}
		}
		else if(baichul_end_flag) //배출완료 정보 전송
		{
			baichul_end_flag = 0;
			Send_to_server(SEND_BAICHUL, "BC-E");	//배출 완료 정보 전송
		}

		if(gnlDisplay)
		{
			//gi8310-0122	
			sprintf(sT0, "%08ld", monitoring_timer_ms);
			lcd70_show_string_mode(10, 280, (const unsigned char *)sT0, 20, 0);
		}
		
	
		
//DEBUG_puts("test_routine()");
		
		test_routine();//회사서버로 보내기 
		
		
		/*
			15분마다
			1. CDMA 날짜 읽어오기
			2. SMS 3개 보내기 설정시간에
			3. 날이 바뀌면, 배출날짜 계산 (D-N), 모뎀 소프트리셋
		*/	
		if(Ten_Mili_Counter>=GET_TIME_INTERFAL_TIME)	// 15분으로 수정 
		{
			Ten_Mili_Counter=0;

/*			
			if(gnlUseLTE)
			{
				get_cdma_time();
				sprintf(sT0, "Date=%04d-%02d-%02d Time=%02d:%02d:%02d", jangbi.cdma.tm.year, jangbi.cdma.tm.month, jangbi.cdma.tm.date, jangbi.cdma.tm.hour, jangbi.cdma.tm.min, jangbi.cdma.tm.sec);
			}
			else
			{
				sprintf(sT0, "Date=%04d-%02d-%02d Time=%02d:%02d:%02d", rtc_year, rtc_month, rtc_date, rtc_hour, rtc_min, rtc_sec);
			}
			if(gnlDisplay) lcd70_show_string_mode(0, 30, (const unsigned char *)sT0, 20, 0);
*/		
			if(gnlUseLTE)
			{
				// Buzzer_On(300);	//11
				// Data_Send_Sms(Reg_Tel); //등록된 번호로 배출 데이카 보내닥 
				
				//	 union { u32 dw; u16 u[2]; u8 c[4];} kwak;
				//hour.c[0]= Sms_Time[0];   //1
				//hour.c[1]= Sms_Time[1];   // 5
				// minute.c[0]= Sms_Time[2]; // 3
				//minute.c[1]= Sms_Time[3]; // 0
			
				////  if(hour.u[0]==jangbi.cdma.tm.hour && minute.u[0]==jangbi.cdma.tm.min)
				//if(Sms_Time[0]==(jangbi.cdma.tm.hour/10)+0x30 && Sms_Time[1]==(jangbi.cdma.tm.hour%10)+0x30 &&
				//	   Sms_Time[2]==(jangbi.cdma.tm.min/10)+0x30 && Sms_Time[3]==(jangbi.cdma.tm.min%10)+0x30)
				/*
				if(Sms_Time[0]==(jangbi.cdma.tm.hour/10)+0x30 && Sms_Time[1]==(jangbi.cdma.tm.hour%10)+0x30) // 등록되 시간 
				{	
					if(!Once_flag)
					{	
						lcd70_show_string_mode(200, 100,"Data sendig to user!!", 16, 0);
						Data_Send_Sms(Reg_Tel); //등록된 번호로 배출 데이카 보내닥 
						Delay_ms(1000);
						Data_Send_Sms(gtech_tel);// 곽원호한테 보내기 
						Once_flag=1;
					}
				}
				*/

				if((Sms_Time_First[0]/10+0x30)==(jangbi.cdma.tm.hour/10)+0x30 && (Sms_Time_First[0]%10+0x30)==(jangbi.cdma.tm.hour%10)+0x30) // 등록되 시간 
				{	
					if(!Once_flag_First)
					{	
						lcd70_show_string_mode(200, 100,"Data sendig to user_First!!", 16, 0);
						
						Data_Send_Sms(Reg_Tel); //등록된 번호로 배출 데이카 보내닥 
						Delay_ms(1000);
						Data_Send_Sms(gtech_tel);// 곽원호한테 보내기 
						Once_flag_First=1;
						//Once_flag=1;
					}
				}
		
				if((Sms_Time_Second[0]/10+0x30)==(jangbi.cdma.tm.hour/10)+0x30 && (Sms_Time_Second[0]%10+0x30)==(jangbi.cdma.tm.hour%10)+0x30) // 등록되 시간 
				{	
					if(!Once_flag_Second)
					{	
						lcd70_show_string_mode(200, 100,"Data sendig to user_Second!!", 16, 0);
						Data_Send_Sms(Reg_Tel); //등록된 번호로 배출 데이카 보내닥 
						Delay_ms(1000);
						Data_Send_Sms(gtech_tel);// 곽원호한테 보내기 
						Once_flag_Second=1;
						//Once_flag=1;
					}
				}
				
				if((Sms_Time_Third[0]/10+0x30)==(jangbi.cdma.tm.hour/10)+0x30 && (Sms_Time_Third[0]%10+0x30)==(jangbi.cdma.tm.hour%10)+0x30) // 등록되 시간 
				{	
					if(!Once_flag_Third)
					{	
						lcd70_show_string_mode(200, 100,"Data sendig to user_Third!!", 16, 0);
						Data_Send_Sms(Reg_Tel); //등록된 번호로 배출 데이카 보내닥 
						Delay_ms(1000);
						Data_Send_Sms(gtech_tel);// 곽원호한테 보내기 
						Once_flag_Third=1;
						//Once_flag=1;
					}
				}
			} //if(gnlUseLTE)
			
//STST20150608	
//			if (gnhToday != jangbi.cdma.tm.date && !working_flag) //일주일치 순환,하루가 바뀌었다. 
//			if (gnhToday != jangbi.cdma.tm.min && !working_flag) //일주일치 순환,하루가 바뀌었다. 
//			if (gnhToday != jangbi.cdma.tm.hour && !working_flag) //일주일치 순환,하루가 바뀌었다. 
//			if (giToday != rtc_hour && !working_flag) //일주일치 순환,하루가 바뀌었다. 
//			if (gnhToday != rtc_min/10 && !working_flag) //일주일치 순환,하루가 바뀌었다. 
			if (giToday != rtc_date && !working_flag) //일주일치 순환,하루가 바뀌었다. 
			{
//STST20150608	
//				gnhToday = jangbi.cdma.tm.date;
///				gnhToday = jangbi.cdma.tm.min;
//				gnhToday = jangbi.cdma.tm.hour;
//				giToday = rtc_hour;
//				gnhToday = rtc_min/10;
				giToday = rtc_date;

				Once_flag=0;
				Once_flag_First=0;
				Once_flag_Second=0;
				Once_flag_Third=0;

				for (i=6; i; i--) gnlPreDailyTotal[i] = gnlPreDailyTotal[i-1];
				gnlPreDailyTotal[0] = gnlDailyTotal;

				gnuDay++;	// increase day
				gnlDailyTotal = 0;
				save_DailyTotals();

				//LTE MODEM power on/off
				modem_power_reset();

				lcd70_write_weight(500,150,gnlDailyTotal);
				//			iPulseCounter = 0;
///				monitoring_timer_ms = 60000;
			}
			
			//Buzzer_On(300);	//11
			//Data_Send_Sms(Reg_Tel); //등록된 번호로 배출 데이카 보내닥 
		} //if(Ten_Mili_Counter>=GET_TIME_INTERFAL_TIME)	// 15분으로 수정  //15분마다 시간읽어서 
	

		//무게를 읽어보기 
		get_weight(1);

		if (working_flag)	// 0417
		{
			lPulseMaxHoldCount++;
			if (10 < lPulseMaxHoldCount)	// 250 msec *10= 2.5초 마다  적산 
			{
				lPulseMaxHoldCount = 0;	
				if (gxlGrossWeight < lPulseRef)
				{
					if((lPulseRef-gxlGrossWeight)<=100)	// 연산한값이 100 kg 이하일때만 배출무게로 받아들임  
					{							 		// 100 kg 이상되면  노이즈로 판단하고 
						gnlDailyTotal += (lPulseRef - gxlGrossWeight);//VER1.32	   critical 
						lPulseRef = gxlGrossWeight;
						lPulseMaxHold = gxlGrossWeight;
						lPulseMaxHoldCount = 0;
						lcd70_write_weight(500,150,gnlDailyTotal);// 오늘의 배출량 보내기 
					}
				}
			}
		}//working flag	


		if (keypush())
		{
			ch=key_disp[KEYCODE];   //1127
			
			switch(ch)
			{
                case KEY_1:	//1127
					/*
					for(i=0;i<10;i++)
					raw_data=Get_adc_from_module();  // 모듈로부터 a/d 값을 읽어온다.
					*/
					
					sprintf(sT0, "v_zero=%6lu  comp=%6lu",(long)v_zero,(long)gplCompensationWeight);
					lcd70_show_string_mode(200, 30, (const unsigned char *)sT0, 20, 0);

					while(!Weight_flag)
					{
						raw_data=Get_adc_from_module_20160629(); 	// 
					};

					v_adc_org  = (long)((float)raw_data * gnfFactor);  // calibration시 영점값 기억 
					gplCompensationWeight=0;
                    v_zero=raw_data;

/* 2016.4.20
                    Initiaize_eeprom();
                    FSMC_DISABLE;
                    kwak.dw=v_zero;

	                I2C_EE_BufferWrite(kwak.c, ADR_v_zero, 4);
	                I2C_EE_BufferRead(kwak.c, ADR_v_zero, 4);
					kwak.dw=gplCompensationWeight;
					I2C_EE_BufferWrite(kwak.c, ADR_gplCompensationWeight, 4);
	                I2C_EE_BufferRead(kwak.c, ADR_gplCompensationWeight, 4);
					
					FSMC_ENABLE;
*/
					v_zero = EE_Write_val(ADR_v_zero, v_zero, 4);
					gplCompensationWeight = EE_Write_val(ADR_gplCompensationWeight, gplCompensationWeight, 4);

					/*
					sprintf(sT0, "v_zerom=%6lu  comp=%6lu",(long)v_zero,(long)gplCompensationWeight);
					lcd70_show_string_mode(200, 70, (const unsigned char *)sT0, 20, 0);
					
					
					Initiaize_eeprom();
					FSMC_DISABLE;	 //1126
					kwak.dw=v_zero;
					I2C_EE_BufferWrite(kwak.c, v_zero, 4);
					I2C_EE_BufferRead( kwak.c,v_zero , 4);
					v_zero=kwak.dw;
					
					
					kwak.dw=gplCompensationWeight;
					I2C_EE_BufferWrite(kwak.c, ADR_gplCompensationWeight, 4);
					I2C_EE_BufferRead( kwak.c,ADR_gplCompensationWeight , 4);
					gplCompensationWeight=kwak.dw;
					FSMC_ENABLE;
					
					sprintf(sT0, "v_zeroa=%6lu  compa=%6lu",(long)v_zero,(long)gplCompensationWeight);
					lcd70_show_string_mode(200, 100, (const unsigned char *)sT0, 20, 0);
					*/
					
					/*
					while(1)
					{
						if (keypush())
						{
							ch=key_disp[KEYCODE];   //1127
							
							if(ch==KEY_ENTER)
							{
								v_adc_org  = (long)((float)raw_data * gnfFactor);  // calibration시 영점값 기억 
								gplCompensationWeight=0;
								v_zero=raw_data;
								
								Initiaize_eeprom();
								FSMC_DISABLE;	 //1126
								kwak.dw=v_zero;
								I2C_EE_BufferWrite(kwak.c, v_zero, 4);
								I2C_EE_BufferRead( kwak.c,v_zero , 4);
								
								kwak.dw=gplCompensationWeight;
								I2C_EE_BufferWrite(kwak.c, ADR_gplCompensationWeight, 4);
								I2C_EE_BufferRead( kwak.c,ADR_gplCompensationWeight , 4);
								//read_factors();
								FSMC_ENABLE;
								v_adc1_buf = (long)(gnfFactor * raw_data);
								diff1 = v_adc1_buf - v_adc_org;
								diff2 = v_adc1_buf - prev_adc1;	// for stable 
								gxlGrossWeight = ((diff1 + 10) / v_ei_multiply_factor) * v_division;
								gxlGrossWeight=gxlGrossWeight+gplCompensationWeight;
							}
							else
							break;
						}
					};
					*/
					break;

				case KEY_WEIGHT: //현재무게키 
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
//					FSMC_ENABLE;
					///bsp_lcd70_init();
					/*
					lcd70_clear_screen(WHITE);
					lcd70_draw_rectangle(450, 130, 700, 230,BLUE);//오늘배출량 박스 그리기 
					lcd70_write_weight(500,150,gnlDailyTotal);
					*/
					lcd70_clear_screen(WHITE);
					
					lcd70_draw_rectangle(450, 130, 700, 230,BLUE);//오늘배출량 INNER BOX 그리기  
					lcd70_show_string_mode(500, 135,"TODAY OUTPUT", 16, 0);
					lcd70_draw_rectangle(445, 125, 705, 235,BLUE);//오늘배출량 OUTER BOX 그리기 
					
					// lcd70_write_weight(500,150,gnlDailyTotal);
					/*
					lcd70_draw_rectangle(100, 130, 350, 230,BLUE);//현재무게 INNNER BOX
					lcd70_draw_rectangle(95, 125, 355, 235,RED);  //현재무게  OUTER BOX  // 2015년 1월6일
					lcd70_show_string_mode(150, 135,"CURRENT WEIGHT", 16, 0);
					*/
					lcd70_draw_rectangle(95, 130, 350, 230,BLUE);//현재무게 INNNER BOX
					lcd70_draw_rectangle(90, 125, 355, 235,RED);  //현재무게  OUTER BOX  // 2015년 1월6일
					lcd70_show_string_mode(150, 135,"CURRENT WEIGHT", 16, 0);
					
					if(working_flag )
						lcd70_draw_demo_bin(start_lamp.pixel_data,550,400,start_lamp.width+550,start_lamp.height+400);// RED 표시부 그리기 
					else
						lcd70_draw_demo_bin(stop_lamp.pixel_data,550,400,stop_lamp.width+550,stop_lamp.height+400);// RED 표시부 그리기 
					
					/*
					lcd70_draw_rectangle(400, 50, 700, 100,BLUE);
					lcd70_draw_rectangle(400, 130, 700, 180,BLUE);
					lcd70_draw_rectangle(400, 210, 700, 260,BLUE);
					lcd70_draw_rectangle(400, 290, 700, 340,BLUE);
					*/
					//	sprintf(str, "CODE=%2d",(int)KEYCODE);
					// lcd70_show_string_mode(13, 74, str, 16, 0);
					// lcd70_draw_demo_bin(start_lamp.pixel_data,700,350,start_lamp.width+700,start_lamp.height+350);	// 호파그리기 
					//	lcd70_draw_demo_bin(start_lamp.pixel_data,550,400,start_lamp.width+550,start_lamp.height+400);	// 호파그리기 
					//	lcd70_draw_demo_bin(start_lamp.pixel_data,550,400,start_lamp.width+550,start_lamp.height+400);	// 호파그리기 
					
					Current_Display=NORMAL_DISPLAY;
					break;

				case KEY_RECORD:		  //1126	   1127
					lcd70_clear_screen(WHITE);
					//gnlPreDailyTotal[0]=gnlDailyTotal;
					/*
					lcd43_show_big_num(250,40,0);
					lcd43_show_big_num(280,40,':');
					
					
					lcd70_write_weight(300,40,gnlDailyTotal);
					*/
					gnlPreDailyTotal[0]=gnlDailyTotal;
					for(i=0;i<7;i++)
					{ 
						lcd43_show_big_num(250,40+(i*60),i);
						lcd43_show_big_num(280,40+(i*60),':');
						lcd70_write_weight(300,40+(i*60),gnlPreDailyTotal[i]);// 오늘의 배출량 보내기 
					}
					Delay_ms(5000);

					goto 	INITIAL;

//					break;

start_func:
					stop_flag = 0;
					
					if (!working_flag)
					{
						if (!byNewStart)	//ver 1.34 if new start
						{
							baichul_start_flag = 1;

							lPulseRef = gxlGrossWeight;
							lRefWeigh = gxlGrossWeight;
							lDay_Ref_Weight=gxlGrossWeight; // 2011 12/24  ver 3.04 		critical
							Start_Weight=gxlGrossWeight; //  
							TodayTotalref= gnlDailyTotal; // 이제까지의 오늘 배출량을 대입하고  
	if(gnlDisplay) 
	{
							sprintf(str, "gxlGross=%6lu ",(long)gxlGrossWeight);
							lcd70_show_string_mode(300, 300,str, 16, 0);
							
							sprintf(str, "pulse_weight=%6lu ",(long)lPulseRef);
							lcd70_show_string_mode(300, 330,str, 16, 0);
							
							sprintf(str, "ref_weight=%6lu ",(long)lRefWeigh);
							lcd70_show_string_mode(300, 360,str, 16, 0);
							
							sprintf(str, "start_weight=%6lu ",(long)Start_Weight);
							lcd70_show_string_mode(300, 390,str, 16, 0);
	}
						}
						lcd70_draw_demo_bin(start_lamp.pixel_data,550,400,start_lamp.width+550,start_lamp.height+400);// RED 표시부 그리기 

						working_flag = 1;
						//bachul_send_timer_ms = 10;	//시작시 배출 정보 전송	2016.4.5
						bachul_send_timer_ms = WORKING_SEND_INTERVAL;	//배출중  정보 전송 시간 초기화

						//bErrorCheck = 1;
						//bStartOn = TRUE;
						//bReStart = FALSE;
					} // !working_flag
					break;
					
stop_func:   //1224
					working_flag = FALSE;
					stop_flag = 1;  // 정지 
					baichul_start_flag = 0;	//시작 플래그
					Stop_Weight=Start_Weight-gxlGrossWeight; //start시의 무게와 stop 시의 배출량계산 
					
					
/////STST20160608
//Stop_Weight = 100;			
					
					
					
					
					//	TodayTotalref_result=gnlDailyTotal-TodayTotalref; //한꺼번에 뺀값 
					TodayTotalref_result=TodayTotalref+Stop_Weight;	 // stop func 
					
					// if(abs(TodayTotalref_result-gnlDailyTotal)<=5*v_division) // 실시간 뺀량과 한꺼번에 뺀량의 
					          // 절대값이 5 눈금 이하로 나면  
					{	
						// stop 에서 한꺼번에 뺀값이 참값 
						gnlDailyTotal = EE_Write_val(ADR_gnlDailyTotal, TodayTotalref_result, 4);
						
						lcd70_write_weight(500,150,gnlDailyTotal);// 오늘의 배출량 보내기 
					}

					if(gnlDisplay)
					{
						lcd70_write_weight(500,300,gnlDailyTotal);// 오늘의 배출량 보내기 
						sprintf(str, "TodayTotalref=%6lu ",(long)TodayTotalref_result);
						lcd70_show_string_mode(500, 370,str, 16, 0);
					}
					
					//NoiseCount = 0;
					//NoiseFactor = 0;
					//NoiseWeightGo = 0;
					//NoiseWeight = 0;
					//NoiseLast = 0;
					//	signal_flag=0;
					//						vputs("333333");delay(100);
					// ver 1.32				nv_wei_No++;	// ver 1.20
					// VER 1.32				gnlPreOutput = lOutput; //ver 1.30
					//						relay_off(END_R);
					//relay_off(LOSS_R); //  LAMPUP = 1;
					//	LcdUpdate();
					//	bErrorCheck = 0;
					//	ErrorMinute = 60;	// ver 1.28
					//ErrorSecond = 60;	// ver 1.28
					lcd70_draw_demo_bin(stop_lamp.pixel_data,550,400,stop_lamp.width+550,stop_lamp.height+400);	//  GREEN LED
					baichul_end_flag = 1;
					//	bStartOn = FALSE;
					//	delay(1300);
					break;

				case KEY_STATUS: //장비상태 
					lcd70_clear_screen(WHITE);
					// bsp_lcd70_init();
					lcd70_clear_screen(WHITE);
					lcd70_draw_rectangle(400, 50, 700, 100,BLUE);
					lcd70_draw_rectangle(400, 130, 700, 180,BLUE);
					lcd70_draw_rectangle(400, 210, 700, 260,BLUE);
					lcd70_draw_rectangle(400, 290, 700, 340,BLUE);
					lcd70_draw_rectangle(400, 370, 700, 420,BLUE);
					
					//	sprintf(str, "CODE=%2d",(int)KEYCODE);
					// lcd70_show_string_mode(13, 74, str, 16, 0);
					
					//sprintf(str, "L/C1 WEIGHT=%06ld kg",One_Weight);
					sprintf(sT0, "DEVICE_TEL:%s", jangbi.cdma.telno);
//					lcd70_show_string_mode(470, 70,"DEVICE_TEL:012-2203-1337", 16, 0);
					lcd70_show_string_mode(470, 70, sT0, 16, 0);
					
					//sprintf(str, "L/C2 WEIGHT=%06ld kg",Two_Weight);
					lcd70_show_string_mode(470, 150, "ANNTENNA=GOOD", 16, 0);
					
					//sprintf(str, "L/C3 WEIGHT=%06ld kg",Three_Weight);
					//lcd70_show_string_mode(470, 230, "USER1:0103443-1529", 16, 0);
					Reg_Tel[11] = 0;
					sprintf(sT0, "TEL=%s", Reg_Tel);
					lcd70_show_string_mode(470, 230, (const unsigned char *)sT0, 20, 0);
					
					sprintf(sT0, "SMS_TIME=%c%c%c%c", Sms_Time[0], Sms_Time[1], Sms_Time[2], Sms_Time[3] );
					lcd70_show_string_mode(470, 310, (const unsigned char *)sT0, 20, 0);
					
					
					//sprintf(str, "L/C4 WEIGHT=%06ld kg",Four_Weight);
					//lcd70_show_string_mode(470, 310, "USER2:010-9195-4072", 16, 0);
					
					lcd70_show_string_mode(470, 390, "USER3:010-2004-0580", 16, 0);
					
					Current_Display=STATUS_DISPLAY;
					Delay_ms(6000);
					goto INITIAL;					
					
//					break;

				default:
					break;
			} //switch(ch)
		} //if (keypush())
	
		/*
		raw_data=Get_adc_from_module();  // 모듈로부터 a/d 값을 읽어온다.
		if(Weight_flag)// 모듈로 부터 무게를 정상적으로 받았으면  현재무게를 표시한다. 
		{
			v_adc1_buf = (long)(gnfFactor * raw_data);
			diff1 = v_adc1_buf - v_adc_org;
			diff2 = v_adc1_buf - prev_adc1;	// for stable 
			gxlGrossWeight = ((diff1 + 10) / v_ei_multiply_factor) * v_division;
			gxlGrossWeight=gxlGrossWeight+gplCompensationWeight;
			lcd70_write_weight(100,150,gxlGrossWeight);// total weight displayin
			Weight_flag=0;
		}
		*/
	
		/*
		INPUT_ONE   // 시작입력이 들어오면  
		lcd70_draw_demo_bin(start_lamp.pixel_data,550,400,start_lamp.width+550,start_lamp.height+400);// RED 표시부 그리기 
		else
		lcd70_draw_demo_bin(stop_lamp.pixel_data,550,400,stop_lamp.width+550,stop_lamp.height+400);	//  GREEN LED
		*/		

		
		Adc_Power_Off_On();  // 옵션보드로 부터 일정시간 응답이 없으면 옵션부 전원을 강제로  ON/OFF 한다
		// 200 번 연속으로 오지 않으면 전원을 다시 on/off 시킨다. 

		//매분마다 실행하는 기능
		if(gbMinEvent)
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
				giToday = rtc_date;
			}
			
		}
		
	} while(1);
} /* main */


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
		   
//DEBUG_puts("SMS RECEIVED");
//DEBUG_puts(RxBuffer2_sms);

	if(RxBuffer2_sms[RxCounter2_sms-28]=='R' &&  RxBuffer2_sms[RxCounter2_sms-27]=='E' && RxBuffer2_sms[RxCounter2_sms-26]=='S')
	{
		lcd70_show_string_mode(300, 50,"MODEM RESET !!!", 20, 0);
		cdma_send_cmd("AT$LGTRESET", 11);	//2014.10.27
	}
	else if(RxBuffer2_sms[RxCounter2_sms-28]=='I' &&  RxBuffer2_sms[RxCounter2_sms-27]=='A' && RxBuffer2_sms[RxCounter2_sms-26]=='P')
	{
		lcd70_show_string_mode(300, 50,"FIRMWARE DOWNLOAD  !!!", 20, 0);
		goto_iap(TMS_SERVER_IP, TMS_SERVER_PORT); 
		//cdma_send_cmd("AT$LGTRESET", 11);	//2014.10.27
	}
	else if(RxBuffer2_sms[RxCounter2_sms-28]=='R' &&  RxBuffer2_sms[RxCounter2_sms-27]=='E' && RxBuffer2_sms[RxCounter2_sms-26]=='G')
	{
		lcd70_show_string_mode(300, 50,"TELEPHONE_REGISTRATION", 20, 0);
		
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

		sprintf(sT0, "TEL=%s", Reg_Tel);
		lcd70_show_string_mode(300, 80, (const unsigned char *)sT0, 20, 0);
	}
    else if(RxBuffer2_sms[RxCounter2_sms-28]=='D' &&  RxBuffer2_sms[RxCounter2_sms-27]=='E' && RxBuffer2_sms[RxCounter2_sms-26]=='L')
	{
		lcd70_show_string_mode(300, 50,"TELEPHONE_DELETION !!!", 20, 0);
	}
	else if(RxBuffer2_sms[RxCounter2_sms-28]=='A' &&  RxBuffer2_sms[RxCounter2_sms-27]=='U' && RxBuffer2_sms[RxCounter2_sms-26]=='T')
//	else if(RxBuffer2_sms[1]=='A' &&  RxBuffer2_sms[2]=='U' && RxBuffer2_sms[3]=='T')
	{
		lcd70_show_string_mode(300, 50,"DATA_SENDING_TIME  !!!", 20, 0);
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

		Sms_Time[0] = temp.c[0] = RxBuffer2_sms[RxCounter2_sms-8];
		Sms_Time[1] = temp.c[1] = RxBuffer2_sms[RxCounter2_sms-7];
		Sms_Time[2] = temp.c[2] = RxBuffer2_sms[RxCounter2_sms-6];
		Sms_Time[3] = temp.c[3] = RxBuffer2_sms[RxCounter2_sms-5];
		
		EE_Write_val(ADR_gnlSms_Send_Time, temp.dw, 4);

//		Delay_ms(1);
//  		FSMC_ENABLE; 

		sprintf(sT0, "SMS TIME=%lu.%lu.%c%c%c%c", temp.dw, (u32 *)Sms_Time, Sms_Time[0], Sms_Time[1], Sms_Time[2], Sms_Time[3]);
		lcd70_show_string_mode(300, 20, (const unsigned char *)sT0, 20, 0);
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
	else if( !strncmp(RxBuffer2_sms, "#RSTTIME=", 9))
	{
		sms_process(RxBuffer2_sms);
	}
	else if( !strncmp(RxBuffer2_sms, "#IAP=", 5))
	{
		lcd70_show_string_mode(300, 50,"FIRMWARE DOWNLOAD  !!!", 20, 0);
		goto_iap(TMS_SERVER_IP, TMS_SERVER_PORT); 
	}
	else if(RxBuffer2_sms[RxCounter2_sms-28]=='M' &&  RxBuffer2_sms[RxCounter2_sms-27]=='A' && RxBuffer2_sms[RxCounter2_sms-26]=='N')
	{
		lcd70_show_string_mode(300, 50,"MANUAL DATA SENDING  !!!", 20, 0);	//1127
		
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
		lcd70_show_string_mode(300, 50,"ZIGBEE !!!", 20, 0);
		
		sprintf(sT0, "AT+UC=%s", &RxBuffer2_sms[8]);
		ZIGBEE_puts(sT0);
	}

//	RxBuffer2_sms[RxCounter2_sms-28]=0XFF;
//	RxBuffer2_sms[RxCounter2_sms-27]=0XFF;
//	RxBuffer2_sms[RxCounter2_sms-26]=0XFF;

	for(iz=0; iz<MAX_RXBUFFER2_SMS; iz++) RxBuffer2_sms[iz] = 0;
	
sprintf(sT0, "Usart3_sms_received_flag=%d, ", Usart3_sms_received_flag);
lcd70_show_string_mode(0, 20,sT0, 16, 0);	//1127
	
	Usart3_sms_received_flag=0;
	RxCounter2_sms = 0;
	Delay_ms(1000);
sprintf(sT0, "Usart3_sms_received_flag=%d, ", Usart3_sms_received_flag);
lcd70_show_string_mode(300, 20,sT0, 16, 0);	//1127
	
	lcd70_show_string_mode(300, 50,"                         ", 16, 0);	//1127
}

//lte, zigbee common
void sms_process(char *msg)
{
	signed long	l;
	unsigned long ul;
	char sT0[50];
	
   	// 실제무게 FACTOR 값 :: #RFAC=12345@, $RFAC=12345@
	if( !strncmp( &msg[1], "RFAC=", 5))	//stx 제외
	{
		lcd70_show_string_mode(300, 50,"REAL_WEIGHT_FACTOR  !!!", 20, 0);
		
		msg[11] = 0;
		Real_Weight_Factor = EE_Write_val(ADR_Real_Weight_Factor, atol(&msg[6]), 4);
		
		sprintf(sT0, "FACTOR=%ld  ", (long)Real_Weight_Factor);
		lcd70_show_string_mode(300, 80, (const unsigned char *)sT0, 20, 0);
		
		Send_to_server(SEND_SETVALUE, "set");
	}
   	// 실제무게 값 :: #RWEIGHT=12345@, $RWEIGHT=12345@
	else if( !strncmp( &msg[1], "RWEIGHT=", 8))	//stx 제외
	{
		lcd70_show_string_mode(300, 50,"REAL_WEIGHT !!!", 20, 0);
		msg[14] = 0;
		gplCompensationWeight = calcCompensationWeight(atol(&msg[9]));
		gplCompensationWeight = EE_Write_val(ADR_Real_Weight_Factor, gplCompensationWeight, 4);
	
		sprintf(sT0, "REAL_WEIGHT=%ld  ", (long)gplCompensationWeight);
		lcd70_show_string_mode(300, 80, (const unsigned char *)sT0, 20, 0);
	
		Send_to_server(SEND_SETVALUE, "set");
		USART1_puts(sT0);
	}
   	// 영점설정 :: #RZERO=12345@
	else if( !strncmp(RxBuffer2_sms, "#RZERO=", 6))
	{
//		sms_process(RxBuffer2_sms);
	}
   	// 상태정보 전송간격 설정(분) :: #RSTTIME=12345@
	else if( !strncmp(RxBuffer2_sms, "#RSTTIME=", 9))
	{
		l = atol(&msg[10]);
		if(( MIN_gxwStatusSendInterval <= gxwStatusSendInterval) && (gxwStatusSendInterval <= MAX_gxwStatusSendInterval))
		{
			gxwStatusSendInterval = EE_Write_val(ADR_gxwStatusSendInterval, gxwStatusSendInterval, 4);
		}
	}
	//날짜, 시간 동기화
	else if( !strncmp(&msg[1], "DATE=", 5) && msg[20] == '@')
	{
		RTC_date_to_sec( &msg[6]);
	}

USART1_puts(msg);	
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
   #define  GAP   30
   #define  START 100
	 
   lcd70_clear_screen(WHITE);
   lcd70_show_string_mode_24x24(200, START, " KEY1      : TEST    ", 16, 0);
   lcd70_show_string_mode_24x24(200, START+(1*GAP), " KEY3      : CAL     ", 16, 0);
   lcd70_show_string_mode_24x24(200, START+(2*GAP), " KEY_ENTER : SET     ", 16, 0);
   lcd70_show_string_mode_24x24(200, START+(3*GAP), " KEY_CLEAR : RETURN  ", 16, 0);
/*
   lcd70_clear_screen(WHITE);
   lcd70_show_string_mode(200, START,         " KEY1       :  TEST    ", 16, 0);
   lcd70_show_string_mode(200, START+(1*GAP) ," KEY3       :  CAL     ", 16, 0);
   lcd70_show_string_mode(200, START+(2*GAP), " KEY_ENTER  :  SET     ", 16, 0);
  
   lcd70_show_string_mode(200, START+(3*GAP), " KEY_CLEAR  :  RETURN  ", 16, 0);
*/	 
Delay_ms(2000);	 
	 
  while(!exit_flag)
  {
  if (keypush())
   {
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
			   exit_flag=1;
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
	   } // of switch 
   } // of keypush()
  }
 }


void Data_Clear(void)
{
	long password;
	int i;
	union { u32 dw; u16 u[2]; u8 c[4];} temp;
	
	lcd70_clear_screen(WHITE);

	lcd70_show_string_mode(100, 10," DATA _CLEAR ", 16, 0);
	lcd70_show_string_mode(100, 40," INPUT PASSWORD!!! ", 16, 0);

	sprintf(str, "gnlDailyTotal=%6lu ",(long)gnlDailyTotal);
	lcd70_show_string_mode(100, 110,str, 16, 0);
	for(i=0;i<7;i++)
	{
		sprintf(str, "%d  gnlPre=%6lu ",(int)i,(long)gnlPreDailyTotal[i]);
		lcd70_show_string_mode(100, 140+(i*30),str, 16, 0);
	}

	sprintf(str, "day=%6lu ",(long)gnuDay);
	lcd70_show_string_mode(100, 350,str, 16, 0);
	password=Get_Value(100,80);

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

		save_DailyTotals();

		Delay_ms(200);	


		FSMC_ENABLE;


		sprintf(str, "gnlDailyTotal=%6lu ",(long)gnlDailyTotal);
		lcd70_show_string_mode(100, 110,str, 16, 0);
		for(i=0;i<7;i++)
		{
			sprintf(str, "%d  gnlPre=%6lu ",(int)i,(long)gnlPreDailyTotal[i]);
			lcd70_show_string_mode(100, 140+(i*30),str, 16, 0);
		}

		sprintf(str, "day=%6lu ",(long)gnuDay);
		lcd70_show_string_mode(100, 350,str, 16, 0);
	}
}


void Adc_Power_Off_On(void)
{
	if( !Adc_Counter)  // 옵션보드로 부터 일정시간 응답이 없으면 옵션부 전원을 강제로  ON/OFF 한다 
	{
		Adc_Power_Off();
		Delay_ms(1000);
		Adc_Power_On();
		Adc_Counter=OPTION_POWER_COUNTER;
	}
}


void logo(void)
{
	#define Y_AXIS_SPACE   50
	char str[20];

POINT_COLOR = BLACK;
BACK_COLOR = WHITE;

	lcd70_show_string_mode_24x24(200, 100+(Y_AXIS_SPACE*1),"MODEL:GPI-8310", 16, 0);
	sprintf(str, "VERSION:%s", VERSION);
	lcd70_show_string_mode_24x24(200, 100+(Y_AXIS_SPACE*2), str, 16, 0);
	lcd70_show_string_mode_24x24(200, 100+(Y_AXIS_SPACE*3),"G-TECH INTERNATIONAL CO., ", 16, 0);
}

void mode_check(void)
{
	char ch;
	
	if (keypush())
	{
		ch=key_disp[KEYCODE];

		switch(ch)
		{
			case  KEY_1:
				test_mode();
				break;
			case  KEY_3:
				cal_mode();
				break;
			case  KEY_ENTER:
				set_mode();
				break;
			case  KEY_PRINT: // 배출량 기록 "0" 으로 초기화 
				Data_Clear();
				break;
			case KEY_SET: test_set();
				break;
		} // of switch 
	}
}
   


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
		raw_data = Get_adc_from_module_20160629();  // 모듈로부터 a/d 값을 읽어온다.
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
			lcd70_show_string_mode(100, 300,str, 16, 0);
			
			sprintf(str, "v_adc_org_1=%6lu ",(long)v_adc_org);
			lcd70_show_string_mode(100, 330,str, 16, 0);

			sprintf(str, "gxlWeight_1=%6lu ",(long)gxlWeight);
			lcd70_show_string_mode(100, 360,str, 16, 0);
		}
		
		// 음수이면 
		if(sign_flag) gxlGrossWeight = gplCompensationWeight - gxlWeight;
		else 		  gxlGrossWeight = gplCompensationWeight + gxlWeight;
		
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
				lcd70_write_weight(100,150,gxlGrossWeight);//  Big font total weight displayin
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



void goto_iap(char *ip, u16 port)																 
{
	int ret_val;
	int retry = 0;
//	int iz;
	char szIP[20];
	
	return;

//	NVIC_InitTypeDef NVIC_InitStructure;

	USART1_putchar(' ');
	USART1_putchar('I');
	USART1_putchar('A');
	USART1_putchar('P');

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

	lcd70_clear_screen(WHITE);

//	lcd70_show_string_mode_24x24(0, 200,"장비를 업그레이드 합니다.", 16, 0);
//	lcd70_show_string_mode_24x24(0, 250,"잠시만 기다려 주십시요.", 16, 0);
	lcd70_show_string_mode(250, 200,"장비를 업그레이드 합니다.", 16, 0);
	lcd70_show_string_mode(250, 250,"잠시만 기다려 주십시요.", 16, 0);
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
	lcd70_show_string_mode(500, 10, (const u8 *)sT0, strlen(sT0), 0);

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
	lcd70_show_string_mode(170, 460, (const unsigned char *)str, 20, 0);
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

get_weight(int dsp_enabled)
{
	long raw_adc;
	
	raw_adc = Get_adc_from_module_20160629();

	if(Weight_flag)// 모듈로 부터 무게를 정상적으로 받았으면  현재무게를 표시한다. 
	{
		Weight_flag = 0;
		
		v_adc1_buf = (long)(gnfFactor * raw_adc);
		diff1 = v_adc1_buf - v_adc_org;
		diff2 = v_adc1_buf - prev_adc1;	// for stable 
		gxlWeight = ((diff1) / v_ei_multiply_factor) * v_division;
		
		if(gnlDisplay)
		{
			sprintf(str, "gxlWeight=%6ld ",(long)gxlWeight);
			lcd70_show_string_mode(10, 360,str, 16, 0);
			sprintf(str, "diff1=%6ld ",(long)diff1);
			lcd70_show_string_mode(10, 300,str, 16, 0);
			sprintf(str, "v_adc_org=%6ld ",(long)v_adc_org);
			lcd70_show_string_mode(10, 330,str, 16, 0);
		}

		gxlGrossWeight = gxlWeight + gplCompensationWeight;
		if(Real_Weight_Factor != 10000)// 무게보정 factor가 가동되어을때 
		{
			gxlGrossWeight = (float)(((Real_Weight_Factor)/10000.0) * gxlGrossWeight);
		}

		//한눈의 값으로 맞추기
		if(gxlGrossWeight < 0) gxlGrossWeight -= (long)(-gxlGrossWeight % v_division);
		else gxlGrossWeight -= ((long)gxlGrossWeight % v_division);
		
		if(dsp_enabled)
		{
			if(gxlGrossWeight < (long)(v_capacity + 2000)) // 현재무게가 v_capacity 보다 작을때 현재무게를 표시한다.
			{
				if(0 <= gxlGrossWeight)
				{
					lcd70_write_weight(100,150,gxlGrossWeight);//  Big font total weight displayin
				}
			}	
			else //over 20160629
			{
				lcd70_write_weight(100, 150, 99999);//  Big font total weight displayin
			}
		}
		
///STST			
//Send_to_server(SEND_STATUS, "status");
	}
}		

/* end of file */
