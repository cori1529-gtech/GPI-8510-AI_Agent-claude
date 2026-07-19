
#ifndef  KEY_PRESENT
#define  KEY_PRESENT

#include "hw_config.h"
#include <stm32f10x.h>



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



typedef struct
{
	GPIO_TypeDef* gpio_reg;
	u16          pin;
}bsp_key_group_type;



typedef            void     (*exti_key_register_function)(void);


extern u16    KEYCODE;
extern u8	  qKEYIN;
extern u8	  nKEYIN;
extern u8	  rKEYIN;
extern char key_disp[70];



/* ------------------------------------------------------------------------------------------------- */
/* BSP KEY */
/* ------------------------------------------------------------------------------------------------- */
extern void bsp_key_gpio_init(void);

char    keypush(void);
u16     KEYINENC(void);
unsigned char    CHKEYIN(void);

void First_digit_On(void);
void Second_digit_On(void);
void Third_digit_On(void);
void Fourth_digit_On(void);
void Fiveth_digit_On(void);



	
#endif
