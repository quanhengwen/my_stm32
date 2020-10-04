/****************************************************************************************
* �ļ�����WINDOW.C
* ���ܣ����ڹ��������и��ִ��ڵĸ��Ƽ������ڲ����ơ�
* ���ߣ����ܱ�
* ���ڣ�2004.02.26
* ��ע��2020.04.10���д������GUI_WindowsDraw(WINDOWS *win)����
****************************************************************************************/
#include "config.h"

#if  GUI_WINDOW_EN==1
/****************************************************************************
* ���ƣ�GUI_WindowsDraw()
* ���ܣ���ʾ���ڡ������ṩ�Ĵ��ڲ������л����ڡ�
* ��ڲ�����win		���ھ��
* ���ڲ���������0��ʾ����ʧ�ܣ�����1��ʾ�����ɹ�
****************************************************************************/
uint8  GUI_WindowsDraw(WINDOWS *win)
{  
	 uint8  *str;
   int32  bak, bak1;  
   /* �������ˣ������������Χ���򷵻�0 */
   if( ( (win->with)<20 ) || ( (win->hight)<20 ) ) return(0);		// ��ȡ��߶ȼ�飬������С����
   if( (win->x + win->with ) > GUI_LCM_XMAX ) return(0);			// ���ڿ���Ƿ����
   if( (win->y + win->hight ) > GUI_LCM_YMAX ) return(0);			// ���ڸ߶��Ƿ����
    
   /* ��ʼ������ */
		GUI_RectangleFill(win->x, win->y, win->x + win->with - 1, win->y + win->hight - 1, back_color);
		GUI_Rectangle(win->x, win->y, win->x + win->with - 1, win->y + win->hight - 1, disp_color);	// ������
 		  if(win->sta==1){ 
				GUI_RectangleFill(win->x,win->y,win->x + win->with - 1,win->y + 18,disp_color);
			}else  GUI_HLine(win->x, win->y + 18, win->x + win->with - 1, disp_color);							// ������Ŀ��

   /* д���� */
   if( win->title != NULL ) 
   {  str = win->title;
		    bak = win->x + 1;		
        bak1 = win->y + 1;	

		  if(win->sta==1) {
				GUI_ExchangeColor();									// ��ʾɫ�뱳��ɫ����
				GUI_PutHZ(bak, bak1, str,16); 
				GUI_ExchangeColor();									// ��ʾɫ�뱳��ɫ����	
			}else  GUI_PutHZ(bak, bak1, str,16);  	 
   }


   /* д״̬�� */
   if( win->state != NULL )
   {  if( win->hight < 60) return(0);								// �ж��Ƿ���Ի�״̬��
      GUI_HLine(win->x, win->y + win->hight - 18, win->x + win->with - 1, disp_color);
       
      str = win->state;
      bak = win->x + 4;
      bak1 = win->y + win->hight - 16;		 
			GUI_PutHZ(bak, bak1, str,16);         
   }
   
   return(1);

}


/****************************************************************************
* ���ƣ�GUI_WindowsHide()
* ���ܣ��������ڡ�
* ��ڲ�����win		���ھ��
* ���ڲ���������0��ʾ����ʧ�ܣ�����1��ʾ�����ɹ�
****************************************************************************/
uint8  GUI_WindowsHide(WINDOWS *win)
{  /* �������ˣ������������Χ���򷵻�0 */
   if( ( (win->with)<20 ) || ( (win->hight)<20 ) ) return(0);		// ��ȡ��߶ȼ�飬������С����
   if( (win->x + win->with ) > GUI_LCM_XMAX ) return(0);			// ���ڿ���Ƿ����
   if( (win->y + win->hight ) > GUI_LCM_YMAX ) return(0);			// ���ڸ߶��Ƿ����
    
   /* �������� */
   GUI_RectangleFill(win->x, win->y, win->x + win->with - 1, win->y + win->hight - 1, back_color);
   return(1);
}

/****************************************************************************
* ���ƣ�GUI_WindowsClr()
* ���ܣ��������ڡ�
* ��ڲ�����win		���ھ��
* ���ڲ�������
* ˵����ʹ�ô˺���ǰҪ��ʹ��GUI_WindowsDraw()�����ڻ�����
****************************************************************************/
void  GUI_WindowsClr(WINDOWS *win)
{  uint8  x0, y0;
   uint8  x1, y1;
 
   /* ����Ҫ���������� */
   x0 = win->x + 1;
   x1 = win->x + win->with - 2;
   y0 = win->y + 13;
   y1 = win->y + win->hight - 2;
   if( win->state != NULL )			// �ж��Ƿ���״̬��				
   {  if( win->hight >= 60)
      {  y1 = win->y + win->hight - 12;
      }
   }
   /* ʹ��������ʵ������ */
   GUI_RectangleFill(x0, y0, x1, y1, back_color);
}
#endif

