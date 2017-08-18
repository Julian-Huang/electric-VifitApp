#ifndef __HX711_H
#define __HX711_H

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#define 	weight_CLK 		RCC_AHB1Periph_GPIOD			//GPIO����ʱ�Ӷ���

//���������������
#define 	ADDT1_PIN 		GPIO_Pin_1								//Data���Ŷ���
#define 	ADSK1_PIN 		GPIO_Pin_2								//SCK���Ŷ���
//�ұ�������������
#define 	ADDT2_PIN 		GPIO_Pin_3								//Data���Ŷ���  
#define 	ADSK2_PIN 		GPIO_Pin_4								//SCK���Ŷ���
//���������������
#define 	ADDT3_PIN 		GPIO_Pin_5								//Data���Ŷ���
#define 	ADSK3_PIN 		GPIO_Pin_6								//SCK���Ŷ���
//�Ҵ�������������
#define 	ADDT4_PIN 		GPIO_Pin_8								//Data���Ŷ���
#define 	ADSK4_PIN 		GPIO_Pin_9								//SCK���Ŷ���
//���������������
#define 	ADDT5_PIN 		GPIO_Pin_10								//Data���Ŷ���
#define 	ADSK5_PIN 		GPIO_Pin_11								//SCK���Ŷ���
//�������Ҳ�������
#define   ADDT6_PIN     GPIO_Pin_14               //Data���Ŷ���
#define   ADSK6_PIN     GPIO_Pin_15               //SCK���Ŷ���
//���ز���
#define 	ADDT7_PIN 		GPIO_Pin_12								//Data���Ŷ���
#define 	ADSK7_PIN 		GPIO_Pin_13								//SCK���Ŷ���


#define 	weight_PORT 	GPIOD											//GPIO����
#define 	ADDT1 				weight_PORT
#define 	ADSK1 				weight_PORT



void Strenth_GPIO_Config(void);
unsigned long ReadCount(uint16_t GPIO_ADSK,uint16_t GPIO_ADDT);
void LiLiang1_test(uint16_t GPIO_ADSK,uint16_t GPIO_ADDT,uint8_t type);
void LiLiang2_test(void);
void LiLiang3_test(void);	
void weight_test(void);
void arm_strength(void);
void thigh_left_strength(void);
void thigh_right_strength(void);
void back_strength(void);
void height_weight_test(void);
void back_strenth_test(void);

#endif

