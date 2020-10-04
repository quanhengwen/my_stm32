/***********************************************************
*支持FreeRTOS ucosii互斥(参考Fatfs重入功能改造)
************************************************************/
#include "kfifo.h"
#include <string.h>

#if _KFIFO_Mutex_EN
#include "FreeRTOS.h"
#include "task.h"
#else
#include "malloc.h"
#endif

#define min(a, b)                (((a) < (b)) ? (a) : (b))

#if _KFIFO_Mutex_EN

//OS_EVENT   *FILE_MUTEX;	   /* 用于互斥 */

int kfifo_cre_syncobj (	/* 1:Function succeeded, 0:Could not create due to any error */
	_Kfifo_t* sobj		/* Pointer to return the created sync object */
)
{
//	unsigned char err;
	int ret;
		
//	FILE_MUTEX = OSMutexCreate(0, &err);	   /* uC/OS-II */
//	*sobj = *FILE_MUTEX;
//	ret = (int)(err == OS_ERR_NONE);

  *sobj = xSemaphoreCreateMutex();	        	/* FreeRTOS */
	ret = (int)(*sobj != NULL);
	
	return ret;
}
int fifo_del_syncobj (	/* 1:Function succeeded, 0:Could not delete due to any error */
	_Kfifo_t sobj		/* Sync object tied to the logical drive to be deleted */
)
{
//	unsigned char err;
	int ret;

//	OSMutexDel(&sobj, OS_DEL_ALWAYS, &err);	 /* uC/OS-II */
//	ret = (int)(err == OS_ERR_NONE);

  vSemaphoreDelete(sobj);		             			/* FreeRTOS */
	ret = 1;

	return ret;
}
int fifo_req_grant (	/* TRUE:Got a grant to access the volume, FALSE:Could not get a grant */
	_Kfifo_t sobj	/* Sync object to wait */
)
{
//	unsigned char err;
//	int ret;
	
//	OSMutexPend(&sobj, _FS_TIMEOUT, &err);		/* uC/OS-II */
//	ret = (int)(err == OS_ERR_NONE);

	ret = (int)(xSemaphoreTake(sobj, _KFIFO_TIMEOUT) == pdTRUE);	/* FreeRTOS */

	return ret;
}

void fifo_rel_grant (
	_Kfifo_t sobj	/* Sync object to be signaled */
)
{
//	OSMutexPost(&sobj);		/* uC/OS-II */

	 xSemaphoreGive(sobj);	/* FreeRTOS */
}
/*****************************************
						FIFO写入上锁功能
*****************************************/
static int lock_kfifo (
	KFIFO* fifo		/* kfifo  object */
)
{
	return fifo_req_grant(fifo->sobj);
}
/*****************************************
						FIFO写入解锁功能
*****************************************/
static void unlock_kfifo (
	KFIFO* fifo,		/* kfifo system object */
)
{
		fifo_rel_grant(fifo->sobj);
}

#endif
//找出最接近 最大2的指数次幂
uint32_t roundup_pow_of_two(uint32_t date_roundup_pow_of_two )
{            
    /* 这里采用 STM32 硬件提供的计算前导零指令 CLZ
     * 举个栗子，假如变量date_roundup_pow_of_two 0x09
     *(二进制为:0000 0000 0000 0000 0000 0000 0000 1001), 即bit3和bit0为1
     * 则__clz( (date_roundup_pow_of_two)的值为28,即最高位为1 前面有28个0,32-28 =3 代表最高位1 的位置
     * 31UL 表示 无符号 int 数字 31,否则默认为 有符号 int 数字 31
			 如果没有ul后缀，则系统默认为Int类型
     * 这里参考  FreeRTOS 的寻找最高优先级任务的写法
 */

    return ( 1UL << ( 32UL - ( uint32_t ) __clz( (date_roundup_pow_of_two) ) ) );

}


/* 
 * 每次调用这个函数都会产生两个内存块 一个指向struct KFIFO,一个指向 KFIFO.buff
 * 因此，如果这两个内存块不再使用，请释放掉
 */

 KFIFO *kfifo_alloc(uint32_t size) 
{   
    uint8_t *buffer;
    
     KFIFO *ret;
	
		// 这里使用FreeRTOS的分配内存的API
		// ret=(struct KFIFO *) pvPortMalloc(sizeof (struct KFIFO));
		// 这里使用自定义的分配内存的API		
        ret = (KFIFO*)mymalloc(sizeof(KFIFO));
    /*  
     * round up to the next power of 2, since our 'let the indices  
     * wrap' tachnique works only in this case.  
      * 如果size是2的次幂圆整，则size & (size - 1)  =0
     */
  
    if (size & (size - 1)) 
    {   
        //BUG_ON(size > 0x80000000);              
        //如果你要申请的buffer 不是2的次幂圆整,就要把size变成2的次幂圆整，方便下面计算
        size = roundup_pow_of_two(size);
    }
        
    //这里使用FreeRTOS的分配内存的API
 //   buffer = (unsigned char*) pvPortMalloc(size);
    //这里使用自定义的分配内存的API		
		buffer = (uint8_t*) mymalloc(size);		
        
    if (!buffer)   //如果申请内存失败
        return 0UL; 
        
        ret->buffer = buffer;
        ret->size   = size;
        ret->in  = 0;
        ret->out = 0;
				#if _KFIFO_Mutex_EN 
				if (!ff_cre_syncobj(&ret->sobj)) return 0UL;
				ret->lock = lock_kfifo(ret);
				ret->unlock = unlock_kfifo(ret); 
				#endif
    if (!ret) 		//如果申请内存失败
			
//			vPortFree(buffer);  //FreeRTOS释放内存函数
        myfree(buffer);    //自定义内存释放函数
  
    return ret;
        
}
/*释放一个FIF0*/
void kfifo_release(KFIFO *pfifo)
{
		pfifo->buffer = NULL;
		pfifo->size   = 0;
		pfifo->in  = 0;
		pfifo->out = 0;
	
		#if _KFIFO_Mutex_EN 
		if (!fifo_del_syncobj(&ret->sobj)) return 0UL;
		ret->lock = NULL;
		ret->unlock = NULL; 
		#endif
	  myfree(pfifo); 
	  myfree(pfifo->buffer);    //自定义内存释放函数
}

