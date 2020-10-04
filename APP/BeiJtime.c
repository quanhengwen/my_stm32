#include "BeiJtime.h"
#include "pic.h"
#include <time.h>
extern u8 Beijing_flag;
 int _pow(int base, int n);
 int atoii(char *s);
 void Unix_To_Time(uint32_t UnixNum);
 
extern int SwitchToGbk(const unsigned char* pszBufIn, int nBufInLen, unsigned char* pszBufOut, int* pnBufOutLen);
/**/
u8 Connect_BeiJtime_server(void)
{
	char *p;
	u8 res = 0;
	
	p=mymalloc(40);		/*申请内存*/
	
	/*先消除socket 0*/
	if(Air302_Send_Cmd("AT+HTTPDESTROY = 1","OK",2,200))
	{
	
	}
	/*打包服务器连接指令*/
	sprintf((char*)p,"AT+HTTPCREATE=0,\"http://%s:%s\"",BEIJTIME_SERVERIP,BEIJTIME_PORTNUM);
	
	if(Air302_Send_Cmd((char*)p,"+HTTPCREATE: 1",3,200))
	{
		res = 1;
	}
	
	myfree(p);			/*释放内存*/
	
	return res;
}

void BeiJtime_Ask(void)
{	
	if(Air302_Send_Cmd("AT+HTTPCON = 1","OK",3,200))
	{
		printf("AT+HTTPSEND=1,0,48,%s\r\n",BEIJTIME_GETPATH);	
	}
}
//解析当前天气
u8 parse_now_BeiJtime(void)
{
	cJSON *jsonData,*jsonItem;	//解析JSON数据使用
	cJSON *jsonDateTime;  		//温度JSON	
	cJSON *jsonWeek;  		  	//温度JSON
	u8 res=0;
	char *ShowString;
	char *Readbuf;
	int len;

	ShowString = mymalloc(100);
	Readbuf = mymalloc(100);

	jsonData = cJSON_Parse(strstr(Usart1ReadBuff,"{\"success"));//获取JSON数据从哪里开始

	if(jsonData)//is JSON
	{	
		jsonItem = cJSON_GetObjectItem(jsonData,"result");//获取 data 字段
		if(jsonItem->type == cJSON_Object)//获取类型
		{
			jsonDateTime = cJSON_GetObjectItem(jsonItem,"timestamp");//获取sd字段
			if(jsonDateTime->type == cJSON_String)
			{
				Unix_To_Time(atoii(jsonDateTime->valuestring));
			}			
			jsonWeek = cJSON_GetObjectItem(jsonItem,"week_2");//获取sd字段
			if(jsonWeek->type == cJSON_String)
			{
				memset(ShowString,NULL, 100);
				memset(Readbuf,NULL, 100);
				SwitchToGbk((const u8*)jsonWeek->valuestring,strlen(jsonWeek->valuestring),(u8 *)Readbuf,&len);
				sprintf(ShowString,"%s",Readbuf);
				GUI_PutHZ(235,17,(u8*)Readbuf,24); //显示湿度
			}			
		}
		res = 1;
	}						
	cJSON_Delete(jsonData);			//释放内存
	myfree(ShowString);
	myfree(Readbuf);
	
	return res;
}

/**
* Description:Unix时间戳解析并设置本地时间
* Parameter:UnixNum  Unix时间戳
* Return: NULL
* Others:
*/
void Unix_To_Time(uint32_t UnixNum)
{
	struct tm *stmU;
	
	_calendar_obj stime;

	time_t Count;
			
	Count = UnixNum;
		  
	stmU = localtime(&Count);
	stime.w_year = stmU->tm_year - 100;
	stime.w_month = stmU->tm_mon + 1;
	stime.w_date = stmU->tm_mday;
	stime.hour = stmU->tm_hour + 8;
	stime.min = stmU->tm_min;
	stime.sec = stmU->tm_sec;
	
	RTC_EnterConfigMode();		/// 允许配置	
	RTC_WaitForLastTask();		//等待最近一次对RTC寄存器的写操作完成
	RTC_Set(stime.w_year+2000,stime.w_month,stime.w_date,stime.hour,stime.min,stime.sec);  //设置时间	
	RTC_ExitConfigMode(); 		//退出配置模式  
			  
}
 /*
 * Description:本地时间生成Unix时间戳
 * Parameter:
 * Return: 
 * Others:
 */
void Time_To_Unix()
{
	char buf[50];
			
	 struct tm stmT;
			 	 
	_calendar_obj stime;
	
	RTC_Get();
		
	stmT.tm_year=stime.w_year+100;
	stmT.tm_mon=stime.w_month-1;  
	stmT.tm_mday=stime.w_date;  
	stmT.tm_hour=stime.hour-8;  
	stmT.tm_min=stime.min;  
	stmT.tm_sec=stime.sec;  
		        
	sprintf(buf,"%u",mktime(&stmT));
//	printf("UTC:%s \r\n",buf);
 }	


// 字符串转整数 "123242"
int atoii(char *s) {
    char *p = s;
    while (*p) { p++; }
    // *p == '\0'
    int sum = 0,t=0;
    p--;
    while (p >= s) {
        sum += ((*p - '0')* _pow(10, t));
        t++;
        p--;
    }
    return sum;
}
// 计算 base ^n 
int _pow(int base, int n) {
    int result = 1;
    while (n > 0) {
        result*=base;
        n--;
    }
    return result;
}
