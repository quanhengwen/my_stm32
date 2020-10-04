/****************************************************************************************
* �ļ�����MILTIMENU.C
* ���ܣ��˵���ʾ������(ͼ���ѡ�˵�)��
* ���ߣ�Mr_Q
* ���ڣ�2020.04.08
* ��ע���˳��򹹼ܿ������������˵���ʾ���ݼ���Ӧִ�г��򣬿�����ɲ˵���Ĺ�����
****************************************************************************************/
#include "miltimenu.h"  
#include "fontupd.h" 
#include "config.h" 

/*�˵�У��ͼ�� ����32x32*/
const unsigned char Zero[]={0x00,0x0F,0xF0,0x00,0x00,0x78,0x1E,0x00,0x01,0xC0,0x03,0x80,0x03,0x00,0x00,0xC0,
0x04,0x00,0x00,0x20,0x08,0x00,0x00,0x10,0x10,0x00,0x00,0x08,0x37,0x9E,0x78,0xCC,
0x20,0x90,0x49,0x24,0x61,0x10,0x49,0x26,0x43,0x1E,0x71,0x22,0x42,0x10,0x49,0x22,
0xC4,0x10,0x49,0x23,0xC7,0x9E,0x48,0xC3,0xC0,0x00,0x00,0x03,0xC0,0x00,0x00,0x03,
0xC0,0x07,0xE0,0x03,0xC0,0x0F,0xF0,0x03,0xFF,0x9C,0x39,0xFF,0xFF,0x98,0x19,0xFF,
0x7F,0x98,0x19,0xFE,0x7F,0x98,0x19,0xFE,0x60,0x18,0x18,0x0E,0x30,0x18,0x18,0x0C,
0x30,0x18,0x18,0x1C,0x18,0x18,0x18,0x18,0x0C,0x1C,0x38,0x30,0x04,0x0F,0xF0,0x20,
0x03,0x07,0xE0,0xC0,0x01,0xC0,0x03,0x80,0x00,0x78,0x1E,0x00,0x00,0x0F,0xF0,0x00,};
/*�˵��ָ���������ͼ�� ����32x32*/
const unsigned char Recover[]={0x00,0x00,0x00,0x00,0x00,0x1F,0xF8,0x00,0x00,0x70,0x0E,0x00,0x01,0xC0,0x03,0x80,
0x03,0x00,0x00,0xC0,0x06,0x00,0x00,0x60,0x0C,0x00,0x00,0x30,0x18,0x00,0x40,0x18,
0x10,0x00,0xC0,0x08,0x30,0x01,0x80,0x0C,0x20,0x03,0xE0,0x04,0x60,0x01,0xB0,0x06,
0x40,0x00,0xD8,0x02,0x40,0x00,0x4C,0x02,0x40,0x00,0x04,0x02,0x40,0x20,0x04,0x02,
0x40,0x20,0x04,0x02,0x40,0x20,0x0C,0x02,0x40,0x30,0x08,0x02,0x40,0x18,0x18,0x02,
0x60,0x0E,0x70,0x06,0x20,0x07,0xC0,0x04,0x30,0x00,0x00,0x0C,0x10,0x00,0x00,0x08,
0x18,0x00,0x00,0x18,0x0C,0x00,0x00,0x30,0x06,0x00,0x00,0x60,0x03,0x00,0x00,0xC0,
0x01,0xC0,0x03,0x80,0x00,0x70,0x0E,0x00,0x00,0x1F,0xF8,0x00,0x00,0x00,0x00,0x00,};
/*�˵��궨ͼ�� ����32x32*/
const unsigned char Calibra[]={0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x38,0x00,0x80,0x1C,0x20,0x00,0x80,0x04,
0x20,0x00,0x80,0x04,0x00,0x1F,0xF8,0x00,0x00,0x30,0x8E,0x00,0x00,0xC0,0x87,0x00,
0x01,0x80,0x81,0x80,0x01,0x00,0x80,0x80,0x03,0x00,0x80,0xC0,0x02,0x00,0x00,0x40,
0x06,0x00,0x00,0x60,0x04,0x00,0x00,0x20,0x04,0x01,0x80,0x20,0x04,0x02,0x40,0x20,
0x7F,0xC2,0x47,0xFE,0x04,0x01,0x80,0x20,0x04,0x00,0x00,0x20,0x06,0x00,0x00,0x60,
0x02,0x00,0x00,0x40,0x03,0x00,0x80,0xC0,0x01,0x00,0x80,0x80,0x01,0x80,0x81,0x80,
0x00,0xC0,0x87,0x00,0x00,0x30,0x8E,0x00,0x00,0x1F,0xF8,0x00,0x20,0x00,0x80,0x04,
0x20,0x00,0x80,0x04,0x38,0x00,0x80,0x1C,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00};
/*�˵�����ͼ�� ����32x32*/
const unsigned char Alarm[]={0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xF0,0x0F,0xFF,0xFF,0xF0,0x08,0x00,0x00,0x10,
0x08,0x00,0x00,0x10,0x0C,0x00,0x00,0x30,0x0F,0xFF,0xFF,0xF0,0x0C,0x00,0x00,0x30,
0x08,0x00,0x00,0x10,0x08,0x00,0x00,0x10,0x08,0x00,0x00,0x10,0x08,0x00,0x00,0x10,
0x08,0x00,0x00,0x10,0x0C,0x03,0xC0,0x30,0x0F,0xFF,0xFF,0xF0,0x0F,0xFE,0x7F,0xF0,
0x0F,0xFE,0x7F,0xF0,0x08,0x00,0x00,0x10,0x0C,0x00,0x00,0x30,0x0C,0x0F,0xF0,0x30,
0x06,0x1C,0x38,0x60,0x03,0x70,0x0E,0xC0,0x07,0xF6,0x6F,0xE0,0x3D,0xE6,0x67,0xBC,
0x38,0xE0,0x07,0x1C,0x3C,0x66,0x66,0x3C,0x0C,0xF6,0x6F,0x30,0x07,0xF0,0x0F,0xE0,
0x03,0x9C,0x39,0xC0,0x01,0x0F,0xF0,0x80,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,};
/*�˵���ҳͼ�� ����32x32*/
const unsigned char HomePage[]={0x00,0x01,0x80,0x00,0x00,0x07,0xE0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x3F,0xFC,0x00,
0x00,0xE0,0x07,0x00,0x01,0xC0,0x03,0x80,0x03,0x80,0x01,0xC0,0x07,0x00,0x00,0xE0,
0x0E,0x00,0x00,0x70,0x1C,0x00,0x00,0x38,0x38,0x00,0x00,0x1C,0x30,0x00,0x00,0x0C,
0x30,0x00,0x00,0x0C,0x30,0x00,0x00,0x0C,0x30,0x00,0x00,0x0C,0x30,0x00,0x00,0x0C,
0x30,0x1F,0xF8,0x0C,0x30,0x3F,0xFC,0x0C,0x30,0x30,0x0C,0x0C,0x30,0x30,0x0C,0x0C,
0x30,0x30,0x0C,0x0C,0x30,0x30,0x0C,0x0C,0x30,0x30,0x0C,0x0C,0x30,0x30,0x0C,0x0C,
0x30,0x30,0x0C,0x0C,0x30,0x30,0x0C,0x0C,0x30,0x30,0x0C,0x0C,0x30,0x30,0x0C,0x0C,
0x3F,0xFF,0xFF,0xFC,0x3F,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};
/*�˵�ʱ������ͼ�� ����32x32*/
const unsigned char Calendar[]={0x08,0x00,0x04,0x00,0x0C,0x00,0x04,0x00,0x0C,0x00,0x04,0x00,0x0C,0x00,0x04,0x00,
0x6D,0xFF,0xE5,0x80,0xE1,0xFF,0xE5,0xC0,0xF3,0xFF,0xF1,0xC0,0xFF,0xFF,0xFF,0xC0,
0xFF,0xFF,0xFF,0xC0,0xFF,0xFF,0xFF,0xC0,0xC0,0x00,0x00,0xC0,0xC0,0x00,0x00,0xC0,
0xCF,0x38,0x70,0xC0,0xCF,0x38,0x70,0x00,0xCF,0x38,0x70,0x00,0xC0,0x00,0x00,0x00,
0xC0,0x00,0x00,0x00,0xC0,0x00,0x07,0xF0,0xC0,0x00,0x1E,0x38,0xCF,0x38,0x38,0x0C,
0xCF,0x38,0x30,0x06,0xCF,0x38,0x61,0x02,0xC0,0x00,0x61,0x03,0xC0,0x00,0x41,0x03,
0xFF,0xF8,0x41,0xE3,0x7F,0xF8,0x41,0xF3,0x00,0x00,0x60,0x03,0x00,0x00,0x60,0x06,
0x00,0x00,0x30,0x0E,0x00,0x00,0x1C,0x1C,0x00,0x00,0x0F,0xF0,0x00,0x00,0x03,0xE0,};

