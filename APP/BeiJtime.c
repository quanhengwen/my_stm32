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
	
	p=mymalloc(40);		/*�����ڴ�*/
	
	/*������socket 0*/
	if(Air302_Send_Cmd("AT+HTTPDESTROY = 1","OK",2,200))
	{
	
	}
	/*�������������ָ��*/
	sprintf((char*)p,"AT+HTTPCREATE=0,\"http://%s:%s\"",BEIJTIME_SERVERIP,BEIJTIME_PORTNUM);
	
	if(Air302_Send_Cmd((char*)p,"+HTTPCREATE: 1",3,200))
	{
		res = 1;
	}
	
	myfree(p);			/*�ͷ��ڴ�*/
	
	return res;
}

void BeiJtime_Ask(void)
{	
	if(Air302_Send_Cmd("AT+HTTPCON = 1","OK",3,200))
	{
		printf("AT+HTTPSEND=1,0,48,%s\r\n",BEIJTIME_GETPATH);	
	}
}
//������ǰ����
u8 parse_now_BeiJtime(void)
{
	cJSON *jsonData,*jsonItem;	//����JSON����ʹ��
	cJSON *jsonDateTime;  		//�¶�JSON	
	cJSON *jsonWeek;  		  	//�¶�JSON
	u8 res=0;
	char *ShowString;
	char *Readbuf;
	int len;

	ShowString = mymalloc(100);
	Readbuf = mymalloc(100);

	jsonData = cJSON_Parse(strstr(Usart1ReadBuff,"{\"success"));//��ȡJSON���ݴ����￪ʼ

	if(jsonData)//is JSON
	{	
		jsonItem = cJSON_GetObjectItem(jsonData,"result");//��ȡ data �ֶ�
		if(jsonItem->type == cJSON_Object)//��ȡ����
		{
			jsonDateTime = cJSON_GetObjectItem(jsonItem,"timestamp");//��ȡsd�ֶ�
			if(jsonDateTime->type == cJSON_String)
			{
				Unix_To_Time(atoii(jsonDateTime->valuestring));
			}			
			jsonWeek = cJSON_GetObjectItem(jsonItem,"week_2");//��ȡsd�ֶ�
			if(jsonWeek->type == cJSON_String)
			{
				memset(ShowString,NULL, 100);
				memset(Readbuf,NULL, 100);
				SwitchToGbk((const u8*)jsonWeek->valuestring,strlen(jsonWeek->valuestring),(u8 *)Readbuf,&len);
				sprintf(ShowString,"%s",Readbuf);
				GUI_PutHZ(235,17,(u8*)Readbuf,24); //��ʾʪ��
			}			
		}
		res = 1;
	}						
	cJSON_Delete(jsonData);			//�ͷ��ڴ�
	myfree(ShowString);
	myfree(Readbuf);
	
	return res;
}

/**
* Description:Unixʱ������������ñ���ʱ��
* Parameter:UnixNum  Unixʱ���
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
	
	RTC_EnterConfigMode();		/// ��������	
	RTC_WaitForLastTask();		//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	RTC_Set(stime.w_year+2000,stime.w_month,stime.w_date,stime.hour,stime.min,stime.sec);  //����ʱ��	
	RTC_ExitConfigMode(); 		//�˳�����ģʽ  
			  
}
 /*
 * Description:����ʱ������Unixʱ���
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


// �ַ���ת���� "123242"
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
// ���� base ^n 
int _pow(int base, int n) {
    int result = 1;
    while (n > 0) {
        result*=base;
        n--;
    }
    return result;
}
