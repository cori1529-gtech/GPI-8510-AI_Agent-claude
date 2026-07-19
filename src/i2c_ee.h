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


//2021년 3월 3일 남은 주소 : 196




#define ADR_gxhTotalOutput         0  // 2020년 12월20일 총배출량
#define ADR_IpAdresss_1             4   //     1일전 Month 
#define ADR_v_adc_org                 8 
#define ADR_v_division                 12 //
#define ADR_v_capacity                16// 상한무게 설정 
#define ADR_IpAdresss_2             20   //     1일전 Month 
#define ADR_v_span                          24    //  빈통무게 설정 
#define ADR_gxhWeightBackup          28   //  빈통무게 설정 
#define ADR_gxhFilter                        32    //  빈통무게 설정 
#define ADR_gxhMotionBand             36    //  빈통무게 설정 
#define ADR_gnlSendingInterval          40   //  이더넷을 통하여 보내는 정보시간 주기 (1분 ~60분)
#define ADR_gxhEndDelay                  44    //  빈통무게 설정 
#define ADR_gxhMyAddress                48    //  빈통무게 설정 
#define ADR_gxhBaudRate                   52    //  빈통무게 설정 
#define ADR_gxuPulseSetWeigh           56   //  빈통무게 설정 
#define ADR_gxuOutputErrorWeigh        60    //  빈통무게 설정 
#define ADR_gxuOutputHighErrorWeigh  64    //  빈통무게 설정 
#define ADR_gnlDailyTotal                    68   //  빈통무게 설정 
#define ADR_gnlPreDailyTotal_0            72    //  빈통무게 설정 
#define ADR_gnlPreDailyTotal_1            76    //  빈통무게 설정 
#define ADR_gnlPreDailyTotal_2            80    //  빈통무게 설정 
#define ADR_gnlPreDailyTotal_3           84    //  빈통무게 설정 
#define ADR_gnlPreDailyTotal_4           88    //  빈통무게 설정 
#define ADR_gnlPreDailyTotal_5           92    //  빈통무게 설정 
#define ADR_gnlPreDailyTotal_6           96   //  빈통무게 설정 
#define ADR_gnuDay                          100   //사료준날로부터 일령
#define ADR_IpAdresss_3                  104   //     1일전 Month 
#define ADR_IpAdresss_4                  108   //     1일전 Month 
#define ADR_IpPort                           112   //     1일전 Month 
#define ADR_Real_Weight_Factor2      116  //  1529 
#define ADR_gnlLanguage                    120   //  silo2 selction : 1 (enable), 0(disable)
#define ADR_gplCompensationWeight  124        //  빈통무게 설정
#define ADR_gplCompensationWeight2  128        //  빈통무게 설정
#define ADR_v_zero                132
#define ADR_gnfFactor             136
#define ADR_Real_Weight_Factor     140   //  1529 
#define ADR_gnlDisplay              144   //  1 : 모니터링값 보이기   0: 보이지 않기  
#define ADR_gnlSerialNo				148 //장비 시리얼 번호
#define ADR_gnlMyAddress			152	//장비 ID(어드레스)

///////#define ADR_gnlHenhouseNo			156	//계사 번호


#define ADR_gnlUseLTE				160	//LTE 사용여부 (0:ZIGBEE, 1:LTE)
#define ADR_gxwStatusSendInterval	164	//상태정보 전송 간격(분)
#define ADR_gnlLcCompensation		168	//로드셀 죽었을 때 보상 ON/OFF

/***********gpi-8510**************************/

#define ADR_gnfFactor_2          172
#define ADR_v_zero_2                  176
#define ADR_v_adc_org_2             180 
#define ADR_v_division_2              184
#define ADR_v_capacity_2             188    // 상한무게 설정 
#define ADR_v_span_2                  192    //  빈통무게 설정 
                                                             //196



/***********gpi-8510**************************/

