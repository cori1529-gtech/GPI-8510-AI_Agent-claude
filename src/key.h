
#ifndef  KEY_PRESENT
#define  KEY_PRESENT

#include "hw_config.h"
#include "buzzer.h"
#include <stm32f10x.h>

/*************  gpi-8510 ************************/

#define    LEFT_ARROW_ON	       if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6) == 0x00)	 //
#define    RIGHT_ARROW_ON	      if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0x00)	 //

#define    UP_ARROW_ON	         if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9) == 0x00)	 //
#define    DOWN_ARROW_ON	      if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5) == 0x00)	//
#define    ENTER_ARROW_ON	      if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7) == 0x00)	//


#define    LEFT_ARROW_OFF	       if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6) == 0x00)	//
#define    RIGHT_ARROW_OFF	     if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5) == 0x00)	//
#define    UP_ARROW_OFF	         if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9) == 0x00)	//
#define    DOWN_ARROW_OFF	     if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5) == 0x00)	//
#define    ENTER_ARROW_OFF	   if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7) == 0x00)	//





/*************  gpi-8510 ************************/





#define    KEY_1    1
#define    KEY_2    2
#define    KEY_3    3
#define    KEY_4    4
#define    KEY_5    5
#define    KEY_6    6
#define    KEY_7    7
#define    KEY_8    8
#define    KEY_9       9
#define    KEY_0        0

#define    KEY_CLEAR     10
#define    KEY_ENTER     11
#define    KEY_WEIGHT    12
#define    KEY_RECORD    13
#define    KEY_STATUS    14
#define    KEY_PRINT     15
#define    KEY_SET       16
#define    KEY_LEFT      17
#define    KEY_RIGHT     18




// KEY Value
#define KEY_NONE	0x00
#define KEY_KEY1	0x01
#define KEY_KEY2	0x02
#define KEY_KEY3	0x04
#define KEY_KEY4	0x08
#define KEY_KEY5	0x10

// 8510 KEY CODE

#define  UP	     1
#define  DOWN   2
#define  LEFT	   3
#define  RIGHT	   4
#define  ENTER	   5 
#define  DOUBLE	   6  


/*
#define	KEYCHECKTIME	3

volatile char key_flag;
volatile int key_count;


volatile char NewKeyIn;
//volatile char ReleaseKeyIn;
volatile char PressKeyIn;
volatile char PreKeyIn;

volatile u16	KeyCode;
volatile u16	KeyCodePre;

volatile u16	KeyCheckTimer = KEYCHECKTIME;

*/




typedef struct
{
	GPIO_TypeDef* gpio_reg;
	u16          pin;
}bsp_key_group_type;



typedef            void     (*exti_key_register_function)(void);


extern u16    KEYCODE;
extern volatile u16	KeyCode;
extern volatile u16	KeyCheckTimer ;


extern volatile char NewKeyIn;
//volatile char ReleaseKeyIn;
extern volatile char PressKeyIn;
extern volatile char PreKeyIn;

extern volatile u16	KeyCodePre;








extern u8	  qKEYIN;
extern u8	  nKEYIN;
extern u8	  rKEYIN;
extern char key_disp[70];



/* ------------------------------------------------------------------------------------------------- */
/* BSP KEY */
/* ------------------------------------------------------------------------------------------------- */
extern void bsp_key_gpio_init(void);
 void Rtc_Write(void);
 void Rtc_Read(void);





char	keypressed(void);
char    keypush(void);
unsigned char    keypush_8510(void);

u16     KEYINENC(void);
void    CHKEYIN(void);

long get_key( int x, int y, char *msg, long val, int max_digit);

void First_digit_On(void);
void Second_digit_On(void);
void Third_digit_On(void);
void Fourth_digit_On(void);
void Fiveth_digit_On(void);
void key_init();



	
#endif
