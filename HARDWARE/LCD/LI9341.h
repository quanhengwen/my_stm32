#ifndef __LI9341_H
#define __LI9341_H		
#include "sys.h"	 
#include "stdlib.h"
 #include "string.h" 

//LI9341��Ҫ������
typedef struct  
{			
	
	uint16_t 	width;			//LI9341 ���
	uint16_t 	height;			//LI9341 �߶�
	uint16_t 	id;				//LI9341 ID
	uint8_t  	dir;			//���������������ƣ�0��������1��������	
	uint16_t	wramcmd;		//��ʼдgramָ��
	uint16_t  setxcmd;			//����x����ָ��
	uint16_t  setycmd;			//����y����ָ�� 
	
}_LI9341_dev; 	  
#define	LCD_LED PCout(10) 				//LCD����    	PC10 
 
#define	LCD_CS_SET  GPIOC->BSRR=1<<9    //Ƭѡ�˿�  		PC9
#define	LCD_RS_SET	GPIOC->BSRR=1<<8    //����/���� 		PC8	   
#define	LCD_WR_SET	GPIOC->BSRR=1<<7    //д����			PC7
#define	LCD_RD_SET	GPIOC->BSRR=1<<6    //������			PC6
								    
#define	LCD_CS_CLR  GPIOC->BRR=1<<9     //Ƭѡ�˿�  		PC9
#define	LCD_RS_CLR	GPIOC->BRR=1<<8     //����/����		PC8	   
#define	LCD_WR_CLR	GPIOC->BRR=1<<7     //д����			PC7
#define	LCD_RD_CLR	GPIOC->BRR=1<<6     //������			PC6   


//PB0~15,��Ϊ������
#define DATAOUT(x) GPIOB->ODR=x; //�������
#define DATAIN     GPIOB->IDR;   //��������	
//LI9341����
extern _LI9341_dev LI9341dev;	//����LI9341��Ҫ����

//LI9341�Ļ�����ɫ�ͱ���ɫ	   
extern uint16_t  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern uint16_t  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

//////////////////////////////////////////////////////////////////////////////////	 
//-----------------LI9341�˿ڶ���---------------- 
#define	LI9341_LED_SET 	GPIO_SetBits(GPIOB,GPIO_Pin_15);			//GPIOB15 ���øߣ���Ļ��
#define	LI9341_LED_CLR 	GPIO_ResetBits(GPIOB,GPIO_Pin_15);		//GPIOB15 ���õͣ���Ļ��


/***************************************************************************************
2^26 =0X0400 0000 = 64MB,ÿ�� BANK ��4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

ѡ��BANK1-BORSRAM4 ���� TFT����ַ��ΧΪ0X6C00 0000 ~ 0X6FFF FFFF
FSMC_A6 ��LI9341��DC(�Ĵ���/����ѡ��)��
�Ĵ�������ַ = 0X6C00 0000
RAM����ַ = 0X6D00 0000 = 0X6C00 0000+2^6*2 = 0X6C00 0000 + 0X80 = 0X6C00 0080
 *2ԭ����FSMC���͵�ַʱ���Զ�����һλ����A6ʵ����A7  2`7=OX80
��ѡ��ͬ�ĵ�ַ��ʱ����ַҪ���¼���  
****************************************************************************************/    
#define FSMC_LI9341_CMD              ((uint32_t)0x6C00007E)          //FSMC_Bank1_NORSRAM����LI9341��������ĵ�ַ
#define FSMC_LI9341_DATA             ((uint32_t)0x6C000080)      		//FSMC_Bank1_NORSRAM����LI9341���ݲ����ĵ�ַ   
//////////////////////////////////////////////////////////////////////////////////
	 
//������ɫ
#define WHITE         	 		0xFFFF
#define BLACK         	 		0x0000	  
#define BLUE         	 	 	0x001F  
#define BRED             		0XF81F
#define GRED 			 		0XFFE0
#define GBLUE			 		0X07FF
#define RED           	 		0xF800
#define MAGENTA       	 		0xF81F
#define GREEN         	 		0x07E0
#define CYAN          	 		0x7FFF
#define YELLOW        	 		0xFFE0
#define BROWN 			 		0XBC40 //��ɫ
#define BRRED 			 		0XFC07 //�غ�ɫ
#define GRAY  			 		0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 		0X01CF	//����ɫ
#define LIGHTBLUE      	 		0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 		0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 		0X841F //ǳ��ɫ
//#define LIGHTGRAY        		0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 		 	0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        			0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           			0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
	    															  
void LI9341_Init(void);													   				//��ʼ��
uint16_t LI9341_ReadID(void);
void LI9341_DisplayOn(void);																//����ʾ
void LI9341_DisplayOff(void);																//����ʾ
void LI9341_Clear(uint16_t Color);	 												//����
void LI9341_SetCursor(uint16_t Xpos, uint16_t Ypos);				//���ù��

void LI9341_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor);
void LI9341_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t _usColor);							//����
void LI9341_DrawRectangle(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);		//������
void LI9341_FillRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);
uint16_t LCD_BGR2RGB(uint16_t c);
void LI9341_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);												//��ʾһ���ַ�
void LI9341_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);  												//��ʾһ������
void LI9341_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);						//��ʾ ����
void LI9341_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);			//��ʾһ���ַ���,12/16����
void LI9341_WriteReg(uint16_t LI9341_Reg, uint16_t LI9341_RegValue);
uint16_t LI9341_ReadReg(uint16_t LI9341_Reg);
void LI9341_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr);
void Picture_Draw(uint16_t S_x,uint16_t S_y,const unsigned char *pic);
//GUI�ӿں���
void LI9341_PutPixel(uint16_t x,uint16_t y,uint16_t color);								//���ٻ���
uint16_t  LI9341_GetPixel(uint16_t x,uint16_t y); 													//���� 
void LI9341_DrawHLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usColor);
void LI9341_DrawVLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usY2 , uint16_t _usColor);
void LI9341_DrawHColorLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, const uint16_t *_pColor);
void LI9341_DrawHTransLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, const uint16_t *_pColor);
uint16_t LCD_GetHeight(void);
uint16_t LCD_GetWidth(void);

void Show_Str1(uint16_t x,uint16_t y,uint8_t*str,uint8_t size);
#endif  

