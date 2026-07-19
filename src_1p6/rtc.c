/*
	rtc.c

	http://poohyhoh.blogspot.kr/2010/09/arm-cortex-m3-stm32-rtc-2.html
	http://wiki.csie.ncku.edu.tw/embedded/RTC
*/


#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"

#include "buzzer.h"
#include "usart.h"
#include "var.h"

int	RTC_TimeUpdated = 0;
				 
void RTC_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStructure;
//	RTC_InitTypeDef RTC_InitStructure;

 /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR, ENABLE);	/* Enable the PWR clock */
	PWR_BackupAccessCmd(ENABLE);	/* Allow access to RTC */
/*
	RCC_LSEConfig(RCC_LSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
*/
	RCC_LSICmd(ENABLE);                                   /* Enable the LSI OSC */
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);   /* Wait till LSI is ready */  
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);               /* Select the RTC Clock Source */


	/* Configure the RTC data register and RTC prescaler */
/*
	RTC_InitStructure.RTC_AsynchPrediv = 127;
	RTC_InitStructure.RTC_SynchPrediv = 249;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStructure);
*/
/**/

/*
	RCC_RTCCLKCmd(ENABLE);	// Enable the RTC Clock
	RTC_WaitForLastTask();
	RTC_SetPrescaler(32768);
	RTC_WaitForSynchro();	// Wait for RTC APB registers synchronisation
	RTC_WaitForLastTask();
*/

	RCC_RTCCLKCmd(ENABLE);	// Enable the RTC Clock
	RTC_WaitForSynchro();	// Wait for RTC APB registers synchronisation

//	RTC_SetPrescaler(32768);
	RTC_SetPrescaler(40000);

	RTC_WaitForLastTask();

 	RTC_ITConfig(RTC_IT_SEC, ENABLE); /* Enable the RTC Second */
}


void LED_Toggle_Red() /* Toggle Red */
{
	static int toggle = 0;
/*
	if(toggle) { toggle = 0; Output_Relay_Off(); }
	else { toggle = 1; Output_Relay_On(); }
*/
}

/*------------------------------------------------------------------------------
  RTC Interrupt Handler
 *------------------------------------------------------------------------------*/
void RTC_IRQHandler(void)
{
	static int msec = 0;
	static int sec = 0;
	static int min = 0;
	static int hour = 0;
	static int day = 0;
	
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET) {
        RTC_ClearITPendingBit(RTC_IT_SEC); /* Clear the RTC Second interrupt */
//      LED_Toggle_Red(); /* Toggle Red */
        RTC_TimeUpdated = 1; /* Enable time update */
	
		gnlTick++;
		
		sec++;
		gbSecEvent = 1;
		if(60 <= sec)
		{
			sec = 0;
			min++;
			gbMinEvent = 1;
			if(60 <= min)
			{
				min = 0;
				hour++;
				gbHourEvent = 1;
				if(hour <= 24)
				{
					hour = 0;
					day++;
					gbDayEvent = 1;
				}
			}
		}
		
		
//        RTC_WaitForLastTask(); /* Wait until last write operation on RTC registers has finished */
        /* Reset RTC Counter when Time is 23:59:59 */
//        if (RTC_GetCounter() == 0x00015180) {
//            RTC_SetCounter(0x0);
//            RTC_WaitForLastTask(); /* Wait until last write operation */
//		}
	}
} // end RTC_IRQHandler



uint8_t USART1_Scanf(uint32_t value) {
    uint32_t index = 0;  uint32_t tmp[2] = {0, 0};
    while (index < 2) {
        while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET) {}
        tmp[index++] = (USART_ReceiveData(USART1));
        if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39)) {
            printf("[ERROR] enter valid number between 0 and 9\n");  index--;
    } }
    index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10); /* Calculate the Corresponding value */
    if (index > value) {
        printf("[ERROR] enter valid number between 0 and %d\n", value); return 0xFF;
    }
    return index;
}

uint32_t Time_Regulate() {
    uint32_t Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;
	printf("Set Hours  ");
    while (Tmp_HH == 0xFF) { Tmp_HH = USART1_Scanf(23); }
    printf(":  %d\n", Tmp_HH);    printf("Set Minutes");
    while (Tmp_MM == 0xFF) { Tmp_MM = USART1_Scanf(59); }
    printf(":  %d\n", Tmp_MM);    printf("Set Seconds");
    while (Tmp_SS == 0xFF) { Tmp_SS = USART1_Scanf(59); }    printf(":  %d\n", Tmp_SS);
    return((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS));
}

void Time_Adjust(void) {
    RTC_WaitForLastTask(); /* Wait until last write operation */
    RTC_SetCounter(Time_Regulate()); /* Change the current time */
    RTC_WaitForLastTask(); /* Wait until last write operation */
}

