#ifndef _WEATHER_H_
#define _WEATHER_H_
#include "main.h"

#define WEATHER_SERVERIP  "api.yytianqi.com"
#define WEATHER_PORTNUM    "80"
#define WEATHER_GETPATH   "/observe?city=CH110101&key=ga86n1t301r2cvsv"  //获取的文件路径

u8 Connect_weather_server(void);  /*连接到天气服务器*/

void weather_Ask(void); 			/*天气数据请求函数*/

u8 parse_now_weather(void);

#endif
