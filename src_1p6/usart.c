
/*
	usart.c
*/

#include <stdio.h>

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"

#include "def.h"
#include "type.h"
#include "global.h"
#include "buzzer.h"
#include "lcd.h"
#include "var.h"	//eeprom 변수들



void USARTx_setting(USART_TypeDef* USARTx, long baudrate, char parity);

void USART2TX_enable(void);
void USART2TX_disable(void);
//void USART3TX_enable();
//void USART3TX_disable();

void USART1_putchar(char ch);
void USART1_puts(char *str);
void USART1_print(char *str);
void USART2_putchar(char ch);
void USART2_puts(char *str);
void USART2_print(char *str);
void USART3_putchar(char ch);
void USART3_puts(char *str);
void USART3_print(char *str);
void USART_2and3_putchar(char ch);
void USART_2and3_puts( char *str);
void USART_2and3_print(char *str);
void UART4_putchar(char ch);

void USARTx_putchar(USART_TypeDef* USARTx, char ch);
void USARTx_puts(USART_TypeDef* USARTx, char *str);
void USARTx_print(USART_TypeDef* USARTx, char *str);

void USART2_Rx_queue_initial(void);
void USART3_Rx_queue_initial(void);
void USART4_Rx_queue_initial(void);
int USART2_Rx_queue_put(char ch);
int USART3_Rx_queue_put(char ch);
int USART4_Rx_queue_put(char ch);
int USART2_Rx_queue_get(void);
int USART3_Rx_queue_get(void);
int USART4_Rx_queue_get(void);
int isUSART2_Rx_queue_empty(void);
int isUSART3_Rx_queue_empty(void);
int isUSART4_Rx_queue_empty(void);

void USARTx_Rx_queue_initial(int *qhead, int *qtail);
int USARTx_Rx_queue_put(char *queue, int *qhead, char ch, int limit);
int USARTx_Rx_queue_get(char *queue, int *qhead, int *qtail, int limit);
int isUSARTx_Rx_queue_empty(int *qhead, int *qtail);

volatile u16 guRS422485TxEnableTimer;

extern volatile u16 guCommLedOnTimer;


#define	USART2_RX_BUF_SIZE	200
char USART2_RxBuf[USART2_RX_BUF_SIZE];
int	USART2_Rx_queue_head = 0;
int	USART2_Rx_queue_tail = 0;

#define	USART3_RX_BUF_SIZE	200
char USART3_RxBuf[USART3_RX_BUF_SIZE];
int	USART3_Rx_queue_head = 0;
int	USART3_Rx_queue_tail = 0;

#define	USART4_RX_BUF_SIZE	200
char USART4_RxBuf[USART4_RX_BUF_SIZE];
int	USART4_Rx_queue_head = 0;
int	USART4_Rx_queue_tail = 0;

u8	Usart2_Interrupt_flag;
u32	RxCounter2;
u8	RxBuffer2[200];
u8	cmd_2[50];//usart 2




#define	MAX_USART4_BUFFER	100
u8	Usart4_Interrupt_flag = 0;
u32	RxCounter4 = 0x00;
vu8 Rx4Cmd[MAX_USART4_BUFFER];
u32	Rx4CmdCounter = 0x00;
u8	RxBuffer4[MAX_USART4_BUFFER];

//////////////////////////////////////////////////
#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>

#include "stm32f10x.h"
#include "hw_config.h"
#include "queue.h"
#include "usart.h"

// setting USART
#define USART_TX_BUFF_CNT	64
#define USART_RX_BUFF_CNT	10

#define USART_TX_DMA_SIZ          64
#define USART_RX_DMA_SIZ          64

q_list_type gbl_qlist_usart1_tx;
q_list_type gbl_qlist_usart1_tx_free;

q_node_type gbl_qdata_usart1_tx[USART_TX_BUFF_CNT];
q_node_type gbl_qdata_usart1_tx_free[USART_TX_BUFF_CNT];

char gbl_usart1_tx_buff[USART_TX_BUFF_CNT][USART_TX_DMA_SIZ];
char gbl_usart1_tx_buff_dma[USART_TX_DMA_SIZ];

q_list_type gbl_qlist_usart1_rx;
q_list_type gbl_qlist_usart1_rx_free;

