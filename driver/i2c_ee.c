/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : i2c_ee.c
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : This file provides a set of functions needed to manage the
*                      communication between I2C peripheral and I2C M24C08 EEPROM.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "i2c_ee.h"
//#include "cal.h"
//#include "main.h" 
//#include "key.h"
//#include "lcd.h"
//#include "platform_config.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_lib.h"
#include "stm32f10x_i2c.h"

#include "hw_config.h"
#include "timer.h"

#include "def.h"

//#include "main.h"
//#include "LCD.h"
//#include "key.h"
#include "CAL.h"
#include "var.h"
//#include "rtc.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2C_Speed              100000
//#define I2C_Speed              400000
//#define I2C_Speed              50000

#define I2C1_SLAVE_ADDRESS7    0xA0
#define I2C_PageSize           16

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u16 EEPROM_ADDRESS;
 long Tel_Number_1;//     01
 long Tel_Number_2;//     3443 
 long Tel_Number_3;//     1529 
 long gnlSms_Send_Time;

 long gnlSms_Send_Time_First;
 long gnlSms_Send_Time_Second;
 long gnlSms_Send_Time_Third;
 long Loadcell_Number;
 long Real_Weight_Factor;   //  1529 
 long Real_Weight_Factor2;   //  1529 
 
 signed long gnlSilo1Selection;
 signed long  gnlSilo2Selection;
 
 signed long gnlLowerOutputError ; //         248   //  silo2 selction : 1 (enable), 0(disable)
 signed long gnlUpperOutputError ; //         252   //  silo2 selction : 1 (enable), 0(disable)
 signed long gnlPulseWeight   ;    //          256   //  silo2 selction : 1 (enable), 0(disable)
 signed long gnlLanguage   ;    //          256   //  silo2 selction : 1 (enable), 0(disable)
 long gnlCdmaUse;  // 1 : use   0: rs-422
 long gnlSendingInterval;
 
 long gxhTotalOutput; // 2020년 12월 2일 총누적배출량
 
  long IpAdresss_1;   //     
 long IpAdresss_2;   //     
 long IpAdresss_3;   //     
 long IpAdresss_4;   //     
 long IpPort;
 
 
  long gnlFeeding1;
  long gnlFeeding2;


 

 //long gxuOutputErrorWeigh;
 //long gxuOutputHighErrorWeigh;

// long gxuPulseSetWeigh;


 
 
 
 
// extern long gnlSilo1Selection;
//extern long gnlSilo2Selection;






/* Private function prototypes -----------------------------------------------*/
void I2C_Configuration(void);

u32 EE_Write_val(int adr, u32 val, u8 bytes);
u32 EE_Read_val(int adr, u8 bytes);


/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configure the used I/O ports pin
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

 long power(int x, char n)
{
	long p;

	if (n == 0) return (long)1;
	p = (long)x;
	while(--n) p = p * x;
	return p;
} /* of DWORD power(int x, byte n) */


/*******************************************************************************
* Function Name  : I2C_Configuration
* Description    : I2C Configuration
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_Configuration(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = I2C1_SLAVE_ADDRESS7;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  
  /* Apply I2C configuration after enabling it */
  I2C_Init(I2C1, &I2C_InitStructure);

  /* I2C Peripheral Enable */
  I2C_Cmd(I2C1, ENABLE);
}

/*******************************************************************************
* Function Name  : I2C_EE_Init
* Description    : Initializes peripherals used by the I2C EEPROM driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_EE_Init()
{
	/* GPIO configuration */
//	bsp_init_gpio();	//AT24C16 EEPROM

	/* I2C configuration */
	I2C_Configuration();

  /* depending on the EEPROM Address selected in the i2c_ee.h file */
#ifdef EEPROM_Block0_ADDRESS
  /* Select the EEPROM Block0 to write on */
  EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
#endif
#ifdef EEPROM_Block1_ADDRESS
  /* Select the EEPROM Block1 to write on */
  EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;
#endif
#ifdef EEPROM_Block2_ADDRESS
  /* Select the EEPROM Block2 to write on */
  EEPROM_ADDRESS = EEPROM_Block2_ADDRESS;
#endif
#ifdef EEPROM_Block3_ADDRESS
  /* Select the EEPROM Block3 to write on */
  EEPROM_ADDRESS = EEPROM_Block3_ADDRESS;
#endif
}
 //#define EEPROM_Block1_ADDRESS 0xA2
