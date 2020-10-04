#ifndef __FIFO_H_
#define __FIFO_H_

#include <stdbool.h>
#include <stdint.h>

typedef void (*lock_fun)(void);

typedef struct{
	
	uint8_t *buf;						/*缓冲区*/
	uint32_t buf_size;
	uint32_t occupy_size;
	uint8_t *pwrite;
	uint8_t *pread;
	
	void (*lock)(void);			/*互斥上锁*/
	void (*unlock)(void);		/*互斥解锁*/

}_fifo_t;

extern void fifo_register(_fifo_t *pfifo, uint8_t *pfifo_buf, uint32_t size,
                            lock_fun lock, lock_fun unlock);
extern void fifo_release(_fifo_t *pfifo);									//释放一个队列缓冲区
extern uint32_t fifo_write(_fifo_t *pfifo, const uint8_t *pbuf, uint32_t size);	//向队列中写入数据
extern uint32_t fifo_read(_fifo_t *pfifo, uint8_t *pbuf, uint32_t size);				//从队列中读取数据
extern uint32_t fifo_get_total_size(_fifo_t *pfifo);			//获取FIFO空间大小
extern uint32_t fifo_get_free_size(_fifo_t *pfifo);				//获取空闲大小
extern uint32_t fifo_get_occupy_size(_fifo_t *pfifo);			//获取有效数据大小


#endif

