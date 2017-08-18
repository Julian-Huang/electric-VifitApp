#ifndef __RGB_H
#define __RGB_H

#include "stm32f4xx.h"

//�ܽ�BSRR�Ĵ�����ַ
#define    RGB1        0X1000       //GPIO_PIN_12
#define    RGB2        0X2000       //GPIO_PIN_12
#define    RGB3        0X0010       //GPIO_PIN_4
#define    RGB4        0X0020       //GPIO_PIN_5
#define    RGB5        0X0040       //GPIO_PIN_6
#define    RGB6        0X0080       //GPIO_PIN_7


//��ͨ������λ����������ɫ��ǳ
#define    RED         0XFF00        //���
#define    GREEN       0XFF0000      //�̹�
#define    BLUE        0XFF          //����
#define    WHITE       0XFFFFFF      //�׹�
#define    CUTDOWN     0X000000      //�ر���ʾ

void RGB_Init(void);
void RGB_Set_Up( uint16_t GPIO_PIN_BR ); 
void RGB_Set_Down( uint16_t GPIO_PIN_BR );
void RGB_Set(u32 G8R8B8 , u16 GPIO_PIN_BR);
void RGB_Rst( uint16_t GPIO_PIN_BR );

#endif
