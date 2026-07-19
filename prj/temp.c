						//20180716
											gnlTotalChickenCount++;
                      gnlTotalChickenWeight+=Received_Chicken_Data;
											gnlChickenAverage=gnlTotalChickenWeight/gnlTotalChickenCount; //평균무게 
										//	Max_weight, Min_weight
											
											Max_weight=(float)(gnlChickenAverage*1.1);
											Min_weight=(float)(gnlChickenAverage*0.9);
											
											if(Received_Chicken_Data>=Min_weight && Received_Chicken_Data<=Max_weight)
											 gnlUniformatyCount++;
									
											utemp1=gnlUniformatyCount;
											utemp2= gnlTotalChickenCount;
                      Uniformatiy=(float)((utemp1/utemp2)*100);
 
											sprintf(str,"%4ld-%4ld-%1ld-%2.1f",(long)Min_weight,(long)Max_weight,(long)gnlUniformatyCount,(float)Uniformatiy);
                       GLCD_string (0,0 ,str);
											gnlVariation_temp=abs(gnlChickenAverage-Received_Chicken_Data);
											gnlVariation_temp1=gnlVariation_temp*gnlVariation_temp;	
									    gnlVariationSumWeight+=gnlVariation_temp1;
										  gnlVariation=gnlVariationSumWeight/(gnlTotalChickenCount);
											Standart_deviation=sqrt(gnlVariation);
												
											New_Average=gnlChickenAverage;
											Standart_deviation_temp=Standart_deviation;
											
											
												Coefficient_variation=(float)(Standart_deviation_temp/New_Average);
											 Coefficient_variation=Coefficient_variation*100; //변동계수 구하기
									
									   sprintf(str, "평균무게:%4ld",(long)gnlChickenAverage);
                     GLCD_string (0,2 ,str);
									   lcd_normal();
	             									
									   sprintf(str, "S.D:%3ld%C.V:%2.1f %%",(long)Standart_deviation,(float)Coefficient_variation);
                     GLCD_string (0,4 ,str);

										 lcd_inverse();
										 sprintf(str, "1동_2번:%4ld g",(long)Received_Chicken_Data);
                     GLCD_string (0,6 ,str);
					