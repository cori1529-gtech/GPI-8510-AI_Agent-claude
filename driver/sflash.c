
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"stm32f10x_lib.h"
#include "hw_config.h"
#include "stm32f10x_it.h"									 
#include "stm32f10x.h"
//#include	"platform_config.h"
#include	"type.h"

#include	"SFlash.h"
#include "board_gpi8510.h"
//#include	"main.h"
//#include	"M2MM.h"
//#include	"melody.h"

//#include	"COMM4.h"
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
////extern const char ASCII[16];
extern char msg[50];

//const char 	Server[4] = {211, 112, 135, 142 };	// Test Server IP
const char 	Server[4] = {61,108,48,26 };	// 20170608 지테크 요청으로 수정
const char 	VAN[4] = {210, 216, 95, 196 };		// VAN IP
const char	WebIP[4] = {61, 101, 246, 154};
const char	GTIP[4] = {112, 217, 212, 250};
const char	FIP[4] = {112, 217, 212, 250};
const char	CIP[4] = {112, 217, 212, 250};
const char	SMSIP[4] = {112, 217, 212, 250};
const char	OPSIP[4] = {112, 217, 212, 250};

stGTechSet GTechSet;

//extern stRTCTime  RTC;

const char aBLANK[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

char	SFLASH_BUFF[MaxDataLen];
u32	SPoint, EPoint;
u32	STLOG, ETLOG;
u32	SELOG, EELOG;

u32	PageCount = 0;
char msg[50];

extern u16	FWINDEX;
extern u16	FWGETCNT;
extern u32	FWGETLength;

////extern const char	AriaKEY[32];
 u32 TLogCnt;
 u32 TSendCnt;
////extern char tSendTime;
///////////////////////////////////////////////////////////////////////////////////
void DBG_Set(void)	{
	
	/*
	sprintf(msg, "SIP : %d.%d.%d.%d:%d", GTechSet.SIP[0],GTechSet.SIP[1],GTechSet.SIP[2],GTechSet.SIP[3], GTechSet.SPort);	DebugString(msg);
	sprintf(msg, "TIP : %d.%d.%d.%d:%d", GTechSet.TIP[0],GTechSet.TIP[1],GTechSet.TIP[2],GTechSet.TIP[3], GTechSet.TPort);	DebugString(msg);
	sprintf(msg, "LIP : %d.%d.%d.%d:%d", GTechSet.LIP[0],GTechSet.LIP[1],GTechSet.LIP[2],GTechSet.LIP[3], GTechSet.LPort);	DebugString(msg);
	sprintf(msg, "GIP : %d.%d.%d.%d:%d", GTechSet.GIP[0],GTechSet.GIP[1],GTechSet.GIP[2],GTechSet.GIP[3], GTechSet.GPort);	DebugString(msg);
	sprintf(msg, "FIP : %d.%d.%d.%d:%d", GTechSet.FIP[0],GTechSet.FIP[1],GTechSet.FIP[2],GTechSet.FIP[3], GTechSet.FPort);	DebugString(msg);
	sprintf(msg, "CIP : %d.%d.%d.%d:%d", GTechSet.CIP[0],GTechSet.CIP[1],GTechSet.CIP[2],GTechSet.CIP[3], GTechSet.CPort);	DebugString(msg);
	
	sprintf(msg, "SMSIP : %d.%d.%d.%d:%d", GTechSet.SMSIP[0],GTechSet.SMSIP[1],GTechSet.SMSIP[2],GTechSet.SMSIP[3], GTechSet.SMSPort);	DebugString(msg);
	sprintf(msg, "OPSIP : %d.%d.%d.%d:%d", GTechSet.OPSIP[0],GTechSet.OPSIP[1],GTechSet.OPSIP[2],GTechSet.OPSIP[3], GTechSet.OPSPort);	DebugString(msg);
	
	sprintf(msg, "VMRATE : %d", GTechSet.VMRATE);	DebugString(msg);
	DebugStrA("MODE : ");	DebugStr(GTechSet.MODE, 4);
	DebugStrA("VMID : ");	DebugStr(GTechSet.VMID, 16);
	DebugStrA("VMMAKE : ");	DebugStr(GTechSet.VMMAKE, 10);
	DebugStrA("VMUID : ");	DebugStr(GTechSet.VMUID, 16);
	DebugStrA("VMFID : ");	DebugStr(GTechSet.VMFID, 10);
	DebugStrA("VMBID : ");	DebugStr(GTechSet.VMBID, 12);
	
	DebugStrA("OPVER : ");	DebugStr(GTechSet.OPVER, 10);
	//DebugStrA("DTE S/N : ");	DebugStr(GTechSet.DTESN, 10);
*/
}

void Init_Set(void)	{
	char i;
	
	for(i=0; i<4; i++)	{
		GTechSet.SIP[i] = Server[i];
		GTechSet.TIP[i] = VAN[i];
		GTechSet.LIP[i] = WebIP[i];
		GTechSet.GIP[i] = GTIP[i];
		GTechSet.FIP[i] = FIP[i];
		GTechSet.CIP[i] = CIP[i];
		GTechSet.SMSIP[i] = SMSIP[i];
		GTechSet.OPSIP[i] = OPSIP[i];
	}
	GTechSet.SPort = 13001;
	//GTechSet.TPort = 10000;
	GTechSet.TPort = 10122;
	GTechSet.LPort = 7788;
	GTechSet.GPort = 9901;
	GTechSet.FPort = 9987;
	GTechSet.CPort = 9977;
	GTechSet.SMSPort = 1530;
	GTechSet.OPSPort = 1730;
	
	GTechSet.VMRATE = 22;
	
	memcpy(GTechSet.MODE, "REAL", 4);
	memcpy(GTechSet.VMID, "WZZ-33A-02-1    ", 16);
	//memcpy(GTechSet.VMMAKE, "GT15070001", 10);	//	20170606 삭제
	memcpy(GTechSet.VMUID, "KKRWZZE023000026", 16);
	memcpy(GTechSet.VMFID, "1111111111", 10);
	memcpy(GTechSet.VMBID, "5068300025  ", 12);
	
	memcpy(GTechSet.OPVER, "00000000  ", 10);
	
	////memcpy(GTechSet.AKEY, AriaKEY, 32);
}

void GetSetting(void)	{
	u16 Tmp;
	
	SPI_FLASH_BufferRead(SFLASH_BUFF, GTECH_SET, SETTBUFS);
	if(MyStrCmp(SFLASH_BUFF, aBLANK, 16) == 0)	{
		Init_Set();
		SaveSetting();
	}
	else	{
		memcpy(&GTechSet, SFLASH_BUFF, SETTBUFS);
	}
	
	Tmp  = ((GTechSet.VMID[4] & 0x0F) * 100);
	Tmp += ((GTechSet.VMID[5] & 0x0F) * 10);
	Tmp +=  (GTechSet.VMID[6] & 0x0F);
	
	Tmp += ((GTechSet.VMID[8] & 0x0F) * 10);
	Tmp += (GTechSet.VMID[9] & 0x0F);
	
	Tmp += (GTechSet.VMID[11] & 0x0F);
	
	////SendTime = (Tmp % 59);
}

void SaveSetting(void)	{
	SPI_FLASH_EraseSector(GTECH_SET);
	
	memcpy(SFLASH_BUFF, &GTechSet, SETTBUFS);
	SPI_FLASH_PageWrite(SFLASH_BUFF, GTECH_SET, SETTBUFS);
}

char CheckPayData(char *BUF)	{
	u16 i;
	
//DebugStrA("[Check Data] ");	DebugStr(BUF, 320);
	
	for(i=0; i<319; i++)	{
		
		if((BUF[i] > 'z') || (BUF[i] < 0x20))	{
			if(BUF[i] != 0x1C)	{
				////sprintf(msg, "[ERR Point] %d, %X", i, BUF[i]);	DebugString(msg);
				return 1;
			}
		}
	}
	
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////
void MemCharCpy(char* DEST, char* Buf, char LEN)	{
	char i;
	
	for(i=0; i<LEN; i++)	{
	////	*DEST++ = ASCII[Buf[i] >> 4];
	////	*DEST++ = ASCII[Buf[i] & 0x0F];
	}
}
///////////////////////////////////////////////////////////////////////////////////
//		User Define Function
///////////////////////////////////////////////////////////////////////////////////
/*
void SaveUser(unsigned int Cnt, char* Buf)	{
	DebugStrA("[SAVE USER] : ");
	ClearSflashBuffer();
	memcpy(SFLASH_BUFF, Buf, 44);
	
	SPI_FLASH_PageWrite(SFLASH_BUFF, (UserStart + (Cnt * UserLen)), 44);
	
	DebugString("OK");
}
void SaveNewUser(unsigned int Cnt, char* Buf)	{
	DebugStrA("[SAVE USER] : ");
	ClearSflashBuffer();
	memcpy(SFLASH_BUFF, Buf, 44);
	
	SPI_FLASH_PageWrite(SFLASH_BUFF, (NUserStart + (Cnt * UserLen)), 44);
	
	DebugString("OK");
}
*/
///////////////////////////////////////////////////////////////////////////////////
void SendInputDBG(void)	{
	u32	GetAddr;
	
	GetAddr = ELogStart;
	////DebugString("====================");
	////DebugString("START");
	////DebugString("====================");
RECHK:
	SPI_FLASH_BufferRead(SFLASH_BUFF, GetAddr, ELogLen);
	if(MyStrCmp(SFLASH_BUFF, aBLANK, 16) == 1)	{
	//	DebugChar(SFLASH_BUFF, 57);
	////	SendDBG(0x0D);
	}
	else	{
		////DebugString("====================");
		////DebugString("END");
		////DebugString("====================");
		return;
	}
	
	GetAddr += ELogLen;
	if(GetAddr >= ELogEnd)	{
		////DebugString("====================");
		///DebugString("END");
		////DebugString("====================");
		return;
	}
	goto RECHK;
}

void SendInputVM(void)	{
	u32	GetAddr;
	
	GetAddr = ELogStart;
RECHK:
	SPI_FLASH_BufferRead(SFLASH_BUFF, GetAddr, ELogLen);
	if(MyStrCmp(SFLASH_BUFF, aBLANK, 16) == 1)	{
		VMData(0xE0, 0, SFLASH_BUFF, 57);
	}
	else	{
		VMData(0xE0, 0xFF, SFLASH_BUFF, 1);
		return;
	}
	GetAddr += ELogLen;
	if(GetAddr >= ELogEnd)	{
		VMData(0xE0, 0xFF, SFLASH_BUFF, 1);
		return;
	}
	goto RECHK;
}

void SendInputServer(void)	{
	u32	GetAddr;
	
	GetAddr = ELogStart;
RECHK:
	SPI_FLASH_BufferRead(SFLASH_BUFF, GetAddr, ELogLen);
	if(MyStrCmp(SFLASH_BUFF, aBLANK, 16))	{
		//SendGServer(SFLASH_BUFF, 56);
	}
	else	{
		return;
	}
	GetAddr += ELogLen;
	if(GetAddr >= ELogEnd)	{
		return;
	}
	goto RECHK;
}

void ClearSflashBuffer(void)	{
	unsigned int i;
	
	for(i=0; i<MaxDataLen; i++)	SFLASH_BUFF[i] = 0;
}

void ClearLogData(void)	{
	unsigned int i;
	u32 fAddr;
	
	SPoint = EPoint = TLogStart;
	STLOG = ETLOG = TLogStart;
	SELOG = EELOG = EventStart;
	TLogCnt = 1;
	
	PageCount = 0;
	SPI_FLASH_EraseSector(PointAddr);
	
	fAddr = TLogStart;
	for(i=0; i<1024; i++)	{
		////SendDBG('>');
		IWDG_ReloadCounter();
		SPI_FLASH_EraseSector(fAddr);
		delay_ms(1);
		fAddr += 0x1000;
	}	////SendDBG(0x0D);
	SPI_FLASH_EraseSector(SELOG);
	SAVEPoint();
	////DebugString("LOG Data Clear OK");
}

void SaveEvent(char *BuF)	{
	u16 i;
	
	for(i=0; i<MaxDataLen; i++)	SFLASH_BUFF[i] = 0;
	
	sprintf(msg, "[SAVE Event] %04d : ", EPoint);	////DebugString(msg);
	
	memcpy(SFLASH_BUFF, GTechSet.VMID, 12);
	memcpy(&SFLASH_BUFF[12], &BuF[8], 44);
	
	SPI_FLASH_PageWrite(SFLASH_BUFF, EPoint, 56);
	
	EPoint += ELogLen;
	if(EPoint >= ELogEnd)	{
		EPoint = ELogStart;
	}
	if((EPoint % 0x1000) == 0)	{
		SPI_FLASH_EraseSector(EPoint);
	}
	SAVEPoint();
}

void SaveLog(unsigned char *BuF, char lCnt)	{
	u16 i;
	
//	sprintf(msg, "|%02d%02d%02d%02d%02d%02d|", RTC.YEAR, RTC.MONTH, RTC.DAY, RTC.HOUR, RTC.MIN, RTC.SEC);
	
	for(i=0; i<MaxDataLen; i++)	SFLASH_BUFF[i] = 0xFF;
	
	sprintf(msg, "[SAVE LOG] %04d : ", EELOG);	////DebugString(msg);
	
	memcpy(SFLASH_BUFF, msg, 14);
	memcpy(&SFLASH_BUFF[14], GTechSet.VMID, 12);
	memcpy(&SFLASH_BUFF[26], BuF, lCnt);
	
	SPI_FLASH_PageWrite(SFLASH_BUFF, EELOG, (lCnt+26));
	
	EELOG += EventLen;
	if(EELOG >= EventEnd)	{
		EELOG = EventStart;
	}
	if((EELOG % 0x1000) == 0)	{
		SPI_FLASH_EraseSector(EELOG);
	}
	SAVEPoint();
}

u32 GetFWLength(unsigned char ch)	{
u32 FwLen;
	unsigned char i;
	
	ClearSflashBuffer();
	SPI_FLASH_BufferRead(SFLASH_BUFF, FirmStart, 4);
	FwLen = 0;
	for(i=0; i<4; i++)	{
		FwLen <<= 8;
		FwLen += SFLASH_BUFF[i];
	}
	
	return FwLen;
}

void ClearFWAddr(void)	{
	unsigned char i;
	u32 addr = FirmStart;
	
	////DebugString("[Clear Firmware Memory]");
	
	for(i=0; i<16; i++)	{
		IWDG_ReloadCounter();
		SPI_FLASH_EraseSector(addr);
		addr += 0x1000;
	}
}

void SaveFWLength(u32 Len, u16 cnt)	{
	
	sprintf(msg, "F/W Length :  %d", Len);////	DebugString(msg);
	sprintf(msg, "F/W Count  :  %d", cnt);////	DebugString(msg);
	
	SFLASH_BUFF[0] = (Len >> 24) & 0xFF;
	SFLASH_BUFF[1] = (Len >> 16) & 0xFF;
	SFLASH_BUFF[2] = (Len >>  8) & 0xFF;
	SFLASH_BUFF[3] = (Len      ) & 0xFF;
	SFLASH_BUFF[4] = (cnt >> 8)  & 0xFF;
	SFLASH_BUFF[5] = (cnt     )  & 0xFF;
	
	SPI_FLASH_PageWrite(SFLASH_BUFF, FirmStart, 6);
}
///////////////////////////////////////////////////////////////////////////////////
u32 CheckPoint(void)	{
	
	u32 Point = 0;
	
CHKPT:	
	SPI_FLASH_BufferRead(SFLASH_BUFF, PointAddr + Point, 16);
	if(MyStrCmp(SFLASH_BUFF, aBLANK, 16) == 0)	{
		return Point;
	}
	Point += 64;
	if(Point >= 0x1000)	{
		return Point;
	}
	goto CHKPT;
}

void READPoint(void)	{
	unsigned char i;
	u32 ret;
	
	ret = CheckPoint();
	if(ret == 0)	{
		//SPoint = EPoint = ELogStart;
		SPoint = EPoint = TLogStart;
		STLOG = ETLOG = TLogStart;
		TLogCnt = 1;
		TSendCnt = 1;
		PageCount = 0;
		SPI_FLASH_EraseSector(PointAddr);
		for(i=0; i<64; i++)	SFLASH_BUFF[i] = 0;
		SFLASH_BUFF[1]  = ((SPoint & 0xFF0000) >> 16);
		SFLASH_BUFF[2]  = ((SPoint & 0xFF00) >> 8);
		SFLASH_BUFF[3]  = ( SPoint & 0xFF);
		
		SFLASH_BUFF[5]  = ((EPoint & 0xFF0000) >> 16);
		SFLASH_BUFF[6]  = ((EPoint & 0xFF00) >> 8);
		SFLASH_BUFF[7]  = ( EPoint & 0xFF);
		
		SFLASH_BUFF[9]  = ((STLOG & 0xFF0000) >> 16);
		SFLASH_BUFF[10]  = ((STLOG & 0xFF00) >> 8);
		SFLASH_BUFF[11]  = ( STLOG & 0xFF);
		
		SFLASH_BUFF[13]  = ((ETLOG & 0xFF0000) >> 16);
		SFLASH_BUFF[14]  = ((ETLOG & 0xFF00) >> 8);
		SFLASH_BUFF[15]  = ( ETLOG & 0xFF);
		
		SFLASH_BUFF[17]  = ((SELOG & 0xFF0000) >> 16);
		SFLASH_BUFF[18]  = ((SELOG & 0xFF00) >> 8);
		SFLASH_BUFF[19]  = ( SELOG & 0xFF);
		
		SFLASH_BUFF[21]  = ((EELOG & 0xFF0000) >> 16);
		SFLASH_BUFF[22]  = ((EELOG & 0xFF00) >> 8);
		SFLASH_BUFF[23]  = ( EELOG & 0xFF);
		
		SFLASH_BUFF[25]  = ((TLogCnt & 0xFF0000) >> 16);
		SFLASH_BUFF[26]  = ((TLogCnt & 0xFF00) >> 8);
		SFLASH_BUFF[27]  = ( TLogCnt & 0xFF);
		
		SFLASH_BUFF[29]  = ((TSendCnt & 0xFF0000) >> 16);
		SFLASH_BUFF[30]  = ((TSendCnt & 0xFF00) >> 8);
		SFLASH_BUFF[31]  = ( TSendCnt & 0xFF);
		
		SPI_FLASH_PageWrite(SFLASH_BUFF, PointAddr + PageCount, 64);
	}
	else if(ret >= 0x1000)	{
		PageCount = 0x1000 - 64;
		
		SPI_FLASH_BufferRead(SFLASH_BUFF, PointAddr + PageCount, 32);
		
		SPoint = (SFLASH_BUFF[1]  * 0x10000) + (SFLASH_BUFF[2]  * 0x100) + SFLASH_BUFF[3];
		EPoint = (SFLASH_BUFF[5]  * 0x10000) + (SFLASH_BUFF[6]  * 0x100) + SFLASH_BUFF[7];
		STLOG = (SFLASH_BUFF[9]  * 0x10000) + (SFLASH_BUFF[10]  * 0x100) + SFLASH_BUFF[11];
		ETLOG = (SFLASH_BUFF[13]  * 0x10000) + (SFLASH_BUFF[14]  * 0x100) + SFLASH_BUFF[15];
		SELOG = (SFLASH_BUFF[17]  * 0x10000) + (SFLASH_BUFF[18]  * 0x100) + SFLASH_BUFF[19];
		EELOG = (SFLASH_BUFF[21]  * 0x10000) + (SFLASH_BUFF[22]  * 0x100) + SFLASH_BUFF[23];
		TLogCnt = (SFLASH_BUFF[25]  * 0x10000) + (SFLASH_BUFF[26]  * 0x100) + SFLASH_BUFF[27];
		TSendCnt = (SFLASH_BUFF[29]  * 0x10000) + (SFLASH_BUFF[30]  * 0x100) + SFLASH_BUFF[31];
		if(SPoint < TLogStart)	{
			SPoint = STLOG;
			EPoint = ETLOG;
		}
	}
	else	{
		PageCount = ret - 64;
		
		SPI_FLASH_BufferRead(SFLASH_BUFF, PointAddr + PageCount, 32);
		
		SPoint = (SFLASH_BUFF[1]  * 0x10000) + (SFLASH_BUFF[2]  * 0x100) + SFLASH_BUFF[3];
		EPoint = (SFLASH_BUFF[5]  * 0x10000) + (SFLASH_BUFF[6]  * 0x100) + SFLASH_BUFF[7];
		STLOG = (SFLASH_BUFF[9]  * 0x10000) + (SFLASH_BUFF[10]  * 0x100) + SFLASH_BUFF[11];
		ETLOG = (SFLASH_BUFF[13]  * 0x10000) + (SFLASH_BUFF[14]  * 0x100) + SFLASH_BUFF[15];
		SELOG = (SFLASH_BUFF[17]  * 0x10000) + (SFLASH_BUFF[18]  * 0x100) + SFLASH_BUFF[19];
		EELOG = (SFLASH_BUFF[21]  * 0x10000) + (SFLASH_BUFF[22]  * 0x100) + SFLASH_BUFF[23];
		TLogCnt = (SFLASH_BUFF[25]  * 0x10000) + (SFLASH_BUFF[26]  * 0x100) + SFLASH_BUFF[27];
		TSendCnt = (SFLASH_BUFF[29]  * 0x10000) + (SFLASH_BUFF[30]  * 0x100) + SFLASH_BUFF[31];
		if(SPoint < TLogStart)	{
			SPoint = STLOG;
			EPoint = ETLOG;
		}
	}
	PageCount += 64;

//	ADD 2016.07.28		
	if(EPoint < TLogStart)	{
		EPoint = TLogStart;
	}
	if(SPoint < TLogStart)	{
		SPoint = TLogStart;
	}
	if(STLOG < TLogStart)	{
		STLOG = TLogStart;
	}
	if(ETLOG < TLogStart)	{
		ETLOG = TLogStart;
	}
	
	sprintf(msg,"PageCount = %06x", PageCount);////	DebugString(msg);
	sprintf(msg,"SPoint = %06x", SPoint);	////	DebugString(msg);
	sprintf(msg,"EPoint = %06x", EPoint);		////DebugString(msg);
	sprintf(msg,"STLOG = %06x", STLOG);		///	DebugString(msg);
	sprintf(msg,"ETLOG = %06x", ETLOG);			////DebugString(msg);
	sprintf(msg,"SELOG = %06x", SELOG);		////	DebugString(msg);
	sprintf(msg,"EELOG = %06x", EELOG);		////	DebugString(msg);
	sprintf(msg,"TLogCnt = %06x", TLogCnt);	///	DebugString(msg);
	sprintf(msg,"TSendCnt = %06x", TSendCnt);		////DebugString(msg);
}

void SAVEPoint(void)	{
	unsigned char i;
	
	for(i=0; i<64; i++)	SFLASH_BUFF[i] = 0;
	/*
	SFLASH_BUFF[1]  = ((SPoint & 0xFF0000) >> 16);
	SFLASH_BUFF[2]  = ((SPoint & 0xFF00) >> 8);
	SFLASH_BUFF[3]  = ( SPoint & 0xFF);
	
	SFLASH_BUFF[5]  = ((EPoint & 0xFF0000) >> 16);
	SFLASH_BUFF[6]  = ((EPoint & 0xFF00) >> 8);
	SFLASH_BUFF[7]  = ( EPoint & 0xFF);
	
	SFLASH_BUFF[9]  = ((STLOG & 0xFF0000) >> 16);
	SFLASH_BUFF[10]  = ((STLOG & 0xFF00) >> 8);
	SFLASH_BUFF[11]  = ( STLOG & 0xFF);
	
	SFLASH_BUFF[13]  = ((ETLOG & 0xFF0000) >> 16);
	SFLASH_BUFF[14]  = ((ETLOG & 0xFF00) >> 8);
	SFLASH_BUFF[15]  = ( ETLOG & 0xFF);
	
	SFLASH_BUFF[17]  = ((SELOG & 0xFF0000) >> 16);
	SFLASH_BUFF[18]  = ((SELOG & 0xFF00) >> 8);
	SFLASH_BUFF[19]  = ( SELOG & 0xFF);
	
	SFLASH_BUFF[21]  = ((EELOG & 0xFF0000) >> 16);
	SFLASH_BUFF[22]  = ((EELOG & 0xFF00) >> 8);
	SFLASH_BUFF[23]  = ( EELOG & 0xFF);
	
	SFLASH_BUFF[25]  = ((TLogCnt & 0xFF0000) >> 16); 
	SFLASH_BUFF[26]  = ((TLogCnt & 0xFF00) >> 8);
	SFLASH_BUFF[27]  = ( TLogCnt & 0xFF);
	
	SFLASH_BUFF[29]  = ((TSendCnt & 0xFF0000) >> 16);
	SFLASH_BUFF[30]  = ((TSendCnt & 0xFF00) >> 8);
	SFLASH_BUFF[31]  = ( TSendCnt & 0xFF);
	*/
	if(PageCount >= 0x1000)	{
		PageCount = 0;
		SPI_FLASH_EraseSector(PointAddr);
	}
	
	SPI_FLASH_PageWrite(SFLASH_BUFF, PointAddr + PageCount, 32);
	PageCount += 64;
}
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////

#ifdef  gcr1000

void SPI_FLASH_Init(void)	{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef   SPI_InitStructure;
	
	////DebugString("SFLASH INIT");
	
	/* GPIOA and SPI1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	/* Configure SPI1 pins: SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* CS pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SPI_FLASH_CS_HIGH();
	
	/* SPI1 Config */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;

	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE);
}

#endif


void SPI_FLASH_Init(void)	{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef   SPI_InitStructure;
	
	////DebugString("SFLASH INIT");
	
	/* GPIOA and SPI1 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	/* Configure SPI1 pins: SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin = SFLASH_SCK_PIN | SFLASH_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SFLASH_SCK_PORT, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin =SFLASH_MISO_PIN;  // 13 :SCK  14 :MISO  15 : MOSI
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;

	GPIO_Init(SFLASH_MISO_PORT, &GPIO_InitStructure);
	
	/* CS pin */
	GPIO_InitStructure.GPIO_Pin = SFLASH_NSS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SFLASH_NSS_PORT, &GPIO_InitStructure);
	
	SPI_FLASH_CS_HIGH();
	
	/* SPI1 Config */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	//SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;

	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	
	SPI_Cmd(SPI2, ENABLE);
}





