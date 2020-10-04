#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define Usart1ReadLen 400  //串口1接收数据数组大小
#define Usart1SendLen 200  //定义最大发送字节数

#define RINGBUFF_LEN  400
#define FLASE 0
#define TRUE 1

extern char Usart1ReadBuff[Usart1ReadLen];  //接收数据缓存
extern u8   Usart1ReadFlage;				//串口1接收到一条完整数据
extern u8   Usart1SendBuff[Usart1SendLen];  //发送数据缓存--环形队列用

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

void uart_init(u32 bound);//串口1初始化
void UsartOutChar(unsigned char c);			//串口发送一个字节
void Uart1_SendStr( char* SendBuff );		//串口1发送字符串数据

#endif


