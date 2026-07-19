#ifndef __TCPIP_SEND_H__
#define __TCPIP_SEND_H__

#include "stm32f10x.h"

void cdma_putchar(char ch);
int cdma_send_cmd(char *cmd, int length);
int cdma_send_msg(char *msg, int length);

int get_cdma_telno(void);
int get_cdma_time(void);
int get_cdma_rssi(void);
void goto_iap(char *ip, u16 port);																 



void Stage_One_DataLoading(char *jangbi_code,  char *cardno, unsigned int uWeight);

int Stage_One_DataLoading_New(char *jangbi_code,  char *cardno, unsigned int uWeight, char *date_time);

int SocketOpen_LU202F_onestop(char *ip, int port, int wait);

void test_routine(void);

void tcpip_send_20140806(char *ip, int port);

void tcpip_send_c1_data_20140806(char *ip, int port);
void tcpip_send_c4_status_20140808(char *ip, int port);

void tcpip_send_form_zigbee_data(char *ip, int port);


void goto_iap(char *ip, u16 port);																 



#endif //__TCPIP_SEND_H__
