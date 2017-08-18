#include "MAX30102.h"



void MAX30102_WriteOneByte(uint8_t side, uint8_t reg_addr, uint8_t data)
{
  i2c_WriteOneByte(side, MAX30102_SLAVE_ADDRESS, reg_addr, data); 
}

uint8_t MAX30102_ReadOneByte(uint8_t side, uint8_t reg_addr)
{
  return i2c_ReadOneByte(side, MAX30102_SLAVE_ADDRESS, reg_addr);
}

void MAX30102_ReadMultiData(uint8_t side, uint8_t reg_addr, uint8_t * Read, uint8_t num)
{
  i2c_ReadMultiData(side, MAX30102_SLAVE_ADDRESS, reg_addr, Read, num);
}

void MAX30102_Reg_Setting(uint8_t side, MAX30102_InitTypeDef MAX30102_InitStructure)
{
  MAX30102_WriteOneByte(side, MX312_INT_EN_1_REG, MAX30102_InitStructure.INT_EN_1);
  MAX30102_WriteOneByte(side, MX312_INT_EN_2_REG, MAX30102_InitStructure.INT_EN_2);

  MAX30102_WriteOneByte(side, MX312_FIFO_CFG_REG, MAX30102_InitStructure.FIFO_CFG);
  MAX30102_WriteOneByte(side, MX312_MODE_CFG_REG, MAX30102_InitStructure.MODE_CFG);
  MAX30102_WriteOneByte(side, MX312_SPO2_CFG_REG, MAX30102_InitStructure.SPO2_CFG);
  
  MAX30102_WriteOneByte(side, MX312_LED1_PA_REG, MAX30102_InitStructure.LED1_PA);
  MAX30102_WriteOneByte(side, MX312_LED2_PA_REG, MAX30102_InitStructure.LED2_PA);
  
  MAX30102_WriteOneByte(side, MX312_PILOT_PA_REG, MAX30102_InitStructure.PLOIT_PA);
  
  MAX30102_WriteOneByte(side, MX312_DIE_TEMP_CFG_REG, MAX30102_InitStructure.DIE_TEMP_CFG);
  
  MAX30102_WriteOneByte(side, MX312_PROX_INT_THRESH_REG, MAX30102_InitStructure.PROX_INT_THRESH);
}

