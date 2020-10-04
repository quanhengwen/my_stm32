
#include <stdint.h>
#include "fifo.h"
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include  "stm32f10x.h"

/**
  * @brief  注册一个fifo
  * @param  pfifo: fifo结构体指针
						pfifo_buf: fifo内存块
						size: 长度
  * @retval none
*/
void fifo_register(_fifo_t *pfifo, uint8_t *pfifo_buf, uint32_t size,lock_fun lock, lock_fun unlock)
{
		pfifo->buf_size =  size;
		pfifo->buf 			=  pfifo_buf;
		pfifo->pwrite   =  pfifo->buf;
		pfifo->pread    =  pfifo->buf;
		pfifo->occupy_size = 0;
		pfifo->lock     =  lock;
		pfifo->unlock   =  unlock;
}
/**
  * @brief  释放fifo
  * @param  pfifo: fifo结构体指针
  * @retval none
*/
void fifo_release(_fifo_t *pfifo)
{
		pfifo->buf_size = 0;				//缓冲区大小为0
		pfifo->occupy_size = 0;			//有效数据大小为0
		pfifo->buf = NULL;					//缓冲区内容为空
		pfifo->pwrite = 0;					//写入缓冲区内容为0	
		pfifo->pread  = 0;					//读取缓冲区内容为0
		pfifo->lock   = NULL;				//上锁函数为空
		pfifo->unlock = NULL;				//解锁函数为空
}
/**
  * @brief  往fifo写数据
  * @param  pfifo: fifo结构体指针
		    pbuf: 待写数据
		    size: 待写数据大小
  * @retval 实际写大小
*/
uint32_t fifo_write(_fifo_t *pfifo,const uint8_t *pbuf,uint32_t size)
{
	uint32_t w_size = 0,free_size = 0;
	
	if((size ==0 )||(pfifo == NULL) || (pbuf == NULL))
	{
			return 0;
	}
	free_size = fifo_get_free_size(pfifo);
	
	if(free_size == 0)
	{
	  return 0;
	}
	if(free_size < size)		
	{
		size = free_size;
	}
	w_size = size;
	
	if(pfifo->lock != NULL)
		pfifo->lock();
	
	while(w_size-- > 0)
	{
		*pfifo->pwrite++ = *pbuf++;
		if(pfifo->pwrite >= (pfifo->buf + pfifo->buf_size))		//写满,超出缓存区大小
		{
				pfifo->pwrite = pfifo->buf;				//指向首地址
		}
		pfifo->occupy_size++;
	}
	if(pfifo->unlock != NULL)
		pfifo->unlock();
	
	return size;
}
/**
  * @brief  从fifo读数据
  * @param  pfifo: fifo结构体指针
		    pbuf: 待读数据缓存
		    size: 待读数据大小
  * @retval 实际读大小
*/
uint32_t fifo_read(_fifo_t *pfifo,uint8_t *pbuf,uint32_t size)
{
		uint32_t r_size = 0,occupy_size = 0;
	
		if((size == 0) || (pfifo == NULL) ||(pbuf == NULL))
		{
			return 0;
		}
		occupy_size = fifo_get_occupy_size(pfifo);
		
		if(occupy_size == 0)
		{
			return 0;
		}
		if(occupy_size < size)
		{
			size = occupy_size;
		}
		
		r_size = size;
		
		if(pfifo->lock != NULL)
		pfifo->lock();
	
		while(r_size-- > 0)
		{
			*pbuf++ = *pfifo->pread++;		//将读取数据存入到缓冲区
			if(pfifo->pread >= (pfifo->buf + pfifo->buf_size))	//溢出
			{
				pfifo->pread = pfifo->buf;	//指向首地址
			}
			pfifo->occupy_size--;
		}
		
		if(pfifo->unlock != NULL)
		pfifo->unlock();
	
	return size;

}

/*
*@brief   获取fifo空间大小
*@param 	pfifo:fifo结构体指针
*@retval  fifo大小
*/
uint32_t fifo_get_total_size(_fifo_t *pfifo)
{
	if(pfifo == NULL)
	{
		return 0;
	}
	return pfifo->buf_size;
}

/*
*@brief   获取fifo空闲空间大小
*@param 	pfifo:fifo结构体指针
*@retval  空闲空间大小
*/
uint32_t fifo_get_free_size(_fifo_t *pfifo)
{
	uint32_t size;
	
	if(pfifo == NULL)
	return 0;
	
	size = pfifo->buf_size - pfifo->occupy_size;
	
	return size;
}
/**
  * @brief  获取fifo已用空间大小
  * @param  pfifo: fifo结构体指针
  * @retval fifo已用大小
*/
uint32_t fifo_get_occupy_size(_fifo_t *pfifo)
{
	if (pfifo==NULL)
		return 0;
    
	return  pfifo->occupy_size;
}
