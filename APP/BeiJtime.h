#ifndef _BEIJTIME_H_
#define _BEIJTIME_H_
#include "main.h"

#define BEIJTIME_SERVERIP  "api.k780.com"
#define BEIJTIME_PORTNUM   "80"
#define BEIJTIME_GETPATH   "/?app=life.time&appkey=54450&sign=f219412bba73b1831413e674b3db724a&format=json"  //��ȡ���ļ�·��

u8 Connect_BeiJtime_server(void);  	/*���ӵ�����ʱ�������*/

void BeiJtime_Ask(void); 			/*����ʱ������������*/

u8 parse_now_BeiJtime(void);

#endif
