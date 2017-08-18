#include "I2C.h" 

//初始化IIC
void i2c_Init(uint8_t side)
{	
  GPIO_InitTypeDef  GPIO_InitStructure;	
  if(side == 0)
  {

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

    //GPIOB0,B1初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
    
    IIC_SCL_LEFT=1;
    IIC_SDA_LEFT=1; 
  }
  else if(side == 1) 
  {

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

    //GPIOB9,B10初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
    
    IIC_SCL_RIGHT=1;
    IIC_SDA_RIGHT=1;
  }
  else if(side == 2) 
  {

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

    //GPIOB0,B1初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化


    //GPIOB8,B9初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
    
    IIC_SCL_LEFT=1;
    IIC_SDA_LEFT=1;
    IIC_SCL_RIGHT=1;
    IIC_SDA_RIGHT=1;
  }
	
}

void delay_0_6(void)
{
  DELAY_US = _0_6us;
  while(DELAY_US--);
}


void delay_1_3(void)
{
  DELAY_US = _1_3us;
  while(DELAY_US--);
}


//产生IIC起始信号
void i2c_Start(uint8_t side)
{
	if(side == 0)         //左心率
  {
    SDA_OUT_LEFT();     //sda线输出
    IIC_SDA_LEFT=1;	  	  
    IIC_SCL_LEFT=1;
    delay_0_6();
    IIC_SDA_LEFT=0;//START:when CLK is high,DATA change form high to low 
    delay_0_6();
    IIC_SCL_LEFT=0;//钳住I2C总线，准备发送或接收数据 
    delay_0_6();
  }
  else if (side == 1)    //右心率
  {
    SDA_OUT_RIGHT();     //sda线输出
    IIC_SDA_RIGHT=1;	  	  
    IIC_SCL_RIGHT=1;
    delay_0_6();
    IIC_SDA_RIGHT=0;//START:when CLK is high,DATA change form high to low 
    delay_0_6();
    IIC_SCL_RIGHT=0;//钳住I2C总线，准备发送或接收数据 
    delay_0_6();
  }
}	  

