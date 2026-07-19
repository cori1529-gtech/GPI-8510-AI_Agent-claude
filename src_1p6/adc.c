/*
	adc.c
*/

#include <stdio.h>

#include "stm32f10x.h"
#include "hw_config.h"

#include "global.h"

#include "adc.h"
#include "var.h"
#include "usart.h"


#define ERROR_WEIGHT 20	
#define ONE_MINUTE   100	

extern long gnlDisplay;

long Get_adc_from_module_20160629(void) //1116
{
	int	iz;
	long weight;
	char sT0[50];	//0116
	long Display_Weight = -1;
	
	// 422 송신 프로토콜 	
	sprintf(sT0, "@-%cR", rotation+0x30); 
	USART2_puts(sT0);
//USART1_print(sT0);
  
	rotation++;
	if(rotation>4) rotation=1;
	
	for(iz=0; iz<100; iz++) //typical response time 45ms~47ms
	{  	
		//delay_ms(100);   // critical !! module로부터 응답을 기다리는 시간, 굉장히 중요함  
		delay_ms(1);
		if(Usart2_Interrupt_flag)
		{
			delay_ms(1);   // critical !! module로부터 응답을 기다리는 시간, 굉장히 중요함  
			break;
		}
	}
//sprintf(sT0, " iz=%d.", iz); 
//USART1_print(sT0);

	
   	// 1,2,3,4 순서대로 ad 모듈을 스캐닝한다 
//==============================
	if(Usart2_Interrupt_flag) // a/d 모듈이  
	{
	    Usart2_Interrupt_flag = 0;
		RxCounter2 = 0;
		
		//junction power off delay counter
        Adc_Counter = OPTION_POWER_COUNTER; // 20 을 기본으로 무조건 넣는다. 
		
		weight = 0;
///		weight = (cmd_2[2]-0x30)*100000+(cmd_2[3]-0x30)*10000+(cmd_2[4]-0x30)*1000+(cmd_2[5]-0x30)*100+(cmd_2[6]-0x30)*10+(cmd_2[7]-0x30);
		for(iz=2; iz<=7; iz++)
		{
			if(('0'<=cmd_2[iz] && cmd_2[iz]<='9') || ('A'<=cmd_2[iz] && cmd_2[iz]<='F'))
			{	
				weight = (weight * 10) + cmd_2[iz]-'0';
				continue;
			}
			else break;
		}
		if(iz < 8) weight = -1;
		else
		{
			//address check
			if( !strncmp(cmd_2, "-1", 2)) 
			{
				One_Weight = weight;
				One_Weight_Ok=1;
				One_Weight_Error_Counter=0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
			}
			else if( !strncmp(cmd_2, "-2", 2)) 
			{
				Two_Weight = weight;
				Two_Weight_Ok=1;
				Two_Weight_Error_Counter=0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
			}
			else if( !strncmp(cmd_2, "-3", 2)) 
			{
				Three_Weight = weight;
				Three_Weight_Ok=1;
				Three_Weight_Error_Counter=0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
			}
			else if( !strncmp(cmd_2, "-4", 2)) 
			{
				Four_Weight = weight;
				Four_Weight_Ok=1;
				Four_Weight_Error_Counter=0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
			}
		}
			
//sprintf(sT0, " w=%ld %s", weight, cmd_2);
//lcd70_show_string_mode(400, 30+(cmd_2[1]-'0')*30, (const unsigned char *)sT0, 20, 0);
//USART1_print(sT0);
			 
		for(iz=0;iz<15;iz++)
		{
			cmd_2[iz]=0xff;
			RxBuffer2[iz]=0xff;
		}
	} //if(Usart2_Interrupt_flag) // a/d 모듈이	
	else  // return이 없으면 
	{
		if(Adc_Counter) Adc_Counter--;
	}

	if(rotation==1) // module전부를 한cycle scaning 이 끝나면 	 
	{
//sprintf(sT0, "%d, %d, %d, %d.", One_Weight_Ok, Two_Weight_Ok, Three_Weight_Ok, Four_Weight_Ok);
//lcd70_show_string_mode(400, 30+(5*30), (const unsigned char *)sT0, 20, 0);
		if(One_Weight_Ok&& Two_Weight_Ok && Three_Weight_Ok &&Four_Weight_Ok) // 무게 4개가 다알아오면 
	   	{
	  		Display_Weight=(One_Weight+Two_Weight+Three_Weight+Four_Weight);
        	One_Weight_Ok=0;
		  	Two_Weight_Ok=0;
		  	Three_Weight_Ok=0;
		  	Four_Weight_Ok=0;	
	  		Weight_flag=1;
	   	} // 무게가 4개 다날아오면 
	 	///////////////////// 통신에러가 발생시 ///////////////////////
	  	else if(!One_Weight_Ok && Two_Weight_Ok && Three_Weight_Ok && Four_Weight_Ok) // 1번 로드셀이 통신두절이면  
		{
			// 에러갯수 조건 집어넣어야하고
			// cal시 비율 조건 계산해야하고 
			One_Weight_Error_Counter++;
			if(One_Weight_Error_Counter>=2)
			{
				Display_Weight=(Two_Weight*2+Three_Weight+Four_Weight);
				One_Weight_Error_Counter=0;
			}
			One_Weight_Ok=0;
			Two_Weight_Ok=0;
			Three_Weight_Ok=0;
			Four_Weight_Ok=0;
			Weight_flag=1;
		}
		// 2번이 통신 두절되면 
		else if(One_Weight_Ok && !Two_Weight_Ok && Three_Weight_Ok && Four_Weight_Ok) // 2번 로드셀이 통신두절이면  
		{
			// 에러갯수 조건 집어넣어야하고	
			// cal시 비율 조건 계산해야하고 
			Two_Weight_Error_Counter++;
			if(Two_Weight_Error_Counter>=2)
			{
				Display_Weight=(One_Weight*2+Three_Weight+Four_Weight);
				Two_Weight_Error_Counter=0;
			}
			One_Weight_Ok=0;
			Two_Weight_Ok=0;
			Three_Weight_Ok=0;
			Four_Weight_Ok=0;
			Weight_flag=1;
		} // 2번이 통신 두절되면  
		// 3번 로드셀이 통신두절이면  
		else if(One_Weight_Ok && Two_Weight_Ok && !Three_Weight_Ok && Four_Weight_Ok) // 3번 로드셀이 통신두절이면  
		{
			// 에러갯수 조건 집어넣어야하고
			// cal시 비율 조건 계산해야하고 
			Three_Weight_Error_Counter++;
			if(Three_Weight_Error_Counter>=2)
			{
				Display_Weight=(One_Weight+Two_Weight+Four_Weight*2);
				Three_Weight_Error_Counter=0;
			}
			One_Weight_Ok=0;
			Two_Weight_Ok=0;
			Three_Weight_Ok=0;
			Four_Weight_Ok=0;
			Weight_flag=1;
		} // 3번이 통신 두절되면 
		// 4번 로드셀이 통신두절이면  
	  	else if(One_Weight_Ok && Two_Weight_Ok && Three_Weight_Ok && !Four_Weight_Ok) // 4번 로드셀이 통신두절이면  
   		{
			// 에러갯수 조건 집어넣어야하고
			// cal시 비율 조건 계산해야하고 
			Four_Weight_Error_Counter++;
			if(Four_Weight_Error_Counter>=2)
			{
				Display_Weight=(One_Weight+Two_Weight+Three_Weight*2);
				Four_Weight_Error_Counter=0;
			}
			One_Weight_Ok=0;
			Two_Weight_Ok=0;
			Three_Weight_Ok=0;
			Four_Weight_Ok=0;
			Weight_flag=1;
		} // 4번이 통신 두절되면  
		// 1,2번 로드셀 통신두절이 되면   
		else if(!One_Weight_Ok && !Two_Weight_Ok && Three_Weight_Ok && Four_Weight_Ok) // 1,2번 로드셀 통신두절이 되면   
		{
			// 에러갯수 조건 집어넣어야하고
			// cal시 비율 조건 계산해야하고 
			One_Weight_Error_Counter++;
			if(One_Weight_Error_Counter>=2)
			{
				Display_Weight=(Three_Weight+Four_Weight)*2;  //3번4번 출력으로만 무게계산 
				One_Weight_Error_Counter=0;
			}
			One_Weight_Ok=0;
			Two_Weight_Ok=0;
			Three_Weight_Ok=0;
			Four_Weight_Ok=0;
			Weight_flag=1;
		} // 1,3 번이 통신 두절되면 
		// 1,3번 로드셀 통신두절이 되면   
		else if(!One_Weight_Ok && Two_Weight_Ok && !Three_Weight_Ok && Four_Weight_Ok) // 1,3번 로드셀 통신두절이 되면   
		{
			// 에러갯수 조건 집어넣어야하고
			// cal시 비율 조건 계산해야하고 
			One_Weight_Error_Counter++;
			if(One_Weight_Error_Counter>=2)
			{
				Display_Weight=(Two_Weight+Four_Weight)*2;  //2번4번 출력으로만 무게계산 
				One_Weight_Error_Counter=0;
			}
			One_Weight_Ok=0;
			Two_Weight_Ok=0;
			Three_Weight_Ok=0;
			Four_Weight_Ok=0;
			Weight_flag=1;
		} // 1,3 번이 통신 두절되면 
		// 1,4번 로드셀 통신두절이 되면   
		else if( !One_Weight_Ok && Two_Weight_Ok && Three_Weight_Ok && !Four_Weight_Ok) // 1,4번 로드셀 통신두절이 되면   
		{
			// 에러갯수 조건 집어넣어야하고
			// cal시 비율 조건 계산해야하고 
			One_Weight_Error_Counter++;
			if(One_Weight_Error_Counter>=2)
			{
				Display_Weight=(Two_Weight+Three_Weight)*2;  //2번3번 출력으로만 무게계산 
				One_Weight_Error_Counter=0;
			}
			One_Weight_Ok=0;
			Two_Weight_Ok=0;
			Three_Weight_Ok=0;
			Four_Weight_Ok=0;
			Weight_flag=1;
		} // 1,4 번이 통신 두절되면 
		// 2,3번 로드셀 통신두절이 되면   
		else if( One_Weight_Ok && !Two_Weight_Ok && !Three_Weight_Ok && Four_Weight_Ok) // 2,3번 로드셀 통신두절이 되면   
		{
			// 에러갯수 조건 집어넣어야하고
			// cal시 비율 조건 계산해야하고 
			Two_Weight_Error_Counter++;
			if(Two_Weight_Error_Counter>=2)
			{
				Display_Weight=(One_Weight+Four_Weight)*2;  //1번4번 출력으로만 무게계산 
				Two_Weight_Error_Counter=0;
			}
			One_Weight_Ok=0;
			Two_Weight_Ok=0;
			Three_Weight_Ok=0;
			Four_Weight_Ok=0;
			Weight_flag=1;
		} // 2,3 번이 통신 두절되면 
		// 2,4번 로드셀 통신두절이 되면   
		else if( One_Weight_Ok && !Two_Weight_Ok && Three_Weight_Ok && !Four_Weight_Ok) // 2,4번 로드셀 통신두절이 되면   
		{
			// 에러갯수 조건 집어넣어야하고
			// cal시 비율 조건 계산해야하고 
			Two_Weight_Error_Counter++;
			if(Two_Weight_Error_Counter>=2)
			{
				Display_Weight=(One_Weight+Three_Weight)*2;  //1번3번 출력으로만 무게계산 
				Two_Weight_Error_Counter=0;
			}
			One_Weight_Ok=0;
			Two_Weight_Ok=0;
			Three_Weight_Ok=0;
			Four_Weight_Ok=0;
			Weight_flag=1;
		} // 2,4 번이 통신 두절되면 
		// 3,4번 로드셀 통신두절이 되면   
		else if( One_Weight_Ok && Two_Weight_Ok && !Three_Weight_Ok && !Four_Weight_Ok) // 3,4번 로드셀 통신두절이 되면   
		{
			// 에러갯수 조건 집어넣어야하고
			// cal시 비율 조건 계산해야하고 
			Three_Weight_Error_Counter++;
			if(Three_Weight_Error_Counter>=2)
			{
				Display_Weight=(One_Weight+Two_Weight)*2;  //1번2번 출력으로만 무게계산 
				Three_Weight_Error_Counter=0;
			}
			One_Weight_Ok=0;
			Two_Weight_Ok=0;
			Three_Weight_Ok=0;
			Four_Weight_Ok=0;
			Weight_flag=1;
		} // 1,2 번이 통신 두절되면 
			
			
//sprintf(sT0, "%ld:%ld, %ld, %ld, %ld %ld.", Weight_flag, One_Weight, Two_Weight, Three_Weight, Four_Weight, Display_Weight);
//USART1_puts(sT0); 
		

if(gnlDisplay)
{
	sprintf(sT0, "%ld, %ld, %ld, %ld %ld.", One_Weight, Two_Weight, Three_Weight, Four_Weight, Display_Weight);
	//lcd70_show_string_mode(300, 150+(5*30), (const unsigned char *)sT0, 20, 0);
	lcd70_show_string_mode(10, 400, (const unsigned char *)sT0, 20, 0);
}
	} // if rotation==1 이면 4번 다하고나면 

	return Display_Weight;
}



