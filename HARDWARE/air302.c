#include "air302.h"


char     *Receive_Data;		//�жϴӰ����ƽ��յ��������Ƿ���Ч
 
AIR302 AIR302_Status;

static u8 ResetModule(void);
static u8 CheckModule(void);
static u8 SetCFUN(u8 mode);
static void CheckCIMI(void);
static void ActivateNetwork(void);
static void CheckNetwork(void);
static void CheckCSQ(void);



//AIR302���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//����,�ڴ�Ӧ������λ��(str��λ��)
char* air302_check_cmd(char *str)
{
	char *strx=0;
	u8 i=0;
	u8 data;

	if(Usart1ReadFlage == 1)		//���յ�һ��������
	{ 
			for(i=0;i<RINGBUFF_LEN;i++)
			{
				if(Read_RingBuff(&data))
				{
					Usart1ReadBuff[i]=data;
				}
				else
				{
					break;
				}
			}
		strx=strstr((const char*)Usart1ReadBuff,(const char*)str);
	} 
	return (char*)strx;
}
//*********************************************
//����˵����
//					 ��װ��ATָ���·���ظ��ԱȺ���
//����˵����
//			CMD_Str����Ҫ���͵������ַ���
//			Ans_Str����Ҫ�Ա�ģ��ظ����ַ���
//			Count_i���ط�����
//			 time_i����ʱʱ��
//����˵����
//				  1��ִ�в�������֤�ɹ�
//				  0��ʧ��
//*********************************************
u8 Air302_Send_Cmd(char *Cmd_Str,char *Ans_Str,u8 Count_i,u16 waittime)
{
	char *RecData;
	unsigned char i;

	Usart1ReadFlage = 0;
	
	for(i = 0;i < Count_i;i++)										//�����ط�����ѭ��
	{
		
		ringBuff_Init();
		
		memset(Usart1ReadBuff,NULL, sizeof(Usart1ReadBuff));	//����,���Լ��Ĵ��ڽ������������������

		printf("%s\r\n",Cmd_Str);									//���ڷ�������

		if(Ans_Str&&waittime)			//��Ҫ�ȴ�Ӧ��
		{
			while(--waittime)			//�ȴ�����ʱ
			{ 
				delay_ms(10);
				if(Usart1ReadFlage == 1)//���յ��ڴ���Ӧ����
				{
					if(air302_check_cmd(Ans_Str))
					return 1;
				}
			}
			if(waittime==0)
			{
				return 0;
			}			
		}		
	}
	return 0;													//���ʧ�ܺ��˳�����0
}
/**
 * ���ܣ���λair302ģ��
 * ������None
 * ����ֵ��
 *        0 ������
 *      ������ �쳣
 */
static u8 ResetModule(void) {
 
Cheak_Power:

	AIR302_RSR_H;	  //����AIR302ģ�鸴λ���š�Ӳ���ػ��ܽţ��͵�ƽ��Ч���ѹܽ�����200ms����ģ��ػ�
	delay_ms(300);
	AIR302_RSR_L;
	delay_ms(300);
	if(Air302_Send_Cmd("AT","OK",3,100))			//����AT���Ƿ�ظ��ж�
	{
		GUI_PutHZ(14,40, "���Գɹ�       ",24);	
	}else
	{	
		goto Cheak_Power;
	}
	if(Air302_Send_Cmd("ATE0","OK",3,100))			//����AT���Ƿ�ظ��ж�
	{
		GUI_PutHZ(14,40, "�رջ���       ",24);	
	}else
	{
		return 1;
	}
	return 0;
}
/**
 * ���ܣ����air302ģ���Ƿ�����
 * ������None
 * ����ֵ��
 *        0 ������
 *      ������ �쳣
 */
static u8 CheckModule(void)
{
	if(Air302_Send_Cmd("AT","OK",3,100))			//����AT���Ƿ�ظ��ж�
	{
		GUI_PutHZ(14,40, "���Գɹ�       ",24);	
	}else
	{	
		return 1;
	}
	return 0;
}
/**
 * ���ܣ�����AIR302��CFUN���ܣ�Ĭ�ϴ�ȫ���ܣ�
 * ������mode  0 ˯��ģʽ    1ȫ������   2����ģʽ
 * ����ֵ��
 *        0 ������
 *      ������ �쳣
 */