/*******************************************************************************
* Function Name  : I2C_EE_BufferWrite
* Description    : Writes buffer of data to the I2C EEPROM.
* Input          : - pBuffer : pointer to the buffer  containing the data to be 
*                    written to the EEPROM.
*                  - WriteAddr : EEPROM's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the EEPROM.
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % I2C_PageSize;
  count = I2C_PageSize - Addr;
  NumOfPage =  NumByteToWrite / I2C_PageSize;
  NumOfSingle = NumByteToWrite % I2C_PageSize;
 
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize); 
    	I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;
      }

      if(NumOfSingle!=0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage== 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / I2C_PageSize;
      NumOfSingle = NumByteToWrite % I2C_PageSize;	
      
      if(count != 0)
      {  
        I2C_EE_PageWrite(pBuffer, WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;  
      }
      if(NumOfSingle != 0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }  
}


void I2C_EE_BufferWrite_New(u16* pBuffer, u16 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % I2C_PageSize;
  count = I2C_PageSize - Addr;
  NumOfPage =  NumByteToWrite / I2C_PageSize;
  NumOfSingle = NumByteToWrite % I2C_PageSize;
 
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) 
    {
      I2C_EE_PageWrite_New(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)
      {
        I2C_EE_PageWrite_New(pBuffer, WriteAddr, I2C_PageSize); 
    	I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;
      }

      if(NumOfSingle!=0)
      {
        I2C_EE_PageWrite_New(pBuffer, WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage== 0) 
    {
      I2C_EE_PageWrite_New(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / I2C_PageSize;
      NumOfSingle = NumByteToWrite % I2C_PageSize;	
      
      if(count != 0)
      {  
        I2C_EE_PageWrite_New(pBuffer, WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        I2C_EE_PageWrite_New(pBuffer, WriteAddr, I2C_PageSize);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;  
      }
      if(NumOfSingle != 0)
      {
        I2C_EE_PageWrite_New(pBuffer, WriteAddr, NumOfSingle); 
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }  
}


/*******************************************************************************
* Function Name  : I2C_EE_ByteWrite
* Description    : Writes one byte to the I2C EEPROM.
* Input          : - pBuffer : pointer to the buffer  containing the data to be 
*                    written to the EEPROM.
*                  - WriteAddr : EEPROM's internal address to write to.
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_EE_ByteWrite(u8* pBuffer, u8 WriteAddr)
{
  /* Send STRAT condition */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));  

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
      
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C1, WriteAddr);
  
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send the byte to be written */
  I2C_SendData(I2C1, *pBuffer); 
   
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send STOP condition */
  I2C_GenerateSTOP(I2C1, ENABLE);
}

/*******************************************************************************
* Function Name  : I2C_EE_PageWrite
* Description    : Writes more than one byte to the EEPROM with a single WRITE
*                  cycle. The number of byte can't exceed the EEPROM page size.
* Input          : - pBuffer : pointer to the buffer containing the data to be 
*                    written to the EEPROM.
*                  - WriteAddr : EEPROM's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the EEPROM.
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_EE_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite)
{
  /* Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); 
  
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  

  /* Send the EEPROM's internal address to write to */    
  I2C_SendData(I2C1, WriteAddr);  

  /* Test on EV8 and clear it */
  while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* While there is data to be written */
  while(NumByteToWrite--)  
  {
    /* Send the current byte */
    I2C_SendData(I2C1, *pBuffer); 

    /* Point to the next byte to be written */
    pBuffer++; 
  
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  }

  /* Send STOP condition */
  I2C_GenerateSTOP(I2C1, ENABLE);
}


void I2C_EE_PageWrite_New(u16* pBuffer, u16 WriteAddr, u16 NumByteToWrite)
{
  /* Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); 
  
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  

  /* Send the EEPROM's internal address to write to */    
  I2C_SendData(I2C1, WriteAddr);  

  /* Test on EV8 and clear it */
  while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* While there is data to be written */
  while(NumByteToWrite--)  
  {
    /* Send the current byte */
    I2C_SendData(I2C1, *pBuffer); 

    /* Point to the next byte to be written */
    pBuffer++; 
  
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  }

  /* Send STOP condition */
  I2C_GenerateSTOP(I2C1, ENABLE);
}






/*******************************************************************************
* Function Name  : I2C_EE_BufferRead
* Description    : Reads a block of data from the EEPROM.
* Input          : - pBuffer : pointer to the buffer that receives the data read 
*                    from the EEPROM.
*                  - ReadAddr : EEPROM's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the EEPROM.
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{  
//USART1_puts("EE_Write_val_0110");


  /* Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);
 
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

//USART1_puts("EE_Write_val_0120");

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

//USART1_puts("EE_Write_val_0130");
  
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(I2C1, ENABLE);

  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C1, ReadAddr);  

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
//USART1_puts("EE_Write_val_0140");
  
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
  
//USART1_puts("EE_Write_val_0150");
  
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  
//USART1_puts("EE_Write_val_0160");
  
  /* While there is data to be read */
  while(NumByteToRead)  
  {
//USART1_puts("EE_Write_val_0162");
  
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C1, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(I2C1, ENABLE);

//USART1_puts("EE_Write_val_0163");

    }