void SPI_FLASH_CS_LOW(void)	{
	GPIO_ResetBits(SFLASH_NSS_PORT, SFLASH_NSS_PIN);
	Delay_us(2);// orignal 1	
}
void SPI_FLASH_CS_HIGH(void)	{
	Delay_us(2);
	GPIO_SetBits(SFLASH_NSS_PORT, SFLASH_NSS_PIN);
}
///////////////////////////////////////////////////////////////////////////////////
void SPI_FLASH_EraseSector(u32 SectorAddr)	{
	SPI_FLASH_WriteEnable();
	
	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(SE);

	SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
	SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
	SPI_FLASH_SendByte(SectorAddr & 0xFF);

	SPI_FLASH_CS_HIGH();
	
	SPI_FLASH_WaitForWriteEnd();
	
	delay_ms(150);	//	20170606 수정 (100 -> 150)
}

void SPI_FLASH_ChipErase(void)	{
	SPI_FLASH_WriteEnable();
	
	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(CE);
	SPI_FLASH_CS_HIGH();
	
	SPI_FLASH_WaitForWriteEnd();
}

///////////////////////////////////////////////////////////////////////////////////
void SPI_FLASH_PageWrite(char* pBuffer, u32 WriteAddr, u16 NumByteToWrite)	{
	SPI_FLASH_WriteEnable();
	
	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(WRITE);

	SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
	SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
	SPI_FLASH_SendByte(WriteAddr & 0xFF);
	
	while (NumByteToWrite--)
	{
		SPI_FLASH_SendByte(*pBuffer);
		pBuffer++;
	}
	
	SPI_FLASH_CS_HIGH();
	
	SPI_FLASH_WaitForWriteEnd();
}
void SPI_FLASH_Write(char* pBuffer, u32 WriteAddr, u16 Opset, u16 Len)	{
	u16 i;
	
	////sprintf(msg, "Flash Write Addr-%X, Len-%d", WriteAddr, Len);	DebugString(msg);
	
	SPI_FLASH_WriteEnable();
	
	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(WRITE);

	SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
	SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
	SPI_FLASH_SendByte(WriteAddr & 0xFF);
	
	for(i=0; i<Len; i++)	{
		SPI_FLASH_SendByte(pBuffer[i+Opset]);
	}
	
	SPI_FLASH_CS_HIGH();
	
	SPI_FLASH_WaitForWriteEnd();
}
///////////////////////////////////////////////////////////////////////////////////
void SPI_FLASH_BufferRead(char* pBuffer, u32 ReadAddr, u16 NumByteToRead)	{
	SPI_FLASH_CS_LOW();
	
	SPI_FLASH_SendByte(READ);
	
	SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
	SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
	SPI_FLASH_SendByte(ReadAddr & 0xFF);
	
	while (NumByteToRead--) /* while there is data to be read */
	{
		*pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
		pBuffer++;
	}
	
	SPI_FLASH_CS_HIGH();
}

