/*
	sms.h

  	2014.8.27
*/

#ifndef	__SMS_H__
#define	__SMS_H__


#define MAX_giSMSSendingRetry	3
extern volatile int giSMSSendingRetry;	
extern volatile int giSMSSendingFlag;

int send_sms(char *receive_telno, char *msg);


#endif	//__SMS_H__

/* end of file */
