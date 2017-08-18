#ifndef __EXTI_H
#define __EXTI_H

#include "sys.h" 
#include "delay.h"

#define KEY1 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)	//PC3 
#define KEY2 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2) //PC2

extern uint8_t motor_flag;
extern u8 mark;

void EXTI_Check_Init(void);	//�ⲿ�жϼ���ʼ��	
void EXTI_Key_Init(void);   //�����ж�

#endif

