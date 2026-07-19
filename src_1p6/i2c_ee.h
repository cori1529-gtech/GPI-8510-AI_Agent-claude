/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : i2c_ee.h
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : Header for i2c_ee.c module
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __I2C_EE_H
#define __I2C_EE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"
#include "hw_config.h"

#define ADR_gnfFactor             0
#define ADR_v_zero                4 
#define ADR_v_adc_org             8 

#define ADR_v_division           12 //

#define ADR_v_capacity           16// 상한무게 설정 
#define ADR_v_span               24    //  빈통무게 설정 

#define ADR_gxhWeightBackup       28   //  빈통무게 설정 
#define ADR_gxhFilter             32    //  빈통무게 설정 
#define ADR_gxhMotionBand         36    //  빈통무게 설정 
#define ADR_gxhEndDelay           44    //  빈통무게 설정 
#define ADR_gxhMyAddress          48    //  빈통무게 설정 
#define ADR_gxhBaudRate           52    //  빈통무게 설정 

#define ADR_gxuPulseSetWeigh      56   //  빈통무게 설정 
#define ADR_gxuOutputErrorWeigh   60    //  빈통무게 설정 
#define ADR_gxuOutputHighErrorWeigh  64    //  빈통무게 설정 
#define ADR_gplCompensationWeight     68    //  빈통무게 설정 
#define ADR_gnlDailyTotal            72    //  빈통무게 설정 
#define ADR_gnlPreDailyTotal_0     80    //  빈통무게 설정 
#define ADR_gnlPreDailyTotal_1      84    //  빈통무게 설정 
#define ADR_gnlPreDailyTotal_2      88    //  빈통무게 설정 
#define ADR_gnlPreDailyTotal_3      92    //  빈통무게 설정 
#define ADR_gnlPreDailyTotal_4      96    //  빈통무게 설정 
#define ADR_gnlPreDailyTotal_5      100    //  빈통무게 설정 
#define ADR_gnlPreDailyTotal_6      104   //  빈통무게 설정 
#define ADR_gnuDay                   108   //사료준날로부터 일령

#define ADR_Tel_Number_1             108   //  010 
#define ADR_Tel_Number_2                  112   //  3443 
#define ADR_Tel_Number_3                   116   //  1529 

#define ADR_gnlSms_Send_Time               120   //  1529 

#define ADR_gnlSms_Send_Time_First                124   //  1529 
#define ADR_gnlSms_Send_Time_Second               128   //  1529 
#define ADR_gnlSms_Send_Time_Third                132   //  1529
#define ADR_Loadcell_Number               136   //  1529 

#define ADR_Real_Weight_Factor              140   //  1529 

#define ADR_gnlDisplay              144   //  1 : 모니터링값 보이기   0: 보이지 않기  

#define ADR_gnlSerialNo				148 //장비 시리얼 번호
#define ADR_gnlMyAddress			152	//장비 ID(어드레스)
#define ADR_gnlHenhouseNo			156	//계사 번호
#define ADR_gnlUseLTE				160	//LTE 사용여부 (0:ZIGBEE, 1:LTE)

#define ADR_gxwStatusSendInterval	164	//상태정보 전송 간격(분)

 
//extern long Loadcell_Number;

#define	DEFAULT_gxhWeightBackup			1
#define	DEFAULT_gxhEndDelay				50
#define	DEFAULT_gplCompensationWeight	0
#define	DEFAULT_gnlSms_Send_Time_First	0
#define	DEFAULT_gnlSms_Send_Time_Second	0
#define	DEFAULT_gnlSms_Send_Time_Third	0
#define	DEFAULT_Loadcell_Number			4
#define	DEFAULT_Real_Weight_Factor	  	10000
#define	DEFAULT_gnlDisplay			  	1
#define	DEFAULT_gnlSerialNo			  	0
#define	DEFAULT_gnlMyAddress			0
#define	DEFAULT_gnlHenhouseNo			0
#define	DEFAULT_gnlUseLTE				0
#define DEFAULT_gxwStatusSendInterval	60	//상태정보 전송 간격(분)


#define	MIN_gxhWeightBackup			0
#define	MIN_gxhEndDelay				0
#define	MIN_gplCompensationWeight	0
#define	MIN_gnlSms_Send_Time_First	0
#define	MIN_gnlSms_Send_Time_Second	0
#define	MIN_gnlSms_Send_Time_Third	0
#define	MIN_Loadcell_Number			4
#define	MIN_Real_Weight_Factor	  	7000
#define	MIN_gnlDisplay			  	0
#define	MIN_gnlSerialNo			  	0
#define	MIN_gnlMyAddress			0
#define	MIN_gnlHenhouseNo			0
#define	MIN_gnlUseLTE				0
#define MIN_gxwStatusSendInterval	1	//상태정보 전송 간격(분)

#define	MAX_gxhWeightBackup			1
#define	MAX_gxhEndDelay				500
#define	MAX_gplCompensationWeight	v_capacity
#define	MAX_gnlSms_Send_Time_First	23
#define	MAX_gnlSms_Send_Time_Second	23
#define	MAX_gnlSms_Send_Time_Third	23
#define	MAX_Loadcell_Number			6
#define	MAX_Real_Weight_Factor	  	13000
#define	MAX_gnlDisplay			  	1
#define	MAX_gnlSerialNo			  	9999999
#define	MAX_gnlMyAddress			999
#define	MAX_gnlHenhouseNo			99
#define	MAX_gnlUseLTE				1
#define MAX_gxwStatusSendInterval	60	//상태정보 전송 간격(분)


extern long Tel_Number_1;//     010
extern long Tel_Number_2;//     3443 
extern long Tel_Number_3;//     1529 
extern long gnlSms_Send_Time;

extern long gnlSms_Send_Time_First;
extern long gnlSms_Send_Time_Second;
extern long gnlSms_Send_Time_Third;

extern long Loadcell_Number;
//extern #define ADR_Real_Weight_Factor              140   //  1529 
extern long Real_Weight_Factor;   //  1529 
//extern long gnlDisplay;


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* The M24C08W contains 4 blocks (128byte each) with the adresses below: E2 = 0 */
/* EEPROM Addresses defines */
//#define EEPROM_Block0_ADDRESS 0xA0   /* E2 = 0 */
#define EEPROM_Block1_ADDRESS 0xA2 /* E2 = 0 */
//#define EEPROM_Block2_ADDRESS 0xA4 /* E2 = 0 */
//#define EEPROM_Block3_ADDRESS 0xA6 /* E2 = 0 */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void I2C_EE_Init(void);
void I2C_EE_ByteWrite(u8* pBuffer, u8 WriteAddr);

void I2C_EE_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite);
void I2C_EE_PageWrite_New(u16* pBuffer, u16 WriteAddr, u16 NumByteToWrite);


void I2C_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite);
void I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);

void I2C_EE_BufferWrite_New(u16* pBuffer, u16 WriteAddr, u16 NumByteToWrite);
//void I2C_EE_BufferRead_New(u16* pBuffer, u16 ReadAddr, u16 NumByteToRead);



void I2C_EE_WaitEepromStandbyState(void);
void write_factors(void); //좌측통 calibration



void write_set_factors(void);
void read_factors(void);

void Data_Read(void); //좌측통 calibration
void Initiaize_eeprom(void);


//void write_Record(void);// 배출기록  




#endif /* __I2C_EE_H */

/* end of file */
