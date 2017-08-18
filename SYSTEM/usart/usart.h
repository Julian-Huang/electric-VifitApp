#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.csom
//�޸�����:2011/6/14
//�汾��V1.4
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
////////////////////////////////////////////////////////////////////////////////// 	

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define USART2_REC_LEN  		200  	//�����������ֽ��� 200
#define EN_USART1_RX      1
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����


/******************************* �궨�� ***************************/
#define            macNVIC_PriorityGroup_x                     NVIC_PriorityGroup_1

/********************************** �������� ***************************************/
void                     USART_printf                       ( USART_TypeDef * USARTx, char * Data, ... );

	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //����1���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART2_RX_BUF[USART_REC_LEN]; //����2���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����1����״̬���	
extern u16 USART2_RX_STA;         		//����2����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void uart2_init(u32 bound);

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void USART2_printf ( USART_TypeDef * USARTx, char * Data, ... );
static char * itoa( int value, char *string, int radix );

#endif