uint32_t __kfifo_put( KFIFO *fifo, uint8_t *buffer, uint32_t len)  
{
    uint32_t L;
    
		if ((len==0) || (fifo==NULL) || (buffer==NULL))
		{
			return 0;
		}
		//环形队列的剩余容量为FIFO->size - fifo->in +fifo->out
	  //让写入的长度取Len和剩余容量中较小的，避免写溢出
    len = min( len , fifo->size - fifo->in + fifo->out );
  
    //	上锁
		#if _KFIFO_Mutex_EN 
			if (fifo->lock != NULL)		
			fifo->lock();	
		#endif		
  
	/* 首先数据从fifo.in 所在的位置开始写,写之前,首先要看一下fifo->in到buffer末尾的大小是不是比len 大*/

	/*
	 * 前面讲到的fifo->size已经是2的次幂圆整,主要是方便这里计算,提升效率
	 * 在对10进行求余的时候,我们发现,余数总是整数中的个位上的数字，而不用管气体位是什么;
	 * 所以,kfifo->in % kfifo->size 可以转换为 kfifo->in & (kfifo->size -1),效率会提升
	 * 所以fifo->size - (fifo->in & (fifo->size - L)) 即为 fifo->in 到 buffer末尾所剩余的长度,
	 * L取len和剩余长度的最小值,即为要拷贝L 字节到fifo->buffer + fifo->in的位置上
	 */ 
    L = min(len, fifo->size - (fifo->in & (fifo->size - 1)));		//新数据写入位置
    
    memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, L);    //将buffer L个数据拷贝到fifo->buffer + fifo->in的位置上
  
    /* then put the rest (if any) at the beginning of the buffer */ 

    memcpy(fifo->buffer, buffer + L, len - L);                          //将剩余数据拷贝到fifo->buffer起始位置

		#if _KFIFO_Mutex_EN 
			if (fifo->unlock != NULL)
			fifo->unlock();
		#endif			

  
		/* 
		 * 注意这里只是用了 fifo->in +=  len而未取模,
		 * 这就是kfifo的设计精妙之处，这里用到了unsigned int的溢出性质,
		 * 当In持续增加到溢出时又会被置为0，这样就节省了每次in向前增加都要取模的性能,
		 */
  
    fifo->in += len; 
        
    /*返回值代表写入数据的个数，这样就可以根据返回值判断缓冲区是否写满*/
    return len;   
}  
  
uint32_t __kfifo_get( KFIFO *fifo, uint8_t *buffer, uint32_t len)
{
    uint32_t L;   
	
		if ((len==0) || (fifo==NULL) || (buffer==NULL))
		{
			return 0;
		}
 		//队列可读取数据长度fifo->in -fifo->out
	  //让读取的长度取Len和可读取大小比较，取较小值，避免写溢出 
    len = min(len, fifo->in - fifo->out);   
  
  
    //	上锁
		#if _KFIFO_Mutex_EN 
			if (fifo->lock != NULL)		
			fifo->lock();	
		#endif	
  
    /* first get the data from fifo->out until the end of the buffer */   
    L = min(len, fifo->size - (fifo->out & (fifo->size - 1)));   				//读取起始位置剩余位置
	
    memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), L);   //拷贝数据到buffer
  
    /* then get the rest (if any) from the beginning of the buffer */   
    memcpy(buffer + L, fifo->buffer, len - L);                					//如果有剩余将剩余数据从fifo->buffer起始位置读取
  
		#if _KFIFO_Mutex_EN 
			if (fifo->unlock != NULL)
			fifo->unlock();
		#endif		
            
		/* 
		 * 注意这里只是用了 fifo->out +=  len而未取模,
		 * 这就是kfifo的设计精妙之处，这里用到了unsigned int的溢出性质,
				C++标准规定无符号整型溢出变成0
		 * 当out持续增加到溢出时又会被置为0
		 * 如果In先溢出，出现in<out的情况，那么in-out为负数(又将溢出)
		 * in-out的值还是为buffer中的数据长度
		 */

    fifo->out += len;					//增加读取长度
  
    return len;  
}
