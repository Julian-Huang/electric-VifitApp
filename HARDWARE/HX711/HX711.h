#ifndef __HX711_H
#define __HX711_H

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#define 	weight_CLK 		RCC_AHB1Periph_GPIOD			//GPIO引脚时钟定义

//左臂升力测量引脚
#define 	ADDT1_PIN 		GPIO_Pin_1								//Data引脚定义
#define 	ADSK1_PIN 		GPIO_Pin_2								//SCK引脚定义
//右臂升力测量引脚
#define 	ADDT2_PIN 		GPIO_Pin_3								//Data引脚定义  
#define 	ADSK2_PIN 		GPIO_Pin_4								//SCK引脚定义
//左大腿力测量引脚
#define 	ADDT3_PIN 		GPIO_Pin_5								//Data引脚定义
#define 	ADSK3_PIN 		GPIO_Pin_6								//SCK引脚定义
//右大腿力测量引脚
#define 	ADDT4_PIN 		GPIO_Pin_8								//Data引脚定义
#define 	ADSK4_PIN 		GPIO_Pin_9								//SCK引脚定义
//背扩力左测量引脚
#define 	ADDT5_PIN 		GPIO_Pin_10								//Data引脚定义
#define 	ADSK5_PIN 		GPIO_Pin_11								//SCK引脚定义
//背扩力右测量引脚
#define   ADDT6_PIN     GPIO_Pin_14               //Data引脚定义
#define   ADSK6_PIN     GPIO_Pin_15               //SCK引脚定义
//体重测量
#define 	ADDT7_PIN 		GPIO_Pin_12								//Data引脚定义
#define 	ADSK7_PIN 		GPIO_Pin_13								//SCK引脚定义


#define 	weight_PORT 	GPIOD											//GPIO引脚
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