q_node_type gbl_qdata_usart1_rx[USART_RX_BUFF_CNT];
q_node_type gbl_qdata_usart1_rx_free[USART_RX_BUFF_CNT];

char gbl_usart1_rx_buff[USART_RX_BUFF_CNT][USART_RX_DMA_SIZ];
char gbl_usart1_rx_buff_dma[USART_RX_DMA_SIZ];

static u16 s_usart1_tx_send_cnt = 0;
static u16 s_usart1_tx_q_free_cnt = 0;
static u16 s_usart1_tx_q_send_cnt = 0;

static s8 usart_format_buff[128];
 /*
char gbSMSSTX = 0;
extern	char gbSMSSTX;
extern	u8 	 smsbuffer[];
extern 	u8   Usart3_SMS_flag;
extern 	u8 	 Sms_Message_Number;  // sms 갯수 
 */



 char gbSMSSTX;
 char gbSMSSTX;
 u8 	 smsbuffer[];
 u8   Usart3_SMS_flag;
 u8 	 Sms_Message_Number;  // sms 갯수 


typedef struct
{
	u16 wr_idx;
	u16 rd_idx;
	char buffer[USART_RX_DMA_SIZ];
}usart_rx_type;

usart_rx_type gbl_usart1_rx_buff_proc;

u16 usart1_dma_transfering = FALSE;
//

static usartmode_type s_usart1_mode = usartmodeIRQ;
static usartmode_type s_usart2_mode = usartmodeIRQ;
static usartmode_type s_usart3_mode = usartmodeIRQ;




/* ------------------------------------------------------------------------------------------------- */
/* BSP USART */
/* ------------------------------------------------------------------------------------------------- */


void init_usart1_buffer(void)
{
	int i;

	q_remove_all(&gbl_qlist_usart1_tx);
	q_remove_all(&gbl_qlist_usart1_tx_free);

	// initialize usart tx queue buffer.
	for(i=0;i<USART_TX_BUFF_CNT;i++)
	{
		gbl_qdata_usart1_tx_free[i].data = &gbl_usart1_tx_buff[i];
		q_add_tail(&gbl_qlist_usart1_tx_free, &gbl_qdata_usart1_tx_free[i]);		
	}

	q_remove_all(&gbl_qlist_usart1_rx);
	q_remove_all(&gbl_qlist_usart1_rx_free);

	// initialize usart rx queue buffer.
	for(i=0;i<USART_RX_BUFF_CNT;i++)
	{
		gbl_qdata_usart1_rx_free[i].data = &gbl_usart1_rx_buff[i];
		q_add_tail(&gbl_qlist_usart1_rx_free, &gbl_qdata_usart1_rx_free[i]);		
	}

	memset(gbl_usart1_tx_buff_dma, 0x00, sizeof(char)*USART_TX_DMA_SIZ);
	memset(gbl_usart1_rx_buff_dma, 0x00, sizeof(char)*USART_RX_DMA_SIZ);
	memset(gbl_usart1_rx_buff_proc.buffer, 0x00, sizeof(char)*USART_RX_DMA_SIZ);	
	gbl_usart1_rx_buff_proc.rd_idx = 0;
	gbl_usart1_rx_buff_proc.wr_idx = 0;

	usart1_dma_transfering = FALSE;
}


/* ------------------------------------------------------------------------------------------------- */
/* extern USART */
/* ------------------------------------------------------------------------------------------------- */
//DEBUG PORT
void USART1_IRQHandler(void)
{
	u16 data_len;
	q_node_type* q_usart_pkt_ptr;
	s8 data[1];
	
	// u16 wr_idx;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		data_len = 1;

		if( q_get_count(&gbl_qlist_usart1_rx_free) > 0 )
		{
			if( (q_usart_pkt_ptr = q_remove_tail(&gbl_qlist_usart1_rx_free)) != NULL )
			{
				data[0] = USART_ReceiveData(USART1) & 0xFF;
				memcpy(q_usart_pkt_ptr->data, data, data_len);
				q_usart_pkt_ptr->len = data_len;
				q_add_tail(&gbl_qlist_usart1_rx, q_usart_pkt_ptr);

			}
		}	

		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	  
	}
}