const unsigned char Info[]={0x00,0x00,0x00,0x00,0x07,0xFF,0xFF,0xE0,0x1F,0xFF,0xFF,0xF8,0x3F,0xFF,0xFF,0xFC,
0x30,0x00,0x00,0x0C,0x30,0x00,0x00,0x0C,0x70,0x00,0x00,0x0E,0x70,0x00,0x00,0x0E,
0x70,0x01,0x80,0x0E,0x70,0x01,0x80,0x0E,0x70,0x00,0x00,0x0E,0x70,0x00,0x00,0x0E,
0x70,0x01,0x80,0x0E,0x70,0x01,0x80,0x0E,0x70,0x01,0x80,0x0E,0x70,0x01,0x80,0x04,
0x70,0x01,0x80,0x04,0x70,0x01,0x80,0x00,0x70,0x01,0x80,0x00,0x70,0x01,0x80,0x00,
0x70,0x00,0x00,0x04,0x70,0x00,0x00,0x0C,0x30,0x00,0x00,0x0C,0x38,0x00,0x00,0x1C,
0x1F,0xF8,0x1F,0xF8,0x0F,0xFC,0x3F,0xF0,0x00,0x0C,0x30,0x00,0x00,0x0E,0x70,0x00,
0x00,0x07,0xE0,0x00,0x00,0x03,0xC0,0x00,0x00,0x01,0x80,0x00,0x00,0x00,0x00,0x00,};

