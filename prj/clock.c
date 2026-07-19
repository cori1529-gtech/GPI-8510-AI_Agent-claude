
http://read.pudn.com/downloads36/sourcecode/embed/112838/HT1381.c__.htm


http://s.pudn.com/search_hot_en.asp?k=ht1380#



#include "iom8v.h"   
#include "macros.h"   
#define uint unsigned int   
#define uchar unsigned char   
   
#define RESET_1   (PORTC |= (1 << 6))   
#define RESET_0   (PORTC &=~ (1 << 6))   
#define DATA_1    (PORTD |= (1 << 4))   
#define DATA_0    (PORTD &=~ (1 << 4))   
#define SCLK_1    (PORTC |= (1 << 5))   
#define SCLK_0    (PORTC &=~ (1 << 5))   
#define DATA_IN   (DDRD |= (1 << 4))   
#define DATA_OUT  (DDRD &=~ (1 << 4))   
   
#define GetSecond 0x81   
#define GetMinute 0x83   
#define GetHour   0x85   
#define GetDate   0x87   
#define GetMonth  0x89   
#define GetDay    0x8b   
#define GetYear   0x8d   
#define GetTime   0x0bf   
#define WP        0x8e   
#define CH        0x80   
   
#define SetSecond 0x80   
#define SetMinute 0x82   
#define SetHour   0x84   
#define SetDate   0x86   
#define SetMonth  0x88   
#define SetDay    0x8a   
#define SetYear   0x8c   
#define SetTime   0x0be   
/*void WriteByte(uchar Data);  
uchar ReadByte();  
uchar Get_Second();  
uchar Get_Minute();  
uchar Get_Hour();  
uchar Get_Date();  
uchar Get_Month();  
uchar Get_Day();  
uchar Get_Year();*/   
//*uchar GetTime();*/   
   
void WriteByte(uchar Data)   
{   
  uchar i;   
  DATA_IN;   
  for(i = 0; i < 8; i++)   
  {   
   SCLK_0;   
   if(Data & 0x01)   
           DATA_1;   
   else   
           DATA_0;   
   SCLK_1;   
   Data <<= 1;   
  }   
  SCLK_0;   
}   
   
uchar ReadByte()   
{   
  uchar i, Data = 0;   
  DATA_OUT;   
  SCLK_0;   
  for(i = 0; i < 8; i++)   
  {   
   SCLK_1;   
   if(PIND & 0x08)   
           Data += 0x80;   
   else   
           Data += 0x00;   
   SCLK_0;        
   Data >>= 1;   
  }   
  return Data;   
}   
   
uchar Get_Second()   
{   
 uchar Second;   
 RESET_0;   
 RESET_1;   
 WriteByte(GetSecond);   
 Second = ReadByte();   
 RESET_0;   
 return Second;   
}   
   
uchar Get_Minute()   
{   
 uchar Minute;   
 RESET_0;   
 RESET_1;   
 WriteByte(GetMinute);   
 Minute = ReadByte();   
 RESET_0;   
 return Minute;   
}   
   
uchar Get_Hour()   
{   
 uchar Hour;   
 RESET_0;   
 RESET_1;   
 WriteByte(GetHour);   
 Hour = ReadByte();   
 RESET_0;   
 return Hour;   
}   
   
uchar Get_Date()   
{   
 uchar Date;   
 RESET_0;   
 RESET_1;   
 WriteByte(GetDate);   
 Date = ReadByte();   
 RESET_0;   
 return Date;   
}   
   
uchar Get_Month()   
{   
 uchar Month;   
 RESET_0;   
 RESET_1;   
 WriteByte(GetMonth);   
 Month = ReadByte();   
 RESET_0;   
 return Month;   
}   
   
uchar Get_Year()   
{   
 uchar Year;   
 RESET_0;   
 RESET_1;   
 WriteByte(GetYear);   
 Year = ReadByte();   
 RESET_0;   
 return Year;   
}   
   