/*
void USART2_IRQHandler(void)
{
	u16 data_len;
	s8 data[1];	

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{

		data_len = 1;
		data[0] = USART_ReceiveData(USART2) & 0xFF;

		usart2_transmit_byte(data[0]);

		USART_ClearITPendingBit(USART2, USART_IT_RXNE);	  
	}
}

 */

//로드셀
void USART2_IRQHandler(void)
{
	u8 buffer2;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  	{
    	// Read one byte from the receive data register 
       
	 	buffer2=USART_ReceiveData(USART2);

//USARTx_putchar( USART1, buffer2);

	 	//@1-123456+0x0d+0x0a : total 11 byte 

		if(buffer2=='@' && RxCounter2<1)
	    {
			RxCounter2=0;
	    }
		RxBuffer2[RxCounter2++] = buffer2;
		if(RxCounter2>50)
		   RxCounter2=0;
     	if(buffer2==0x0a)					//extern u8 Tmoney_Status_flag; // 티머니 카드 읽을때 1  아니면 0
	    {
 			Usart2_Interrupt_flag = 1;		     

 			cmd_2[0]=RxBuffer2[RxCounter2-10];	//@
 			cmd_2[1]=RxBuffer2[RxCounter2-9];	//-
 			cmd_2[2]=RxBuffer2[RxCounter2-8];	//address
 			cmd_2[3]=RxBuffer2[RxCounter2-7];	//weight1
 			cmd_2[4]=RxBuffer2[RxCounter2-6];	//weight2
 			cmd_2[5]=RxBuffer2[RxCounter2-5];	//weight3
 			cmd_2[6]=RxBuffer2[RxCounter2-4];	//weight4
 			cmd_2[7]=RxBuffer2[RxCounter2-3];	//weight5
 			cmd_2[8]=RxBuffer2[RxCounter2-2];	//weight6
 			cmd_2[9]=RxBuffer2[RxCounter2-1];	//weight7
		}
	}
}


 /*
//2014.9.24 for cdma
void USART3_IRQHandler(void)
{
	u16 buffer1;
//	s8 data[1];	

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		buffer1=USART_ReceiveData(USART3);
		if(buffer1!=0x1ff) 
		{
		 RxCdmaBuffer[RxCdmaCounter++] = buffer1;
		 RxCdmaSmsBuffer[RxCdmaSmsCounter++] = buffer1;

		// extern vu8 	RxCdmaSmsBuffer[30];
        // extern vu16	RxCdmaSmsCounter;

		}
		//usart3_transmit_byte(buffer1);
		
     // sms 전문  @IAP=005#(종량제) -> 8310  #IAP=005@
		if(buffer1=='#')
		   RxCdmaSmsCounter=0;
		 if(buffer1=='@')	  //  @MPO=033# 
		 {
        
		   //if(RxCdmaCounter==8) //  @MPO=033#	 // 7 자리 
		   {
		    cmd_2[0]=RxCdmaSmsBuffer[RxCdmaSmsCounter-8];
            cmd_2[1]=RxCdmaSmsBuffer[RxCdmaSmsCounter-7];
            cmd_2[2]=RxCdmaSmsBuffer[RxCdmaSmsCounter-6];
            cmd_2[3]=RxCdmaSmsBuffer[RxCdmaSmsCounter-5];						   
            cmd_2[4]=RxCdmaSmsBuffer[RxCdmaSmsCounter-4];
		    cmd_2[5]=RxCdmaSmsBuffer[RxCdmaSmsCounter-3]; 
		    cmd_2[6]=RxCdmaSmsBuffer[RxCdmaSmsCounter-2];
            //RxCdmaCounter=0;
		    Usart3_SMS_flag=1;
			//cdma_send_cmd("AT$LGTACK", 9); 	//2014.10.21	 문자를 2번이상 보내지 않는다 .
			//Sms_Message_Number=7;
		    }
		  }
		   
	
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);	  
	}
}
 */

 
