#ifndef __I2C_H
#define __I2C_H
#include "sys.h"  
   		   
//������IO��������
#define SDA_IN_LEFT()  {GPIOB->MODER&=~(3<<(0*2));GPIOB->MODER|=0<<0*2;}	//PB0����ģʽ
#define SDA_OUT_LEFT() {GPIOB->MODER&=~(3<<(0*2));GPIOB->MODER|=1<<0*2;} //PB0���ģʽ

//������IO��������
#define SDA_IN_RIGHT()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9����ģʽ
#define SDA_OUT_RIGHT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9���ģʽ

//������IO��������	 
#define IIC_SCL_LEFT    PBout(1) //SCL
#define IIC_SDA_LEFT    PBout(0) //SDA	 
#define READ_SDA_LEFT   PBin(0)  //����SDA 

//������IO��������	 
#define IIC_SCL_RIGHT    PBout(8) //SCL
#define IIC_SDA_RIGHT    PBout(9) //SDA	   
#define READ_SDA_RIGHT   PBin(9)  //����SDA 

#define  _0_6us   0x6//0x6 //0x02
#define  _1_3us   0x10//0x10  //0x7

extern uint8_t DELAY_US;

//IIC���в�������
void i2c_Init(uint8_t side);                //��ʼ��IIC��IO��				 
void i2c_Start(uint8_t side);				//����IIC��ʼ�ź�
void i2c_Stop(uint8_t side);	  			//����IICֹͣ�ź�
void i2c_Send_Byte(uint8_t side, u8 txd);			//IIC����һ���ֽ�
u8 i2c_Read_Byte(uint8_t side, unsigned char ack);//IIC��ȡһ���ֽ�
u8 i2c_Wait_Ack(uint8_t side); 				//IIC�ȴ�ACK�ź�
void i2c_Ack(uint8_t side);					//IIC����ACK�ź�
void i2c_NAck(uint8_t side);				//IIC������ACK�ź�

void i2c_WriteOneByte(uint8_t side, uint8_t slave_addr, uint8_t reg_addr, uint8_t data);
uint8_t i2c_ReadOneByte(uint8_t side, uint8_t slave_addr, uint8_t reg_addr);
void i2c_ReadMultiData(uint8_t side, uint8_t slave_addr, uint8_t reg_addr, uint8_t *Read, uint8_t num);

void delay_0_6(void);        //��׼��ʱ0.6us
void delay_1_3(void);        //��׼��ʱ1.3us
  
#endif




