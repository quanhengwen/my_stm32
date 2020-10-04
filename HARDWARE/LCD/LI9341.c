/*****************************************************************************
  * @file    LI9341.c
  * @author  Mr_Q
  * @version V1.0
  * @date    2019-11-21
  * @brief   ͨ���Ͷ��CD4X�ⶨ��������(����FreeRTOS����ϵͳ��STemwin GUI��ʾ���桢
						 FATFS�ļ�ϵͳ��USB�洢�����⴮��)�ⶨ������ѡ����Ϊ13�����壬����3��
						 �����ʽ��
	ע����� 1.������У׼���궨������ͨ��2Сʱ�Ժ��ٽ��в���
  ***************************************************************************/
#include "LI9341.h"
#include "stdlib.h"
#include "font.h" 
#include "delay.h"	 
#include "fatfs_flash_spi.h"
#include  "config.h"

//LI9341�Ļ�����ɫ�ͱ���ɫ	   
uint16_t POINT_COLOR = RED;	//������ɫ
uint16_t BACK_COLOR = 0xFFFF;  //����ɫ

static void LI9341_SetDispWin(uint16_t Xpos, uint16_t Ypos, uint16_t _usHeight, uint16_t _usWidth);
//����LI9341��Ҫ����
//Ĭ��Ϊ����
_LI9341_dev LI9341dev;
	 
/**
  * @brief  ��LI9341д������
  * @param  usData :Ҫд�������
  * @retval ��
  */	
void LI9341_WR_REG(__IO uint16_t regval)
{   
	LCD_RS_CLR;//д��ַ  
 	LCD_CS_CLR; 
	DATAOUT(regval); 
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET;  
}
/**
  * @brief  ��LI9341д������
  * @param  usData :Ҫд�������
  * @retval ��
  */	
