#include "I2C.h" 

//��ʼ��IIC
void i2c_Init(uint8_t side)
{	
  GPIO_InitTypeDef  GPIO_InitStructure;	
  if(side == 0)
  {

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��

    //GPIOB0,B1��ʼ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
    
    IIC_SCL_LEFT=1;
    IIC_SDA_LEFT=1; 
  }
  else if(side == 1) 
  {

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��

    //GPIOB9,B10��ʼ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
    
    IIC_SCL_RIGHT=1;
    IIC_SDA_RIGHT=1;
  }
  else if(side == 2) 
  {

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��

    //GPIOB0,B1��ʼ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��


    //GPIOB8,B9��ʼ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
    
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


//����IIC��ʼ�ź�
void i2c_Start(uint8_t side)
{
	if(side == 0)         //������
  {
    SDA_OUT_LEFT();     //sda�����
    IIC_SDA_LEFT=1;	  	  
    IIC_SCL_LEFT=1;
    delay_0_6();
    IIC_SDA_LEFT=0;//START:when CLK is high,DATA change form high to low 
    delay_0_6();
    IIC_SCL_LEFT=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
    delay_0_6();
  }
  else if (side == 1)    //������
  {
    SDA_OUT_RIGHT();     //sda�����
    IIC_SDA_RIGHT=1;	  	  
    IIC_SCL_RIGHT=1;
    delay_0_6();
    IIC_SDA_RIGHT=0;//START:when CLK is high,DATA change form high to low 
    delay_0_6();
    IIC_SCL_RIGHT=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
    delay_0_6();
  }
}	  

//����IICֹͣ�ź�
void i2c_Stop(uint8_t side)
{
	if(side == 0)
  {
    SDA_OUT_LEFT();//sda�����
    IIC_SCL_LEFT=0;
    IIC_SDA_LEFT=0;//STOP:when CLK is high DATA change form low to high
    IIC_SCL_LEFT=1; 
    delay_0_6();
    IIC_SDA_LEFT=1;//����I2C���߽����ź�
    delay_0_6();
  }
  else if (side == 1)
  {
    SDA_OUT_RIGHT();//sda�����
    IIC_SCL_RIGHT=0;
    IIC_SDA_RIGHT=0;//STOP:when CLK is high DATA change form low to high
    IIC_SCL_RIGHT=1; 
    delay_0_6();
    IIC_SDA_RIGHT=1;//����I2C���߽����ź�
    delay_0_6();
  }
							   	
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 i2c_Wait_Ack(uint8_t side)
{
	u8 ucErrTime=0;
  if(side == 0)
  {
    SDA_IN_LEFT();      //SDA����Ϊ����  
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
    IIC_SCL_LEFT=0;//ʱ�����0 	   
    return 0;  
  }
  if(side == 1)
  {
    SDA_IN_RIGHT();      //SDA����Ϊ����  
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
    IIC_SCL_RIGHT=0;//ʱ�����0 	   
    return 0;  
  }
} 

//����ACKӦ��
void i2c_Ack(uint8_t side)
{
	if(side == 0)
  {
    IIC_SCL_LEFT=0;         //CLK����
    SDA_OUT_LEFT();
    IIC_SDA_LEFT=0;         //��CLKΪ�͵�ƽ�ڼ����ͣ�����Ӧ��
    delay_0_6();
    IIC_SCL_LEFT=1;         //��֤CLK�ڸߵ�ƽ�ڼ䣬SDA�߱���Ϊԭ�ȵĵ͵�ƽ
    delay_0_6();
    IIC_SCL_LEFT=0; 
  }
  else if(side == 1)
  {
    IIC_SCL_RIGHT=0;         //CLK����
    SDA_OUT_RIGHT();
    IIC_SDA_RIGHT=0;         //��CLKΪ�͵�ƽ�ڼ����ͣ�����Ӧ��
    delay_0_6();
    IIC_SCL_RIGHT=1;         //��֤CLK�ڸߵ�ƽ�ڼ䣬SDA�߱���Ϊԭ�ȵĵ͵�ƽ
    delay_0_6();
    IIC_SCL_RIGHT=0; 
  }
}

//������ACKӦ��		    
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

//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void i2c_Send_Byte(uint8_t side,u8 txd)
{                        
   u8 t;   
  if(side == 0)
  {
    SDA_OUT_LEFT(); 	    
    IIC_SCL_LEFT=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA_LEFT=(txd&0x80)>>7;
        txd<<=1; 	  
    delay_0_6();   //��TEA5767��������ʱ���Ǳ����
    IIC_SCL_LEFT=1;
    delay_0_6();
    IIC_SCL_LEFT=0;
    }
  }
  else if(side == 1)
  {
    SDA_OUT_RIGHT(); 	    
    IIC_SCL_RIGHT=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA_RIGHT=(txd&0x80)>>7;
        txd<<=1; 	  
    delay_0_6();   //��TEA5767��������ʱ���Ǳ����
    IIC_SCL_RIGHT=1;
    delay_0_6();
    IIC_SCL_RIGHT=0;
    }
  } 
} 	 

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 i2c_Read_Byte(uint8_t side,unsigned char ack)
{
	unsigned char i,receive=0;
	if(side == 0)
  {
    SDA_IN_LEFT();//SDA����Ϊ����
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
          i2c_NAck(side);//����nACK
      else
          i2c_Ack(side); //����ACK   
      return receive;
  }
  else if(side == 1)
  {
    SDA_IN_RIGHT();//SDA����Ϊ����
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
          i2c_NAck(side);//����nACK
      else
          i2c_Ack(side); //����ACK   
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
