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

/*1码的表示方法为850us的高电平,400us的低电平
注意一定要是寄存器操作，否则IP的速度不够，
延迟时间不要轻易改动，因为这是用示波器测量出来的，比较精准*/
void RGB_Set_Up( uint16_t GPIO_PIN_BR )
{
	GPIOB->BSRRL = GPIO_PIN_BR;
	delay_ns(6);               //6,3
	GPIOB->BSRRH = GPIO_PIN_BR;
	delay_ns(6);               //6,3
}

/*0码的表示方法为400us的高电平，850us的低电平。
注意一定要是寄存器操作，否则IO口速度不够，
延迟时间不要轻易改动，因为这是用示波器测量出来的，比较精准*/
void RGB_Set_Down( uint16_t GPIO_PIN_BR )
{
	GPIOB->BSRRL = GPIO_PIN_BR;    //置高
	delay_ns(1);
	GPIOB->BSRRH = GPIO_PIN_BR;    //置低
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

//将某个灯截止不亮
void RGB_Rst( uint16_t GPIO_PIN_BR )
{
	   GPIOB->BSRRH = GPIO_PIN_BR;
	   delay_us(80);
}