//USART1_puts("EE_Write_val_0164");
  
    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {      
      /* Read a byte from the EEPROM */
      *pBuffer = I2C_ReceiveData(I2C1);

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;        

//USART1_puts("EE_Write_val_0165");

    }   

//USART1_puts("EE_Write_val_0168");
  
  }

//USART1_puts("EE_Write_val_0180");
  
  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C1, ENABLE);

//USART1_puts("EE_Write_val_0190");

}

/*******************************************************************************
* Function Name  : I2C_EE_WaitEepromStandbyState
* Description    : Wait for EEPROM Standby state
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_EE_WaitEepromStandbyState(void)      
{
  vu16 SR1_Tmp = 0;
 
  do
  {
    // Send START condition 
    I2C_GenerateSTART(I2C1, ENABLE);
    // Read I2C1 SR1 register 
    SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
    //Send EEPROM address for write 
    I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  }while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));
  
  // Clear AF flag 
  I2C_ClearFlag(I2C1, I2C_FLAG_AF);
	
	

   I2C_GenerateSTOP(I2C1, ENABLE);	 	//ADD 2016.4.27
	
}

/*
 #define ADR_gnfFactor             0
#define ADR_v_zero                4 
#define ADR_v_adc_org             8  

#define ADR_Bulk_Enable   12 // 부피센서 체크 enable 1:   disable :0
#define ADR_Bulk_Weight   16 // 부피센서 enable 시 연동되는 무게  
#define ADR_Upper_Weight  20// 상한무게  
 */
