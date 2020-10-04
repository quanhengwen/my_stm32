#ifndef _KFIFO_H_
#define _KFIFO_H_

#include <stdbool.h>
#include <stdint.h>

#define _KFIFO_Mutex_EN		0
#define _KFIFO_TIMEOUT		0
#define	_Kfifo_t			SemaphoreHandle_t

#if _KFIFO_Mutex_EN
//����һ���ṹ��kfifo
typedef void (*lock_fun)(void);

#endif



typedef struct 
{   
    uint8_t *buffer;    /* ���ݻ����� */   
    uint32_t size;       /* ��������С */   
    uint32_t in;         /* data is added at offset (in % size) */   
    uint32_t out;        /* data is extracted from off. (out % size) */   
  
	#if _KFIFO_Mutex_EN 
		/*STM32ֻ��һ�����ģ�ͬһʱ��ֻ��д���߶�����˲���Ҫ*/
		_Kfifo_t	sobj;	
		void (*lock)(void);			/* �������� */
		void (*unlock)(void);		/* ������� */ 
	#endif
}KFIFO;

uint32_t roundup_pow_of_two( uint32_t date_roundup_pow_of_two );

KFIFO *kfifo_alloc(uint32_t size);

void kfifo_release(KFIFO *pfifo);

uint32_t __kfifo_put(KFIFO *fifo, uint8_t *buffer, uint32_t len);

uint32_t __kfifo_get(KFIFO *fifo, uint8_t *buffer, uint32_t len);

#endif