static u8 SetCFUN(u8 mode) {
	
	u8 res = 0;
	switch(mode)
	{
		case 0:  /*˯��ģʽ*/
		if(Air302_Send_Cmd("AT+CFUN=0","OK",3,20))			//����AT���Ƿ�ظ��ж�
		{
			GUI_PutHZ(14,40, "���Գɹ�       ",24);	
		}else
		{	
			res = 1;
		}
		break;
		case 1:  /*ȫ������*/
		if(Air302_Send_Cmd("AT+CFUN=1","OK",3,20))			//����AT���Ƿ�ظ��ж�
		{
			GUI_PutHZ(14,40, "���Գɹ�       ",24);	
		}else
		{	
			res = 1;
		}
		break;	
		
		case 2:  /*����ģʽ*/
		if(Air302_Send_Cmd("AT+CFUN=4","OK",3,20))			//����AT���Ƿ�ظ��ж�
		{
			GUI_PutHZ(14,40, "���Գɹ�       ",24);	
		}else
		{	
			res = 1;
		}
		break;	
		default:
			break;
	
	}
	return res;
}
/**
 * ���ܣ����AIR302��SIM���Ƿ�����
 * ������None
 * ����ֵ��
 *        0 ������
 *      ������ �쳣
 */
static void CheckCIMI(void) {
 
	if(Air302_Send_Cmd("AT+CPIN?\r\n","READY",3,100))			//����AT���Ƿ�ظ��ж�
	{
		GUI_PutHZ(14,40, "SIM������      ",24);	
	}else if(Air302_Send_Cmd("AT+CPIN?\r\n","ERROR",3,100))	
	{	
		GUI_PutHZ(14,40, "û��SIM��      ",24);	
	}
}

static void CheckCSQ(void)
{
	if(Air302_Send_Cmd("AT+CPIN?\r\n","READY",3,100))			//����AT���Ƿ�ظ��ж�
	{
		GUI_PutHZ(14,40, "SIM������      ",24);	
	}else if(Air302_Send_Cmd("AT+CPIN?\r\n","ERROR",3,100))	
	{	
		GUI_PutHZ(14,40, "û��SIM��      ",24);	
	}
}
void Clear_Buffer( void )
{
	memset(Usart1ReadBuff,NULL, sizeof(Usart1ReadBuff));//����,���Լ��Ĵ��ڽ������������������
//	Usart1ReadCnt = 0;
}

void AIR302_Init( void )
{
	u8 buf_csq[3];
	u8 buf_dis[30];	
	u8 csq_value=0;	
	
Cheak_Power:

//	AIR302_RSR_H;	  //����AIR302ģ�鸴λ���š�Ӳ���ػ��ܽţ��͵�ƽ��Ч���ѹܽ�����200ms����ģ��ػ�
//	delay_ms(500);
//	AIR302_RSR_L;
//	delay_ms(500);
	if(Air302_Send_Cmd("AT","OK",3,200))			//����AT���Ƿ�ظ��ж�
	{
//		GUI_PutHZ(14,40, "���Գɹ�       ",16);	
	}else
	{
//		GUI_PutHZ(14,40, "δ����,���ڿ��� ",16);	
		goto Cheak_Power;
	}

	if(Air302_Send_Cmd("ATE0","OK",3,200))			//����AT���Ƿ�ظ��ж�
	{
//		GUI_PutHZ(14,40, "�رջ���       ",16);	
	}else
	{
	}

	if(Air302_Send_Cmd("AT+CFUN=1","OK",3,200))			//����AT���Ƿ�ظ��ж�
	{
//		GUI_PutHZ(14,40, "�������й���     ",16);	
	}else
	{	
		GUI_PutHZ(14,40, "����ʧ��         ",16);
	}

//	if(Air302_Send_Cmd("AT+CIMI","4600",3,200))			//����AT���Ƿ�ظ��ж�
//	{
////		GUI_PutHZ(14,40, "���Ż�ȡ�ɹ�      ",16);	
//	}else
//	{	
//		GUI_PutHZ(14,40, "���Ż�ȡʧ��      ",16);
//	}
Link_Net:

	if(Air302_Send_Cmd("AT+CGATT?","+CGATT: 1",3,200))			//����AT���Ƿ�ظ��ж�
	{
//		GUI_PutHZ(14,40, "GPRS���ӳɹ�      ",24);	
	}else
	{	
//		GUI_PutHZ(14,40, "GPRS��������      ",24);
		goto Link_Net;
	}	

	if(Air302_Send_Cmd("AT+CESQ","+CESQ",3,100))			//����AT���Ƿ�ظ��ж�
	{
		AIR302_Status.CSQ = (Usart1ReadBuff[7]-0x30) * 10 + (Usart1ReadBuff[8]-0x30 );
		if( ( AIR302_Status.CSQ == 99 ) || ( (Usart1ReadBuff[7]-0x30) == 0 ) )	//˵��û���źŻ��źŹ�С
			{
				while( 1 )
				{
					AIR302_Status.NetStatus = 0;
//					GUI_PutHZ(14,40, "�ź�����ʧ��      ",24);
				}
			}
			else
			{
//				GUI_PutHZ(14,40, "�ź���������      ",24);
				AIR302_Status.NetStatus = 1;
			}
	}else
	{	
//				GUI_PutHZ(14,40, "�źŲ�ѯʧ��      ",24);
	}
	
	if(Air302_Send_Cmd("AT+CSQ","OK",3,200))			//����AT���Ƿ�ظ��ж�
	{
		GUI_PutHZ(10,170,"              �豸������           ",16);
//		strncpy((char *)buf_csq,(char *)Usart1ReadBuff+8,2);//������������
//		csq_value=char_num((u8 *)buf_csq,2);
//		sprintf(buf_dis,"�ź�ǿ��:%d   ",csq_value);
//		GUI_PutHZ(14,40, (char*)buf_dis,24);	
	}else
	{	
//		GUI_PutHZ(14,40, "�ź�������ȡʧ��   ",24);
	}

	
}


