///////////////////////////////////////////////////////////////////////////////////
//	
///////////////////////////////////////////////////////////////////////////////////

#include "type.h"
#define	PointAddr	0x000000
#define CSAM_AUTH	0x002000
#define	GTECH_SET	0x003000
#define	FWStatus	0x004000

#define	ELogStart	0x100000	//	배출로그
#define	ELogEnd		0x200000	//	
#define	ELogLen		64

#define	EBLogStart	0x200000	//	결재로그
#define	EBLogEnd	0x600000	//	8150건
#define	EBLogLen	512

#define	TLogStart	0x600000	//	결재로그
#define	TLogEnd		0xA00000	//	8150건
#define	TLogLen		512

#define	EventStart	0xA00000	//	상태로그
#define	EventEnd	0xE00000
#define	EventLen	128

#define	GTBINStart	0xE00000

#define	FirmStart	0xFE0000

#define	MaxDataLen	1024

///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
#define SPI_FLASH_PageSize    0x100

#define WRITE      0x02  /* Write to Memory instruction */
#define WRSR       0x01  /* Write Status Register instruction */
#define WREN       0x06  /* Write enable instruction */

#define READ       0x03  /* Read from Memory instruction */
#define RDSR       0x05  /* Read Status Register instruction  */
#define RDID       0x9F  /* Read identification */
#define SE         0x20  /* Sector Erase instruction */
#define BE         0xD8  /* Block Erase instruction */
#define CE         0xC7  /* Chip Erase instruction */

#define WIP_Flag   0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte 0xA5

#define SFLASH_ID	0x1C3018	//	EN25Q128
#define SFLASH_ID0	0x1C7018
#define SFLASH_ID1	0xEF4018	//	W25Q128

///////////////////////////////////////////////////////////////////////////////////
#define	SETTBUFS	185


extern u32 TLogCnt;
extern u32 TSendCnt;

typedef struct GT_SET	{
	char SIP[4];	//	공단서버
	u16  SPort;
	
	char TIP[4];	//	결재서버
	u16  TPort;
	
	char LIP[4];	//	이통사 서버
	u16  LPort;
	
	char GIP[4];	//	G-Tech Monitor Server
	u16  GPort;
	
	char FIP[4];	//	F/W Update Server
	u16  FPort;
	
	char CIP[4];	//	.
	u16  CPort;
	
	char SMSIP[4];	//	SMS Server
	u16  SMSPort;
	
	char OPSIP[4];	//	Operation Server
	u16  OPSPort;
	
	char VMRATE;
	
	char MODE[4];	//	.
	char VMID[16];	//	.
	char VMMAKE[10];	//	.
	char VMUID[16];	//	.
	char VMFID[10];	//	.
	char VMBID[12];	//	.
	
	char AKEY[32];	//	Aria Key
	
	char OPVER[10];
	char DTESN[10];
	
//	KSCC & EBC 통합용	
	char EIP[4];	//	EB 결재서버
	u16  EPort;
	char VMFIDB[10];	//	EB ID
	
}	stGTechSet;
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void SPI_FLASH_CS_LOW(void);
void SPI_FLASH_CS_HIGH(void);

void DBG_Set(void);
void Init_Set(void);
void GetSetting(void);
void SaveSetting(void);

char CheckPayData(char *BUF);

void MemCharCpy(char* DEST, char* Buf, char LEN);

void SPI_FLASH_Init(void);
void SPI_FLASH_EraseSector(u32 SectorAddr);
void SPI_FLASH_ChipErase(void);
void SPI_FLASH_PageWrite(char* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_Write(char* pBuffer, u32 WriteAddr, u16 Opset, u16 Len);
void SPI_FLASH_BufferRead(char* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32 SPI_FLASH_ReadID(void);
void SPI_FLASH_StartReadSequence(u32 ReadAddr);

/*----- Low layer function -----*/
u8 SPI_FLASH_SendByte(unsigned char byte);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

void ClearFWAddr(void);
void SaveFWLength(u32 Len, u16 cnt);
void ClearSflashBuffer(void);
u32 GetFWLength(unsigned char ch);

u32 CheckPoint(void);
void READPoint(void);
void SAVEPoint(void);
void SaveEvent(char *BuF);

void ClearLogData(void);

void SaveLog(unsigned char *BuF, char lCnt);
void VMData(unsigned char cmd, unsigned char code, char *Buf, WORD len)	;

////////////////////////////////////////////////
//void SaveUser(unsigned int Cnt, char* Buf);
//void SaveNewUser(unsigned int Cnt, char* Buf);

void SendInputVM(void);
void SendInputServer(void);

void SendInputDBG(void);
unsigned char MyStrCmp(char *s, const char *ch, unsigned char Len);

///const char ASCII[16]; const char	AriaKEY[32];
////const char ASCII[16];
//u32 TLogCnt;
//u32 TSendCnt;
//u8  SendTime;

