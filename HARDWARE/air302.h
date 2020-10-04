#ifndef __IAR302_H
#define __IAR302_H

#include "main.h"


//�豸��Ԫ�� �����Լ�����д�ͺ�
#define ProductKey	 	"xxx"
#define DeviceName	 	"xxx"
#define DeviceSecret	"xxx"
 
//MQTT	��������			
#define PubTopic       	"/sys/xxx/xxx/thing/event/property/post"			//ProductKey	DeviceName
//MQTT	��������
#define SubTopic       	"/sys/xxx/xxx/thing/service/property/set"			//ProductKey	DeviceName
 
 
#define CurrentTemperature 		"CurrentTemperature"
#define CurrentHumidity			"CurrentHumidity"
#define TPSet					"TPSet"

void Clear_Buffer( void );			//��ջ���

u8 Air302_Send_Cmd(char *Cmd_Str,char *Ans_Str,u8 Count_i,u16 waittime);
char* air302_check_cmd(char *str);
void AIR302_Init( void );			//��ʼ�� AIR302
char AIR302_Receive_Data( void );	//���մ��ƶ˷��͹���������


void Asso_QMTCFG( char* Product, char* Name, char* Secret );
void Asso_QMTCONN( char* ConnectID );
void Asso_QMTSUB( char* Sub );
void Asso_QMTPUB( char* Pub, char* value );

void MQTT_Init( char* Product, char* Name, char* Secret, char* ConnectID ); // MQTT ���ӳ�ʼ��
void MQTT_Sub( char* Sub );			 //��������
void MQTT_Pub( char* Pub, char* value );//��������


typedef struct
{
	u8 CSQ;			//�����ź�ֵ
	u8 NetStatus;	//����״ֵ̬
} AIR302;

#endif