#define ADR_gnlPreDailyTotal2_0     200    //  빈통무게 설정 
#define ADR_gnlPreDailyTotal2_1     204    //  빈통무게 설정 
#define ADR_gnlPreDailyTotal2_2     208    //  빈통무게 설정 
#define ADR_gnlPreDailyTotal2_3     212    //  빈통무게 설정 
#define ADR_gnlPreDailyTotal2_4     216    //  빈통무게 설정 
#define ADR_gnlPreDailyTotal2_5     220    //  빈통무게 설정 
#define ADR_gnlPreDailyTotal2_6     224   //  빈통무게 설정 
#define ADR_gnuDay2                 228   //사료준날로부터 일령
#define ADR_gnlDailyTotal2          232   //  빈통무게 설정 
#define ADR_gnlCdmaUse               236   // CDAM or RS-422 
#define ADR_gnlSilo1Selection            240   //  silo1 selction : 1 (enable), 0(disable)
#define ADR_gnlSilo2Selection            244   //  silo2 selction : 1 (enable), 0(disable)
#define ADR_gnlLowerOutputError            248   //  silo2 selction : 1 (enable), 0(disable)
#define ADR_gnlUpperOutputError            252   //  silo2 selction : 1 (enable), 0(disable)
#define ADR_gnlPulseWeight                 256   //  silo2 selction : 1 (enable), 0(disable)


// 급이 표시 -> 급이는 한 cycle 관련된 내용만 표시한다.   156, 196

#define ADR_gnlFeeding1  156 // silo 1번의 한번 배출시의 급이량
#define ADR_gnlFeeding2  196 // silo 2번의 한번 배출시의 급이량





/*
signed long gnlLowerOutputError ; //         248   //  silo2 selction : 1 (enable), 0(disable)
 signed long gnlUpperOutputError ; //         252   //  silo2 selction : 1 (enable), 0(disable)
 signed long gnlPulseWeight   ;    //          256   //  silo2 selction : 1 (enable), 0(disable)
 signed long gnlLanguage   ;    //          256   //  silo2 selction : 1 (enable), 0(disable)
 */


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
#define DEFAULT_gnlLcCompensation		0	//로드셀 죽었을 때 보상 ON/OFF

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
#define MIN_gnlLcCompensation		0	//로드셀 죽었을 때 보상 ON/OFF

#define	MAX_gxhWeightBackup			1
#define	MAX_gxhEndDelay				500
#define	MAX_gplCompensationWeight	v_capacity
#define	MAX_gnlSms_Send_Time_First	23
#define	MAX_gnlSms_Send_Time_Second	23
#define	MAX_gnlSms_Send_Time_Third	23
#define	MAX_Loadcell_Number			6
#define	MAX_Real_Weight_Factor	  	13000
#define	MAX_gnlDisplay			  	1
#define	MAX_gnlSerialNo			  	9999999  ///// M1706001
#define	MAX_gnlMyAddress			999
#define	MAX_gnlHenhouseNo			99
#define	MAX_gnlUseLTE				1
#define MAX_gxwStatusSendInterval	60	//상태정보 전송 간격(분)
#define MAX_gnlLcCompensation		1	//로드셀 죽었을 때 보상 ON/OFF




//#define ADR_gnlFeeding1  156 // silo 1번의 한번 배출시의 급이량
//#define ADR_gnlFeeding2  196 // silo 2번의 한번 배출시의 급이량

extern long gnlFeeding1;
extern long gnlFeeding2;


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
extern long Real_Weight_Factor2;   //  1529



extern signed long gnlSilo1Selection;
extern signed long gnlSilo2Selection;

extern signed long gnlLowerOutputError ; //         248   //  silo2 selction : 1 (enable), 0(disable)
extern signed long gnlUpperOutputError ; //         252   //  silo2 selction : 1 (enable), 0(disable)
extern signed long gnlPulseWeight   ;    //          256   //  silo2 selction : 1 (enable), 0(disable)
extern signed long gnlLanguage;
extern  long gnlCdmaUse;
extern long gnlSendingInterval;

extern long gxhTotalOutput; // 2021년 3월 3일 총누적배출량

extern long IpAdresss_1;   //     
extern long IpAdresss_2;   //     
extern long IpAdresss_3;   //     
extern long IpAdresss_4;   //     
extern long IpPort;


//extern long gxuOutputErrorWeigh;
//extern long gxuOutputHighErrorWeigh;
//extern long gxuPulseSetWeigh;


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
void write_factors(void); //silo_1 calibration
void write_factors_2(void); // silo_2  calibration




void write_set_factors(void);
void read_factors(void);

void Data_Read(void); //좌측통 calibration
void Initiaize_eeprom(void);


//void write_Record(void);// 배출기록  




#endif /* __I2C_EE_H */

/* end of file */
