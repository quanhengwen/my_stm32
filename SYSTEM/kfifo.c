/***********************************************************
*֧��FreeRTOS ucosii����(�ο�Fatfs���빦�ܸ���)
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

//OS_EVENT   *FILE_MUTEX;	   /* ���ڻ��� */

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
						FIFOд����������
*****************************************/
static int lock_kfifo (
	KFIFO* fifo		/* kfifo  object */
)
{
	return fifo_req_grant(fifo->sobj);
}
/*****************************************
						FIFOд���������
*****************************************/
static void unlock_kfifo (
	KFIFO* fifo,		/* kfifo system object */
)
{
		fifo_rel_grant(fifo->sobj);
}

#endif
//�ҳ���ӽ� ���2��ָ������
uint32_t roundup_pow_of_two(uint32_t date_roundup_pow_of_two )
{            
    /* ������� STM32 Ӳ���ṩ�ļ���ǰ����ָ�� CLZ
     * �ٸ����ӣ��������date_roundup_pow_of_two 0x09
     *(������Ϊ:0000 0000 0000 0000 0000 0000 0000 1001), ��bit3��bit0Ϊ1
     * ��__clz( (date_roundup_pow_of_two)��ֵΪ28,�����λΪ1 ǰ����28��0,32-28 =3 �������λ1 ��λ��
     * 31UL ��ʾ �޷��� int ���� 31,����Ĭ��Ϊ �з��� int ���� 31
			 ���û��ul��׺����ϵͳĬ��ΪInt����
     * ����ο�  FreeRTOS ��Ѱ��������ȼ������д��
 */

    return ( 1UL << ( 32UL - ( uint32_t ) __clz( (date_roundup_pow_of_two) ) ) );

}


/* 
 * ÿ�ε����������������������ڴ�� һ��ָ��struct KFIFO,һ��ָ�� KFIFO.buff
 * ��ˣ�����������ڴ�鲻��ʹ�ã����ͷŵ�
 */

 KFIFO *kfifo_alloc(uint32_t size) 
{   
    uint8_t *buffer;
    
     KFIFO *ret;
	
		// ����ʹ��FreeRTOS�ķ����ڴ��API
		// ret=(struct KFIFO *) pvPortMalloc(sizeof (struct KFIFO));
		// ����ʹ���Զ���ķ����ڴ��API		
        ret = (KFIFO*)mymalloc(sizeof(KFIFO));
    /*  
     * round up to the next power of 2, since our 'let the indices  
     * wrap' tachnique works only in this case.  
      * ���size��2�Ĵ���Բ������size & (size - 1)  =0
     */
  
    if (size & (size - 1)) 
    {   
        //BUG_ON(size > 0x80000000);              
        //�����Ҫ�����buffer ����2�Ĵ���Բ��,��Ҫ��size���2�Ĵ���Բ���������������
        size = roundup_pow_of_two(size);
    }
        
    //����ʹ��FreeRTOS�ķ����ڴ��API
 //   buffer = (unsigned char*) pvPortMalloc(size);
    //����ʹ���Զ���ķ����ڴ��API		
		buffer = (uint8_t*) mymalloc(size);		
        
    if (!buffer)   //��������ڴ�ʧ��
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
    if (!ret) 		//��������ڴ�ʧ��
			
//			vPortFree(buffer);  //FreeRTOS�ͷ��ڴ溯��
        myfree(buffer);    //�Զ����ڴ��ͷź���
  
    return ret;
        
}
/*�ͷ�һ��FIF0*/
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
	  myfree(pfifo->buffer);    //�Զ����ڴ��ͷź���
}