long Get_adc_from_module(void) //1116
{
	int	iz;
	long Display_Weight;
	char sT0[50];	//0116

	// 422 송신 프로토콜 
	//request adc data to junction box
	sprintf(sT0, "@-%cR", rotation+0x30); 
	USART2_puts(sT0);
//USART1_print(sT0);
  
	rotation++;
	if(rotation>4) rotation=1;
	
	//wait response from junction box
	for(iz=0; iz<100; iz++) //typical response time 45ms~47ms
	{  	
		//delay_ms(100);   // critical !! module로부터 응답을 기다리는 시간, 굉장히 중요함  
		delay_ms(1);
		if(Usart2_Interrupt_flag)
		{
			delay_ms(1);   // critical !! module로부터 응답을 기다리는 시간, 굉장히 중요함  
			break;
		}
	}
//sprintf(sT0, " iz=%d.", iz); 
//USART1_puts(sT0);

	
   	// 1,2,3,4 순서대로 ad 모듈을 스캐닝한다 
//==============================
	if(Usart2_Interrupt_flag) // a/d 모듈이  
	{
        Adc_Counter=OPTION_POWER_COUNTER; // 20 을 기본으로 무존건 넣은다. 
	    Usart2_Interrupt_flag=0;
		RxCounter2=0;	     
	
		switch(cmd_2[1]) //address
		{
			case '1':
		    	One_Weight=(cmd_2[2]-0x30)*100000+(cmd_2[3]-0x30)*10000+(cmd_2[4]-0x30)*1000+(cmd_2[5]-0x30)*100+(cmd_2[6]-0x30)*10+(cmd_2[7]-0x30);
		    	One_Weight_Ok=1;
				One_Weight_Error_Counter=0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
		   		break;
		  	case '2':
		    	Two_Weight=(cmd_2[2]-0x30)*100000+(cmd_2[3]-0x30)*10000+(cmd_2[4]-0x30)*1000+(cmd_2[5]-0x30)*100+(cmd_2[6]-0x30)*10+(cmd_2[7]-0x30);
		    	Two_Weight_Ok=1;
				Two_Weight_Error_Counter=0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
		   		break;
          	case '3':
				Three_Weight=(cmd_2[2]-0x30)*100000+(cmd_2[3]-0x30)*10000+(cmd_2[4]-0x30)*1000+(cmd_2[5]-0x30)*100+(cmd_2[6]-0x30)*10+(cmd_2[7]-0x30);
		    	Three_Weight_Ok=1;
				Three_Weight_Error_Counter=0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
		   		break;
          	case '4':
		  		Four_Weight=(cmd_2[2]-0x30)*100000+(cmd_2[3]-0x30)*10000+(cmd_2[4]-0x30)*1000+(cmd_2[5]-0x30)*100+(cmd_2[6]-0x30)*10+(cmd_2[7]-0x30);
		  		Four_Weight_Ok=1;
				Four_Weight_Error_Counter=0; // 한번이라도 정상으로 들어오면 에러 카운터 clear
		   		break;
		   default:
		     	break;
        }
		
		cmd_2[10] = 0;
sprintf(sT0, "%s", cmd_2);
//lcd70_show_string_mode(400, 30+(cmd_2[1]-'0')*30, (const unsigned char *)sT0, 20, 0);
USART1_puts(sT0);
	 	 
		for(iz=0;iz<15;iz++)
		{
			cmd_2[iz]=0xff;
			RxBuffer2[iz]=0xff;
		}
	} //if(Usart2_Interrupt_flag) // a/d 모듈이	
	else  // return이 없으면 
	{
		if(Adc_Counter) Adc_Counter--;
	}

	if(rotation==1) // module전부를 한cycle scaning 이 끝나면 	 
	{


//sprintf(sT0, "%d, %d, %d, %d.", One_Weight_Ok, Two_Weight_Ok, Three_Weight_Ok, Four_Weight_Ok);
//lcd70_show_string_mode(400, 30+(5*30), (const unsigned char *)sT0, 20, 0);


		if(One_Weight_Ok&& Two_Weight_Ok && Three_Weight_Ok &&Four_Weight_Ok) // 무게 4개가 다알아오면 
	   	{
	  		Display_Weight=(One_Weight+Two_Weight+Three_Weight+Four_Weight);
        	One_Weight_Ok=0;
		  	Two_Weight_Ok=0;
		  	Three_Weight_Ok=0;
		  	Four_Weight_Ok=0;	

if(gnlDisplay)
{
	sprintf(sT0, "%ld, %ld, %ld, %ld %ld.", One_Weight, Two_Weight, Three_Weight, Four_Weight, Display_Weight);
	//lcd70_show_string_mode(300, 150+(5*30), (const unsigned char *)sT0, 20, 0);
	lcd70_show_string_mode(10, 400, (const unsigned char *)sT0, 20, 0);
}

      		for(iz=0;iz<15;iz++)
	     	{
		   		cmd_2[iz]=0xff;
		   		RxBuffer2[iz]=0xff;
		 	}
	  		Weight_flag=1;
			RxCounter2=0;
	    	return Display_Weight;
	   	} // 무게가 4개 다날아ㅇ오면 


	 	///////////////////// 통신에러가 발생시 ///////////////////////
		if(!One_Weight_Ok ||  !Two_Weight_Ok  ||  !Three_Weight_Ok  || !Four_Weight_Ok) // 4개중에 하나라도 통신에러가 발생하면 
	  	{
	  		if(!One_Weight_Ok&& Two_Weight_Ok && Three_Weight_Ok &&Four_Weight_Ok) // 1번 로드셀이 통신두절이면  
	   		{
         		// 에러갯수 조건 집어넣어야하고
		 		// cal시 비율 조건 계산해야하고 
		  		One_Weight_Error_Counter++;
		  		if(One_Weight_Error_Counter>=2)
		  		{
	  	   			Display_Weight=(Two_Weight*2+Three_Weight+Four_Weight);
		   			One_Weight_Error_Counter=0;
		  		}
          		One_Weight_Ok=0;
		  		Two_Weight_Ok=0;
		  		Three_Weight_Ok=0;
		  		Four_Weight_Ok=0;
         		for(iz=0;iz<15;iz++)
	     		{
		   			cmd_2[iz]=0xff;
		   			RxBuffer2[iz]=0xff;
		 		}
	  			Weight_flag=1;
				RxCounter2=0;
	    		return Display_Weight;
	   		} // 1,2번이 통신 두절되면 
	
      		if(One_Weight_Ok&& !Two_Weight_Ok && Three_Weight_Ok &&Four_Weight_Ok) // 2번 로드셀이 통신두절이면  
	   		{
         		// 에러갯수 조건 집어넣어야하고	
		 		// cal시 비율 조건 계산해야하고 
		  		Two_Weight_Error_Counter++;
		  		if(Two_Weight_Error_Counter>=2)
		  		{
	  	   			Display_Weight=(One_Weight*2+Three_Weight+Four_Weight);
		   			Two_Weight_Error_Counter=0;
		  		}
          		One_Weight_Ok=0;
		  		Two_Weight_Ok=0;
		  		Three_Weight_Ok=0;
		  		Four_Weight_Ok=0;
         		for(iz=0;iz<15;iz++)
	     		{
		   			cmd_2[iz]=0xff;
		   			RxBuffer2[iz]=0xff;
		 		}
	  			Weight_flag=1;
				RxCounter2=0;
	    		return Display_Weight;
	   		} // 2번이 통신 두절되면  
	
 	   		if(One_Weight_Ok&& Two_Weight_Ok && !Three_Weight_Ok &&Four_Weight_Ok) // 3번 로드셀이 통신두절이면  
	   		{
         		// 에러갯수 조건 집어넣어야하고
		 		// cal시 비율 조건 계산해야하고 
		  		Three_Weight_Error_Counter++;
		  		if(Three_Weight_Error_Counter>=2)
		  		{
	  	   			Display_Weight=(One_Weight+Two_Weight+Four_Weight*2);
		   			Three_Weight_Error_Counter=0;
		  		}
          		One_Weight_Ok=0;
		  		Two_Weight_Ok=0;
		  		Three_Weight_Ok=0;
		  		Four_Weight_Ok=0;
         		for(iz=0;iz<15;iz++)
	     		{
		   			cmd_2[iz]=0xff;
		   			RxBuffer2[iz]=0xff;
		 		}
	  			Weight_flag=1;
				RxCounter2=0;
	    		return Display_Weight;
	   		} // 3번이 통신 두절되면 
	
	  		if(One_Weight_Ok&& Two_Weight_Ok && Three_Weight_Ok &&!Four_Weight_Ok) // 4번 로드셀이 통신두절이면  
	   		{
         		// 에러갯수 조건 집어넣어야하고
		 		// cal시 비율 조건 계산해야하고 
		  		Four_Weight_Error_Counter++;
		  		if(Four_Weight_Error_Counter>=2)
		  		{
	  	   			Display_Weight=(One_Weight+Two_Weight+Three_Weight*2);
		   			Four_Weight_Error_Counter=0;
		  		}
          		One_Weight_Ok=0;
		  		Two_Weight_Ok=0;
		  		Three_Weight_Ok=0;
		  		Four_Weight_Ok=0;
         		for(iz=0;iz<15;iz++)
	     		{
		   			cmd_2[iz]=0xff;
		   			RxBuffer2[iz]=0xff;
		 		}
	  			Weight_flag=1;
				RxCounter2=0;
	    		return Display_Weight;
	   		} // 4번이 통신 두절되면  


	   		if(!One_Weight_Ok && !Two_Weight_Ok && Three_Weight_Ok &&Four_Weight_Ok) // 1,2번 로드셀 통신두절이 되면   
	   		{
         		// 에러갯수 조건 집어넣어야하고
		 		// cal시 비율 조건 계산해야하고 
		  		One_Weight_Error_Counter++;
		  		if(One_Weight_Error_Counter>=2)
		  		{
	  	   			Display_Weight=(Three_Weight+Four_Weight)*2;  //3번4번 출력으로만 무게계산 
		   			One_Weight_Error_Counter=0;
		  		}
          		One_Weight_Ok=0;
		  		Two_Weight_Ok=0;
		  		Three_Weight_Ok=0;
		  		Four_Weight_Ok=0;
         		for(iz=0;iz<15;iz++)
	     		{
		   			cmd_2[iz]=0xff;
		   			RxBuffer2[iz]=0xff;
		 		}
	  			Weight_flag=1;
				RxCounter2=0;
	    		return Display_Weight;
	   		} // 1,2 번이 통신 두절되면 

	  		if(One_Weight_Ok&& Two_Weight_Ok && !Three_Weight_Ok &&!Four_Weight_Ok) // 1,2번 로드셀 통신두절이 되면   
	   		{
         		// 에러갯수 조건 집어넣어야하고
		 		// cal시 비율 조건 계산해야하고 
		 		Three_Weight_Error_Counter++;
		  		if(Three_Weight_Error_Counter>=2)
		  		{
	  	   			Display_Weight=(One_Weight+Two_Weight)*2;  //3번4번 출력으로만 무게계산 
		   			Three_Weight_Error_Counter=0;
		  		}
          		One_Weight_Ok=0;
		  		Two_Weight_Ok=0;
		  		Three_Weight_Ok=0;
		  		Four_Weight_Ok=0;
         		for(iz=0;iz<15;iz++)
	     		{
		   			cmd_2[iz]=0xff;
		   			RxBuffer2[iz]=0xff;
		 		}
	  			Weight_flag=1;
				RxCounter2=0;
	    		return Display_Weight;
	   		} // 1,2 번이 통신 두절되면 

		} //4개중에서 하나라도 통신에러가 생기면 
		///////////////////// 통신에러가 발생시 ///////////////////////
	 
	} // if rotation==1 이면 4번 다하고나면 

	return -1;
}


/* end of file */