const unsigned char Expand[]={0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x04,0x80,0x00,0x00,0x3C,0xF0,
0x00,0x00,0x40,0x08,0x00,0x00,0x47,0x88,0x00,0x00,0x48,0x48,0x31,0xFF,0xD7,0xAC,
0x72,0x00,0x14,0xA2,0x32,0xFC,0x14,0xA2,0x22,0xFE,0xD7,0xAC,0x22,0xFC,0x48,0x48,
0x22,0x00,0x47,0x88,0x21,0xFF,0xC0,0x08,0x20,0x80,0x3C,0xF0,0x20,0x80,0x3C,0x80,
0x23,0x00,0x00,0xC0,0x3E,0x7C,0x02,0x40,0x02,0xFE,0x07,0x40,0x1E,0xFE,0x07,0x40,
0x32,0x00,0x00,0x40,0x23,0xFF,0xFF,0xC0,0x20,0x80,0x01,0x00,0x20,0x80,0x01,0x00,
0x21,0xFF,0xFF,0x80,0x22,0x00,0x00,0x40,0x22,0xFE,0x03,0x40,0x22,0xFE,0x07,0x40,
0x32,0xFC,0x02,0x40,0x33,0x00,0x00,0xC0,0x01,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,};

//private typedef
typedef void (*tMenuFunc)(void);     //����ָ�룬�������ú������������Ĳ���
typedef struct sMenuItem *tMenuItem; //ָ���Ӳ˵��ṹ���ָ��
typedef struct sMenu *tMenu;     	   //ָ�����˵��ṹ���ָ��

/*private define*/
#define  MAX_MENU_LEVELS 4   //�������˵�����Ϊ4

#define countof(a)  (sizeof(a) / sizeof(*(a)))
	
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 MenuItemIndex = 0,nMenuLevel =0,nMenuPage=0; //����˵���Ŀ�����š��˵���������
u8 ItemNumb[MAX_MENU_LEVELS];        //��ǰ�˵�ѡ��洢����
u8 menu_sta=0;											 //�˵��Ƿ�ʹ��ͼ��  0�ޣ�1��	
tMenuItem psMenuItem, psCurrentMenuItem;
tMenu psPrevMenu[MAX_MENU_LEVELS];   			//��һ���˵�
tMenu psCurrentMenu;    			 			//��ǰ�˵��ṹ������

/*�˵�֧��ͼ��ģʽ��ʹ��ʱ���˵�Smenu��ӦSmenuItem�Ӳ˵�����ȫ����ͼƬ��ַ����ȫΪ���ָ�ʽ�����ñس���*/
struct sMenuItem
{
	u8* pszTitle;   		 				//�˵���Ŀ����
	
	tMenuFunc pfMenuFunc;				//��ǰѡ���Ӧ���ܳ���
	tMenuFunc pfUpDownMenuFunc;			//�˳���ǰ�����Ӧ���ܳ���
	tMenu psSubMenu;   					//�Ӳ˵�
	
	u8* bmp;			  						//�˵���Ӧͼ���ַ32x32�ֱ���	
};

struct sMenu
{
	u8* pszTitle;			  				//�˵�������
	tMenuItem psItems;					//�˵���Ӧѡ��ṹ��
	u8 nItems;									//�˵���Ӧѡ����Ŀ��
};

void IdleFunc(void)
{
	/*Nothing to execute:return*/
	return;	
}

//void Menu_KeyAnction(u8 key)
//{
//	if(key==KEY_S1_PRES_SHORT) 				DownFunc();
//	else if(key==KEY_S2_PRES_SHORT) 		SelFunc();
//}

/*------------------------------ 4���˵� -------------------------------*/


/*------------------------------ 3���˵� -------------------------------*/
struct sMenuItem RestoreMenuItems[] = {{" 1.����         ", IdleFunc, IdleFunc},
                                       {" 3.����         ", ReturnFunc, IdleFunc},};

struct sMenu RestoreMenu =  {"     �� �� �� ��     ", RestoreMenuItems, countof(RestoreMenuItems)};

struct sMenuItem SpecialMenuItems[] = {	{" 1.��ʱ�洢     ", IdleFunc, IdleFunc},
                                        {" 2.��������     ", IdleFunc, IdleFunc},
                                        {" 3.����         ", ReturnFunc, IdleFunc},};

struct sMenu SpecialMenu =  {"     �� չ �� ��     ", SpecialMenuItems, countof(SpecialMenuItems)};


struct sMenuItem CommMenuItems[] = {		{" 1.����ģʽ     ", IdleFunc, IdleFunc},
                                        {" 2.���ڵ���     ", IdleFunc, IdleFunc},
                                        {" 3.����         ", ReturnFunc, IdleFunc},};
struct sMenu CommMenu = 		{"     ͨ Ѷ ģ ʽ     ", CommMenuItems, countof(CommMenuItems)};


struct sMenuItem CalendarMenuItems[] = {{" 1.ʱ������     ", IdleFunc, IdleFunc},
                                        {" 2.��������     ", IdleFunc, IdleFunc},
                                        {" 3.����         ", ReturnFunc, IdleFunc},};
struct sMenu CalendarMenu = {"     ʱ �� �� ��     ", CalendarMenuItems, countof(CalendarMenuItems)};


struct sMenuItem AlarmMenuItems[] = {	{" 1.CO2����      ", IdleFunc, IdleFunc},
                                        {" 2.CH4����      ", IdleFunc, IdleFunc},
                                        {" 3.����         ", ReturnFunc, IdleFunc},};													
