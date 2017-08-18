#ifndef __MAX30102_H_
#define __MAX30102_H_

#include "stm32f4xx.h"
#include "I2C.h"
#include "usart.h"
#include "delay.h"
#include "math.h"
#include "jansson.h"
#define 	MAX30102_SLAVE_ADDRESS  	0xAE
#define   MAX30102_REV_ID           0xFE
#define   MAX30102_PART_ID          0xFF

typedef enum
{
  //Status Register
  MX312_INT_STA_1_REG   		 =     	0x00,
  MX312_INT_STA_2_REG   		 =   	  0x01,
  MX312_INT_EN_1_REG    		 =   	  0x02,
  MX312_INT_EN_2_REG    		 =   	  0x03,

  //FIFO Register
  MX312_FIFO_WR_PTR_REG      = 	    0x04,
  MX312_OVF_COUNTER_REG      =      0x05,
  MX312_FIFO_RD_PTR_REG      =      0x06,
  MX312_FIFO_DATA_REG   	   =   	  0x07,

  //Configuartion Register
  MX312_FIFO_CFG_REG      	 =	    0x08,
  MX312_MODE_CFG_REG         =      0x09,
  MX312_SPO2_CFG_REG     		 =   	  0x0A,
  MX312_LED1_PA_REG      		 =	    0x0C,
  MX312_LED2_PA_REG          =      0x0D,
  
  MX312_PILOT_PA_REG         =      0x10,
  MX312_MULLED_MODE_CTRL_21_REG  =  0x11,
  MX312_MULLED_MODE_CTRL_43_REG  =  0x12,
  
  //Die Temperature Register
  MX312_TEMP_INT_REG       	 =	    0x1F,
  MX312_TEMP_FRACT_REG     	 =	    0x20,
  MX312_DIE_TEMP_CFG_REG     =      0x21,
  
  //Proximity Function Regsiter
  MX312_PROX_INT_THRESH_REG  =  0x30,
}MAX30102_REG;

typedef enum
{
  MX312_A_FULL_EN       =   (uint8_t)(0x01 << 7),
  MX312_PPG_RDY_EN      =   (uint8_t)(0x01 << 6),
  MX312_ALC_OVF_EN      =   (uint8_t)(0x01 << 5),
  MX312_PROX_INT_EN     =   (uint8_t)(0x01 << 4),
  MX312_DIE_TEMP_RDY_EN =   (uint8_t)(0x01 << 1),
}MAX30102_INT_SETTING;

typedef enum
{
  SMP_AVE_1      =      (uint8_t)(0x00 << 5),
  SMP_AVE_2      =      (uint8_t)(0x01 << 5),
  SMP_AVE_4      =      (uint8_t)(0x02 << 5),
  SMP_AVE_8      =      (uint8_t)(0x03 << 5),
  SMP_AVE_16     =      (uint8_t)(0x04 << 5),
  SMP_AVE_32     =      (uint8_t)(0x05 << 5),
  
  FIFO_ROLLOVER_EN    =    (uint8_t)(0x01 << 4),
  
  FIFO_A_FULL_0     =     0x0,
  FIFO_A_FULL_1,
  FIFO_A_FULL_2,
  FIFO_A_FULL_3,
  FIFO_A_FULL_4,
  FIFO_A_FULL_5,
  FIFO_A_FULL_6,
  FIFO_A_FULL_7,
  FIFO_A_FULL_8,
  FIFO_A_FULL_9,
  FIFO_A_FULL_10,
  FIFO_A_FULL_11,
  FIFO_A_FULL_12,
  FIFO_A_FULL_13,
  FIFO_A_FULL_14,
  FIFO_A_FULL_15,
}MAX30102_FIFO_CFG_SETTING; 

typedef enum
{
  MX312_SHDN_EN          =    (uint8_t)(0x01 << 7),
  MX312_SHDN_DIS         =    (uint8_t)(0x00 << 7),
  MX312_RESET_EN         =    (uint8_t)(0x01 << 6),
  MX312_HR_MODE          =    (uint8_t)0x2,
  MX312_SPO2_MODE        =    (uint8_t)0x3,
  MX312_MULLED_MODE      =    (uint8_t)0x7,
  
}MAX30102_MODE_CFG_SETTING;

typedef enum
{
  SPO2_ADC_RGE_2048     =    0x0 << 5,
  SPO2_ADC_RGE_4096     =    0x1 << 5,
  SPO2_ADC_RGE_8192     =    0x2 << 5,
  SPO2_ADC_RGE_16384    =    0x3 << 5,
  
  SPO2_SR_50        =      0x0 << 2,
  SPO2_SR_100       =      0x1 << 2,
  SPO2_SR_200       =      0x2 << 2,
  SPO2_SR_400       =      0x3 << 2,
  SPO2_SR_800       =      0x4 << 2,
  SPO2_SR_1000      =      0x5 << 2,
  SPO2_SR_1600      =      0x6 << 2,
  SPO2_SR_3200      =      0x7 << 2,
  
  LED_PW_69us       =      0x0,
  LED_PW_118us      =      0x1,
  LED_PW_215us      =      0x2,
  LED_PW_411us      =      0x3,
  
  ADC_RESOL_15      =      0x0,
  ADC_RESOL_16      =      0x1,
  ADC_RESOL_17      =      0x2,
  ADC_RESOL_18      =      0x3,
  
}MAX30102_SPO2_CFG_SETTING;