/* �ڸú�����ʹ���� $����Ϊ�˴����㡣�ڰ������ϡ�������Ϣ��ʱ����������ݿ�ͷҪ���� $ */
char BC302_Receive_Data( void )
{
	char* position = NULL;
	
	Receive_Data = strstr( (const char*)Usart1ReadBuff, (const char*)"+QMTRECV" );
	if( Receive_Data )
	{
		position = strchr( Receive_Data, '$' );
//		printf( "+++���յ�������Ϊ��%s\r\n", position );
	}
	
	return *(position+1);		//���� $ ����ĵ�һ���ַ�
}
	
	
void Asso_QMTCFG( char* Product, char* Name, char* Secret )
{
	char temp[200];

	memset( temp, 0, sizeof( temp ) );	//��� temp���������ش���
	
	strcat( temp, "AT+QMTCFG=\"aliauth\",0,\"" );	//AT+MQTCFG="aliauth",0,"
	strcat( temp, Product );		//AT+MQTCFG="aliauth",0,"ProductKey
	strcat( temp, "\",\"" );		//AT+MQTCFG="aliauth",0,"ProductKey","
	strcat( temp, Name ); 			//AT+MQTCFG="aliauth",0,"ProductKey","DeviceName
	strcat( temp, "\",\"" );		//AT+MQTCFG="aliauth",0,"ProductKey","DeviceName","
	strcat( temp, Secret );			//AT+MQTCFG="aliauth",0,"ProductKey","DeviceName","DeviceSecret
	strcat( temp, "\"\r\n" );		//AT+MQTCFG="aliauth",0,"ProductKey","DeviceName","DeviceSecret"\r\n

	Uart1_SendStr( temp );
}


void Asso_QMTCONN( char* ConnectID )
{
	char temp[200];
	
	memset( temp, 0, sizeof( temp ) );	//��� temp���������ش���
	
	strcat( temp, "AT+QMTCONN=0,\"" );	//AT+QMTCONN=0,"
	strcat( temp, ConnectID );			//AT+QMTCONN=0,"ConnectID
	strcat( temp, "\"\r\n" );			//AT+QMTCONN=0,"ConnectID"\r\n
	
	Uart1_SendStr( temp );
}


void Asso_QMTSUB( char* Sub )
{
	char temp[200];
	
	memset( temp, 0, sizeof( temp ) );	//��� temp���������ش���
	
	strcat( temp, "AT+QMTSUB=0,1,\"" );	//AT+QMTSUB=0,1,"
	strcat( temp, Sub );				//AT+QMTSUB=0,1,"SubTopic
	strcat( temp, "\",0\r\n" );			//AT+QMTSUB=0,1,"SubTopic",0\r\n
	
	Uart1_SendStr( temp );
}


