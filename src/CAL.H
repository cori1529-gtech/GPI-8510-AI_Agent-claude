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

#define	 CAL_ZERO_AD_SET	0
#define	 CAL_SPAN_AD_SET	1
#define  CAL_SAMPLING_NUMBER	50

#define SET_END		 	-100	//저장 안하고 SET MODE 종료
#define	SET_FORWARD		-101	//저장 안하고 다음 번호로
#define	SET_BACKWARD	-102	//저장 안하고 앞으로
#define	SET_AGAIN		-103	//다시
#define	SET_RESTART		-194	//처음부터 다시
#define SET_FUNCNO_1 	-201	//SET 모드 1이면 리턴값이 0이므로 -201로 변환해서 반환함





extern unsigned  int  v_division,v_division_2;
extern unsigned  int  v_ei_multiply_factor;
//extern int  v_div = 0;
extern char  str_cal[50];

extern unsigned long  v_zero,v_zero_2;
extern unsigned long  v_span,v_span_2;		// ver 1.30
extern unsigned long  v_e_resolution, v_i_resolution;
//extern unsigned long   v_capacity,v_setting_weight  = 5000;
extern unsigned long   v_capacity,v_capacity_2;
extern unsigned long  lc_capacity;
extern unsigned char   lc_number;
extern signed long v_adc_org,v_adc_org_2;
extern  unsigned long v_adc1_org;
extern unsigned char v_decimal_point;
extern float  gnfFactor,gnfFactor_2;
extern char cal_selection;


/*-------------  calibration mode function  ----------------*/
void    cal_mode(void);
void    cal_mode_2(void);



long   little_span_adjust(void);
long   little_span_adjust_2(void);

void    zero_span_set(unsigned char);
void	factor_access(void);
int     Get_division(void);
int     Get_capacity(void);
long    Get_adc_value(char status);
unsigned long dwValueSet(void); //1114
//void  little_span_adjust(void);
long cal_zero_span_ad_set(int which);
int Set_Weight(void);
int calc_factor(void);
int calc_factor_2(void);

long   Get_Capacity_New(long init_val);




//unsigned int  dp_value_set (unsigned int init_value, char *flag, unsigned int limit);

/*
UINT	dp_value_set(UINT init_value, BYTE *flag, UINT limit);
*/

#endif  /*__CAL_H__*/
/*--------------------  end of file ---------------------*/

