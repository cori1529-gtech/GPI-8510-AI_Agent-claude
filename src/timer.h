
#ifndef  TIMER_PRESENT
#define  TIMER_PRESENT

typedef enum
{
	timer1ServiceFunction,
	timer2ServiceFunction,
	timerServiceFunctionMAX
} timer_register_function_type;

typedef            void     (*timer_register_function)(void);

typedef struct _timer_service_function_type
{
	timer_register_function_type service_type;
	timer_register_function run;
} timer_service_function_type;

extern unsigned long  Ten_Mili_Counter;
extern unsigned long Sec_Mili_Counter;
extern unsigned long Sending_Mili_Counter;
extern unsigned long Led_Mili_Counter;
extern unsigned long Error_Led_Mili_Counter;
extern unsigned long Led_Mili_Counter_2;
extern unsigned long Error_Led_Mili_Counter_2;
extern unsigned long Zero_Mili_Counter;

extern unsigned long  Pulse_timer;

extern unsigned long Feeding_Counter1; //배출량 계산주기 타이머
extern unsigned long Feeding_Counter2; //배출량 계산주기 타이머




//extern unsigned int  Minute;

/***************** RTC ********************************/

extern void Delay (unsigned int nCount); 
extern void Clock_Pin_Init(void); 



#define PBPORT         GPIOB 
#define DS7_SCLK   GPIO_Pin_5 //DS1302?? 
#define DS6_IO    GPIO_Pin_8 // DS1302 ?据 
#define DS5_RST   GPIO_Pin_9 //DS1302片?/?位 
extern unsigned char  Settimebuf[7],Gettimebuf[7];

extern void Clock_Pin_Init(void);
extern void W8bit_HT1381(u8 wrdata);
extern unsigned char R8bit_HT1381(void);
extern void Write1381(unsigned char Addr, unsigned char Wdata);
extern unsigned char Read1381(unsigned char Addr) ;
extern void Set1381(unsigned char *pSecDa);
extern void Get1381(void);



/******************RTC******************************/








/* ------------------------------------------------------------------------------------------------- */
/* BSP Timer */
/* ------------------------------------------------------------------------------------------------- */
extern void register_timer_function(timer_register_function_type timer_fn_type, timer_register_function fn);
extern void bsp_init_timer2(void/*isr_function timer4_isr*/);
extern u16 bsp_get_timer2_cnt(void);
extern void bsp_set_timer2_cnt( u16 cnt);

#endif
