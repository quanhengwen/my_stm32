#ifndef _WEATHER_H_
#define _WEATHER_H_
#include "main.h"

#define WEATHER_SERVERIP  "api.yytianqi.com"
#define WEATHER_PORTNUM    "80"
#define WEATHER_GETPATH   "/observe?city=CH110101&key=ga86n1t301r2cvsv"  //��ȡ���ļ�·��

u8 Connect_weather_server(void);  /*���ӵ�����������*/

void weather_Ask(void); 			/*��������������*/

u8 parse_now_weather(void);

#endif