struct sMenu AlarmMenu = 	 	{"     �� �� �� ��     ", AlarmMenuItems, countof(AlarmMenuItems)};

struct sMenuItem CalibraMenuItems[] = {	{" 1.CO2�궨      ", IdleFunc, IdleFunc},
                                        {" 2.CH4�궨      ", IdleFunc, IdleFunc},
                                        {" 3.����         ", ReturnFunc, IdleFunc},};														
struct sMenu CalibraMenu = {"     �� �� �� ��     ", CalibraMenuItems, countof(CalibraMenuItems)};

struct sMenuItem ZeroMenuItems[] = {	{" 1.CO2У��      ", IdleFunc, IdleFunc},
                                        {" 2.CH4У��      ", IdleFunc, IdleFunc},
                                        {" 3.����         ", ReturnFunc, IdleFunc},};													
struct sMenu ZeroMenu =  {"     �� �� У ��     ", ZeroMenuItems, countof(ZeroMenuItems)};

///*------------------------------ 2���˵� -------------------------------*/
struct sMenuItem MainMenuItems[] = {				//���˵���Ŀ
	{".�豸У�� ", IdleFunc,  IdleFunc, &ZeroMenu,		  (u8*)Zero},
	{".�豸�궨 ", IdleFunc,  IdleFunc, &CalibraMenu,  (u8*)Calibra},	
	{".�������� ", IdleFunc,  IdleFunc, &AlarmMenu,    (u8*)Alarm},
//	{".ͨѶģʽ ", IdleFunc,  IdleFunc, &CommMenu,     (u8*)PC},	
	{".��չ���� ", IdleFunc,  IdleFunc, &SpecialMenu,   (u8*)Expand},
	{".ʱ������ ", IdleFunc,  IdleFunc, &CalendarMenu, (u8*)Calendar},	
	{".�豸��Ϣ ", IdleFunc, IdleFunc, NULL,				   (u8*)Info},
	{".�ָ����� ", IdleFunc,  IdleFunc, &RestoreMenu,	 (u8*)Recover},
	{".������ҳ ", ReturnFunc,IdleFunc, NULL,				   (u8*)HomePage}};
													
struct sMenu MainMenu =  {"     �� �� �� ��     ", MainMenuItems, countof(MainMenuItems)};

/*------------------------------ Menu level 1 -------------------------------*/
struct sMenuItem SeletMenuItems[] = {				//���˵���Ŀ
	{" 1.�豸����     ", EnterMenu, IdleFunc },
	{" 2.������ҳ     ", IdleFunc, IdleFunc}};													
struct sMenu SeletMenu = {"     �� �� ѡ ��     ", SeletMenuItems, countof(SeletMenuItems)};