u32 SPI_FLASH_ReadID(void)	{
	u32 Temp = 0;
	
	////DebugString("SPI_FLASH_ReadID");
	
	SPI_FLASH_CS_LOW();
	
 SPI_FLASH_SendByte(0x9F);
//	SPI_FLASH_SendByte(0x90);
	
	Temp = Temp + SPI_FLASH_SendByte(Dummy_Byte);	Temp <<= 8;
	Temp = Temp + SPI_FLASH_SendByte(Dummy_Byte);	Temp <<= 8;
	Temp = Temp + SPI_FLASH_SendByte(0X00);
	
	SPI_FLASH_CS_HIGH();
	
	return Temp;
}

void SPI_FLASH_StartReadSequence(u32 ReadAddr)	{
	SPI_FLASH_CS_LOW();
	
	SPI_FLASH_SendByte(READ);
	
	SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
	SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
	SPI_FLASH_SendByte(ReadAddr & 0xFF);
}

///////////////////////////////////////////////////////////////////////////////////

/*
u8 SPI_FLASH_SendByte(unsigned char byte)	{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI2, byte);
	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI2);
}
*/


u8 SPI_FLASH_SendByte(unsigned char byte)	{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI2, byte);
	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI2);
}




///////////////////////////////////////////////////////////////////////////////////
void SPI_FLASH_WriteEnable(void)	{
	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(WREN);
	SPI_FLASH_CS_HIGH();
}

