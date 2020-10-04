#ifndef __LI9341_H
#define __LI9341_H		
#include "sys.h"	 
#include "stdlib.h"
 #include "string.h" 

//LI9341重要参数集
typedef struct  
{			
	
	uint16_t 	width;			//LI9341 宽度
	uint16_t 	height;			//LI9341 高度
	uint16_t 	id;				//LI9341 ID
	uint8_t  	dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	uint16_t	wramcmd;		//开始写gram指令
	uint16_t  setxcmd;			//设置x坐标指令
	uint16_t  setycmd;			//设置y坐标指令 
	
}_LI9341_dev; 	  
#define	LCD_LED PCout(10) 				//LCD背光    	PC10 
 
#define	LCD_CS_SET  GPIOC->BSRR=1<<9    //片选端口  		PC9
#define	LCD_RS_SET	GPIOC->BSRR=1<<8    //数据/命令 		PC8	   
#define	LCD_WR_SET	GPIOC->BSRR=1<<7    //写数据			PC7
#define	LCD_RD_SET	GPIOC->BSRR=1<<6    //读数据			PC6
								    
#define	LCD_CS_CLR  GPIOC->BRR=1<<9     //片选端口  		PC9
#define	LCD_RS_CLR	GPIOC->BRR=1<<8     //数据/命令		PC8	   
#define	LCD_WR_CLR	GPIOC->BRR=1<<7     //写数据			PC7
#define	LCD_RD_CLR	GPIOC->BRR=1<<6     //读数据			PC6   


//PB0~15,作为数据线
#define DATAOUT(x) GPIOB->ODR=x; //数据输出
#define DATAIN     GPIOB->IDR;   //数据输入	
//LI9341参数
extern _LI9341_dev LI9341dev;	//管理LI9341重要参数

//LI9341的画笔颜色和背景色	   
extern uint16_t  POINT_COLOR;//默认红色    
extern uint16_t  BACK_COLOR; //背景颜色.默认为白色

//////////////////////////////////////////////////////////////////////////////////	 
//-----------------LI9341端口定义---------------- 
#define	LI9341_LED_SET 	GPIO_SetBits(GPIOB,GPIO_Pin_15);			//GPIOB15 设置高，屏幕亮
#define	LI9341_LED_CLR 	GPIO_ResetBits(GPIOB,GPIO_Pin_15);		//GPIOB15 设置低，屏幕灭


/***************************************************************************************
2^26 =0X0400 0000 = 64MB,每个 BANK 有4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

选择BANK1-BORSRAM4 连接 TFT，地址范围为0X6C00 0000 ~ 0X6FFF FFFF
FSMC_A6 接LI9341的DC(寄存器/数据选择)脚
寄存器基地址 = 0X6C00 0000
RAM基地址 = 0X6D00 0000 = 0X6C00 0000+2^6*2 = 0X6C00 0000 + 0X80 = 0X6C00 0080
 *2原因是FSMC在送地址时会自动右移一位，即A6实际是A7  2`7=OX80
当选择不同的地址线时，地址要重新计算  
****************************************************************************************/    
#define FSMC_LI9341_CMD              ((uint32_t)0x6C00007E)          //FSMC_Bank1_NORSRAM用于LI9341命令操作的地址
#define FSMC_LI9341_DATA             ((uint32_t)0x6C000080)      		//FSMC_Bank1_NORSRAM用于LI9341数据操作的地址   
//////////////////////////////////////////////////////////////////////////////////
	 
//画笔颜色
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
#define BROWN 			 		0XBC40 //棕色
#define BRRED 			 		0XFC07 //棕红色
#define GRAY  			 		0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 		0X01CF	//深蓝色
#define LIGHTBLUE      	 		0X7D7C	//浅蓝色  
#define GRAYBLUE       	 		0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 		0X841F //浅绿色
//#define LIGHTGRAY        		0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 		 	0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        			0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           			0X2B12 //浅棕蓝色(选择条目的反色)
	    															  
void LI9341_Init(void);													   				//初始化
uint16_t LI9341_ReadID(void);
void LI9341_DisplayOn(void);																//开显示
void LI9341_DisplayOff(void);																//关显示
void LI9341_Clear(uint16_t Color);	 												//清屏
void LI9341_SetCursor(uint16_t Xpos, uint16_t Ypos);				//设置光标

void LI9341_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor);
void LI9341_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t _usColor);							//画线
void LI9341_DrawRectangle(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);		//画矩形
void LI9341_FillRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);
uint16_t LCD_BGR2RGB(uint16_t c);
void LI9341_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);												//显示一个字符
void LI9341_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);  												//显示一个数字
void LI9341_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);						//显示 数字
void LI9341_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);			//显示一个字符串,12/16字体
void LI9341_WriteReg(uint16_t LI9341_Reg, uint16_t LI9341_RegValue);
uint16_t LI9341_ReadReg(uint16_t LI9341_Reg);
void LI9341_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr);
void Picture_Draw(uint16_t S_x,uint16_t S_y,const unsigned char *pic);
//GUI接口函数
void LI9341_PutPixel(uint16_t x,uint16_t y,uint16_t color);								//快速画点
uint16_t  LI9341_GetPixel(uint16_t x,uint16_t y); 													//读点 
void LI9341_DrawHLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usColor);
void LI9341_DrawVLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usY2 , uint16_t _usColor);
void LI9341_DrawHColorLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, const uint16_t *_pColor);
void LI9341_DrawHTransLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, const uint16_t *_pColor);
uint16_t LCD_GetHeight(void);
uint16_t LCD_GetWidth(void);

void Show_Str1(uint16_t x,uint16_t y,uint8_t*str,uint8_t size);
#endif  

