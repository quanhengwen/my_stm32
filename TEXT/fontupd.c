/*****************************************************************************
  * @file    fontupd.c
  * @author  Mr_Q
  * @version V1.0
  * @date    2019-01-17
  * @brief   �ֿ������ʼ�������ں����ֿ����
  ***************************************************************************/ 
#include "fontupd.h"  
#include "fatfs_flash_spi.h"  

//�ֿ�����ʼ��ַ
#define FONTINFOADDR 	1024*(1024*12+500) 				//MiniSTM32�Ǵ�12M+500K��ַ��ʼ��

_font_info ftinfo;

u8 font_init(void)
{			  												 
	FLASH_SPI_disk_initialize(); 
	
 	SPI_FLASH_BufferRead((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//����ftinfo�ṹ������
	
	if(ftinfo.fontok!=0XAA)return 1;								//�ֿ����. 
	
	return 0;		 //�ֿ���ȷ   
}

/****************************End File Mr_Q_2019-11-5***************************/

