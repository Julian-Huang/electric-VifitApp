#include "Motor.h"

void StepMotor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
	
	//����PA7ΪSTEP
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//����PA6ΪDIR
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;   //��©���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	while(motor_flag==1)
//	{ 
//		StepMotor_ctrl(1, 8);  //dir=1Ϊ������
//		if(motor_flag == 0)
//		{
//			 StepMotor_ctrl(0, 200*16*10+43*16);  //dir=0Ϊ������,����0�� 20*16 1��1cm
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
		delay_us(32*1.8);   //1.6 ,1.8 ��Խ��,��ʱ��,Խ��
		GPIO_ResetBits(GPIOA, GPIO_Pin_7);
		delay_us(96*1.8);
	}
}















