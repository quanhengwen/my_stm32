#include "sys.h"
#include "usart.h"	
#include "string.h"

extern u8 rec;
char Usart1ReadBuff[Usart1ReadLen]={0};  //�������ݻ���
u8   Usart1ReadFlage=0;					 //����1���յ�һ����������
u8   Usart1SendBuff[Usart1SendLen]={0};  //�������ݻ���--���ζ�����

/*����HTTP����*/
u8  HttpHeadCnt = 0;
u8  HttpHeadOK = 0;//���յ�������http����
u8  HttpRevDataOK = 0;//��ȡ��http����

RingBuff_t ringBuff;//����������������

//��ʼ�����λ�����
void ringBuff_Init(void)
{
	//��ʼ����������ṹ����Ϣ
   ringBuff.Head = 0;
   ringBuff.Tail = 0;
   ringBuff.Lenght = 0;
}

//д���ζ���
u8 Write_RingBuff(u8 data)
{
  if(ringBuff.Lenght >= RINGBUFF_LEN) //�ж��ǻ������Ƿ�����
    {
      return FLASE;
   }
    ringBuff.Ring_Buff[ringBuff.Tail]=data;
    ringBuff.Tail = (ringBuff.Tail+1)%RINGBUFF_LEN;//��ֹԽ��Ƿ�����
    ringBuff.Lenght++;
    return TRUE;
}
//�����ζ���
u8 Read_RingBuff(u8 *rData)
{
   if(ringBuff.Lenght == 0)//
    {
       return FLASE;
    }
   *rData = ringBuff.Ring_Buff[ringBuff.Head];		//�Ƚ��ȳ�,�ӻ�����ͷ��ʼ��
   ringBuff.Head = (ringBuff.Head+1)%RINGBUFF_LEN;	//��ֹԽ��Ƿ�����
   ringBuff.Lenght--;
   return TRUE;
}

/**
* @brief   ����1��ʼ��
* @param   bound  ������
* @param   None
* @param   None
* @retval  None
* @warning None
* @example 
**/
void uart_init(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9

	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
}

/**
* @brief  ���ڷ���һ���ֽ�
* @param  *c:���͵�����ָ��  cnt:���ݸ���
* @param  None
* @param  None
* @retval None
* @example 
**/
void UsartOutChar(unsigned char c)
{
	while((USART1->SR&0x40)==0);						//�ȴ���һ�δ������ݷ������  
	USART1->DR = c;
}


void USART1_IRQHandler(void)//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);	//����жϱ�־λ		

		TIM_SetCounter(TIM3,0);             		//���������
		if(Usart1ReadFlage == 0)
		TIM_Cmd(TIM3,ENABLE);           		  	//ʹ�ܶ�ʱ��3
		Res = 	USART_ReceiveData(USART1);	
		Write_RingBuff(Res);						//��ȡ���յ����� 
			
		//����http����-------------------------------Start
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

		if(HttpHeadOK == 1)//http���ݵ�head�Ѿ���ȥ,���������ʵ����
		{
			HttpHeadOK=0;
			
			HttpHeadCnt = 0;
			
			ringBuff_Init();
			
			memset(Usart1ReadBuff,NULL, sizeof(Usart1ReadBuff));	//����,���Լ��Ĵ��ڽ������������������
			
		}	
		
	} 
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)//�����������
	{
		USART_ClearFlag(USART1,USART_FLAG_ORE);		//��SR
		USART_ReceiveData(USART1);					//��DR
	}
} 
/* �������ķ������ݺ������������͵����ݴ�ӡ������һ */
void Uart1_SendStr( char* SendBuff )
{
	while(*SendBuff!='\0')									//ͨ������Ƿ��ַ���ĩβ
	{
		UsartOutChar(*SendBuff);
		SendBuff++;
	}
}
/**
* @brief  ʹ��microLib�ķ���,ʹ��printf
* @warningg ��ѡ microLib
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