void read_factors()
{
 //  union { u32 dw; u16 u[2];} temp;
  float facter;
  union { u32 dw; u16 u[2]; u8 c[4];} temp;
  //Initiaize_eeprom();
//  FSMC_DISABLE;	

/// I2C_EE_BufferRead( temp.c, ADR_v_zero, 4);
/// v_zero=temp.dw;
	v_zero = EE_Read_val( ADR_v_zero, 4);	//zero a/d 값

//  I2C_EE_BufferRead( temp.c, ADR_v_span, 4);
//  v_span=temp.dw;
	v_span = EE_Read_val( ADR_v_span, 4);
  
//  I2C_EE_BufferRead( temp.c, ADR_v_capacity, 4);
//	v_capacity=temp.dw;
	v_capacity = EE_Read_val( ADR_v_capacity, 4);
if(v_capacity<0 || v_capacity>999999)
	 v_capacity=15000;
	
	
	
//  I2C_EE_BufferRead( temp.c, ADR_v_division, 4);
//  v_division=temp.dw;
	v_division = EE_Read_val( ADR_v_division, 4);
  
//  I2C_EE_BufferRead( temp.c, ADR_gnfFactor, 4);
//  v_zero=temp.dw;
	temp.dw = EE_Read_val( ADR_gnfFactor, 4);
  	gnfFactor = (float)temp.dw / (float)power(10, 9);


	v_zero_2 = EE_Read_val( ADR_v_zero_2, 4);	//zero a/d 값

//  I2C_EE_BufferRead( temp.c, ADR_v_span, 4);
//  v_span=temp.dw;
	v_span_2 = EE_Read_val( ADR_v_span_2, 4);
  
//  I2C_EE_BufferRead( temp.c, ADR_v_capacity, 4);
//	v_capacity=temp.dw;
	v_capacity_2 = EE_Read_val( ADR_v_capacity_2, 4);

 if(v_capacity_2<0 || v_capacity_2>999999)
	 v_capacity_2=15000;



//  I2C_EE_BufferRead( temp.c, ADR_v_division, 4);
//  v_division=temp.dw;
	v_division_2 = EE_Read_val( ADR_v_division_2, 4);
  
//  I2C_EE_BufferRead( temp.c, ADR_gnfFactor, 4);
//  v_zero=temp.dw;
	temp.dw = EE_Read_val( ADR_gnfFactor_2, 4);
  	gnfFactor_2 = (float)temp.dw / (float)power(10, 9);



//gnlDailyTotal = EE_Write_val(ADR_gnlDailyTotal, TodayTotalref_result, 4); Delay_ms(10);




	gxhWeightBackup = EE_Read_val( ADR_gxhWeightBackup, 4);
	if((gxhWeightBackup < MIN_gxhWeightBackup) || (MAX_gxhWeightBackup < gxhWeightBackup))  gxhWeightBackup = DEFAULT_gxhWeightBackup;

  gxhFilter = EE_Read_val( ADR_gxhFilter, 4);
	if((gxhFilter < 1) || (gxhFilter > 50))  gxhFilter = 20;

  gxhEndDelay = EE_Read_val( ADR_gxhEndDelay, 4);
	if((gxhEndDelay < MIN_gxhEndDelay) || (MAX_gxhEndDelay < gxhEndDelay))  gxhEndDelay = DEFAULT_gxhEndDelay;
  
  gnlSilo1Selection = EE_Read_val( ADR_gnlSilo1Selection, 4);
	if((gnlSilo1Selection <1) || (gnlSilo1Selection > 2))  gnlSilo1Selection = 1;
	
	gplCompensationWeight = EE_Read_val( ADR_gplCompensationWeight, 4);
	if((gplCompensationWeight < MIN_gplCompensationWeight) || (MAX_gplCompensationWeight < gplCompensationWeight))  gplCompensationWeight = DEFAULT_gplCompensationWeight;

	gplCompensationWeight2 = EE_Read_val( ADR_gplCompensationWeight2, 4);
	if((gplCompensationWeight2 < MIN_gplCompensationWeight) || (MAX_gplCompensationWeight < gplCompensationWeight2))  gplCompensationWeight2 = DEFAULT_gplCompensationWeight;

  


  Real_Weight_Factor = EE_Read_val( ADR_Real_Weight_Factor, 4);
	if((Real_Weight_Factor < MIN_Real_Weight_Factor) || (MAX_Real_Weight_Factor < Real_Weight_Factor))  Real_Weight_Factor = DEFAULT_Real_Weight_Factor;
//	Real_Weight_Factor=9000;
	
  Real_Weight_Factor2 = EE_Read_val( ADR_Real_Weight_Factor2, 4);
	if((Real_Weight_Factor2 < MIN_Real_Weight_Factor) || (MAX_Real_Weight_Factor < Real_Weight_Factor))  Real_Weight_Factor2 = DEFAULT_Real_Weight_Factor;
	
  //Real_Weight_Factor2=9000;


	gxwStatusSendInterval = EE_Read_val( ADR_gxwStatusSendInterval, 4);
	if((gxwStatusSendInterval < MIN_gxwStatusSendInterval) || (MAX_gxwStatusSendInterval < gxwStatusSendInterval))  gxwStatusSendInterval = DEFAULT_gxwStatusSendInterval;

	
	
	gnlLowerOutputError = EE_Read_val( ADR_gnlLowerOutputError, 4);
	if((gnlLowerOutputError < 10) || (500 < gnlLowerOutputError))  gnlLowerOutputError = 0; 
	
  
	gnlUpperOutputError = EE_Read_val( ADR_gnlUpperOutputError, 4);
	if((gnlUpperOutputError < 100) || (10000 < gnlUpperOutputError))  gnlUpperOutputError = 0; 
	

 	gnlPulseWeight = EE_Read_val( ADR_gnlPulseWeight, 4);
	if((gnlPulseWeight < 1) || (50 < gnlPulseWeight))  gnlPulseWeight = 10; 

  
	 	gnlLanguage = EE_Read_val( ADR_gnlLanguage, 4);
	if((gnlLanguage < 0) || (1 < gnlLanguage))  gnlLanguage = 1; 


//gnlCdmaUse=EE_Write_val( ADR_gnlCdmaUse, gnlCdmaUse, 4); Delay_ms(20);//계사 번호
   	gnlCdmaUse = EE_Read_val( ADR_gnlCdmaUse, 4);
	if((gnlCdmaUse < 0) || (1 < gnlCdmaUse))  gnlCdmaUse = 1; 

  	gxuPulseSetWeigh = EE_Read_val( ADR_gxuPulseSetWeigh, 4);
	if((gxuPulseSetWeigh < 0) || (99 < gxuPulseSetWeigh))  gxuPulseSetWeigh = 10; 

///gxuOutputErrorWeigh
 
   	gxuOutputErrorWeigh = EE_Read_val( ADR_gxuOutputErrorWeigh, 4);
	  if(gxuOutputErrorWeigh>1000)
			gxuOutputErrorWeigh=0;

		
		gxuOutputHighErrorWeigh = EE_Read_val( ADR_gxuOutputHighErrorWeigh, 4);
	  if(gxuOutputHighErrorWeigh>80000)
			gxuOutputHighErrorWeigh=0;

		
		
		
/*
	if (30000 < gxuOutputErrorWeigh)
	{
		gxuOutputErrorWeigh = 5;
		eeprom_error(ADR_gxuOutputErrorWeigh, (UINT)gxuOutputErrorWeigh);
	}
  */


//I2C_EE_BufferRead( temp.c, ADR_gnlDisplay, 4);
	//gnlDisplay=temp.dw;
	gnlDisplay = EE_Read_val( ADR_gnlDisplay, 4);
	if((gnlDisplay < MIN_gnlDisplay) || (MAX_gnlDisplay < gnlDisplay))  gnlDisplay = DEFAULT_gnlDisplay;

	gnlSerialNo = EE_Read_val( ADR_gnlSerialNo, 4);	//장비 시리얼 번호
	if((gnlSerialNo < MIN_gnlSerialNo) || (MAX_gnlSerialNo < gnlSerialNo))  gnlSerialNo = DEFAULT_gnlSerialNo;

	gnlMyAddress = EE_Read_val( ADR_gnlMyAddress, 4);	//장비 ID(어드레스)
	if((gnlMyAddress < MIN_gnlMyAddress) || (MAX_gnlMyAddress < gnlMyAddress))  gnlMyAddress = DEFAULT_gnlMyAddress;

	//gnlHenhouseNo = EE_Read_val( ADR_gnlHenhouseNo, 4);	//계사 번호
	//if((gnlHenhouseNo < MIN_gnlHenhouseNo) || (MAX_gnlHenhouseNo < gnlHenhouseNo))  gnlHenhouseNo = DEFAULT_gnlHenhouseNo;

	gnlUseLTE = EE_Read_val( ADR_gnlUseLTE, 4);	//LTE 사용여부 (0:지그비, 1:LTE)
	if((gnlUseLTE < MIN_gnlUseLTE) || (MAX_gnlUseLTE < gnlUseLTE))  gnlUseLTE = DEFAULT_gnlUseLTE;
	
	
	
	gnlSendingInterval = EE_Read_val( ADR_gnlSendingInterval, 4);	//전송주기 설정 
	if((gnlSendingInterval <0) || (10 < gnlSendingInterval))  gnlSendingInterval = 10;
	
	
	//gnlSendingInterval
	
	
	
	gnlSerialNo=EE_Read_val( ADR_gnlSerialNo, 4); Delay_ms(10);
	//if(gnlSerialNo<1900000  || gnlSerialNo>3000000)// 19년도는 지났으니까
  if(gnlSerialNo<1700000  || gnlSerialNo>3000000)// 19년도는 지났으니까
	  gnlSerialNo=2001000;
	
	
	gnlDailyTotal = EE_Read_val( ADR_gnlDailyTotal, 4); Delay_ms(10);
	gnlPreDailyTotal[0] = EE_Read_val( ADR_gnlPreDailyTotal_0, 4);Delay_ms(10);
	gnlPreDailyTotal[1] = EE_Read_val( ADR_gnlPreDailyTotal_1, 4);Delay_ms(10);
	gnlPreDailyTotal[2] = EE_Read_val( ADR_gnlPreDailyTotal_2, 4);Delay_ms(10);
	gnlPreDailyTotal[3] = EE_Read_val( ADR_gnlPreDailyTotal_3, 4);Delay_ms(10);
	gnlPreDailyTotal[4] = EE_Read_val( ADR_gnlPreDailyTotal_4, 4);Delay_ms(10);
	gnlPreDailyTotal[5] = EE_Read_val( ADR_gnlPreDailyTotal_5, 4);Delay_ms(10);
	gnlPreDailyTotal[6] = EE_Read_val( ADR_gnlPreDailyTotal_6, 4);Delay_ms(10);
	gnuDay = EE_Read_val( ADR_gnuDay, 4);Delay_ms(10);


//silo1
  gnlDailyTotal2 = EE_Read_val( ADR_gnlDailyTotal2, 4);Delay_ms(10);
	gnlPreDailyTotal2[0] = EE_Read_val( ADR_gnlPreDailyTotal2_0, 4);Delay_ms(10);
	gnlPreDailyTotal2[1] = EE_Read_val( ADR_gnlPreDailyTotal2_1, 4);Delay_ms(10);
	gnlPreDailyTotal2[2] = EE_Read_val( ADR_gnlPreDailyTotal2_2, 4);Delay_ms(10);
	gnlPreDailyTotal2[3] = EE_Read_val( ADR_gnlPreDailyTotal2_3, 4);Delay_ms(10);
	gnlPreDailyTotal2[4] = EE_Read_val( ADR_gnlPreDailyTotal2_4, 4);Delay_ms(10);
	gnlPreDailyTotal2[5] = EE_Read_val( ADR_gnlPreDailyTotal2_5, 4);Delay_ms(10);
	gnlPreDailyTotal2[6] = EE_Read_val( ADR_gnlPreDailyTotal2_6, 4);Delay_ms(10);
	gnuDay2 = EE_Read_val( ADR_gnuDay2, 4);Delay_ms(10);
	
	
	
	gxhTotalOutput = EE_Read_val( ADR_gxhTotalOutput, 4);	//총누적 배출량
	if((gxhTotalOutput <0) || (100000000 < gxhTotalOutput))  gxhTotalOutput = 0;

	
	
	
	IpAdresss_1= EE_Read_val( ADR_IpAdresss_1,4);//
   IpAdresss_2= EE_Read_val( ADR_IpAdresss_2,4);//
   IpAdresss_3= EE_Read_val( ADR_IpAdresss_3,4);//
   IpAdresss_4= EE_Read_val( ADR_IpAdresss_4,4);//
   IpPort= EE_Read_val( ADR_IpPort,4);//
   
	
	
	if(IpAdresss_1<100 || IpAdresss_1>255) // ip 주소가  잘못되었다
	{
		//지테크 서버로 설정
		IpAdresss_1=112;
		IpAdresss_2=217;
		IpAdresss_3=212;
		IpAdresss_4=250;
		IpPort=8391;
		
		IpAdresss_1 = EE_Write_val(ADR_IpAdresss_1, IpAdresss_1, 4); Delay_ms(30);
	  IpAdresss_2 = EE_Write_val(ADR_IpAdresss_2, IpAdresss_2, 4); Delay_ms(30);
	  IpAdresss_3 = EE_Write_val(ADR_IpAdresss_3, IpAdresss_3, 4); Delay_ms(30);
	  IpAdresss_4 = EE_Write_val(ADR_IpAdresss_4, IpAdresss_4, 4); Delay_ms(30);
	 IpPort = EE_Write_val(ADR_IpPort, IpPort, 4); Delay_ms(30);
		
	}
	
	
	
//	 long gnlFeeding1;
//  long gnlFeeding2;

	gnlFeeding1 = EE_Read_val( ADR_gnlFeeding1, 4);	//1번 배출시 양
	 if((gnlFeeding1 <0) || (9999 < gnlFeeding1))  gnlFeeding1 = 0;
	
	
	gnlFeeding2 = EE_Read_val( ADR_gnlFeeding2, 4);	//2번 배출시 양
	 if((gnlFeeding2 <0) || (9999 < gnlFeeding2))  gnlFeeding2 = 0;
	
	
	
	
     


//   FSMC_ENABLE;
} /* of read_factors() */