void time_adjust_8()
{
   /* set 8:29:55 */
/*
   RTC_TimeTypeDef RTC_TimeStruct;
   RTC_TimeStruct.RTC_Hours = 0x08;
   RTC_TimeStruct.RTC_Minutes = 0x29;
   RTC_TimeStruct.RTC_Seconds = 0x55;

   RTC_SetTime(RTC_Format_BCD, &RTC_TimeStruct);
*/
}

void time_adjust(int hour, int min, int sec)
{
    RTC_WaitForLastTask(); /* Wait until last write operation */
//    RTC_SetCounter(Time_Regulate()); /* Change the current time */
    RTC_SetCounter(hour*3600 + min*60 + sec); /* Change the current time */
    RTC_WaitForLastTask(); /* Wait until last write operation */
}

void rtc_calendar_adjust(unsigned long seconds)
{
    RTC_WaitForLastTask(); /* Wait until last write operation */
    RTC_SetCounter(seconds); /* Change the current date, time */
    RTC_WaitForLastTask(); /* Wait until last write operation */
}


void Time_Show(void) {
    uint32_t THH = 0, TMM = 0, TSS = 0; uint32_t TimeVar;
	char str[30];

    while (1) {  /* Infinite loop */
        if (0 == RTC_TimeUpdated) { continue; }
        RTC_TimeUpdated = 0;
        TimeVar = RTC_GetCounter();

        THH = TimeVar / 3600;  /* Compute  hours */
        TMM = (TimeVar % 3600) / 60;  /* Compute minutes */
        TSS = (TimeVar % 3600) % 60;  /* Compute seconds */
        sprintf(str, "Time: %0.2d:%0.2d:%0.2d\r", THH, TMM, TSS);
		USART1_puts(str);
    }
}

void RTC_Calendar_Test(void)
{
	USART1_puts("123456");

// LED_Off_All();  USART_GetCharacter(USART1);  printf("RTC_Calendar_Test\n");
    RTC_Configuration();  /* RTC Configuration */
//    Time_Adjust();  /* Adjust time by values entred by the user on the hyperterminal */

//    BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);

//    RCC_ClearFlag();  /* Clear reset flags */
    Time_Show();   /* Display time in infinite loop */
}


int RTC_date_to_sec(char *sdate)
{
	int	 year, month, date, hour, min, sec;
	u32	rtc_seconds, rtc_days, rtc_year_since_2000;
	// the beginning of each month
	const u32 DaysToMonth[13] = { 0,31,59,90,120,151,181,212,243,273,304,334,365 };	
	
	int iz = 0;
	int ret_val = 0;
	char str[50];
	
	year = (sdate[iz+0]-'0')*1000 + (sdate[iz+1]-'0')*100 + (sdate[iz+2]-'0')*10 + (sdate[iz+3]-'0');
	month = (sdate[iz+4]-'0')*10 + (sdate[iz+5]-'0');
	date = (sdate[iz+6]-'0')*10 + (sdate[iz+7]-'0');
	hour = (sdate[iz+8]-'0')*10 + (sdate[iz+9]-'0');
	min =  (sdate[iz+10]-'0')*10 + (sdate[iz+11]-'0');
	sec = (sdate[iz+12]-'0')*10 + (sdate[iz+13]-'0');

//sprintf(str, "BEFORE CHECK %d-%02d-%02d %02d:%02d:%02d", year, month, date, hour, min, sec);
//MONITORING_puts(str);

	if((2016<=year) && (year<=2099))
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
							ret_val = 6400;	//OK
	/*
							jangbi.tm.year = year;
							jangbi.tm.month = month;
							jangbi.tm.date = date;
							jangbi.tm.hour = hour;
							jangbi.tm.min = min;
							jangbi.tm.sec = sec;
	*/
							jangbi.cdma.tm.year = year;
							jangbi.cdma.tm.month = month;
							jangbi.cdma.tm.date = date;
							jangbi.cdma.tm.hour = hour;
							jangbi.cdma.tm.min = min;
							jangbi.cdma.tm.sec = sec;

							rtc_year_since_2000 = year - 2000;
							rtc_days = rtc_year_since_2000*365 + (rtc_year_since_2000/4) + 1; //leap year  until 2099. (+1 is 2000)
							rtc_days += DaysToMonth[month];
							if((2<month) && !(year%4)) rtc_days++;	//leap year  until 2099.
							rtc_days += date - 1;
							rtc_seconds = rtc_days * 3600 * 24 + hour*3600 + min*60 + sec;
							rtc_calendar_adjust(rtc_seconds);
							
//sprintf(str, "SYNC::%d-%02d-%02d %02d:%02d:%02d, %lu,%lu", year, month, date, hour, min, sec, rtc_seconds, rtc_days);
sprintf(str, "SYNC::%d-%02d-%02d %02d:%02d:%02d", year, month, date, hour, min, sec);
MONITORING_puts(str);	
						}
					}
				}
			}
		}
	}

	return ret_val;
}

/* end of file */
