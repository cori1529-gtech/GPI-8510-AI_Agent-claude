/*
	def.h
*/

#ifndef	__DEF_H__
#define	__DEF_H__

#define	VERSION	"1.8"	//2017.3.2
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
