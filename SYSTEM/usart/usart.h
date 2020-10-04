#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define Usart1ReadLen 400  //����1�������������С
#define Usart1SendLen 200  //����������ֽ���

#define RINGBUFF_LEN  400
#define FLASE 0
#define TRUE 1

extern char Usart1ReadBuff[Usart1ReadLen];  //�������ݻ���
extern u8   Usart1ReadFlage;				//����1���յ�һ����������
extern u8   Usart1SendBuff[Usart1SendLen];  //�������ݻ���--���ζ�����

typedef struct
{
    u16 Head;           
    u16 Tail;
    u16 Lenght;
    u8  Ring_Buff[RINGBUFF_LEN];
}RingBuff_t;

void ringBuff_Init(void);
u8 Write_RingBuff(u8 data);
u8 Read_RingBuff(u8 *rData);

void uart_init(u32 bound);//����1��ʼ��
void UsartOutChar(unsigned char c);			//���ڷ���һ���ֽ�
void Uart1_SendStr( char* SendBuff );		//����1�����ַ�������

#endif