//产生IIC停止信号
void i2c_Stop(uint8_t side)
{
	if(side == 0)
  {
    SDA_OUT_LEFT();//sda线输出
    IIC_SCL_LEFT=0;
    IIC_SDA_LEFT=0;//STOP:when CLK is high DATA change form low to high
    IIC_SCL_LEFT=1; 
    delay_0_6();
    IIC_SDA_LEFT=1;//发送I2C总线结束信号
    delay_0_6();
  }
  else if (side == 1)
  {
    SDA_OUT_RIGHT();//sda线输出
    IIC_SCL_RIGHT=0;
    IIC_SDA_RIGHT=0;//STOP:when CLK is high DATA change form low to high
    IIC_SCL_RIGHT=1; 
    delay_0_6();
    IIC_SDA_RIGHT=1;//发送I2C总线结束信号
    delay_0_6();
  }
							   	
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 i2c_Wait_Ack(uint8_t side)
{
	u8 ucErrTime=0;
  if(side == 0)
  {
    SDA_IN_LEFT();      //SDA设置为输入  
    IIC_SDA_LEFT=1;delay_0_6();	 
    IIC_SCL_LEFT=1;delay_0_6();	 
    while(READ_SDA_LEFT)
    {
      ucErrTime++;
      if(ucErrTime>250)
      {
        i2c_Stop(side);
        return 1;
      }
    }
    IIC_SCL_LEFT=0;//时钟输出0 	   
    return 0;  
  }
  if(side == 1)
  {
    SDA_IN_RIGHT();      //SDA设置为输入  
    IIC_SDA_RIGHT=1;delay_0_6();	 
    IIC_SCL_RIGHT=1;delay_0_6();	 
    while(READ_SDA_RIGHT)
    {
      ucErrTime++;
      if(ucErrTime>250)
      {
        i2c_Stop(side);
        return 1;
      }
    }
    IIC_SCL_RIGHT=0;//时钟输出0 	   
    return 0;  
  }
} 

//产生ACK应答
void i2c_Ack(uint8_t side)
{
	if(side == 0)
  {
    IIC_SCL_LEFT=0;         //CLK拉低
    SDA_OUT_LEFT();
    IIC_SDA_LEFT=0;         //在CLK为低电平期间拉低，产生应答
    delay_0_6();
    IIC_SCL_LEFT=1;         //保证CLK在高电平期间，SDA线保持为原先的低电平
    delay_0_6();
    IIC_SCL_LEFT=0; 
  }
  else if(side == 1)
  {
    IIC_SCL_RIGHT=0;         //CLK拉低
    SDA_OUT_RIGHT();
    IIC_SDA_RIGHT=0;         //在CLK为低电平期间拉低，产生应答
    delay_0_6();
    IIC_SCL_RIGHT=1;         //保证CLK在高电平期间，SDA线保持为原先的低电平
    delay_0_6();
    IIC_SCL_RIGHT=0; 
  }
}

//不产生ACK应答		    
void i2c_NAck(uint8_t side)
{
	if(side == 0)
  {
    IIC_SCL_LEFT=0;
    SDA_OUT_LEFT();
    IIC_SDA_LEFT=1;
    delay_0_6();
    IIC_SCL_LEFT=1;
    delay_0_6();
    IIC_SCL_LEFT=0;
  }
  else if(side == 1)
  {
    IIC_SCL_RIGHT=0;
    SDA_OUT_RIGHT();
    IIC_SDA_RIGHT=1;
    delay_0_6();
    IIC_SCL_RIGHT=1;
    delay_0_6();
    IIC_SCL_RIGHT=0;
  }
}				

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void i2c_Send_Byte(uint8_t side,u8 txd)
{                        
   u8 t;   
  if(side == 0)
  {
    SDA_OUT_LEFT(); 	    
    IIC_SCL_LEFT=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA_LEFT=(txd&0x80)>>7;
        txd<<=1; 	  
    delay_0_6();   //对TEA5767这三个延时都是必须的
    IIC_SCL_LEFT=1;
    delay_0_6();
    IIC_SCL_LEFT=0;
    }
  }
  else if(side == 1)
  {
    SDA_OUT_RIGHT(); 	    
    IIC_SCL_RIGHT=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA_RIGHT=(txd&0x80)>>7;
        txd<<=1; 	  
    delay_0_6();   //对TEA5767这三个延时都是必须的
    IIC_SCL_RIGHT=1;
    delay_0_6();
    IIC_SCL_RIGHT=0;
    }
  } 
} 	 

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 i2c_Read_Byte(uint8_t side,unsigned char ack)
{
	unsigned char i,receive=0;
	if(side == 0)
  {
    SDA_IN_LEFT();//SDA设置为输入
      for(i=0;i<8;i++ )
    {
          IIC_SCL_LEFT=0; 
          delay_1_3();
          IIC_SCL_LEFT=1;
          receive<<=1;
          if(READ_SDA_LEFT)receive++;   
      delay_0_6();  
      }					 
      if (!ack)
          i2c_NAck(side);//发送nACK
      else
          i2c_Ack(side); //发送ACK   
      return receive;
  }
  else if(side == 1)
  {
    SDA_IN_RIGHT();//SDA设置为输入
      for(i=0;i<8;i++ )
    {
          IIC_SCL_RIGHT=0; 
          delay_1_3();
          IIC_SCL_RIGHT=1;
          receive<<=1;
          if(READ_SDA_RIGHT)receive++;   
      delay_0_6();  
      }					 
      if (!ack)
          i2c_NAck(side);//发送nACK
      else
          i2c_Ack(side); //发送ACK   
      return receive;
  }
}

void i2c_WriteOneByte(uint8_t side,uint8_t slave_addr, uint8_t reg_addr, uint8_t data)			
{
  i2c_Start(side);
  i2c_Send_Byte(side,slave_addr);
  i2c_Wait_Ack(side);
  i2c_Send_Byte(side,reg_addr);
  i2c_Wait_Ack(side);
  
  i2c_Send_Byte(side,data);
  i2c_Wait_Ack(side);
  i2c_Stop(side);
}

uint8_t i2c_ReadOneByte(uint8_t side,uint8_t slave_addr, uint8_t reg_addr)
{
  uint8_t data;
  
  i2c_Start(side);
  i2c_Send_Byte(side,slave_addr);
  i2c_Wait_Ack(side);
  i2c_Send_Byte(side,reg_addr);
  i2c_Wait_Ack(side);
  
  i2c_Start(side);
  i2c_Send_Byte(side,slave_addr + 1);
  i2c_Wait_Ack(side);
  
  data = i2c_Read_Byte(side,0);
  i2c_Stop(side);
  
  return data;
}

//for MAX30102
void i2c_ReadMultiData(uint8_t side, uint8_t slave_addr, uint8_t reg_addr, uint8_t *Read, uint8_t num)
{
  uint8_t i;
  
  i2c_Start(side);
  i2c_Send_Byte(side,slave_addr);
  i2c_Wait_Ack(side);
  i2c_Send_Byte(side,reg_addr);
  i2c_Wait_Ack(side);
  
  i2c_Start(side);
  i2c_Send_Byte(side,slave_addr + 1);
  i2c_Wait_Ack(side);
  
  for(i = 0; i < (num-1); i++)
  {
    *Read = i2c_Read_Byte(side,1);
    Read++;
  }
  *Read = i2c_Read_Byte(side,0);
  i2c_Stop(side);
}
