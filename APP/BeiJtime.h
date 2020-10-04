#ifndef _BEIJTIME_H_
#define _BEIJTIME_H_
#include "main.h"

#define BEIJTIME_SERVERIP  "api.k780.com"
#define BEIJTIME_PORTNUM   "80"
#define BEIJTIME_GETPATH   "/?app=life.time&appkey=54450&sign=f219412bba73b1831413e674b3db724a&format=json"  //获取的文件路径

u8 Connect_BeiJtime_server(void);  	/*连接到北京时间服务器*/

void BeiJtime_Ask(void); 			/*北京时间数据请求函数*/

u8 parse_now_BeiJtime(void);

#endif