void Initiaize_eeprom(void)  //1126
{
}

 /*
 void read_factors()
{
 //  union { u32 dw; u16 u[2];} temp;
  float facter;
  union { u32 dw; u16 u[2]; u8 c[4];} temp;
 
   I2C_EE_BufferRead( temp.c, ADR_Bulk_Enable, 4);
   Bulk_Enable=temp.dw;
   if(Bulk_Enable<0 || Bulk_Enable>1)
      Bulk_Enable=1;  // default 부피센서 감지는 enable 
 
	Delay_ms(200);
  
   I2C_EE_BufferRead( temp.c, ADR_Bulk_Weight, 4);
   Bulk_Weight=temp.dw;
  
  
   if(Bulk_Weight<0 || Bulk_Weight>9000) // 0 kg 보다작고 90 보다크면 
      Bulk_Weight=7000;  // 초기부피센서 연동되는 무게는 70 kg
	Delay_ms(200);
    
	   
   I2C_EE_BufferRead( temp.c, ADR_Upper_Weight, 4);
   Upper_Weight=temp.dw;
   if(Upper_Weight<8000 || Upper_Weight>12000) //  80 kg 보다작고 120 보다크면 
      Upper_Weight=10000;  // 초기무게는 100 kg  

   Delay_ms(200);
  
	
} 
 */