//2014.9.24 for CDMA  1126
void USART3_IRQHandler(void)
{
	u16 rxd;

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		rxd = USART_ReceiveData(USART3);

		RxCdmaQ_put(rxd);
//USART1_putchar(rxd);

		if(rxd != 0x1ff)
		{
	///		if(Keco_card_info_flag)	// 시간,rssi등 읽어올때 
			{
				RxCdmaBuffer[RxCdmaCounter++] = rxd;
				if(RX_CDMA_BUFFER_SIZE <= RxCdmaCounter) RxCdmaCounter = 0;
			}
	///		else
			{
				//@# .... @# 때문에
				if(rxd == '#') { sms_receiving_flag = 1; RxCounter2_sms = 0; }
				if(sms_receiving_flag)
				{	
					RxBuffer2_sms[RxCounter2_sms++] = rxd;
					if(MAX_RXBUFFER2_SMS <= RxCounter2_sms) RxCounter2_sms = 0;
					if((rxd == 0x0a) || (RxBuffer2_sms[RxCounter2_sms-2] == 0x0d) || (rxd == '@') || (250<RxCounter2_sms))
					{
						sms_receiving_flag = 0;
						Usart3_sms_received_flag = 1;
						RxBuffer2_sms[RxCounter2_sms++] = 0;
						RxCounter2_sms = 0;
					}
				}
			}
		}

		USART_ClearITPendingBit(USART3, USART_IT_RXNE);	  
	}
}


// UART4 for Zigbee
void UART4_IRQHandler(void)
{
	int iz;
	u16 rxd;

	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		rxd = USART_ReceiveData(UART4);
//USART1_putchar(rxd);	//rs-232 echo
//		beep(10);

		if(rxd != 0x01FF)
		{
//			led_rx_on();	//rx
			USART4_Rx_queue_put(rxd);
			RxBuffer4[RxCounter4++] = rxd;

			if(rxd == 0x0A) //
			{
///			}
///			else 
///			if(rxd == 0x0D) //ETX	///if(buffer1==0x03 && RxBuffer2[RxCounter2-10]==0X02 )
///			{
//				if(6 < RxCounter3)
				{
					Usart4_Interrupt_flag = 4;
					for(Rx4CmdCounter=0; Rx4CmdCounter<RxCounter4; Rx4CmdCounter++) Rx4Cmd[Rx4CmdCounter] = RxBuffer4[Rx4CmdCounter];
/////				Rx4Cmd[Rx4CmdCounter++] = rxd;
					Rx4Cmd[Rx4CmdCounter] = 0;

//					CdmaSendQueue_puts(Rx4Cmd);
				}
				RxCounter4 = 0;
			}

		}  //if(buffer1!=0x1FF)  
		
		if(MAX_USART4_BUFFER < RxCounter4) RxCounter4 = 0;

		USART_ClearITPendingBit(UART4, USART_IT_RXNE);	  
	}
}


void led_tx_on()
{
}

void USARTx_Initial()
{
	USART2_Rx_queue_initial();
	USART3_Rx_queue_initial();
	USART4_Rx_queue_initial();

	USARTx_setting( USART1, 115200, 0);
	USARTx_setting( USART2, 115200, 0);
	USARTx_setting( USART3, 115200, 0);
	USARTx_setting( UART4, 115200, 0);
}

