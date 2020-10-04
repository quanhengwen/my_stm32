#ifndef __IAR302_H
#define __IAR302_H

#include "main.h"


//设备三元组 根据自己的填写就好
#define ProductKey	 	"xxx"
#define DeviceName	 	"xxx"
#define DeviceSecret	"xxx"
 
//MQTT	发布主题			
#define PubTopic       	"/sys/xxx/xxx/thing/event/property/post"			//ProductKey	DeviceName
//MQTT	订阅主题
#define SubTopic       	"/sys/xxx/xxx/thing/service/property/set"			//ProductKey	DeviceName
 
 
#define CurrentTemperature 		"CurrentTemperature"
#define CurrentHumidity			"CurrentHumidity"
#define TPSet					"TPSet"

void Clear_Buffer( void );			//清空缓存

u8 Air302_Send_Cmd(char *Cmd_Str,char *Ans_Str,u8 Count_i,u16 waittime);
char* air302_check_cmd(char *str);
void AIR302_Init( void );			//初始化 AIR302
char AIR302_Receive_Data( void );	//接收从云端发送过来的数据


void Asso_QMTCFG( char* Product, char* Name, char* Secret );
void Asso_QMTCONN( char* ConnectID );
void Asso_QMTSUB( char* Sub );
void Asso_QMTPUB( char* Pub, char* value );

void MQTT_Init( char* Product, char* Name, char* Secret, char* ConnectID ); // MQTT 连接初始化
void MQTT_Sub( char* Sub );			 //订阅主题
void MQTT_Pub( char* Pub, char* value );//发布主题


typedef struct
{
	u8 CSQ;			//网络信号值
	u8 NetStatus;	//网络状态值
} AIR302;

#endif