void SPI_FLASH_WaitForWriteEnd(void)	{
	unsigned char FLASH_Status = 0;
	
	SPI_FLASH_CS_LOW();
	
	SPI_FLASH_SendByte(RDSR);
	
	do	{
		FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);
	}
	while ((FLASH_Status & WIP_Flag) == SET); /* Write in progress */
	
	SPI_FLASH_CS_HIGH();
}



unsigned char MyStrCmp(char *s, const char *ch, unsigned char Len)	{
	unsigned char i = 0;
	
	while(Len--)	{
		if(s[i] != ch[i])	return 1;
		i++;
	}
	return 0;
}


void VMData(unsigned char cmd, unsigned char code, char *Buf, WORD len)	{
	unsigned int i, sCRC;
	
	GPIO_ResetBits(GPIOC, GPIO_Pin_2);
	//SendUART4(STX);	Curr_CRC = 0;
	//SendUART4(cmd);
	//SendUART4(len >> 8);	SendUART4(len & 0xFF);
	//SendUART4(code);
	
	for(i=0; i<(len-1); i++)	{
		//SendUART4(Buf[i]);
	}	
	
////	sCRC = Curr_CRC;
////	SendUART4(sCRC >> 8);	SendUART4(sCRC & 0xFF);
////	SendUART4(ETX);
	GPIO_ResetBits(GPIOC, GPIO_Pin_7);
}