void USARTx_setting(USART_TypeDef* USARTx, long baudrate, char parity)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	if(USARTx == USART1)
	{
//		init_usart1_buffer();

		/* Enable the USART1 Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		

		/* Enable USART1 clocks */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		/* 
		   USART1_REMAP USART1 remapping
		   This bit is set and cleared by software. It controls the mapping of USART1 TX and RX alternate
		   functions on the GPIO ports.
	
		   0: No remap (TX/PA9, RX/PA10)
		   1: Remap (TX/PB6, RX/PB7)		
		*/
		// GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);	

		/**********  Configurtion  USART1 GPIO *********************************/
		// Configure USART1 Rx (PA.10) as input floating 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		// Configure USART1 Tx (PA.09) as alternate function push-pull 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
	else if(USARTx == USART2)
	{
//		init_usart1_buffer();

		/* Enable the USART2 Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	
		/* Enable USART2 clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

		/**********  Configurtion  USART2 GPIO *********************************/
		// Configure USART2 Rx (PA.3) as input floating 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		// Configure USART2 Tx (PA.2) as alternate function push-pull 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		//485 TX ENABLE ACTIVE HIGH, 422 ALWAYS HIGH
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;	//485 TX ENABLE, NORMAL LOW
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
	else if(USARTx == USART3)
	{
//		init_usart1_buffer();

		/* Enable USART3 clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

		// Enable the USART3 Interrupt 
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		/**********  Configurtion  USART3 GPIO *********************************/
		// Configure USART3 Rx (PB.11) as input floating 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		// Configure USART3 Tx (PB.10) as alternate function push-pull 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		/*
		//485 TX ENABLE ACTIVE HIGH, 422 ALWAYS HIGH
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;	//485 TX ENABLE, NORMAL LOW
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		*/
	}
	else if(USARTx == UART4)
	{
//		init_usart1_buffer();

		/* Enable UART4 clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

		// Enable the UART4 Interrupt 
		NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		/**********  Configurtion  UART4 GPIO *********************************/
		// Configure UART4 Rx (PC.11) as input floating 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		// Configure UART4 Tx (PC.10) as alternate function push-pull 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	}
	else return;

	// Configure USARTx
	USART_DeInit(USARTx);
	USART_InitStructure.USART_BaudRate = baudrate;	//115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;

	if(parity == 1) USART_InitStructure.USART_Parity = USART_Parity_Odd;	//odd parity
	else if(parity == 2) USART_InitStructure.USART_Parity = USART_Parity_Even;	//even parity
	else USART_InitStructure.USART_Parity = USART_Parity_No;	//none parity

	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTx, &USART_InitStructure);

  	//Enable USARTx Receive and Transmit interrupts 
	USART_ClearFlag(USARTx, USART_FLAG_TC);
	USART_ClearFlag(USARTx, USART_IT_RXNE);    //?中?，以免一?用中?后立??生中?
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
	//Enable the USARTx
	USART_Cmd(USARTx, ENABLE);
}

void usart_transmit_byte( USART_TypeDef* port, u16 chr)
{
	USARTx_putchar( port, chr);
}

// #define SEND_DELAY 3
void USART1_putchar(char ch)
{
//	int dly = 300;	//baudrate 115200
	int dly = 300*48;	//baudrate 2400

/***
	while((USART_GetFlagStatus(USART1, USART_IT_TXE) == RESET) && --dly);
//	while((USART_GetFlagStatus(USART1, USART_IT_TXE) == RESET));
	USART_SendData(USART1, ch);
//	while((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) && --dly);
	//Delay_ms(SEND_DELAY);
****/

	while((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) && --dly);
	USART_SendData(USART1, ch);
	led_tx_on();	
}

void USART1_puts(char *str)
{
	while(*str)USART1_putchar(*str++);
	USART1_putchar(0x0d); //Delay_ms(SEND_DELAY);
	USART1_putchar(0x0a); //Delay_ms(SEND_DELAY);
}

void USART1_print(char *str)
{
	while(*str)USART1_putchar(*str++);
}




void usart1_tx_proc(void)
{
	int i;
	u16 data_len;
	// char tx_data[USART_TX_DMA_SIZ];
	q_node_type* q_usart_pkt_ptr;



	if( s_usart1_mode == usartmodeIRQ )
	{
		while( (q_usart_pkt_ptr = q_remove_tail(&gbl_qlist_usart1_tx)) != NULL )
		{
			data_len = q_usart_pkt_ptr->len;
			memcpy(gbl_usart1_tx_buff_dma, q_usart_pkt_ptr->data, q_usart_pkt_ptr->len);

			for(i=0;i<data_len;i++)
			{
				usart_transmit_byte(USART1, (u16)gbl_usart1_tx_buff_dma[i]);
			}
			q_add_tail(&gbl_qlist_usart1_tx_free, q_usart_pkt_ptr);

		}
	}	
	
}


void usart_transmit_string(USART_TypeDef* port, const void *data) 
{
	int i;
	u16 data_len;
	int tx_count;
	const char* usart_data;
	q_node_type* q_usart_pkt_ptr;

	usart_data = (char*)data;
	data_len = strlen(usart_data);

	tx_count = (data_len - 1) / USART_TX_DMA_SIZ + 1;

	INTLOCK();


	if( q_get_count(&gbl_qlist_usart1_tx_free) >= tx_count )
	{
		for(i=0;i<tx_count-1;i++)
		{
			if( (q_usart_pkt_ptr = q_remove_tail(&gbl_qlist_usart1_tx_free)) != NULL )
			{
				memcpy(q_usart_pkt_ptr->data, usart_data, USART_TX_DMA_SIZ);
				q_usart_pkt_ptr->len = USART_TX_DMA_SIZ;
				q_add_tail(&gbl_qlist_usart1_tx, q_usart_pkt_ptr);

				data_len = data_len - USART_TX_DMA_SIZ;
				usart_data += USART_TX_DMA_SIZ;

			}
		}

		if( (q_usart_pkt_ptr = q_remove_tail(&gbl_qlist_usart1_tx_free)) != NULL )
		{
			memcpy(q_usart_pkt_ptr->data, usart_data, data_len);
			q_usart_pkt_ptr->len = data_len;
			q_add_tail(&gbl_qlist_usart1_tx, q_usart_pkt_ptr);

		}

		if( usart1_dma_transfering == FALSE )
			usart1_tx_proc();

			
	}	


	INTFREE();
	
}

void usart1_transmit_string(char *data)
{
	USART1_print(data);
}

void usart1_transmit_string_format(const char * szFormat, ... )
{
	va_list varpars;
	int nLen;

	va_start(varpars, szFormat);
	nLen = vsprintf( (char*)usart_format_buff, szFormat, varpars);
	va_end(varpars);

	usart_format_buff[nLen] = 0x00;

	USART1_print(usart_format_buff);	
}

	 
/*
and section 23.2.17.7 USART_ClearITPendingBit page 558 it can clear pending TC interrupt

and the other option is to write to Tx data register 
"TC flag can be also cleared by
software sequence: a read operation to USART_SR register
(USART_GetFlagStatus()) followed by a write operation to
USART_TDR register (USART_SendData())."

"TC pending bit can be also cleared
by software sequence: a read operation to USART_SR
register (USART_GetITStatus()) followed by a write operation
to USART_TDR register (USART_SendData())."
*/


void USART2TX_enable()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	guRS422485TxEnableTimer = 6; //about 6 ms
}

