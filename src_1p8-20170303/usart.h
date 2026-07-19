
#ifndef  USART_PRESENT
#define  USART_PRESENT

#include <stm32f10x_usart.h>

#define BAUDRATE_485 9600

/* ------------------------------------------------------------------------------------------------- */
/* BSP USART */
/* ------------------------------------------------------------------------------------------------- */
typedef enum
{
	usartmodeDMA,
	usartmodeIRQ,
	usartmodeMAX
} usartmode_type;

typedef enum
{
	usart1ServiceFunction,
	usart2ServiceFunction,
	usart3ServiceFunction,
	usartServiceFunctionMAX
} usart_register_function_type;


extern u8 Usart2_Interrupt_flag;
extern u32 RxCounter2;
extern u8 RxBuffer2[];
extern u8 cmd_2[50];//usart 2

extern u8 Usart4_Interrupt_flag;

extern vu8 	RxCdmaBuffer[];
extern vu16 RxCdmaCounter;

extern vu8 	RxCdmaSmsBuffer[30];
extern vu16	RxCdmaSmsCounter;

extern char gbSMSSTX;
extern	char gbSMSSTX;
extern	u8 	 smsbuffer[];
extern 	u8   Usart3_SMS_flag;
extern 	u8 	 Sms_Message_Number;  // sms °¹¼ö 
extern u8 		Keco_card_info_flag;

typedef            void     (*usart_register_function)(char data);

typedef struct _usart_service_function_type
{
	usart_register_function_type service_type;
	usart_register_function run;
} usart_service_function_type;

//#define USART1_putchar(ch) 		usart1_transmit_byte(ch)	//add 2014.9.25
void USART1_puts(char *str);	//add 2014.9.25
#define STDIO_puts(str) 		SART1_puts(str)	//add 2014.9.25
#define MONITORING_puts(str) 	USART1_puts(str)	//add 2014.9.25
#define MONITORING_putchar(ch) 	USART1_putchar(ch)	//add 2014.9.25


extern void init_usart1_buffer(void);				    
extern void usart_transmit_byte( USART_TypeDef* port, u16 chr);
extern void usart1_transmit_byte( u16 chr);
extern void usart1_tx_proc(void);

extern void usart_transmit_string(USART_TypeDef* port, const void *data);
extern void usart1_transmit_string(char *data);
extern void usart1_transmit_string_format(const char * szFormat, ... );

void ZIGBEE_puts(char *str);

#endif                                                          

/* End of module include. */
