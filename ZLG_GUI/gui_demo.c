#include "Sys_Control.h"
/****************************************************************************
* 名称DemoLine()
* 功能绘画直线演示
* 入口参数无
* 出口参数无
****************************************************************************/
void DemoLine(void)
{ 
	WINDOWS demow;
	/* 显示演示窗口 */
	demow.x = 0;
	demow.y = 0;
	demow.with = 127;
	demow.hight = 63;
	demow.title = (uint8 *) "Line for Demo";
//	demow.state = (uint8 *) "Enter a key return.";
	GUI_WindowsDraw(&demow);
	/* 打钩 */
//	GUI_Line(90, 60, 110, 90, 1);
//	GUI_Line(110, 90, 190, 50, 1);
	GUI_LineWith(2, 20, 20, 20, 3, 1);
//	GUI_LineWith(70, 90, 170, 40, 3, 1);
//	/* 画垂直线及水平线 */
//	GUI_RLine(70, 45, 70, 1);
//	GUI_HLine(140, 85, 180, 1);
//	WaitAKey(); // 等待一按键
//	GUI_WindowsHide(&demow);
}
/****************************************************************************
* 名称RunDemo()
* 功能进行GUI 的演示包括画线画圆圆弧椭圆矩形多边形bmp 图及
* 汉字动画等
* 入口参数无
* 出口参数无
****************************************************************************/
//void RunDemo(void)
//{ 	
//	WINDOWS mainwindows;
//	MENUICO mainmenu[8];
//	uint8 select; // 菜单选项变量
//	uint8 key;
//	uint8 i;
//	/* 设置主窗口并显示输出 */
//	mainwindows.x = 0;
//	mainwindows.y = 0;
//	mainwindows.with = 320;
//	mainwindows.hight = 240;
//	mainwindows.title = (uint8 *) "Graphics Function Demo!";
//	mainwindows.state = NULL;
//	GUI_WindowsDraw(&mainwindows); // 绘制主窗口
//	/* 设置图标菜单 */
//	for(i=0; i<8; i++)
//	{ 	if(i<4)
//		{ mainmenu[i].x = i*59+11;
//			mainmenu[i].y = 20;
//		}
//		else
//		{  mainmenu[i].x = (i-4)*59+11;
//			mainmenu[i].y = 72;
//		}
//		mainmenu[i].state = 0; // 初始化为末选中状态
//	}
//	/* 连接相应的图标数据 */
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
//	/* 连接菜单功能函数 */
//	mainmenu[0].Function = (void(*)())DemoLine;
//	mainmenu[1].Function = (void(*)())DemoCircle;
//	mainmenu[2].Function = (void(*)())DemoArc;
//	mainmenu[3].Function = (void(*)())DemoEllipse;
//	mainmenu[4].Function = (void(*)())DemoRectang;
//	mainmenu[5].Function = (void(*)())DemoPoly;
//	mainmenu[6].Function = (void(*)())DemoBmp;
//	mainmenu[7].Function = (void(*)())MoveCircle;
//	/* 设置默认菜单 */
//	mainmenu[0].state = 1;
//	select = 0;
//	/* 进行主菜单显示及选择操作 */
//	while(1)
//	{ 
//		for(i=0; i<8; i++) // 显示图标菜单
//		{ 
//			GUI_MenuIcoDraw(&mainmenu[i]);
//		}
//		/* 选择功能 */
//		while(1)
//		{  key = WaitKey();
//			if(key==KEY_OK) break; // 点击OK 键选择
//			if(key==KEY_NEXT)
//			{ mainmenu[select].state = 0; // 取消上一选择
//			GUI_MenuIcoDraw(&mainmenu[select]);
//			select++; // 指向下一菜单
//			if(select>7) select=0;
//			mainmenu[select].state = 1;
//			GUI_MenuIcoDraw(&mainmenu[select]);
//			}
//			if(key==KEY_BACK)
//			{ mainmenu[select].state = 0; // 取消上一选择
//			GUI_MenuIcoDraw(&mainmenu[select]);
//			if(select==0) select=7;
//			else select--; // 指向下一菜单
//			mainmenu[select].state = 1;
//			GUI_MenuIcoDraw(&mainmenu[select]);
//			}
//		}
//		/* 执行相应功能 */
//		(*mainmenu[select].Function)();
//		if(select>=6) GUI_WindowsDraw(&mainwindows); // 主窗口重绘(清屏)
//	}
//}