typedef enum
{
  PA_00,PA_01,PA_02,PA_03,
  PA_04,PA_05,PA_06,PA_07,
  PA_08,PA_09,PA_10,PA_11,
  PA_12,PA_13,PA_14,PA_15,
  
  
}MAX30102_LED_PA, MAX30102_PILOT_PA;

typedef enum
{
  TEMP_EN     =    0x1,
}MAX30102_TEMP_SETTING;

typedef struct
{
  uint8_t INT_EN_1;
  uint8_t INT_EN_2;
  
  uint8_t FIFO_CFG;
  uint8_t MODE_CFG;
  uint8_t SPO2_CFG;
  
  uint8_t LED1_PA;
  uint8_t LED2_PA;
  
  uint8_t PLOIT_PA;
  uint8_t SLOT_21;
  uint8_t SLOT_43;
  
  uint8_t DIE_TEMP_CFG;
  
  uint8_t PROX_INT_THRESH;
}MAX30102_InitTypeDef;

extern int time_counting;

typedef struct
{
	int32_t IR_Sample_PreProcess;
	int32_t RED_Sample;
	int32_t IR_Sample;
}SPO2_Sample;

#ifndef sample_array_size
#define sample_array_size 16
#endif

void MAX30102_WriteOneByte(uint8_t side, uint8_t reg_addr, uint8_t data);
uint8_t MAX30102_ReadOneByte(uint8_t side, uint8_t reg_addr);
void MAX30102_ReadMultiData(uint8_t side, uint8_t reg_addr, uint8_t * Read, uint8_t num);
void MAX30102_Reg_Setting(uint8_t side, MAX30102_InitTypeDef MAX30102_InitStructure);
void MAX30102_Standby(uint8_t side);
SPO2_Sample MAX30102_data_process(uint8_t side); 
void MAX30102_HeartRate_SPO2_Detect(uint8_t side);
void ir_red_JSON(int32_t ir, int32_t red);

/*************************************RED dealing FUN*******************************************/
extern void RED_quickSort(int32_t *RED_a, int8_t *RED_b, int RED_start, int RED_end);
extern void RED_Arrange_Array_Num(int8_t RED_array_num[], int8_t RED_last_num);
extern void RED_found_pos(int8_t RED_start, int8_t RED_last, int8_t RED_first_num, int8_t RED_last_num, int32_t RED_new_sample,int32_t RED_sample_sort[]);
extern void RED_Insert_sample(int8_t RED_first_num, int8_t RED_last_num, int32_t RED_new_sample, int32_t RED_sample_sort[], int8_t RED_sample_num[]);
extern double RED_data_output(int32_t RED_sample_sort[]);

/*************************************IR dealing FUN*******************************************/
extern void IR_quickSort(int32_t *a, int8_t *b, int start, int end);
extern void IR_Arrange_Array_Num(int8_t array_num[], int8_t last_num);
extern void IR_found_pos(int8_t start, int8_t last, int8_t first_num, int8_t last_num, int32_t new_sample,int32_t sample_sort[]);
extern void IR_Insert_sample(int8_t first_num, int8_t last_num, int32_t new_sample, int32_t sample_sort[], int8_t sample_num[]);
extern double IR_data_output(int32_t sample_sort[]);

extern void MAX30102_ShutDown(uint8_t side) ;

/*************************IR & HR****************************/
//IR & HR sample
extern int32_t IR_sample[sample_array_size];    
extern int32_t RED_sample[sample_array_size];
extern int32_t IR_Array_Sort[sample_array_size - 1];
extern int32_t RED_Array_Sort[sample_array_size - 1];
extern int8_t IR_Array_Num[sample_array_size - 1];
extern int8_t RED_Array_Num[sample_array_size - 1];

extern uint8_t Pilot_PA;
extern uint8_t Pilot_THRESH;  

extern int8_t New_sample_Num_RED;   
extern int8_t last_sample_num_RED;

extern int8_t New_sample_Num_IR;   
extern int8_t last_sample_num_IR;

extern uint8_t data[6];

extern int64_t RED_sample_DC;
extern int64_t IR_sample_DC;
extern int64_t IR_sample_DC_Pre;

extern int8_t RED_RDY;
extern int8_t IR_RDY;
/***********************************************************/


#endif
