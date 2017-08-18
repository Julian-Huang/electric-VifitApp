#ifndef __I2C_H
#define __I2C_H
#include "sys.h"  
   		   
//左心率IO方向设置
#define SDA_IN_LEFT()  {GPIOB->MODER&=~(3<<(0*2));GPIOB->MODER|=0<<0*2;}	//PB0输入模式
#define SDA_OUT_LEFT() {GPIOB->MODER&=~(3<<(0*2));GPIOB->MODER|=1<<0*2;} //PB0输出模式

//右心率IO方向设置
#define SDA_IN_RIGHT()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9输入模式
#define SDA_OUT_RIGHT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9输出模式

//左心率IO操作函数	 
#define IIC_SCL_LEFT    PBout(1) //SCL
#define IIC_SDA_LEFT    PBout(0) //SDA	 
#define READ_SDA_LEFT   PBin(0)  //输入SDA 

//右心率IO操作函数	 
#define IIC_SCL_RIGHT    PBout(8) //SCL
#define IIC_SDA_RIGHT    PBout(9) //SDA	   
#define READ_SDA_RIGHT   PBin(9)  //输入SDA 

#define  _0_6us   0x6//0x6 //0x02
#define  _1_3us   0x10//0x10  //0x7

extern uint8_t DELAY_US;

//IIC所有操作函数
void i2c_Init(uint8_t side);                //初始化IIC的IO口				 
void i2c_Start(uint8_t side);				//发送IIC开始信号
void i2c_Stop(uint8_t side);	  			//发送IIC停止信号
void i2c_Send_Byte(uint8_t side, u8 txd);			//IIC发送一个字节
u8 i2c_Read_Byte(uint8_t side, unsigned char ack);//IIC读取一个字节
u8 i2c_Wait_Ack(uint8_t side); 				//IIC等待ACK信号
void i2c_Ack(uint8_t side);					//IIC发送ACK信号
void i2c_NAck(uint8_t side);				//IIC不发送ACK信号

void i2c_WriteOneByte(uint8_t side, uint8_t slave_addr, uint8_t reg_addr, uint8_t data);
uint8_t i2c_ReadOneByte(uint8_t side, uint8_t slave_addr, uint8_t reg_addr);
void i2c_ReadMultiData(uint8_t side, uint8_t slave_addr, uint8_t reg_addr, uint8_t *Read, uint8_t num);

void delay_0_6(void);        //精准延时0.6us
void delay_1_3(void);        //精准延时1.3us
  
#endif




