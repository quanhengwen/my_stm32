#include "Sys_Control.h"
/****************************************************************************
* ����DemoLine()
* ���ܻ滭ֱ����ʾ
* ��ڲ�����
* ���ڲ�����
****************************************************************************/
void DemoLine(void)
{ 
	WINDOWS demow;
	/* ��ʾ��ʾ���� */
	demow.x = 0;
	demow.y = 0;
	demow.with = 127;
	demow.hight = 63;
	demow.title = (uint8 *) "Line for Demo";
//	demow.state = (uint8 *) "Enter a key return.";
	GUI_WindowsDraw(&demow);
	/* �� */
//	GUI_Line(90, 60, 110, 90, 1);
//	GUI_Line(110, 90, 190, 50, 1);
	GUI_LineWith(2, 20, 20, 20, 3, 1);
//	GUI_LineWith(70, 90, 170, 40, 3, 1);
//	/* ����ֱ�߼�ˮƽ�� */
//	GUI_RLine(70, 45, 70, 1);
//	GUI_HLine(140, 85, 180, 1);
//	WaitAKey(); // �ȴ�һ����
//	GUI_WindowsHide(&demow);
}
/****************************************************************************
* ����RunDemo()
* ���ܽ���GUI ����ʾ�������߻�ԲԲ����Բ���ζ����bmp ͼ��
* ���ֶ�����
* ��ڲ�����
* ���ڲ�����
****************************************************************************/
//void RunDemo(void)
//{ 	
//	WINDOWS mainwindows;
//	MENUICO mainmenu[8];
//	uint8 select; // �˵�ѡ�����
//	uint8 key;
//	uint8 i;
//	/* ���������ڲ���ʾ��� */
//	mainwindows.x = 0;
//	mainwindows.y = 0;
//	mainwindows.with = 320;
//	mainwindows.hight = 240;
//	mainwindows.title = (uint8 *) "Graphics Function Demo!";
//	mainwindows.state = NULL;
//	GUI_WindowsDraw(&mainwindows); // ����������
//	/* ����ͼ��˵� */
//	for(i=0; i<8; i++)
//	{ 	if(i<4)
//		{ mainmenu[i].x = i*59+11;
//			mainmenu[i].y = 20;
//		}
//		else
//		{  mainmenu[i].x = (i-4)*59+11;
//			mainmenu[i].y = 72;
//		}
//		mainmenu[i].state = 0; // ��ʼ��Ϊĩѡ��״̬
//	}
//	/* ������Ӧ��ͼ������ */
//	mainmenu[0].icodat = (uint8 *) menuico1;
//	mainmenu[0].title = (uint8 *) menuchar1;
//	mainmenu[1].icodat = (uint8 *) menuico2;
//	mainmenu[1].title = (uint8 *) menuchar2;
//	mainmenu[2].icodat = (uint8 *) menuico3;
//	mainmenu[2].title = (uint8 *) menuchar3;
//	mainmenu[3].icodat = (uint8 *) menuico4;
//	mainmenu[3].title = (uint8 *) menuchar4;
//	mainmenu[4].icodat = (uint8 *) menuico5;
//	mainmenu[4].title = (uint8 *) menuchar5;
//	mainmenu[5].icodat = (uint8 *) menuico6;
//	mainmenu[5].title = (uint8 *) menuchar6;
//	mainmenu[6].icodat = (uint8 *) menuico7;
//	mainmenu[6].title = (uint8 *) menuchar7;
//	mainmenu[7].icodat = (uint8 *) menuico8;
//	mainmenu[7].title = (uint8 *) menuchar8;
//	/* ���Ӳ˵����ܺ��� */
//	mainmenu[0].Function = (void(*)())DemoLine;
//	mainmenu[1].Function = (void(*)())DemoCircle;
//	mainmenu[2].Function = (void(*)())DemoArc;
//	mainmenu[3].Function = (void(*)())DemoEllipse;
//	mainmenu[4].Function = (void(*)())DemoRectang;
//	mainmenu[5].Function = (void(*)())DemoPoly;
//	mainmenu[6].Function = (void(*)())DemoBmp;
//	mainmenu[7].Function = (void(*)())MoveCircle;
//	/* ����Ĭ�ϲ˵� */
//	mainmenu[0].state = 1;
//	select = 0;
//	/* �������˵���ʾ��ѡ����� */
//	while(1)
//	{ 
//		for(i=0; i<8; i++) // ��ʾͼ��˵�
//		{ 
//			GUI_MenuIcoDraw(&mainmenu[i]);
//		}
//		/* ѡ���� */
//		while(1)
//		{  key = WaitKey();
//			if(key==KEY_OK) break; // ���OK ��ѡ��
//			if(key==KEY_NEXT)
//			{ mainmenu[select].state = 0; // ȡ����һѡ��
//			GUI_MenuIcoDraw(&mainmenu[select]);
//			select++; // ָ����һ�˵�
//			if(select>7) select=0;
//			mainmenu[select].state = 1;
//			GUI_MenuIcoDraw(&mainmenu[select]);
//			}
//			if(key==KEY_BACK)
//			{ mainmenu[select].state = 0; // ȡ����һѡ��
//			GUI_MenuIcoDraw(&mainmenu[select]);
//			if(select==0) select=7;
//			else select--; // ָ����һ�˵�
//			mainmenu[select].state = 1;
//			GUI_MenuIcoDraw(&mainmenu[select]);
//			}
//		}
//		/* ִ����Ӧ���� */
//		(*mainmenu[select].Function)();
//		if(select>=6) GUI_WindowsDraw(&mainwindows); // �������ػ�(����)
//	}
//}
