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
			 len = USART_RX_STA&0x3fff;					//�õ��˴ν��յ������ݳ���
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
			len = USART_RX_STA&0x3fff;					//�õ��˴ν��յ������ݳ���
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
					while(motor_flag==1)    //ֱ�����������ж� flag=0
					{
						StepMotor_ctrl(1, 8);  //dir=1Ϊ������
					}
					StepMotor_ctrl(0, fabs((double)(10-new_distance))*200*16);  //dir=0Ϊ������,��������λ��
				}
				else if(new_distance<=0)
				{
					motor_flag = 1;
					while(motor_flag==1)    //ֱ�����������ж� flag=0
					{
						 StepMotor_ctrl(0, 8);  //dir=0Ϊ������
					}
					StepMotor_ctrl(1, fabs((double)(10+new_distance))*200*16);  //dir=1Ϊ������,��������λ��
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
		 len = USART_RX_STA&0x3fff;					//�õ��˴ν��յ������ݳ���
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
	
	const char platform[]="platform";                             //��̨���ڱ�ʶ��
	const char rgbLED[]="rgbLED";                                 //RGB�Ƶ��ڱ�ʶ��
	const char height_weight[]="height_weight";					          //��ߺ����ز�����ʶ��
	const char heartrate_left[]="heartrate_left";				          //�����ʲ�����ʶ��
	const char heartrate_right[]="heartrate_right";               //�����ʲ�����ʶ��
	const char back_strength[]="back_strength";					          //������������ʶ��
	const char armLift_strength[]="armLift_strength";             //������������ʶ��
	const char right_thigh_strength[]="right_thigh_strength";     //������������ʶ��
	const char left_thigh_strength[]="left_thigh_strength";       //������������ʶ��
	const char stop_measure_height[]="stop_measure_height";       //ֹͣ��߱�ʶ��
	
	if(strcmp(p,platform)==0)                           //��̨����
	{
		for(t=0;t<len;t++)
		{		
			p[t]=NULL; 
		}
		Motor_test();
	}
	
	if(strcmp(p,rgbLED)==0)                             //RGB�Ƶ���
	{
		for(t=0;t<len;t++)
		{		
			p[t]=NULL; 
		}
		RGB_test();
	}
	
	if(strcmp(p,heartrate_right)==0)                    //���������ʲ���
	{
//		for(t=0;t<len;t++)
//		{		
//			p[t]=NULL; 
//		}
		heartrate_spo2_test(1);
	}
	
	if(strcmp(p,heartrate_left)==0)                     //���������ʲ���
	{
		USART_RX_STA = 0;
		for(t=0;t<len;t++)
		{		
			p[t]=NULL; 
		}
		heartrate_spo2_test(0);
		
	}
	
	if(strcmp(p,height_weight)==0)                      //���,���ز���
	{
		height_weight_test();		
	}	
	if(strcmp(p,back_strength)==0)                      //����������
	{
		back_strenth_test(); 
	}
	if(strcmp(p,armLift_strength)==0)                   //����������
	{
		arm_strength();
	}
	if(strcmp(p,right_thigh_strength)==0)               //����������
	{
		thigh_right_strength();
	}
	if(strcmp(p,left_thigh_strength)==0)                //����������
	{
		thigh_left_strength();
	}
	if(strcmp(p,stop_measure_height)==0)                //ֹͣ���        
	{
		USART2_printf(USART2,"break\r\n");
		mark_stop = 0; 
		printf("mark_stop = 0\r\n");
		
//		USART2_printf(USART2,"Read\r\n");

	}
	
}



