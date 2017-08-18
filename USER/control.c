#include "control.h"
#include "jansson.h"
#include <string.h>
#include "json.h"
#include "rgb.h"
#include "HX711.h"
#include "Motor.h"
#include <stdlib.h> 
#include "usart.h"	

extern u8 mark_stop;

void RGB_test(void)
{	
	u32 RGB;
  u8  op;	
	u8  t,len;
	json_t *text_0;
	char *out;
	u32 SBUF;
	
//  printf("Yes!\n");
	while(1)
	{			
		if(USART_RX_STA&0x8000)
		{
			 len = USART_RX_STA&0x3fff;					//得到此次接收到的数据长度
			 SBUF = atof(USART_RX_BUF );
			
			
			 op = SBUF%10;
			
			 if( (op>=1)||(op<=6) )
			 {
				 switch(op)
				 {
					 case 1: RGB=RGB1; break;
					 case 2: RGB=RGB2; break;
					 case 3: RGB=RGB3; break;
					 case 4: RGB=RGB4; break;
					 case 5: RGB=RGB5; break;
					 case 6: RGB=RGB6; break;
				 }
			 }
			 
			 SBUF = SBUF/10;
			 
			
			 if( SBUF <= 0xffffff )
			 {
				 RGB_Rst(RGB);
				 RGB_Rst(RGB);
				 RGB_Rst(RGB);
				 RGB_Rst(RGB);
				 RGB_Rst(RGB);
				 RGB_Rst(RGB);
				 
				 
				 RGB_Set(SBUF,RGB);
				 RGB_Set(SBUF,RGB);
				 RGB_Set(SBUF,RGB);
				 RGB_Set(SBUF,RGB);
				 RGB_Set(SBUF,RGB);
				 RGB_Set(SBUF,RGB);
	         
//	         RGB_Rst(RGB);
			 
	         text_0 = json_pack("{si}","Yes",1);
	         out = json_dumps(text_0,JSON_ENCODE_ANY);
	         printf("%s\r\n",	out);
				 
	         free(out);
	         json_decref(text_0);
	         free(text_0);
				

			     for(t=0;t<len;t++)
			     {							 
			         USART_RX_BUF[t]=NULL; 
						   
			     }
			     USART_RX_STA = 0;
					 
			     break;
			 }

			 
			 for(t=0;t<len;t++)
			 {		
			    USART_RX_BUF[t]=NULL; 
			 }
			 USART_RX_STA = 0;
		}

	}
}

extern uint8_t motor_flag;
float new_distance, past_distance = 0, dif_distance;
u8 dir;
u8 Round=3;

void Motor_test(void)
{
	u8  t,len;
	u8 *p;
	char *out;
	json_t *text_0;
	USART_RX_STA = 0;
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{
			len = USART_RX_STA&0x3fff;					//得到此次接收到的数据长度
			p = USART_RX_BUF;
			new_distance = atof(p);
			USART_RX_STA = 0;
			for(t=0;t<len;t++)
			{
				p[t]=NULL; 
				USART_RX_BUF[t]=NULL; 
			}
			
			if(new_distance > 9.5 || new_distance < -9.5)
			{
				text_0 = json_pack("{si}","state",0);
				out = json_dumps(text_0,JSON_ENCODE_ANY);
				printf("%s\r\n",	out); 

				for(t=0;t<len;t++)
				{
					USART_RX_BUF[t]=NULL; 
				}

				free(out); 
				json_decref(text_0);
				free(text_0);
				return;
			}
//			printf("New distance = %f\n",new_distance);
			
			
			if(Round!=0)
			{				 
				dif_distance = new_distance - past_distance;
				(new_distance > past_distance) ? (dir = 1) : (dir = 0);
				StepMotor_ctrl(dir, fabs(dif_distance)*200*16);
				Round--;

//				printf("Diference Value:%.1f\n\n",dif_distance);
			}
			else
			{
				if(new_distance>=0)
				{
					motor_flag = 1;
					while(motor_flag==1)    //直到触发按键中断 flag=0
					{
						StepMotor_ctrl(1, 8);  //dir=1为正方向
					}
					StepMotor_ctrl(0, fabs((double)(10-new_distance))*200*16);  //dir=0为负方向,调到所需位置
				}
				else if(new_distance<=0)
				{
					motor_flag = 1;
					while(motor_flag==1)    //直到触发按键中断 flag=0
					{
						 StepMotor_ctrl(0, 8);  //dir=0为负方向
					}
					StepMotor_ctrl(1, fabs((double)(10+new_distance))*200*16);  //dir=1为正方向,调到所需位置
				}
				Round = 3;
			}
			past_distance = new_distance;
			
			text_0 = json_pack("{si}","state",1);
			out = json_dumps(text_0,JSON_ENCODE_ANY);
			printf("%s\r\n",	out); 

			for(t=0;t<len;t++)
			{
				USART_RX_BUF[t]=NULL; 
			}

			free(out); 
			json_decref(text_0);
			free(text_0);
			
			break;
		}
	}
}