void USART2TX_disable()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}
void USART2_putchar(char ch)
{
//	int dly = 300;	//baudrate 115200
	int dly = 300*48;	//baudrate 2400

////	while((USART_GetFlagStatus(USART2, USART_IT_TXE) == RESET) && --dly);
//	while((USART_GetFlagStatus(USART2, USART_IT_TXE) == RESET));

///20160629	USART2TX_enable();
	while((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) && --dly);
	USART_SendData(USART2, ch);
//	while((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) && --dly);
 
	 //while((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) );
   // Delay_ms(2);

	//Delay_ms(SEND_DELAY);
	led_tx_on();	
}

void USART2_puts(char *str)
{
	while(*str)USART2_putchar(*str++);
	USART2_putchar(0x0d); //Delay_ms(SEND_DELAY);
	USART2_putchar(0x0a); //Delay_ms(SEND_DELAY);
}

void USART2_print(char *str)
{
	while(*str)USART2_putchar(*str++);
}


void USART3_putchar(char ch)
{
//	int dly = 300;	//baudrate 115200
	int dly = 300*48;	//baudrate 2400

//	USART3TX_enable();

////	while((USART_GetFlagStatus(USART2, USART_IT_TXE) == RESET) && --dly);
//	while((USART_GetFlagStatus(USART2, USART_IT_TXE) == RESET));

	while((USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) && --dly);
	USART_SendData(USART3, ch);
//	while((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) && --dly);
 
	 //while((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) );
   // Delay_ms(2);

	//Delay_ms(SEND_DELAY);

	led_tx_on();	
}

void USART3_puts(char *str)
{
	while(*str)USART3_putchar(*str++);
	USART3_putchar(0x0d); //Delay_ms(SEND_DELAY);
	USART3_putchar(0x0a); //Delay_ms(SEND_DELAY);
}

void USART3_print(char *str)
{
	while(*str)USART3_putchar(*str++);
}


