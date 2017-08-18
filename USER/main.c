#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "json.h"
#include "control.h"
#include <string.h>
#include "json.h"
#include "rgb.h"
#include "Motor.h"
#include "timer.h"
#include "exti.h" 
#include "led.h"
#include "HX711.h"

extern u8 USART_RX_BUF[USART_REC_LEN];   


uint8_t DELAY_US;
int time_counting;
uint8_t motor_flag = 1;

#ifndef sample_array_size
#define sample_array_size 16
#endif

/*************************IR & HR****************************/
//IR & HR sample
int32_t IR_sample[sample_array_size];    
int32_t RED_sample[sample_array_size];
int32_t IR_Array_Sort[sample_array_size - 1];
int32_t RED_Array_Sort[sample_array_size - 1];
int8_t IR_Array_Num[sample_array_size - 1] = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
int8_t RED_Array_Num[sample_array_size - 1] = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};

uint8_t Pilot_PA = 0x01;
uint8_t Pilot_THRESH = 0x01; 

int8_t New_sample_Num_RED = 0;   
int8_t last_sample_num_RED = 0;

int8_t New_sample_Num_IR = 0;   
int8_t last_sample_num_IR = 0;

uint8_t data[6];

int64_t RED_sample_DC = 0;
int64_t IR_sample_DC = 0;
int64_t IR_sample_DC_Pre = 0;

int8_t RED_RDY = 1;
int8_t IR_RDY = 0;



/*************************************RED dealing FUN*******************************************/
void RED_quickSort(int32_t *RED_a, int8_t *RED_b, int RED_start, int RED_end);
void RED_Arrange_Array_Num(int8_t RED_array_num[], int8_t RED_last_num);
void RED_found_pos(int8_t RED_start, int8_t RED_last, int8_t RED_first_num, int8_t RED_last_num, int32_t RED_new_sample,int32_t RED_sample_sort[]);
void RED_Insert_sample(int8_t RED_first_num, int8_t RED_last_num, int32_t RED_new_sample, int32_t RED_sample_sort[], int8_t RED_sample_num[]);
double RED_data_output(int32_t RED_sample_sort[]);

/*************************************IR dealing FUN*******************************************/
void IR_quickSort(int32_t *a, int8_t *b, int start, int end);
void IR_Arrange_Array_Num(int8_t array_num[], int8_t last_num);
void IR_found_pos(int8_t start, int8_t last, int8_t first_num, int8_t last_num, int32_t new_sample,int32_t sample_sort[]);
void IR_Insert_sample(int8_t first_num, int8_t last_num, int32_t new_sample, int32_t sample_sort[], int8_t sample_num[]);
double IR_data_output(int32_t sample_sort[]);

void MAX30102_ShutDown(uint8_t side);
/***********************************************************/

int main(void)
{ 
	u8  t,len;
	u8  *p;
	double value;
	long int num;
//	json_t *text_0 ;
//	char *control_flag;
//	char *out;

	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168); 		//延时初始化 
	i2c_Init(2);        //I2C初始化
	uart_init(115200);	//串口1初始化波特率为115200
  uart2_init(115200); //串口2初始化波特率为115200

	EXTI_Key_Init();    //微动按键初始化
	RGB_Init();         //RGB灯初始化
	TIM3_Int_Init(5000-1,8400-1);	   //定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数5000次为500ms     
	EXTI_Check_Init();  //中断检测初始化
//	LED_Init();         //LED灯初始化 调试用
	Strenth_GPIO_Config();    //力量测试初始化
	StepMotor_Init();   //大电机初始化

//  while(1)
//  {
//		
//		if(USART2_RX_STA&0X8000)
//		{
//			len = USART2_RX_STA&0x3fff;					//得到此次接收到的数据长度
//      value = atof( (char *)USART2_RX_BUF);
//			
//			printf("%f\r\n",(double)value);
//			
//			USART2_RX_STA = 0;
//			for(t=0;t<len;t++)
//			{
//				USART2_RX_BUF[t]=NULL;
//			}
//		}
//	}
  
//	printf("system start");
//	while(1)
//	{
//		num = ReadCount(ADSK1_PIN,ADDT1_PIN);
//		delay_us(5);
//		printf("%ld\r\n", num);
//	}
		

	while(1)
	{
		while(USART_RX_STA == 0);
		{
//			printf("receive");
			if(USART_RX_STA&0x8000)
			{

				  p = USART_RX_BUF;
				  len = USART_RX_STA&0x3fff;					//得到此次接收到的数据长度
				
					
				  USART_RX_STA = 0;
				  test_control( (char *)p, len );	  	
				  							
					for(t=0;t<len;t++)
				  {
			      p[t]=NULL; 
				  }
			}
		}		
	}  	

}

