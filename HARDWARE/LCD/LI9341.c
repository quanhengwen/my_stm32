/*****************************************************************************
  * @file    LI9341.c
  * @author  Mr_Q
  * @version V1.0
  * @date    2019-11-21
  * @brief   通用型多参CD4X测定器主程序(基于FreeRTOS操作系统、STemwin GUI显示界面、
						 FATFS文件系统、USB存储及虚拟串口)测定器，可选气体为13种气体，共计3种
						 组合形式。
	注意事项： 1.传感器校准、标定需连续通电2小时以后再进行操作
  ***************************************************************************/
#include "LI9341.h"
#include "stdlib.h"
#include "font.h" 
#include "delay.h"	 
#include "fatfs_flash_spi.h"
#include  "config.h"

//LI9341的画笔颜色和背景色	   
uint16_t POINT_COLOR = RED;	//画笔颜色
uint16_t BACK_COLOR = 0xFFFF;  //背景色

static void LI9341_SetDispWin(uint16_t Xpos, uint16_t Ypos, uint16_t _usHeight, uint16_t _usWidth);
//管理LI9341重要参数
//默认为竖屏
_LI9341_dev LI9341dev;
	 
/**
  * @brief  向LI9341写入命令
  * @param  usData :要写入的命令
  * @retval 无
  */	
void LI9341_WR_REG(__IO uint16_t regval)
{   
	LCD_RS_CLR;//写地址  
 	LCD_CS_CLR; 
	DATAOUT(regval); 
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET;  
}
/**
  * @brief  向LI9341写入数据
  * @param  usData :要写入的数据
  * @retval 无
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
  * @brief  向LI9341读LI9341数据
  * @param  
  * @retval 读到的值
  */	
uint16_t LI9341_RD_DATA(void)
{
	u16 t;
 	GPIOB->CRL=0X88888888; //PB0-7  上拉输入
	GPIOB->CRH=0X88888888; //PB8-15 上拉输入
	GPIOB->ODR=0X0000;     //全部输出0

	LCD_RS_SET;
	LCD_CS_CLR;
	//读取数据(读寄存器时,并不需要读2次)
	LCD_RD_CLR;
	delay_us(2);				   
	t=DATAIN;  
	LCD_RD_SET;
	LCD_CS_SET; 

	GPIOB->CRL=0X33333333; //PB0-7  上拉输出
	GPIOB->CRH=0X33333333; //PB8-15 上拉输出
	GPIOB->ODR=0XFFFF;    //全部输出高
	return t;  
}					   
/**
  * @brief  向LI9341指定寄存器写数据
  * @param  寄存器地址 要写入的值
  * @retval 
  */	
void LI9341_WriteReg(uint16_t LI9341_Reg,uint16_t LI9341_RegValue)
{	
	LI9341_WR_REG(LI9341_Reg);		
	LI9341_WR_DATA( LI9341_RegValue);//写入数据	    		 
}	   
/**
  * @brief  向LI9341指定寄存器读数据
  * @param  寄存器地址
  * @retval 读到的值
  */	
uint16_t LI9341_ReadReg(uint16_t LI9341_Reg)
{										   
	LI9341_WR_REG(LI9341_Reg);		//写入要读的寄存器序号	  
	return LI9341_RD_DATA();		//返回读到的值
}   

