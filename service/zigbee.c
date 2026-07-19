/*
	zigbee.c
*/						   
#include <stdio.h>
#include <string.h>

void zigbee_setting()
{
//	int	iz;
	char str[50];

	int	guMyAdr = 0;
	int	guParentAdr = 0;
	int	guChildAdr = 0;

//	vputs("ST");
	strcpy(str, "ATZ");  ZIGBEE_puts(str); Delay_ms(1500);
	strcpy(str, "ATE1"); ZIGBEE_puts(str); Delay_ms(500);
	strcpy(str, "ATR1"); ZIGBEE_puts(str); Delay_ms(500);
	strcpy(str, "AT+NODETYPE=1"); ZIGBEE_puts(str); Delay_ms(500);

	sprintf(str, "AT+PANID=831%X", guParentAdr); ZIGBEE_puts(str); Delay_ms(500);
//	strcpy(str, "AT+PANID=8310"); ZIGBEE_puts(str); Delay_ms(500);
	strcpy(str, "AT+GROUPID=2000"); ZIGBEE_puts(str); Delay_ms(500);
	strcpy(str, "ATS11=1"); ZIGBEE_puts(str); Delay_ms(500);

	strcpy(str, "ATZ"); ZIGBEE_puts(str); Delay_ms(2000);
//	strcpy(str, "ATE1"); ZIGBEE_puts(str); Delay_ms(500);

	strcpy(str, "AT+PERMIT=30"); ZIGBEE_puts(str); Delay_ms(500);
	strcpy(str, "ATZ"); ZIGBEE_puts(str); Delay_ms(2000);

	sprintf(str, "AT+MULTICAST=2000,%X%X-Hello, World!", guParentAdr, guMyAdr);
	ZIGBEE_puts(str);
}

/* end of file */
