/*
	def.h
*/

#ifndef	__DEF_H__
#define	__DEF_H__

//#define	VERSION	"2.6"	//2017.6.26
//#define	VERSION	"2.7"	//2012.5.03  // CS5555 공용으로 사용할수 있게 수정
//#define	VERSION	"2.8"	//2012.5.03  // CS5555 공용으로 사용할수 있게 수정
 #define	VERSION	"2.9"	//2012.5.03  // CS5555 공용으로 사용할수 있게 수정
 
 #define	VERSION	"3.2"	//2012.5.03  // CS5555 공용으로 사용할수 있게 수정
 
 //#define	VERSION	"3.4"	// 2025 8 7 : LTE 모듈을 
 
#define	VERSION	"3.6"	// 2025   11월 6일 
 
#define	VERSION	"3.7"	// 2025   12월 10일 
 
#define	VERSION	"3.8"	// 2025   12월 10일 
 
#define	VERSION	"4.0"	// 2025   12월 10일 





//#define	VERSION	"1.8"	//2017.3.2
//#define	VERSION	"1.7"	//2016.7.25
//#define	VERSION	"1.6"


#define	FSMC_ENABLE    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE)
#define	FSMC_DISABLE   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE)


//2016.4.20 #define	RXCARDBUFFER2SIZE	8000
#define	RXCARDBUFFER2SIZE	2000
#define	MAX_RXBUFFER2_SMS	256

#define	RX_CDMA_BUFFER_SIZE	2000


#endif	//#ifdef __DEF_H__

/* end of file */
