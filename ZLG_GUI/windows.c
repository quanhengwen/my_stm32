/****************************************************************************************
* 文件名：WINDOW.C
* 功能：窗口管理。将进行各种窗口的给制及窗口内部控制。
* 作者：黄绍斌
* 日期：2004.02.26
* 备注：2020.04.10进行代码更改GUI_WindowsDraw(WINDOWS *win)函数
****************************************************************************************/
#include "config.h"

#if  GUI_WINDOW_EN==1
/****************************************************************************
* 名称：GUI_WindowsDraw()
* 功能：显示窗口。根据提供的窗口参数进行画窗口。
* 入口参数：win		窗口句柄
* 出口参数：返回0表示操作失败，返回1表示操作成功
****************************************************************************/
uint8  GUI_WindowsDraw(WINDOWS *win)
{  
	 uint8  *str;
   int32  bak, bak1;  
   /* 参数过滤，若窗口起出范围，则返回0 */
   if( ( (win->with)<20 ) || ( (win->hight)<20 ) ) return(0);		// 宽度、高度检查，限制最小窗口
   if( (win->x + win->with ) > GUI_LCM_XMAX ) return(0);			// 窗口宽度是否溢出
   if( (win->y + win->hight ) > GUI_LCM_YMAX ) return(0);			// 窗口高度是否溢出
    
   /* 开始画窗口 */
		GUI_RectangleFill(win->x, win->y, win->x + win->with - 1, win->y + win->hight - 1, back_color);
		GUI_Rectangle(win->x, win->y, win->x + win->with - 1, win->y + win->hight - 1, disp_color);	// 画窗口
 		  if(win->sta==1){ 
				GUI_RectangleFill(win->x,win->y,win->x + win->with - 1,win->y + 18,disp_color);
			}else  GUI_HLine(win->x, win->y + 18, win->x + win->with - 1, disp_color);							// 画标题目栏

   /* 写标题 */
   if( win->title != NULL ) 
   {  str = win->title;
		    bak = win->x + 1;		
        bak1 = win->y + 1;	

		  if(win->sta==1) {
				GUI_ExchangeColor();									// 显示色与背景色交换
				GUI_PutHZ(bak, bak1, str,16); 
				GUI_ExchangeColor();									// 显示色与背景色交换	
			}else  GUI_PutHZ(bak, bak1, str,16);  	 
   }


   /* 写状态栏 */
   if( win->state != NULL )
   {  if( win->hight < 60) return(0);								// 判断是否可以画状态栏
      GUI_HLine(win->x, win->y + win->hight - 18, win->x + win->with - 1, disp_color);
       
      str = win->state;
      bak = win->x + 4;
      bak1 = win->y + win->hight - 16;		 
			GUI_PutHZ(bak, bak1, str,16);         
   }
   
   return(1);

}


/****************************************************************************
* 名称：GUI_WindowsHide()
* 功能：消隐窗口。
* 入口参数：win		窗口句柄
* 出口参数：返回0表示操作失败，返回1表示操作成功
****************************************************************************/
uint8  GUI_WindowsHide(WINDOWS *win)
{  /* 参数过滤，若窗口起出范围，则返回0 */
   if( ( (win->with)<20 ) || ( (win->hight)<20 ) ) return(0);		// 宽度、高度检查，限制最小窗口
   if( (win->x + win->with ) > GUI_LCM_XMAX ) return(0);			// 窗口宽度是否溢出
   if( (win->y + win->hight ) > GUI_LCM_YMAX ) return(0);			// 窗口高度是否溢出
    
   /* 消隐窗口 */
   GUI_RectangleFill(win->x, win->y, win->x + win->with - 1, win->y + win->hight - 1, back_color);
   return(1);
}

/****************************************************************************
* 名称：GUI_WindowsClr()
* 功能：清屏窗口。
* 入口参数：win		窗口句柄
* 出口参数：无
* 说明：使用此函数前要先使用GUI_WindowsDraw()将窗口画出。
****************************************************************************/
void  GUI_WindowsClr(WINDOWS *win)
{  uint8  x0, y0;
   uint8  x1, y1;
 
   /* 设置要清屏的区域 */
   x0 = win->x + 1;
   x1 = win->x + win->with - 2;
   y0 = win->y + 13;
   y1 = win->y + win->hight - 2;
   if( win->state != NULL )			// 判断是否有状态栏				
   {  if( win->hight >= 60)
      {  y1 = win->y + win->hight - 12;
      }
   }
   /* 使用填充矩形实现清屏 */
   GUI_RectangleFill(x0, y0, x1, y1, back_color);
}
#endif