void heartrate_spo2_test(uint8_t side) 
{
	int SBUF;
	uint8_t t,len;
	uint8_t loopFlag = 1;
	const char heartrate_left[]="heartrate_left";
	
	MAX30102_Standby(side);	
	
	while(loopFlag)
	{
	 if(USART_RX_STA&0x8000)
	 {
		 len = USART_RX_STA&0x3fff;					//得到此次接收到的数据长度
		 USART_RX_STA = 0;
		 
		 if( strcmp((char *)USART_RX_BUF,heartrate_left) == 0 )
		 {
			 MAX30102_HeartRate_SPO2_Detect(side);
		 }
		 else if( strcmp((char *)USART_RX_BUF,"hrShotdown") == 0)
		 {
			 loopFlag = 0;
//			break;
		 }
		 else
		 {

		 }
		 for(t=0;t<len;t++)
		 {		
			 USART_RX_BUF[t]=NULL; 
		 }
		}
	}
//	MAX30102_HeartRate_SPO2_Detect(side);
		MAX30102_ShutDown(side);
  
}



void test_control( char *p, uint8_t len ) 
{
	uint8_t t;
	
	const char platform[]="platform";                             //云台调节标识符
	const char rgbLED[]="rgbLED";                                 //RGB灯调节标识符
	const char height_weight[]="height_weight";					          //身高和体重测量标识符
	const char heartrate_left[]="heartrate_left";				          //左心率测量标识符
	const char heartrate_right[]="heartrate_right";               //右心率测量标识符
	const char back_strength[]="back_strength";					          //背扩力测量标识符
	const char armLift_strength[]="armLift_strength";             //臂升力测量标识符
	const char right_thigh_strength[]="right_thigh_strength";     //右腿力测量标识符
	const char left_thigh_strength[]="left_thigh_strength";       //左腿力测量标识符
	const char stop_measure_height[]="stop_measure_height";       //停止测高标识符
	
	if(strcmp(p,platform)==0)                           //云台调整
	{
		for(t=0;t<len;t++)
		{		
			p[t]=NULL; 
		}
		Motor_test();
	}
	
	if(strcmp(p,rgbLED)==0)                             //RGB灯调整
	{
		for(t=0;t<len;t++)
		{		
			p[t]=NULL; 
		}
		RGB_test();
	}
	
	if(strcmp(p,heartrate_right)==0)                    //调用右心率测量
	{
//		for(t=0;t<len;t++)
//		{		
//			p[t]=NULL; 
//		}
		heartrate_spo2_test(1);
	}
	
	if(strcmp(p,heartrate_left)==0)                     //调用左心率测量
	{
		USART_RX_STA = 0;
		for(t=0;t<len;t++)
		{		
			p[t]=NULL; 
		}
		heartrate_spo2_test(0);
		
	}
	
	if(strcmp(p,height_weight)==0)                      //身高,体重测量
	{
		height_weight_test();		
	}	
	if(strcmp(p,back_strength)==0)                      //背扩力测量
	{
		back_strenth_test(); 
	}
	if(strcmp(p,armLift_strength)==0)                   //臂升力测量
	{
		arm_strength();
	}
	if(strcmp(p,right_thigh_strength)==0)               //右腿力测量
	{
		thigh_right_strength();
	}
	if(strcmp(p,left_thigh_strength)==0)                //左腿力测量
	{
		thigh_left_strength();
	}
	if(strcmp(p,stop_measure_height)==0)                //停止测高        
	{
		USART2_printf(USART2,"break\r\n");
		mark_stop = 0; 
		printf("mark_stop = 0\r\n");
		
//		USART2_printf(USART2,"Read\r\n");

	}
	
}