void LI9341_WR_DATA(__IO uint16_t data)
{	  
	LCD_RS_SET;
	LCD_CS_CLR;
	DATAOUT(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET; 
}
/**
  * @brief  ��LI9341��LI9341����
  * @param  
  * @retval ������ֵ
  */	
uint16_t LI9341_RD_DATA(void)
{
	u16 t;
 	GPIOB->CRL=0X88888888; //PB0-7  ��������
	GPIOB->CRH=0X88888888; //PB8-15 ��������
	GPIOB->ODR=0X0000;     //ȫ�����0

	LCD_RS_SET;
	LCD_CS_CLR;
	//��ȡ����(���Ĵ���ʱ,������Ҫ��2��)
	LCD_RD_CLR;
	delay_us(2);				   
	t=DATAIN;  
	LCD_RD_SET;
	LCD_CS_SET; 

	GPIOB->CRL=0X33333333; //PB0-7  �������
	GPIOB->CRH=0X33333333; //PB8-15 �������
	GPIOB->ODR=0XFFFF;    //ȫ�������
	return t;  
}					   
/**
  * @brief  ��LI9341ָ���Ĵ���д����
  * @param  �Ĵ�����ַ Ҫд���ֵ
  * @retval 
  */	
void LI9341_WriteReg(uint16_t LI9341_Reg,uint16_t LI9341_RegValue)
{	
	LI9341_WR_REG(LI9341_Reg);		
	LI9341_WR_DATA( LI9341_RegValue);//д������	    		 
}	   
/**
  * @brief  ��LI9341ָ���Ĵ���������
  * @param  �Ĵ�����ַ
  * @retval ������ֵ
  */	
uint16_t LI9341_ReadReg(uint16_t LI9341_Reg)
{										   
	LI9341_WR_REG(LI9341_Reg);		//д��Ҫ���ļĴ������	  
	return LI9341_RD_DATA();		//���ض�����ֵ
}   

//��mdk -O1ʱ���Ż�ʱ��Ҫ����
//��ʱi
void opt_delay(uint8_t i)
{
	while(i--);
}	 
//LI9341������ʾ
void LI9341_DisplayOn(void)
{					   
	LI9341_WR_REG(0X29);		//������ʾ
}	 
//LI9341�ر���ʾ
void LI9341_DisplayOff(void)
{	   
	LI9341_WR_REG(0X28);		//�ر���ʾ
} 
/*
*********************************************************************************************************
*	�� �� ��: LI9341_SetDirection
*	����˵��: ������ʾ����
*	��    ��:  _ucDir : ��ʾ������� 0 ��������, 1=����180�ȷ�ת, 2=����, 3=����180�ȷ�ת
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LI9341_SetDirection(uint8_t _ucDir)
{
	/*
		Memory Access Control (3600h)
		This command defines read/write scanning direction of the frame memory.

		These 3 bits control the direction from the MPU to memory write/read.

		Bit  Symbol  Name  Description
		D7   MY  Row Address Order
		D6   MX  Column Address Order
		D5   MV  Row/Column Exchange
		D4   ML  Vertical Refresh Order  LI9341 vertical refresh direction control. ��

		D3   BGR RGB-BGR Order   Color selector switch control
		     (0 = RGB color filter panel, 1 = BGR color filter panel )
		D2   MH  Horizontal Refresh ORDER  LI9341 horizontal refreshing direction control.
		D1   X   Reserved  Reserved
		D0   X   Reserved  Reserved
	*/
	LI9341_WR_REG(0x36);
	/* 0 ��ʾ����(��������)��1��ʾ����(��������), 2��ʾ����(���������)  3��ʾ���� (�������ұ�) */
	if (_ucDir == 0)
	{
		LI9341_WR_DATA(0xA8);	/* ����(���������) */
		LI9341dev.height = 240;
		LI9341dev.width = 320;
	}
	else if (_ucDir == 1)
	{
		LI9341_WR_DATA(0x68);	/* ���� (�������ұ�) */
		LI9341dev.height = 240;
		LI9341dev.width = 320;
	}
	else if (_ucDir == 2)
	{
		LI9341_WR_DATA(0xC8);	/* ����(��������) */
		LI9341dev.height = 320;
		LI9341dev.width = 240;
	}
	else if (_ucDir == 3)
	{
		LI9341_WR_DATA(0x00);	/* ����(��������) */
		LI9341dev.height = 320;
		LI9341dev.width = 240;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: LI9341_SetDispWin
*	����˵��: ������ʾ���ڣ����봰����ʾģʽ��TFT����оƬ֧�ִ�����ʾģʽ�����Ǻ�һ���12864����LI9341���������
*	��    ��:
*		_usX : ˮƽ����
*		_usY : ��ֱ����
*		_usHeight: ���ڸ߶�
*		_usWidth : ���ڿ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void LI9341_SetDispWin(uint16_t Xpos, uint16_t Ypos, uint16_t _usHeight, uint16_t _usWidth)
{
	LI9341_WR_REG(0x2A);			
	LI9341_WR_DATA(Xpos>> 8); 		
	LI9341_WR_DATA(Xpos); 		
	LI9341_WR_DATA((Xpos+ _usWidth - 1) >> 8);
	LI9341_WR_DATA(Xpos+ _usWidth - 1);		

	LI9341_WR_REG(0x2B);			  		
	LI9341_WR_DATA(Ypos>> 8); 		
	LI9341_WR_DATA(Ypos); 		
	LI9341_WR_DATA((Ypos+ _usHeight - 1) >> 8);
	LI9341_WR_DATA(Ypos+ _usHeight - 1);	
}
//���ù��λ��
//Xpos:������
//Ypos:������
void LI9341_SetCursor(uint16_t Xpos, uint16_t Ypos)
{	 
	LI9341_WR_REG(0x2A);			
	LI9341_WR_DATA(Xpos>> 8); 		
	LI9341_WR_DATA(Xpos&0XFF); 
	LI9341_WR_DATA(Xpos>> 8); 		
	LI9341_WR_DATA(Xpos&0XFF); 

	LI9341_WR_REG(0x2B);			  		
	LI9341_WR_DATA(Ypos>> 8); 		
	LI9341_WR_DATA(Ypos&0XFF); 
	LI9341_WR_DATA(Ypos>> 8); 		
	LI9341_WR_DATA(Ypos&0XFF); 
} 		    
/*
*********************************************************************************************************
*	�� �� ��: LI9341_PutPixel
*	����˵��: ��1������
*	��    ��:
*			_usX,_usY : ��������
*			_usColor  ��������ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LI9341_PutPixel(uint16_t x,uint16_t y,uint16_t color)
{	   
	LI9341_SetCursor(x,y);//���ù��λ��
	LI9341_WR_REG(0x2C);
	LI9341_WR_DATA(color);
}	
//��ILI93xx����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ��
//ͨ���ú���ת��
//c:GBR��ʽ����ɫֵ
//����ֵ��RGB��ʽ����ɫֵ
u16 LCD_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 
/*
*********************************************************************************************************
*	�� �� ��: LI9341_GetPixel
*	����˵��: ��ȡ1������
*	��    ��:
*			_usX,_usY : ��������
*			_usColor  ��������ɫ
*	�� �� ֵ: RGB��ɫֵ ��565��
*********************************************************************************************************
*/
uint16_t LI9341_GetPixel(uint16_t x,uint16_t y)
{
 	uint16_t r=0,g=0,b=0;	   
	LI9341_SetCursor(x,y);						/* ���ù��λ�� */
	
	LI9341_WR_REG(0X2E);						//9320 ���Ͷ�GRAMָ�� 
  
 	r=LI9341_RD_DATA();							/* ��1���ƶ������� */	   
	opt_delay(2);	  
 	r=LI9341_RD_DATA();  		  				//ʵ��������ɫ
	opt_delay(2);	  
	b=LI9341_RD_DATA(); 
	g=r&0XFF;		//����9341/5310/5510,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
	g<<=8; 
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));//ILI9341/NT35310/LI9341��Ҫ��ʽת��һ��
}	
/*
*********************************************************************************************************
*	�� �� ��: Init_9488
*	����˵��: ��ʼ��LI9341������
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LI9341_Init(void)
{ 	 	
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); //ʹ��PORTB,Cʱ�Ӻ�AFIOʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//����SWD��ʧ��JTAG
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;	   ///PORTC6~10�����������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //GPIOC	

	GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	//  PORTB�������
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB
 
	GPIO_SetBits(GPIOB,GPIO_Pin_All); 		 
	delay_ms(50); // 
	LI9341dev.id = LI9341_ReadID();				//��ȡLI9341�汾��	
	LI9341_WR_REG(0xCF);  
	LI9341_WR_DATA(0x00); 
	LI9341_WR_DATA(0xC1); 
	LI9341_WR_DATA(0X30); 
	LI9341_WR_REG(0xED);  
	LI9341_WR_DATA(0x64); 
	LI9341_WR_DATA(0x03); 
	LI9341_WR_DATA(0X12); 
	LI9341_WR_DATA(0X81); 
	LI9341_WR_REG(0xE8);  
	LI9341_WR_DATA(0x85); 
	LI9341_WR_DATA(0x10); 
	LI9341_WR_DATA(0x7A); 
	LI9341_WR_REG(0xCB);  
	LI9341_WR_DATA(0x39); 
	LI9341_WR_DATA(0x2C); 
	LI9341_WR_DATA(0x00); 
	LI9341_WR_DATA(0x34); 
	LI9341_WR_DATA(0x02); 
	LI9341_WR_REG(0xF7);  
	LI9341_WR_DATA(0x20); 
	LI9341_WR_REG(0xEA);  
	LI9341_WR_DATA(0x00); 
	LI9341_WR_DATA(0x00); 
	LI9341_WR_REG(0xC0);    	//Power control 
	LI9341_WR_DATA(0x1B);   	//VRH[5:0] 
	LI9341_WR_REG(0xC1);    	//Power control 
	LI9341_WR_DATA(0x01);   	//SAP[2:0];BT[3:0] 
	LI9341_WR_REG(0xC5);    	//VCM control 
	LI9341_WR_DATA(0x30); 	 	//3F
	LI9341_WR_DATA(0x30); 	 	//3C
	LI9341_WR_REG(0xC7);    	//VCM control2 
	LI9341_WR_DATA(0XB7); 
	LI9341_WR_REG(0x36);    	//Memory Access Control 
	LI9341_WR_DATA(0x48); 
	LI9341_WR_REG(0x3A);   
	LI9341_WR_DATA(0x55); 
	LI9341_WR_REG(0xB1);   
	LI9341_WR_DATA(0x00);   
	LI9341_WR_DATA(0x1A); 
	LI9341_WR_REG(0xB6);    	//Display Function Control 
	LI9341_WR_DATA(0x0A); 
	LI9341_WR_DATA(0xA2); 
	LI9341_WR_REG(0xF2);    	//3Gamma Function Disable 
	LI9341_WR_DATA(0x00); 
	LI9341_WR_REG(0x26);    	//Gamma curve selected 
	LI9341_WR_DATA(0x01); 
	LI9341_WR_REG(0xE0);    	//Set Gamma 
	LI9341_WR_DATA(0x0F); 
	LI9341_WR_DATA(0x2A); 
	LI9341_WR_DATA(0x28); 
	LI9341_WR_DATA(0x08); 
	LI9341_WR_DATA(0x0E); 
	LI9341_WR_DATA(0x08); 
	LI9341_WR_DATA(0x54); 
	LI9341_WR_DATA(0XA9); 
	LI9341_WR_DATA(0x43); 
	LI9341_WR_DATA(0x0A); 
	LI9341_WR_DATA(0x0F); 
	LI9341_WR_DATA(0x00); 
	LI9341_WR_DATA(0x00); 
	LI9341_WR_DATA(0x00); 
	LI9341_WR_DATA(0x00); 		 
	LI9341_WR_REG(0XE1);    
	LI9341_WR_DATA(0x00); 
	LI9341_WR_DATA(0x15); 
	LI9341_WR_DATA(0x17); 
	LI9341_WR_DATA(0x07); 
	LI9341_WR_DATA(0x11); 
	LI9341_WR_DATA(0x06); 
	LI9341_WR_DATA(0x2B); 
	LI9341_WR_DATA(0x56); 
	LI9341_WR_DATA(0x3C); 
	LI9341_WR_DATA(0x05); 
	LI9341_WR_DATA(0x10); 
	LI9341_WR_DATA(0x0F); 
	LI9341_WR_DATA(0x3F); 
	LI9341_WR_DATA(0x3F); 
	LI9341_WR_DATA(0x0F); 
	LI9341_WR_REG(0x2B); 
	LI9341_WR_DATA(0x00);
	LI9341_WR_DATA(0x00);
	LI9341_WR_DATA(0x01);
	LI9341_WR_DATA(0x3f);
	LI9341_WR_REG(0x2A); 
	LI9341_WR_DATA(0x00);
	LI9341_WR_DATA(0x00);
	LI9341_WR_DATA(0x00);
	LI9341_WR_DATA(0xef);	 
	LI9341_WR_REG(0x11); 			//Exit Sleep
	delay_ms(120);
	LI9341_WR_REG(0x29); 			//display on	

	LI9341_SetDirection(0);
	LI9341_LED_SET;					//��������

	LI9341_Clear(BLACK);
}  
/*
*********************************************************************************************************
*	�� �� ��: LI9341_ReadID
*	����˵��: ��ȡLI9341����оƬID�� 4��bit
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint16_t LI9341_ReadID(void)
{
	uint8_t ID[3];
	LI9341_WR_REG(0xD3);
	LI9341_RD_DATA();//�ն�
	ID[0] = LI9341_RD_DATA();
	ID[1] = LI9341_RD_DATA();
	ID[2] = LI9341_RD_DATA();
	return ((ID[1]<<8) | ID[2]);
}

/*
*********************************************************************************************************
*	�� �� ��: LI9341_Clear
*	����˵��: �����������ɫֵ����
*	��    ��: _usColor : ����ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LI9341_Clear(uint16_t color)
{
	uint32_t index=0;      
	uint32_t totalpoint;
	LI9341_SetDispWin(0,0,LI9341dev.height,LI9341dev.width);
	LI9341_WR_REG(0x2C);								//��ʼд��GRAM	
	totalpoint = (LI9341dev.width*LI9341dev.height)/8; 				//�õ��ܵ���	
	for(index=0;index<totalpoint;index++)
	{
		LI9341_WR_DATA(color);
		LI9341_WR_DATA(color);
		LI9341_WR_DATA(color);
		LI9341_WR_DATA(color);
		
		LI9341_WR_DATA(color);
		LI9341_WR_DATA(color);
		LI9341_WR_DATA(color);
		LI9341_WR_DATA(color);
	}
}  
//����
//x1,y1:�������
//x2,y2:�յ�����  
void LI9341_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t _usColor)
{
	int32_t dx , dy ;
	int32_t tx , ty ;
	int32_t inc1 , inc2 ;
	int32_t d , iTag ;
	int32_t x , y ;

	/* ���� Bresenham �㷨����2��仭һ��ֱ�� */

	LI9341_PutPixel(x1 , y1 , _usColor);

	/* ��������غϣ���������Ķ�����*/
	if ( x1 == x2 && y1 == y2 )
	{
		return;
	}

	iTag = 0 ;
	/* dx = abs ( _usX2 - _usX1 ); */
	if (x2 >= x1)
	{
		dx = x2 - x1;
	}
	else
	{
		dx = x1 - x2;
	}

	/* dy = abs ( _usY2 - _usY1 ); */
	if (y2 >= y1)
	{
		dy = y2 - y1;
	}
	else
	{
		dy = y1 - y2;
	}

	if ( dx < dy )   /*���dyΪ�Ƴ������򽻻��ݺ����ꡣ*/
	{
		uint16_t temp;

		iTag = 1 ;
		temp = x1; x1 = y1; y1 = temp;
		temp = x2; x2 = y2; y2 = temp;
		temp = dx; dx = dy; dy = temp;
	}
	tx = x2 > x1 ? 1 : -1 ;    /* ȷ������1���Ǽ�1 */
	ty = y2 > y1 ? 1 : -1 ;
	x = x1 ;
	y = y1 ;
	inc1 = 2 * dy ;
	inc2 = 2 * ( dy - dx );
	d = inc1 - dx ;
	while ( x != x2 )     /* ѭ������ */
	{
		if ( d < 0 )
		{
			d += inc1 ;
		}
		else
		{
			y += ty ;
			d += inc2 ;
		}
		if ( iTag )
		{
			LI9341_PutPixel ( y , x , _usColor) ;
		}
		else
		{
			LI9341_PutPixel ( x , y , _usColor) ;
		}
		x += tx ;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: LCD_GetHeight
*	����˵��: ��ȡLCD�ֱ���֮�߶�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint16_t LCD_GetHeight(void)
{
	return LI9341dev.height;
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_GetWidth
*	����˵��: ��ȡLCD�ֱ���֮���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint16_t LCD_GetWidth(void)
{
	return LI9341dev.width;
}

/*
*********************************************************************************************************
*	�� �� ��: LI9341_DrawHLine
*	����˵��: ����һ��ˮƽ�� ����Ҫ����UCGUI�Ľӿں�����
*	��    ��:  _usX1    ����ʼ��X����
*			  _usY1    ��ˮƽ�ߵ�Y����
*			  _usX2    ��������X����
*			  _usColor : ��ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LI9341_DrawHLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usColor)
{
	uint16_t i;

	LI9341_SetDispWin(_usX1, _usY1, 1, _usX2 - _usX1 + 1);

	LI9341_WR_REG(0x2C);

	/* д�Դ� */
	for (i = 0; i <_usX2-_usX1 + 1; i++)
	{
		LI9341_WR_DATA(_usColor);
	}
}
/*
*********************************************************************************************************
*	�� �� ��: LI9341_DrawVLine
*	����˵��: ����ֱƽ�� ��UCGUI�Ľӿں���
*	��    ��: X,Y���������ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LI9341_DrawVLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usY2 , uint16_t _usColor)
{
	uint16_t i;
	
	for (i = _usY1; i <=_usY2; i++)
	{	
		LI9341_PutPixel(_usX1, i, _usColor);	
	}
}
/*
*********************************************************************************************************
*	�� �� ��: LI9341_DrawHColorLine
*	����˵��: ����һ����ɫˮƽ�� ����Ҫ����UCGUI�Ľӿں�����
*	��    ��:  _usX1    ����ʼ��X����
*			  _usY1    ��ˮƽ�ߵ�Y����
*			  _usWidth ��ֱ�ߵĿ��
*			  _pColor : ��ɫ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LI9341_DrawHColorLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, const uint16_t *_pColor)
{
	uint16_t i;
	
	LI9341_SetDispWin(_usX1, _usY1, 1, _usWidth);

	LI9341_WR_REG(0x2C);

	/* д�Դ� */
	for (i = 0; i <_usWidth; i++)
	{
		LI9341_WR_DATA(*_pColor++);
	}
}
/*
*********************************************************************************************************
*	�� �� ��: LI9341_DrawHTransLine
*	����˵��: ����һ����ɫ͸����ˮƽ�� ����Ҫ����UCGUI�Ľӿں������� ��ɫֵΪ0��ʾ͸��ɫ
*	��    ��:  _usX1    ����ʼ��X����
*			  _usY1    ��ˮƽ�ߵ�Y����
*			  _usWidth ��ֱ�ߵĿ��
*			  _pColor : ��ɫ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LI9341_DrawHTransLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, const uint16_t *_pColor)
{
	uint16_t i, j;
	uint16_t Index;

	LI9341_SetCursor(_usX1, _usY1);

	/* д�Դ� */
	LI9341_WR_REG(0x2C);
	for (i = 0,j = 0; i < _usWidth; i++, j++)
	{
		Index = *_pColor++;
	    if (Index)
        {
			 LI9341_WR_DATA(Index);
		}
		else
		{
			LI9341_SetCursor(_usX1 + j, _usY1);
			LI9341_WR_REG(0x22);
			LI9341_WR_DATA(Index);
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: LI9341_DrawRectangle
*	����˵��: �����Ρ�
*	��    ��:
*			_usX,_usY���������Ͻǵ�����
*			_usHeight �����εĸ߶�
*			_usWidth  �����εĿ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LI9341_DrawRectangle(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor)
{
	LI9341_DrawLine(_usX, _usY, _usX + _usWidth - 1, _usY, _usColor);
	LI9341_DrawLine(_usX, _usY + _usHeight - 1, _usX + _usWidth - 1, _usY + _usHeight - 1, _usColor);
	LI9341_DrawLine(_usX, _usY, _usX, _usY + _usHeight - 1, _usColor);	/* �� */
	LI9341_DrawLine(_usX + _usWidth - 1, _usY, _usX + _usWidth - 1, _usY + _usHeight, _usColor);	/* �� */
}
/*
*********************************************************************************************************
*	�� �� ��: LI9341_DrawCircle
*	����˵��: ����һ��Բ���ʿ�Ϊ1������
*	��    ��:
*			_usX,_usY  ��Բ�ĵ�����
*			_usRadius  ��Բ�İ뾶
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LI9341_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor)
{
	int32_t  D;			/* Decision Variable */
	uint32_t  CurX;		/* ��ǰ X ֵ */
	uint32_t  CurY;		/* ��ǰ Y ֵ */

	D = 3 - (_usRadius << 1);
	CurX = 0;
	CurY = _usRadius;

	while (CurX <= CurY)
	{
		LI9341_PutPixel(_usX + CurX, _usY + CurY, _usColor);
		LI9341_PutPixel(_usX + CurX, _usY - CurY, _usColor);
		LI9341_PutPixel(_usX - CurX, _usY + CurY, _usColor);
		LI9341_PutPixel(_usX - CurX, _usY - CurY, _usColor);
		LI9341_PutPixel(_usX + CurY, _usY + CurX, _usColor);
		LI9341_PutPixel(_usX + CurY, _usY - CurX, _usColor);
		LI9341_PutPixel(_usX - CurY, _usY + CurX, _usColor);
		LI9341_PutPixel(_usX - CurY, _usY - CurX, _usColor);

		if (D < 0)
		{
			D += (CurX << 2) + 6;
		}
		else
		{
			D += ((CurX - CurY) << 2) + 10;
			CurY--;
		}
		CurX++;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: LI9341_FillRect
*	����˵��: �����Ρ�
*	��    ��:
*			_usX,_usY���������Ͻǵ�����
*			_usHeight �����εĸ߶�
*			_usWidth  �����εĿ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LI9341_FillRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor)
{
	uint32_t i;

	/*
	 ---------------->---
	|(_usX��_usY)        |
	V                    V  _usHeight
	|                    |
	 ---------------->---
		  _usWidth
	*/

	LI9341_SetDispWin(_usX, _usY, _usHeight, _usWidth);

	LI9341_WR_REG(0x2C);
	
	for (i = 0; i < _usHeight * _usWidth; i++)
	{
		LI9341_WR_DATA(_usColor);
	}
}
								  
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LI9341_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{  							  
  uint8_t temp,t1,t;
	uint16_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 		//����1206����
		else if(size==16)temp=asc2_1608[num][t];	//����1608����
		else if(size==24)temp=asc2_2412[num][t];	//����2412����
		else return;								//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)		LI9341_PutPixel(x,y,disp_color);
			else if(mode==0)	LI9341_PutPixel(x,y,back_color);
			temp<<=1;
			y++;
			if(y>=LI9341dev.height)return;		//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=LI9341dev.width)return;	//��������
				break;
			}
		}  	 
	}  	    	   	 	  
}   
//m^n����
//����ֵ:m^n�η�.
uint32_t LI9341_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
void LI9341_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LI9341_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LI9341_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LI9341_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
void LI9341_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{  
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LI9341_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LI9341_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LI9341_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LI9341_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LI9341_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{         
	uint8_t x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LI9341_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

/*
*********************************************************************************************************
*	�� �� ��: LI9341_DrawBMP
*	����˵��: ��LCD����ʾһ��BMPλͼ��λͼ����ɨ����򣺴����ң����ϵ���
*	��    ��:
*			_usX, _usY : ͼƬ������
*			_usHeight  ��ͼƬ�߶�
*			_usWidth   ��ͼƬ���
*			_ptr       ��ͼƬ����ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LI9341_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr)
{
	uint32_t index = 0;
	const uint16_t *p;

	/* ����ͼƬ��λ�úʹ�С�� ��������ʾ���� */
	LI9341_SetDispWin(_usX, _usY, _usHeight, _usWidth);
	p = _ptr;
	
	for (index = 0; index < _usHeight * _usWidth; index++)
	{
		LI9341_PutPixel(_usX, _usY, *p++);
	}
	/* �˳����ڻ�ͼģʽ */
	LI9341_SetDispWin(0, 0, LI9341dev.height, LI9341dev.width);//�˳�������ʾģʽ����Ϊȫ����ʾģʽ
}
void Picture_Draw(uint16_t S_x,uint16_t S_y,const unsigned char *pic)
{
	uint32_t i=0;
	uint16_t temp,D_x=0,D_y=0;
		 
	while(i<768000)      
	{
		temp=(uint16_t)(pic[i]+(pic[i+1]<<8));     
		if(D_x>799) {D_x=0;D_y++;}         
		LI9341_PutPixel(S_x+D_x,S_y+D_y,temp);   
		D_x++;
		i=i+2;
	}    
}
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size)
{		    
	unsigned char qh,ql;
	unsigned char i;					  
	unsigned long foffset; 
	u8 csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	qh=*code;
	ql=*(++code);
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//�� ���ú���
	{   		    
	    for(i=0;i<csize;i++)*mat++=0x00;//�������
	    return; //��������
	}          
	if(ql<0x7f)ql-=0x40;//ע��!
	else ql-=0x41;
	qh-=0x81;   
	foffset=((unsigned long)190*qh+ql)*csize;	//�õ��ֿ��е��ֽ�ƫ����  		  
	switch(size)
	{
		case 12:
		  SPI_FLASH_BufferRead(mat,foffset+ftinfo.f12addr,24);
		break;
		case 16:
		  SPI_FLASH_BufferRead(mat,foffset+ftinfo.f16addr,32);
		break;
		case 24:
		  SPI_FLASH_BufferRead(mat,foffset+ftinfo.f24addr,72);
		break;	
	}     												    
}  
//��ʾһ��ָ����С�ĺ���
//x,y :���ֵ�����
//font:����GBK��
//size:�����С
//mode:0,������ʾ,1,������ʾ	   
void Show_Font(u16 x,u16 y,u8 *font,u8 size)
{
	u8 temp,t,t1;
	u16 x0=x;
	u8 dzk[72];   
	u8 csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	if(size!=12&&size!=16&&size!=24)return;	//��֧�ֵ�size
	Get_HzMat(font,dzk,size);	//�õ���Ӧ��С�ĵ������� 
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];			//�õ���������                          
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80) LI9341_PutPixel(x,y,disp_color);
			else LI9341_PutPixel(x,y,back_color); 
			temp<<=1;
			x++;
			if((x-x0)==size)
			{
				x=x0;
				y++;
				break;
			}
		}  	 
	} 
}
//��ָ��λ�ÿ�ʼ��ʾһ���ַ���	    
//��֧���Զ�����
//(x,y):��ʼ����
//width,height:����
//str  :�ַ���
//size :�����С
//mode:0,�ǵ��ӷ�ʽ;1,���ӷ�ʽ    	   		   
void Show_Str1(u16 x,u16 y,u8*str,u8 size)
{					
	u16 x0=x;						  	  
    u8 bHz=0;     //�ַ���������  	    				    				  	  
    while(*str!=0)//����δ����
    { 
        if(!bHz)
        {
			if(*str>0x80)bHz=1;//���� 
			else              //�ַ�
			{         
				if(*str==13)//���з���
				{         
						y+=size;
						x=x0;
						str++; 
				}  
				else LI9341_ShowChar(x,y,*str,size,0);//��Ч����д�� 
				str++; 
				x+=size/2; //�ַ�,Ϊȫ�ֵ�һ�� 
			}
        }
		else//���� 
        {     
			bHz=0;//�к��ֿ�    				     
			Show_Font(x,y,str,size); //��ʾ�������,������ʾ 
			str+=2; 
			x+=size;//��һ������ƫ��	    
        }						 
    }   
}  			 		 

