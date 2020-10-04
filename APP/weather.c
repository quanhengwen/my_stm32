#include "weather.h"
#include "pic.h"

extern int SwitchToGbk(const unsigned char* pszBufIn, int nBufInLen, unsigned char* pszBufOut, int* pnBufOutLen);
/**/
u8 Connect_weather_server(void)
{
	char *p;
	u8 res = 0;
	
	p=mymalloc(40);		/*申请内存*/
	
	/*先消除socket 0*/
	if(Air302_Send_Cmd("AT+HTTPDESTROY = 0","OK",2,200))
	{
	
	}
	/*打包服务器连接指令*/
	sprintf((char*)p,"AT+HTTPCREATE=0,\"http://%s:%s\"",WEATHER_SERVERIP,WEATHER_PORTNUM);
	
	if(Air302_Send_Cmd((char*)p,"+HTTPCREATE: 0",3,200))
	{
		res = 1;
	}
	
	myfree(p);			/*释放内存*/
	
	return res;
}

void weather_Ask(void)
{	
	if(Air302_Send_Cmd("AT+HTTPCON = 0","OK",3,200))
	{
		printf("AT+HTTPSEND=0,0,48,%s\r\n",WEATHER_GETPATH);	
	}
}
//解析当前天气
u8 parse_now_weather(void)
{
	cJSON *jsonData,*jsonItem;	//解析JSON数据使用
	cJSON *jsonUpdate;  		//温度JSON	
	cJSON *jsonTemp;  		  	//温度JSON
	cJSON *jsonHumi;  		  	//湿度JSON
	cJSON *jsonCityName;	  	//城市名JSON
	cJSON *jsonTianq;		  	//天气JSON
	cJSON *jsonTianqN;		  	//天气JSON	
	cJSON *jsonWind;		  	//天气JSON
	cJSON *jsonWindForce;	  	//天气JSON

	char *ShowString;
	char *Readbuf;
	int len;

	u8 tian_offset;
	
	ShowString = mymalloc(100);
	Readbuf = mymalloc(100);

	jsonData = cJSON_Parse(strstr(Usart1ReadBuff,"{\"code"));//获取JSON数据从哪里开始

	if(jsonData)//is JSON
	{	
		jsonItem = cJSON_GetObjectItem(jsonData,"data");//获取 data 字段
		if(jsonItem->type == cJSON_Object)//获取类型
		{
			jsonUpdate = cJSON_GetObjectItem(jsonItem,"lastUpdate");//获取sd字段
			if(jsonUpdate->type == cJSON_String)
			{
				memset(Readbuf,NULL, 100);
				sprintf(Readbuf,"%s",jsonUpdate->valuestring);//获取湿度数据 存到Humidity数组
				GUI_PutHZ(140,208,(u8*)Readbuf,16); //显示湿度
			}
			jsonCityName = cJSON_GetObjectItem(jsonItem,"cityName");//获取气温字段
			if(jsonCityName->type == cJSON_String)
			{					
				memset(ShowString,NULL, 100);
				memset(Readbuf,NULL, 100);
				SwitchToGbk((const u8*)jsonCityName->valuestring,strlen(jsonCityName->valuestring),(u8 *)Readbuf,&len);
				sprintf(ShowString,"%s",Readbuf);
				GUI_PutHZ(84,180, (u8*)ShowString,24);
			}
			
			jsonTianqN = cJSON_GetObjectItem(jsonItem,"numtq");		//获取天气代码
			if(jsonTianqN->type == cJSON_String)
			{			
				if(strstr(jsonTianqN->valuestring,"01"))
				{
					GUI_LoadPic(40,100,(uint8*)bmp01_1,80,80);	
					tian_offset = 56;
				}	
				else if(strstr(jsonTianqN->valuestring,"02"))
				{
					GUI_LoadPic(40,100,(uint8*)bmp02_1,80,80);	
					tian_offset = 68;
				}

			}	
			
			jsonTianq = cJSON_GetObjectItem(jsonItem,"tq");			//获取天气名称
			if(jsonTianq->type == cJSON_String)
			{					
				memset(ShowString,NULL, 100);
				memset(Readbuf,NULL, 100);
				SwitchToGbk((const u8*)jsonTianq->valuestring,strlen(jsonTianq->valuestring),(u8 *)Readbuf,&len);
				sprintf(ShowString,"%s   ",Readbuf);
				GUI_PutHZ((0+tian_offset),63, (u8*)ShowString,24);
			}	
				
			jsonWindForce = cJSON_GetObjectItem(jsonItem,"fl");//获取气温字段
			if(jsonWindForce->type == cJSON_String)
			{					
				memset(ShowString,NULL, 100);
				memset(Readbuf,NULL, 100);
				SwitchToGbk((const u8*)jsonWindForce->valuestring,strlen(jsonWindForce->valuestring),(u8 *)Readbuf,&len);
				sprintf(ShowString,"%s   ",Readbuf);
				GUI_PutHZ(225,135, (u8*)ShowString,24);
			}					
			jsonWind = cJSON_GetObjectItem(jsonItem,"fx");//获取气温字段
			if(jsonWind->type == cJSON_String)
			{					
				memset(ShowString,NULL, 100);
				memset(Readbuf,NULL, 100);
				SwitchToGbk((const u8*)jsonWind->valuestring,strlen(jsonWind->valuestring),(u8 *)Readbuf,&len);
				sprintf(ShowString,"%s   ",Readbuf);
				GUI_PutHZ(225,170, (u8*)ShowString,24);
			}					
			jsonTemp = cJSON_GetObjectItem(jsonItem,"qw");//获取气温字段
			if(jsonTemp->type == cJSON_String)
			{
				memset(Readbuf,NULL, 100);
				sprintf(Readbuf,"%s%s",jsonTemp->valuestring,"  ℃");//获取湿度数据 存到Humidity数组				
				GUI_PutHZ(233,63,(u8*)Readbuf,24); //显示温度

			}					
			jsonHumi = cJSON_GetObjectItem(jsonItem,"sd");//获取sd字段
			if(jsonHumi->type == cJSON_String)
			{
				memset(Readbuf,NULL, 100);
				sprintf(Readbuf,"%s%s",jsonHumi->valuestring,"  %");//获取湿度数据 存到Humidity数组
				GUI_PutHZ(233,98,(u8*)Readbuf,24); //显示湿度
			}	
		}
	}						
	cJSON_Delete(jsonData);			//释放内存
	myfree(ShowString);
	myfree(Readbuf);
	return 0;
}

