/*
	type.h
*/

#ifndef	__TYPE_H__
#define	__TYPE_H__

#include "stypes.h"

//<<<<<< parsing.c 
#define MAX_TOKEN_QUEUE	10
#define MAX_TOKEN_SIZE	128

typedef struct token_st
{
	char tok_tmp[MAX_TOKEN_SIZE];
	int	 tok_tmp_index;
	
	char tok[MAX_TOKEN_QUEUE][MAX_TOKEN_SIZE];
	int	 head;
	int  tail;
} TOKEN_ST;	


/* Boolean type */
typedef enum { FALSE = 0, TRUE } BOOL;

//전역변수 
struct timedate
{
	int	year;
	int	month;
	int date;
	int hour;
	int min;
	int sec;
	int msec;
};	// tm;

//<<<<<< cdma
struct cdma_info
{
	char telno[13];
	unsigned int  rssi;
	struct timedate tm;
};	// cdma;

//<<<<<< 종량제
struct jangbi_info
{
	unsigned long uid;

	int		send_c1_time;			//배출정보 전송할 시간 분. (TEST ONLY)
	int		send_c1_flag;
	int		send_c1_success_flag;	//배출정보 전송할 플래그

	int		send_c4_time;			//상태정보 전송할 시간 분.
	int		send_c4_flag;
	int		send_c4_success_flag;	//상태정보 전송할 플래그

	struct timedate tm;
	struct cdma_info cdma;
};


//<<<<<< 7인치 lcd graphic
struct bin {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[300 * 400 * 2 + 1];
};


typedef struct
{
	u8  pic_head[2];				//1
	u16 pic_size_l;			    //2
	u16 pic_size_h;			    //3
	u16 pic_nc1;				    //4
	u16 pic_nc2;				    //5
	u16 pic_data_address_l;	    //6
	u16 pic_data_address_h;		//7	
	u16 pic_message_head_len_l;	//8
	u16 pic_message_head_len_h;	//9
	u16 pic_w_l;					//10
	u16 pic_w_h;				    //11
	u16 pic_h_l;				    //12
	u16 pic_h_h;				    //13	
	u16 pic_bit;				    //14
	u16 pic_dip;				    //15
	u16 pic_zip_l;			    //16
	u16 pic_zip_h;			    //17
	u16 pic_data_size_l;		    //18
	u16 pic_data_size_h;		    //19
	u16 pic_dipx_l;			    //20
	u16 pic_dipx_h;			    //21	
	u16 pic_dipy_l;			    //22
	u16 pic_dipy_h;			    //23
	u16 pic_color_index_l;	    //24
	u16 pic_color_index_h;	    //25
	u16 pic_other_l;			    //26
	u16 pic_other_h;			    //27
	u16 pic_color_p01;		    //28
	u16 pic_color_p02;		    //29
	u16 pic_color_p03;		    //30
	u16 pic_color_p04;		    //31
	u16 pic_color_p05;		    //32
	u16 pic_color_p06;		    //33
	u16 pic_color_p07;		    //34
	u16 pic_color_p08;			//35			
}BMP_HEAD;

typedef struct
{
	u16 x;
	u16 y;
	u8  r;
	u8  g;
	u8  b;		
}BMP_POINT;


//<<<<<< 7인치 lcd 종량제 램프
struct lamp{
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[70 * 70 * 2 + 1];
};

#endif	//#ifdef __TYPE_H__

/* end of file */