/*******************************************************************************
* Function Name  : Menu_Init							//�˵���ʼ��
* Description    : Initializes the navigation menu.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Menu_Init(void)   //�˵���ʼ��
{
	psCurrentMenu = &SeletMenu;							//��ǰ�˵���ֵ
	psPrevMenu[nMenuLevel] = psCurrentMenu; 			//��һ���˵���ֵ
	psMenuItem = SeletMenuItems;						//��ǰ�˵���Ŀ
}

/*******************************************************************************
* Function Name  : DisplayMenu					//�˵���ʾ����
* Description    : Displays the current menu.   ��ʾ��ǰ�˵�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DisplayMenu(void)
{
		u32 Line = 0, index = 0;

		tMenuItem psMenuItem2;
	
		GUI_ClearSCR();     //����

		GUI_PutHZ(80,Line+2, psCurrentMenu->pszTitle,FONT16);   //��ʾ�˵�����

		Line += 26;																					 		//��������

		if(nMenuPage<(psCurrentMenu->nItems/MAX_PAGE_LINES))
		{
				while(!(index >= MAX_PAGE_LINES))       //������ʾ��ǰ�˵���Ŀ	
				{
					psMenuItem2 = &(psCurrentMenu->psItems[(nMenuPage*MAX_PAGE_LINES)+index]);
					if(psMenuItem2->bmp != NULL)
					{
						menu_sta = 1;
						GUI_PutHZ(120,Line, psMenuItem2->pszTitle,FONT24); 
						index++;
						Line += 26;
					}
					else{
							//��ֵ��ǰ��Ŀ���Ƶ�
						menu_sta = 0;
						GUI_PutHZ(0,Line, psMenuItem2->pszTitle,FONT24);    //��ʾ��ǰ��Ŀ���� */
						index++;
						Line += 26;
					}					
				}		
		}else{
			 while(!(index >= ((psCurrentMenu->nItems)%MAX_PAGE_LINES)))       //������ʾ��ǰ�˵���Ŀ	
				{
					psMenuItem2 = &(psCurrentMenu->psItems[(nMenuPage*MAX_PAGE_LINES)+index]);
					if(psMenuItem2->bmp != NULL)
					{
						menu_sta = 1;
						GUI_PutHZ(120,Line, psMenuItem2->pszTitle,FONT24);    //��ʾ��ǰ��Ŀ���� */
						index++;
						Line += 26;
					}
					else{
						menu_sta = 0;
						GUI_PutHZ(0,Line, psMenuItem2->pszTitle,FONT24);    //��ʾ��ǰ��Ŀ���� */
						index++;
						Line += 26;
					}
				}		
		}
	    /* Get the current menu */
	    psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);		//��ȡ��ǰ�˵�
	    GUI_ExchangeColor();										//��ʾɫ�뱳��ɫ����	
		if(menu_sta==1)
		{
		  GUI_LoadPic1(8,24,psMenuItem->bmp,32,32 );
		  GUI_PutHZ(120,((MenuItemIndex%MAX_PAGE_LINES + 1) * 26), psMenuItem->pszTitle,FONT24);  //ͻ����ʾ��ǰѡ����Ŀ
		}else{
		  GUI_PutHZ(0,((MenuItemIndex%MAX_PAGE_LINES + 1) * 26), psMenuItem->pszTitle,FONT24);  //ͻ����ʾ��ǰѡ����Ŀ
		  GUI_PutHZ(166,((MenuItemIndex%MAX_PAGE_LINES + 1) * 26),"�K",FONT24);
		}

		GUI_ExchangeColor();									// ��ʾɫ�뱳��ɫ����
		GUI_Line(1,20,319,20,WHITE);
		GUI_Rectangle(0,0,319,239,WHITE);
		