void MAX30102_Standby(uint8_t side)
{
  int8_t count;
	/********************************Initiail Process**********************************/
  MAX30102_InitTypeDef MX312_InitStruct;
  MAX30102_WriteOneByte(side, MX312_MODE_CFG_REG, MX312_RESET_EN);
  
  MX312_InitStruct.INT_EN_1 = MX312_PROX_INT_EN | MX312_PPG_RDY_EN;  //开启阈值，中断
  MX312_InitStruct.INT_EN_2 = MX312_DIE_TEMP_RDY_EN;      //温度传感器中断
  MX312_InitStruct.FIFO_CFG = SMP_AVE_1 | FIFO_ROLLOVER_EN | FIFO_A_FULL_2;  //SMP_AVE_1：开启采用值取平均值，FIFO_ROLLOVER_EN：不断循环，FIFO_A_FULL_2 ：FIFO满时提示        
  MX312_InitStruct.MODE_CFG = MX312_SPO2_MODE;   //开启2个通道（红光和红外）
  MX312_InitStruct.SPO2_CFG = SPO2_ADC_RGE_2048 | SPO2_SR_800 | LED_PW_215us; //ADC转换阈值，800/s，脉冲宽度
  MX312_InitStruct.DIE_TEMP_CFG = TEMP_EN; 
  
  MX312_InitStruct.LED1_PA = 0x01;
  MX312_InitStruct.LED2_PA = 0x01; 
   
  MX312_InitStruct.PLOIT_PA = 0x02; 
  MX312_InitStruct.PROX_INT_THRESH = 0x01;  //取阈值，发光强度
  
  MAX30102_Reg_Setting(side, MX312_InitStruct);
  
	/* 使光逐渐增强  */
//  while(Pilot_PA < 0x1f)
//  {
//    if((MAX30102_ReadOneByte(side, MX312_INT_STA_1_REG) & 0x10) == 0x10)
//    {

//      Pilot_PA += 0x02; 
//      Pilot_THRESH += 0x0b;
//      MAX30102_WriteOneByte(side, MX312_PILOT_PA_REG, Pilot_PA);
//      MAX30102_WriteOneByte(side, MX312_PROX_INT_THRESH_REG, Pilot_THRESH);  
//    }
//  }

  Pilot_PA = 0x01;
  Pilot_THRESH = 0x01;
  
  MX312_InitStruct.SPO2_CFG = SPO2_ADC_RGE_16384 | SPO2_SR_200 | LED_PW_411us;
  MX312_InitStruct.LED1_PA = 0xF0;  //EF
  MX312_InitStruct.LED2_PA = 0xF0;  //EF
  
  MX312_InitStruct.PLOIT_PA = 0x06;
  MX312_InitStruct.PROX_INT_THRESH = 0x02;  
  MAX30102_Reg_Setting(side, MX312_InitStruct);
	/********************************Initiail Process**********************************/
	
	
	/********************************Checking Process**********************************/
  do
  {
    while((MAX30102_ReadOneByte(side, MX312_INT_STA_1_REG) & 0x40) != 0x40);
		
		
    MAX30102_ReadMultiData(side, MX312_FIFO_DATA_REG, data, 6); 
    RED_sample[New_sample_Num_RED] = data[0] << 15 | data[1] << 7 | data[2] >> 1;
    IR_sample[New_sample_Num_IR] = data[3] << 15 | data[4] << 7 | data[5] >> 1;
		if(New_sample_Num_IR == (sample_array_size - 1))
		{
			New_sample_Num_IR = 0;
			New_sample_Num_RED = 0;
		}
		else 
		{
			New_sample_Num_IR++;
			New_sample_Num_RED++;
		}
//    (New_sample_Num_IR == (sample_array_size - 1)) ? ((New_sample_Num_IR = 0), (New_sample_Num_RED = 0)) : ((New_sample_Num_IR++), (New_sample_Num_RED++));
  }while((IR_sample[New_sample_Num_IR] < 1300000) || (New_sample_Num_IR != (sample_array_size - 1)));
  				
  for(count = 0; count < 15;count++) 
  {
    IR_Array_Sort[count] = IR_sample[count];
		RED_Array_Sort[count] = RED_sample[count];
  }
  IR_quickSort(IR_Array_Sort, IR_Array_Num, 0, 14);
	/********************************Checking Process**********************************/
}
 
void MAX30102_ShutDown(uint8_t side)
{
	int8_t i, j;
	MAX30102_WriteOneByte(side, MX312_MODE_CFG_REG, MX312_SHDN_EN);	
  New_sample_Num_RED = 0;
	New_sample_Num_IR = 0;
	last_sample_num_RED = 0;
	last_sample_num_IR = 0;
  for(i = 0, j = 15; i < 15; i++, j--)
	{
    IR_Array_Num[i] = j;
		RED_Array_Num[i] = j;
    IR_sample[i] = 0;
		RED_sample[i] = 0;
		IR_Array_Sort[i] = 0;
		RED_Array_Sort[i] = 0;
	}
	IR_sample[15] = 0;
	RED_sample[15] = 0;
	RED_sample_DC = 0;
	IR_sample_DC = 0;
	IR_sample_DC_Pre = 0;
	RED_RDY = 0;
	IR_RDY = 1;
} 

SPO2_Sample MAX30102_data_process(uint8_t side) 
{
	SPO2_Sample sample; 
 	while((i2c_ReadOneByte(side, MAX30102_SLAVE_ADDRESS, MX312_INT_STA_1_REG) & 0x40) != 0x40);  //check if sample is ready

	MAX30102_ReadMultiData(side, MX312_FIFO_DATA_REG, data, 6); 
  sample.RED_Sample = data[0] << 15 | data[1] << 7 | data[2] >> 1;
	sample.IR_Sample = data[3] << 15 | data[4] << 7 | data[5] >> 1; 
	//printf("%d\n\r", sample.IR_Sample);
	
	RED_sample_DC += ((sample.RED_Sample - RED_sample_DC) >> 7); 
	IR_sample_DC += ((sample.IR_Sample - IR_sample_DC) >> 5);
	
//	printf("%d   %d\n",New_sample_Num_IR,last_sample_num_IR);
	
//	IR_Arrange_Array_Num(IR_Array_Num, last_sample_num_IR);
//	IR_found_pos(0, 14, New_sample_Num_IR, last_sample_num_IR, sample.IR_Sample, IR_Array_Sort);
//	IR_Insert_sample(New_sample_Num_IR, last_sample_num_IR, sample.IR_Sample, IR_Array_Sort, IR_Array_Num);

//	sample.IR_Sample_PreProcess = (int32_t)IR_data_output(IR_Array_Sort);
	sample.IR_Sample_PreProcess = sample.IR_Sample;
	IR_sample_DC_Pre += ((sample.IR_Sample_PreProcess - IR_sample_DC_Pre) >> 5);
	sample.IR_Sample_PreProcess -= IR_sample_DC_Pre;
//	sample.IR_Sample_PreProcess = sample.IR_Sample;
	return sample;
} 
  