uint32_t __kfifo_put( KFIFO *fifo, uint8_t *buffer, uint32_t len)  
{
    uint32_t L;
    
		if ((len==0) || (fifo==NULL) || (buffer==NULL))
		{
			return 0;
		}
		//���ζ��е�ʣ������ΪFIFO->size - fifo->in +fifo->out
	  //��д��ĳ���ȡLen��ʣ�������н�С�ģ�����д���
    len = min( len , fifo->size - fifo->in + fifo->out );
  
    //	����
		#if _KFIFO_Mutex_EN 
			if (fifo->lock != NULL)		
			fifo->lock();	
		#endif		
  
	/* �������ݴ�fifo.in ���ڵ�λ�ÿ�ʼд,д֮ǰ,����Ҫ��һ��fifo->in��bufferĩβ�Ĵ�С�ǲ��Ǳ�len ��*/

	/*
	 * ǰ�潲����fifo->size�Ѿ���2�Ĵ���Բ��,��Ҫ�Ƿ����������,����Ч��
	 * �ڶ�10���������ʱ��,���Ƿ���,�������������еĸ�λ�ϵ����֣������ù�����λ��ʲô;
	 * ����,kfifo->in % kfifo->size ����ת��Ϊ kfifo->in & (kfifo->size -1),Ч�ʻ�����
	 * ����fifo->size - (fifo->in & (fifo->size - L)) ��Ϊ fifo->in �� bufferĩβ��ʣ��ĳ���,
	 * Lȡlen��ʣ�೤�ȵ���Сֵ,��ΪҪ����L �ֽڵ�fifo->buffer + fifo->in��λ����
	 */ 
    L = min(len, fifo->size - (fifo->in & (fifo->size - 1)));		//������д��λ��
    
    memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, L);    //��buffer L�����ݿ�����fifo->buffer + fifo->in��λ����
  
    /* then put the rest (if any) at the beginning of the buffer */ 

    memcpy(fifo->buffer, buffer + L, len - L);                          //��ʣ�����ݿ�����fifo->buffer��ʼλ��

		#if _KFIFO_Mutex_EN 
			if (fifo->unlock != NULL)
			fifo->unlock();
		#endif			

  
		/* 
		 * ע������ֻ������ fifo->in +=  len��δȡģ,
		 * �����kfifo����ƾ���֮���������õ���unsigned int���������,
		 * ��In�������ӵ����ʱ�ֻᱻ��Ϊ0�������ͽ�ʡ��ÿ��in��ǰ���Ӷ�Ҫȡģ������,
		 */
  
    fifo->in += len; 
        
    /*����ֵ����д�����ݵĸ����������Ϳ��Ը��ݷ���ֵ�жϻ������Ƿ�д��*/
    return len;   
}  
  
uint32_t __kfifo_get( KFIFO *fifo, uint8_t *buffer, uint32_t len)
{
    uint32_t L;   
	
		if ((len==0) || (fifo==NULL) || (buffer==NULL))
		{
			return 0;
		}
 		//���пɶ�ȡ���ݳ���fifo->in -fifo->out
	  //�ö�ȡ�ĳ���ȡLen�Ϳɶ�ȡ��С�Ƚϣ�ȡ��Сֵ������д��� 
    len = min(len, fifo->in - fifo->out);   
  
  
    //	����
		#if _KFIFO_Mutex_EN 
			if (fifo->lock != NULL)		
			fifo->lock();	
		#endif	
  
    /* first get the data from fifo->out until the end of the buffer */   
    L = min(len, fifo->size - (fifo->out & (fifo->size - 1)));   				//��ȡ��ʼλ��ʣ��λ��
	
    memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), L);   //�������ݵ�buffer
  
    /* then get the rest (if any) from the beginning of the buffer */   
    memcpy(buffer + L, fifo->buffer, len - L);                					//�����ʣ�ཫʣ�����ݴ�fifo->buffer��ʼλ�ö�ȡ
  
		#if _KFIFO_Mutex_EN 
			if (fifo->unlock != NULL)
			fifo->unlock();
		#endif		
            
		/* 
		 * ע������ֻ������ fifo->out +=  len��δȡģ,
		 * �����kfifo����ƾ���֮���������õ���unsigned int���������,
				C++��׼�涨�޷�������������0
		 * ��out�������ӵ����ʱ�ֻᱻ��Ϊ0
		 * ���In�����������in<out���������ôin-outΪ����(�ֽ����)
		 * in-out��ֵ����Ϊbuffer�е����ݳ���
		 */

    fifo->out += len;					//���Ӷ�ȡ����
  
    return len;  
}
