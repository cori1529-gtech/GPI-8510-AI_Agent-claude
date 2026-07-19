/*
		CAL.H

	VER 1.10 - 1997. 12. 8.  GI-8500
	  1. COMPILER  IAR 4.20

	Copyright (c) 1997 by G-TECH
	All Rights Reserved.
*/

#ifndef __CAL_H__
#define __CAL_H__

/* CALIBRATION */
#define CAL_SAMPLING_NUMBER	10
#define ZERO_LOW_LIMIT		100L   		/* 500000L */
//#define ZERO_HIGH_LIMIT		500000L		/* 600000L */
#define ZERO_HIGH_LIMIT		50000000L		/* 600000L */
#define ZERO_ERROR_LIMIT    20000000L
//#define FULL_LIMIT			10000000L	/* 500000L */
#define FULL_LIMIT			16000000L	/* 500000L */


extern unsigned  int  v_division;
extern unsigned  int  v_ei_multiply_factor;
//extern int  v_div = 0;
extern char  str_cal[50];

extern unsigned long  v_zero;
extern unsigned long  v_span;		// ver 1.30
extern unsigned long  v_e_resolution, v_i_resolution;
//extern unsigned long   v_capacity,v_setting_weight  = 5000;
extern unsigned long   v_capacity;
extern unsigned long  lc_capacity;
extern unsigned char   lc_number;
extern signed long v_adc_org;
extern  unsigned long v_adc1_org;
extern unsigned char v_decimal_point;
extern float  gnfFactor;



/*-------------  calibration mode function  ----------------*/
void    cal_mode(void);
void    little_span_adjust(void);
void    zero_span_set(unsigned char);
void	factor_access(void);
int     Get_division(void);
int     Get_capacity(void);
long    Get_adc_value(char status);
unsigned long dwValueSet(void); //1114
void  little_span_adjust(void);


//unsigned int  dp_value_set (unsigned int init_value, char *flag, unsigned int limit);

/*
UINT	dp_value_set(UINT init_value, BYTE *flag, UINT limit);
*/

#endif  /*__CAL_H__*/
/*--------------------  end of file ---------------------*/