//当mdk -O1时间优化时需要设置
//延时i
void opt_delay(uint8_t i)
{
	while(i--);
}	 
//LI9341开启显示
void LI9341_DisplayOn(void)
{					   
	LI9341_WR_REG(0X29);		//开启显示
}	 
//LI9341关闭显示
void LI9341_DisplayOff(void)
{	   
	LI9341_WR_REG(0X28);		//关闭显示
} 
/*
*********************************************************************************************************
*	函 数 名: LI9341_SetDirection
*	功能说明: 设置显示方向。
*	形    参:  _ucDir : 显示方向代码 0 横屏正常, 1=横屏180度翻转, 2=竖屏, 3=竖屏180度翻转
*	返 回 值: 无
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
		D4   ML  Vertical Refresh Order  LI9341 vertical refresh direction control. 、

		D3   BGR RGB-BGR Order   Color selector switch control
		     (0 = RGB color filter panel, 1 = BGR color filter panel )
		D2   MH  Horizontal Refresh ORDER  LI9341 horizontal refreshing direction control.
		D1   X   Reserved  Reserved
		D0   X   Reserved  Reserved
	*/
	LI9341_WR_REG(0x36);
	/* 0 表示竖屏(排线在下)，1表示竖屏(排线在上), 2表示横屏(排线在左边)  3表示横屏 (排线在右边) */
	if (_ucDir == 0)
	{
		LI9341_WR_DATA(0xA8);	/* 横屏(排线在左边) */
		LI9341dev.height = 240;
		LI9341dev.width = 320;
	}
	else if (_ucDir == 1)
	{
		LI9341_WR_DATA(0x68);	/* 横屏 (排线在右边) */
		LI9341dev.height = 240;
		LI9341dev.width = 320;
	}
	else if (_ucDir == 2)
	{
		LI9341_WR_DATA(0xC8);	/* 竖屏(排线在上) */
		LI9341dev.height = 320;
		LI9341dev.width = 240;
	}
	else if (_ucDir == 3)
	{
		LI9341_WR_DATA(0x00);	/* 竖屏(排线在下) */
		LI9341dev.height = 320;
		LI9341dev.width = 240;
	}
}
/*
*********************************************************************************************************
*	函 数 名: LI9341_SetDispWin
*	功能说明: 设置显示窗口，进入窗口显示模式。TFT驱动芯片支持窗口显示模式，这是和一般的12864点阵LI9341的最大区别。
*	形    参:
*		_usX : 水平坐标
*		_usY : 垂直坐标
*		_usHeight: 窗口高度
*		_usWidth : 窗口宽度
*	返 回 值: 无
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
//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
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
*	函 数 名: LI9341_PutPixel
*	功能说明: 画1个像素
*	形    参:
*			_usX,_usY : 像素坐标
*			_usColor  ：像素颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void LI9341_PutPixel(uint16_t x,uint16_t y,uint16_t color)
{	   
	LI9341_SetCursor(x,y);//设置光标位置
	LI9341_WR_REG(0x2C);
	LI9341_WR_DATA(color);
}	
//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
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
*	函 数 名: LI9341_GetPixel
*	功能说明: 读取1个像素
*	形    参:
*			_usX,_usY : 像素坐标
*			_usColor  ：像素颜色
*	返 回 值: RGB颜色值 （565）
*********************************************************************************************************
*/
uint16_t LI9341_GetPixel(uint16_t x,uint16_t y)
{
 	uint16_t r=0,g=0,b=0;	   
	LI9341_SetCursor(x,y);						/* 设置光标位置 */
	
	LI9341_WR_REG(0X2E);						//9320 发送读GRAM指令 
  
 	r=LI9341_RD_DATA();							/* 第1个哑读，丢弃 */	   
	opt_delay(2);	  
 	r=LI9341_RD_DATA();  		  				//实际坐标颜色
	opt_delay(2);	  
	b=LI9341_RD_DATA(); 
	g=r&0XFF;		//对于9341/5310/5510,第一次读取的是RG的值,R在前,G在后,各占8位
	g<<=8; 
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));//ILI9341/NT35310/LI9341需要公式转换一下
}	
/*
*********************************************************************************************************
*	函 数 名: Init_9488
*	功能说明: 初始化LI9341驱动器
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void LI9341_Init(void)
{ 	 	
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); //使能PORTB,C时钟和AFIO时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//开启SWD，失能JTAG
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;	   ///PORTC6~10复用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //GPIOC	

	GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	//  PORTB推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB
 
	GPIO_SetBits(GPIOB,GPIO_Pin_All); 		 
	delay_ms(50); // 
	LI9341dev.id = LI9341_ReadID();				//读取LI9341版本号	
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
	LI9341_LED_SET;					//点亮背光

	LI9341_Clear(BLACK);
}  
/*
*********************************************************************************************************
*	函 数 名: LI9341_ReadID
*	功能说明: 读取LI9341驱动芯片ID， 4个bit
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t LI9341_ReadID(void)
{
	uint8_t ID[3];
	LI9341_WR_REG(0xD3);
	LI9341_RD_DATA();//空读
	ID[0] = LI9341_RD_DATA();
	ID[1] = LI9341_RD_DATA();
	ID[2] = LI9341_RD_DATA();
	return ((ID[1]<<8) | ID[2]);
}

/*
*********************************************************************************************************
*	函 数 名: LI9341_Clear
*	功能说明: 根据输入的颜色值清屏
*	形    参: _usColor : 背景色
*	返 回 值: 无
*********************************************************************************************************
*/
void LI9341_Clear(uint16_t color)
{
	uint32_t index=0;      
	uint32_t totalpoint;
	LI9341_SetDispWin(0,0,LI9341dev.height,LI9341dev.width);
	LI9341_WR_REG(0x2C);								//开始写入GRAM	
	totalpoint = (LI9341dev.width*LI9341dev.height)/8; 				//得到总点数	
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
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LI9341_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t _usColor)
{
	int32_t dx , dy ;
	int32_t tx , ty ;
	int32_t inc1 , inc2 ;
	int32_t d , iTag ;
	int32_t x , y ;

	/* 采用 Bresenham 算法，在2点间画一条直线 */

	LI9341_PutPixel(x1 , y1 , _usColor);

	/* 如果两点重合，结束后面的动作。*/
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

	if ( dx < dy )   /*如果dy为计长方向，则交换纵横坐标。*/
	{
		uint16_t temp;

		iTag = 1 ;
		temp = x1; x1 = y1; y1 = temp;
		temp = x2; x2 = y2; y2 = temp;
		temp = dx; dx = dy; dy = temp;
	}
	tx = x2 > x1 ? 1 : -1 ;    /* 确定是增1还是减1 */
	ty = y2 > y1 ? 1 : -1 ;
	x = x1 ;
	y = y1 ;
	inc1 = 2 * dy ;
	inc2 = 2 * ( dy - dx );
	d = inc1 - dx ;
	while ( x != x2 )     /* 循环画点 */
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
*	函 数 名: LCD_GetHeight
*	功能说明: 读取LCD分辨率之高度
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t LCD_GetHeight(void)
{
	return LI9341dev.height;
}

/*
*********************************************************************************************************
*	函 数 名: LCD_GetWidth
*	功能说明: 读取LCD分辨率之宽度
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t LCD_GetWidth(void)
{
	return LI9341dev.width;
}

/*
*********************************************************************************************************
*	函 数 名: LI9341_DrawHLine
*	功能说明: 绘制一条水平线 （主要用于UCGUI的接口函数）
*	形    参:  _usX1    ：起始点X坐标
*			  _usY1    ：水平线的Y坐标
*			  _usX2    ：结束点X坐标
*			  _usColor : 颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void LI9341_DrawHLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usColor)
{
	uint16_t i;

	LI9341_SetDispWin(_usX1, _usY1, 1, _usX2 - _usX1 + 1);

	LI9341_WR_REG(0x2C);

	/* 写显存 */
	for (i = 0; i <_usX2-_usX1 + 1; i++)
	{
		LI9341_WR_DATA(_usColor);
	}
}
/*
*********************************************************************************************************
*	函 数 名: LI9341_DrawVLine
*	功能说明: 画垂直平线 用UCGUI的接口函数
*	形    参: X,Y的坐标和颜色
*	返 回 值: 无
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
*	函 数 名: LI9341_DrawHColorLine
*	功能说明: 绘制一条彩色水平线 （主要用于UCGUI的接口函数）
*	形    参:  _usX1    ：起始点X坐标
*			  _usY1    ：水平线的Y坐标
*			  _usWidth ：直线的宽度
*			  _pColor : 颜色缓冲区
*	返 回 值: 无
*********************************************************************************************************
*/
void LI9341_DrawHColorLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, const uint16_t *_pColor)
{
	uint16_t i;
	
	LI9341_SetDispWin(_usX1, _usY1, 1, _usWidth);

	LI9341_WR_REG(0x2C);

	/* 写显存 */
	for (i = 0; i <_usWidth; i++)
	{
		LI9341_WR_DATA(*_pColor++);
	}
}
/*
*********************************************************************************************************
*	函 数 名: LI9341_DrawHTransLine
*	功能说明: 绘制一条彩色透明的水平线 （主要用于UCGUI的接口函数）， 颜色值为0表示透明色
*	形    参:  _usX1    ：起始点X坐标
*			  _usY1    ：水平线的Y坐标
*			  _usWidth ：直线的宽度
*			  _pColor : 颜色缓冲区
*	返 回 值: 无
*********************************************************************************************************
*/
void LI9341_DrawHTransLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, const uint16_t *_pColor)
{
	uint16_t i, j;
	uint16_t Index;

	LI9341_SetCursor(_usX1, _usY1);

	/* 写显存 */
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
*	函 数 名: LI9341_DrawRectangle
*	功能说明: 填充矩形。
*	形    参:
*			_usX,_usY：矩形左上角的坐标
*			_usHeight ：矩形的高度
*			_usWidth  ：矩形的宽度
*	返 回 值: 无
*********************************************************************************************************
*/
void LI9341_DrawRectangle(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor)
{
	LI9341_DrawLine(_usX, _usY, _usX + _usWidth - 1, _usY, _usColor);
	LI9341_DrawLine(_usX, _usY + _usHeight - 1, _usX + _usWidth - 1, _usY + _usHeight - 1, _usColor);
	LI9341_DrawLine(_usX, _usY, _usX, _usY + _usHeight - 1, _usColor);	/* 左 */
	LI9341_DrawLine(_usX + _usWidth - 1, _usY, _usX + _usWidth - 1, _usY + _usHeight, _usColor);	/* 右 */
}
/*
*********************************************************************************************************
*	函 数 名: LI9341_DrawCircle
*	功能说明: 绘制一个圆，笔宽为1个像素
*	形    参:
*			_usX,_usY  ：圆心的坐标
*			_usRadius  ：圆的半径
*	返 回 值: 无
*********************************************************************************************************
*/
void LI9341_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor)
{
	int32_t  D;			/* Decision Variable */
	uint32_t  CurX;		/* 当前 X 值 */
	uint32_t  CurY;		/* 当前 Y 值 */

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
*	函 数 名: LI9341_FillRect
*	功能说明: 填充矩形。
*	形    参:
*			_usX,_usY：矩形左上角的坐标
*			_usHeight ：矩形的高度
*			_usWidth  ：矩形的宽度
*	返 回 值: 无
*********************************************************************************************************
*/
void LI9341_FillRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor)
{
	uint32_t i;

	/*
	 ---------------->---
	|(_usX，_usY)        |
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
								  
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LI9341_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{  							  
  uint8_t temp,t1,t;
	uint16_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 		//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)		LI9341_PutPixel(x,y,disp_color);
			else if(mode==0)	LI9341_PutPixel(x,y,back_color);
			temp<<=1;
			y++;
			if(y>=LI9341dev.height)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=LI9341dev.width)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
}   
//m^n函数
//返回值:m^n次方.
uint32_t LI9341_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
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
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
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
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LI9341_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{         
	uint8_t x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LI9341_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

/*
*********************************************************************************************************
*	函 数 名: LI9341_DrawBMP
*	功能说明: 在LCD上显示一个BMP位图，位图点阵扫描次序：从左到右，从上到下
*	形    参:
*			_usX, _usY : 图片的坐标
*			_usHeight  ：图片高度
*			_usWidth   ：图片宽度
*			_ptr       ：图片点阵指针
*	返 回 值: 无
*********************************************************************************************************
*/
void LI9341_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr)
{
	uint32_t index = 0;
	const uint16_t *p;

	/* 设置图片的位置和大小， 即设置显示窗口 */
	LI9341_SetDispWin(_usX, _usY, _usHeight, _usWidth);
	p = _ptr;
	
	for (index = 0; index < _usHeight * _usWidth; index++)
	{
		LI9341_PutPixel(_usX, _usY, *p++);
	}
	/* 退出窗口绘图模式 */
	LI9341_SetDispWin(0, 0, LI9341dev.height, LI9341dev.width);//退出窗口显示模式，变为全屏显示模式
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
	u8 csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数	 
	qh=*code;
	ql=*(++code);
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//非 常用汉字
	{   		    
	    for(i=0;i<csize;i++)*mat++=0x00;//填充满格
	    return; //结束访问
	}          
	if(ql<0x7f)ql-=0x40;//注意!
	else ql-=0x41;
	qh-=0x81;   
	foffset=((unsigned long)190*qh+ql)*csize;	//得到字库中的字节偏移量  		  
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
//显示一个指定大小的汉字
//x,y :汉字的坐标
//font:汉字GBK码
//size:字体大小
//mode:0,正常显示,1,叠加显示	   
void Show_Font(u16 x,u16 y,u8 *font,u8 size)
{
	u8 temp,t,t1;
	u16 x0=x;
	u8 dzk[72];   
	u8 csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数	 
	if(size!=12&&size!=16&&size!=24)return;	//不支持的size
	Get_HzMat(font,dzk,size);	//得到相应大小的点阵数据 
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];			//得到点阵数据                          
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
//在指定位置开始显示一个字符串	    
//不支持自动换行
//(x,y):起始坐标
//width,height:区域
//str  :字符串
//size :字体大小
//mode:0,非叠加方式;1,叠加方式    	   		   
void Show_Str1(u16 x,u16 y,u8*str,u8 size)
{					
	u16 x0=x;						  	  
    u8 bHz=0;     //字符或者中文  	    				    				  	  
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
			if(*str>0x80)bHz=1;//中文 
			else              //字符
			{         
				if(*str==13)//换行符号
				{         
						y+=size;
						x=x0;
						str++; 
				}  
				else LI9341_ShowChar(x,y,*str,size,0);//有效部分写入 
				str++; 
				x+=size/2; //字符,为全字的一半 
			}
        }
		else//中文 
        {     
			bHz=0;//有汉字库    				     
			Show_Font(x,y,str,size); //显示这个汉字,空心显示 
			str+=2; 
			x+=size;//下一个汉字偏移	    
        }						 
    }   
}  			 		 

