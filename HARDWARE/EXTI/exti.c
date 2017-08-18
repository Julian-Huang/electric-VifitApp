#include "exti.h"
#include "led.h"
#include "usart.h"
#include <stdlib.h> 

//外部中断初始化程序
//初始化PC9为中断输入.
void EXTI_Check_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	
   GPIO_Init(GPIOC, &GPIO_InitStructure);	 
}


void EXTI_Key_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	//中断引脚PC1,PC2,PC3配置
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);//PC1 连接到中断线1
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource2);//PC2 连接到中断线2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource3);//PC3 连接到中断线3
	
	/* 配置EXTI_Line1,2,3 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1 | EXTI_Line2 | EXTI_Line3 ;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    								//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 								//上升沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;															//中断线使能
	EXTI_Init(&EXTI_InitStructure);																				//配置
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//外部中断1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级1
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//外部中断2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级1
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//外部中断3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
}

void EXTI1_IRQHandler(void)
{
	delay_ms(10);	//消抖
	if(EXTI_GetITStatus(EXTI_Line1) != RESET) 
	{
		motor_flag = 0;   
	}
	EXTI_ClearITPendingBit(EXTI_Line1);  //清除LINE1上的中断标志位 
}

void EXTI2_IRQHandler(void)
{
	delay_ms(10);	//消抖
	if(EXTI_GetITStatus(EXTI_Line2) != RESET) 
	{
		
		motor_flag = 0;
	}
	EXTI_ClearITPendingBit(EXTI_Line2);  //清除LINE2上的中断标志位 
}


void EXTI3_IRQHandler(void)
{
	   
	 if(EXTI_GetITStatus(EXTI_Line3) != RESET)
		 mark = 1; 
//		 USART2_printf(USART2,"require\r\n");
//		 printf("Iterrupt!\r\n");
//		 GPIO_ResetBits(GPIOC,GPIO_Pin_13 );
		 
	assert_param(IS_EXTI_LINE(EXTI_Line));
  EXTI->PR = EXTI_Line3;
}















