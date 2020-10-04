#include "sys.h"
#include "usart.h"	
#include "string.h"

extern u8 rec;
char Usart1ReadBuff[Usart1ReadLen]={0};  //接收数据缓存
u8   Usart1ReadFlage=0;					 //串口1接收到一条完整数据
u8   Usart1SendBuff[Usart1SendLen]={0};  //发送数据缓存--环形队列用

/*处理HTTP数据*/
u8  HttpHeadCnt = 0;
u8  HttpHeadOK = 0;//接收到正常的http数据
u8  HttpRevDataOK = 0;//获取了http数据

RingBuff_t ringBuff;//创建几个环形数组

//初始化环形缓冲区
void ringBuff_Init(void)
{
	//初始化环形数组结构体信息
   ringBuff.Head = 0;
   ringBuff.Tail = 0;
   ringBuff.Lenght = 0;
}

//写环形队列
u8 Write_RingBuff(u8 data)
{
  if(ringBuff.Lenght >= RINGBUFF_LEN) //判断是缓冲区是否已满
    {
      return FLASE;
   }
    ringBuff.Ring_Buff[ringBuff.Tail]=data;
    ringBuff.Tail = (ringBuff.Tail+1)%RINGBUFF_LEN;//防止越界非法访问
    ringBuff.Lenght++;
    return TRUE;
}
//读环形队列
u8 Read_RingBuff(u8 *rData)
{
   if(ringBuff.Lenght == 0)//
    {
       return FLASE;
    }
   *rData = ringBuff.Ring_Buff[ringBuff.Head];		//先进先出,从缓冲区头开始出
   ringBuff.Head = (ringBuff.Head+1)%RINGBUFF_LEN;	//防止越界非法访问
   ringBuff.Lenght--;
   return TRUE;
}

/**
* @brief   串口1初始化
* @param   bound  波特率
* @param   None
* @param   None
* @retval  None
* @warning None
* @example 
**/
void uart_init(u32 bound){
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9

	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART1, ENABLE);                    //使能串口1 
}

/**
* @brief  串口发送一个字节
* @param  *c:发送的数据指针  cnt:数据个数
* @param  None
* @param  None
* @retval None
* @example 
**/
void UsartOutChar(unsigned char c)
{
	while((USART1->SR&0x40)==0);						//等待上一次串口数据发送完成  
	USART1->DR = c;
}


void USART1_IRQHandler(void)//串口1中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);	//清除中断标志位		

		TIM_SetCounter(TIM3,0);             		//计数器清空
		if(Usart1ReadFlage == 0)
		TIM_Cmd(TIM3,ENABLE);           		  	//使能定时器3
		Res = 	USART_ReceiveData(USART1);	
		Write_RingBuff(Res);						//读取接收到数据 
			
		//解析http数据-------------------------------Start
		//HTTP/1.1 200 OK
		if(!HttpHeadOK)
		{
			if(Res=='+' && HttpHeadCnt==0)HttpHeadCnt++;
			else if(Res=='H' && HttpHeadCnt==1)HttpHeadCnt++;
			else if(Res=='T' && HttpHeadCnt==2)HttpHeadCnt++;
			else if(Res=='T' && HttpHeadCnt==3)HttpHeadCnt++;
			else if(Res=='P' && HttpHeadCnt==4)HttpHeadCnt++;
			else if(Res=='R' && HttpHeadCnt==5)HttpHeadCnt++;
			else if(Res=='E' && HttpHeadCnt==6)HttpHeadCnt++;
			else if(Res=='S' && HttpHeadCnt==7)HttpHeadCnt++;
			else if(Res=='P' && HttpHeadCnt==8)HttpHeadCnt++;
			else if(Res=='C' && HttpHeadCnt==9)HttpHeadCnt++;
			else if(Res==':' && HttpHeadCnt==10){HttpHeadOK = 1;HttpHeadCnt=0;}  
			else
			{
				HttpHeadCnt=0;
			}
		}

		if(HttpHeadOK == 1)//http数据的head已经过去,后面的是真实数据
		{
			HttpHeadOK=0;
			
			HttpHeadCnt = 0;
			
			ringBuff_Init();
			
			memset(Usart1ReadBuff,NULL, sizeof(Usart1ReadBuff));	//清零,把自己的串口接收数组放在这里清零
			
		}	
		
	} 
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)//接收数据溢出
	{
		USART_ClearFlag(USART1,USART_FLAG_ORE);		//读SR
		USART_ReceiveData(USART1);					//读DR
	}
} 
/* 串口三的发送数据函数，并将发送的数据打印到串口一 */
void Uart1_SendStr( char* SendBuff )
{
	while(*SendBuff!='\0')									//通过检测是否字符串末尾
	{
		UsartOutChar(*SendBuff);
		SendBuff++;
	}
}
/**
* @brief  使用microLib的方法,使用printf
* @warningg 勾选 microLib
* @param  None
* @param  None
* @param  None
* @retval None
* @example 
**/

int fputc(int ch, FILE *f)
{
	UsartOutChar((uint8_t) ch);
  return ch;
}
