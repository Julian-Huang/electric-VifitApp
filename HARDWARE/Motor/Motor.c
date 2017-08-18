#include "Motor.h"

void StepMotor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
	
	//配置PA7为STEP
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//配置PA6为DIR
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;   //开漏输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	while(motor_flag==1)
//	{ 
//		StepMotor_ctrl(1, 8);  //dir=1为正方向
//		if(motor_flag == 0)
//		{
//			 StepMotor_ctrl(0, 200*16*10+43*16);  //dir=0为负方向,调回0点 20*16 1次1cm
//		}
//	}
	motor_flag = 1;
}

void StepMotor_ctrl(uint8_t dir, uint32_t steps)
{
	uint32_t i;
	dir == 1 ? GPIO_SetBits(GPIOA, GPIO_Pin_6) : GPIO_ResetBits(GPIOA, GPIO_Pin_6);
	for(i=0; i<=steps; i++)
  {
		GPIO_SetBits(GPIOA, GPIO_Pin_7);
		delay_us(32*1.8);   //1.6 ,1.8 数越大,延时久,越慢
		GPIO_ResetBits(GPIOA, GPIO_Pin_7);
		delay_us(96*1.8);
	}
}















