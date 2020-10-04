#include "air302.h"


char     *Receive_Data;		//判断从阿里云接收到的数据是否有效
 
AIR302 AIR302_Status;

static u8 ResetModule(void);
static u8 CheckModule(void);
static u8 SetCFUN(u8 mode);
static void CheckCIMI(void);
static void ActivateNetwork(void);
static void CheckNetwork(void);
static void CheckCSQ(void);



//AIR302发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//其他,期待应答结果的位置(str的位置)
char* air302_check_cmd(char *str)
{
	char *strx=0;
	u8 i=0;
	u8 data;

	if(Usart1ReadFlage == 1)		//接收到一次数据了
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
//函数说明：
//					 封装的AT指令下发与回复对比函数
//参数说明：
//			CMD_Str：将要发送的命令字符串
//			Ans_Str：将要对比模组回复的字符串
//			Count_i：重发次数
//			 time_i：超时时间
//返回说明：
//				  1：执行并接收验证成功
//				  0：失败
//*********************************************
u8 Air302_Send_Cmd(char *Cmd_Str,char *Ans_Str,u8 Count_i,u16 waittime)
{
	char *RecData;
	unsigned char i;

	Usart1ReadFlage = 0;
	
	for(i = 0;i < Count_i;i++)										//根据重发次数循环
	{
		
		ringBuff_Init();
		
		memset(Usart1ReadBuff,NULL, sizeof(Usart1ReadBuff));	//清零,把自己的串口接收数组放在这里清零

		printf("%s\r\n",Cmd_Str);									//串口发送命令

		if(Ans_Str&&waittime)			//需要等待应答
		{
			while(--waittime)			//等待倒计时
			{ 
				delay_ms(10);
				if(Usart1ReadFlage == 1)//接收到期待的应答结果
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
	return 0;													//多次失败后退出返回0
}
/**
 * 功能：复位air302模组
 * 参数：None
 * 返回值：
 *        0 ：正常
 *      其他： 异常
 */
static u8 ResetModule(void) {
 
Cheak_Power:

	AIR302_RSR_H;	  //拉低AIR302模块复位引脚。硬件关机管脚，低电平有效。把管脚拉低200ms以上模块关机
	delay_ms(300);
	AIR302_RSR_L;
	delay_ms(300);
	if(Air302_Send_Cmd("AT","OK",3,100))			//发送AT看是否回复判断
	{
		GUI_PutHZ(14,40, "测试成功       ",24);	
	}else
	{	
		goto Cheak_Power;
	}
	if(Air302_Send_Cmd("ATE0","OK",3,100))			//发送AT看是否回复判断
	{
		GUI_PutHZ(14,40, "关闭回显       ",24);	
	}else
	{
		return 1;
	}
	return 0;
}
/**
 * 功能：检查air302模组是否正常
 * 参数：None
 * 返回值：
 *        0 ：正常
 *      其他： 异常
 */
static u8 CheckModule(void)
{
	if(Air302_Send_Cmd("AT","OK",3,100))			//发送AT看是否回复判断
	{
		GUI_PutHZ(14,40, "测试成功       ",24);	
	}else
	{	
		return 1;
	}
	return 0;
}
/**
 * 功能：设置AIR302的CFUN功能（默认打开全功能）
 * 参数：mode  0 睡眠模式    1全部功能   2飞行模式
 * 返回值：
 *        0 ：正常
 *      其他： 异常
 */
static u8 SetCFUN(u8 mode) {
	
	u8 res = 0;
	switch(mode)
	{
		case 0:  /*睡眠模式*/
		if(Air302_Send_Cmd("AT+CFUN=0","OK",3,20))			//发送AT看是否回复判断
		{
			GUI_PutHZ(14,40, "测试成功       ",24);	
		}else
		{	
			res = 1;
		}
		break;
		case 1:  /*全部功能*/
		if(Air302_Send_Cmd("AT+CFUN=1","OK",3,20))			//发送AT看是否回复判断
		{
			GUI_PutHZ(14,40, "测试成功       ",24);	
		}else
		{	
			res = 1;
		}
		break;	
		
		case 2:  /*飞行模式*/
		if(Air302_Send_Cmd("AT+CFUN=4","OK",3,20))			//发送AT看是否回复判断
		{
			GUI_PutHZ(14,40, "测试成功       ",24);	
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
 * 功能：检查AIR302的SIM卡是否正常
 * 参数：None
 * 返回值：
 *        0 ：正常
 *      其他： 异常
 */
static void CheckCIMI(void) {
 
	if(Air302_Send_Cmd("AT+CPIN?\r\n","READY",3,100))			//发送AT看是否回复判断
	{
		GUI_PutHZ(14,40, "SIM卡正常      ",24);	
	}else if(Air302_Send_Cmd("AT+CPIN?\r\n","ERROR",3,100))	
	{	
		GUI_PutHZ(14,40, "没有SIM卡      ",24);	
	}
}

static void CheckCSQ(void)
{
	if(Air302_Send_Cmd("AT+CPIN?\r\n","READY",3,100))			//发送AT看是否回复判断
	{
		GUI_PutHZ(14,40, "SIM卡正常      ",24);	
	}else if(Air302_Send_Cmd("AT+CPIN?\r\n","ERROR",3,100))	
	{	
		GUI_PutHZ(14,40, "没有SIM卡      ",24);	
	}
}
void Clear_Buffer( void )
{
	memset(Usart1ReadBuff,NULL, sizeof(Usart1ReadBuff));//清零,把自己的串口接收数组放在这里清零
//	Usart1ReadCnt = 0;
}

void AIR302_Init( void )
{
	u8 buf_csq[3];
	u8 buf_dis[30];	
	u8 csq_value=0;	
	
Cheak_Power:

//	AIR302_RSR_H;	  //拉低AIR302模块复位引脚。硬件关机管脚，低电平有效。把管脚拉低200ms以上模块关机
//	delay_ms(500);
//	AIR302_RSR_L;
//	delay_ms(500);
	if(Air302_Send_Cmd("AT","OK",3,200))			//发送AT看是否回复判断
	{
//		GUI_PutHZ(14,40, "测试成功       ",16);	
	}else
	{
//		GUI_PutHZ(14,40, "未开机,正在开机 ",16);	
		goto Cheak_Power;
	}

	if(Air302_Send_Cmd("ATE0","OK",3,200))			//发送AT看是否回复判断
	{
//		GUI_PutHZ(14,40, "关闭回显       ",16);	
	}else
	{
	}

	if(Air302_Send_Cmd("AT+CFUN=1","OK",3,200))			//发送AT看是否回复判断
	{
//		GUI_PutHZ(14,40, "开启所有功能     ",16);	
	}else
	{	
		GUI_PutHZ(14,40, "开启失败         ",16);
	}

//	if(Air302_Send_Cmd("AT+CIMI","4600",3,200))			//发送AT看是否回复判断
//	{
////		GUI_PutHZ(14,40, "卡号获取成功      ",16);	
//	}else
//	{	
//		GUI_PutHZ(14,40, "卡号获取失败      ",16);
//	}
Link_Net:

	if(Air302_Send_Cmd("AT+CGATT?","+CGATT: 1",3,200))			//发送AT看是否回复判断
	{
//		GUI_PutHZ(14,40, "GPRS连接成功      ",24);	
	}else
	{	
//		GUI_PutHZ(14,40, "GPRS正在连接      ",24);
		goto Link_Net;
	}	

	if(Air302_Send_Cmd("AT+CESQ","+CESQ",3,100))			//发送AT看是否回复判断
	{
		AIR302_Status.CSQ = (Usart1ReadBuff[7]-0x30) * 10 + (Usart1ReadBuff[8]-0x30 );
		if( ( AIR302_Status.CSQ == 99 ) || ( (Usart1ReadBuff[7]-0x30) == 0 ) )	//说明没有信号或信号过小
			{
				while( 1 )
				{
					AIR302_Status.NetStatus = 0;
//					GUI_PutHZ(14,40, "信号搜索失败      ",24);
				}
			}
			else
			{
//				GUI_PutHZ(14,40, "信号搜索正常      ",24);
				AIR302_Status.NetStatus = 1;
			}
	}else
	{	
//				GUI_PutHZ(14,40, "信号查询失败      ",24);
	}
	
	if(Air302_Send_Cmd("AT+CSQ","OK",3,200))			//发送AT看是否回复判断
	{
		GUI_PutHZ(10,170,"              设备已联网           ",16);
//		strncpy((char *)buf_csq,(char *)Usart1ReadBuff+8,2);//拷贝两个数据
//		csq_value=char_num((u8 *)buf_csq,2);
//		sprintf(buf_dis,"信号强度:%d   ",csq_value);
//		GUI_PutHZ(14,40, (char*)buf_dis,24);	
	}else
	{	
//		GUI_PutHZ(14,40, "信号质量获取失败   ",24);
	}

	
}


/* 在该函数中使用了 $，是为了处理方便。在阿里云上“发布消息”时，输入的数据开头要带上 $ */
char BC302_Receive_Data( void )
{
	char* position = NULL;
	
	Receive_Data = strstr( (const char*)Usart1ReadBuff, (const char*)"+QMTRECV" );
	if( Receive_Data )
	{
		position = strchr( Receive_Data, '$' );
//		printf( "+++接收到的数据为：%s\r\n", position );
	}
	
	return *(position+1);		//返回 $ 后面的第一个字符
}
	
	
void Asso_QMTCFG( char* Product, char* Name, char* Secret )
{
	char temp[200];

	memset( temp, 0, sizeof( temp ) );	//清空 temp，避免隐藏错误
	
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
	
	memset( temp, 0, sizeof( temp ) );	//清空 temp，避免隐藏错误
	
	strcat( temp, "AT+QMTCONN=0,\"" );	//AT+QMTCONN=0,"
	strcat( temp, ConnectID );			//AT+QMTCONN=0,"ConnectID
	strcat( temp, "\"\r\n" );			//AT+QMTCONN=0,"ConnectID"\r\n
	
	Uart1_SendStr( temp );
}


void Asso_QMTSUB( char* Sub )
{
	char temp[200];
	
	memset( temp, 0, sizeof( temp ) );	//清空 temp，避免隐藏错误
	
	strcat( temp, "AT+QMTSUB=0,1,\"" );	//AT+QMTSUB=0,1,"
	strcat( temp, Sub );				//AT+QMTSUB=0,1,"SubTopic
	strcat( temp, "\",0\r\n" );			//AT+QMTSUB=0,1,"SubTopic",0\r\n
	
	Uart1_SendStr( temp );
}


/* 此处的CurrentHumidity 是阿里云“功能定义”->“添加功能”->所选功能的标识符号，如果是以上篇文章的话，需要将其改为 mlux */
void Asso_QMTPUB( char* Pub, char* value )
{
	char temp[200];
	
	memset( temp, 0, sizeof( temp ) );	//清空 temp，避免隐藏错误
	
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
	//------------检验信息------------
//	printf( "+++已录入设备信息\r\n" );
	
	
	Uart1_SendStr( "AT+QMTOPEN=0,\"139.196.135.135\",1883\r\n" );	//通过 tcp 方式连接阿里云
	delay_ms( 300 );
	Receive_Data = strstr( (const char*)Usart1ReadBuff, (const char*)"+QMTOPEN; 0,0" );	//查看返回状态

	
	/* 必须等到成功连上阿里云 */
	while( Receive_Data == NULL )
	{
		Clear_Buffer();
		Uart1_SendStr( "AT+QMTOPEN=0,\"139.196.135.135\",1883\r\n" );	//通过 tcp 方式连接阿里云
		delay_ms( 300 );
		Receive_Data = strstr( (const char*)Usart1ReadBuff, (const char*)"+QMTOPEN; 0,0" );	//查看返回状态
	}
	
	
	Clear_Buffer();	
	//------------检验信息------------
//	printf( "+++已成功连接阿里云\r\n" );
	

	delay_ms( 300 );
	Receive_Data = strstr( (const char*)Usart1ReadBuff, (const char*)"+QMTCONN: 0,0,0" );
	
	
	/* 必须等到成功匹配到 MQTT */
	while( Receive_Data == NULL )
	{
		Clear_Buffer();
		Asso_QMTCONN( ConnectID );
		delay_ms( 300 );
		Receive_Data = strstr( (const char*)Usart1ReadBuff, (const char*)"+QMTCONN; 0,0,0" );	//查看返回状态	
	}
	
	
	Clear_Buffer();
	//------------检验信息------------
//	printf( "+++已成功对接阿里云上的设备\r\n" );
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
	//------------检验信息------------
//	printf( "+++已成功订阅主题\r\n" );
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
	//------------检验信息------------
//	printf( "+++已成功发送信息\r\n" );
}