/* �˴���CurrentHumidity �ǰ����ơ����ܶ��塱->����ӹ��ܡ�->��ѡ���ܵı�ʶ���ţ����������ƪ���µĻ�����Ҫ�����Ϊ mlux */
void Asso_QMTPUB( char* Pub, char* value )
{
	char temp[200];
	
	memset( temp, 0, sizeof( temp ) );	//��� temp���������ش���
	
	strcat( temp, "AT+QMTPUB=0,0,0,0,\"" );	//AT+QMTPUB=0,0,0,0,"
	strcat( temp, Pub );				//AT+QMTPUB=0,0,0,0,"PubTopic
	strcat( temp, "\",\"{params:{CurrentHumidity:" ); //AT+QMTPUB=0,0,0,0,"PubTopic","{params:{CurrentHumidity:
	strcat( temp, value );	//AT+QMTPUB=0,0,0,0,"PubTopic","{params:{CurrentHumidity:value
	strcat( temp, "}}\"" );	//AT+QMTPUB=0,0,0,0,"PubTopic","{params:{CurrentHumidity:value}}"
	
	Uart1_SendStr( temp );
}


void MQTT_Init( char* Product, char* Name, char* Secret, char* ConnectID )
{
	Asso_QMTCFG( Product, Name, Secret );
	delay_ms( 300 );

	
	Clear_Buffer();	
	//------------������Ϣ------------
//	printf( "+++��¼���豸��Ϣ\r\n" );
	
	
	Uart1_SendStr( "AT+QMTOPEN=0,\"139.196.135.135\",1883\r\n" );	//ͨ�� tcp ��ʽ���Ӱ�����
	delay_ms( 300 );
	Receive_Data = strstr( (const char*)Usart1ReadBuff, (const char*)"+QMTOPEN; 0,0" );	//�鿴����״̬

	
	/* ����ȵ��ɹ����ϰ����� */
	while( Receive_Data == NULL )
	{
		Clear_Buffer();
		Uart1_SendStr( "AT+QMTOPEN=0,\"139.196.135.135\",1883\r\n" );	//ͨ�� tcp ��ʽ���Ӱ�����
		delay_ms( 300 );
		Receive_Data = strstr( (const char*)Usart1ReadBuff, (const char*)"+QMTOPEN; 0,0" );	//�鿴����״̬
	}
	
	
	Clear_Buffer();	
	//------------������Ϣ------------
//	printf( "+++�ѳɹ����Ӱ�����\r\n" );
	

	delay_ms( 300 );
	Receive_Data = strstr( (const char*)Usart1ReadBuff, (const char*)"+QMTCONN: 0,0,0" );
	
	
	/* ����ȵ��ɹ�ƥ�䵽 MQTT */
	while( Receive_Data == NULL )
	{
		Clear_Buffer();
		Asso_QMTCONN( ConnectID );
		delay_ms( 300 );
		Receive_Data = strstr( (const char*)Usart1ReadBuff, (const char*)"+QMTCONN; 0,0,0" );	//�鿴����״̬	
	}
	
	
	Clear_Buffer();
	//------------������Ϣ------------
//	printf( "+++�ѳɹ��ԽӰ������ϵ��豸\r\n" );
}


void MQTT_Sub( char* Sub )
{
	Clear_Buffer();
	Asso_QMTSUB( Sub );
	delay_ms( 300 );
	Receive_Data = strstr( (const char*)Usart1ReadBuff, (const char*)"+QMTSUB: 0,1,0,0" );
	
	while( Receive_Data == NULL )
	{
		Receive_Data = strstr( (const char*)Usart1ReadBuff, (const char*)"+QMTSUB: 0,1,0,0" );
	}
	
	Clear_Buffer();
	//------------������Ϣ------------
//	printf( "+++�ѳɹ���������\r\n" );
}


void MQTT_Pub( char* Pub, char* value )
{
	Clear_Buffer();
	Asso_QMTPUB( Pub, value );
	delay_ms( 300 );
	Receive_Data = strstr( (const char*)Usart1ReadBuff, (const char*)"+QMTPUB: 0,0,0" );
	
	while( Receive_Data == NULL )
	{
		Receive_Data = strstr( (const char*)Usart1ReadBuff, (const char*)"+QMTPUB: 0,0,0" );
	}
	
	Clear_Buffer();
	//------------������Ϣ------------
//	printf( "+++�ѳɹ�������Ϣ\r\n" );
}