//		FuncSelt = MenuPage;									/*���ز˵�ѡ��*/
}
/*******************************************************************************
* ��������  : SelFunc()
* ����      :��ȷ�ϰ�������ʱ����Ӧ����
*                  
* ����      : ��
* ���      : ��
* ����      : ��
*******************************************************************************/
void SelFunc(void)
{
	  psCurrentMenuItem = psMenuItem;

	  if(psMenuItem->psSubMenu != '\0')
	  {
			/* Update the current Item by the submenu */
			MenuItemIndex = 0;
			nMenuPage = 0;			//�˵�ҳ������
			psCurrentMenu = psMenuItem->psSubMenu;
			psMenuItem = &(psCurrentMenu->psItems)[MenuItemIndex];
			DisplayMenu();
			nMenuLevel++;
			psPrevMenu[nMenuLevel] = psCurrentMenu;
	  } 
	  psCurrentMenuItem->pfMenuFunc();
}
/*******************************************************************************
* ��������  : UpFunc()
* ����      :�����ϰ�������ʱ����Ӧ����
*                  
* ����      : ��
* ���      : ��
* ����      : ��
*******************************************************************************/
void UpFunc(void)
{
	psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];

	if(menu_sta == 1)	GUI_PutHZ(120,((MenuItemIndex%MAX_PAGE_LINES + 1) * 26), psMenuItem->pszTitle,FONT24);
	else GUI_PutHZ(0,((MenuItemIndex%MAX_PAGE_LINES + 1) * 26), psMenuItem->pszTitle,FONT24);
		
	if(MenuItemIndex > 0)
	{
		MenuItemIndex--;
//		if(nMenuPage!=(MenuItemIndex/MAX_PAGE_LINES))    //�˵���ǰҳ��
//		{
//			nMenuPage=(MenuItemIndex/MAX_PAGE_LINES);		 
//			DisplayMenu();								//����ˢ�²˵�
//		} 
	}
	else
	{
		MenuItemIndex = psCurrentMenu->nItems - 1;
//		nMenuPage=(MenuItemIndex/MAX_PAGE_LINES);
//		DisplayMenu();											//����ˢ�²˵�
	}
	psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
	GUI_ExchangeColor();									// ��ʾɫ�뱳��ɫ����

	if(menu_sta==1)
	{
		GUI_LoadPic1(8,24,psMenuItem->bmp,32,32 );
		GUI_PutHZ(120,((MenuItemIndex%MAX_PAGE_LINES + 1) * 26), psMenuItem->pszTitle,FONT24);  //ͻ����ʾ��ǰѡ����Ŀ
	}else{
		GUI_PutHZ(0,((MenuItemIndex%MAX_PAGE_LINES + 1) * 26), psMenuItem->pszTitle,FONT24);  //ͻ����ʾ��ǰѡ����Ŀ
		GUI_PutHZ(166,((MenuItemIndex%MAX_PAGE_LINES + 1) * 26),"�K",FONT24);
	}
	GUI_ExchangeColor();									// ��ʾɫ�뱳��ɫ����
	GUI_Line(1,20,319,20,WHITE);
	GUI_Rectangle(0,0,319,239,WHITE);
	ItemNumb[nMenuLevel] = MenuItemIndex;
}
/*******************************************************************************
* ��������  : DownFunc()
* ����      :�����°�������ʱ����Ӧ����
*                  
* ����      : ��
* ���      : ��
* ����      : ��
*******************************************************************************/
void DownFunc(void)
{
	  psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];

	  if(menu_sta == 1)	GUI_PutHZ(120,((MenuItemIndex%MAX_PAGE_LINES + 1) * 26), psMenuItem->pszTitle,FONT24);
	  else GUI_PutHZ(0,((MenuItemIndex%MAX_PAGE_LINES + 1) * 26), psMenuItem->pszTitle,FONT24);

	  /* Test on the MenuItemIndex value before incrementing it */
	  if(MenuItemIndex >= ((psCurrentMenu->nItems)-1))
	  {
			 MenuItemIndex = 0;
	//			 nMenuPage=0;
	//			 DisplayMenu();                  		 //����ˢ�²˵�
	  }
	  else
	  {
			 MenuItemIndex++;
	//			 if(nMenuPage!=(MenuItemIndex/MAX_PAGE_LINES))    //�˵���ǰҳ��
	//			 {
	//					nMenuPage=(MenuItemIndex/MAX_PAGE_LINES);		 
	//				 	DisplayMenu();									//����ˢ�²˵�
	//			 } 
	  }
	/* Get the current menu */
	psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
	GUI_ExchangeColor();									// ��ʾɫ�뱳��ɫ����
	if(menu_sta==1)
	{
		GUI_LoadPic1(8,24,psMenuItem->bmp,32,32 );
		GUI_PutHZ(120,((MenuItemIndex%MAX_PAGE_LINES + 1) * 26), psMenuItem->pszTitle,FONT24);  //ͻ����ʾ��ǰѡ����Ŀ
	}else{
		GUI_PutHZ(0,((MenuItemIndex%MAX_PAGE_LINES + 1) * 26), psMenuItem->pszTitle,FONT24);  //ͻ����ʾ��ǰѡ����Ŀ
		GUI_PutHZ(166,((MenuItemIndex%MAX_PAGE_LINES + 1) * 26),"�K",FONT24);
	}
	GUI_ExchangeColor();									// ��ʾɫ�뱳��ɫ����
	GUI_Line(1,20,319,20,WHITE);
	GUI_Rectangle(0,0,319,239,WHITE);
	ItemNumb[nMenuLevel] = MenuItemIndex;
}
/*******************************************************************************
* Function Name  : ReturnFunc
* Description    : This function is executed when the "RETURN" menu is selected.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ReturnFunc(void)
{
	  psMenuItem->pfUpDownMenuFunc();

	  if(nMenuLevel == 0)
	  {
			nMenuLevel++;
	  }
	  psCurrentMenu = psPrevMenu[nMenuLevel-1];
	  psMenuItem = &psCurrentMenu->psItems[0];
	  ItemNumb[nMenuLevel] = 0;
	  MenuItemIndex = 0;
	  nMenuLevel--;

	  if(nMenuLevel != 0)
	  {
			DisplayMenu();
	  }
	  else
	  {	
//			GUI_LoadPic(0,0,(uint8*)MAIN,128,64);
			menu_sta = 0;   //��ʼ���˵�ͼ��״̬����ֹ����
			nMenuPage= 0;		//��ʼ���˵�ҳ������ֹ����
//			FuncSelt=MainPage;
	  }
}

///*******************************************************************************
//* Function Name  : Buffercmp
//* Description    : Compares two buffers.
//* Input          : - pBuffer1, pBuffer2: buffers to be compared.
//*                : - BufferLength: buffer's length
//* Output         : None
//* Return         : 0: pBuffer1 identical to pBuffer2
//*                  1: pBuffer1 differs from pBuffer2
//*******************************************************************************/
//static u8 Buffercmp(u8* pBuffer1, u8* pBuffer2, u16 BufferLength)
//{
//  while(BufferLength--)
//  {
//    if(*pBuffer1 != *pBuffer2)
//    {
//      return 1;
//    }

