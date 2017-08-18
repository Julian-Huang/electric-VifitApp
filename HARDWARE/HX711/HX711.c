#include "HX711.h"   
#include <stdio.h>
#include "json.h"
#include "delay.h"
#include "usart.h"	
#include <string.h>


u8 mark = 0;
u8 mark_stop = 0;

void Strenth_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  //开启称重模块端口(AHB1)的时钟
  RCC_AHB1PeriphClockCmd(weight_CLK, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = ADDT1_PIN|ADDT2_PIN|ADDT3_PIN|ADDT4_PIN|ADDT5_PIN|ADDT6_PIN|ADDT7_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;   
  GPIO_Init(weight_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = ADSK1_PIN|ADSK2_PIN|ADSK3_PIN|ADSK4_PIN|ADSK5_PIN|ADSK6_PIN|ADSK7_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   
  GPIO_Init(weight_PORT,&GPIO_InitStructure);
}

/***************
作用：	读取HX711数据
函数名：ReadCount
参数:		uint16_t GPIO_ADSK,uint16_t GPIO_ADDT
返回值：数据值
***************/
unsigned long ReadCount(uint16_t GPIO_ADSK,uint16_t GPIO_ADDT)
{
  unsigned long Count;
  uint16_t i;
	
  GPIO_ResetBits(ADSK1, GPIO_ADSK);//ADSK = 0
  Count = 0;
  while(GPIO_ReadInputDataBit(ADDT1, GPIO_ADDT));   //检测数据线是否为高电平
	
  for(i = 0; i <24; i++)
  {
    GPIO_SetBits(ADSK1, GPIO_ADSK);//ADSK = 1;
    Count = Count << 1;
    GPIO_ResetBits(ADSK1, GPIO_ADSK);//ADSK = 0;
    if(GPIO_ReadInputDataBit(ADDT1, GPIO_ADDT))Count++;
  }
	
  GPIO_SetBits(ADSK1, GPIO_ADSK);//ADSK = 1;
  Count = Count^0x800000;
  GPIO_ResetBits(ADSK1, GPIO_ADSK);//ADSK = 0;
  
	return(Count);
	
}

/*****************
作用：	测量者大腿力
函数名：LiLiang1_test
参数：	uint16_t GPIO_ADSK,uint16_t GPIO_ADDT,uint8_t type
返回值：无
*****************/
void LiLiang1_test(uint16_t GPIO_ADSK,uint16_t GPIO_ADDT,uint8_t type)
{
	json_t *text_0;
	char *out;
	uint16_t times=0;
	uint32_t i,j;
	long long NumAvg_1 ;
	long long NumSum_1 ;
	long long Num;
	double Num_display;
	
  for(j = 0; j < 10; j++)
  {
    for(i = 0xfffff; i > 0; i--);
    NumSum_1 += ReadCount(GPIO_ADSK,GPIO_ADDT);
  }
  NumAvg_1 = NumSum_1 / 10;


	while(times<150)   //大致10S
  {
    Num = ReadCount(GPIO_ADSK,GPIO_ADDT);
//		printf("%lld\r\n",Num);
		
    times++;
		
    switch(type)
		{
			
			case 1: Num_display=(double)(Num-NumAvg_1) * 0.007 + 9;                      //右腿力
			    		printf("right_thigh_strength: %d g\n",(int)Num_display);
              text_0 = json_pack("{si}","weight",(int)Num_display);
		          out = json_dumps(text_0,JSON_ENCODE_ANY);
		          printf("%s\r\n", out);                       break;			                   
			
			case 2: Num_display=(double)(Num-NumAvg_1) * 0.008 + 9;                      //左腿力             
			        printf("left_thigh_strength: %d g\n",(int)Num_display);
              text_0 = json_pack("{si}","weight",(int)Num_display);
		          out = json_dumps(text_0,JSON_ENCODE_ANY);
		          printf("%s\r\n", out);                       break;                
		}				
    
		free(out);
	  json_decref(text_0);
	  free(text_0);
		
    for(i = 0x8fff; i > 0; i--);
		
  }
}

/********************
作用：	臂升力的测量
函数名：LiLiang2_test
参数：	无
返回值：无
********************/
void LiLiang2_test(void)	
{
	json_t *text_1;
	json_t *text_2;
	char *out1;
	char *out2;
	uint16_t times=0;
	uint32_t i,j;
	double NumAvg_1,NumAvg_2;
	double NumSum_1,NumSum_2 ;
	double Num1, Num2 ;
	double Num_display1,Num_display2 ;
	
	
	for(j = 0; j < 5; j++)
  {
    NumSum_1 += ReadCount(ADSK2_PIN,ADDT2_PIN);
		delay_us(5);
		NumSum_2 += ReadCount(ADSK1_PIN,ADDT1_PIN);
		delay_us(5);
		
  }
  NumAvg_1 = NumSum_1 / 5;
	NumAvg_2 = NumSum_2 / 5;
	
	while(times<300)   //大致10S
  {
		
    Num1 = ReadCount(ADSK2_PIN,ADDT2_PIN);
		delay_us(5);
		Num2 = ReadCount(ADSK1_PIN,ADDT1_PIN);
		delay_us(5);
		
//		printf("%lld\r\n",Num);
		times++;

    
    Num_display1 = (double)(Num1-NumAvg_1) * 0.006 + 9;	        //右臂升力
		Num_display2 = (double)(Num2-NumAvg_2) * 0.006 + 9;					//左臂升力
    
    printf("arm_right_strenth: %.0f\n\n",Num_display1);
		delay_us(5);
		printf("arm_left_strenth: %.0f\n",Num_display2);
//		text_1 = json_pack("{si}","arm_right_strenth",(int)Num_display1);      //右背扩力
//		out1 = json_dumps(text_1,JSON_ENCODE_ANY);
//		printf("%s\r\n", out1);
		
//		text_2 = json_pack("{si}","arm_left_strenth",(int)Num_display2);       //左背扩力
//		out2 = json_dumps(text_2,JSON_ENCODE_ANY);
//		printf("%s\r\n", out2);
		
		free(out1);
	  json_decref(text_1);
	  free(text_1);
		
		free(out2);
	  json_decref(text_2);
	  free(text_2);
		
		
    for(i = 0x8fff; i > 0; i--);
  }

}

/********************
作用：	背扩力的测量
函数名：LiLiang2_test
参数：	无
返回值：无
********************/
void LiLiang3_test(void)	
{
	json_t *text_1;
	json_t *text_2;
	char *out1;
	char *out2;
	uint16_t times=0;
	uint32_t i,j;
	long long NumAvg_1,NumAvg_2;
	long long NumSum_1,NumSum_2 ;
	long long Num1, Num2 ;
	double Num_display1,Num_display2;
	int16_t height = 170;
	u8  t,len;
	u8 *p;
	const char back_strength[]="back_strength";
	
	USART2_printf(USART2,"require\r\n");
	
	do
	{
	 if(USART_RX_STA&0x8000)
	 {
		 
		 
		 if( strcmp((char *)USART_RX_BUF,back_strength) == 0 )
		 {
			 if(mark==1)
			 {
				USART2_RX_STA = 0;
				USART2_printf(USART2,"require\r\n");
			  while((USART2_RX_STA&0x8000) == 0);
				len = USART2_RX_STA&0x3fff;					//得到此次接收到的数据长度
				height = atof((char*)USART2_RX_BUF);
				
				for(t=0;t<len;t++)
				{		
					 USART2_RX_BUF[t]=NULL; 
				}
				USART2_RX_STA = 0;
				mark = 0;
			}
						
			Num1 = ReadCount(ADSK1_PIN,ADDT1_PIN);
			delay_us(5);
			Num2 = ReadCount(ADSK2_PIN,ADDT2_PIN);
			delay_us(5);
			
			text_1 = json_pack("{sisisi}","left",(int)Num1, "right", (int)Num2, "height", (int)height);//(int)Num_display1);      //右臂升力
			out1 = json_dumps(text_1,JSON_ENCODE_ANY);
			printf("%s\r\n", out1);
			
			free(out1);
			json_decref(text_1);
			free(text_1);
			
			
			len = USART_RX_STA&0x3fff;					//得到此次接收到的数据长度
			USART_RX_STA = 0;
				for(t=0;t<len;t++)
			 {		
				 USART_RX_BUF[t]=NULL; 
			 }
		 }
			else if( strcmp((char *)USART_RX_BUF,"strengthBreak") == 0 )
		 {
			 break;
			}		 
	
//	for(j = 0; j < 1; j++)
//  {
//    NumSum_1 += ReadCount(ADSK2_PIN,ADDT2_PIN);
//		delay_us(5);
//		NumSum_2 += ReadCount(ADSK1_PIN,ADDT1_PIN);
//		delay_us(5);
//		
//  }
//  NumAvg_1 = NumSum_1 / 1;
//	NumAvg_2 = NumSum_2 / 1;
	
//	while(times<150)   //大致10S
//  {
//		
		

		
//		printf("%lld\r\n",Num);
		
//    times++;
    
//    Num_display1 = (double)(Num1-NumAvg_1) * 0.006 + 9;	        //右臂升力
//		Num_display2 = (double)(Num2-NumAvg_2) * 0.006 + 9;					//左臂升力

		
    

//		printf("%f\r\n", Num_display1);

		
//		text_2 = json_pack("{si}","left_armLift_strength",(int)Num1);//(int)Num_display2);      //左臂升力
//		out2 = json_dumps(text_2,JSON_ENCODE_ANY);
//		printf("%s\r\n", out2);
		

		
//		free(out2);
//	  json_decref(text_2);
//	  free(text_2);
		
//		
//    for(i = 0x8fff; i > 0; i--); 
  }
	}while(1);
	USART2_printf(USART2,"break\r\n");
	delay_ms(8);
}



/********************
作用：	体重测量
函数名：weight_test
参数：	无
返回值：无
********************/
void weight_test(void)
{
	json_t *text_0;
	char *out;
	uint16_t times=0;
	uint32_t i,j;
	long long NumAvg;
	long long NumSum;
	long long Num ;
	double Num_display ;
	
	for(j = 0; j < 5; j++)
  {
    NumSum += ReadCount(ADSK7_PIN,ADDT7_PIN);
		delay_us(5);
  }
  NumAvg = NumSum / 5;

	while(times<1)   //读一次力
  {
		times++;
		
    Num = ReadCount(ADSK7_PIN,ADDT7_PIN);
    
    Num_display = (double)(Num-NumAvg) * 0.006 + 9;	  //体重
    
//		printf("%.2f\r\n", Num_display);
		text_0 = json_pack("{si}","weight",(int)Num_display);
		out = json_dumps(text_0,JSON_ENCODE_ANY);
		printf("%s\r\n", out);
		
		free(out);
	  json_decref(text_0);
	  free(text_0);
		
    for(i = 0x8fff; i > 0; i--);
  }
	
}


void arm_strength(void)                             //臂升力
{
	double hei;	
	uint8_t t,len;
	json_t *text_0;
	char *out;
	
	LiLiang2_test();
	
	if(mark_stop==1)
	{
	  USART2_printf(USART2,"require\r\n");
		mark_stop = 0;
	}
	
	if(mark==1)
	{
		if(USART2_RX_STA!=0)
	  {
			len = USART2_RX_STA&0x3fff;					//得到此次接收到的数据长度
	    hei = atof((char*)USART2_RX_BUF);
			
			text_0 = json_pack("{si}","heartrate",(double)hei);
			out = json_dumps(text_0,JSON_ENCODE_ANY);
			printf("%s\n\r",out);
//			printf("%.2f\r\n",hei);
			
			free(out);
	    json_decref(text_0);
	    free(text_0);
			
		  USART2_RX_STA = 0;
			
			hei = 0;
			for(t=0;t<len;t++)
	    {		
		     USART2_RX_BUF[t]=NULL; 
	    }
	  }
		mark = 0;
	}
	
}
	
void thigh_left_strength(void)                      //左大腿力
{
	double hei;	
	uint8_t t,len;
	json_t *text_0;
	char *out;
	
	LiLiang1_test(ADSK3_PIN,ADDT3_PIN,2);

  if(mark_stop==1)
	{
	  USART2_printf(USART2,"require\r\n");
		mark_stop = 0;
	}

	if(mark==1)
	{
		if(USART2_RX_STA!=0)
	  {
			len = USART2_RX_STA&0x3fff;					//得到此次接收到的数据长度
	    hei = atof((char*)USART2_RX_BUF);
			
			text_0 = json_pack("{si}","heartrate",(double)hei);
			out = json_dumps(text_0,JSON_ENCODE_ANY);
			printf("%s\n\r",out);
//			printf("%.2f\r\n",hei);
			
			free(out);
	    json_decref(text_0);
	    free(text_0);
			
		  USART2_RX_STA = 0;
			
			hei = 0;
			for(t=0;t<len;t++)
	    {		
		     USART2_RX_BUF[t]=NULL; 
	    }
	  }
		mark = 0;
	}
  	
}

void thigh_right_strength(void)                     //右大腿力
{
	double hei;	
	uint8_t t,len;
	json_t *text_0;
	char *out;
	
	LiLiang1_test(ADSK4_PIN,ADDT4_PIN,1);			

  if(mark_stop==1)
	{
	  USART2_printf(USART2,"require\r\n");
		mark_stop = 0;
	}
	
	if(mark==1)
	{
		if(USART2_RX_STA!=0)
	  {
			len = USART2_RX_STA&0x3fff;					//得到此次接收到的数据长度
	    hei = atof((char*)USART2_RX_BUF);
			
			text_0 = json_pack("{si}","heartrate",(double)hei);
			out = json_dumps(text_0,JSON_ENCODE_ANY);
			printf("%s\n\r",out);
//			printf("%.2f\r\n",hei);
			
			free(out);
	    json_decref(text_0);
	    free(text_0);
			
		  USART2_RX_STA = 0;
			
			hei = 0;
			for(t=0;t<len;t++)
	    {		
		     USART2_RX_BUF[t]=NULL; 
	    }
	  }
		mark = 0;
	}
	
}

void back_strenth_test(void)              //背扩力测量
{
	double hei;	
	uint8_t t,len;
	json_t *text_0;
	char *out;
	
	LiLiang3_test();
	
//	if(mark_stop==1)
//	{
//	  USART2_printf(USART2,"require\r\n");
//		mark_stop = 0;
//	}
	
//	if(mark==1)
//	{
//		if(USART2_RX_STA!=0)
//	  {
//			len = USART2_RX_STA&0x3fff;					//得到此次接收到的数据长度
//	    hei = atof((char*)USART2_RX_BUF);
//			
//			text_0 = json_pack("{si}","heartrate",(double)hei);
//			out = json_dumps(text_0,JSON_ENCODE_ANY);
//			printf("%s\n\r",out);
////			printf("%.2f\r\n",hei);
//			
//			free(out);
//	    json_decref(text_0);
//	    free(text_0);
//			
//		  USART2_RX_STA = 0;
//			
//			hei = 0;
//			for(t=0;t<len;t++)
//	    {		
//		     USART2_RX_BUF[t]=NULL; 
//	    }
//	  }
//		mark = 0;
//	}
	
}

void height_weight_test(void)             //体重
{
	double hei;	
	uint8_t t,len;
	json_t *text_0;
	char *out;
	
	weight_test();
	
	if(mark_stop==0)
	{
	  USART2_printf(USART2,"require\r\n");
		mark_stop = 1;
		if(mark_stop == 1)
		{
		  printf("Yes!\n");
		}
	}
		
  if(mark==1)
	{
		printf("mark==1\n");
		USART2_printf(USART2,"Read\r\n");
//		delay_ms(500);               //注意延时会错开
		while(USART2_RX_STA == 0);

		len = USART2_RX_STA&0x3fff;					//得到此次接收到的数据长度
		hei = atof((char*)USART2_RX_BUF);
		
//			text_0 = json_pack("{si}","height",(int)hei);
//			out = json_dumps(text_0,JSON_ENCODE_ANY);
//			printf("%s\n\r",out);
		printf("%.2f\r\n",hei);
		
//			free(out);
//	    json_decref(text_0);
//	    free(text_0);
		hei=0;
		USART2_RX_STA = 0;
		
		for(t=0;t<len;t++)
		{		
			 USART2_RX_BUF[t]=NULL; 
		}

		mark = 0;
		if(mark == 0)
		{
			printf("mark==0\r\n");
		}
	} 
}