/* of read_factors() */

   void write_factors()
{
/* 
		 #define ADR_gnfFactor             0
       #define ADR_v_zero                4 
      #define ADR_v_adc_org             8 
      #define ADR_v_division           12 //
      #define ADR_v_capacity           16// 상한무게 설정 
      #define ADR_v_span               24    //  빈통무게 설정 
*/

  union { u32 dw; u16 u[2]; u8 c[4];} temp;
  
	// Delay_ms(1000);
 FSMC_DISABLE;
  temp.dw=v_zero;
	I2C_EE_BufferWrite(temp.c, ADR_v_zero, 4);
	Delay_ms(10); //very very critical
	
 temp.dw=v_division;
	I2C_EE_BufferWrite(temp.c, ADR_v_division, 4);
  
Delay_ms(10);
temp.dw=v_capacity;
	I2C_EE_BufferWrite(temp.c, ADR_v_capacity, 4);
  	
 Delay_ms(10);
	temp.dw = (u32)(gnfFactor * (float)power(10, 9));
    I2C_EE_BufferWrite(temp.c, ADR_gnfFactor, 4);
 Delay_ms(10);


 temp.dw=v_span;
	I2C_EE_BufferWrite(temp.c, ADR_v_span, 4);
 Delay_ms(10);
///

  temp.dw=v_zero_2;
	I2C_EE_BufferWrite(temp.c, ADR_v_zero_2, 4);
	Delay_ms(10); //very very critical
	
 temp.dw=v_division_2;
	I2C_EE_BufferWrite(temp.c, ADR_v_division_2, 4);
  
Delay_ms(10);
temp.dw=v_capacity_2;
	I2C_EE_BufferWrite(temp.c, ADR_v_capacity_2, 4);
  	
 Delay_ms(10);
	temp.dw = (u32)(gnfFactor_2 * (float)power(10, 9));
    I2C_EE_BufferWrite(temp.c, ADR_gnfFactor_2, 4);
 Delay_ms(10);


 temp.dw=v_span_2;
	I2C_EE_BufferWrite(temp.c, ADR_v_span_2, 4);
 Delay_ms(10);


    if(cal_selection==1)
		{	
    gplCompensationWeight = 0;
		gplCompensationWeight = EE_Write_val(ADR_gplCompensationWeight, gplCompensationWeight, 4);
    }
		else
		{	
    gplCompensationWeight2 = 0;
		gplCompensationWeight2 = EE_Write_val(ADR_gplCompensationWeight2, gplCompensationWeight2, 4);
    }






/*  
  temp.dw=v_division;
	I2C_EE_BufferWrite(temp.c, ADR_v_division, 4);
  
  temp.dw=v_capacity;
	I2C_EE_BufferWrite(temp.c, ADR_v_capacity, 4);
  	
 temp.dw=v_span;
	I2C_EE_BufferWrite(temp.c, ADR_v_span, 4);
 */
	/*
	temp.dw = (u32)(gnfFactor * (float)power(10, 9));
    I2C_EE_BufferWrite(temp.c, ADR_gnfFactor, 4);

  temp.dw=v_adc_org;
	I2C_EE_BufferWrite(temp.c, ADR_v_adc_org, 4);
   
  temp.dw=v_division;
	I2C_EE_BufferWrite(temp.c, ADR_v_division, 4);
  
  temp.dw=v_capacity;
	I2C_EE_BufferWrite(temp.c, ADR_v_capacity, 4);
  	
 temp.dw=v_span;
	I2C_EE_BufferWrite(temp.c, ADR_v_span, 4);
  */
 
 
 
 /*
 	v_zero = EE_Read_val( ADR_v_zero, 4);	//zero a/d 값

//  I2C_EE_BufferRead( temp.c, ADR_v_span, 4);
//  v_span=temp.dw;
	v_span = EE_Read_val( ADR_v_span, 4);
  
//  I2C_EE_BufferRead( temp.c, ADR_v_capacity, 4);
//	v_capacity=temp.dw;
	v_capacity = EE_Read_val( ADR_v_capacity, 4);

//  I2C_EE_BufferRead( temp.c, ADR_v_division, 4);
//  v_division=temp.dw;
	v_division = EE_Read_val( ADR_v_division, 4);
  
//  I2C_EE_BufferRead( temp.c, ADR_gnfFactor, 4);
//  v_zero=temp.dw;
	temp.dw = EE_Read_val( ADR_gnfFactor, 4);
  	gnfFactor = (float)temp.dw / (float)power(10, 9);
*/
 
 
} 
 
   void write_factors_2()
{

  union { u32 dw; u16 u[2]; u8 c[4];} temp;
  

  temp.dw=v_zero_2;
	I2C_EE_BufferWrite(temp.c, ADR_v_zero_2, 4);
	Delay_ms(10); //very very critical
	
 temp.dw=v_division_2;
	I2C_EE_BufferWrite(temp.c, ADR_v_division_2, 4);
  
Delay_ms(10);
temp.dw=v_capacity_2;
	I2C_EE_BufferWrite(temp.c, ADR_v_capacity_2, 4);
  	
 Delay_ms(10);
	temp.dw = (u32)(gnfFactor_2 * (float)power(10, 9));
    I2C_EE_BufferWrite(temp.c, ADR_gnfFactor_2, 4);
 Delay_ms(10);


 temp.dw=v_span_2;
	I2C_EE_BufferWrite(temp.c, ADR_v_span_2, 4);
 Delay_ms(10);






 
 
} 
 