void MAX30102_HeartRate_SPO2_Detect(uint8_t side)
{
	SPO2_Sample Sample_Now;
	int32_t round = 1;
	int32_t IR_HR_NOW = 0;
	json_t *text_0;
	char *out;
	int8_t avg_repeat = 0;
	

//	printf("Yes!\n");
	
//	do{
//	Sample_Now = MAX30102_data_process(side);
//	IR_HR_NOW = Sample_Now.IR_Sample_PreProcess;
////		printf("%d\n", IR_HR_NOW);
//	} while (IR_HR_NOW > 500);
	
//	while(round--)
//	{
//		
		while(1)
		{
		Sample_Now = MAX30102_data_process(side); 
		IR_HR_NOW += Sample_Now.IR_Sample_PreProcess;
		avg_repeat++;
		
//		printf("%d\n",IR_HR_NOW);
		if(avg_repeat == 2)
		{ 
			text_0 = json_pack("{si}","heartrate",(int)IR_HR_NOW);
			out = json_dumps(text_0,JSON_ENCODE_ANY);
			printf("%s\n\r",out);
			
			free(out);
			json_decref(text_0);
			free(text_0);
			avg_repeat = 0;
			IR_HR_NOW = 0;
			break;
		}
	}
//	}
}
/*************************************IR dealing FUN*******************************************/
 


void IR_quickSort(int32_t *a, int8_t *b, int start, int end)
{
  if(start < end)
  {
    int i = start - 1, j = end + 1, x1 = a[(start+end) / 2], x2 = b[start], tmp;
    while(1)
    {
      while(a[++i] < x1);
      while(a[--j] > x1);
      if( i >= j)
        break;
      tmp = a[i];
      a[i] = a[j];
      a[j] = tmp;
      tmp = b[i];
      b[i] = b[j];
      b[j] = tmp;
    }
    IR_quickSort(a, b, start, i - 1);
    IR_quickSort(a, b, j + 1, end);
  }
}

void IR_Arrange_Array_Num(int8_t array_num[], int8_t last_num)
{
  int8_t i = 0; 
  for(; i < 15; i++)
  {
    if(array_num[i] == 15) 
      last_num = i;
    else
      array_num[i]++;
  } 
}

void IR_found_pos(int8_t start, int8_t last, int8_t first_num, int8_t last_num, int32_t new_sample,int32_t sample_sort[])
{
  int8_t i, j;
  if(last - start > 1)
  {
    if(new_sample < sample_sort[(last + start) / 2])
      i = start, j = (last + start) / 2;
    else
      i = (last + start) / 2, j = last;
    IR_found_pos(i, j, first_num, last_num, new_sample, sample_sort);
  } 
  else
  {
    if(last_num >= last)
      first_num = last;
    else
      first_num = start; 
  }
}

void IR_Insert_sample(int8_t first_num, int8_t last_num, int32_t new_sample, int32_t sample_sort[], int8_t sample_num[])
{
  if(first_num == last_num)
  {
    sample_sort[first_num] = new_sample;
    sample_num[first_num] = 1;
  }
  else
  {
    if(first_num < last_num)
      for(; last_num > first_num; last_num--)
        sample_sort[last_num] = sample_sort[last_num - 1];
    else
      for(; last_num < first_num; last_num++)
        sample_sort[last_num] = sample_sort[last_num + 1];
    sample_sort[first_num] = new_sample;
    sample_num[first_num] = 1;
  }
}

double IR_data_output(int32_t sample_sort[])
{
  uint8_t i;
  double sum;
  for(i = 4, sum = 0; i < 11; i++)
    sum += sample_sort[i];
  return (sum / 7);
}

/*************************************IR dealing FUN*******************************************/