void UART4_putchar(char ch)
{
//	int dly = 300;	//baudrate 115200
	int dly = 300*48;	//baudrate 2400

//	USART3TX_enable();

////	while((USART_GetFlagStatus(USART2, USART_IT_TXE) == RESET) && --dly);
//	while((USART_GetFlagStatus(USART2, USART_IT_TXE) == RESET));

	while((USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET) && --dly);
	USART_SendData(UART4, ch);
//	while((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) && --dly);
 
	 //while((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) );
   // Delay_ms(2);

	//Delay_ms(SEND_DELAY);

	led_tx_on();	
}

void UART4_print(char *str)
{
	while(*str)UART4_putchar(*str++);
}

void UART4_puts(char *str)
{
	UART4_print(str);
	UART4_putchar(0x0d); //Delay_ms(SEND_DELAY);
	UART4_putchar(0x0a); //Delay_ms(SEND_DELAY);
}


void USARTx_putchar(USART_TypeDef* USARTx, char ch)
{
//	int dly = 300;	//baudrate 115200
	int dly = 300*48;	//baudrate 2400

	while((USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET) && --dly);
	USART_SendData(USARTx, ch);
//	while((USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET) && --dly);
	led_tx_on();	
}

void USARTx_puts(USART_TypeDef* USARTx, char *str)
{
	while(*str) USARTx_putchar(USARTx, *str++);
	USARTx_putchar(USARTx, 0x0d); //Delay_ms(SEND_DELAY);
	USARTx_putchar(USARTx, 0x0a); //Delay_ms(SEND_DELAY);
}

void USARTx_print(USART_TypeDef* USARTx, char *str)
{
	while(*str) USARTx_putchar(USARTx, *str++);
}

void USART3TX_enable()
{
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
	guRS422485TxEnableTimer = 6; //about 6 ms
}

void USART3TX_disable()
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}

//USART2 AND USART3 동시에 출력 (전송시간에 따른 시간지연을 최소화하기 위함.)
void USART_2and3_putchar(char ch)
{
//	int dly = 300;	//baudrate 115200
	int dly = 300*48;	//baudrate 2400

//	USART3TX_enable();
	while((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) && --dly);
	USART_SendData(USART2, ch);
	USART_SendData(USART3, ch);
	
	led_tx_on();	
}

void USART_2and3_puts( char *str)
{
	while(*str) USART_2and3_putchar(*str++);
	USART_2and3_putchar(0x0d); //Delay_ms(SEND_DELAY);
	USART_2and3_putchar(0x0a); //Delay_ms(SEND_DELAY);
}

void USART_2and3_print(char *str)
{
	while(*str) USART_2and3_putchar(*str++);
}

void USART2_Rx_queue_initial()
{
	USARTx_Rx_queue_initial( &USART2_Rx_queue_head, &USART2_Rx_queue_tail);
}

void USART3_Rx_queue_initial()
{
	USARTx_Rx_queue_initial( &USART3_Rx_queue_head, &USART3_Rx_queue_tail);
}

void USART4_Rx_queue_initial()
{
	USARTx_Rx_queue_initial( &USART4_Rx_queue_head, &USART4_Rx_queue_tail);
}

void USARTx_Rx_queue_initial(int *qhead, int *qtail)
{
	*qhead = *qtail = 0;
}

//*buffer, head, tail, max

/***
int USARTx_Rx_queue_put(char ch)
{
    if ((USART2_Rx_queue_head+1) % USART2_Rx_queue_size == USART2_Rx_queue_tail) return FALSE;

	USART2_Rx_queue_head = (USART2_Rx_queue_head+1) % USART2_Rx_queue_size;
	USART2_RxBuf[USART2_Rx_queue_head] =  ch;

    return TRUE;
}

char USARTx_Rx_queue_get()
{
	if (isUSART2_Rx_queue_empty() == TRUE)	return TRUE;
	
	USART2_Rx_queue_tail = (USART2_Rx_queue_tail+1) % USART2_Rx_queue_size;

	return USART2_RxBuf[USART2_Rx_queue_tail];
}
	
int isUSARTx_Rx_queue_empty()
{
	if(USART2_Rx_queue_head == USART2_Rx_queue_tail) return TRUE;
	else return FALSE;
}
***/

