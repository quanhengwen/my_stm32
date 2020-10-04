/*****************************************************************************
  * @file    fontupd.c
  * @author  Mr_Q
  * @version V1.0
  * @date    2019-01-17
  * @brief   字库参数初始化，用于后续字库调用
  ***************************************************************************/ 
#include "fontupd.h"  
#include "fatfs_flash_spi.h"  

//字库存放起始地址
#define FONTINFOADDR 	1024*(1024*12+500) 				//MiniSTM32是从12M+500K地址开始的

_font_info ftinfo;

u8 font_init(void)
{			  												 
	FLASH_SPI_disk_initialize(); 
	
 	SPI_FLASH_BufferRead((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//读出ftinfo结构体数据
	
	if(ftinfo.fontok!=0XAA)return 1;								//字库错误. 
	
	return 0;		 //字库正确   
}

/****************************End File Mr_Q_2019-11-5***************************/

