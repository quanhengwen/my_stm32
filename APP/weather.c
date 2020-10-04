#include "weather.h"
#include "pic.h"

extern int SwitchToGbk(const unsigned char* pszBufIn, int nBufInLen, unsigned char* pszBufOut, int* pnBufOutLen);
/**/
u8 Connect_weather_server(void)
{
	char *p;
	u8 res = 0;
	
	p=mymalloc(40);		/*�����ڴ�*/
	
	/*������socket 0*/
	if(Air302_Send_Cmd("AT+HTTPDESTROY = 0","OK",2,200))
	{
	
	}
	/*�������������ָ��*/
	sprintf((char*)p,"AT+HTTPCREATE=0,\"http://%s:%s\"",WEATHER_SERVERIP,WEATHER_PORTNUM);
	
	if(Air302_Send_Cmd((char*)p,"+HTTPCREATE: 0",3,200))
	{
		res = 1;
	}
	
	myfree(p);			/*�ͷ��ڴ�*/
	
	return res;
}

void weather_Ask(void)
{	
	if(Air302_Send_Cmd("AT+HTTPCON = 0","OK",3,200))
	{
		printf("AT+HTTPSEND=0,0,48,%s\r\n",WEATHER_GETPATH);	
	}
}
//������ǰ����
u8 parse_now_weather(void)
{
	cJSON *jsonData,*jsonItem;	//����JSON����ʹ��
	cJSON *jsonUpdate;  		//�¶�JSON	
	cJSON *jsonTemp;  		  	//�¶�JSON
	cJSON *jsonHumi;  		  	//ʪ��JSON
	cJSON *jsonCityName;	  	//������JSON
	cJSON *jsonTianq;		  	//����JSON
	cJSON *jsonTianqN;		  	//����JSON	
	cJSON *jsonWind;		  	//����JSON
	cJSON *jsonWindForce;	  	//����JSON

	char *ShowString;
	char *Readbuf;
	int len;

	u8 tian_offset;
	
	ShowString = mymalloc(100);
	Readbuf = mymalloc(100);

	jsonData = cJSON_Parse(strstr(Usart1ReadBuff,"{\"code"));//��ȡJSON���ݴ����￪ʼ

	if(jsonData)//is JSON
	{	
		jsonItem = cJSON_GetObjectItem(jsonData,"data");//��ȡ data �ֶ�
		if(jsonItem->type == cJSON_Object)//��ȡ����
		{
			jsonUpdate = cJSON_GetObjectItem(jsonItem,"lastUpdate");//��ȡsd�ֶ�
			if(jsonUpdate->type == cJSON_String)
			{
				memset(Readbuf,NULL, 100);
				sprintf(Readbuf,"%s",jsonUpdate->valuestring);//��ȡʪ������ �浽Humidity����
				GUI_PutHZ(140,208,(u8*)Readbuf,16); //��ʾʪ��
			}
			jsonCityName = cJSON_GetObjectItem(jsonItem,"cityName");//��ȡ�����ֶ�
			if(jsonCityName->type == cJSON_String)
			{					
				memset(ShowString,NULL, 100);
				memset(Readbuf,NULL, 100);
				SwitchToGbk((const u8*)jsonCityName->valuestring,strlen(jsonCityName->valuestring),(u8 *)Readbuf,&len);
				sprintf(ShowString,"%s",Readbuf);
				GUI_PutHZ(84,180, (u8*)ShowString,24);
			}
			
			jsonTianqN = cJSON_GetObjectItem(jsonItem,"numtq");		//��ȡ��������
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
			
			jsonTianq = cJSON_GetObjectItem(jsonItem,"tq");			//��ȡ��������
			if(jsonTianq->type == cJSON_String)
			{					
				memset(ShowString,NULL, 100);
				memset(Readbuf,NULL, 100);
				SwitchToGbk((const u8*)jsonTianq->valuestring,strlen(jsonTianq->valuestring),(u8 *)Readbuf,&len);
				sprintf(ShowString,"%s   ",Readbuf);
				GUI_PutHZ((0+tian_offset),63, (u8*)ShowString,24);
			}	
				
			jsonWindForce = cJSON_GetObjectItem(jsonItem,"fl");//��ȡ�����ֶ�
			if(jsonWindForce->type == cJSON_String)
			{					
				memset(ShowString,NULL, 100);
				memset(Readbuf,NULL, 100);
				SwitchToGbk((const u8*)jsonWindForce->valuestring,strlen(jsonWindForce->valuestring),(u8 *)Readbuf,&len);
				sprintf(ShowString,"%s   ",Readbuf);
				GUI_PutHZ(225,135, (u8*)ShowString,24);
			}					
			jsonWind = cJSON_GetObjectItem(jsonItem,"fx");//��ȡ�����ֶ�
			if(jsonWind->type == cJSON_String)
			{					
				memset(ShowString,NULL, 100);
				memset(Readbuf,NULL, 100);
				SwitchToGbk((const u8*)jsonWind->valuestring,strlen(jsonWind->valuestring),(u8 *)Readbuf,&len);
				sprintf(ShowString,"%s   ",Readbuf);
				GUI_PutHZ(225,170, (u8*)ShowString,24);
			}					
			jsonTemp = cJSON_GetObjectItem(jsonItem,"qw");//��ȡ�����ֶ�
			if(jsonTemp->type == cJSON_String)
			{
				memset(Readbuf,NULL, 100);
				sprintf(Readbuf,"%s%s",jsonTemp->valuestring,"  ��");//��ȡʪ������ �浽Humidity����				
				GUI_PutHZ(233,63,(u8*)Readbuf,24); //��ʾ�¶�

			}					
			jsonHumi = cJSON_GetObjectItem(jsonItem,"sd");//��ȡsd�ֶ�
			if(jsonHumi->type == cJSON_String)
			{
				memset(Readbuf,NULL, 100);
				sprintf(Readbuf,"%s%s",jsonHumi->valuestring,"  %");//��ȡʪ������ �浽Humidity����
				GUI_PutHZ(233,98,(u8*)Readbuf,24); //��ʾʪ��
			}	
		}
	}						
	cJSON_Delete(jsonData);			//�ͷ��ڴ�
	myfree(ShowString);
	myfree(Readbuf);
	return 0;
}

