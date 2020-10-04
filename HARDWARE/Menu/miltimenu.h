/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : menu.h
* Author             : MCD Application Team
* Version            : V1.1.2
* Date               : 09/22/2008
* Description        : This file contains all the functions prototypes for the
*                      menu navigation firmware driver.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MILTIMENU_H
#define __MILTIMENU_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define  MAX_MENU_LEVELS 4
#define  NOKEY  0
#define  SEL    1
#define  DOWN   2
#define  UP     3
#define  MAX_PAGE_LINES  8  //菜单页可显示最大条目数

typedef enum {
							MainPage=1,
							PassdWord,
							MenuPage,
							ZeroPage ,
							SpanPage,
							AlarmPage,
							TimePage,
							DatePage,
							CommPage,
							TSavePage,
							FallPage,
							FallCheckPage,
							AboutPage,
							USBStorage,
							RestoreFac,
						} MainMenuPage;
/* Module private variables --------------------------------------------------*/
extern u8 FuncSelt;

/* Exported macro ------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))
extern unsigned char PC[];
/* Private functions ---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Menu_Init(void);
void EnterMenu(void);
void DisplayMenu(void);
void SelFunc(void);
void UpFunc(void);
void DownFunc(void);
void ReturnFunc(void);
void IdleFunc(void);
void Warn_Titel(unsigned char *dat);				/*菜单提示栏*/
void Show_Details(unsigned char *dat);			/*菜单标题界面*/

/*主界面*/
void Screen_MainMenu_Refresh(void);
/**/
void Main_KeyAnction(u8 key);
/**/
void Menu_KeyAnction(u8 key);
/**/
void Passd_KeyAnction(u8 key);

/**/
void Zero_KeyAnction(u8 key);
void CO2ZeroFunc(void); 
void CH4ZeroFunc(void); 
void Zero_Refresh(void);

/**/
void Span_KeyAnction(u8 key);
void CO2SpanFunc(void); 
void CH4SpanFunc(void); 
void Span_Refresh(void);

/**/
void Alarm_KeyAnction(u8 key);
void CO2AlarmFunc(void);
void CH4AlarmFunc(void);

/**/
void TimeSetFunc(void); 
void Time_KeyAnction(u8 key);
void Time_Refresh(void);

/**/
void DateSetFunc(void); 
void Date_KeyAnction(u8 key);
void Date_Refresh(void);

/**/
void Storage_Func(void);
void VirCom_Func(void);
void Storage_KeyAnction(u8 key);

void TimiSFunc(void);
void TimiS_KeyAnction(u8 key);
/**/
void PassdFunc(void);												/*密码功能函数*/
void HelpFunc(void);
/**/
void Restore_FacFunc(void);

void FallCheckSFunc(void);
void FallWarning(void);			/*跌倒报警函数*/
void FallCheck_KeyAnction(u8 key);
void FallWarnKeyAnction(u8 key);
/**/
void AboutFunc(void);
void About_KeyAnction(u8 key);

#endif /* __MENU_H */
