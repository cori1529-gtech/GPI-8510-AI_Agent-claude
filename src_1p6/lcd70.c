#include <stdio.h>
#include "stdlib.h"

#include  <stm32f10x.h>
#include "hw_config.h"

#include "def.h"

#include "lcd.h" 
#include "lcd70.h"
#include "usart.h"	
#include "var.h"


#define LCDBUFSIZE_800_400	800*480

#define LCD70_DBG_LOG
#undef	LCD70_DBG_LOG


extern const unsigned char asc2_1206[95][12];
extern const unsigned char asc2_1608[95][16];
//extern const unsigned char icon_demo[63720];
extern const unsigned char BIG_ASCII[195][16];
extern const unsigned char P2_16[];		//2015.4.28  test__onl7y
extern const unsigned char P2_24[];		//2015.4.28  test__onl7y
extern const unsigned char ASCII_16X16[];
extern const unsigned char ASCII_24X24[];



/*
static const struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; 
  unsigned char	 pixel_data[70 * 70 * 2 + 1];
} start_lamp = {

 */



#define DISP_HOR_BACK_PORCH		210
#define DISP_HOR_FRONT_PORCH		45
#define DISP_VER_BACK_PORCH		34	
#define DISP_VER_FRONT_PORCH		10


#define DISP_HOR_RESOLUTION  800
#define DISP_VER_RESOLUTION  480

unsigned int  HDP_70=DISP_HOR_RESOLUTION;	// DISP_HOR_RESOLUTION
//unsigned int  HT_70=850;
//unsigned int  HPS_70=51;
unsigned int  LPS_70=3;
unsigned char HPW_70=1;		// DISP_HOR_PULSE_WIDTH


unsigned int  VDP_70=DISP_VER_RESOLUTION;	// DISP_VER_RESOLUTION
//unsigned int  VT_70=530;
//unsigned int  VPS_70=50;
unsigned int  FPS_70=30;
unsigned char VPW_70=1;		// DISP_VER_PULSE_WIDTH

#define HT_70 (HDP_70+HPW_70+DISP_HOR_BACK_PORCH+DISP_HOR_FRONT_PORCH)
#define HPS_70 (HPW_70+DISP_HOR_BACK_PORCH)

#define VT_70 (VPW_70+DISP_VER_BACK_PORCH+DISP_VER_FRONT_PORCH+VDP_70)
#define VSP_70 (VPW_70+DISP_VER_BACK_PORCH)


/*
| FPS | VPW |
| VPS         |         VDP                |
VT = VDP + VPS + FPS

| LPS | HPW |
| HPS         |         HDP                |
HT = HDP + HPS + LPS
*/

u8 brightness = 0x80;
u8 contrast = 0x80;
u8 saturation = 0x80;

u8 s1963gpio;



void lcd43_show_big_num(unsigned int x,unsigned int y,unsigned int num)
{
    unsigned int n,t;
	unsigned int temp;
	unsigned int t1,deadline;
	
	lcd_wr_reg(0x002A);	
	lcd_wr_data(x>>8);	    
	lcd_wr_data(x&0x00ff);
	lcd_wr_data((x+29)>>8);	    
	lcd_wr_data((x+29)&0x00ff);
    lcd_wr_reg(0x002b);	
	lcd_wr_data(y>>8);	    
	lcd_wr_data(y&0x00ff);
	lcd_wr_data((y+59)>>8);	    
	lcd_wr_data((y+59)&0x00ff);
	lcd_wr_reg(0x002c);


	if(num==':')t1=150;
	else if(num=='.')t1=165;
	else if(num=='-')t1=180;
	else if(num=='C')t1=195;
	else t1=15*num;
	deadline=t1+15;
		   
	for(;t1<deadline;t1++)
	{	 
		for(n=0;n<16;n++)
		{
			temp=BIG_ASCII[t1][n];
			for(t=0;t<8;t++)
			{
				if(temp&0x80) 
///				   lcd_wr_data(POINT_COLOR);
				   lcd_wr_data(BLACK);
				  // 	 lcd43_draw_point(x+t,y+n,POINT_COLOR);
				else 
///				    lcd_wr_data(BACK_COLOR);
				    lcd_wr_data(WHITE);
					// lcd43_draw_point(x+t,y+n,BACK_COLOR);
				
				temp<<=1;
				if(((n%4)==3)&&t==5)break;
			}
		}
	} 

}