u32 EE_Write_val(int adr, u32 val, u8 bytes)
{
	union { u32 dw; u16 u[2]; u8 c[4];} temp;
	u32 ret_val;

//USART1_puts("EE_Write_val_0010");
	FSMC_DISABLE;
	
	//Delay_ms(2);

//	I2C_EE_BufferRead( temp.c, adr, 4);
	
	if(bytes == 1) temp.c[0] = val;
	else if (bytes == 2) temp.u[0] = val;
	else temp.dw = val;
			  
	I2C_EE_BufferWrite( temp.c, adr, 4);

//USART1_puts("EE_Write_val_0020");

//	I2C_EE_BufferRead( temp.c, adr, 4);
	//EE_Read_val(adr, bytes);

//USART1_puts("EE_Write_val_0024");

	//if(bytes == 1) ret_val = temp.c[0];
	//else if (bytes == 2) ret_val = temp.u[0];
	//else ret_val = temp.dw;

//USART1_puts("EE_Write_val_0028");

	Delay_ms(1);

	FSMC_ENABLE; 
	//Delay_ms(2);
//USART1_puts("EE_Write_val_0030");

//20170303	return ret_val;
	return val;
}

u32 EE_Read_val(int adr, u8 bytes)
{
	union { u32 dw; u16 u[2]; u8 c[4];} temp;
	u32 ret_val;

	FSMC_DISABLE;
	Delay_ms(1);

	I2C_EE_BufferRead( temp.c, adr, 4);

	if(bytes == 1) ret_val = temp.c[0];
	else if (bytes == 2) ret_val = temp.u[0];
	else ret_val = temp.dw;

	Delay_ms(1);
	FSMC_ENABLE; 
	
	return ret_val;
}