uchar Get_Day()   
{   
 uchar Day;   
 RESET_0;   
 RESET_1;   
 WriteByte(GetDay);   
 Day = ReadByte();   
 RESET_0;   
 return Day;   
}   
   
void Get_Time(uchar Time[])   
{   
 uchar i, *p;   
 RESET_0;   
 RESET_1;   
 p = Time;   
 WriteByte(GetTime);   
 for(i = 0; i < 8; i++)   
 {   
  *p = ReadByte();   
  p++;   
 }   
 RESET_0;   
}   
   
void WriteEnable()   
{   
 uchar i;   
 RESET_0;   
 RESET_1;   
 WriteByte(WP);   
 for(i = 0; i < 7; i++)   
 {   
  SCLK_0;   
  SCLK_1;   
 }   
 SCLK_0;   
 DATA_1;   
 SCLK_1;   
 SCLK_0;   
 RESET_0;   
 RESET_1;   
 WriteByte(CH);   
 for(i = 0; i < 7; i++)   
 {   
  SCLK_0;   
  SCLK_1;   
 }   
 SCLK_0;   
 DATA_1;   
 SCLK_1;   
 SCLK_0;   
 RESET_0;   
}   
   
void Set_Second(uchar Second)   
{   
 RESET_0;   
 RESET_1;   
 WriteByte(SetSecond);   
 WriteByte(Second);   
 RESET_0;   
}   
   
void Set_Minute(uchar Minute)   
{   
 RESET_0;   
 RESET_1;   
 WriteByte(SetMinute);   
 WriteByte(Minute);   
 RESET_0;   
}   
   
void Set_Hour(uchar Hour)   
{   
 RESET_0;   
 RESET_1;   
 WriteByte(SetHour);   
 WriteByte(Hour);   
 RESET_0;   
}   
   
void Set_Date(uchar Date)   
{   
 RESET_0;   
 RESET_1;   
 WriteByte(SetDate);   
 WriteByte(Date);   
 RESET_0;   
}   
   
void Set_Month(uchar Month)   
{   
 RESET_0;   
 RESET_1;   
 WriteByte(SetMonth);   
 WriteByte(Month);   
 RESET_0;   
}   
   
void Set_Year(uchar Year)   
{   
 RESET_0;   
 RESET_1;   
 WriteByte(SetYear);   
 WriteByte(Year);   
 RESET_0;   
}   
   
void Set_Day(uchar Day)   
{   
 RESET_0;   
 RESET_1;   
 WriteByte(SetDay);   
 WriteByte(Day);   
 RESET_0;   
}   
   
void Set_Time(uchar Time[])   
{   
 uchar i, *p;   
 RESET_0;   
 RESET_1;   
 p = Time;   
 WriteByte(SetTime);   
 for(i = 0; i < 8; i++)   
 {   
  WriteByte(*p);   
  p++;   
 }   
 RESET_0;   
}   
   
void TestTime()   
{   
 uchar Second, Minute, Hour, Date, Month, Year, Day, Time[7];   
 uchar Sec = 1, Min = 2, H = 3, D = 4, Mon = 5, Y = 6, day = 7, time[7] = {9,8,7,6,5,4,3};   
 Second = Get_Second();   
 Minute = Get_Minute();   
 Hour = Get_Hour();   
 Date = Get_Date();   
 Month = Get_Month();   
 Year = Get_Year();   
 Day = Get_Day();   
 //Get_Time(Time[7]);   
    
 Set_Second(Sec);   
 Set_Minute(Min);   
 Set_Hour(H);   
 Set_Date(D);   
 Set_Month(Mon);   
 Set_Year(Y);   
 Set_Day(day);   
 //Set_Time(time[7]);   
}   
   
/*void TestFlash()  
{  
 SPI_SendDate(FlashWriteAddress[3]);  
 SPI_ReceiveDate(FlashReadAddress[3]);  
}*/   
void main()   
{   
   
 TestTime();   
 /*InitialPTR4000();  
 while(1) {  
      asm("sleep");  
 }*/   
}   