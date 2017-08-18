#include "sys.h" 
#include "delay.h"
#include "rgb.h"


void RGB_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
	
	GPIO_Init( GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits( GPIOB, GPIO_Pin_12);
	GPIO_SetBits( GPIOB, GPIO_Pin_13);
	GPIO_SetBits( GPIOB, GPIO_Pin_4);
	GPIO_SetBits( GPIOB, GPIO_Pin_5);
	GPIO_SetBits( GPIOB, GPIO_Pin_6);
	GPIO_SetBits( GPIOB, GPIO_Pin_7);
}

/*1��ı�ʾ����Ϊ850us�ĸߵ�ƽ,400us�ĵ͵�ƽ
ע��һ��Ҫ�ǼĴ�������������IP���ٶȲ�����
�ӳ�ʱ�䲻Ҫ���׸Ķ�����Ϊ������ʾ�������������ģ��ȽϾ�׼*/
void RGB_Set_Up( uint16_t GPIO_PIN_BR )
{
	GPIOB->BSRRL = GPIO_PIN_BR;
	delay_ns(6);               //6,3
	GPIOB->BSRRH = GPIO_PIN_BR;
	delay_ns(6);               //6,3
}

/*0��ı�ʾ����Ϊ400us�ĸߵ�ƽ��850us�ĵ͵�ƽ��
ע��һ��Ҫ�ǼĴ�������������IO���ٶȲ�����
�ӳ�ʱ�䲻Ҫ���׸Ķ�����Ϊ������ʾ�������������ģ��ȽϾ�׼*/
void RGB_Set_Down( uint16_t GPIO_PIN_BR )
{
	GPIOB->BSRRL = GPIO_PIN_BR;    //�ø�
	delay_ns(1);
	GPIOB->BSRRH = GPIO_PIN_BR;    //�õ�
	delay_ns(10);
}	

void RGB_Set(u32 G8R8B8 , u16 GPIO_PIN_BR)
{
	    int i;
	    u8 byte = 0;
	    for(i = 23; i>=0; i--)
	    {
		      byte = ( (G8R8B8>>i) & 0x01 );
				  if(byte==1)
					{
						RGB_Set_Up(GPIO_PIN_BR);
						
					}
					else
					{
						RGB_Set_Down(GPIO_PIN_BR);
					}
	    }
}

//��ĳ���ƽ�ֹ����
void RGB_Rst( uint16_t GPIO_PIN_BR )
{
	   GPIOB->BSRRH = GPIO_PIN_BR;
	   delay_us(80);
}