void save_DailyTotals()
{
	gnuDay = EE_Write_val(ADR_gnuDay, gnuDay, 4); Delay_ms(10);
	gnlDailyTotal = EE_Write_val(ADR_gnlDailyTotal, gnlDailyTotal, 4); Delay_ms(10);
	gnlPreDailyTotal[0] = EE_Write_val(ADR_gnlPreDailyTotal_0, gnlPreDailyTotal[0], 4); Delay_ms(30);
	gnlPreDailyTotal[1] = EE_Write_val(ADR_gnlPreDailyTotal_1, gnlPreDailyTotal[1], 4); Delay_ms(30);
	gnlPreDailyTotal[2] = EE_Write_val(ADR_gnlPreDailyTotal_2, gnlPreDailyTotal[2], 4); Delay_ms(30);
	gnlPreDailyTotal[3] = EE_Write_val(ADR_gnlPreDailyTotal_3, gnlPreDailyTotal[3], 4); Delay_ms(30);
	gnlPreDailyTotal[4] = EE_Write_val(ADR_gnlPreDailyTotal_4, gnlPreDailyTotal[4], 4); Delay_ms(30);
	gnlPreDailyTotal[5] = EE_Write_val(ADR_gnlPreDailyTotal_5, gnlPreDailyTotal[5], 4); Delay_ms(30);
	gnlPreDailyTotal[6] = EE_Write_val(ADR_gnlPreDailyTotal_6, gnlPreDailyTotal[6], 4); Delay_ms(30);
	
	gnuDay2 = EE_Write_val(ADR_gnuDay2, gnuDay2, 4); Delay_ms(10);
	gnlDailyTotal2 = EE_Write_val(ADR_gnlDailyTotal2, gnlDailyTotal2, 4); Delay_ms(10);
	gnlPreDailyTotal2[0] = EE_Write_val(ADR_gnlPreDailyTotal2_0, gnlPreDailyTotal2[0], 4); Delay_ms(30);
	gnlPreDailyTotal2[1] = EE_Write_val(ADR_gnlPreDailyTotal2_1, gnlPreDailyTotal2[1], 4); Delay_ms(30);
	gnlPreDailyTotal2[2] = EE_Write_val(ADR_gnlPreDailyTotal2_2, gnlPreDailyTotal2[2], 4); Delay_ms(30);
	gnlPreDailyTotal2[3] = EE_Write_val(ADR_gnlPreDailyTotal2_3, gnlPreDailyTotal2[3], 4); Delay_ms(30);
	gnlPreDailyTotal2[4] = EE_Write_val(ADR_gnlPreDailyTotal2_4, gnlPreDailyTotal2[4], 4); Delay_ms(30);
	gnlPreDailyTotal2[5] = EE_Write_val(ADR_gnlPreDailyTotal2_5, gnlPreDailyTotal2[5], 4); Delay_ms(30);
	gnlPreDailyTotal2[6] = EE_Write_val(ADR_gnlPreDailyTotal2_6, gnlPreDailyTotal2[6], 4); Delay_ms(30);
	
	
	
	
}

/* end of file */