//    pBuffer1++;
//    pBuffer2++;
//  }

//  return 0;
//}
void EnterMenu(void)
{
		MenuItemIndex = 0;
		nMenuPage = 0;			//�˵�ҳ������
		psCurrentMenu = &MainMenu;
		psMenuItem = &(psCurrentMenu->psItems)[MenuItemIndex];
		DisplayMenu();
		nMenuLevel++;
		psPrevMenu[nMenuLevel] = psCurrentMenu;
//		FuncSelt = MenuPage;
}
/**************************************************
* ���ƣ�Warn_Titel()
* ���ܣ�������ʾ����
* ��ڲ�����uint8 *dat
* ���ڲ�������
* ˵����ϵͳ״̬��ʾ����
***************************************************/
void Warn_Titel(unsigned char *dat)
{
	WINDOWS Warn_Win;
	/* ��ʾ��ʾ���� */
	Warn_Win.x = 25;
	Warn_Win.y = 8;
	Warn_Win.with = 78;
	Warn_Win.hight = 46;
	Warn_Win.title = (unsigned char *)"  WARNING! ";
	Warn_Win.state = NULL;
	Warn_Win.sta = 1;
	/*����������������ֻ�������*/
	GUI_ClearSCR();
	GUI_WindowsDraw(&Warn_Win);
	GUI_PutHZ(34,32,dat,FONT16);
}

/**************************************************
* ���ƣ�Show_Details()
* ���ܣ�������ʾ���ڽ���
* ��ڲ�����uint8 *dat
* ���ڲ�������
* ˵����ϵͳ״̬��ʾ����
***************************************************/
void Show_Details(unsigned char *dat)
{
	WINDOWS Warn_Win;
	/* ��ʾ��ʾ���� */
	Warn_Win.x = 0;
	Warn_Win.y = 0;
	Warn_Win.with = 320;
	Warn_Win.hight = 240;
	Warn_Win.title = (unsigned char *)dat;
	Warn_Win.state = NULL;
	Warn_Win.sta = 1;
	/*����������������ֻ�������*/
	GUI_ClearSCR();
	GUI_WindowsDraw(&Warn_Win);
}


/***************************************End File Mr_Q_2019-02-27**********************************************/