int USART2_Rx_queue_put(char ch)
{
	return USARTx_Rx_queue_put(USART2_RxBuf, &USART2_Rx_queue_head, ch, USART2_RX_BUF_SIZE);
}

int USART3_Rx_queue_put(char ch)
{
	return USARTx_Rx_queue_put(USART3_RxBuf, &USART3_Rx_queue_head, ch, USART3_RX_BUF_SIZE);
}

int USART4_Rx_queue_put(char ch)
{
	return USARTx_Rx_queue_put(USART4_RxBuf, &USART4_Rx_queue_head, ch, USART4_RX_BUF_SIZE);
}

int USARTx_Rx_queue_put(char *queue, int *qhead, char ch, int limit)
{
//  if (*qhead+1) % USART2_Rx_queue_size == qtail) return FALSE;

	*qhead = ++*qhead % limit;
	queue[*qhead] =  ch;

    return TRUE;
}

int USART2_Rx_queue_get(void)
{
	return USARTx_Rx_queue_get( USART2_RxBuf, &USART2_Rx_queue_head, &USART2_Rx_queue_tail, USART2_RX_BUF_SIZE);
}

int USART3_Rx_queue_get()
{
	return USARTx_Rx_queue_get( USART3_RxBuf, &USART3_Rx_queue_head, &USART3_Rx_queue_tail, USART3_RX_BUF_SIZE);
}

int USART4_Rx_queue_get()
{
	return USARTx_Rx_queue_get( USART4_RxBuf, &USART4_Rx_queue_head, &USART4_Rx_queue_tail, USART4_RX_BUF_SIZE);
}

int USARTx_Rx_queue_get( char *queue, int *qhead, int *qtail, int limit)
{
	if (isUSARTx_Rx_queue_empty( qhead, qtail) == TRUE)	return -1;	//TRUE;
	
	*qtail = ++*qtail % limit;

	return queue[*qtail];
}

int isUSART2_Rx_queue_empty()
{
	return isUSARTx_Rx_queue_empty( &USART2_Rx_queue_head, &USART2_Rx_queue_tail);
}

int isUSART3_Rx_queue_empty()
{
	return isUSARTx_Rx_queue_empty( &USART3_Rx_queue_head, &USART3_Rx_queue_tail);
}

int isUSART4_Rx_queue_empty()
{
	return isUSARTx_Rx_queue_empty( &USART4_Rx_queue_head, &USART4_Rx_queue_tail);
}

int isUSARTx_Rx_queue_empty(int *qhead, int *qtail)
{
	if(*qhead == *qtail) return TRUE;
	else return FALSE;
}

int RxCdmaQ_put(u16 ch)
{
	RxCdmaQ_qhead = ++RxCdmaQ_qhead % RXCDMAQ_SIZE;
	RxCdmaQ[RxCdmaQ_qhead] = ch & 0x00ff;

	return RxCdmaQ_qhead;
}


int RxCdmaQ_get()
{
	if (RxCdmaQ_qhead == RxCdmaQ_qtail)	return -1;	//queue empty
	
	RxCdmaQ_qtail = ++RxCdmaQ_qtail % RXCDMAQ_SIZE;

	return RxCdmaQ_qtail;
}

int RxCdmaTokenQ_puts(char *txt)
{
	int	ret_val = -1;

	RxCdmaTokenQ_qhead = ++RxCdmaTokenQ_qhead % RXCDMATOKENQ_SIZE;
	strncpy(RxCdmaTokenQ[RxCdmaTokenQ_qhead], txt, strlen(txt)+1);

//USART1_puts(CdmaSendQueue[CdmaSendQueue_qhead]);

	return ret_val;
}

int RxCdmaTokenQ_gets()
{
	if (RxCdmaTokenQ_qhead == RxCdmaTokenQ_qtail)	return -1;	//queue empty
	
	RxCdmaTokenQ_qtail = ++RxCdmaTokenQ_qtail % RXCDMATOKENQ_SIZE;

//USART1_puts(CdmaSendQueue_queue[CdmaSendQueue_qtail]);

	return RxCdmaTokenQ_qtail;
}




/* end of file */