void bsp_lcd70_init(void)
{
	bsp_lcd_gpio_init();

///	delay_ms(10);
	
/*	
	delay_ms(1000);	///
	delay_ms(1000);	///
	delay_ms(1000);	///
	delay_ms(1000);	///
*/	

	LCD_CS_HIGH();

	// LCD Data(0 ~ 7)
	GPIO_SetBits(GPIOD, GPIO_Pin_14| GPIO_Pin_15 |GPIO_Pin_0 | GPIO_Pin_1);  	 
	GPIO_SetBits(GPIOE, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);   

	
	LCD_RESET_LOW();

	LCD_RD_HIGH();
	LCD_WR_HIGH();
	
/*	
	delay_ms(1000);	///
	delay_ms(1000);	///
	delay_ms(1000);	///
	delay_ms(1000);	///
*/		
//	delay_ms(4000);	///

	delay_ms(10);
	bsp_fsmc_lcd_init();
  delay_ms(10);

/*
	delay_ms(1000);	///
	delay_ms(1000);	///
	delay_ms(1000);	///
	delay_ms(1000);	///
*/	

#ifdef LCD70_DBG_LOG
	usart1_transmit_string("\r\nfsmc init\r\n");
#endif

//USART1_puts("lcd_rst();  begin");
	lcd_rst();

///	delay_ms(20);
	//delay_ms(20);
	//delay_ms(20);
	//delay_ms(20);
	
/*
	delay_ms(1000);	///
	delay_ms(1000);	///
	delay_ms(1000);	///
	delay_ms(1000);	///


USART1_puts("lcd_rst();  end");	
*/

#ifdef LCD70_DBG_LOG
	usart1_transmit_string("\r\nlcd_rst()\r\n");
#endif


	lcd_wr_reg(0x00E2);		// Set PLL with OSC = 10MHz (hardware),	Multiplier N = 35, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 360MHz
	
//USART1_puts("lcd_PLL 0010");	
//delay_ms(10);	
	lcd_wr_data(0x0023);	
	
//delay_ms(10);	
//USART1_puts("lcd_PLL 0011");	
	
	lcd_wr_data(0x0002);	// Divider M = 2, PLL = 360/(M+1) = 120MHz
	//lcd_wr_data(0x0004);	// Validate M and N values
	
//USART1_puts("lcd_PLL 0012");	
	
	lcd_wr_data(0x0054);	// Validate M and N values


//USART1_puts("lcd_PLL M N end");	


#ifdef LCD70_DBG_LOG
	lcd_wr_reg(0x00E3);
	usart1_transmit_string("\r\n");
	usart1_transmit_string_format("e30=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("e31=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("e32=%02x\r\n",lcd_rd_data());
#endif
	
	lcd_wr_reg(0x00E0);  // PLL enable
	lcd_wr_data(0x0001);
	delay_ms(10);
//delay_ms(100); ///	

	lcd_wr_reg(0x00E0);		// Start PLL command again
	lcd_wr_data(0x0003);	// now, use PLL output as system clock	

#ifdef LCD70_DBG_LOG
	lcd_wr_reg(0xE4);	
	usart1_transmit_string_format("e40=%02x\r\n",lcd_rd_data());
#endif


//USART1_puts("lcd_PLL end");	

//	delay_ms(60);

///	delay_ms(20);
///	delay_ms(20);
///	delay_ms(20);
	lcd_wr_reg(0x0001);  // software reset

///	delay_ms(20);
///	delay_ms(20);
	delay_ms(20);
	lcd_wr_reg(0x00E6);	//PLL setting for PCLK, depends on resolution
	lcd_wr_data(0x0004);
	lcd_wr_data(0x0093);
	lcd_wr_data(0x00E0);
#if 0
	lcd_wr_data(0x0001);
	lcd_wr_data(0x0033);
	lcd_wr_data(0x0032);
#endif
#ifdef LCD70_DBG_LOG
	lcd_wr_reg(0x00E7);
	usart1_transmit_string("\r\n");
	usart1_transmit_string_format("e70=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("e71=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("e72=%02x\r\n",lcd_rd_data());
#endif

	lcd_wr_reg(0x00B0);	//LCD SPECIFICATION
	lcd_wr_data(0x0010);
	lcd_wr_data(0x0080);
	lcd_wr_data(( (HDP_70-1) >>8)& 0X00FF);  //Set HDP
	lcd_wr_data( (HDP_70-1) & 0X00FF);
    lcd_wr_data(( (VDP_70-1) >>8) & 0X00FF);  //Set VDP
	lcd_wr_data( (VDP_70-1) & 0X00FF);
    lcd_wr_data(0x0000);
#ifdef LCD70_DBG_LOG
	lcd_wr_reg(0x00B1);
	usart1_transmit_string("\r\n");
	usart1_transmit_string_format("b10=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("b11=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("b12=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("b13=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("b14=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("b15=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("b16=%02x\r\n",lcd_rd_data());
#endif

	lcd_wr_reg(0x00B4);	//HSYNC
	lcd_wr_data(( (HT_70-1) >>8)& 0X00FF);  //Set HT
	lcd_wr_data( (HT_70-1) & 0X00FF);
	lcd_wr_data(( (HPS_70-1) >>8)&0X00FF);  //Set HPS
	lcd_wr_data( (HPS_70-1) & 0X00FF);
	lcd_wr_data( (HPW_70-1) );			   //Set HPW
	lcd_wr_data(0x0000);
	lcd_wr_data(0x0000);
	lcd_wr_data(0x0000);
#ifdef LCD70_DBG_LOG
	lcd_wr_reg(0x00B5);
	usart1_transmit_string("\r\n");
	usart1_transmit_string_format("b50=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("b51=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("b52=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("b53=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("b54=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("b55=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("b56=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("b57=%02x\r\n",lcd_rd_data());
#endif

	//lcd_wr_data(( LPS_70>>8)&0X00FF);  //Set HPS
	//lcd_wr_data( LPS_70&0X00FF);
	//lcd_wr_data(0x0000);

	lcd_wr_reg(0x00B6);	//VSYNC
	lcd_wr_data(( (VT_70-1) >>8)&0X00FF);   //Set VT
	lcd_wr_data( (VT_70-1) &0X00FF);
	lcd_wr_data(( (VSP_70-1) >>8)&0X00FF);  //Set VPS
	lcd_wr_data( (VSP_70-1) &0X00FF);
	lcd_wr_data( (VPW_70-1) );			   //Set VPW
	lcd_wr_data(0x0000);
	lcd_wr_data(0x0000);
#ifdef LCD70_DBG_LOG
	lcd_wr_reg(0x00B7);
	usart1_transmit_string("\r\n");
	usart1_transmit_string_format("b70=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("b71=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("b72=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("b73=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("b74=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("b75=%02x\r\n",lcd_rd_data());
	usart1_transmit_string_format("b76=%02x\r\n",lcd_rd_data());	
#endif


	lcd_wr_reg(0x00F0); //pixel data interface	  usart1_transmit_string("==========Get Display mode==========\r\n");
//	usart1_transmit_string("11111r\n");
	lcd_wr_data(0x0003);
//		usart1_transmit_string("22222r\n");
	lcd_wr_reg(0x0021);
//		usart1_transmit_string("33333r\n");
    lcd_wr_reg(0x00BC);
//		usart1_transmit_string("44444r\n");
	lcd_wr_data(brightness);
//		usart1_transmit_string("55555r\n");
    lcd_wr_data(contrast);
//		usart1_transmit_string("66666r\n");
    lcd_wr_data(saturation);
//		usart1_transmit_string("77777r\n");
    lcd_wr_data(0x0001);
//		usart1_transmit_string("88888r\n");


#if 0

	lcd_wr_reg(0x3A);	//Set pixel format, i.e. the bpp
	usart1_transmit_string("99999r\n");

	lcd_wr_data(0x55); 	//set 16bpp
	usart1_transmit_string("aaaaar\n");

	
	lcd_wr_reg(0xF0);	//Éè16bit Set pixel data interface
	usart1_transmit_string("bbbbbr\n");
 
	lcd_wr_data(0x03);	//16-bit(565 format) data for 16bpp PIC32MX only
	usart1_transmit_string("cccccr\n");

	
#ifdef LCD70_DBG_LOG
	lcd_wr_reg(0xF1);
	usart1_transmit_string_format("F10=%02x\r\n",lcd_rd_data());
#endif
	
	lcd_wr_reg(0x21);
	
    lcd_wr_reg(0xBC);
	lcd_wr_data(brightness);
    lcd_wr_data(contrast);
    lcd_wr_data(saturation);
    lcd_wr_data(0x01);
        
///	lcd_wr_reg(0x0029);	// Turn on display; show the image on display
	
#ifdef LCD70_DBG_LOG
    usart1_transmit_string("==========get power mode==========\r\n");
	lcd_wr_reg(0x0A);
	usart1_transmit_string_format("0A0=%02x\r\n",lcd_rd_data());
    usart1_transmit_string("==========get adder mode==========\r\n");
	lcd_wr_reg(0x0B);
	usart1_transmit_string_format("0B0=%02x\r\n",lcd_rd_data());
    usart1_transmit_string("==========Get Pixel Format==========\r\n");
	lcd_wr_reg(0x0C);
	usart1_transmit_string_format("0B0=%02x\r\n",lcd_rd_data());
    usart1_transmit_string("==========Get Display mode==========\r\n");
	lcd_wr_reg(0x0D);
	usart1_transmit_string_format("0B0=%02x\r\n",lcd_rd_data());
    usart1_transmit_string("==========Get Signal Mode==========\r\n");
	lcd_wr_reg(0x0E);
	usart1_transmit_string_format("0B0=%02x\r\n",lcd_rd_data());
    usart1_transmit_string("==========Get Post Proc==========\r\n");
	lcd_wr_reg(0xBD);
	usart1_transmit_string_format("0B0=%02x\r\n",lcd_rd_data());
    usart1_transmit_string_format("0B0=%02x\r\n",lcd_rd_data());
    usart1_transmit_string_format("0B0=%02x\r\n",lcd_rd_data());
    usart1_transmit_string_format("0B0=%02x\r\n",lcd_rd_data());
#endif  
        
	lcd_wr_reg(0xB8);
	lcd_wr_data(0x0F);
	lcd_wr_data(0x01);

#endif

///	delay_ms(5); // usart1_transmit_string("dddddr\n");

///	lcd70_clear_screen(WHITE);	//usart1_transmit_string("eeeeer\n");

	lcd_wr_reg(0x0029); /*display on*/  //usart1_transmit_string("fffffr\n");

	LCD_PWM_LIGHT_HIGH();	 //usart1_transmit_string("gggggr\n");

}

void lcd70_set_area(u16 x1, u16 y1, u16 x2, u16 y2)
{
	lcd_wr_reg(0x2A);
	lcd_wr_data(x1>>8);
	lcd_wr_data(x1);
	lcd_wr_data(x2>>8);
	lcd_wr_data(x2);
	lcd_wr_reg(0x2B);
	lcd_wr_data(y1>>8);
	lcd_wr_data(y1);
	lcd_wr_data(y2>>8);
	lcd_wr_data(y2);
}

void lcd70_clear_screen_test(unsigned int color)
{                    
	register u32 size;

	
	lcd70_set_area(0,0,299,199);
	lcd_wr_reg(0x2C);
	
	
	size = 300*200;
	while(size--)
		lcd_wr_data(color);


}

// 800 * 480
void lcd70_clear_screen(unsigned int color)
{                    
	
	register u32 size;

	lcd70_set_area(0,0,799,479);
	lcd_wr_reg(0x2C);
	
	size = LCDBUFSIZE_800_400;

	while(size--) lcd_wr_data(color);
}



void lcd70_draw_point(u16 x, u16 y, u16 color)
{						
	lcd70_set_area(x,y,x,y);
	lcd_wr_reg(0x2C);
	lcd_wr_data(color);
} 

void lcd70_show_char(u16 x, u16 y, u8 num, u16 color)
{       
	#define MAX_CHAR70_POSX (DISP_HOR_RESOLUTION - 20)
	#define MAX_CHAR70_POSY (DISP_VER_RESOLUTION - 20)

	u16 size = 16;

    unsigned int temp;
    unsigned int pos,t;      

    if(x>MAX_CHAR70_POSX||y>MAX_CHAR70_POSY)
		return;
   	lcd_wr_reg(0x002A);	
	lcd_wr_data(x>>8);	    
	lcd_wr_data(x&0x00ff);
	lcd_wr_data(x+(size/2-1)>>8);	    
	lcd_wr_data(x+(size/2-1)&0x00ff);
    lcd_wr_reg(0x002b);	
	lcd_wr_data(y>>8);	    
	lcd_wr_data(y&0x00ff);
	lcd_wr_data((y+size-1)>>8);	    
	lcd_wr_data((y+size-1)&0x00ff);
	lcd_wr_reg(0x002c);

	num=num-' ';

	for(pos=0;pos<size;pos++)
	{
		if(size==12)
			temp=asc2_1206[num][pos];
		else 
			temp=asc2_1608[num][pos];

		for(t=0;t<size/2;t++)
	    {                 
	        if(temp&0x01)
				lcd_wr_data(color);
	        else 
				lcd_wr_data(0xffff);

	        //if(temp&0x01)
			//	lcd70_draw_point(x+t,y+pos,color);

	        temp>>=1; 
	    }
	}
	
} 

void lcd70_show_char_mode(unsigned int x,unsigned int y,unsigned int num,unsigned int size,unsigned int mode)
{       
    unsigned int temp;
    unsigned int pos,t;      

    if(x>MAX_CHAR70_POSX||y>MAX_CHAR70_POSY)
		return;
   	lcd_wr_reg(0x002A);	
	lcd_wr_data(x>>8);	    
	lcd_wr_data(x&0x00ff);
	lcd_wr_data(x+(size/2-1)>>8);	    
	lcd_wr_data(x+(size/2-1)&0x00ff);
    lcd_wr_reg(0x002b);	
	lcd_wr_data(y>>8);	    
	lcd_wr_data(y&0x00ff);
	lcd_wr_data((y+size-1)>>8);	    
	lcd_wr_data((y+size-1)&0x00ff);
	lcd_wr_reg(0x002c);

	num=num-' ';
	if(!mode)
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];
			else temp=asc2_1608[num][pos];

			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)
				 // lcd_wr_data(POINT_COLOR);
		          lcd70_draw_point(x+t,y+pos,POINT_COLOR);
				else 
				  //lcd_wr_data(BACK_COLOR); 
				  lcd70_draw_point(x+t,y+pos,BACK_COLOR);    
		        temp>>=1; 
		    }
		}	
	}
	else
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];
			else temp=asc2_1608[num][pos];
			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)lcd70_draw_point(x+t,y+pos,POINT_COLOR);
		        temp>>=1; 
		    }
		}
	}
} 

void lcd70_show_string_mode(unsigned int x,unsigned int y,const unsigned char *p,unsigned int size,unsigned int mode)
{
	if( size == 0 )	  size = 16;

    while(*p!='\0')
    {   
		    
        if(x>MAX_CHAR70_POSX)
		{
			x=0;
			y+=size;
		}
        
		if(y>MAX_CHAR70_POSY)
		{
			y=x=0;
			lcd70_clear_screen(WHITE);
		}
        
		lcd70_show_char_mode(x,y,*p,size,mode);
        x = x + size/2;
        p++;
    }  
}

void lcd70_fill_color(u16 x,u16 y,u32 len,u32 wid, u16 color )
{                    

	u32 size;

	lcd70_set_area(x,y,len,wid);
	lcd_wr_reg(0x2C);
	
	size = (len-x)*(wid-y);

	while(size--)
		lcd_wr_data(color);
}   

void lcd70_show_string(u16 x, u16 y, const u8 *p, u16 color)
{         
	u16 size = 16;

    while(*p!='\0')
    {   
		    
        if(x>MAX_CHAR70_POSX)
		{
			x=0;
			y+=size;
		}
        
		if(y>MAX_CHAR70_POSY)
		{
			y=x=0;
			lcd70_clear_screen(WHITE);
		}
        
		lcd70_show_char(x,y,*p,color);
        x = x + size/2;
        p++;
    }  
}

void lcd70_draw_point_big(u16 x,u16 y, u16 color)
{
	lcd70_draw_point(x,y,color);
	lcd70_draw_point(x+1,y,color);
	lcd70_draw_point(x,y+1,color);
	lcd70_draw_point(x+1,y+1,color);
}

void lcd70_show_num(u16 x,u16 y,u32 num, u16 color)
{      
	u32 res;   	   
	u8 t=0,t1=0;   
	res=num;
	if(!num)lcd70_show_char(x,y,'0',color);
	while(res)  						
	{
		res/=10;
		t++;
	}
	t1=t;
	while(t)							
	{
		res=mn(10,t-1); 	 
	    lcd70_show_char(x+(t1-t)*6,y,(num/res)%10+'0', color);
		t--;
	}
	if(t1==3) lcd70_show_char(x+18,y,' ', color);
} 

void lcd70_draw_line(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	u16 t; 
	int xerr=0, yerr=0, delta_x, delta_y, distance; 
	int incx, incy, uRow, uCol; 

	delta_x=x2-x1; // computation increase of coordinates 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; // establishes on foot the direction 
	else if(delta_x==0)incx=0; // perpendicular line 
	else {incx=-1; delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0; // level line 
	else {incy=-1; delta_y=-delta_y;} 
	if (delta_x>delta_y) distance=delta_x; // selects the basic increase coordinate axis 
	else distance=delta_y; 
	for (t=0; t<=distance+1; t++) // linedraw output 
	{  
		lcd70_draw_point(uRow, uCol, color); // picture spot 
		xerr+=delta_x; 
		yerr+=delta_y; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 

void lcd70_draw_rectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	lcd70_draw_line(x1, y1, x2, y1, color);
	lcd70_draw_line(x1, y1, x1, y2, color);
	lcd70_draw_line(x2, y1, x2, y2, color);
	lcd70_draw_line(x1, y2, x2, y2, color);
}

// is assigning the position to draw one to assign the size the circle
// (x, y): Central point
//r: Radius
void lcd70_draw_circle (u8 x0, u16 y0, u8 r, u16 color)
{
	int a, b;
	int di;
	a=0; b=r;	  
	di=3-(r<<1);             // judgment spot position symbol
	while(a<=b)
	{
		lcd70_draw_point(x0-b, y0-a, color);             //3           
		lcd70_draw_point(x0+b, y0-a, color);             //0           
		lcd70_draw_point(x0-a, y0+b, color);             //1       
		lcd70_draw_point(x0-b, y0-a, color);             //7           
		lcd70_draw_point(x0-a, y0-b, color);             //2             
		lcd70_draw_point(x0+b, y0+a, color);             //4               
		lcd70_draw_point(x0+a, y0-b, color);             //5
		lcd70_draw_point(x0+a, y0+b, color);             //6 
		lcd70_draw_point(x0-b, y0+a, color);             
		a++;
		// uses the Bresenham algorithm to make a circle     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		lcd70_draw_point(x0+a, y0+b, color);
	}
} 

void lcd70_set_cursor(u16 x, u16 y)
{	
	// set column address							       
	lcd_wr_reg(0x002A);				    
	lcd_wr_data(x>>8);
	lcd_wr_data(x);	    			
	lcd_wr_data(x>>8);
	lcd_wr_data(x);	    		
		    
	// set page address
    lcd_wr_reg(0x002b);	
	lcd_wr_data(y>>8);
	lcd_wr_data(y);	    			    
	lcd_wr_data(y>>8);
	lcd_wr_data(y);
} 

void lcd70_write_ram_prepare(void) 
{
	// write memory start			
	lcd_wr_reg(0x2c);
}


void lcd70_draw_demo_bin(const unsigned char *img, unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend)
{                    
    unsigned long n,disp_pix_num;	
    unsigned int temp;
 
	lcd_wr_reg(0x002A);	
	lcd_wr_data(xsta>>8);	    
	lcd_wr_data(xsta&0x00ff);
	lcd_wr_data(xend-1>>8);	    
	lcd_wr_data(xend-1&0x00ff);
    lcd_wr_reg(0x002b);	
	lcd_wr_data(ysta>>8);	    
	lcd_wr_data(ysta&0x00ff);
	lcd_wr_data(yend-1>>8);	    
	lcd_wr_data(yend-1&0x00ff);
	lcd_wr_reg(0x002c);

	n=0;

	disp_pix_num = (xend-xsta) * (yend-ysta) * 2;
	while(n<disp_pix_num)
	 { 
	  temp=(uint16_t)( img[n])+(img[n+1]<<8);
	  lcd_wr_data(temp);
	  n=n+2;
	 }	 
}



void lcd70_draw_demo_icon(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned char picturenumber)
{                    
    unsigned long n,disp_pix_num;	
    unsigned int temp;
 


	disp_pix_num=(unsigned long)(yend-ysta)*(xend-xsta)*2;



	lcd_wr_reg(0x002A);	
	lcd_wr_data(xsta>>8);	    
	lcd_wr_data(xsta&0x00ff);
	lcd_wr_data(xend-1>>8);	    
	lcd_wr_data(xend-1&0x00ff);
    lcd_wr_reg(0x002b);	
	lcd_wr_data(ysta>>8);	    
	lcd_wr_data(ysta&0x00ff);
	lcd_wr_data(yend-1>>8);	    
	lcd_wr_data(yend-1&0x00ff);
	lcd_wr_reg(0x002c);

	n=0;
   /*
	while(n<disp_pix_num)

	 { 
	  temp=(uint16_t)( icon_demo[n+picturenumber*7080]<<8)+icon_demo[n+1+picturenumber*7080];
	  lcd_wr_data(temp);
	  n=n+2;
	 }	 
*/

}

void lcd70_draw_demo_icon_all(void)
{

	unsigned char t;
	unsigned int tx,ty;
	lcd70_clear_screen(WHITE);
	
	
	tx=10;ty=10;
	for(t=0;t<9;t++)
	{	   
					    
		tx = 100*(t%5)+10;//10,90,170,250,330	    
		if((t==5)||(t>5))
			ty=153;	  
		
		lcd70_draw_demo_icon(tx,ty,tx+59,ty+60,t);
		
	
		lcd70_demo(0,t,0);
		
	}

	//lcd70_clear_screen(WHITE);
	//lcd70_draw_demo_bin(0,0,300,400);

   /*
	for(i=0;i<100000;i++)
	{
	lcd70_write_weight(50,100,i);
	
	}

	*/
    /*
	void lcd70_write_weight(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend.,long weight);

	lcd43_show_big_num(100,100,1);
	lcd43_show_big_num((100+30),100,2);
	lcd43_show_big_num((100+30*2),100,3);
	lcd43_show_big_num((100+30*3),100,4);
	lcd43_show_big_num((100+30*4),100,5);
	lcd43_show_big_num((100+30*5),100,6);

	*/


	
}

void lcd70_write_weight(unsigned int xsta,unsigned int ysta,long weight)
{
	int iz;
	char temp[15];

    sprintf(temp,"%6ld",weight);
	
	for(iz=0; iz<6; iz++)
	{
		if( '0'<=temp[iz] && temp[iz]<='9') lcd43_show_big_num((xsta+30*iz), ysta, temp[iz]-0x30);
		else if(temp[iz] == '-') lcd43_show_big_num((xsta+30*iz), ysta,'-');
		else lcd43_show_big_num((xsta+30*iz), ysta,'C');
	}
/*	
	if(temp[1]!=' ')
	lcd43_show_big_num((xsta+30),ysta,temp[1]-0x30);
	else
	lcd43_show_big_num((xsta+30),ysta,'C');
	
	if(temp[2]!=' ') 
	lcd43_show_big_num((xsta+30*2),ysta,temp[2]-0x30);
	else
	lcd43_show_big_num((xsta+30*2),ysta,'C');
	
	if(temp[3]!=' ')
	lcd43_show_big_num((xsta+30*3),ysta,temp[3]-0x30);
	else
	lcd43_show_big_num((xsta+30*3),ysta,'C');
	
	if(temp[4]!=' ')
	lcd43_show_big_num((xsta+30*4),ysta,temp[4]-0x30);
	else
    lcd43_show_big_num((xsta+30*4),ysta,'C');
	
	if(temp[5]!=' ')
	lcd43_show_big_num((xsta+30*5),ysta,temp[5]-0x30);
	else
	lcd43_show_big_num((xsta+30*5),ysta,'C');
*/
}

void lcd70_demo(unsigned char M_Type,unsigned char Cnt,unsigned char En)
{
	unsigned int tx;
	unsigned int ty;
		
	ty=10; 
	lcd_wr_reg(0x0028); //display on
	
	if(M_Type==0)
	{	  
		tx=100*(Cnt%5)+10;//10,90,170			
		
		if((Cnt==5)||(Cnt>5))
			ty=153;	 

		if(En)
		{	  
			POINT_COLOR=WHITE;
			BACK_COLOR=BLUE;
		}else
		{
			POINT_COLOR=BLACK;
			BACK_COLOR=WHITE;
		}

		if( Cnt == 0 )
	  		lcd70_show_string_mode(tx+13, ty+74, "Picture", 16, 0);	
		else if( Cnt == 1 )
	  		lcd70_show_string_mode(tx+13, ty+74, "Thils", 16, 0);	
		else if( Cnt == 2 )
	  		lcd70_show_string_mode(tx+13, ty+74, "Game", 16, 0);	
		else if( Cnt == 3 )
	  		lcd70_show_string_mode(tx+13, ty+74, "Earth", 16, 0);
		else if( Cnt == 4 )
	  		lcd70_show_string_mode(tx+13, ty+74, "Music", 16, 0);
		else if( Cnt == 5 )
	  		lcd70_show_string_mode(tx+13, ty+74, "Clock", 16, 0);
		else if( Cnt == 6 )
	  		lcd70_show_string_mode(tx+13, ty+74, "Apple", 16, 0);
		else if( Cnt == 7 )
	  		lcd70_show_string_mode(tx+13, ty+74, "Radio", 16, 0);
		else if( Cnt == 8 )
	  		lcd70_show_string_mode(tx+13, ty+74, "SMS", 16, 0);

	}
	
	lcd_wr_reg(0x0029); //display on
	POINT_COLOR=BLACK;
	BACK_COLOR=WHITE;

}

//2015.4.29 add
void lcd70_show_string_mode_24x24(unsigned int x,unsigned int y,const unsigned char *p,unsigned int size,unsigned int mode)
{
	if( size == 0 )	  size = 16;

    while(*p!='\0')
    {   
        if(x>MAX_CHAR70_POSX)
		{
			x=0;
			y+=size;
		}
        
		if(y>MAX_CHAR70_POSY)
		{
			y=x=0;
			lcd70_clear_screen(WHITE);
		}
        
		lcd70_show_char_mode_24x24(x,y,*p,size,mode);
		if(size == 16) x = x + size - 2;
		else if(size == 24) x = x + 24 - 6;
		else x = x + size;
        p++;
    }  
}

void lcd70_show_char_mode_24x24(unsigned int x,unsigned int y,unsigned int num,unsigned int size,unsigned int mode)
{       
    unsigned int temp, temp1, temp2, temp3, temp4;
    unsigned int pos,t;      

    if(x>MAX_CHAR70_POSX||y>MAX_CHAR70_POSY)
		return;
   	lcd_wr_reg(0x002A);	
	lcd_wr_data(x>>8);	    
	lcd_wr_data(x&0x00ff);
	lcd_wr_data(x+(size/2-1)>>8);	    
	lcd_wr_data(x+(size/2-1)&0x00ff);
    lcd_wr_reg(0x002b);	
	lcd_wr_data(y>>8);	    
	lcd_wr_data(y&0x00ff);
	lcd_wr_data((y+size-1)>>8);	    
	lcd_wr_data((y+size-1)&0x00ff);
	lcd_wr_reg(0x002c);

	num=num-' ';
	if(!mode)
	{
		for(pos=0;pos<size;pos++)
		{
        	temp1 <<= 1; 
			if(size==16) //14x16 ±Û¾¾ ¾ÕÀ¸·Î ´©¿ò  ¾Õ,µÚ 1°³¾¿ °ø¹é(ÇÈ¼¿) ¾ø¾Ú
			{
				temp1 = ASCII_16X16[num*32 + pos*2];
				temp2 = ASCII_16X16[num*32 + pos*2+1];

				for(t=0;t<7;t++)
		    	{                 
		        	if(temp1 & 0x80)
				 		// lcd_wr_data(POINT_COLOR);
		          		lcd70_draw_point(x+t,y+pos,POINT_COLOR);
					else 
				  		//lcd_wr_data(BACK_COLOR); 
				  		lcd70_draw_point(x+t,y+pos,BACK_COLOR);    
		        	temp1 <<= 1; 
		    	}

				for(t=7;t<14;t++)
		    	{                 
		        	if(temp2 & 0x80)
				 		// lcd_wr_data(POINT_COLOR);
		          		lcd70_draw_point(x+t,y+pos,POINT_COLOR);
					else 
				  		//lcd_wr_data(BACK_COLOR); 
				  		lcd70_draw_point(x+t,y+pos,BACK_COLOR);    
		        	temp2 <<= 1; 
		    	}
			}

			else if(size==24) //18x24 ±ÛÀÚ ¶È¹Ù·Î, ¾Õ3°³, À§ 3°³ °ø¹é(ÇÈ¼¿) ¾ø¾Ú.
			{
				temp1 = ASCII_24X24[num*72 + pos*3];
				temp2 = ASCII_24X24[num*72 + pos*3+1];
				temp3 = ASCII_24X24[num*72 + pos*3+2];

	        	temp1 <<= 3; 
				for(t=0;t<5;t++)
		    	{                 
		        	if(temp1 & 0x80)
				 		// lcd_wr_data(POINT_COLOR);
		          		lcd70_draw_point(x+t,y+pos,POINT_COLOR);
					else 
				  		//lcd_wr_data(BACK_COLOR); 
				  		lcd70_draw_point(x+t,y+pos,BACK_COLOR);    
		        	temp1 <<= 1; 
		    	}

				for(t=5;t<13;t++)
		    	{                 
		        	if(temp2 & 0x80)
				 		// lcd_wr_data(POINT_COLOR);
		          		lcd70_draw_point(x+t,y+pos,POINT_COLOR);
					else 
				  		//lcd_wr_data(BACK_COLOR); 
				  		lcd70_draw_point(x+t,y+pos,BACK_COLOR);    
		        	temp2 <<= 1; 
		    	}

				for(t=13;t<18;t++)
		    	{                 
		        	if(temp3 & 0x80)
				 		// lcd_wr_data(POINT_COLOR);
		          		lcd70_draw_point(x+t,y+pos,POINT_COLOR);
					else 
				  		//lcd_wr_data(BACK_COLOR); 
				  		lcd70_draw_point(x+t,y+pos,BACK_COLOR);    
		        	temp3 <<= 1; 
		    	}
			}


/***
			else temp=asc2_1608[num][pos];

			if(size == 24){	//2015.4.28 test__only
				for(t=0;t<size;t++)
			    {                 
					temp=P2_24[(num%4)*24*24*3 + pos*24*3 + t*3];	//2015.4.28 test__only
			        if(temp&0x01)
					 // lcd_wr_data(POINT_COLOR);
		        	  lcd70_draw_point(x+t,y+pos,POINT_COLOR);
					else 
					  //lcd_wr_data(BACK_COLOR); 
					  lcd70_draw_point(x+t,y+pos,BACK_COLOR);    
			    }
			}
			else

			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)
				 // lcd_wr_data(POINT_COLOR);
		          lcd70_draw_point(x+t,y+pos,POINT_COLOR);
				else 
				  //lcd_wr_data(BACK_COLOR); 
				  lcd70_draw_point(x+t,y+pos,BACK_COLOR);    
		        temp>>=1; 
		    }
***/
		}	
	}
	else
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];
			else temp=asc2_1608[num][pos];
			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)lcd70_draw_point(x+t,y+pos,POINT_COLOR);
		        temp>>=1; 
		    }
		}
	}
} 
